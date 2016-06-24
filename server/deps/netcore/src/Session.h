
#ifndef SESSION_HPP__
#define SESSION_HPP__

#include <string>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include "SessionHandle.h"
#include "SessionEvent.h"
#include "MsgBlock.h"

using namespace std;

class Session
{
public:
	virtual ~Session();
	static Session * createSession();
public:
	virtual int init(event_base * event_base, SessionHandle * session_handle) = 0;

	virtual int initWithSock(event_base * event_base, evutil_socket_t sock, SessionHandle * session_handle) = 0;

	virtual bool connect(const string & ip_str, int port) = 0;

	virtual void handleMsgBlock() = 0;

	virtual bool handleOutputStream(char * buffer, size_t buf_size) = 0;

	virtual MsgBlock & getInputMsgBlock() = 0;

	virtual evutil_socket_t getSock() = 0;

	virtual bool isWriting() = 0;

	virtual void isWriting(bool is_writing) = 0;

	virtual void enableWriteEvent() = 0;

	virtual const string & getRemoteIpAddr() = 0;

	virtual int getLocalPort() = 0;

protected:
	virtual void initSock() = 0;
private:

};

#endif
