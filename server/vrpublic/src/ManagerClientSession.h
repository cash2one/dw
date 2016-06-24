
#ifndef MANAGER_CLIENT_SESSION_HPP__
#define MANAGER_CLIENT_SESSION_HPP__

//#include "SessionPool.h"
#include "SessionServer.h"
#include "Packet_.h"
#include "StatCount.h"

class ManagerClientSession : public InputStreamHandle
{
public:
	ManagerClientSession();
	~ManagerClientSession();
	static ManagerClientSession * instance();
public:
	void update();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session, int error_code);
	virtual size_t handleInputStream(SessionId session, char * buffer, size_t buf_len);

	//virtual void newConnection(netstream::Session_t session);

	//virtual void connectionClosed(netstream::Session_t session, int trigger_source);

	//virtual void handleInputStream(netstream::Session_t session, ACE_Message_Block & msg_block);
public:
	int init(const string & addr, int port, int net_type);

	void handleOutputStream(SessionId session, char * buffer, size_t buf_len);

	//void removeSession(netstream::Session_t session);
	void removeSession(SessionId session);
protected:
private:
	//netstream::SessionPool * m_session_pool;
	SessionServer * m_session_server;

	static ManagerClientSession * m_instance;

	StatCount m_stat_connection_count;

	
};

#endif
