
#include "ManageServerSession.h"
#include "ManageClientSession.h"
#include "GateConfig.h"
#include "Logger.h"

ManageServerSession * ManageServerSession::m_instance = nullptr;

ManageServerSession::ManageServerSession()
	: m_session_server(NULL)
	, m_thread(NULL)
{

}

ManageServerSession::~ManageServerSession()
{

}

ManageServerSession * ManageServerSession::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageServerSession();
	}

	return m_instance;
}

int ManageServerSession::init(const string & ip_addr, int port)
{
	DEF_LOG_INFO("init server session, ip is <%s>, port is <%d>", ip_addr.c_str(), port);

	m_session_server = SessionServer::createSessionServer(NPT_TCP);
	m_session_server->init(this);
	m_session_server->listen(ip_addr, port);

	boost::function0<int> f = boost::bind(&ManageServerSession::svc, this);
	m_thread = new boost::thread(f);

	return 0;
}

void ManageServerSession::handleClientPacket(InterPacket * inter_pack)
{
	boost::mutex::scoped_lock lock(m_client_packet_vec_mtx);
	m_client_packet_vec.push_back(inter_pack);
}

void ManageServerSession::handleClientPacket(InterPacketVec_t & pack_vec)
{
	boost::mutex::scoped_lock lock(m_client_packet_vec_mtx);
	std::copy(pack_vec.begin(), pack_vec.end(), std::back_inserter(m_client_packet_vec));
}

void ManageServerSession::notifyServerSocketClosed(uint64 gate_guid)
{
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_MSG_PLAYER_CLOSE_CONNECTION, gate_guid);
	this->handleClientPacket(pack);
}

int ManageServerSession::svc()
{
	boost::posix_time::milliseconds empty_sleep_time(1);

	InterPacketVec_t client_pack_vec;
	InterPacketVec_t server_event_pack_vec;

	size_t server_evt_num = 0;
	size_t client_pack_num = 0;
	while (true)
	{
		this->getClientPacket(client_pack_vec, server_event_pack_vec);

		server_evt_num = server_event_pack_vec.size();
		client_pack_num = client_pack_vec.size();

		for (auto pack : server_event_pack_vec)
		{
			this->doHandleServerEventPacket(pack);
			delete pack;
		}
		server_event_pack_vec.clear();

		for (auto pack : client_pack_vec)
		{
			this->doHandleClientPacket(pack);
			delete pack;
		}
		client_pack_vec.clear();

		if (0 == server_evt_num && 0 == client_pack_num)
		{
			boost::this_thread::sleep(empty_sleep_time);
		}
	}
	return 0;
}

void ManageServerSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	DEF_LOG_DEBUG("get new server session");

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(MSG_NEW_CLIENT_SESSION, 0);
	pack->session_id = session_id;
	
	boost::mutex::scoped_lock lock(m_client_packet_vec_mtx);
	m_server_event_packet_vec.push_back(pack);
}

void ManageServerSession::handleNewConnectionFailed()
{

}

void ManageServerSession::handleCloseConnection(SessionId session_id, int error_code)
{
	DEF_LOG_DEBUG("get close server session");

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(MSG_CLOSE_CLIENT_SESSION, 0);
	pack->session_id = session_id;

	boost::mutex::scoped_lock lock(m_client_packet_vec_mtx);
	m_server_event_packet_vec.push_back(pack);
}

size_t ManageServerSession::handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	InterPacketVec_t pack_vec;
	size_t ret = parseInterPacketFromStream(session_id, buffer, buf_len, pack_vec);

	ManageClientSession::instance()->handleServerPacket(pack_vec);
	return ret;
}

void ManageServerSession::handlePacket(InterPacketVec_t & pack_vec)
{
	//ManageClientSession::instance()->handleServerPacket();
}

void ManageServerSession::getClientPacket(InterPacketVec_t & pack_vec, InterPacketVec_t & server_event_pack_vec)
{
	boost::mutex::scoped_lock lock(m_client_packet_vec_mtx);
	if (m_client_packet_vec.size() > 0)
	{
		std::copy(m_client_packet_vec.begin(), m_client_packet_vec.end(), std::back_inserter(pack_vec));
		m_client_packet_vec.clear();
	}

	if (m_server_event_packet_vec.size() > 0)
	{
		std::copy(m_server_event_packet_vec.begin(), m_server_event_packet_vec.end(), std::back_inserter(server_event_pack_vec));
		m_server_event_packet_vec.clear();
	}
}

void ManageServerSession::doHandleServerEventPacket(InterPacket * packet)
{
	if (MSG_NEW_CLIENT_SESSION == packet->msg_head.msg_id)
	{
		m_server_info.session_id = packet->session_id;
		m_server_info.is_validation = true;

		this->registerGate(packet->session_id);
	}
	else if (MSG_CLOSE_CLIENT_SESSION == packet->msg_head.msg_id)
	{
		m_server_info.session_id = NULL;
		m_server_info.is_validation = false;
	}
	else
	{
		DEF_LOG_DEBUG("do not handle server event msg");
	}
}

void ManageServerSession::doHandleClientPacket(InterPacket * packet)
{
	if (NULL != m_server_info.session_id)
	{
		m_session_server->handleOutputStream(m_server_info.session_id, packet->getStream(), packet->getStreamSize());
		packet->cleanStream();
	}
	else
	{
		DEF_LOG_ERROR("the sever session is NULL while handle output stream");
	}
}

void ManageServerSession::registerGate(SessionId session_id)
{
	ClientCfg & client_cfg = GateConfig::instance()->getClientCfg();

	DEF_LOG_INFO("ready to register gate info to server, ip is <%s>, port is <%d>", client_cfg.ip_addr.c_str(), client_cfg.port);

	flatbuffers::FlatBufferBuilder builder;
	Offset<String> ip_addr_off = builder.CreateString(client_cfg.ip_addr);

	Offset<SMsgGateRegister> off_reg = CreateSMsgGateRegister(builder, ip_addr_off, client_cfg.port);

	builder.Finish(off_reg);

	InterPacket inter_pack;
	inter_pack.setMsgInfo(E_SMsgGateRegister, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	inter_pack.session_id = session_id;

	m_session_server->handleOutputStream(session_id, inter_pack.getStream(), inter_pack.getStreamSize());

	inter_pack.cleanStream();
}