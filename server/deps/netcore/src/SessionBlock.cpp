
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <string.h>
#include "SessionBlock.h"

void block_read_cb(struct bufferevent *bev, void *ptr)
{
	SessionBlock * session = (SessionBlock *)ptr;
	session->handleInputEvent();
}

void block_event_cb(struct bufferevent *bev, short events, void *ptr)
{
	SessionBlock * session = (SessionBlock *)ptr;
	if (events & BEV_EVENT_ERROR)
	{
		// error
		session->handleCloseEvent();
	}
}

void block_write_cb(struct bufferevent *bev, void *ptr)
{
	SessionBlock * session = (SessionBlock *)ptr;
	session->handleOutputEvent();
}

SessionBlock::SessionBlock()
	: m_sock()
	, m_event_base(NULL)
	, m_buffer_event(NULL)
	, m_is_writing(false)
{

}

SessionBlock::~SessionBlock()
{
	if (NULL != m_buffer_event)
	{
		bufferevent_free(m_buffer_event);
		m_buffer_event = NULL;
	}
}

int SessionBlock::init(event_base * event_base, SessionHandle * session_handle)
{
	m_event_base = event_base;
	m_session_handle = session_handle;

	m_msg_block.init(10240);

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

int SessionBlock::initWithSock(event_base * event_base, evutil_socket_t sock, SessionHandle * session_handle)
{
	m_event_base = event_base;
	m_sock = sock;
	m_session_handle = session_handle;

	m_msg_block.init(1024);

	m_buffer_event = bufferevent_socket_new(m_event_base, sock, BEV_OPT_CLOSE_ON_FREE);

	if (NULL == m_buffer_event)
	{
		return -1;
	}

	this->initSock();

	return 0;
}

bool SessionBlock::connect(const string & ip_str, int port)
{
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip_str.c_str());
	sin.sin_port = htons(port);

	if (bufferevent_socket_connect(m_buffer_event, (struct sockaddr*)&sin, sizeof(sin)) == 0)
	{
		this->initSock();
		return true;
	}
	else
	{
		return false;
	}
}

void SessionBlock::handleInputEvent()
{
	m_session_handle->handleReadEvent(this);
	//bufferevent_enable(m_buffer_event, EV_READ | EV_PERSIST);
}

void SessionBlock::handleOutputEvent()
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

void SessionBlock::handleCloseEvent()
{

	m_session_handle->handleCloseEvent(this);
}

void SessionBlock::handleMsgBlock()
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

bool SessionBlock::handleOutputStream(char * buffer, size_t buf_size)
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

MsgBlock & SessionBlock::getInputMsgBlock()
{
	return m_msg_block;
}

evutil_socket_t SessionBlock::getSock()
{
	return m_sock;
}

bool SessionBlock::isWriting()
{
	return m_is_writing;
}

void SessionBlock::isWriting(bool is_writing)
{
	m_is_writing = is_writing;
}

void SessionBlock::enableWriteEvent()
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

void SessionBlock::initSock()
{
	evutil_make_socket_nonblocking(m_sock);

	bufferevent_setcb(m_buffer_event, block_read_cb, block_write_cb, block_event_cb, this);

	bufferevent_enable(m_buffer_event, EV_READ | EV_PERSIST);

	bufferevent_setwatermark(m_buffer_event, EV_READ, 0, 102400);

	bufferevent_setwatermark(m_buffer_event, EV_WRITE, 0, 102400);
}
