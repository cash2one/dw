
#include "ManageVrSession.h"
#include "ManageMatch.h"
#include "ManagePlayerTrInfo.h"
#include "ManageBattleSession.h"
#include "check_macro.h"

ManageVrSession::ManageVrSession()
	: m_manage_server_pump(NULL)
{

}

ManageVrSession::~ManageVrSession()
{

}

ManageVrSession * ManageVrSession::instance()
{
	return Singleton<ManageVrSession>::instance();
}

int ManageVrSession::init(const VrCfg & vr_cfg)
{
	m_vr_cfg = vr_cfg;

	m_manage_server_pump = MessageServerPump<InterPacket>::createMessageServerPump();
	m_manage_server_pump->init(this, NPT_TCP);
	m_manage_server_pump->listen(m_vr_cfg.ip_addr, m_vr_cfg.port);

	this->registerMsgHandle();

	return 0;
}

int ManageVrSession::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_manage_server_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	this->updateMatch();

	return (int)dispatch_pack_num;
}

void ManageVrSession::handleOutputPacket(InterPacket * packet)
{
	DEF_LOG_DEBUG("send msg to vr, guid is <%llu>, msg id is <%hd>, len is <%hd>", packet->msg_head.guid, packet->msg_head.msg_id, packet->msg_head.msg_len);

	m_manage_server_pump->handleOutputStream(packet->session_id, packet->getStream(), packet->getStreamSize());
	packet->cleanStream();
	delete packet;
}

void ManageVrSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{


}

void ManageVrSession::handleCloseConnection(SessionId session_id)
{

}

int ManageVrSession::handleSMsgVrRegisterTr(InterPacket * input_packet, const SMsgVrRegisterTr * input_msg)
{
	this->addVr(input_packet->session_id, input_msg->area_id(), input_msg->server_id());
	return 0;
}

int ManageVrSession::handleSMsgMatchReq(InterPacket * input_packet, const SMsgMatchReq * input_msg)
{
	DEF_LOG_DEBUG("recive match msg, <%d, %d>", input_msg->team_type(), input_msg->match_type());
	ManageMatch::instance()->handleMatch(input_packet, input_msg);
	//ManageMatch * manage_match = this->getManageMatch(input_msg->team_type(), input_msg->match_type());
	//if (NULL == manage_match)
	//{
	//	DEF_LOG_ERROR("do not find manage match, <%d, %d>", input_msg->team_type(), input_msg->match_type());
	//	return -1;
	//}
	////ManageMatch::instance()->handleMatch(input_packet->msg_head.guid, input_packet->session_id);
	//manage_match->handleMatch(input_packet->msg_head.guid, input_packet->session_id);
	return 0;
}

int ManageVrSession::handleMsgCancelMatchReq(InterPacket * input_packet, const EmptyMsg * input_msg)
{
	ManageMatch::instance()->handleMatchCancel(input_packet);
	return 0;
}

int ManageVrSession::handleProtoPlayer(InterPacket * input_packet, const ProtoAgentMsg * input_msg)
{
	bool is_new_player = false;
	PlayerTrInfo * player_info = ManagePlayerTrInfo::instance()->getPlayer(input_packet->msg_head.guid);
	if (NULL == player_info)
	{
		player_info = new PlayerTrInfo();
		player_info->setAccountId(input_packet->msg_head.guid);
		player_info->setSessionId(input_packet->session_id);
		is_new_player = true;
	}

	if (!player_info->getPlayer().ParseFromArray(input_packet->getBody(), input_packet->msg_head.msg_len))
	{
		// error
		DEF_LOG_INFO("get player info : <%llu>", input_packet->msg_head.guid);
		DEF_LOG_ERROR("failed to parse Player");
		if (is_new_player)
		{
			delete player_info;
		}
		return -1;
	}

	if (is_new_player)
	{
		ManagePlayerTrInfo::instance()->addPlayer(player_info);
	}

	DEF_LOG_INFO("get player info : <%llu, %s>", input_packet->msg_head.guid, player_info->getPlayer().summoner_name().c_str());
	
	return 0;
}

int ManageVrSession::handlePlayerOffline(InterPacket * input_packet, const EmptyMsg * input_msg)
{
	ManageMatch::instance()->handlePlayerOffline(input_packet);

	return 0;
}

int ManageVrSession::handleSMsgVrReciveBattleReportNotifyBattle(InterPacket * input_packet, const EmptyMsg * input_msg)
{
	DEF_LOG_DEBUG("recv SMsgVrReciveBattleReportNotifyBattle, guid is <%llu>", input_packet->msg_head.guid);
	PlayerTrInfo * player_info = ManagePlayerTrInfo::instance()->getPlayer(input_packet->msg_head.guid);
	CK_NULL_RETURN(player_info, -1);

	InterPacket * pack = input_packet->clone();
	pack->session_id = player_info->getBattleSession();
	ManageBattleSession::instance()->handleOutputPacket(pack);

	player_info->setBattleSession(NULL);

	return 0;
}

void ManageVrSession::registerMsgHandle()
{
	m_manage_server_pump->addMsgHandle(E_SMsgVrRegisterTr, new PacketHandle<ManageVrSession, InterPacket, SMsgVrRegisterTr>(this, &ManageVrSession::handleSMsgVrRegisterTr));

	m_manage_server_pump->addMsgHandle(E_SMsgMatchReq, new PacketHandle<ManageVrSession, InterPacket, SMsgMatchReq>(this, &ManageVrSession::handleSMsgMatchReq));

	m_manage_server_pump->addMsgHandle(Opcode_E_MsgCancelMatchReq, new PacketHandle<ManageVrSession, InterPacket, EmptyMsg>(this, &ManageVrSession::handleMsgCancelMatchReq));

	m_manage_server_pump->addMsgHandle(E_proto_Player, new PacketHandle<ManageVrSession, InterPacket, ProtoAgentMsg>(this, &ManageVrSession::handleProtoPlayer));

	m_manage_server_pump->addMsgHandle(E_PlayerOffline, new PacketHandle<ManageVrSession, InterPacket, EmptyMsg>(this, &ManageVrSession::handlePlayerOffline));

	m_manage_server_pump->addMsgHandle(E_SMsgVrReciveBattleReportNotifyBattle, new PacketHandle<ManageVrSession, InterPacket, EmptyMsg>(this, &ManageVrSession::handleSMsgVrReciveBattleReportNotifyBattle));

	
}

void ManageVrSession::addVr(SessionId session_id, int area_id, int gs_id)
{
	AreaInfo * area_info = NULL;
	{
		auto it = m_area_info_map.find(area_id);
		if (m_area_info_map.end() == it)
		{
			area_info = new AreaInfo();
			area_info->area_id = area_id;
		}
		else
		{
			area_info = it->second;
		}
	}

	auto it = area_info->vr_info_map.find(gs_id);
	if (it != area_info->vr_info_map.end())
	{
		DEF_LOG_ERROR("get reduplication vr, area id is <%d> gs id is <%d>, session id is <%x>", area_id, gs_id, session_id);
		this->removeVr(area_id, gs_id);
	}

	VrInfo * vr_info = new VrInfo();
	vr_info->area_id = area_id;
	vr_info->gs_id = gs_id;
	vr_info->session_id = session_id;

	area_info->vr_info_map[gs_id] = vr_info;
	m_vr_session_map[session_id] = vr_info;
}

void ManageVrSession::removeVr(int area_id, int gs_id)
{
	auto it = m_area_info_map.find(area_id);
	if (m_area_info_map.end() == it)
	{
		DEF_LOG_ERROR("do not find area info while remove vr, area id is <%d>, gs id is <%d>", area_id, gs_id);
		return;
	}

	auto vr_it = it->second->vr_info_map.find(gs_id);
	if (it->second->vr_info_map.end() == vr_it)
	{
		DEF_LOG_ERROR("do not find vr info while remove vr, area id is <%d>, gs id is <%d>", area_id, gs_id);
		return;
	}

	VrInfo * vr_info = vr_it->second;

	it->second->vr_info_map.erase(vr_it);
	m_vr_session_map.erase(vr_info->session_id);

	delete vr_info;
}

VrInfo * ManageVrSession::getVr(SessionId session_id)
{
	auto it = m_vr_session_map.find(session_id);
	if (it != m_vr_session_map.end())
	{
		return it->second;
	}

	return NULL;
}

ManageMatch * ManageVrSession::getManageMatch(int team_type, int match_type)
{
	//int inter_match_type = match_type;
	//auto it = m_manage_match_map.find(inter_match_type);
	//if (it != m_manage_match_map.end())
	//{
	//	return it->second;
	//}

	//ManageMatch * ret = new ManageMatch();
	//ret->init(inter_match_type);
	//m_manage_match_map[inter_match_type] = ret;

	return NULL;
}

void ManageVrSession::updateMatch()
{
	for (auto it : m_manage_match_map)
	{
		ManageMatch * match = it.second;
		match->update();
	}
}

