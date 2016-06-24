
#ifndef SESSION_SERVER_IMP_HPP__
#define SESSION_SERVER_IMP_HPP__

#include "SessionServer.h"
#include "AcceptServer.h"
#include "IoServer.h"

class SessionServerImp : public SessionServer
{
public:
	SessionServerImp();
	~SessionServerImp();
public:
	virtual int init(InputStreamHandle * input_stream_handle);

	virtual int listen(const string & ip_str, int port);

	virtual void connect(const string & ip_str, int port, bool auto_reconnect = false);

	virtual void handleOutputStream(SessionId SessionId, char * buffer, size_t buf_len);

	virtual void closeSession(SessionId session_id);

protected:
	void startupWindowsSock();
private:
	AcceptServer * m_accept_server;
	IoServer * m_io_server;
	SessionHandle * m_session_handle;
};

#endif
