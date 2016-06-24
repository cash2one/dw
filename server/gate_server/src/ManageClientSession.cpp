
#include "ManageClientSession.h"
#include "opcode.h"
#include "ManageServerSession.h"
#include "Logger.h"
#include "GateConfig.h"

ManageClientSession * ManageClientSession::m_instance = NULL;

ManageClientSession::ManageClientSession()
	: m_client_guid_index(0)
{

}

ManageClientSession::~ManageClientSession()
{

}

ManageClientSession * ManageClientSession::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageClientSession();
	}

	return m_instance;
}

void ManageClientSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(MSG_NEW_CLIENT_SESSION, 0, NULL);
	pack->session_id = session_id;
	this->handleClientPacket(pack);
}
	
void ManageClientSession::handleNewConnectionFailed()
{

}

void ManageClientSession::handleCloseConnection(SessionId session_id, int error_code)
{
	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(MSG_CLOSE_CLIENT_SESSION, 0, NULL);
	pack->session_id = session_id;
	this->handleClientPacket(pack);
}

size_t ManageClientSession::handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	ClientPacketVec_t pack_vec;
	size_t ret = parseClientPacketFromStream(session_id, buffer, buf_len, pack_vec);
	this->handleClientPacket(pack_vec);
	return ret;
}

int ManageClientSession::init(const string & addr, int port, int net_type)
{
	DEF_LOG_INFO("init client session, ip is <%s>, port is <%d>, net type is <%d>", addr.c_str(), port, net_type);

	m_manage_client_ping.init(this, GateConfig::instance()->getClientCfg().ping_interval * 2);

	m_session_server = SessionServer::createSessionServer((NetProtocolType)net_type);
	m_session_server->init(this);

	m_session_server->listen(addr, port);

	return 0;
}

int ManageClientSession::update()
{
	ClientPacketVec_t pack_vec;
	InterPacketVec_t server_pack_vec;

	this->getClientPacket(pack_vec);
	int ret = (int)pack_vec.size();

	InterPacketVec_t client_pack_vec;
	
	for (auto pack : pack_vec)
	{
		if (MSG_NEW_CLIENT_SESSION == pack->msg_head.msg_id)
		{
			this->doHandleNewConnection(pack);
			DEF_LOG_DEBUG("get new client connection <%x>", pack->session_id);
		}
		else if (MSG_CLOSE_CLIENT_SESSION == pack->msg_head.msg_id)
		{
			this->doHandleCloseConnection(pack);
			DEF_LOG_DEBUG("get close client connection <%x>", pack->session_id);
		}
		else
		{
			DEF_LOG_DEBUG("get client msg, msg id is <%hd>, len is <%hd>", pack->msg_head.msg_id, pack->msg_head.msg_len);

			//MyGuid_t guid = this->getSessionGuid(pack->session_id);
			ClientSessionInfo * cs_info = this->getSessionInfo(pack->session_id);
			if (NULL != cs_info)
			{
				InterPacket * inter_pack = makeInterPacket(cs_info->session_guid, pack);
				client_pack_vec.push_back(inter_pack);

				if (!cs_info->send_msg_to_gs)
				{
					cs_info->send_msg_to_gs = true;
				}
			}
			else
			{
				DEF_LOG_DEBUG("do not find client guid by session <%x>", pack->session_id);
			}
		}

		delete pack;
	}

	if (client_pack_vec.size() > 0)
	{
		ManageServerSession::instance()->handleClientPacket(client_pack_vec);
		client_pack_vec.clear();
	}

	// process server pack
	this->getServerPacket(server_pack_vec);
	for (auto pack : server_pack_vec)
	{
		if (E_ClientPingTimeOut == pack->msg_head.msg_id)
		{
			this->handleClientPingTimeout(pack);
		}
		else
		{
			this->handleServerPacket(pack);
		}

		delete pack;
	}
	server_pack_vec.clear();

	return ret;
}

void ManageClientSession::handleClientPacket(ClientPacketVec_t & pack_vec)
{
	boost::mutex::scoped_lock lock(m_client_packet_vec_mtx);
	std::copy(pack_vec.begin(), pack_vec.end(), std::back_inserter(m_client_packet_vec));
}

void ManageClientSession::handleClientPacket(ClientPacket * packet)
{
	boost::mutex::scoped_lock lock(m_client_packet_vec_mtx);
	m_client_packet_vec.push_back(packet);
}

void ManageClientSession::handleServerPacket(InterPacketVec_t & pack_vec)
{
	boost::mutex::scoped_lock lock(m_server_pack_vec_mtx);
	std::copy(pack_vec.begin(), pack_vec.end(), std::back_inserter(m_server_pack_vec));
}

void ManageClientSession::handleServerPacket(InterPacket * packet)
{
	SessionId session_id = this->getSessionIdByGuid(packet->msg_head.guid);
	if (session_id > 0)
	{
		DEF_LOG_DEBUG("get server msg, msg id is <%hd>, len is <%hd>, guid is <%llu>", packet->msg_head.msg_id, packet->msg_head.msg_len, packet->msg_head.guid);

		//ClientPacket * pack = makeClientPacket(packet);
		ServerPacket * pack = makeServerPacket(packet);
		m_session_server->handleOutputStream(session_id, pack->getStream(), pack->getStreamSize());
		pack->cleanStream();
		delete pack;
	}
	else
	{
		DEF_LOG_DEBUG("do not get session by guid <%llu>, msg id is <%hd>, msg len is <%hd>", packet->msg_head.guid, packet->msg_head.msg_id, packet->msg_head.msg_len);
	}
}

void ManageClientSession::pingTimeoutNotify(ClientGuidSet_t & client_guid_set)
{
	InterPacketVec_t pack_vec;
	for (auto guid : client_guid_set)
	{
		InterPacket * pack = new InterPacket();
		pack->setMsgInfo(E_ClientPingTimeOut, guid);
		pack_vec.push_back(pack);
	}
	this->handleServerPacket(pack_vec);
}

void ManageClientSession::handleClientPingTimeout(InterPacket * packet)
{
	// todo
	DEF_LOG_ERROR("client ping timeout <%llu>", packet->msg_head.guid);

	SessionId session = this->getSessionIdByGuid(packet->msg_head.guid);
	if (NULL != session)
	{
		this->removeClientSession(session);
	}
	else
	{
		DEF_LOG_ERROR("client ping timeout, remove the session, but did not find it in map <%llu>", packet->msg_head.guid);
	}
}

void ManageClientSession::getServerPacket(InterPacketVec_t & pack_vec)
{
	boost::mutex::scoped_lock lock(m_server_pack_vec_mtx);
	std::copy(m_server_pack_vec.begin(), m_server_pack_vec.end(), std::back_inserter(pack_vec));
	m_server_pack_vec.clear();
}

void ManageClientSession::getClientPacket(ClientPacketVec_t & pack_vec)
{
	boost::mutex::scoped_lock lock(m_client_packet_vec_mtx);
	std::copy(m_client_packet_vec.begin(), m_client_packet_vec.end(), std::back_inserter(pack_vec));
	m_client_packet_vec.clear();
}

MyGuid_t ManageClientSession::getSessionGuid(SessionId session_id)
{
	ClientSessionInfoMap_t::iterator it = m_client_session_info_map.find(session_id);
	if (it != m_client_session_info_map.end())
	{
		return it->second->session_guid;
	}

	return 0;
}

ClientSessionInfo * ManageClientSession::getSessionInfo(SessionId session_id)
{
	ClientSessionInfoMap_t::iterator it = m_client_session_info_map.find(session_id);
	if (it != m_client_session_info_map.end())
	{
		return it->second;
	}

	return NULL;
}

SessionId ManageClientSession::getSessionIdByGuid(MyGuid_t guid)
{
	ClientGuidInfoMap_t::iterator it = m_client_guid_info_map.find(guid);
	if (it != m_client_guid_info_map.end())
	{
		return it->second->session_id;
	}

	return 0;
}

void ManageClientSession::doHandleNewConnection(ClientPacket * pack)
{
	++m_client_guid_index;
	ClientSessionInfo cs_info(pack->session_id, m_client_guid_index);
	this->addClientSession(cs_info);

	m_manage_client_ping.addClient(cs_info.session_guid);

	DEF_LOG_DEBUG("assign new client session guid <%llu>, session is <%x>", m_client_guid_index, pack->session_id);
}

void ManageClientSession::doHandleCloseConnection(ClientPacket * pack)
{
	this->removeClientSession(pack->session_id);
}

void ManageClientSession::addClientSession(const ClientSessionInfo & cs_info)
{
	ClientSessionInfo * si = new ClientSessionInfo();
	*si = cs_info;

	auto it = m_client_session_info_map.find(cs_info.session_id);
	if (it != m_client_session_info_map.end())
	{
		DEF_LOG_ERROR("add new session, but find it in map <%x>", cs_info.session_id);
		this->removeClientSession(cs_info.session_id);
	}

	m_client_session_info_map[cs_info.session_id] = si;
	m_client_guid_info_map[cs_info.session_guid] = si;
}

void ManageClientSession::removeClientSession(SessionId session_id)
{
	//MyGuid_t guid = this->getSessionGuid(session_id);
	ClientSessionInfo * cs_info = this->getSessionInfo(session_id);
	if (NULL == cs_info)
	{
		DEF_LOG_DEBUG("do not find session where remove <%x>", session_id);
		return;
	}

	m_manage_client_ping.removeClient(cs_info->session_guid);

	if (cs_info->send_msg_to_gs)
	{
		ManageServerSession::instance()->notifyServerSocketClosed(cs_info->session_guid);
	}

	DEF_LOG_INFO("start to remove session , guid is <%llu>, session is <%x>", cs_info->session_guid, session_id);
	{
		auto it = m_client_session_info_map.find(session_id);
		if (it != m_client_session_info_map.end())
		{
			delete it->second;
			m_client_session_info_map.erase(it);
		}
	}
	
	{
		auto it = m_client_guid_info_map.find(cs_info->session_guid);
		if (it != m_client_guid_info_map.end())
		{
			m_client_guid_info_map.erase(it);
		}
	}
}

