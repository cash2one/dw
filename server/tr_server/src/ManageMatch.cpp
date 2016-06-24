
#include <boost/bind.hpp>
#include "ManageMatch.h"
#include "Singleton.h"
#include "ManageBattleSession.h"
#include "MessageMaker.h"
#include "ManageVrSession.h"
#include "check_macro.h"

#define GET_TR_PLAYER()	\
	PlayerTrInfo * player = ManagePlayerTrInfo::instance()->getPlayer(input_packet->msg_head.guid);	\
	if (NULL == player)	\
	{	\
		DEF_LOG_ERROR("failed to get player info <%llu>", input_packet->msg_head.guid);		\
		return -1;	\
	}

ManageMatch::ManageMatch()
	: m_match_id_index(0)
{

}

ManageMatch::~ManageMatch()
{

}

ManageMatch * ManageMatch::instance()
{
	return Singleton<ManageMatch>::instance();
}

int ManageMatch::handleMatch(InterPacket * input_packet, const SMsgMatchReq * input_msg)
{
	PlayerTrInfo * player = ManagePlayerTrInfo::instance()->getPlayer(input_packet->msg_head.guid);
	if (NULL == player)
	{
		DEF_LOG_ERROR("failed to get player info while request match <%llu>", input_packet->msg_head.guid);
		return -1;
	}

	if (PTS_matching == player->getState())
	{
		DEF_LOG_ERROR("player request matching, but player is already in matching, cancel matching first, user id is <%llu>", input_packet->msg_head.guid);
		// cancel matching first
		m_manage_player_state.processEvent(player, PTE_cancel_match);
	}

	// todo
	CK_NOT_EQUAL_RETURN(PTS_base, player->getState(), -1);

	MatchInfo & match_info = player->getMatchInfo();
	match_info.team_type = input_msg->team_type();
	match_info.match_type = input_msg->match_type();

	m_manage_player_state.processEvent(player, PTE_request_match);

	//MatchQueue * match_queue = this->getMatchQueue(input_msg->match_type());
	//if (NULL == match_queue)
	//{
	//	return -1;
	//}

	//match_queue->handleMatch(input_packet->msg_head.guid, input_packet->session_id);
	return 0;
}

int ManageMatch::handleMatchCancel(InterPacket * input_packet)
{
	GET_TR_PLAYER();

	m_manage_player_state.processEvent(player, PTE_cancel_match);

	return 0;
}

int ManageMatch::handlePlayerOffline(InterPacket * input_packet)
{
	GET_TR_PLAYER();

	m_manage_player_state.processEvent(player, PTE_offline);

	return 0;
}

void ManageMatch::playerBattleFinish(PlayerTrInfo * player)
{
	m_manage_player_state.processEvent(player, PTE_battle_finish);
}

void ManageMatch::handleSMsgNotifyBattleMatchResultAck(uint64_t match_battle_id, int result)
{
	MatchBattle * match_battle = this->getMatchBattle(match_battle_id);
	CK_NULL_RETURN(match_battle, );

	const MatchCellVec_t & match_cell_vec = match_battle->getMatchCellVec();

	for (auto match_cell : match_cell_vec)
	{
		InterPacket * pack = MessageMaker::makeMsgMatchEnterBattle(match_cell->getUserId(), match_cell->getSessionId(), match_battle->getBattleInfo()->battle_gate_ip, match_battle->getBattleInfo()->battle_gate_port);
		ManageVrSession::instance()->handleOutputPacket(pack);
	}
}

int ManageMatch::init()
{

	m_manage_player_state.registerState(PTS_base, PTE_request_match, PTS_matching, boost::bind(&ManageMatch::stateRequestMatch, this, _1));
	
	m_manage_player_state.registerState(PTS_matching, PTE_match_success, PTS_in_battle, boost::bind(&ManageMatch::stateMatchSuccess, this, _1));
	//m_manage_player_state.registerState(PTS_matching, PTE_request_match, PTS_matching, boost::bind(&ManageMatch::stateMatchingRequestMatch, this, _1));
	m_manage_player_state.registerState(PTS_matching, PTE_offline, PTS_base, boost::bind(&ManageMatch::stateMatchingPlayerOffline, this, _1));

	//m_manage_player_state.registerState(PTS_is_confirming,	PTE_match_confirmed,	PTS_is_confirmed, );
	//m_manage_player_state.registerState(PTS_is_confirmed,	PTE_all_confirmed,		PTS_in_battle, );
	m_manage_player_state.registerState(PTS_in_battle, PTE_battle_finish, PTS_base, boost::bind(&ManageMatch::stateBattleFinish, this, _1));

	m_manage_player_state.registerState(PTS_matching, PTE_cancel_match, PTS_base, boost::bind(&ManageMatch::stateCancelMatch, this, _1));

	//m_manage_player_state.registerState(PTS_is_confirming,	PTE_offline,			PTS_base);
	//m_manage_player_state.registerState(PTS_is_confirmed,	PTE_offline,			PTS_base);
	//m_manage_player_state.registerState(PTS_in_battle,		PTE_offline,			PTS_in_battle);
	//m_manage_player_state.registerState(PTS_base,			PTE_offline,			PTS_base);

	//m_manage_player_state.registerState(PTS_in_battle,		PTE_offline,			PTS_base);

	return 0;
}

int ManageMatch::update()
{
	for (auto match_queue : m_match_queue_map)
	{
		match_queue.second->update();
	}
	return 0;
}

void ManageMatch::addMatchCell(MatchCell * match_cell)
{
	CK_NULL_RETURN(match_cell, );

	auto it = m_match_cell_map.find(match_cell->getUserId());

	if (m_match_cell_map.end() != it)
	{
		// should never happen
		DEF_LOG_ERROR("add reduplication match cell, account is <%llu>", match_cell->getUserId());
		return;
	}

	PlayerTrInfo * player = ManagePlayerTrInfo::instance()->getPlayer(match_cell->getUserId());
	CK_NULL_RETURN(player, );

	player->setMatchCell(match_cell);

	m_match_cell_map.insert(std::make_pair(match_cell->getUserId(), match_cell));
}

void ManageMatch::removeMatchCell(MatchCell * match_cell)
{
	CK_NULL_RETURN(match_cell, );
	auto it = m_match_cell_map.find(match_cell->getUserId());
	if (m_match_cell_map.end() == it)
	{
		// should never happen
		DEF_LOG_ERROR("remove match cell, but did not find it in map, account id is <%llu>", match_cell->getUserId());
		return;
	}

	m_match_cell_map.erase(it);

	PlayerTrInfo * player = ManagePlayerTrInfo::instance()->getPlayer(match_cell->getUserId());
	if (NULL == player)
	{
		DEF_LOG_ERROR("did not get player while remove match cell, account id is <%llu>", match_cell->getUserId());
		return;
	}

	player->setMatchCell(NULL);
}

void ManageMatch::removeMatchCellAndDelete(PlayerTrInfo * player_info)
{
	auto it = m_match_cell_map.find(player_info->getAccountId());
	if (m_match_cell_map.end() == it)
	{
		DEF_LOG_ERROR("do not find match cell while removeMatchCellAndDelete, <%llu>", player_info->getAccountId());
		return ;
	}

	MatchCell * match_cell = it->second;
	delete match_cell;
	m_match_cell_map.erase(it);

	player_info->setMatchCell(NULL);
}

MatchBattle * ManageMatch::matchSuccess(BattleInfo * battle_info, MatchCellVec_t & match_cell_vec)
{
	++m_match_id_index;
	MatchBattle * match_battle = new MatchBattle();
	match_battle->init(m_match_id_index, battle_info, match_cell_vec);

	this->addMatchBattle(match_battle);

	for (auto match_cell : match_cell_vec)
	{
		PlayerTrInfo * player_info = ManagePlayerTrInfo::instance()->getPlayer(match_cell->getUserId());
		CK_NULL_CMD(player_info, continue);

		player_info->setBattleSession(battle_info->session_id);
		
		m_manage_player_state.processEvent(player_info, PTE_match_success);
	}

	return match_battle;
}

bool ManageMatch::stateRequestMatch(PlayerTrInfo * player_info)
{
	CK_FALSE_RETURN(NULL == player_info->getMatchCell(), false);

	MatchQueue * match_queue = this->getMatchQueue(player_info->getMatchInfo().match_type);

	CK_NULL_RETURN(match_queue, false);

	match_queue->handleMatch(player_info->getAccountId(), player_info->getSessionId());

	return true;
}

//bool ManageMatch::stateMatchingRequestMatch(PlayerTrInfo * player_info)
//{
//	DEF_LOG_ERROR("player is in matching while requset match operation, user id is <%llu>", player_info->getAccountId());
//
//	// cancel matching first, the match again
//	return true;
//}


bool ManageMatch::stateMatchSuccess(PlayerTrInfo * player_info)
{
	CK_NULL_RETURN(player_info->getMatchCell(), false);

	return true;
}

bool ManageMatch::stateCancelMatch(PlayerTrInfo * player_info)
{
	if (!this->cancelMatching(player_info))
	{
		DEF_LOG_ERROR("failed to cancel matching while player requests to cancel matching, user id is <%llu>, match status is <%d>", player_info->getAccountId(), player_info->getState());
		return false;
	}

	return true;
}

bool ManageMatch::stateMatchingPlayerOffline(PlayerTrInfo * player_info)
{
	if (!this->cancelMatching(player_info))
	{
		DEF_LOG_ERROR("failed to cancel matching while player offline, user id is <%llu>, match status is <%d>", player_info->getAccountId(), player_info->getState());
		false;
	}

	return true;
}

bool ManageMatch::stateBattleFinish(PlayerTrInfo * player_info)
{
	CK_NULL_RETURN(player_info->getMatchCell(), false);
	CK_NOT_EQUAL_RETURN(player_info->getState(), PTS_in_battle, false);

	MatchQueue * match_queue = this->getMatchQueue(player_info->getMatchInfo().match_type);

	CK_NULL_RETURN(match_queue, false);

	this->removeMatchCellAndDelete(player_info);

	return true;
}

MatchQueue * ManageMatch::getMatchQueue(int match_type)
{
	auto it = m_match_queue_map.find(match_type);
	if (it != m_match_queue_map.end())
	{
		return it->second;
	}

	MatchQueue * match_queue = new MatchQueue();
	match_queue->init(match_type);
	m_match_queue_map[match_type] = match_queue;

	return match_queue;
}

MatchBattle * ManageMatch::getMatchBattle(uint64_t match_battle_id)
{
	auto it = m_match_battle_map.find(match_battle_id);
	if (m_match_battle_map.end() == it)
	{
		DEF_LOG_ERROR("failed to get MatchBattle by id <%llu>", match_battle_id);
		return NULL;
	}

	return it->second;
}

void ManageMatch::addMatchBattle(MatchBattle * match_battle)
{
	auto it = m_match_battle_map.find(match_battle->getMatchId());
	if (it  != m_match_battle_map.end())
	{
		DEF_LOG_ERROR("add reduplication MatchBattle id <%llu>", match_battle->getMatchId());
		return;
	}

	m_match_battle_map.insert(std::make_pair(match_battle->getMatchId(), match_battle));

	DEF_LOG_DEBUG("add MatchBattle id <%llu>, battle id is <%d>, member size is <%d>", match_battle->getMatchId(), match_battle->getBattleInfo()->battle_server_id, match_battle->getMatchCellVec().size());
}

void ManageMatch::removeMatchBattle(uint64_t match_battle_id)
{
	auto it = m_match_battle_map.find(match_battle_id);
	if (m_match_battle_map.end() == it)
	{
		// should never happen
		DEF_LOG_ERROR("remove MatchBattle, but did not find it in map<%llu>", match_battle_id);
		return;
	}

	MatchBattle * match_battle = it->second;

	DEF_LOG_DEBUG("remove MatchBattle, id is <%llu>, battle id is <%d>, member size is <%d>", match_battle_id, match_battle->getBattleInfo()->battle_server_id, match_battle->getMatchCellVec().size());

	delete match_battle;

	m_match_battle_map.erase(it);
}

bool ManageMatch::cancelMatching(PlayerTrInfo * player_info)
{
	DEF_LOG_DEBUG("cancel match, user id is <%llu>, curr state is <%d>", player_info->getAccountId(), player_info->getState());

	CK_NULL_RETURN(player_info->getMatchCell(), false);
	CK_NOT_EQUAL_RETURN(player_info->getState(), PTS_matching, false);

	MatchQueue * match_queue = this->getMatchQueue(player_info->getMatchInfo().match_type);

	CK_NULL_RETURN(match_queue, false);

	// can cancel
	InterPacket * pack = MessageMaker::makeMsgCancelMatchAck(player_info->getSessionId(), player_info->getAccountId(), 1);
	ManageVrSession::instance()->handleOutputPacket(pack);

	match_queue->handleMatchCancel(player_info->getAccountId());

	return true;
}

