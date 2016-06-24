
#include "PlayerLoginVR.h"
#include "Logger.h"
#include "check_macro.h"
#include "pool.h"
#include "Logger.h"
#include "MessageMaker.h"
#include "account.pb.h"
#include "opcode.h"
#include "get_entity_macro.h"
#include "pluginDepot.h"

PlayerLoginVR::PlayerLoginVR()
	: m_vrp_session_id(NULL)
	, m_gate_port(0)
	, m_gate_net_type(0)
	, m_area_id(0)
	, m_server_id(0)
{
	m_plugin_name = "PlayerLoginVR";
}

PlayerLoginVR::~PlayerLoginVR()
{

}

INPUT_FUN_DEFINE(PlayerLoginVR, E_SMsgGateRegister, SMsgGateRegister)
{
	m_gate_ip = NULL != input_msg->ip_addr() ? input_msg->ip_addr()->c_str() : "";

	m_gate_port = input_msg->port();
	
	this->registerGateInfoToVrpublic();
	return 0;
}

INPUT_FUN_DEFINE(PlayerLoginVR, E_SMsgVrpublicNotifyVrLoginInfo, SMsgVrpublicNotifyVrLoginInfo)
{
	string user_name = input_msg->user_name() != NULL ? input_msg->user_name()->c_str() : "";
	string login_key = input_msg->login_key() != NULL ? input_msg->login_key()->c_str() : "";

	this->addPlayerLoginInfo(input_msg->user_guid(), user_name, login_key);


	flatbuffers::FlatBufferBuilder builder;
	InterPacket * ack_pack = new InterPacket();

	string gate_ip = input_msg->gate_ip() != NULL ? input_msg->gate_ip()->c_str() : "";
	flatbuffers::Offset<flatbuffers::String> off_gate_ip = builder.CreateString(gate_ip);
	string announcement = input_msg->announcement() != NULL ? input_msg->announcement()->c_str() : "";
	flatbuffers::Offset<flatbuffers::String> off_announcement = builder.CreateString(announcement);

	flatbuffers::Offset<SMsgVrpublicNotifyVrLoginInfoAck> off_notify = CreateSMsgVrpublicNotifyVrLoginInfoAck(builder, input_msg->gs_id(), off_gate_ip, input_msg->gate_port(), off_announcement);
	builder.Finish(off_notify);


	ack_pack->setMsgInfo(E_SMsgVrpublicNotifyVrLoginInfoAck, input_packet->msg_head.guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	ack_pack->session_id = input_packet->session_id;

	m_plugin_cfg.sender->sendMsg2VRPublic(ack_pack);

	return 0;
}

INPUT_FUN_DEFINE(PlayerLoginVR, E_VrModuleVrpConnected, EmptyMsg)
{
	m_vrp_session_id = input_packet->session_id;
	this->registerGateInfoToVrpublic();

	return 0;
}

INPUT_FUN_DEFINE(PlayerLoginVR, E_VrModuleVrpConnectionClosed, EmptyMsg)
{
	if (input_packet->session_id == m_vrp_session_id)
	{
		m_vrp_session_id = NULL;
	}

	return 0;
}

INPUT_FUN_DEFINE(PlayerLoginVR, E_MSG_PLAYER_CLOSE_CONNECTION, EmptyMsg)
{
	PlayerLoginInfo * player_login_info = this->getPlayerByGateGuid(input_packet->msg_head.guid);
	CK_NULL_RETURN(player_login_info, -1);

	Get_Entity_Summoner(player_login_info->summoner_id);
	CK_NULL_RETURN(summoner, -1);

	this->notifyPlayerOffline(summoner->guid());

	m_plugin_cfg.pool->remove(summoner->guid());

	return 0;
}

INPUT_FUN_DEFINE(PlayerLoginVR, Opcode_E_MsgVRLoginReq, MsgVRLoginReq)
{
	DEF_LOG_DEBUG("");

	string session_key = input_msg->session_key() != NULL ? input_msg->session_key()->str() : "";

	PlayerLoginInfo * player_login_info = this->getPlayerLoginInfo(input_msg->account_id());
	if (NULL == player_login_info)
	{
		DEF_LOG_ERROR("failed to get player login info, account id is <%llu>, session_code is <%s>", input_msg->account_id(), session_key.c_str());
		return -1;
	}

	player_login_info->gate_guid = input_packet->msg_head.guid;
	this->addGateGuid(input_packet->msg_head.guid, player_login_info);

	//CK_FALSE_RETURN(PS_base == player_login_info->player_status, -1);

	if (session_key != player_login_info->login_key)
	{
		// error
		DEF_LOG_ERROR("player login validation failed, account id is  <%llu>, user name is <%s>, client key is <%s>, vrp key is <%s>", player_login_info->summoner_id, player_login_info->user_name.c_str(), session_key.c_str(), player_login_info->login_key.c_str());
		return -1;
	}

	DEF_LOG_DEBUG("player login success, account id is <%llu>, name is <%s>", player_login_info->summoner_id, player_login_info->user_name.c_str());

	player_login_info->player_status = PS_loading;
	Get_Entity_Summoner(player_login_info->summoner_id);
	if (NULL != summoner)
	{
		player_login_info->player_status = PS_online;
		this->notifyPlayerOnline(summoner->guid());
		return 0;
	}

	m_plugin_cfg.pool->load(player_login_info->summoner_id, std::bind(&PlayerLoginVR::loadDataCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	return 0;
}

INPUT_FUN_DEFINE(PlayerLoginVR, Opcode_E_MsgCreateSummonerReq, MsgCreateSummonerReq)
{
	PlayerLoginInfo * player_login_info = this->getPlayerByGateGuid(input_packet->msg_head.guid);
	CK_NULL_RETURN(player_login_info, -1);

	string summoner_name = input_msg->summoner_name() != NULL ? input_msg->summoner_name()->c_str() : "";

	CK_TRUE_PRINT(summoner_name.size() == 0);

	//if (!BaseFunc::IllegalWordCheck::IsLegalString(summoner_name))
	//{
	//	MessageMaker::sendErrorPacket(packet->session_id, packet->msg_head.guid, MobaGo::FlatBuffer::Error_IllegalChar);
	//	return 0;
	//}

	// to do
	// check special character

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgCreateSummonerAck, input_packet->msg_head.guid, 0, (const char *)0);
	m_plugin_cfg.sender->sendMsg2Client(pack);

	Entity::summoner * entity_sum = new Entity::summoner();
	entity_sum->set_guid(player_login_info->summoner_id);
	entity_sum->set_gate_guid(player_login_info->gate_guid);
	entity_sum->set_sum_name(summoner_name);
	//sumnameÒ²Òª´«£¬ todo list

	m_plugin_cfg.pool->add(entity_sum->guid(), entity_sum);

	this->notifyCreateSummoner(player_login_info->summoner_id);
	this->notifyPlayerOnline(player_login_info->summoner_id);

	return 0;
}

REQUEST_FUN_DEFINE(PlayerLoginVR, R_GetSummonerGuidByGateGuid, EmptyMsg)
{
	uint64_t gate_guid = req.guid;
	req.guid = 0;

	PlayerLoginInfo * player_login_info = this->getPlayerByGateGuid(gate_guid);
	CK_NULL_RETURN(player_login_info, -1);

	req.guid = player_login_info->summoner_id;

	return 0;
}

REQUEST_FUN_DEFINE(PlayerLoginVR, R_GetGateGuidBySummonerId, EmptyMsg)
{
	uint64_t summoner_id = req.guid;
	req.guid = 0;

	PlayerLoginInfo * player_login_info = this->getPlayerLoginInfo(summoner_id);
	CK_NULL_RETURN(player_login_info, -1);

	req.guid = player_login_info->gate_guid;

	return 0;
}

NOTIFY_FUN_DEFINE(PlayerLoginVR, N_VrModuleNotifyAreaId, EmptyMsg)
{
	uint32_t area_id = 0;
	uint32_t server_id = 0;
	memcpy(&area_id, (char *)&ntf.guid, sizeof(area_id));
	memcpy(&server_id, (char *)&ntf.guid + sizeof(area_id), sizeof(server_id));
	m_area_id = (int)area_id;
	m_server_id = (int)server_id;

	DEF_LOG_INFO("get area id and server id <%d, %d>", m_area_id, m_server_id);

	return 0;
}

void PlayerLoginVR::loadDataCallback(uint32_t req_id, uint64_t entity_guid, MSG_TYPE * entity_msg)
{
	PlayerLoginInfo * player_login_info = this->getPlayerLoginInfo(entity_guid);
	CK_NULL_RETURN(player_login_info, );

	if (NULL == entity_msg)
	{
		// should create summoner
		InterPacket * pack = this->makeMsgError(player_login_info->gate_guid, Error_NeedCreateSummoner);
		m_plugin_cfg.sender->sendMsg2Client(pack);
	}
	else
	{
		// load summoner success
		Entity::summoner * summoner = (Entity::summoner *)entity_msg;
		summoner->set_gate_guid(player_login_info->gate_guid);

		m_plugin_cfg.pool->add(summoner->guid(), summoner, false);

		InterPacket * pack = this->makeMsgReqSummonerInfoAck(player_login_info->gate_guid, summoner->sum_name());
		this->notifyPlayerOnline(player_login_info->summoner_id);
		m_plugin_cfg.sender->sendMsg2Client(pack);

		this->notifyPlayerOnline(summoner->guid());
	}
}

void PlayerLoginVR::notifyCreateSummoner(uint64_t summoner_guid)
{
	EmptyMsg * notify_msg = NULL;
	// to be set with more info. todo list
	this->notifyPluginMsg(N_CreateSummoner, summoner_guid, notify_msg);
}

void PlayerLoginVR::notifyPlayerOnline(uint64_t summoner_guid)
{
	EmptyMsg * notify_msg = NULL;
	this->notifyPluginMsg(N_PlayerOnline, summoner_guid, notify_msg);
}

void PlayerLoginVR::notifyPlayerOffline(uint64_t summoner_guid)
{
	EmptyMsg * notify_msg = NULL;
	this->notifyPluginMsg(N_PlayerOffline, summoner_guid, notify_msg);
}

template<class _MsgType>
void PlayerLoginVR::notifyPluginMsg(int msg_id, uint64_t summoner_guid, _MsgType * notify_msg)
{
	//string buf = notify_msg->SerializeAsString();
	//InterPacket * pack = new InterPacket();
	//if (buf.size() == 0)
	//{
	//	pack->setMsgInfo(msg_id, summoner_guid);
	//}
	//else
	//{
	//	pack->setMsgInfo(msg_id, summoner_guid, buf.size(), buf.data());
	//}

	PackInfo pack(msg_id, summoner_guid, NULL);

	m_plugin_cfg.depot->notify(pack);

}

void PlayerLoginVR::registerGateInfoToVrpublic()
{
	if (m_gate_ip.size() == 0 || NULL == m_vrp_session_id)
	{
		return;
	}

	DEF_LOG_INFO("report gate info to vrpublic, gate ip is <%s>, gate port is <%d>", m_gate_ip.c_str(), m_gate_port);
	InterPacket * pack = PlayerLoginVR::makeSMsgVrRegisterVrpublic(NULL, m_area_id, m_server_id, m_gate_ip, m_gate_port);
	m_plugin_cfg.sender->sendMsg2VRPublic(pack);

}

void PlayerLoginVR::addPlayerLoginInfo(uint64_t summoner_id, const string & user_name, const string & login_key)
{
	PlayerLoginInfo * player_info = NULL;
	auto it = m_player_login_info_map.find(summoner_id);
	if (it != m_player_login_info_map.end())
	{
		it->second->login_key = login_key;
	}
	else
	{
		PlayerLoginInfo * player_info = new PlayerLoginInfo(summoner_id, user_name, login_key);
		m_player_login_info_map.insert(std::make_pair(summoner_id, player_info));
	}
}

PlayerLoginVR::PlayerLoginInfo * PlayerLoginVR::getPlayerLoginInfo(uint64_t summoner_id)
{
	auto it = m_player_login_info_map.find(summoner_id);
	if (it != m_player_login_info_map.end())
	{
		return it->second;
	}

	return NULL;
}

void PlayerLoginVR::addGateGuid(uint64_t gate_guid, PlayerLoginInfo * player_login_info)
{
	auto it = m_player_login_info_gate_guid_map.find(gate_guid);
	CK_TRUE_RETURN(m_player_login_info_gate_guid_map.end() != it, );

	player_login_info->gate_guid = gate_guid;
	m_player_login_info_gate_guid_map.insert(std::make_pair(gate_guid, player_login_info));
}

PlayerLoginVR::PlayerLoginInfo * PlayerLoginVR::getPlayerByGateGuid(uint64_t gate_guid)
{
	auto it = m_player_login_info_gate_guid_map.find(gate_guid);
	CK_TRUE_RETURN(m_player_login_info_gate_guid_map.end() == it, NULL);

	return it->second;
}

InterPacket * PlayerLoginVR::makeSMsgVrRegisterVrpublic(SessionId session_id, int area_id, int server_id, const string & gate_ip_addr, int gate_port)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> off_ip_addr = builder.CreateString(gate_ip_addr);
	flatbuffers::Offset<SMsgVrRegisterVrpublic> off_set = CreateSMsgVrRegisterVrpublic(builder, area_id, server_id, off_ip_addr, gate_port);
	builder.Finish(off_set);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVrRegisterVrpublic, 0, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * PlayerLoginVR::makeMsgError(uint64_t gate_guid, int error_code)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<MsgError> off_err = CreateMsgError(builder, error_code);
	builder.Finish(off_err);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgError, gate_guid, builder.GetSize(), builder.GetBufferPointer());

	return pack;
}

InterPacket * PlayerLoginVR::makeMsgReqSummonerInfoAck(uint64_t gate_guid, const string & summoner_name)
{
	FlatBufferBuilder builder;
	Offset<String> off_name = builder.CreateString(summoner_name.c_str());
	Offset<MsgReqSummonerInfoAck> off_ack = CreateMsgReqSummonerInfoAck(builder, off_name);
	builder.Finish(off_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgReqSummonerInfoAck, gate_guid, builder.GetSize(), builder.GetBufferPointer());

	return pack;
}
