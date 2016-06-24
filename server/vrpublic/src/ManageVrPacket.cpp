
#include "ManageVrPacket.h"
#include "opcode.h"
#include "VrMessageManager.h"
#include "ManageVrSession.h"
#include "Logger.h"

ManageVrPacket * ManageVrPacket::m_instance = nullptr;

ManageVrPacket::ManageVrPacket()
{

}

ManageVrPacket::~ManageVrPacket()
{

}

ManageVrPacket * ManageVrPacket::instance()
{
	if (nullptr == m_instance)
	{
		m_instance = new ManageVrPacket();
	}

	return m_instance;
}

int ManageVrPacket::init()
{
	VrMessageManager::instance()->addVrMsgHandle(E_SMsgVrRegisterVrpublic, 
		new PackMsg<ManageVrPacket, SMsgVrRegisterVrpublic>(this, NULL, &ManageVrPacket::handleSMsgVrRegisterVrpublic));

	VrMessageManager::instance()->addVrMsgHandle(MSG_CLOSE_CLIENT_SESSION,
		new PackMsg<ManageVrPacket, EmptyMsg>(this, NULL, &ManageVrPacket::handleVrCloseSession));

	return 0;
}

int ManageVrPacket::handleSMsgVrRegisterVrpublic(InterPacket * packet, const SMsgVrRegisterVrpublic * msg)
{
	DEF_LOG_INFO("get new vr, area id is <%d>, server id is <%d>, gate ip is <%s:%d>", msg->area_id(), msg->server_id(), msg->gate_ip_addr()->c_str(), msg->gate_port());

	ManageVrSession::instance()->addNewVr(packet->session_id, msg->area_id(), msg->server_id(), msg->gate_ip_addr()->c_str(), msg->gate_port());
	return 0;
}

int ManageVrPacket::handleVrCloseSession(InterPacket * packet, const EmptyMsg * msg)
{
	ManageVrSession::instance()->removeVr(packet->session_id);
	return 0;
}
