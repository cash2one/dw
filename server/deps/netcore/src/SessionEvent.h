
#ifndef SESSION_EVENT_HPP__
#define SESSION_EVENT_HPP__

#include "netcoredef.h"
#include "SessionHandle.h"

class NETCORE_EXPORT SessionHandle
{
public:
	virtual void handleAcceptedSession(evutil_socket_t sock) = 0;
	virtual void handleReadEvent(SessionId session_id) = 0;
	virtual void handleWriteEvent(SessionId session_id) = 0;
	virtual void handleConnectEvent(SessionId session_id) = 0;
	virtual void handleCloseEvent(SessionId session_id) = 0;
	//	virtual boost::asio::io_service * getIoServer() = 0;
};

#endif
