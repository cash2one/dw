
#include "ClientProtocol.h"
#include "Robot.h"
#include "Logger.h"
#include "Single.hpp"
#include "FRPlugin.h"
#include "ManageLoginRobot.h"
#include "ManageVrRobot.h"
#include "FunCallCount.hpp"

int Robot::m_choose_server_id = 1;

Robot::Robot()
	: m_session(NULL)
	, m_packet_count(0)
	, m_robot_msg_handle(NULL)
	, m_vr_port(0)
	, m_is_reconnect_battle(false)
	, m_robot_status(RS_Base)
{

}

Robot::~Robot()
{

}

int Robot::init(SessionId session_id, RobotMessageHandle * robot_msg_handle)
{
	m_session = session_id;
	m_robot_msg_handle = robot_msg_handle;

	this->initMsgHandler();

	return 0;
}

void Robot::setRobotStatus(RobotStatus robot_status)
{
	m_robot_status = robot_status;
}

bool Robot::addPlayerMsgHandle(int msg_id, MsgHandleBase * msg_handle)
{
	MessageHandleMap_t::iterator it = m_msg_handle_map.find(msg_id);
	if (it != m_msg_handle_map.end())
	{
		// error
		return false;
	}

	msg_handle->msg_id = msg_id;
	m_msg_handle_map.insert(std::make_pair(msg_id, msg_handle));

	return true;

}

int Robot::handlePacket(ServerPacket * packet)
{
	++m_packet_count;
	if (m_packet_count % 2000 == 0)
	{
		BATTLE_LOG_DEBUG("recv server msg num : <%d,  %d>", m_player_info->user_id, m_packet_count);
	}
	MsgHandleBase * msg_handle = getMsgHandle(packet->msg_head.msg_id);
	if (NULL != msg_handle)
	{
		msg_handle->handle(m_session, packet->getBody(), packet->msg_head.msg_len);
	}
	else
	{
		// error
		return -1;
	}
	return 0;
}

void Robot::update()
{
	if (RS_Connected_vrpublic == m_robot_status)
	{
		this->loginVrpublic();
	}
	else if (RS_Connected_vr == m_robot_status)
	{
		this->loginVr();
	}
	else if (RS_Connected_battle == m_robot_status)
	{
		this->loginBattle();
	}
	//if (!m_player_info->is_logined)
	//{
	//	ClientPacket * pack = ClientProtocol::makeMsgLoginReq(m_player_info->account);
	//	pack->session_id = m_session;
	//	m_robot_msg_handle->handleOutputPacket(pack);
	//	
	//}

	//if (!m_player_info->is_logined)
	//{
	//	int server_id = 1;
	//	ClientPacket * pack = ClientProtocol::makeMsgChooseServerReq(server_id);
	//	pack->session_id = m_session;
	//	m_robot_msg_handle->handleOutputPacket(pack);
	//	m_player_info->is_logined = true;
	//}
	////if (!m_player_info->is_logined)ZZ
	////{
	////	ClientPacket * pack = ClientProtocol::makeMsgLobbyJoinGameReq(m_player_info->account, m_player_info->hero_skin_id);
	////	m_manager_robot->handleOutputPacket(m_session, pack);
	////	m_player_info->is_logined = true;
	////}

	////if (m_player_info->is_battle_started)
	////{
	////	ClientPacket * pack = ClientProtocol::makeMsgClientFrameSyncReqMove(m_player_info->user_id, 1, 2, 3);
	////	m_manager_robot->handleOutputPacket(m_session, pack);
	////}
}

PlayerInfo * Robot::getPlayerInfo()
{
	return m_player_info;
}

void Robot::setSession(SessionId session_id, RobotMessageHandle * robot_msg_handle)
{
	m_session = session_id;

	m_robot_msg_handle = robot_msg_handle;
}

SessionId Robot::getSession()
{
	return m_session;
}

void Robot::setVrServerInfo(const string & ip_addr, int port, const string & session_key)
{
	m_vr_ip_addr = ip_addr;
	m_vr_port = port;
	m_vr_session_key = session_key;
}

const string & Robot::getVrIp()
{
	return m_vr_ip_addr;
}

int Robot::getVrPort()
{
	return m_vr_port;
}

const string & Robot::getVrSessionKey()
{
	return m_vr_session_key;
}

void Robot::setBattleServerInfo(const string & ip_addr, int port, const string & session_key, bool is_reconnect_battle)
{
	m_battle_ip_addr = ip_addr;
	m_battle_port = port;
	m_battle_session_key = session_key;
	m_is_reconnect_battle = is_reconnect_battle;
}

const string & Robot::getBattleIp()
{
	return m_battle_ip_addr;
}

int Robot::getBattlePort()
{
	return m_battle_port;
}

bool Robot::isReconnectBattle()
{
	return m_is_reconnect_battle;
}

const string & Robot::getBattleSessionKey()
{
	return m_battle_session_key;
}

void Robot::connectedVrpublic()
{
	m_robot_status = RS_Connected_vrpublic;
}

void Robot::connectedVr()
{
	m_robot_status = RS_Connected_vr;
}

void Robot::connectedBattle()
{
	m_robot_status = RS_Connected_battle;
}

void Robot::setAccountId(MyGuid_t account_id)
{
	m_player_info->account_id = account_id;
}

void Robot::setChooseServerId(int server_id)
{
	if (server_id > 0)
	{
		m_choose_server_id = server_id;
	}

	DEF_LOG_INFO("setting choose server id is <%d>", m_choose_server_id);
}

int Robot::handleMsgServerListAck(session_t session_id, const MsgServerListAck * input_msg)
{
	if (input_msg->server_list()->size() == 0)
	{
		DEF_LOG_ERROR("get empty server list");
		return -1;
	}


	ClientPacket * pack = ClientProtocol::makeMsgChooseServerReq(m_choose_server_id);

	pack->session_id = session_id;
	m_robot_msg_handle->handleOutputPacket(pack);

	return 0;
}

int Robot::handleMsgChooseServerAck(session_t session_id, const MsgChooseServerAck * msg)
{
	string gate_ip = msg->ip() != NULL ? msg->ip()->c_str() : "";
	string session_key = msg->session_key() != NULL ? msg->session_key()->c_str() : "";
	string announcement = msg->announcement()->c_str() != NULL ? msg->announcement()->c_str() : "";
	DEF_LOG_INFO("get choose server reply, user id is <%llu>, ip is <%s>, port is <%d>, session key is <%s>, announcement is <%s>", msg->user_id(), gate_ip.c_str(), msg->port(), session_key.c_str(), announcement.c_str());


	ManageLoginRobot::instance()->removeRobot(session_id);

	Robot * r = Single<RunningRobot>::GetInstance()->running[session_id];
	r->setAccountId(msg->user_id());
	uint64 user_id = msg->user_id();
	r->setVrServerInfo(msg->ip() != NULL ? msg->ip()->c_str() : "", msg->port(), msg->session_key() != NULL ? msg->session_key()->c_str() : "");

	r->setRobotStatus(RS_Connected_vr);

	ManageVrRobot::instance()->handleRobot(r);
	return 0;
}

//int Robot::handleMsgServerListAck(session_t session_id, const MsgServerListAck * msg)
//{
//	if (msg->server_list()->size() == 0)
//	{
//		DEF_LOG_ERROR("get server list size is zero");
//		return 1;
//	}
//
//	ClientPacket * pack = ClientProtocol::makeMsgChooseServerReq(msg->server_list()->Get(0)->server_id());
//	pack->session_id = m_session;
//	m_robot_msg_handle->handleOutputPacket(pack);
//
//	return 0;
//}

int Robot::handleMsgLobbyJoinGameAck(session_t session_id, const MsgLobbyJoinGameAck * msg)
{
	m_player_info->user_id = msg->uPlayerUid();
	return 0;
}

int Robot::handleMsgLobbyGameStartAck(session_t session_id, const MsgLobbyGameStartAck * msg)
{
	ClientPacket * pack = ClientProtocol::makeMsgCombatPlayerReadyReq(m_player_info->user_id);
	pack->session_id = m_session;
	m_robot_msg_handle->handleOutputPacket(pack);
	return 0;
}

int Robot::handleMsgCombatPlayerReadyAck(session_t session_id, const MsgCombatPlayerReadyAck * msg)
{
	if (msg->bytIsAlreadyRunning() == 1)
	{
		// normal
	}
	else if (msg->bytIsAlreadyRunning() == 2)
	{
		// reconnection
	}
	else
	{
		// error
	}
	return 0;
}

int Robot::handleMsgCombatBattleStartAck(session_t session_id, const MsgCombatBattleStartAck * msg)
{
	m_player_info->is_battle_started = true;
	DEF_LOG_INFO("battle is started <%d>", m_player_info->user_id);
	return 0;
}

int Robot::handleMsgFrameSyncDetail(session_t session_id, const MsgFrameSyncDetail * msg)
{
	return 0;
}

int Robot::handleMsgReconnectionAck(session_t session_id, const MsgReconnectionAck * msg)
{
	return 0;
}

int Robot::handleMsgError(session_t session_id, const MsgError * msg)
{
	if (msg->error_code() == MobaGo::FlatBuffer::Error_NeedCreateSummoner)
	{
		ClientPacket * pack = ClientProtocol::makeMsgCreateSummonerReq(session_id, m_player_info->account, m_player_info->account);
		m_robot_msg_handle->handleOutputPacket(pack);
	}
	else
	{
		DEF_LOG_ERROR("handleMsgError, error id is:%d", msg->error_code());
	}
	return 0;
}

void Robot::sendMsg(ClientPacket * pack)
{
	m_robot_msg_handle->handleOutputPacket(pack);
}

int Robot::handleMsgReqSummonerInfoAck(session_t session_id, const MsgReqSummonerInfoAck * msg)
{
	DEF_LOG_INFO("create summoner name success:summoner_name:%s, summoner_id:<%llu>", msg->summoner_name() == NULL ? "" : msg->summoner_name()->c_str(), m_player_info->account_id);

	m_robot_status = RS_Enter_vr;

	//ClientPacket * pack = ClientProtocol::makeMsgMatchReq(session_id, 1, 2);
	//m_robot_msg_handle->handleOutputPacket(pack);

	int search_friend_times = Single<RobotCfg>::GetInstance()->search_friend_times;
	for (int i = 0; i < search_friend_times; ++i)
	{
		string robot_name = Single<FRPlugin>::GetInstance()->randomCreateRobotName(Single<RobotCfg>::GetInstance()->random_create_name_begin_index,
			Single<RobotCfg>::GetInstance()->random_create_name_end_index);
		ClientPacket * pack = ClientProtocol::makeMsgSearchFriendReq(session_id, robot_name);

		m_robot_msg_handle->handleOutputPacket(pack);

		Single<FunCallCount>::GetInstance()->addSearchFriendCount();
	}

	return 0;
}

int Robot::handleMsgCreateSummonerAck(session_t session_id, const EmptyMsg * msg)
{
	DEF_LOG_INFO("create summoner name success <%llu>", m_player_info->account_id);

	m_robot_status = RS_Enter_vr;

	//ClientPacket * pack = ClientProtocol::makeMsgMatchReq(session_id, 1, 2);
	int search_friend_times = Single<RobotCfg>::GetInstance()->search_friend_times;
	for (int i = 0; i < search_friend_times; ++i)
	{
		string robot_name = Single<FRPlugin>::GetInstance()->randomCreateRobotName(Single<RobotCfg>::GetInstance()->random_create_name_begin_index,
			Single<RobotCfg>::GetInstance()->random_create_name_end_index);
		ClientPacket * pack = ClientProtocol::makeMsgSearchFriendReq(session_id, robot_name);

		m_robot_msg_handle->handleOutputPacket(pack);

		Single<FunCallCount>::GetInstance()->addSearchFriendCount();
	}

	return 0;
}

int Robot::handleMsgVRLoginAck(session_t session_id, const MsgVRLoginAck * input_msg)
{
	ClientPacket * pack = ClientProtocol::makeMsgReqSummonerInfoReq(session_id);
	m_robot_msg_handle->handleOutputPacket(pack);

	m_robot_status = RS_Connected_battle;


	return 0;
}

int Robot::handleMsgMatchEnterBattle(session_t session_id, const MsgMatchEnterBattle * input_msg)
{
	string ip_addr = input_msg->server_ip() != NULL ? input_msg->server_ip()->c_str() : "";

	this->setBattleServerInfo(ip_addr, input_msg->server_port(), "");

	return 0;
}

int Robot::handleMsgReconnectBattle(session_t session_id, const MsgReconnectBattle * input_msg)
{
	DEF_LOG_DEBUG("recive re connect battle msg");

	string ip_addr = input_msg->server_ip() != NULL ? input_msg->server_ip()->c_str() : "";

	this->setBattleServerInfo(ip_addr, input_msg->server_port(), "", true);
	return 0;
}

int Robot::handleMsgCancelMatchAck(session_t session_id, const MsgCancelMatchAck * input_msg)
{
	DEF_LOG_INFO("recive cancel match msg, result is <%d>", input_msg->result());
	return 0;
}

int Robot::handleMsgMatchAck(session_t session_id, const MsgMatchAck * input_msg)
{
	DEF_LOG_INFO("recive match result msg, match result is <%d>", input_msg->match_result());
	return 0;
}

int Robot::handleMsgSearchFriendAck(session_t session_id, const MobaGo::FlatBuffer::MsgSearchFriendAck * msg)
{
	DEF_LOG_INFO("handleMsgSearchFriendAck: friend_id:%llu, friend_head_id:%d, friend_status:%d, ", msg->friend_info()->friend_id(), msg->friend_info()->head_id(), msg->friend_info()->status());

	ClientPacket * pack = ClientProtocol::makeMsgAddFriendReq(session_id, 216454257098274205);
	m_robot_msg_handle->handleOutputPacket(pack);
	return 0;
}

int Robot::handleMsgAddFriendAck(session_t session_id, const MobaGo::FlatBuffer::MsgAddFriendAck * msg)
{
	DEF_LOG_INFO("handleMsgAddFriendAck: add_friend_result:%d", msg->add_friend_result());

	ClientPacket * pack = ClientProtocol::makeMsgDelFriendReq(session_id, 216454257098274205);
	m_robot_msg_handle->handleOutputPacket(pack);
	return 0;
}

int Robot::handleMsgDelFriendAck(session_t session_id, const MobaGo::FlatBuffer::MsgDelFriendAck * msg)
{
	DEF_LOG_INFO("handleMsgDelFriendAck: del_friend_result:%d", msg->del_friend_result());

	ClientPacket * pack = ClientProtocol::makeMsgConnfirmAddFriendReq(session_id, 216454257098274205);
	m_robot_msg_handle->handleOutputPacket(pack);
	return 0;
}

int Robot::handleMsgConfirmAddFriendAck(session_t session_id, const MobaGo::FlatBuffer::MsgConfirmAddFriendAck * msg)
{
	DEF_LOG_INFO("handleMsgConfirmAddFriendAck: friend_id:%llu", msg->friend_info()->friend_id());

	ClientPacket * pack = ClientProtocol::makeMsgAddCancelFriendReq(session_id, 216454257098274205);
	m_robot_msg_handle->handleOutputPacket(pack);
	return 0;
}

int Robot::handleMsgAddCancelFriendAck(session_t session_id, const MobaGo::FlatBuffer::MsgAddCancelFriendAck * msg)
{
	DEF_LOG_INFO("handleMsgAddCancelFriendAck: confirm_cancel_result:%d", msg->confirm_cancel_result());

	ClientPacket * pack = ClientProtocol::makeMsgFriendListReq(session_id);
	m_robot_msg_handle->handleOutputPacket(pack);
	return 0;
}

int Robot::handleMsgFriendListAck(session_t session_id, const MsgFriendListAck * msg)
{
	if (msg->friend_list()->size() != 0)
	{
		DEF_LOG_INFO("handleMsgFriendListAck: friend_id:%llu", msg->friend_list()->Get(0));
	}

	ClientPacket * pack = ClientProtocol::makeMsgApplyForFriendListReq(session_id);
	m_robot_msg_handle->handleOutputPacket(pack);
	return 0;
}

int Robot::handleMsgApplyForFriendListAck(session_t session_id, const MsgApplyForFriendListAck * msg)
{
	if (msg->apply_friend_list()->size() != 0)
	{
		DEF_LOG_INFO("handleMsgApplyForFriendListAck: friend_id:%llu", msg->apply_friend_list()->Get(0));
	}

	ClientPacket * pack = ClientProtocol::makeMsgFriendDetailInfoReq(session_id, 216454257098274205);
	m_robot_msg_handle->handleOutputPacket(pack);

	return 0;
}

int Robot::handleMsgFriendDetailInfoAck(session_t session_id, const MobaGo::FlatBuffer::MsgFriendDetailInfoAck * msg)
{
	DEF_LOG_INFO("handleMsgFriendListAck: friend_id:%llu", msg->friend_info()->friend_id());

	// ClientPacket * pack = ClientProtocol::makeMsgSearchFriendReq(session_id, "robot_1016");
	// m_robot_msg_handle->handleOutputPacket(pack);

	return 0;
}

void Robot::initMsgHandler()
{
	this->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgServerListAck,
		new PackMsg<Robot, MobaGo::FlatBuffer::MsgServerListAck>(this, &Robot::handleMsgServerListAck));

	this->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgChooseServerAck,
		new PackMsg<Robot, MobaGo::FlatBuffer::MsgChooseServerAck>(this, &Robot::handleMsgChooseServerAck));


	this->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgReqSummonerInfoAck,
		new PackMsg<Robot, MobaGo::FlatBuffer::MsgReqSummonerInfoAck>(this, &Robot::handleMsgReqSummonerInfoAck));

	this->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgCreateSummonerAck,
		new PackMsg<Robot, EmptyMsg>(this, &Robot::handleMsgCreateSummonerAck));

	this->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgError,
		new PackMsg<Robot, MsgError>(this, &Robot::handleMsgError));


	this->addPlayerMsgHandle(MobaGo::Protocol::Protocol_Lobby_E_PROTOCOL_LOBBY_JOIN_GAME_ACK,
		new PackMsg<Robot, MobaGo::FlatBuffer::MsgLobbyJoinGameAck>(this, &Robot::handleMsgLobbyJoinGameAck));

	this->addPlayerMsgHandle(MobaGo::Protocol::Protocol_Lobby_E_PROTOCOL_LOBBY_GAME_START_ACK,
		new PackMsg<Robot, MobaGo::FlatBuffer::MsgLobbyGameStartAck>(this, &Robot::handleMsgLobbyGameStartAck));

	this->addPlayerMsgHandle(MobaGo::Protocol::Protocol_Combat_E_PROTOCOL_COMBAT_PLAYER_READY_ACK,
		new PackMsg<Robot, MobaGo::FlatBuffer::MsgCombatPlayerReadyAck>(this, &Robot::handleMsgCombatPlayerReadyAck));

	this->addPlayerMsgHandle(MobaGo::Protocol::Protocol_Combat_E_PROTOCOL_COMBAT_BATTLE_START_ACK,
		new PackMsg<Robot, MobaGo::FlatBuffer::MsgCombatBattleStartAck>(this, &Robot::handleMsgCombatBattleStartAck));

	this->addPlayerMsgHandle(MobaGo::Protocol::Protocol_Combat_E_PROTOCOL_COMBAT_MSG_SERVER_FRAME_SYNC_ACK,
		new PackMsg<Robot, MobaGo::FlatBuffer::MsgFrameSyncDetail>(this, &Robot::handleMsgFrameSyncDetail));

	this->addPlayerMsgHandle(MobaGo::Protocol::Protocol_Combat_E_PROTOCOL_COMBAT_MSG_RECONNECTION_ACK,
		new PackMsg<Robot, MobaGo::FlatBuffer::MsgReconnectionAck>(this, &Robot::handleMsgReconnectionAck));

}

MsgHandleBase * Robot::getMsgHandle(int msg_id)
{
	MessageHandleMap_t::iterator it = m_msg_handle_map.find(msg_id);
	if (it != m_msg_handle_map.end())
	{
		return it->second;
	}

	return NULL;
}

void Robot::logout()
{
	ClientPacket * pack = ClientProtocol::makeMsgLoginReq(m_player_info->account);
	pack->session_id = m_session;
	m_robot_msg_handle->handleOutputPacket(pack);

	m_robot_status = RS_Sent_vrpublic_login_msg;
}

void Robot::loginVrpublic()
{
	ClientPacket * pack = ClientProtocol::makeMsgLoginReq(m_player_info->account);
	pack->session_id = m_session;
	m_robot_msg_handle->handleOutputPacket(pack);

	m_robot_status = RS_Sent_vrpublic_login_msg;
}

void Robot::loginVr()
{
	ClientPacket * pack = ClientProtocol::makeMsgVRLoginReq(m_session, "", m_player_info->account_id, m_vr_session_key);
	m_robot_msg_handle->handleOutputPacket(pack);

	m_robot_status = RS_sent_vr_login_msg;
}

void Robot::loginBattle()
{
	if (m_is_reconnect_battle)
	{
		ClientPacket * pack = ClientProtocol::makeMsgReEnterBattleReq(m_session, m_player_info->account_id);

		m_robot_msg_handle->handleOutputPacket(pack);
	}
	else
	{
		ClientPacket * pack = ClientProtocol::makeMsgEnterBattle(m_session, m_player_info->account_id);

		m_robot_msg_handle->handleOutputPacket(pack);
	}

	m_robot_status = RS_sent_battle_login_msg;
}

