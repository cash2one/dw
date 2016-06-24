
#ifndef MANAGER_SESSION_HPP__
#define MANAGER_SESSION_HPP__

//#include "SessionPool.h"
#include <string>
#include "SessionServer.h"
#include "Packet_.h"
#include "StatCount.h"
#include "MessagePump.h"
#include "Singleton.h"
#include "inter_generated.h"

using namespace std;

class ManagerSession : public MessagePumpHandle
{
public:
	ManagerSession();
	~ManagerSession();
	static ManagerSession * instance();
public:
	int init(const string & ip_addr, int port);

	int update();

	void addMsgHandle(int msg_id, PacketHandleBase<InterPacket> * msg_handle);

	void handleOutputStream(char * buffer, size_t buf_len);

	void removeSession(SessionId session_id);

	int handleSMsgGateRegister(InterPacket * input_pack, const MobaGo::FlatBuffer::SMsgGateRegister * input_msg);
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);

private:
	MessageServerPump<InterPacket> * m_message_pump;

	SessionId m_session_id;
};

#endif
