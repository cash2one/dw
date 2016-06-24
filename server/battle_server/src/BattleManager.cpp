
#include "BattleManager.h"

#include "ManagerSession.h"
#include "Logger.h"
#include "opcode.h"
#include "BattleLog.h"
#include "StatTime.h"
#include "ProtocolBuilder.h"
#include "Singleton.h"
#include "BattleConfig.h"
#include "check_macro.h"
#include "ManageTrSession.h"

using namespace MobaGo::FlatBuffer;

#define GET_PLAYER_BATTLE_INFO(ret)	\
	Player * player = m_player_manager.getPlayerBySession(inter_pack->msg_head.guid);	\
	if (NULL == player)	\
		{	\
		BATTLE_LOG_ERROR("failed to get player by session <%llu>", inter_pack->msg_head.guid);	\
		return ret;	\
	}	\
	Battle * battle = player->getBattle();	\
	if (NULL == battle)	\
	{	\
		BATTLE_LOG_ERROR("failed to get player battle, player session is <%llu>", inter_pack->msg_head.guid);	\
		return ret;	\
	}

//#define GET_PLAYER_INFO(ret)	\
//	Player * player = m_player_manager.getPlayerBySession(session_id);	\
//	if (NULL == player)	\
//	{	\
//		return ret;	\
//	}

BattleManager::BattleManager()
	: m_player_index(0)
	, m_current_batte(NULL)
	, m_last_update_battle_time(0)
	, m_del_frame_pack_for_press_test(false)
{
	//m_current_batte = new Battle();
}

BattleManager::~BattleManager()
{

}

BattleManager * BattleManager::instance()
{
	return Singleton<BattleManager>::instance();
}

int BattleManager::init(bool del_frame_pack_for_press_test)
{
	m_del_frame_pack_for_press_test = del_frame_pack_for_press_test;

	CK_FALSE_RETURN(BattleConfig::loadBattleSetting(m_battle_setting_cfg), -1);

	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Opcode_E_MsgEnterBattleReq,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgEnterBattleReq>(this, &BattleManager::handleMsgEnterBattleReq));

	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Opcode_E_MsgReEnterBattleReq,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgReEnterBattleReq>(this, &BattleManager::handleMsgReEnterBattleReq));

	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Opcode_E_MsgSelectHeroReq,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgSelectHeroReq>(this, &BattleManager::handleMsgSelectHeroReq));

	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Opcode_E_MsgConfirmHeroReq,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgConfirmHeroReq>(this, &BattleManager::handleMsgConfirmHeroReq));

	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Opcode_E_MsgReportBattleResultReq,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgReportBattleResultReq>(this, &BattleManager::handleMsgReportBattleResultReq));


	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Protocol_Base_E_PROTOCOL_KEEPALIVE,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgLobbyKeepAliveReq>(this, &BattleManager::handleMsgLobbyKeepAliveReq));

	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Protocol_Lobby_E_PROTOCOL_LOBBY_JOIN_GAME_REQ,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgLobbyJoinGameReq>(this, &BattleManager::handleMsgLobbyJoinGameReq));

	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Protocol_Combat_E_PROTOCOL_COMBAT_PLAYER_READY_REQ,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgCombatPlayerReadyReq>(this, &BattleManager::handleMsgCombatPlayerReadyReq));

	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Protocol_Combat_E_PROTOCOL_COMBAT_MSG_CLIENT_FRAME_SYNC_REQ,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgClientFrameSyncReq>(this, &BattleManager::handleMsgClientFrameSyncReq));

	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Protocol_Combat_E_PROTOCOL_COMBAT_MSG_FRAME_REPAIR_REQ,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgRepairFrameReq>(this, &BattleManager::handleMsgRepairFrameReq));

	ManagerSession::instance()->addMsgHandle(MobaGo::Protocol::Protocol_Combat_E_PROTOCOL_COMBAT_MSG_RECONNECTION_REQ,
		new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgReconnectionReq>(this, &BattleManager::handleMsgReconnectionReq));

	ManagerSession::instance()->addMsgHandle(E_MSG_PLAYER_CLOSE_CONNECTION,
		new PacketHandle<BattleManager, InterPacket, EmptyMsg>(this, &BattleManager::handlePlayerCloseConnection));

	// client validation
	for (int i = Opcode_E_MsgClientValidation; i <= Opcode_E_MsgClientValidationEnd; ++i)
	{
		ManagerSession::instance()->addMsgHandle(i,
			new PacketHandle<BattleManager, InterPacket, MobaGo::FlatBuffer::MsgClientValidation>(this, &BattleManager::handleClientValidation));
	}

	//m_current_batte->init(this);

	return 0;
}

void BattleManager::update()
{
	uint64 curr_time = getSystemTime();

	//PlayerBattleInfoSet_t battle_list = m_battle_set;

	uint64 start_time = getSystemMilliSeconds();

	if (start_time - m_last_update_battle_time < 10)
	{
		return;
	}

	m_last_update_battle_time = start_time;

	int battle_size = m_battle_set.size();

	for (PlayerBattleInfoSet_t::iterator it = m_battle_set.begin(); it != m_battle_set.end(); )
	{
		Battle * battle = *it;
		if (battle->isBattleEnd(curr_time))
		{
			// normally end or timeout
			this->removeBattle(battle);
			m_battle_set.erase(it++);
		}
		else
		{
			battle->update(curr_time);
			++it;
		}
	}

	m_manage_stat.update();

	uint64 end_time = getSystemMilliSeconds();

	int diff_time = int(end_time - start_time);
	if (diff_time > 30)
	{
		BATTLE_LOG_ERROR("get bigger update of BattleManager, battle size is <%d>, current battle size is <%d>, spend time is <%d>", battle_size, m_battle_set.size(), diff_time);
	}
}

//void BattleManager::connectClosed(session_t session_id)
//{
//	//GET_PLAYER_BATTLE_INFO();
//
//	//battle_info->battle->removePlayer(session_id);
//	//this->removePlayerBattleInfoBySession(session_id);
//}

uint64 BattleManager::getSystemTime()
{
	return getSystemMilliSeconds();
}

int BattleManager::handleMsgPing(InterPacket * inter_pack, const MsgPing * msg)
{
	// todo
	return 0;

	InterPacket * pack = ProtocolBuilder::createMsgPing(msg->client_start_time(), msg->gate_recv_time(), msg->server_recv_time());
	this->sendPlayerMsg(inter_pack->msg_head.guid, pack, true);
	return 0;
}

int BattleManager::handleMsgEnterBattleReq(InterPacket * inter_pack, const MsgEnterBattleReq * msg)
{
	//GET_PLAYER_BATTLE_INFO(-1);

	uint64_t accoun_id = msg->user_id();
	Player * player = m_player_manager.getPlayerById(accoun_id);
	if (NULL == player)
	{
		// error
		DEF_LOG_ERROR("do not find player by id <%llu>", accoun_id);
		return -1;
	}

	Battle * battle = player->getBattle();

	player->setSession(inter_pack->msg_head.guid);
	player->setGateGuid(inter_pack->msg_head.guid);

	battle->playerEnterBattle(player);

	this->connectPlayerSession(player, inter_pack->msg_head.guid);

	return 0;
}

int BattleManager::handleMsgReEnterBattleReq(InterPacket * inter_pack, const MsgReEnterBattleReq * msg)
{
	//GET_PLAYER_BATTLE_INFO(-1);
	uint64_t accoun_id = msg->user_id();
	Player * player = m_player_manager.getPlayerById(accoun_id);
	if (NULL == player)
	{
		// error
		DEF_LOG_ERROR("do not find player by id <%llu>", accoun_id);
		return -1;
	}

	player->setSession(inter_pack->msg_head.guid);
	player->setGateGuid(inter_pack->msg_head.guid);

	this->connectPlayerSession(player, inter_pack->msg_head.guid);

	Battle * battle = player->getBattle();

	player->setSession(inter_pack->msg_head.guid);
	player->setGateGuid(inter_pack->msg_head.guid);

	player->getReconnectInfo().player_status = PS_IS_RECONNECTING;
	player->getReconnectInfo().last_send_frame_index = 0;

	battle->playerReEnterBattle(player);

	this->connectPlayerSession(player, inter_pack->msg_head.guid);

	return 0;
}

int BattleManager::handleMsgSelectHeroReq(InterPacket * inter_pack, const MsgSelectHeroReq * msg)
{
	GET_PLAYER_BATTLE_INFO(-1);

	battle->playerSelectHero(player, msg->hero_id());

	return 0;
}

int BattleManager::handleMsgConfirmHeroReq(InterPacket * inter_pack, const MsgConfirmHeroReq * msg)
{
	GET_PLAYER_BATTLE_INFO(-1);

	battle->playerConfirmHero(player, msg->hero_id());

	return 0;
}

int BattleManager::handleMsgReportBattleResultReq(InterPacket * inter_pack, const MsgReportBattleResultReq * msg)
{
	GET_PLAYER_BATTLE_INFO(-1);

	battle->playerReportBattleResult(player, msg);
	return 0;
}

int BattleManager::handleSMsgNotifyBattleMatchResult(InterPacket * inter_pack, const SMsgNotifyBattleMatchResult * msg)
{
	DEF_LOG_DEBUG("recv tr match result, player size is <%d>", NULL == msg->player_list() ? 0 : msg->player_list()->size());
	Battle * battle_info = new Battle();
	battle_info->init(this, msg->player_list()->size() / 2, false);
	this->addBattle(battle_info);
	
	for (int i = 0; i < msg->player_list()->size(); ++i)
	{
		auto player_info = msg->player_list()->Get(i);
		Player * player = battle_info->addPlayer(inter_pack->session_id, player_info->user_id(), inter_pack->msg_head.guid, 0, "");
		this->addPlayer(player);
		DEF_LOG_DEBUG("get player <%llu>", player_info->user_id());
	}

	InterPacket * pack = MessageMaker::makeSMsgNotifyBattleMatchResultAck(msg->match_id(), 1);
	ManageTrSession::instance()->handleOutputPacket(pack);

	return 0;
}

int BattleManager::handleSMsgVrReciveBattleReportNotifyBattle(InterPacket * inter_pack, const EmptyMsg * msg)
{
	DEF_LOG_DEBUG("recv SMsgVrReciveBattleReportNotifyBattle, user id is <%llu>", inter_pack->msg_head.guid);

	GET_PLAYER_BATTLE_INFO(-1);

	battle->vrReciveBattleReportNotifyBattle(player);

	return 0;
}

int BattleManager::handleMsgLobbyKeepAliveReq(InterPacket * inter_pack, const MobaGo::FlatBuffer::MsgLobbyKeepAliveReq * msg)
{
	uint32_t tt = msg->uReserved();
	return 0;
}

int BattleManager::handleMsgLobbyJoinGameReq(InterPacket * inter_pack, const MobaGo::FlatBuffer::MsgLobbyJoinGameReq * msg)
{
	return -1;

	//BATTLE_LOG_INFO("player login game, session is <%x>", inter_pack->session_id);
	//Player * player = m_player_manager.getPlayerByName(msg->sUserName()->c_str(), false);
	//if (NULL != player)
	//{
	//	if (NULL != player->getBattle())
	//	{
	//		this->checkPlayerSessionByRelogin(player);
	//		m_player_manager.changePlayerSession(player, inter_pack->session_id);
	//		player->getBattle()->playerReconnect(player, inter_pack->session_id);
	//		return 0;
	//	}
	//	else
	//	{
	//		// error
	//		BATTLE_LOG_ERROR("player reconnection, but did not find player battle <%d>", player->getId());
	//	}
	//	return -1;
	//}

	//if (NULL == m_current_batte)
	//{
	//	m_current_batte = new Battle();
	//	m_current_batte->init(this, m_del_frame_pack_for_press_test);
	//}

	//int hero_id = msg->uHeroSkinId();

	//BATTLE_LOG_INFO("skin id is <%d>", hero_id);
	//
	//player = m_current_batte->addPlayer(inter_pack->session_id, ++m_player_index, hero_id, msg->sUserName()->c_str());

	//CHECK_RETURN(NULL != player, -1)

	//this->addPlayer(player);
	//
	//if (m_current_batte->isMemberFull())
	//{
	//	m_battle_set.insert(m_current_batte);

	//	m_current_batte = NULL;

	//}

	//return 0;
}

int BattleManager::handleMsgCombatPlayerReadyReq(InterPacket * inter_pack, const MobaGo::FlatBuffer::MsgCombatPlayerReadyReq * msg)
{
	GET_PLAYER_BATTLE_INFO(-1);

	battle->playerReadyForBattle(player);
	return 0;
}

int BattleManager::handleMsgClientFrameSyncReq(InterPacket * inter_pack, const MobaGo::FlatBuffer::MsgClientFrameSyncReq * msg)
{
	GET_PLAYER_BATTLE_INFO(-1);

	//BATTLE_LOG_INFO("recv client frame sync pack, seq is <%d>", msg->uSeq());

	battle->processClientFramePack(player, msg);

	return 0;
}

int BattleManager::handleMsgRepairFrameReq(InterPacket * inter_pack, const MobaGo::FlatBuffer::MsgRepairFrameReq * msg)
{
	GET_PLAYER_BATTLE_INFO(-1);

	battle->processMsgRepairFrameReq(player, msg);
	return 0;
}

int BattleManager::handleMsgReconnectionReq(InterPacket * inter_pack, const MsgReconnectionReq * msg)
{
	GET_PLAYER_BATTLE_INFO(-1);

	battle->processMsgReconnectionReq(player, msg);
	return 0;
}

int BattleManager::handlePlayerCloseConnection(InterPacket * inter_pack, const EmptyMsg *)
{
	BATTLE_LOG_INFO("connection closed <%x>", inter_pack->session_id);

	GET_PLAYER_BATTLE_INFO(-1);

	//if (battle->isBattleCreate())
	//{
	//	m_player_manager.removePlayerBySession(player);
	//}
	//else
	//{
	//	m_player_manager.removePlayer(player);
	//}

	//m_player_manager.removePlayerBySession(player);

	player->getReconnectInfo().player_status = PS_OFFLINE;

	player->setSession(0);

	battle->playerOffline(player);

	return 0;
}

int BattleManager::handleClientValidation(InterPacket * inter_pack, const MsgClientValidation * msg)
{
	GET_PLAYER_BATTLE_INFO(-1);

	battle->handleClientValidation(inter_pack->msg_head.msg_id, player->getId(), msg->frame_no(), msg->check_value()->c_str());
	return 0;
}

//int BattleManager::handleClientValidation_1(session_t session_id, const EmptyMsg *)
//{
//
//}
//
//int BattleManager::handleClientValidation_2(session_t session_id, const EmptyMsg *)
//{
//
//}
//
//int BattleManager::handleClientValidation_3(session_t session_id, const EmptyMsg *)
//{
//
//}
//
//int BattleManager::handleClientValidation_4(session_t session_id, const EmptyMsg *)
//{
//
//}
//
//int BattleManager::handleClientValidation_5(session_t session_id, const EmptyMsg *)
//{
//
//}
//
//int BattleManager::handleClientValidation_6(session_t session_id, const EmptyMsg *)
//{
//
//}
//
//int BattleManager::handleClientValidation_7(session_t session_id, const EmptyMsg *)
//{
//
//}
//
//int BattleManager::handleClientValidation_8(session_t session_id, const EmptyMsg *)
//{
//
//}
//
//int BattleManager::handleClientValidation_9(session_t session_id, const EmptyMsg *)
//{
//
//}
//
//int BattleManager::handleClientValidation_10(session_t session_id, const EmptyMsg *)
//{
//
//}


void BattleManager::sendPlayerMsg(uint64_t session_id, InterPacket * packet, bool is_delete_packet)
{

	//BATTLE_LOG_INFO("send player messgae , session is <%d>, msg id is <%d> len is <%d>", session_id, packet->msg_head.msg_id, packet->msg_head.msg_len);

	//ManagerSession::instance()->getNet()->handleOutputStream((netstream::Session_t)session_id, packet->getStream(), packet->getStreamSize());
	InterPacket * pk = packet;
	if (!is_delete_packet)
	{
		pk = packet->clone();
	}

	m_manage_stat.addOutgoingPacket(1, packet->getStreamSize());

	//pk->session_id = session_id;
	ManagerSession::instance()->handleOutputStream(pk->getStream(), pk->getStreamSize());
	pk->cleanStream();
	delete pk;
}

void BattleManager::sendPlayerMsg(Player * player, InterPacket * packet, bool is_delete_packet)
{
	CHECK_RETURN(NULL != player, );
	CHECK_RETURN(NULL != packet, );

	if (this->isPlayerCanReciveMsg(player))
	{
		this->sendPlayerMsg(player->getSession(), packet, is_delete_packet);
	}
	else
	{
		if (is_delete_packet)
		{
			delete packet;
		}
	}
}

void BattleManager::battleIsStarting(Battle * battle)
{
	//PlayerMap_t & player_map = battle->getPlayerMap();
	//for (PlayerMap_t::iterator it = player_map.begin(); it != player_map.end(); ++it)
	//{
	//	this->addPlayer(it->second);
	//}
}

ManageStat & BattleManager::getStat()
{
	return m_manage_stat;
}

bool BattleManager::isPlayerCanReciveMsg(Player * player)
{
	int player_status = player->getReconnectInfo().player_status;
	if (PS_BASE == player_status || PS_OFFLINE == player_status)
	{
		return false;
	}

	return true;
}

const BattleSetting & BattleManager::getBattleSetting()
{
	return m_battle_setting_cfg;
}

int BattleManager::getRandomValue()
{
	// todo
	return getSystemMilliSeconds() % 100000000;
}

//void BattleManager::connectionClosed(session_t session_id)
//{
//	//GET_PLAYER_BATTLE_INFO();
//
//	///battle_info->battle->removePlayer(session_id);
//}

void BattleManager::addPlayer(Player * player)
{
	m_player_manager.addPlayer(player);
}

void BattleManager::connectPlayerSession(Player * player, uint64_t session)
{
	m_player_manager.connectPlayerSession(player, session);
}

void BattleManager::removePlayer(Player * player)
{
	m_player_manager.removePlayer(player);
}

void BattleManager::removeBattle(Battle * battle)
{
	PlayerMap_t & player_map = battle->getPlayerMap();
	for (PlayerMap_t::iterator sub_it = player_map.begin(); sub_it != player_map.end(); ++sub_it)
	{
		Player * player = sub_it->second;
		m_player_manager.removePlayer(player);
	}
			
	delete battle;
}

void BattleManager::checkPlayerSessionByRelogin(Player * player)
{
	if (this->isPlayerLoginAgainWithoutLogout(player))
	{
		// remove the pri session, player login again
		m_player_manager.removePlayerBySession(player);
		//ManagerSession::instance()->removeSession(player->getSession());
	}
}

bool BattleManager::isPlayerLoginAgainWithoutLogout(Player * player)
{
	Player * session_player = m_player_manager.getPlayerBySession(player->getSession());
	if (NULL == session_player)
	{
		return false;
	}

	return session_player == player;
}

void BattleManager::addBattle(Battle * battle)
{
	m_battle_set.insert(battle);
}
