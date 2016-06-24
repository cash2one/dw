
#ifndef SESSION_SERVER_HPP__
#define SESSION_SERVER_HPP__

#include <string>
#include "SessionHandle.h"

using namespace std;

enum NetProtocolType
{
	NPT_TCP	= 1,
	NPT_RAKNET,
};

class NETCORE_EXPORT SessionServer
{
public:
	virtual int init(InputStreamHandle * input_stream_handle) = 0;

	virtual int listen(const string & ip_str, int port) = 0;

	virtual void connect(const string & ip_str, int port, bool auto_reconnect = false) = 0;

	virtual void handleOutputStream(SessionId session, char * buffer, size_t buf_len) = 0;

	virtual void closeSession(SessionId session_id) = 0;

	static SessionServer * createSessionServer(NetProtocolType net_type = NPT_TCP);
};

#endif
