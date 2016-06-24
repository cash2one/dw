
#include <boost/date_time.hpp>
#include "Logger.h"
#include "IoServer.h"

IoServer::IoServer()
	: m_event_base(NULL)
	, m_input_stream_handle(NULL)
	, m_tread_handle(NULL)
{

}

IoServer::~IoServer()
{
	if (NULL != m_tread_handle)
	{
		delete m_tread_handle;
	}
}

void IoServer::connect(const string & ip_str, int port, bool auto_reconnect)
{
	boost::mutex::scoped_lock lock(m_connect_info_vec_mtx);
	m_connect_info_vec.push_back(ConnectInfo(ip_str, port, auto_reconnect));
}

void IoServer::handleOutputStream(Session * session, char * buffer, size_t buf_len)
{
	boost::mutex::scoped_lock lock(m_output_buffer_info_vec_mtx);
	m_output_buffer_info_vec.push_back(OutputBufferInfo(buffer, buf_len, session));
}


Session * IoServer::createSessionWithSock(evutil_socket_t sock)
{
	Session * session = Session::createSession();
	session->initWithSock(m_event_base, sock, this);
	return session;
}

Session * IoServer::createSession()
{
	Session * session = Session::createSession();
	session->init(m_event_base, this);
	return session;
}

void IoServer::handleAcceptedSession(evutil_socket_t sock)
{
	boost::mutex::scoped_lock lock(m_accepted_socket_vec_mtx);
	m_accepted_socket_vec.push_back(sock);
	//Session * session = this->createSessionWithSock(sock);
}

void IoServer::handleReadEvent(SessionId session_id)
{
	Session * session = (Session *)session_id;
	MsgBlock & msg_block = session->getInputMsgBlock();

	session->handleMsgBlock();

	while (msg_block.size() > 0)
	{
		size_t read_len = m_input_stream_handle->handleInputStream(session, msg_block.rdPtr(), msg_block.size());
		if (0 == read_len)
		{
			// no whole msg to extract
			break;
		}

		if (read_len > msg_block.size())
		{
			// error
			read_len = msg_block.size();
		}

		msg_block.rdPtr(read_len);

		msg_block.crunch();

		session->handleMsgBlock();
	}
}

void IoServer::handleWriteEvent(SessionId session_id)
{
	Session * session = (Session *)session_id;
	// do nothing
}

void IoServer::handleConnectEvent(SessionId session_id)
{
	Session * session = (Session *)session_id;
	m_input_stream_handle->handleNewConnection(session_id, session->getRemoteIpAddr(), session->getLocalPort(), NULL);
}

void IoServer::handleCloseEvent(SessionId session_id)
{
	// 
	Session * session = (Session *)session_id;
	m_input_stream_handle->handleCloseConnection(session, 0);
	this->checkReconnectWhileSocketClose(session_id);
	this->removeSession((Session *)session_id);
	this->destroySession(session);
}

void IoServer::closeSession(SessionId session_id)
{
	boost::mutex::scoped_lock lock(m_output_buffer_info_vec_mtx);
	m_is_closing_session_set.insert(session_id);
}

void IoServer::getOutputBuffer(OutputBufferInfoList_t & buffer_vec, std::unordered_set<SessionId> & is_closing_session_set)
{
	boost::mutex::scoped_lock lock(m_output_buffer_info_vec_mtx);
	std::copy(m_output_buffer_info_vec.begin(), m_output_buffer_info_vec.end(), std::back_inserter(buffer_vec));
	m_output_buffer_info_vec.clear();

	if (m_is_closing_session_set.size() > 0)
	{
		for (auto session : m_is_closing_session_set)
		{
			is_closing_session_set.insert(session);
		}

		m_is_closing_session_set.clear();
	}
}

bool IoServer::handleBufferInfo(OutputBufferInfo & buffer_info)
{
	Session * session = buffer_info.session;
	if (this->isAvilableSession(session))
	{
		return session->handleOutputStream(buffer_info.buffer, buffer_info.buf_len);
	}

	return true;
}

void IoServer::getAcceptedSocket(SocketVec_t & socket_vec)
{
	boost::mutex::scoped_lock lock(m_accepted_socket_vec_mtx);
	std::copy(m_accepted_socket_vec.begin(), m_accepted_socket_vec.end(), std::back_inserter(socket_vec));
	m_accepted_socket_vec.clear();
}

void IoServer::handleNewSocket(evutil_socket_t sock)
{
	Session * session = this->createSessionWithSock(sock);
	this->addSession(session);
	//session->extractRemoteIp();
	if (NULL != m_input_stream_handle)
	{
		m_input_stream_handle->handleNewConnection(session, session->getRemoteIpAddr(), session->getLocalPort(), NULL);
	}
}

void IoServer::addSession(Session * session)
{
	SessionSet_t::iterator it = m_avilable_session_set.find(session);
	if (it != m_avilable_session_set.end())
	{
		// error
		this->destroySession(*it);
		m_avilable_session_set.erase(it);
	}

	m_avilable_session_set.insert(session);
}

bool IoServer::isAvilableSession(Session * session)
{
	SessionSet_t::iterator it = m_avilable_session_set.find(session);
	return it != m_avilable_session_set.end() ? true : false;
}

void IoServer::removeSession(Session * session)
{
	SessionSet_t::iterator it = m_avilable_session_set.find(session);
	if (it != m_avilable_session_set.end())
	{
		m_avilable_session_set.erase(it);
	}
	else
	{
		// error
	}
}

void IoServer::destroySession(Session * session)
{
	delete session;
}

void IoServer::destroyOutputBufferInfo(OutputBufferInfo & output_buffer_info)
{
	delete[] output_buffer_info.buffer;
}

void IoServer::getConnectInfo(ConnectInfoVec_t & connect_vec)
{
	boost::mutex::scoped_lock lock(m_connect_info_vec_mtx);
	std::copy(m_connect_info_vec.begin(), m_connect_info_vec.end(), std::back_inserter(connect_vec));
	m_connect_info_vec.clear();
}

Session * IoServer::doConnect(const string & ip_str, int port)
{
	Session * session = this->createSession();
	session->init(m_event_base, this);

	if (session->connect(ip_str, port))
	{
		return session;
	}
	else
	{
		return NULL;
	}
}

void IoServer::checkReconnectInfo()
{
	for (auto it = m_reconnect_info_lst.begin(); it != m_reconnect_info_lst.end(); ++it)
	{
		ReconnectInfo & reconn_info = *it;
		if (NULL == reconn_info.session)
		{
			time_t curr_time = getSystemMilliSeconds();
			if (curr_time - reconn_info.last_connect_time >= 45 * 1000)
			{
				reconn_info.last_connect_time = curr_time;

				Session * session = this->doConnect(reconn_info.ip_str, reconn_info.port);
				if (NULL != session)
				{
					reconn_info.session = session;
					this->addSession(session);
				}
			}
		}
	}
}

void IoServer::checkReconnectWhileSocketClose(SessionId session_id)
{
	if (m_reconnect_info_lst.size() > 0)
	{
		for (auto it = m_reconnect_info_lst.begin(); it != m_reconnect_info_lst.end(); ++it)
		{
			ReconnectInfo & reconn_info = *it;
			if (session_id == reconn_info.session)
			{
				reconn_info.last_connect_time = getSystemMilliSeconds();
				reconn_info.session = NULL;
				break;
			}
		}
	}
}

int IoServer::init(InputStreamHandle * input_stream_handle)
{
	m_input_stream_handle = input_stream_handle;

	event_config * event_cfg = event_config_new();
	
	event_config_set_flag(event_cfg, EVENT_BASE_FLAG_NOLOCK);

	m_event_base = event_base_new_with_config(event_cfg);
	if (NULL == m_event_base)
	{
		return -1;
	}

	boost::function0<int> f = boost::bind(&IoServer::svc, this);
	m_tread_handle = new boost::thread(f);

	return 0;
}

int IoServer::svc()
{
	OutputBufferInfoList_t buffer_vec;
	SocketVec_t accepted_socket_vec;
	ConnectInfoVec_t connect_info_vec;
	boost::posix_time::milliseconds empty_sleep_time(1);
	SessionSet_t can_write_session_set;
	std::unordered_set<SessionId> is_closing_session_set;

	while (true)
	{
		int num = event_base_loop(m_event_base, EVLOOP_NONBLOCK | EVLOOP_ONCE);

		this->getOutputBuffer(buffer_vec, is_closing_session_set);

		this->processClosingSession(is_closing_session_set);
		is_closing_session_set.clear();

		size_t buf_num = buffer_vec.size();

		time_t start_time = getSystemMilliSeconds();

		for (OutputBufferInfoList_t::iterator it = buffer_vec.begin(); it != buffer_vec.end(); )
		{
			OutputBufferInfo & buffer_info = *it;

			Session * session = buffer_info.session;
			if (!this->isAvilableSession(session))
			{
				DEF_LOG_DEBUG("handle output , but the session is not avilable <%x>, buffer len is <%d>", buffer_info.session, buffer_info.buf_len);

				this->destroyOutputBufferInfo(buffer_info);
				it = buffer_vec.erase(it);

				continue;
			}

			if (session->handleOutputStream(buffer_info.buffer, buffer_info.buf_len))
			{
				this->destroyOutputBufferInfo(buffer_info);
				it = buffer_vec.erase(it);

				can_write_session_set.insert(session);
			}
			else
			{
				++it;
			}
		}

		time_t end_time = getSystemMilliSeconds();

		if ((end_time - start_time >= 20 || buf_num > 100000) && buf_num > 0)
		{
			int diff_time = (int)(end_time - start_time);
			float one_time = float(diff_time) / buf_num;
			DEF_LOG_INFO("handle output packet, packet num is <%d>, total time is <%d>, one packet dispatch time is <%.4f>, left pack is <%d>", buf_num, diff_time, one_time, buffer_vec.size());
		}

		for (auto session : can_write_session_set)
		{
			session->enableWriteEvent();
		}
		can_write_session_set.clear();

		this->getAcceptedSocket(accepted_socket_vec);

		size_t new_sock_num = accepted_socket_vec.size();
		for (auto sock : accepted_socket_vec)
		{
			this->handleNewSocket(sock);
		}
		accepted_socket_vec.clear();

		getConnectInfo(connect_info_vec);
		for (auto connect_info : connect_info_vec)
		{
			Session * session = this->doConnect(connect_info.ip_str, connect_info.port);
			if (connect_info.auto_reconnect)
			{
				m_reconnect_info_lst.push_back(ReconnectInfo(connect_info.ip_str, connect_info.port, session));
			}
			if (NULL != session)
			{
				//m_input_stream_handle->handleNewConnection(session);
				DEF_LOG_DEBUG("success to connect <%s:%d>", connect_info.ip_str.c_str(), connect_info.port);
				this->addSession(session);
			}
			else
			{
				DEF_LOG_ERROR("failed to connect <%s:%d>", connect_info.ip_str.c_str(), connect_info.port);
			}
		}
		connect_info_vec.clear();

		this->checkReconnectInfo();

		if (0 == buf_num && 0 == new_sock_num)
		{
			boost::this_thread::sleep(empty_sleep_time);
		}
	}
	return 0;
}

void IoServer::processClosingSession(std::unordered_set<SessionId> & is_closing_session_set)
{
	for (auto session_id : is_closing_session_set)
	{
		DEF_LOG_DEBUG("process closing session <%x>", session_id);
		Session * session = (Session *)session_id;
		if (this->isAvilableSession(session))
		{
			this->removeSession(session);
			this->destroySession(session);
		}
	}
}


