
#ifndef MANAGECLIENTPACKET_HPP__
#define MANAGECLIENTPACKET_HPP__

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "PackMsg.h"
#include "protocol_generated.h"
#include "login_generated.h"
//#include "msg_error_generated.h"
#include "SessionServer.h"
#include "ManageServerList.h"
#include "Account.h"
#include "ManageVrSession.h"
#include "EmptyMsg.h"

using namespace MobaGo::FlatBuffer;

class ManageClientPacket
{
public:
	ManageClientPacket();
	~ManageClientPacket();
public:
	static ManageClientPacket * instance();
public:
	int init();
public:

	int handleMsgLoginReq(InterPacket * input_pack, const MsgLoginReq * msg);
	int handleMsgChooseServerReq(InterPacket * input_pack, const MsgChooseServerReq * msg);
	int handleSMsgVrpublicNotifyVrLoginInfoAck(InterPacket * input_pack, const EmptyMsg * msg);
	int handleSMsgClientClosedGateNotifyServer(InterPacket * input_pack, const EmptyMsg * msg);

protected:
private:
	static ManageClientPacket * m_instance;
};
#endif
