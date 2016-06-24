
#include "ManagerSession.h"
#include "inter_generated.h"
#include "opcode.h"
#include "ManageTrSession.h"

ManagerSession::ManagerSession()
	: m_message_pump(NULL)
	, m_session_id(NULL)
{

}

ManagerSession::~ManagerSession()
{

}

ManagerSession * ManagerSession::instance()
{
	return Singleton<ManagerSession>::instance();
}

int ManagerSession::init(const string & ip_addr, int port)
{
	BATTLE_LOG_INFO("gate ip is <%s>, port is <%d>", ip_addr.c_str(), port);

	m_message_pump = MessageServerPump<InterPacket>::createMessageServerPump();
	m_message_pump->init(this, NPT_TCP);
	m_message_pump->connect(ip_addr, port, true);

	m_message_pump->addMsgHandle(E_SMsgGateRegister, new PacketHandle<ManagerSession, InterPacket, MobaGo::FlatBuffer::SMsgGateRegister>(this, &ManagerSession::handleSMsgGateRegister));

	return 0;
}

int ManagerSession::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_message_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	return dispatch_pack_num;
}

void ManagerSession::addMsgHandle(int msg_id, PacketHandleBase<InterPacket> * msg_handle)
{
	m_message_pump->addMsgHandle(msg_id, msg_handle);
}

void ManagerSession::handleOutputStream(char * buffer, size_t buf_len)
{
	m_message_pump->handleOutputStream(m_session_id, buffer, buf_len);
}

void ManagerSession::removeSession(SessionId session_id)
{
	// todo
}

int ManagerSession::handleSMsgGateRegister(InterPacket * input_pack, const MobaGo::FlatBuffer::SMsgGateRegister * input_msg)
{
	string gate_ip = input_msg->ip_addr() != NULL ? input_msg->ip_addr()->c_str() : "";
	ManageTrSession::instance()->updateGateInfo(gate_ip, input_msg->port());

	return 0;
}

void ManagerSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	m_session_id = session_id;
}

void ManagerSession::handleCloseConnection(SessionId session_id)
{
	if (session_id == m_session_id)
	{
		m_session_id = NULL;
	}
}


