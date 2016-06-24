
#include "SessionServerImp.h"
#include "Logger.h"

bool is_startup_windows_sock = false;

SessionServerImp::SessionServerImp()
	: m_accept_server(NULL)
	, m_io_server(NULL)
	, m_session_handle(NULL)
{

}

SessionServerImp::~SessionServerImp()
{

}

int SessionServerImp::init(InputStreamHandle * input_stream_handle)
{
	this->startupWindowsSock();
	m_io_server = new IoServer();

	return m_io_server->init(input_stream_handle);
}

int SessionServerImp::listen(const string & ip_str, int port)
{
	m_accept_server = new AcceptServer();
	if (m_accept_server->init(ip_str, port, m_io_server) != 0)
	{
		return -1;
	}

	return 0;
}

void SessionServerImp::connect(const string & ip_str, int port, bool auto_reconnect)
{
	m_io_server->connect(ip_str, port, auto_reconnect);
}

void SessionServerImp::handleOutputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	Session * session = (Session *)session_id;
	m_io_server->handleOutputStream(session, buffer, buf_len);
}

void SessionServerImp::closeSession(SessionId session_id)
{
	m_io_server->closeSession(session_id);
}

void SessionServerImp::startupWindowsSock()
{
#ifdef WIN32

	if (!is_startup_windows_sock)
	{
		is_startup_windows_sock = true;

		WSADATA wsaData;
		int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (0 != err)
		{
			DEF_LOG_ERROR("WSAStartup failed with error <%d>", err);
		}
	}

#endif // WIN32

}
