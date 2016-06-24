
#ifndef SESSION_HANDLE_HPP__
#define SESSION_HANDLE_HPP__

#include <string>
#include "netcoredef.h"
#include "MsgBlock.h"

using namespace std;

typedef void * SessionId;

class NETCORE_EXPORT InputStreamHandle
{
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session) = 0;
	virtual void handleNewConnectionFailed() = 0;
	virtual void handleCloseConnection(SessionId session_id, int error_code) = 0;
	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len) = 0;
};

#endif

