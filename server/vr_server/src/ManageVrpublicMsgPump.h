
#ifndef ManageVrpublicMsgPump_hpp__
#define ManageVrpublicMsgPump_hpp__

#include "MessagePump.h"
#include "Singleton.h"
#include "VrConfig.h"

class ManageVrpublicMsgPump : public MessagePumpHandle
{
public:
	ManageVrpublicMsgPump();
	~ManageVrpublicMsgPump();
	static ManageVrpublicMsgPump * instance();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);

public:
	int init(const VrpublicCfg & vrp_cfg);

	int update();

	bool handleOutputPacket(InterPacket * pack);
protected:
	void notifyAreaIdToPlugin();
private:
	MessageServerPump<InterPacket> * m_message_pump;
	SessionId m_session_id;
};

#endif
