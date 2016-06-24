
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <Ws2tcpip.h>
#endif
#include "Logger.h"
#include "SessionEventBuffer.h"

#define SOCK_HIGH_WATER_MASK 102400

void eb_read_cb(struct bufferevent *bev, void *ptr)
{
	SessionEventBuffer * session = (SessionEventBuffer *)ptr;
	session->handleInputEvent();
}

void eb_event_cb(struct bufferevent *bev, short events, void *ptr)
{
	SessionEventBuffer * session = (SessionEventBuffer *)ptr;
	if (events & BEV_EVENT_ERROR || events & BEV_EVENT_EOF)
	{
		// error
		session->handleCloseEvent();

		DEF_LOG_DEBUG("get socket error event <%x>, session is <%x>", events, ptr);
	}
	else if (events & BEV_EVENT_CONNECTED)
	{
		session->extractRemoteIp();
		session->handleConnectEvent();

		//DEF_LOG_DEBUG("get sock new connection event <%x>, session is <%x>, remote ip is <%s:%d>", events, ptr, ip_str, ip_adr_get.sin_port);
	}
	else
	{
		DEF_LOG_INFO("get not close socket event : <%x>, session is <%x>", events, ptr);
	}
}

void eb_write_cb(struct bufferevent *bev, void *ptr)
{
	SessionEventBuffer * session = (SessionEventBuffer *)ptr;
	session->handleOutputEvent();
}

SessionEventBuffer::SessionEventBuffer()
	: m_sock()
	, m_event_base(NULL)
	, m_buffer_event(NULL)
	, m_is_writing(false)
	, m_local_port(0)
{

}

SessionEventBuffer::~SessionEventBuffer()
{
	if (NULL != m_buffer_event)
	{
		bufferevent_free(m_buffer_event);
		m_buffer_event = NULL;
	}
}

int SessionEventBuffer::init(event_base * event_base, SessionHandle * session_handle)
{
	m_event_base = event_base;
	m_session_handle = session_handle;

	m_msg_block.init(10240000);

	m_buffer_event = bufferevent_socket_new(m_event_base, -1, BEV_OPT_CLOSE_ON_FREE);

	if (NULL == m_buffer_event)
	{
		return -1;
	}

	//evutil_make_socket_nonblocking(m_sock);

	//bufferevent_setcb(m_buffer_event, read_cb, write_cb, event_cb, this);

	//bufferevent_enable(m_buffer_event, EV_READ);

	return 0;
}

int SessionEventBuffer::initWithSock(event_base * event_base, evutil_socket_t sock, SessionHandle * session_handle)
{
	m_event_base = event_base;
	m_sock = sock;
	m_session_handle = session_handle;

	m_buffer_event = bufferevent_socket_new(m_event_base, sock, BEV_OPT_CLOSE_ON_FREE);

	if (NULL == m_buffer_event)
	{
		return -1;
	}

	this->initSock();

	this->extractRemoteIp();

	m_msg_block.init(10024000);

	return 0;
}

bool SessionEventBuffer::connect(const string & ip_str, int port)
{
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip_str.c_str());
	sin.sin_port = htons(port);

	bufferevent_setcb(m_buffer_event, eb_read_cb, eb_write_cb, eb_event_cb, this);

	if (bufferevent_socket_connect(m_buffer_event, (struct sockaddr*)&sin, sizeof(sin)) == 0)
	{
		//this->initSock();
		bufferevent_setwatermark(m_buffer_event, EV_READ, 0, SOCK_HIGH_WATER_MASK);

		bufferevent_setwatermark(m_buffer_event, EV_WRITE, 0, SOCK_HIGH_WATER_MASK);

		return true;
	}
	else
	{
		return false;
	}
}

void SessionEventBuffer::handleInputEvent()
{
	m_session_handle->handleReadEvent(this);
	//bufferevent_enable(m_buffer_event, EV_READ | EV_PERSIST);
}

void SessionEventBuffer::handleOutputEvent()
{
	struct evbuffer * output = bufferevent_get_output(m_buffer_event);
	size_t len = evbuffer_get_length(output);
	if (len > 0)
	{
		this->isWriting(true);
		bufferevent_enable(m_buffer_event, EV_WRITE);
	}
	else
	{
		bufferevent_disable(m_buffer_event, EV_WRITE);
		this->isWriting(false);
	}
}

void SessionEventBuffer::handleConnectEvent()
{
	bufferevent_enable(m_buffer_event, EV_READ | EV_PERSIST);
	this->m_session_handle->handleConnectEvent(this);
}

void SessionEventBuffer::handleCloseEvent()
{
	m_session_handle->handleCloseEvent(this);
}

void SessionEventBuffer::handleMsgBlock()
{
	struct evbuffer * input = bufferevent_get_input(m_buffer_event);
	size_t len = evbuffer_get_length(input);
	if (len > 0)
	{
		size_t len = evbuffer_copyout(input, m_msg_block.wtPtr(), m_msg_block.space());
		if (len > 0)
		{
			m_msg_block.wtPtr(len);
			evbuffer_drain(input, len);
		}
	}
}

bool SessionEventBuffer::handleOutputStream(char * buffer, size_t buf_size)
{
	if (0 == buf_size)
	{
		// should never happen
		return true;
	}

	if (this->isWriting())
	{
		return false;
	}

	struct evbuffer * output = bufferevent_get_output(m_buffer_event);
	if (0 == evbuffer_add(output, buffer, buf_size))
	{
		//bufferevent_enable(m_buffer_event, EV_WRITE);
		//this->isWriting(true);
		return true;
	}
	else
	{
		return false;
	}
}

MsgBlock & SessionEventBuffer::getInputMsgBlock()
{
	return m_msg_block;
}

evutil_socket_t SessionEventBuffer::getSock()
{
	return bufferevent_getfd(m_buffer_event);
}

bool SessionEventBuffer::isWriting()
{
	return m_is_writing;
}

void SessionEventBuffer::setRemoteIpAddr(const string & remote_ip_addr, int port)
{
	m_remote_ip_addr = remote_ip_addr;
	m_local_port = port;
}

const string & SessionEventBuffer::getRemoteIpAddr()
{
	return m_remote_ip_addr;
}

int SessionEventBuffer::getLocalPort()
{
	return m_local_port;
}

void SessionEventBuffer::extractRemoteIp()
{
	struct sockaddr_in ip_adr_get;
	int ip_adr_len;

	ip_adr_len = sizeof(ip_adr_get);
	getpeername(this->getSock(), (sockaddr *)&ip_adr_get, (socklen_t *)&ip_adr_len);

	char ip_str[50] = { 0 };
	inet_ntop(AF_INET, &ip_adr_get.sin_addr, ip_str, (socklen_t)50);
	this->setRemoteIpAddr(ip_str, ip_adr_get.sin_port);
}

void SessionEventBuffer::isWriting(bool is_writing)
{
	m_is_writing = is_writing;
}

void SessionEventBuffer::enableWriteEvent()
{
	// todo
	struct evbuffer * output = bufferevent_get_output(m_buffer_event);
	size_t len = evbuffer_get_length(output);
	if (len > 0)
	{
		bufferevent_enable(m_buffer_event, EV_WRITE);
		this->isWriting(true);
	}
}

void SessionEventBuffer::initSock()
{
	evutil_make_socket_nonblocking(m_sock);

	bufferevent_setcb(m_buffer_event, eb_read_cb, eb_write_cb, eb_event_cb, this);

	bufferevent_enable(m_buffer_event, EV_READ | EV_PERSIST);

	bufferevent_setwatermark(m_buffer_event, EV_READ, 0, SOCK_HIGH_WATER_MASK);

	bufferevent_setwatermark(m_buffer_event, EV_WRITE, 0, SOCK_HIGH_WATER_MASK);

}
