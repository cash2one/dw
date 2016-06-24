
#include "ManageClientSession.h"
#include "ManageMatch.h"

ManageClientSession::ManageClientSession()
	: m_manage_server_pump(NULL)
{

}

ManageClientSession::~ManageClientSession()
{

}

ManageClientSession * ManageClientSession::instance()
{
	return Singleton<ManageClientSession>::instance();
}

int ManageClientSession::init(const VrCfg & vr_cfg)
{
	m_vr_cfg = vr_cfg;

	m_manage_server_pump = MessageServerPump<InterPacket>::createMessageServerPump();
	m_manage_server_pump->init(this, NPT_RAKNET);
	m_manage_server_pump->listen(m_vr_cfg.ip_addr, m_vr_cfg.port);

	this->registerMsgHandle();

	return 0;
}

int ManageClientSession::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_manage_server_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	this->updateMatch();

	return dispatch_pack_num;
}

void ManageClientSession::handleOutputPacket(InterPacket * packet)
{
	m_manage_server_pump->handleOutputStream(packet->session_id, packet->getStream(), packet->getStreamSize());
	packet->cleanStream();
	delete packet;
}

void ManageClientSession::handleNewConnection(SessionId session_id)
{


}

void ManageClientSession::handleCloseConnection(SessionId session_id)
{

}

int ManageClientSession::handleSMsgVrRegisterTr(InterPacket * input_packet, const SMsgVrRegisterTr * input_msg)
{
	this->addVr(input_packet->session_id, input_msg->area_id(), input_msg->server_id());
	return 0;
}

int ManageClientSession::handleSMsgMatchReq(InterPacket * input_packet, const SMsgMatchReq * input_msg)
{
	DEF_LOG_ERROR("recive match msg, <%d, %d>", input_msg->team_type(), input_msg->match_type());
	ManageMatch * manage_match = this->getManageMatch(input_msg->team_type(), input_msg->match_type());
	if (NULL == manage_match)
	{
		DEF_LOG_ERROR("do not find manage match, <%d, %d>", input_msg->team_type(), input_msg->match_type());
		return -1;
	}
	//ManageMatch::instance()->handleMatch(input_packet->msg_head.guid, input_packet->session_id);
	manage_match->handleMatch(input_packet->msg_head.guid, input_packet->session_id);
	return 0;
}

int ManageClientSession::handleMsgCancelMatchReq(InterPacket * input_packet, const SMsgMatchReq * input_msg)
{
	//ManageMatch::instance()->handleMatchCancel(input_packet->msg_head.guid);
	return 0;
}

void ManageClientSession::registerMsgHandle()
{
	m_manage_server_pump->addMsgHandle(E_SMsgVrRegisterTr, new PacketHandle<ManageClientSession, InterPacket, SMsgVrRegisterTr>(this, &ManageClientSession::handleSMsgVrRegisterTr));

	m_manage_server_pump->addMsgHandle(E_SMsgMatchReq, new PacketHandle<ManageClientSession, InterPacket, SMsgMatchReq>(this, &ManageClientSession::handleSMsgMatchReq));

	m_manage_server_pump->addMsgHandle(Opcode_E_MsgCancelMatchReq, new PacketHandle<ManageClientSession, InterPacket, SMsgMatchReq>(this, &ManageClientSession::handleMsgCancelMatchReq));
}

void ManageClientSession::addVr(SessionId session_id, int area_id, int gs_id)
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

void ManageClientSession::removeVr(int area_id, int gs_id)
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

VrInfo * ManageClientSession::getVr(SessionId session_id)
{
	auto it = m_vr_session_map.find(session_id);
	if (it != m_vr_session_map.end())
	{
		return it->second;
	}

	return NULL;
}

ManageMatch * ManageClientSession::getManageMatch(int team_type, int match_type)
{
	int inter_match_type = match_type;
	auto it = m_manage_match_map.find(inter_match_type);
	if (it != m_manage_match_map.end())
	{
		return it->second;
	}

	ManageMatch * ret = new ManageMatch();
	ret->init(inter_match_type);
	m_manage_match_map[inter_match_type] = ret;

	return ret;
}

void ManageClientSession::updateMatch()
{
	for (auto it : m_manage_match_map)
	{
		ManageMatch * match = it.second;
		match->update();
	}
}

