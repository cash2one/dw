
#ifndef MANAGE_VRPUBLIC_SESSION_HPP__
#define MANAGE_VRPUBLIC_SESSION_HPP__

#include "SessionServer.h"
#include "SessionHandle.h"
#include "Packet_.h"
#include "VrConfig.h"

class ManageVrpublicSession : public InputStreamHandle
{
public:
	ManageVrpublicSession();
	~ManageVrpublicSession();
public:
	static ManageVrpublicSession * instance();

	int init(const VrpublicCfg & vrpublic_cfg);

	void handleOutputPacket(InterPacket * packet);

	void getNewSession(SessionId session_id);

	void getCloseSession(SessionId session_id);
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session_id, int error_code);
	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len);

protected:
private:
	VrpublicCfg m_vrpublic_cfg;

	SessionServer * m_session_server;

	SessionId m_session_id;

	static ManageVrpublicSession * m_instance;
};

#endif
