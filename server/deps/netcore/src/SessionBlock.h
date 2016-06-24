

#ifndef SESSION_BLOCK_HPP__
#define SESSION_BLOCK_HPP__

#include "Session.h"
#include "SessionEvent.h"

class SessionBlock : public Session
{
public:
	SessionBlock();
	~SessionBlock();

public:
	int init(event_base * event_base, SessionHandle * session_handle);

	int initWithSock(event_base * event_base, evutil_socket_t sock, SessionHandle * session_handle);

	bool connect(const string & ip_str, int port);

	void handleInputEvent();

	void handleOutputEvent();

	void handleCloseEvent();

	void handleMsgBlock();

	bool handleOutputStream(char * buffer, size_t buf_size);

	MsgBlock & getInputMsgBlock();

	evutil_socket_t getSock();

	bool isWriting();

	void isWriting(bool is_writing);

	void enableWriteEvent();

protected:
	void initSock();
private:
	evutil_socket_t m_sock;
	event_base * m_event_base;
	bufferevent * m_buffer_event;
	SessionHandle * m_session_handle;
	bool m_is_writing;

	MsgBlock m_msg_block;
};

#endif
