
#ifndef SESSION_EVENT_BUFFER_HPP__
#define SESSION_EVENT_BUFFER_HPP__

#include <string>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include "Session.h"
#include "SessionHandle.h"
#include "MsgBlock.h"

using namespace std;

class SessionEventBuffer : public Session
{
public:
	SessionEventBuffer();
	~SessionEventBuffer();

public:
	int init(event_base * event_base, SessionHandle * session_handle);

	int initWithSock(event_base * event_base, evutil_socket_t sock, SessionHandle * session_handle);

	bool connect(const string & ip_str, int port);

	void handleInputEvent();

	void handleOutputEvent();

	void handleConnectEvent();

	void handleCloseEvent();

	void handleMsgBlock();

	bool handleOutputStream(char * buffer, size_t buf_size);

	MsgBlock & getInputMsgBlock();

	evutil_socket_t getSock();

	bool isWriting();

	void isWriting(bool is_writing);

	void enableWriteEvent();

	void setRemoteIpAddr(const string & remote_ip_addr, int port);

	const string & getRemoteIpAddr();

	int getLocalPort();

	void extractRemoteIp();

protected:
	void initSock();
private:
	evutil_socket_t m_sock;
	event_base * m_event_base;
	bufferevent * m_buffer_event;
	SessionHandle * m_session_handle;
	bool m_is_writing;
	string m_remote_ip_addr;
	int m_local_port;

	MsgBlock m_msg_block;
};

#endif
