
#ifndef MANAGE_VRPUBLIC_PACKET_HPP__
#define MANAGE_VRPUBLIC_PACKET_HPP__

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "PackMsg.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "inter_generated.h"
#include "EmptyMsg.h"

using namespace MobaGo::FlatBuffer;

class ManageVrpublicPacket
{
public:
	ManageVrpublicPacket();
	~ManageVrpublicPacket();

	static ManageVrpublicPacket * instance();
public:
	int init();
public:
	int handleSMsgGateRegister(InterPacket * packet, const SMsgGateRegister * msg);

	int handleNewSession(InterPacket * packet, const EmptyMsg * msg);

	int handleCloseSession(InterPacket * packet, const EmptyMsg * msg);

	void reportGateInfoToVrpublic();
protected:
private:
	string m_gate_client_ip_addr;
	int m_gate_client_port;

	static ManageVrpublicPacket * m_instance;
};
#endif