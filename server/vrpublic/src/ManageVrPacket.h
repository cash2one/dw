
#ifndef MANAGE_VR_PACKET_HPP__
#define MANAGE_VR_PACKET_HPP__

#include "inter_generated.h"
#include "EmptyMsg.h"
#include "Packet_.h"

using namespace MobaGo::FlatBuffer;

class ManageVrPacket
{
public:
	ManageVrPacket();
	~ManageVrPacket();
public:
	static ManageVrPacket * instance();
public:
	int init();

	int handleSMsgVrRegisterVrpublic(InterPacket * packet, const SMsgVrRegisterVrpublic * msg);

	int handleVrCloseSession(InterPacket * packet, const EmptyMsg * msg);

protected:
private:
	static ManageVrPacket * m_instance;
};

#endif
