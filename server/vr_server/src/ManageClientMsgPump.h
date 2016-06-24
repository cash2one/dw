
#ifndef MANAGE_CLIENT_MSG_PUMP_HPP__
#define MANAGE_CLIENT_MSG_PUMP_HPP__

#include "MessagePump.h"
#include "Singleton.h"
#include "VrConfig.h"

class ManageClientMsgPump : public MessagePumpHandle
{
public:
	ManageClientMsgPump();
	~ManageClientMsgPump();
	static ManageClientMsgPump * instance();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);

public:
	int init(const ClientCfg & client_cfg);

	int update();

	bool handleOutputPacket(InterPacket * pack);
protected:
private:
	MessageServerPump<InterPacket> * m_message_pump;
	SessionId m_session_id;
};

#endif
