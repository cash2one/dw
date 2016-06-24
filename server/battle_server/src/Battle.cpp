
#include "Logger.h"
#include "Battle.h"
#include "BattleManager.h"
#include "BattleLog.h"
#include "check_macro.h"
#include "opcode.h"
#include "ManageTrSession.h"


int ONE_FRAME_TIME = 50;
int reconnect_send_frame_number = 300;

Battle::Battle()
	: m_battle_status(BS_PREPARE)
	, m_battle_start_time(0)
	, m_del_frame_pack_for_press_test(false)
	, m_one_team_number(0)
	, m_random_seed(0)
{
	m_battle_start_time = BattleManager::instance()->getSystemTime();
}

Battle::~Battle()
{
	BATTLE_LOG_INFO("start to delete battle, frame packet size is <%d>", m_frame_packet_info_map.size());

	time_t start_time = getSystemMilliSeconds();

	for (PacketFrameInfoMap_t::iterator it = m_frame_packet_info_map.begin(); it != m_frame_packet_info_map.end(); ++it)
	{
		FramePacketInfo * frame_packet_info = it->second;
		delete frame_packet_info->packet;
		delete frame_packet_info;
	}

	time_t end_time = getSystemMilliSeconds();
	int diff_time = (end_time - start_time);
	if (diff_time > 10 && m_frame_packet_info_map.size() > 0)
	{
		float one_time = float(diff_time) / m_frame_packet_info_map.size();
		BATTLE_LOG_INFO("delete battle frame packet, packet num is <%d>, time is <%d>, one time is <%.4f>", m_frame_packet_info_map.size(), diff_time, one_time);
	}

	for (PlayerMap_t::iterator it = m_player_map.begin(); it != m_player_map.end(); ++it)
	{
		delete it->second;
	}
}

int Battle::init(BattleManager * battle_manager, int one_team_number, bool del_frame_pack_for_press_test)
{
	m_del_frame_pack_for_press_test = del_frame_pack_for_press_test;

	m_random_seed = m_battle_manager->getRandomValue();

	m_battle_timeout_info.battle_create_time = getSystemMilliSeconds();

	m_battle_status = BS_PREPARE;

	m_battle_manager = battle_manager;

	m_one_team_number = one_team_number;

	m_battle_team.init(m_one_team_number);


	return 0;
}

void Battle::update(time_t current_time)
{
	if (BS_START_BATTLE != m_battle_status)
	{
		this->checkBeforeBattleTimeout();
		return;
	}

	m_manage_client_validation.init(m_player_map.size());

	if (!m_manage_client_validation.isCheckSuccess())
	{
		//BATTLE_LOG_INFO("client validation failed");
		return;
	}

	int diff_time = int (current_time - m_battle_frame_info.last_frame_time);
	if (diff_time > ONE_FRAME_TIME * 2)
	{
		// error
		//BATTLE_LOG_ERROR("get bigger diff_time : <%d>", diff_time);
	}

	if (diff_time >= ONE_FRAME_TIME)
	{
		diff_time -= ONE_FRAME_TIME;
		m_battle_frame_info.last_frame_time += ONE_FRAME_TIME;
		m_battle_frame_info.frame_index += 1;
		this->makeCurrentFrame(m_battle_frame_info.frame_index, m_battle_frame_info.last_frame_time);

		if (diff_time >= ONE_FRAME_TIME)
		{
			// error, server is under press
			m_battle_frame_info.last_frame_time += diff_time - diff_time % ONE_FRAME_TIME;
			BATTLE_LOG_ERROR("server is under pressing, extra expire time is <%d>, frame time is <%d>", diff_time, ONE_FRAME_TIME);
		}
		//BATTLE_LOG_DEBUG("make battle farame");
	}

	m_manage_client_validation.update(m_battle_frame_info.frame_index);
}

void Battle::makeCurrentFrame(uint32 frame_index, time_t current_time)
{
	InterPacket * packet = createMsgFrameSyncPack(0);
	this->broadcastPacketFrame(packet);

	if (!m_del_frame_pack_for_press_test)
	{
		m_frame_packet_info_map.insert(std::make_pair(frame_index, new FramePacketInfo(frame_index, packet)));
	}
	else
	{
		delete packet;
	}

	this->clearFrameAction(m_current_frame_action);
	m_current_frame_action.clear();
}

void Battle::broadcastPacket(InterPacket * packet, int team_id, bool all_team)
{
	PlayerMap_t team_player;
	team_player.clear();
	if (all_team)
	{
		team_player = m_player_map;
	}
	else
	{
		team_player = m_battle_team.getTeamById(team_id);
	}

	for (PlayerMap_t::iterator it = team_player.begin(); it != team_player.end(); ++it)
	{
		Player * player = it->second;

		if (PS_ONLINE != player->getReconnectInfo().player_status)
		{
			continue;
		}

		this->updateInterPacketGuid(packet, player->getGateGuid());
		m_battle_manager->sendPlayerMsg(player, packet, false);
	}

	delete packet;
}

void Battle::broadcastPacketFrame(InterPacket * packet)
{
	for (PlayerMap_t::iterator it = m_player_map.begin(); it != m_player_map.end(); ++it)
	{
		Player * player = it->second;

		if (BPS_START_BATTLE != player->getStatus())
		{
			continue;
		}

		this->updateInterPacketGuid(packet, player->getGateGuid());
		m_battle_manager->sendPlayerMsg(player, packet, false);
	}

}

bool Battle::isTimeout(time_t current_time)
{
	bool ret = m_player_map.size() == m_offline_player_set.size();
	return ret;

	//return false;
	//return current_time - m_battle_start_time >= 20 * 60 * 1000 ? true : false;
}

bool Battle::isBattleEnd(time_t current_time)
{
	// to do, timeout or normally end
	return m_battle_result.isBattleEnd();
}

bool Battle::isMemberFull()
{
	if (m_player_map.size() >= (size_t)m_one_team_number * 2)
	{
		m_battle_status = m_battle_status;
		return true;
	}

	return false;
}

void Battle::playerReconnect(Player * player, session_t session_id)
{
	m_offline_player_set.erase(player);

	this->notifyClientJoinGame(player, player->getId(), player->getTeamId(), player->getGateGuid());

	if (player->getBattle()->isBattleCreate())
	{
		InterPacket * packet = this->createMsgLobbyGameStartAck(player->getGateGuid());
		m_battle_manager->sendPlayerMsg(player, packet, true);

		player->playerClientLoading();

		if (PS_OFFLINE == player->getReconnectInfo().player_status)
		{
			this->playerLoginWithLogout(player);
		}
		else
		{
			this->playerLoginWithoutLogout(player);
		}
	}
	else
	{

	}
}

void Battle::playerOffline(Player * player)
{
	m_offline_player_set.insert(player);

	player->getReconnectInfo().player_status = PS_OFFLINE;
}

void Battle::playerEnterBattle(Player * player)
{
	player->enterBattle();

	InterPacket * pack = MessageMaker::makeMsgEnterBattleAck(NULL, player->getGateGuid(), this->m_battle_team.getTeamById(player->getTeamId()));
	m_battle_manager->sendPlayerMsg(player, pack, true);

	pack = MessageMaker::makeMsgBeforeBattleParamNty(player->getGateGuid(), m_battle_manager->getBattleSetting());
	m_battle_manager->sendPlayerMsg(player, pack, true);

	pack = MessageMaker::makeMsgBattleParamNty(player->getGateGuid(), m_battle_manager->getBattleSetting(), m_random_seed);
	m_battle_manager->sendPlayerMsg(player, pack, true);
}

void Battle::playerReEnterBattle(Player * player)
{
	player->enterBattle();

	// todo
	InterPacket * pack = MessageMaker::makeMsgReEnterBattleAck(NULL, player->getGateGuid(), 1);
	m_battle_manager->sendPlayerMsg(player, pack, true);

	pack = MessageMaker::makeMsgBeforeBattleParamNty(player->getGateGuid(), m_battle_manager->getBattleSetting());
	m_battle_manager->sendPlayerMsg(player, pack, true);

	pack = MessageMaker::makeMsgBattleParamNty(player->getGateGuid(), m_battle_manager->getBattleSetting(), m_random_seed);
	m_battle_manager->sendPlayerMsg(player, pack, true);

	pack = this->createMsgLobbyGameStartAck(0);
	pack->updateGuid(player->getGateGuid());
	m_battle_manager->sendPlayerMsg(player, pack, true);
}

void Battle::playerSelectHero(Player * player, int hero_id)
{
	player->setHeroId(hero_id);

	InterPacket * pack = MessageMaker::makeMsgSelectHeroAck(NULL, player->getId(), player->getHeroId());
	// send hero info to everyone
	broadcastPacket(pack, player->getTeamId());
	//m_battle_manager->sendPlayerMsg(player->getSession(), pack, true);
}

void Battle::playerConfirmHero(Player * player, int hero_id)
{
	player->confirmHero(hero_id);

	InterPacket * pack = MessageMaker::makeMsgConfirmHeroAck(NULL, player->getId(), player->getHeroId());
	//m_battle_manager->sendPlayerMsg(player, pack, true);
	// broadcast
	broadcastPacket(pack, player->getTeamId());
	if (this->isAllPlayerConfirmedHero())
	{
		this->notifyClientLoading();
		//this->notifyClientCountDown();
	}
}

Player * Battle::addPlayer(session_t session_id, PlayerId_t player_id, uint64 gate_guid, int hero_id, const string & player_name)
{
	CHECK_RETURN(BS_PREPARE == m_battle_status, NULL);

	PlayerMap_t::iterator it = m_player_map.find(player_id);
	if (it != m_player_map.end())
	{
		return NULL;
	}

	Player * player = new Player();
	player->init(0, player_id, hero_id, gate_guid, player_name, this);
	m_player_map.insert(std::make_pair(player_id, player));

	m_battle_team.addPlayer(player);

	//m_offline_player_set.erase(player);

	this->notifyClientJoinGame(player, player->getId(), player->getTeamId(), player->getGateGuid());

	//if (m_battle_team.getAllMemberNumber() >= m_one_team_number * 2)
	//{
	//	// 
	//	this->notifyClientLoading();
	//}

	return player;
}

void Battle::removePlayer(Player * player)
{
	//PlayerMap_t::iterator it = m_player_map.find(player->getId());
	//if (it != m_player_map.end())
	//{
	//	m_battle_team.removePlayer(it->second);
	//	delete it->second;
	//	m_player_map.erase(it);
	//}
	//else
	//{
	//	BATTLE_LOG_ERROR("do not find player <%d>", player->getId());
	//}
}

void Battle::playerReadyForBattle(Player * player)
{
	//if (PS_IS_RECONNECTING == player->getReconnectInfo().player_status)
	if (this->isBattleStart())
	{
		// reconnect or loading timeout
		this->playerReadyForReconnection(player);
		return;
	}

	CHECK_RETURN(BS_CLIENT_LOADING == m_battle_status, );

	this->changePlayerStatus(player, BPS_READY);

	InterPacket * packet = this->createMsgCombatPlayerReadyAck(1, player->getGateGuid());
	m_battle_manager->sendPlayerMsg(player, packet, true);

	//bool is_all_ready = m_battle_team.getAllMemberReadyNumber() >= ONE_TEAM_MEMBER_NUMBER * 2;
	bool is_all_ready = m_battle_team.isAllReady();

	if (is_all_ready)
	{
		this->notifyClientEnterBattle();

		this->startBattle();

	}
}

void Battle::playerReadyForReconnection(Player * player)
{
	{
		InterPacket * packet = this->createMsgCombatPlayerReadyAck(2, player->getGateGuid());
		m_battle_manager->sendPlayerMsg(player, packet, true);
	}

	{
		InterPacket * packet = this->createMsgCombatBattleStartAck(player->getGateGuid());
		m_battle_manager->sendPlayerMsg(player, packet, true);
	}
}

void Battle::playerReportBattleResult(Player * player, const MsgReportBattleResultReq * msg)
{
	DEF_LOG_DEBUG("recv player report battle result, player is <%llu>, win team is <%hhd>", player->getId(), msg->win_team());

	m_battle_result.playerReportBattleResult(player, msg);

	InterPacket * pack = MessageMaker::makeMsgReportBattleResultAck(player->getGateGuid());
	m_battle_manager->sendPlayerMsg(player, pack, true);

	if (m_battle_result.isBattleEnd() && !m_battle_result.isBattleEndNotifyClient())
	{
		m_battle_result.isBattleEndNotifyClient(true);
		// notify
		this->notifyClientBattleEnd();
		this->notifyTrBattleEnd();
	}
}

void Battle::vrReciveBattleReportNotifyBattle(Player * player)
{
	m_battle_result.vrReciveBattleReportNotifyBattle(player);
}

//Player * Battle::getPlayerBySession(session_t session_id)
//{
//	PlayerMap_t::iterator it = m_player_map.find(session_id);
//	if (it != m_player_map.end())
//	{
//		return it->second;
//	}
//
//	return NULL;
//}

bool Battle::isBattleCreate()
{
	return true;
}

bool Battle::isBattleStart()
{
	return m_battle_status >= BS_START_BATTLE;
}

PlayerMap_t & Battle::getPlayerMap()
{
	return m_player_map;
}

void Battle::changePlayerSession(Player * player, session_t session)
{
	//player->setSession(session);
}

void Battle::processClientFramePack(Player * player, const MsgClientFrameSyncReq * cc_msg)
{
	CHECK_RETURN(BS_START_BATTLE == m_battle_status, );

	const MsgFrameSyncAction * msg_action = cc_msg->oAction();

	vector<uint8> * char_vec = new vector<uint8>();
	char_vec->resize(msg_action->aData()->size());

	memcpy(char_vec->data(), (char *)msg_action->aData()->Data(), msg_action->aData()->size());
	m_current_frame_action.push_back(new PlayerFrameInfo(char_vec, player->getId(), msg_action->bytType(), m_battle_frame_info.frame_index + 1));
}

void Battle::processClientToClientMsg(Player * player, const MsgFrameCommandC2C * cc_msg)
{

}

void Battle::processClientToServerMsg(Player * player, const MsgFrameCommandC2S * cs_msg)
{

}

void Battle::processMsgRepairFrameReq(Player * player, const MsgRepairFrameReq * msg)
{
	CHECK_RETURN(PS_ONLINE == player->getReconnectInfo().player_status, );
	CHECK_RETURN(BS_START_BATTLE == m_battle_status, );

	InterPacket * packet = this->createMsgRepairAck(*msg->aFrames(), player->getGateGuid());
	m_battle_manager->sendPlayerMsg(player, packet, true);
}

void Battle::processMsgReconnectionReq(Player * player, const MsgReconnectionReq * msg)
{
	//CHECK_RETURN(PS_IS_RECONNECTING == player->getReconnectInfo().player_status, );

	uint32 start_frame_index = player->getReconnectInfo().last_send_frame_index;

	InterPacket * packet = NULL;
	if (player->getReconnectInfo().last_send_frame_index + reconnect_send_frame_number > m_battle_frame_info.frame_index)
	{
		packet = this->createMsgReconnectionAck(player->getReconnectInfo().last_send_frame_index, m_battle_frame_info.frame_index - player->getReconnectInfo().last_send_frame_index + 1, true, player->getGateGuid());
		player->getReconnectInfo().last_send_frame_index = m_battle_frame_info.frame_index + 1;
		player->getReconnectInfo().player_status = PS_ONLINE;
	}
	else
	{
		packet = this->createMsgReconnectionAck(player->getReconnectInfo().last_send_frame_index, reconnect_send_frame_number, false, player->getGateGuid());
		player->getReconnectInfo().last_send_frame_index += reconnect_send_frame_number;
	}
	m_battle_manager->sendPlayerMsg(player, packet, true);
}

void Battle::handleClientValidation(int msg_id, PlayerId_t player_id, uint32_t frame_no, const string & validation_value)
{
	m_manage_client_validation.handleValidation(m_battle_frame_info.frame_index, frame_no, msg_id, player_id, validation_value);
}

void Battle::startBattle()
{
	m_battle_status = BS_START_BATTLE;
	m_battle_start_time = BattleManager::instance()->getSystemTime();
	m_battle_frame_info.last_frame_time = m_battle_start_time;
	m_battle_frame_info.frame_index = 0;

	m_battle_status = BS_START_BATTLE;

	this->changePlayerStatus(BPS_START_BATTLE);

	m_battle_manager->battleIsStarting(this);

	this->makeCurrentFrame(m_battle_frame_info.frame_index, m_battle_frame_info.last_frame_time);

}

void Battle::changePlayerStatus(BattlePlayerStatus bps_value)
{
	for (PlayerMap_t::iterator it = m_player_map.begin(); it != m_player_map.end(); ++it)
	{
		Player * player = it->second;
		changePlayerStatus(player, bps_value);
	}
}

void Battle::changePlayerStatus(Player * player, BattlePlayerStatus bps_value)
{
	switch (bps_value)
	{
	case BPS_CLIENT_LOADING:
		// todo
		//if (BPS_COUNT_DOWN == player->getStatus() || BPS_IS_RECONNECTING == player->getStatus())
		if (BPS_COUNT_DOWN == player->getStatus() || BPS_IS_RECONNECTING == player->getStatus() || BPS_PREPARE == player->getStatus())
		{
			player->playerClientLoading();
		}
		break;
	case BPS_READY:
		if (BPS_CLIENT_LOADING == player->getStatus())
		{
			player->playerClientReady();
		}
		break;
	case BPS_START_BATTLE:
		if (BPS_READY == player->getStatus())
		{
			player->playerEnterBattle();
		}
		else
		{
			DEF_LOG_ERROR("player is not ready while battle is starting, player <%llu>, status <%d>", player->getId(), player->getStatus());
		}
		break;
	}
}

void Battle::notifyClientCountDown()
{
	BATTLE_LOG_DEBUG("start to count down");
	m_battle_status = BS_COUNT_DOWN;

	m_battle_timeout_info.all_player_confirmed_hero_time = getSystemMilliSeconds();

	this->sendNotifyClientCountDown();
	this->changePlayerStatus(BPS_COUNT_DOWN);
}

void Battle::notifyClientLoading()
{
	BATTLE_LOG_INFO("start to notify client to load for battle");
	m_battle_status = BS_CLIENT_LOADING;

	m_battle_timeout_info.all_player_start_loading_time = getSystemMilliSeconds();

	this->sendNotifyClientLoadingMsg();
	this->changePlayerStatus(BPS_CLIENT_LOADING);
}

void Battle::sendNotifyClientCountDown()
{
	InterPacket * packet = this->createMsgBattleCountDownNty(m_battle_manager->getBattleSetting().buy_skin_timeout);

	for (PlayerMap_t::iterator it = m_player_map.begin(); it != m_player_map.end(); ++it)
	{
		Player * player = it->second;
		this->updateInterPacketGuid(packet, player->getGateGuid());
		m_battle_manager->sendPlayerMsg(player, packet, false);
	}

	delete packet;
}

void Battle::sendNotifyClientLoadingMsg()
{
	InterPacket * packet = this->createMsgLobbyGameStartAck(0);
	
	for (PlayerMap_t::iterator it = m_player_map.begin(); it != m_player_map.end(); ++it)
	{
		Player * player = it->second;
		this->updateInterPacketGuid(packet, player->getGateGuid());
		m_battle_manager->sendPlayerMsg(player, packet, false);
	}

	delete packet;
}

void Battle::notifyClientJoinGame(Player * player, PlayerId_t player_id, uint32 team_id, uint64_t gate_guid)
{
	InterPacket * packet = createMsgLobbyJoinGameAck(0, player_id, gate_guid, team_id);
	m_battle_manager->sendPlayerMsg(player, packet, true);
}

void Battle::notifyClientEnterBattle()
{
	InterPacket * packet = this->createMsgCombatBattleStartAck(0);
	for (PlayerMap_t::iterator it = m_player_map.begin(); it != m_player_map.end(); ++it)
	{
		Player * player = it->second;
		this->updateInterPacketGuid(packet, player->getGateGuid());
		BattleManager::instance()->sendPlayerMsg(player, packet, false);
	}

	delete packet;
}

void Battle::notifyClientBattleEnd()
{
	InterPacket * packet = MessageMaker::makeMsgBattleEndNty(0);
	this->broadcastPacket(packet, 0, true);
	//for (auto it : m_player_map)
	//{
	//	Player * player = it.second;
	//	
	//}
}

void Battle::notifyTrBattleEnd()
{
	vector<uint64> player_vec;
	for (auto it : m_player_map)
	{
		player_vec.push_back(it.second->getId());
	}
	InterPacket * pack = this->makeSMsgBattleEndNotifyVr();
	ManageTrSession::instance()->handleOutputPacket(pack);

}

//void Battle::sendReqFrameToPlayer(session_t session_id, const Vector<uint32> & frame_req)
//{
//	Player * player = this->getPlayerBySession(session_id);
//	if (NULL == player)
//	{
//		//error
//		return;
//	}
//
//	for (Vector<uint32>::const_iterator it = frame_req.begin(); it != frame_req.end(); ++it)
//	{
//		int frame_no = *it;
//		FramePacketInfo * frame_pack_info = this->getFramePacketInfo(frame_no);
//		if (NULL == frame_pack_info)
//		{
//			// error
//			BATTLE_LOG_ERROR("failed to get frame packet info , frame index is <%d>", frame_no);
//			continue;
//		}
//		m_battle_manager->sendPlayerMsg(session_id, frame_pack_info->packet, false);
//	}
//}

FramePacketInfo * Battle::getFramePacketInfo(uint32 frame_index)
{
	PacketFrameInfoMap_t::iterator it = m_frame_packet_info_map.find(frame_index);
	if (it != m_frame_packet_info_map.end())
	{
		return it->second;
	}

	return NULL;
}

void Battle::playerLoginWithLogout(Player * player)
{
	if (this->isBattleStart())
	{
		player->getReconnectInfo().last_send_frame_index = 0;
		player->getReconnectInfo().player_status = PS_IS_RECONNECTING;
		player->getReconnectInfo().last_send_frame_index = 0;
	}
	else
	{

	}
}

void Battle::playerLoginWithoutLogout(Player * player)
{
	if (this->isBattleStart())
	{
		player->getReconnectInfo().last_send_frame_index = 0;
		player->getReconnectInfo().player_status = PS_ONLINE;
		player->getReconnectInfo().last_send_frame_index = 0;
	}
	else
	{

	}
}

void Battle::clearFrameAction(PlayerFrameInfoVec_t & frame_action_vec)
{
	for (auto action_info : frame_action_vec)
	{
		delete action_info;
	}

	frame_action_vec.clear();
}

bool Battle::isAllPlayerConfirmedHero()
{
	bool ret = true;
	for (auto it : m_player_map)
	{
		if (!it.second->isConfirmedHero())
		{
			ret = false;
		}
	}

	return ret;
}

void Battle::updateInterPacketGuid(InterPacket * packet, uint64_t guid)
{
	packet->msg_head.guid = guid;
	memcpy(packet->getStream(), &packet->msg_head, sizeof(packet->msg_head));
}

void Battle::checkBeforeBattleTimeout()
{
	time_t curr_time = getSystemMilliSeconds();
	if (BS_PREPARE == m_battle_status)
	{
		CK_TRUE_PRINT(0 == m_battle_timeout_info.battle_create_time);

		if (curr_time - m_battle_timeout_info.battle_create_time >= m_battle_manager->getBattleSetting().select_hero_timeout)
		{
			this->timeoutOfSelectHero();
		}
	}
	else if (BS_COUNT_DOWN == m_battle_status)
	{
		CK_TRUE_PRINT(0 == m_battle_timeout_info.all_player_confirmed_hero_time);
		if (curr_time - m_battle_timeout_info.all_player_confirmed_hero_time >= m_battle_manager->getBattleSetting().buy_skin_timeout)
		{
			this->timeoutOfBuySkin();
		}
	}
	else if (BS_CLIENT_LOADING == m_battle_status)
	{
		CK_TRUE_PRINT(0 == m_battle_timeout_info.all_player_start_loading_time);

		if (curr_time - m_battle_timeout_info.all_player_start_loading_time >= m_battle_manager->getBattleSetting().loading_timeout)
		{
			this->timeoutOfClientLoading();
		}
	}
}

void Battle::timeoutOfSelectHero()
{
	DEF_LOG_DEBUG("timeout of select hero");

	for (auto it : m_player_map)
	{
		Player * player = it.second;
		if (0 == player->getHeroId())
		{
			player->randomHeroForBattle();

			InterPacket * pack = MessageMaker::makeMsgSelectHeroAck(NULL, player->getId(), player->getHeroId());
			// send hero info to everyone
			broadcastPacket(pack, player->getTeamId());
		}

		this->notifyClientCountDown();

	}
}

void Battle::timeoutOfBuySkin()
{
	DEF_LOG_DEBUG("timeout of count down");

	m_battle_timeout_info.all_player_start_loading_time = getSystemMilliSeconds();

	this->notifyClientLoading();
}

void Battle::timeoutOfClientLoading()
{
	this->startBattle();
}

InterPacket * Battle::createMsgLobbyGameStartAck(int count_down)
{
	InterPacket * packet = new InterPacket();
	InterHead head;
	head.msg_id = Protocol_Lobby_E_PROTOCOL_LOBBY_GAME_START_ACK;

	vector<flatbuffers::Offset<MsgGamePlayerInfo> > msg_player_info_vec;
	FlatBufferBuilder builder;
	for (PlayerMap_t::iterator it = m_player_map.begin(); it != m_player_map.end(); ++it)
	{
		Player * player = it->second;
		flatbuffers::Offset<MsgGamePlayerInfo> player_off = CreateMsgGamePlayerInfo(builder, player->getId(), player->getHeroId(), player->getTeamId());
		msg_player_info_vec.push_back(player_off);
	}

	//MsgLobbyGameStartAckBuilder game_start_builder(builder);
	Offset<Vector<flatbuffers::Offset<MsgGamePlayerInfo> > > player_list_off = builder.CreateVector(msg_player_info_vec);
	//Offset<Vector<flatbuffers::Offset<MsgGamePlayerInfo> > > player_list_off = builder.CreateVector(msg_player_info_vec);
	//game_start_builder.add_usCountdown(6);
	//game_start_builder.add_oPlayerList(player_list_off);

	//Offset<MsgLobbyGameStartAck> start_off = game_start_builder.Finish();
	Offset<MsgLobbyGameStartAck> start_off = CreateMsgLobbyGameStartAck(builder, count_down, player_list_off);
	builder.Finish(start_off);

	head.msg_len = builder.GetSize();
	packet->setMsgInfo(head, (char *)builder.GetBufferPointer());	

	//for (vector<FlatBufferBuilder *>::iterator it = builder_vec.begin(); it != builder_vec.end(); ++it)
	//{
	//	delete *it;
	//}

	return packet;
}

InterPacket * Battle::createMsgBattleCountDownNty(int count_down)
{
	FlatBufferBuilder builder;
	Offset<MsgBattleCountDownNty> off_msg = CreateMsgBattleCountDownNty(builder, count_down);
	builder.Finish(off_msg);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgBattleCountDownNty, 0, builder.GetSize(), builder.GetBufferPointer());

	return pack;
}

InterPacket * Battle::createMsgLobbyJoinGameAck(int is_success, PlayerId_t player_id, uint64_t gate_guid, int team_id)
{
	InterHead head;
	head.msg_id = Protocol_Lobby_E_PROTOCOL_LOBBY_JOIN_GAME_ACK;
	head.guid = gate_guid;

	FlatBufferBuilder builder;
	Offset<MsgLobbyJoinGameAck> join_game_off = CreateMsgLobbyJoinGameAck(builder, is_success, player_id, team_id);
	builder.Finish(join_game_off);


	//{
	//	const MobaGo::FlatBuffer::MsgLobbyJoinGameAck * join_game = flatbuffers::GetRoot<MobaGo::FlatBuffer::MsgLobbyJoinGameAck>(builder.GetBufferPointer());
	//	int a = join_game->uPlayerUid();
	//	int b = join_game->bytIsSuccess();
	//}

	InterPacket * packet = new InterPacket();

	head.msg_len = builder.GetSize();
	packet->setMsgInfo(head, (char *)builder.GetBufferPointer());
	return packet;
}

InterPacket * Battle::createMsgCombatBattleStartAck(uint64 gate_guid)
{
	FlatBufferBuilder builder;
	Offset<MsgCombatBattleStartAck> battle_start_off = CreateMsgCombatBattleStartAck(builder, 99);
	builder.Finish(battle_start_off);

	InterPacket * packet = new InterPacket();
	InterHead head;
	head.msg_id = Protocol_Combat_E_PROTOCOL_COMBAT_BATTLE_START_ACK;
	head.msg_len = builder.GetSize();
	head.guid = gate_guid;
	packet->setMsgInfo(head, (char *)builder.GetBufferPointer());
	return packet;
}

InterPacket * Battle::createMsgFrameSyncPack(uint64_t gate_guid)
{
	FlatBufferBuilder builder;
	vector<flatbuffers::Offset<MsgFrameSyncAction> > action_offset_vec;
	vector<Offset<Vector<uint8> > > action_data_off_vec;
	for (PlayerFrameInfoVec_t::iterator it = m_current_frame_action.begin(); it != m_current_frame_action.end(); ++it)
	{
		PlayerFrameInfo * player_frame_info = *it;
		Offset<Vector<uint8> > action_bytes_off = builder.CreateVector(*player_frame_info->player_action_bytes);
		action_data_off_vec.push_back(action_bytes_off);
	}

	int action_data_off_index = 0;
	for (PlayerFrameInfoVec_t::iterator it = m_current_frame_action.begin(); it != m_current_frame_action.end(); ++it)
	{
		PlayerFrameInfo * player_frame_info = *it;
		Offset<Vector<uint8> > action_bytes_off = action_data_off_vec[action_data_off_index];
		++action_data_off_index;

		MsgFrameSyncActionBuilder action_build(builder);

		action_build.add_bytType(player_frame_info->action_type);
		action_build.add_aData(action_bytes_off);

		Offset<MsgFrameSyncAction> action_off = action_build.Finish();

		action_offset_vec.push_back(action_off);
	}

	Offset<Vector<flatbuffers::Offset<MsgFrameSyncAction> > > action_list_off;
	if (action_offset_vec.size() > 0)
	{
		action_list_off = builder.CreateVector(action_offset_vec);
		//detail_builder.add_aFrameActions(action_list_off);
	}

	MsgFrameSyncDetailBuilder detail_builder(builder);
	if (action_offset_vec.size() > 0)
	{
		detail_builder.add_aFrameActions(action_list_off);
	}

	detail_builder.add_uFrameNo(m_battle_frame_info.frame_index);
	Offset<MsgFrameSyncDetail> detail_off = detail_builder.Finish();
	builder.Finish(detail_off);
	
	InterPacket * packet = new InterPacket();
	InterHead head;
	head.msg_id = Protocol_Combat_E_PROTOCOL_COMBAT_MSG_SERVER_FRAME_SYNC_ACK;
	head.msg_len = builder.GetSize();
	head.guid = gate_guid;
	packet->setMsgInfo(head, (char *)builder.GetBufferPointer());

	//for (vector<FlatBufferBuilder *>::iterator it = builder.begin(); it != builder_vec.end(); ++it)
	//{
	//	delete *it;
	//}

	return packet;
}

InterPacket * Battle::createMsgReconnectionAck(uint32 start_frome_index, uint32 frame_count, bool is_reach_end, uint64_t gate_guid)
{
	BATTLE_LOG_INFO("createMsgReconnectionAck, start frame index <%d>, count is <%d>, is reach end <%d>", start_frome_index, frame_count, is_reach_end);

	InterPacket * packet = new InterPacket();
	packet->msg_head.msg_id = Protocol_Combat_E_PROTOCOL_COMBAT_MSG_RECONNECTION_ACK;
	packet->msg_head.msg_len = 0;
	packet->msg_head.guid = gate_guid;
	packet->msg_buffer = new char[65535];
	int pos = InterPacket::pack_head_size;

	uint8 is_eof = is_reach_end ? 2 : 1;
	pos += ServerPacket::appendUint8(packet->msg_buffer + pos, is_eof);
	pos += ServerPacket::appendUint16(packet->msg_buffer + pos, frame_count);

	for (uint32 i = start_frome_index; i < start_frome_index + frame_count; ++i)
	{
		FramePacketInfo * frame_pack_info = this->getFramePacketInfo(i);
		if (NULL == frame_pack_info)
		{
			break;
		}
		
		this->updateInterPacketGuid(packet, gate_guid);

		pos += ServerPacket::appendUint16(packet->msg_buffer + pos, frame_pack_info->packet->msg_head.msg_len);
		memcpy(packet->msg_buffer + pos, frame_pack_info->packet->msg_buffer + InterPacket::pack_head_size, frame_pack_info->packet->msg_head.msg_len);
		pos += frame_pack_info->packet->msg_head.msg_len;
	}

	packet->msg_head.msg_len = pos;
	memcpy(packet->msg_buffer, &packet->msg_head, InterPacket::pack_head_size);

	BATTLE_LOG_INFO("send player reconnect msg, start frame index <%d, %d>, packet size is <%hd>", start_frome_index, frame_count, packet->msg_head.msg_len);

	return packet;
}

InterPacket * Battle::createMsgRepairAck(const flatbuffers::Vector<uint32_t> & frame_index_vec, uint64_t gate_guid)
{
	InterPacket * packet = new InterPacket();
	//packet->msg_head.msg_id = Protocol_Combat_E_PROTOCOL_COMBAT_MSG_FRAME_REPAIR_REQ;
	//packet->msg_head.msg_len = 0;
	packet->msg_buffer = new char[65535];
	int pos = 0;

	for (flatbuffers::Vector<uint32_t>::const_iterator it = frame_index_vec.begin(); it != frame_index_vec.end(); ++it)
	{
		FramePacketInfo * frame_pack_info = this->getFramePacketInfo(*it);
		if (NULL == frame_pack_info)
		{
			// error
			break;
		}

		this->updateInterPacketGuid(frame_pack_info->packet, gate_guid);
		memcpy(packet->msg_buffer + pos, frame_pack_info->packet->msg_buffer, InterPacket::pack_head_size + frame_pack_info->packet->msg_head.msg_len);
		pos += InterPacket::pack_head_size + frame_pack_info->packet->msg_head.msg_len;
	}

	int frame_size = frame_index_vec.size();
	int msg_id = packet->msg_head.msg_len;
	BATTLE_LOG_INFO("send player repaire msg, frame size is <%d>, packet size is <%d>", frame_size, msg_id);

	packet->msg_head.msg_len = pos - InterPacket::pack_head_size;
	return packet;
}

InterPacket * Battle::createMsgCombatPlayerReadyAck(int battle_status, uint64_t gate_guid)
{
	FlatBufferBuilder builder;
	Offset<MsgCombatBattleStartAck> ack_off = CreateMsgCombatBattleStartAck(builder, battle_status);
	builder.Finish(ack_off);

	InterPacket * packet = new InterPacket();
	InterHead head;
	head.msg_id = Protocol_Combat_E_PROTOCOL_COMBAT_PLAYER_READY_ACK;
	head.msg_len = builder.GetSize();
	head.guid = gate_guid;
	packet->setMsgInfo(head, (char *)builder.GetBufferPointer());
	return packet;
}

InterPacket * Battle::makeSMsgBattleEndNotifyVr()
{
	FlatBufferBuilder builder;
	
	vector<Offset<PPlayerBattleResult>> off_player_vec;
	for (auto it : m_player_map)
	{
		Player * player = it.second;
		Offset<PPlayerBattleResult> off_player = CreatePPlayerBattleResult(builder, player->getId());

		off_player_vec.push_back(off_player);
	}

	Offset<flatbuffers::Vector<flatbuffers::Offset<PPlayerBattleResult>>> off_vec = builder.CreateVector(off_player_vec);

	Offset<SMsgBattleEndNotifyVr> off_nty = CreateSMsgBattleEndNotifyVr(builder, m_battle_result.getWinTeamId(), off_vec);
	builder.Finish(off_nty);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgBattleEndNotifyVr, 0, builder.GetSize(), builder.GetBufferPointer());

	return pack;
}
