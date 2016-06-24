
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <Ws2tcpip.h>
#endif
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "AcceptServer.h"
#include "Logger.h"

void accept_connection_cb(struct evconnlistener * listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx)
{
	AcceptServer * accept_server = (AcceptServer *)(ctx);
	accept_server->handleNewConnection(fd, address);

	struct sockaddr_in ip_adr_get;
	int ip_adr_len;

	ip_adr_len = sizeof(ip_adr_get);
	getpeername(fd, (sockaddr *)&ip_adr_get, (socklen_t *)&ip_adr_len);

	char ip_str[50] = { 0 };
	inet_ntop(AF_INET, &ip_adr_get.sin_addr, ip_str, (socklen_t)50);

	DEF_LOG_DEBUG("get accepted new connection, remote ip is <%s:%d>", ip_str, ip_adr_get.sin_port);

}

void accept_error_cb(struct evconnlistener *listener, void *ctx)
{
	DEF_LOG_ERROR("get accept error");
}

AcceptServer::AcceptServer()
	: m_event_base(NULL)
	, m_event_listener(NULL)
	, m_session_handle(NULL)
	, m_port(0)
	, m_tread_handle(0)
{

}

AcceptServer::~AcceptServer()
{

}

int AcceptServer::init(const string & ip_str, int port, SessionHandle * session_handle)
{
	m_ip_str = ip_str;
	m_port = port;
	m_session_handle = session_handle;

	if (!this->initListener())
	{
		return -1;
	}

	boost::function0<int> f = boost::bind(&AcceptServer::svc, this);
	m_tread_handle = new boost::thread(f);

	return 0;
}

int AcceptServer::svc()
{
	//event_base_dispatch(m_event_base);

	int ev_num = 0;
	boost::posix_time::milliseconds empty_sleep_time(1);
	while (true)
	{
		ev_num = event_base_loop(m_event_base, 0);
		if (1 == ev_num)
		{
			boost::this_thread::sleep(empty_sleep_time);
		}
	}
	return 0;
}

void AcceptServer::handleNewConnection(evutil_socket_t fd, struct sockaddr *address)
{
	m_session_handle->handleAcceptedSession(fd);
}

bool AcceptServer::initListener()
{
	event_config * event_cfg = event_config_new();

#ifdef WIN32
	event_config_set_flag(event_cfg, EVENT_BASE_FLAG_NOLOCK);
#else
	event_config_set_flag(event_cfg, EVENT_BASE_FLAG_NOLOCK);
#endif

	m_event_base = event_base_new_with_config(event_cfg);

	if (NULL == m_event_base)
	{
		return false;
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(m_ip_str.c_str());
	sin.sin_port = htons(m_port);

	m_event_listener = evconnlistener_new_bind(m_event_base, accept_connection_cb, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&sin, sizeof(sin));

	if (NULL == m_event_listener)
	{
		DEF_LOG_ERROR("failed to listen on <%s:%d>", m_ip_str.c_str(), m_port);
		return false;
	}

	DEF_LOG_INFO("success to listen on <%s:%d>", m_ip_str.c_str(), m_port);

	evconnlistener_set_error_cb(m_event_listener, accept_error_cb);

	return true;
}
