
#include "ManageVrSession.h"
#include "VrMessageManager.h"
#include "Logger.h"
#include "opcode.h"
#include "ManageServerList.h"

ManageVrSession * ManageVrSession::m_instance = NULL;

ManageVrSession::ManageVrSession()
{

}

ManageVrSession::~ManageVrSession()
{

}

ManageVrSession * ManageVrSession::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageVrSession();
	}

	return m_instance;
}

int ManageVrSession::init(const string & ip_addr, int port)
{
	DEF_LOG_INFO("init manage vr session, ip is <%s:%d>", ip_addr.c_str(), port);

	m_session_server = SessionServer::createSessionServer(NPT_TCP);
	m_session_server->init(this);
	m_session_server->listen(ip_addr, port);

	return 0;
}

VrSession * ManageVrSession::RandGetVrServer()
{
	return NULL;
}

VrSession * ManageVrSession::GetVrServerByIndex(int area_id, int gs_index)
{
	return NULL;
}

void ManageVrSession::addNewVr(SessionId session_id, int area_id, int gs_id, const string & gate_ip_addr, int gate_port)
{
	//boost::mutex::scoped_lock lock(m_vr_session_info_map_mtx);

	VrSession * vr_session = new VrSession();
	vr_session->session_id = session_id;
	vr_session->area_id = area_id;
	vr_session->gs_index = gs_id;
	vr_session->gate_ip_addr = gate_ip_addr;
	vr_session->gate_port = gate_port;

	m_vr_session_info_map[session_id] = vr_session;

	AreaInfo * area_info = NULL;

	auto it = m_area_info_map.find(area_id);
	if (m_area_info_map.end() == it)
	{
		area_info = new AreaInfo();
		area_info->area_id = area_id;
		m_area_info_map[area_id] = area_info;
	}
	else
	{
		area_info = it->second;
	}

	area_info->vr_session_map[gs_id] = vr_session;

	ManageServerList::instance()->vrIsOnline(session_id, area_id, gs_id, gate_ip_addr, gate_port);
}

void ManageVrSession::removeVr(SessionId session_id)
{
	//boost::mutex::scoped_lock lock(m_vr_session_info_map_mtx);

	auto it = m_vr_session_info_map.find(session_id);
	if (m_vr_session_info_map.end() == it)
	{
		return;
	}

	VrSession * vr_session = it->second;
	m_vr_session_info_map.erase(it);

	auto area_it = m_area_info_map.find(vr_session->area_id);
	if (m_area_info_map.end() == area_it)
	{
		// error
		DEF_LOG_ERROR("failed to get area info by <%d>", vr_session->area_id);
		return;
	}

	AreaInfo * area_info = area_it->second;
	area_info->vr_session_map.erase(vr_session->gs_index);

	ManageServerList::instance()->vrIsOffline(vr_session->area_id, vr_session->gs_index);

	delete vr_session;
}

void ManageVrSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{

}

void ManageVrSession::handleNewConnectionFailed()
{

}

void ManageVrSession::handleCloseConnection(SessionId session_id, int error_code)
{
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(MSG_CLOSE_CLIENT_SESSION, 0);

	VrMessageManager::instance()->handleVrPacket(pack);
}

size_t ManageVrSession::handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	InterPacketVec_t pack_vec;
	size_t ret = parseInterPacketFromStream(session_id, buffer, buf_len, pack_vec);

	VrMessageManager::instance()->handleVrPacket(pack_vec);
	return ret;
}

void ManageVrSession::handleOutputPacket(InterPacket * pack)
{
	m_session_server->handleOutputStream(pack->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();
}
