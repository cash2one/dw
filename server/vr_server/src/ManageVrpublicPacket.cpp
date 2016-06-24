
#include "ManageVrpublicPacket.h"
#include "ManageVrpublicMessage.h"
#include "opcode.h"
#include "MessageMaker.h"
#include "VrConfig.h"
#include "ManageVrpublicSession.h"

ManageVrpublicPacket * ManageVrpublicPacket::m_instance = NULL;

ManageVrpublicPacket::ManageVrpublicPacket()
{

}

ManageVrpublicPacket::~ManageVrpublicPacket()
{

}

ManageVrpublicPacket * ManageVrpublicPacket::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageVrpublicPacket();
	}

	return m_instance;
}

int ManageVrpublicPacket::init()
{
	ManageVrpublicMessage::instance()->addInputMsgHandle(E_SMsgGateRegister,
		new PackMsg<ManageVrpublicPacket, MobaGo::FlatBuffer::SMsgGateRegister>(this, NULL, &ManageVrpublicPacket::handleSMsgGateRegister));

	ManageVrpublicMessage::instance()->addInputMsgHandle(MSG_NEW_CLIENT_SESSION,
		new PackMsg<ManageVrpublicPacket, EmptyMsg>(this, NULL, &ManageVrpublicPacket::handleNewSession));

	ManageVrpublicMessage::instance()->addInputMsgHandle(MSG_CLOSE_CLIENT_SESSION,
		new PackMsg<ManageVrpublicPacket, EmptyMsg>(this, NULL, &ManageVrpublicPacket::handleCloseSession));

	return 0;
}

int ManageVrpublicPacket::handleSMsgGateRegister(InterPacket * packet, const SMsgGateRegister * msg)
{
	m_gate_client_ip_addr = msg->ip_addr()->c_str();
	m_gate_client_port = msg->port();

	this->reportGateInfoToVrpublic();
	return 0;
}

int ManageVrpublicPacket::handleNewSession(InterPacket * packet, const EmptyMsg * msg)
{
	ManageVrpublicSession::instance()->getNewSession(packet->session_id);

	this->reportGateInfoToVrpublic();
	return 0;
}

int ManageVrpublicPacket::handleCloseSession(InterPacket * packet, const EmptyMsg * msg)
{
	ManageVrpublicSession::instance()->getCloseSession(packet->session_id);

	return 0;
}

void ManageVrpublicPacket::reportGateInfoToVrpublic()
{
	if (m_gate_client_ip_addr.size() == 0)
	{
		return;
	}

	DEF_LOG_INFO("report gate info to vrpublic, gate ip is <%s>, gate port is <%d>", m_gate_client_ip_addr.c_str(), m_gate_client_port);

	InterPacket * pack = MessageMaker::makeSMsgVrRegisterVrpublic(NULL, VrConfig::instance()->getVrCfg().area_id, VrConfig::instance()->getVrCfg().server_id, m_gate_client_ip_addr, m_gate_client_port);

	ManageVrpublicSession::instance()->handleOutputPacket(pack);
}
