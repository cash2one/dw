
#include "ManageSession.h"
#include "Logger.h"
#include "opcode.h"
#include "ManageConfig.h"

ManageSession::ManageSession()
	: m_session_server(NULL)
{
}

ManageSession::~ManageSession()
{
}

void ManageSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{

}

void ManageSession::handleNewConnectionFailed()
{

}

void ManageSession::handleCloseConnection(SessionId session_id, int error_code)
{

}

size_t ManageSession::handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	InterPacketVec_t pack_vec;
	size_t ret = parseInterPacketFromStream(session_id, buffer, buf_len, pack_vec);

	{
		boost::mutex::scoped_lock lock(m_input_pack_vec_mtx);
		std::copy(pack_vec.begin(), pack_vec.end(), std::back_inserter(m_input_pack_vec));
	}

	return ret;
}

void ManageSession::update()
{
	InterPacketVec_t pack_vec;
	{
		boost::mutex::scoped_lock lock(m_input_pack_vec_mtx);
		std::copy(m_input_pack_vec.begin(), m_input_pack_vec.end(), std::back_inserter(pack_vec));
		m_input_pack_vec.clear();
	}

	for (auto pack : pack_vec)
	{
		this->handleInputPacket(pack);
		delete pack;
	}

	pack_vec.clear();

}

int ManageSession::init()
{
	m_session_server = SessionServer::createSessionServer(NPT_TCP);
	if (NULL == m_session_server)
	{
		DEF_LOG_ERROR("failed to create session pool\n");
		return -1;
	}

	if (m_session_server->init(this) == -1)
	{
		DEF_LOG_ERROR("failed to init session pool\n");
		return -1;
	}

	if (m_session_server->listen(ManageConfig::instance()->getGuidCfg().addr, ManageConfig::instance()->getGuidCfg().port) == -1)
	{
		DEF_LOG_ERROR("failed to listen, addr is <%s:%d>\n", ManageConfig::instance()->getGuidCfg().addr.c_str(), ManageConfig::instance()->getGuidCfg().port);
		return -1;
	}

	m_alloc_guid.init(ManageConfig::instance()->getGuidCfg().seed_value, ManageConfig::instance()->getGuidCfg().index);

	DEF_LOG_INFO("success to init ManageSession, guidsys is running\n");
	return 0;
}

int ManageSession::stop()
{

	return 0;
}

int ManageSession::finit()
{
	return 0;
}

void ManageSession::handleInputPacket(InterPacket * pack)
{
	if (E_SMsgRequestGuidReq == pack->msg_head.msg_id)
	{
		InterPacket * ret = m_alloc_guid.processRequest(pack);
		if (NULL != ret)
		{
			m_session_server->handleOutputStream(pack->session_id, ret->getStream(), ret->getStreamSize());
			ret->cleanStream();

			delete ret;
		}
		else
		{
			// error
		}
	}
	else
	{
		DEF_LOG_ERROR("get unknow opcode <%d>, size is <%d>\n", pack->msg_head.msg_id, pack->msg_head.msg_len);
	}
}
