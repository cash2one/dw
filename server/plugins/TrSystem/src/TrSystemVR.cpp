
#include "TrSystemVR.h"
#include "get_entity_macro.h"
#include "get_entity_macro.h"
#include "account.pb.h"
#include "check_macro.h"
#include "account.pb.h"
#include "pool.h"
#include "plugin.h"

TrSystemVR::TrSystemVR()
{
	m_plugin_name = "TrSystemVR";
}

TrSystemVR::~TrSystemVR()
{

}

INPUT_FUN_DEFINE(TrSystemVR, Opcode_E_MsgMatchReq, MsgMatchReq)
{
	uint64_t summoner_guid = this->getSummonerGuidByGateGuid(input_packet->msg_head.guid);
	CK_EQUAL_RETURN(summoner_guid, 0, -1);

	Get_Entity_Summoner(summoner_guid);
	CK_NULL_RETURN(summoner, -1);

	this->updateSummonerToTr(summoner);
	
	vector<uint64_t> group_member;
	group_member.push_back(summoner->guid());

	InterPacket * pack = this->makeSMsgMatchReq(summoner->guid(), input_msg->team_type(), input_msg->match_type(), group_member);
	m_plugin_cfg.sender->sendMsg2TR(pack);

	return 0;
}

INPUT_FUN_DEFINE(TrSystemVR, Opcode_E_MsgCancelMatchReq, EmptyMsg)
{
	uint64_t summoner_guid = this->getSummonerGuidByGateGuid(input_packet->msg_head.guid);
	CK_EQUAL_RETURN(summoner_guid, 0, -1);

	Get_Entity_Summoner(summoner_guid);
	CK_NULL_RETURN(summoner, -1);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgCancelMatchReq, summoner->guid());

	m_plugin_cfg.sender->sendMsg2TR(pack);

	return 0;
}

INPUT_FUN_DEFINE(TrSystemVR, E_MsgMatchAck_Copy, MsgMatchAck)
{
	Get_Entity_Summoner(input_packet->msg_head.guid);
	CK_NULL_RETURN(summoner, -1);

	//if (summoner->match_status() == MS_matching_req)
	{
		summoner->set_match_status(MS_matching);

		InterPacket * pack = input_packet->clone();
		pack->updateHead(summoner->gate_guid(), Opcode_E_MsgMatchAck);
		m_plugin_cfg.sender->sendMsg2Client(pack);
	}

	return 0;
}

INPUT_FUN_DEFINE(TrSystemVR, E_MsgCancelMatchAck_Copy, MsgCancelMatchAck)
{
	Get_Entity_Summoner(input_packet->msg_head.guid);
	CK_NULL_RETURN(summoner, -1);

	//if (summoner->match_status() == MS_matching)
	{
		summoner->set_match_status(MS_base);

		InterPacket * pack = input_packet->clone();
		pack->updateHead(summoner->gate_guid(), Opcode_E_MsgCancelMatchAck);
		m_plugin_cfg.sender->sendMsg2Client(pack);
	}

	return 0;
}

INPUT_FUN_DEFINE(TrSystemVR, E_MsgMatchEnterBattle_Copy, MsgMatchEnterBattle)
{
	PlayerInfo * player_info = this->getPlayerInfo(input_packet->msg_head.guid);
	CK_NULL_RETURN(player_info, -1);

	player_info->match_status = MS_in_battle;

	player_info->battle_server_port = input_msg->server_port();
	player_info->battle_server_ip = input_msg->server_ip() != NULL ? input_msg->server_ip()->c_str() : "";
	player_info->battle_session_key = input_msg->session_key() != NULL ? input_msg->session_key()->c_str() : "";


	Get_Entity_Summoner(input_packet->msg_head.guid);
	if (NULL != summoner)
	{
		InterPacket * pack = input_packet->clone();
		pack->updateHead(summoner->gate_guid(), Opcode_E_MsgMatchEnterBattle);
		m_plugin_cfg.sender->sendMsg2Client(pack);
	}

	return 0;
}

INPUT_FUN_DEFINE(TrSystemVR, E_SMsgBattleEndNotifyVr, SMsgBattleEndNotifyVr)
{
	PlayerInfo * player_info = this->getPlayerInfo(input_packet->msg_head.guid);
	CK_NULL_RETURN(player_info, -1);

	player_info->match_status = MS_base;

	InterPacket * pack = new InterPacket(E_SMsgVrReciveBattleReportNotifyBattle, player_info->summoner_guid);
	m_plugin_cfg.sender->sendMsg2TR(pack);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// notify msg
//////////////////////////////////////////////////////////////////////////
NOTIFY_FUN_DEFINE(TrSystemVR, N_CreateSummoner, EmptyMsg)
{
	this->eventPlayerOnline(ntf.guid);
	return 0;
}

NOTIFY_FUN_DEFINE(TrSystemVR, N_PlayerOnline, EmptyMsg)
{
	this->eventPlayerOnline(ntf.guid);
	return 0;
}

NOTIFY_FUN_DEFINE(TrSystemVR, N_PlayerOffline, EmptyMsg)
{
	this->eventPlayerOffline(ntf.guid);
	return 0;
}

uint64_t TrSystemVR::getSummonerGuidByGateGuid(uint64_t gate_guid)
{
	PackInfo pack(R_GetSummonerGuidByGateGuid, gate_guid, NULL);
	m_plugin_cfg.depot->request(pack);

	return pack.guid;
}

InterPacket * TrSystemVR::makeSMsgMatchReq(MyGuid_t gate_guid, int team_type, int match_type, vector<MyGuid_t> & group_member)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<Vector<uint64_t>> off_group_mem = builder.CreateVector(group_member);
	Offset<SMsgMatchReq> off_req = CreateSMsgMatchReq(builder, team_type, match_type, off_group_mem);
	builder.Finish(off_req);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgMatchReq, gate_guid, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * TrSystemVR::makeMsgReconnectBattle(MyGuid_t gate_guid, int battle_port, const string & battle_ip, const string & battle_session_key)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<String> off_ip = builder.CreateString(battle_ip);
	Offset<String> off_key = builder.CreateString(battle_session_key);

	Offset<MsgReconnectBattle> off_re = CreateMsgReconnectBattle(builder, off_ip, battle_port, off_key);
	builder.Finish(off_re);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgReconnectBattle, gate_guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
}

InterPacket * TrSystemVR::makeMsgVRLoginAck(MyGuid_t gate_guid, int battle_status)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<MsgVRLoginAck> off_ack = CreateMsgVRLoginAck(builder, battle_status);
	builder.Finish(off_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgVRLoginAck, gate_guid, builder.GetSize(), builder.GetBufferPointer());

	return pack;
}

void TrSystemVR::updateSummonerToTr(Entity::summoner * summoner)
{
	Proto::Player player;
	player.set_account_id(summoner->guid());
	player.set_summoner_name(summoner->sum_name());
	string str_buf = player.SerializeAsString();

	InterPacket * pack = new InterPacket();

	pack->setMsgInfo(E_proto_Player, summoner->guid(), str_buf.size(), str_buf.data());

	m_plugin_cfg.sender->sendMsg2TR(pack);
}

TrSystemVR::PlayerInfo * TrSystemVR::getPlayerInfo(uint64_t summoner_guid)
{
	auto it = m_player_info_map.find(summoner_guid);
	if (it != m_player_info_map.end())
	{
		return it->second;
	}

	return NULL;
}

bool TrSystemVR::addPlayerInfo(PlayerInfo * player_info)
{
	auto it = m_player_info_map.find(player_info->summoner_guid);
	if (it != m_player_info_map.end())
	{
		DEF_LOG_ERROR("add player info error, reduplication, <%llu>", player_info->summoner_guid);
		return false;
	}

	m_player_info_map.insert(std::make_pair(player_info->summoner_guid, player_info));

	return true;
}

void TrSystemVR::notifyClientReEnterBattle(PlayerInfo * player_info)
{
	Get_Entity_Summoner(player_info->summoner_guid);
	CK_NULL_RETURN(summoner, );

	InterPacket * pack = this->makeMsgReconnectBattle(summoner->gate_guid(), player_info->battle_server_port, player_info->battle_server_ip, player_info->battle_session_key);
	m_plugin_cfg.sender->sendMsg2Client(pack);
}

void TrSystemVR::eventPlayerOnline(uint64_t summoner_guid)
{
	Get_Entity_Summoner(summoner_guid);
	CK_NULL_RETURN(summoner, );

	PlayerInfo * player_info = this->getPlayerInfo(summoner_guid);
	if (NULL == player_info)
	{
		player_info = new PlayerInfo();
		player_info->summoner_guid = summoner_guid;
		this->addPlayerInfo(player_info);

		InterPacket * pack = this->makeMsgVRLoginAck(summoner->gate_guid(), 1);
		m_plugin_cfg.sender->sendMsg2Client(pack);
	}
	else
	{
		player_info->is_online = true;
		if (MS_in_battle == player_info->match_status)
		{
			InterPacket * pack = this->makeMsgVRLoginAck(summoner->gate_guid(), 2);
			m_plugin_cfg.sender->sendMsg2Client(pack);
			this->notifyClientReEnterBattle(player_info);
		}
		else
		{
			InterPacket * pack = this->makeMsgVRLoginAck(summoner->gate_guid(), 1);
			m_plugin_cfg.sender->sendMsg2Client(pack);
		}
	}
}

void TrSystemVR::eventPlayerOffline(uint64_t summoner_guid)
{
	InterPacket * pack = this->makePlayerOfflineMsg(summoner_guid);
	m_plugin_cfg.sender->sendMsg2TR(pack);
}

InterPacket * TrSystemVR::makePlayerOfflineMsg(uint64_t summoner_guid)
{
	InterPacket * pack = new InterPacket(E_PlayerOffline, summoner_guid);
	return pack;
}
