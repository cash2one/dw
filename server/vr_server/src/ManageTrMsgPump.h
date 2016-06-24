
#ifndef ManageTrMsgPump_hpp__
#define ManageTrMsgPump_hpp__

#include "MessagePump.h"
#include "Singleton.h"
#include "VrConfig.h"

class ManageTrMsgPump : public MessagePumpHandle
{
public:
	ManageTrMsgPump();
	~ManageTrMsgPump();
	static ManageTrMsgPump * instance();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);

public:
	int init(const TrCfg & cfg_value);

	int update();

	bool handleOutputPacket(InterPacket * pack);
protected:
private:
	MessageServerPump<InterPacket> * m_message_pump;
	SessionId m_session_id;
};

#endif
