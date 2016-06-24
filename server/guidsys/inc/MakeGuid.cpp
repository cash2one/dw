
#include "MakeGuid.h"
#include "Logger.h"

MakeGuid * MakeGuid::m_instance = NULL;

MakeGuid::MakeGuid()
: m_request_no(5000)
, m_is_stop(false)
, m_session_server(NULL)
, m_thread(NULL)
{
}

MakeGuid::~MakeGuid()
{
}

bool MakeGuid::get_guid(EntityType entity_type, uint64 & guid)
{
	if (m_guid_info.guid_rest == 0)
	{
		if (!getGuidInfo(m_guid_info))
		{
			return false;
		}
	}

	guid = MAKE_ENTITY_GUID(entity_type, m_guid_info.seed, m_guid_info.index);
	m_guid_info.index += 1;
	m_guid_info.guid_rest -= 1;

	return true;
}

MakeGuid * MakeGuid::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new MakeGuid();
	}

	return m_instance;
}

void MakeGuid::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	InterPacket * pack = new InterPacket();

	pack->setMsgInfo(MSG_NEW_CLIENT_SESSION, 0, 0, (const char *)0);

	pack->session_id = session_id;

	this->addInputPacket(pack);

}

void MakeGuid::handleNewConnectionFailed()
{

}

void MakeGuid::handleCloseConnection(SessionId session_id, int error_code)
{
	InterPacket * pack = new InterPacket();
	pack->msg_head.msg_id = MSG_CLOSE_CLIENT_SESSION;
	pack->msg_head.msg_len = 0;
	pack->session_id = session_id;

	this->addInputPacket(pack);
}

size_t MakeGuid::handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	InterPacketVec_t pack_vec;
	size_t ret = parseInterPacketFromStream(session_id, buffer, buf_len, pack_vec);
	for (auto pack : pack_vec)
	{
		this->addInputPacket(pack);
	}
	return ret;
}

int MakeGuid::init(const string & guid_srv_addr, int port, uint32 once_request_no)
{
	DEF_LOG_INFO("init make guid, ip is <%s>, port is <%d>, once request no is <%d>", guid_srv_addr.c_str(), port, once_request_no);

	m_request_no = once_request_no;
	m_connection_info.ip_addr = guid_srv_addr;
	m_connection_info.port = port;

	m_session_server = SessionServer::createSessionServer(NPT_TCP);
	m_session_server->init(this);
	
	//if (connectSrv() == -1)
	//{
	//	DEF_LOG_ERROR("failed to connect to guid server, addr is <%s>", guid_srv_addr.c_str());
	//	return -1;
	//}

	boost::function0<int> f = boost::bind(&MakeGuid::svc, this);
	m_thread = new boost::thread(f);

	return 0;
}

int MakeGuid::stop()
{
	m_is_stop = true;
	return 0;
}

int MakeGuid::svc()
{
	REPORT_THREAD_INFO();

	InterPacketVec_t input_pack_vec;
	boost::posix_time::milliseconds empty_sleep_time(100);

	while (!m_is_stop)
	{
		boost::this_thread::sleep(empty_sleep_time);

		this->checkConnection();

		this->checkGuidInfo();

		{
			boost::mutex::scoped_lock lock(m_input_pack_vec_mtx);
			if (m_input_pack_vec.size() == 0)
			{
				continue;
			}

			std::copy(m_input_pack_vec.begin(), m_input_pack_vec.end(), std::back_inserter(input_pack_vec));
			m_input_pack_vec.clear();
		}

		for (auto pack : input_pack_vec)
		{
			this->handleInputPacket(pack);
			delete pack;
		}

		input_pack_vec.clear();

	}

	return 0;
}

int MakeGuid::connectSrv()
{
	m_session_server->connect(m_connection_info.ip_addr, m_connection_info.port, true);
	m_connection_info.last_request_connection_time = getSystemMilliSeconds();
	return 0;
}

bool MakeGuid::getGuidInfo(GuidInfo & guid_info)
{
	boost::mutex::scoped_lock lock(m_input_pack_vec_mtx);
	if (m_guid_info_que.size() == 0)
	{
		return false;
	}

	guid_info = m_guid_info_que.front();
	m_guid_info_que.pop_front();

	return true;
}

void MakeGuid::checkConnection()
{
	if (NULL == m_connection_info.session_id)
	{
		time_t curr_time = getSystemMilliSeconds();
		if (curr_time - m_connection_info.last_request_connection_time >= 60 * 1000)
		{
			this->connectSrv();
		}
	}
}

void MakeGuid::checkGuidInfo()
{
	boost::mutex::scoped_lock lock(m_input_pack_vec_mtx);
	if (NULL == m_connection_info.session_id)
	{
		return;
	}

	if (m_guid_info_que.size() == 0)
	{
		if (!m_guid_request_info.is_requesting)
		{
			// do request
			this->doGuidRequest();
			m_guid_request_info.is_requesting = true;
		}
	}
}

void MakeGuid::doGuidRequest()
{
	InterPacket pack;

	flatbuffers::FlatBufferBuilder build;
	flatbuffers::Offset<MobaGo::FlatBuffer::SMsgRequestGuidReq> req_off = MobaGo::FlatBuffer::CreateSMsgRequestGuidReq(build, m_request_no);
	build.Finish(req_off);

	pack.setMsgInfo(E_SMsgRequestGuidReq, 0, build.GetSize(), (char *)build.GetBufferPointer());
	pack.session_id = m_connection_info.session_id;

	m_session_server->handleOutputStream(m_connection_info.session_id, pack.getStream(), pack.getStreamSize());
	pack.cleanStream();
}

void MakeGuid::addInputPacket(InterPacket * pack)
{
	boost::mutex::scoped_lock lock(m_input_pack_vec_mtx);
	m_input_pack_vec.push_back(pack);
}

void MakeGuid::handleInputPacket(InterPacket * pack)
{
	if (E_SMsgRequestGuidAck == pack->msg_head.msg_id)
	{
		this->handleGuidRequest(pack);
	}
	else if (MSG_NEW_CLIENT_SESSION == pack->msg_head.msg_id)
	{
		this->handleNewConnPack(pack);
	}
	else if (MSG_CLOSE_CLIENT_SESSION == pack->msg_head.msg_id)
	{
		this->handleCloseConnPack(pack);
	}
	else
	{
		DEF_LOG_ERROR("get unknown packet, msg id is <%hd>, msg len is <%hd>", pack->msg_head.msg_id, pack->msg_head.msg_len);
	}
}

void MakeGuid::handleGuidRequest(InterPacket * pack)
{
	const SMsgRequestGuidAck * msg = flatbuffers::GetRoot<SMsgRequestGuidAck>(pack->getBody());
	flatbuffers::Verifier verifier((uint8_t *)pack->getBody(), pack->msg_head.msg_len);
	if (NULL == msg || !msg->Verify(verifier))
	{
		// error
		DEF_LOG_ERROR("can not get SMsgRequestGuidAck by packet <%hd, %hd>", pack->msg_head.msg_id, pack->msg_head.msg_len);
		return;
	}

	GuidInfo guid_info;
	guid_info.seed = msg->seed_index();
	guid_info.index = msg->start_index();
	guid_info.guid_no = msg->create_number();
	guid_info.guid_rest = msg->create_number();

	m_guid_info_que.push_back(guid_info);
}

void MakeGuid::handleNewConnPack(InterPacket * pack)
{
	if (NULL != m_connection_info.session_id)
	{
		// error
	}

	m_connection_info.session_id = pack->session_id;
}

void MakeGuid::handleCloseConnPack(InterPacket * pack)
{
	m_connection_info.session_id = NULL;
}
