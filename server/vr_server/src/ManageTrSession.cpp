
#include "ManageTrSession.h"
#include "MessageMaker.h"
#include "ManageAccount.h"

ManageTrSession * ManageTrSession::m_instance = NULL;

ManageTrSession::ManageTrSession()
{

}

ManageTrSession::~ManageTrSession()
{

}

ManageTrSession * ManageTrSession::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageTrSession();
	}

	return m_instance;
}

int ManageTrSession::init(const TrCfg & tr_cfg)
{
	m_message_pump = MessageServerPump<InterPacket>::createMessageServerPump();
	m_message_pump->init(this, NPT_TCP);
	m_message_pump->connect(tr_cfg.ip_addr, tr_cfg.port, true);

	m_message_pump->addMsgHandle(0, new PacketHandle<ManageTrSession, InterPacket, MsgLobbyKeepAliveReq>(this, &ManageTrSession::handleMsgLobbyKeepAliveReq));

	m_message_pump->addMsgHandle(Opcode_E_MsgMatchAck, new PacketHandle<ManageTrSession, InterPacket, MsgMatchAck>(this, &ManageTrSession::handleMsgMatchAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgCancelMatchAck, new PacketHandle<ManageTrSession, InterPacket, MsgCancelMatchAck>(this, &ManageTrSession::handleMsgCancelMatchAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgMatchEnterBattle, new PacketHandle<ManageTrSession, InterPacket, MsgMatchEnterBattle>(this, &ManageTrSession::handleMsgMatchEnterBattle));

	m_message_pump->addMsgHandle(E_SMsgBattleEndNotifyVr, new PacketHandle<ManageTrSession, InterPacket, SMsgBattleEndNotifyVr>(this, &ManageTrSession::handleSMsgBattleEndNotifyVr));

	return 0;
}

int ManageTrSession::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_message_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	return dispatch_pack_num;

}

void ManageTrSession::handleOutputPacket(InterPacket * pack)
{
	m_message_pump->handleOutputStream(m_tr_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();
	delete pack;
}

int ManageTrSession::handleMsgLobbyKeepAliveReq(InterPacket * input_packet, const MsgLobbyKeepAliveReq * input_msg)
{
	return 0;
}

int ManageTrSession::handleMsgMatchAck(InterPacket * input_packet, const MsgMatchAck * input_msg)
{
	DEF_LOG_DEBUG("recv MsgMatchAck, guid is <%llu>", input_packet->msg_head.guid);

	Account * account = ManageAccount::GetInstance()->getAccount(input_packet->msg_head.guid);
	if (NULL == account)
	{
		DEF_LOG_ERROR("do not find accout by account id <%llu>", input_packet->msg_head.guid);
		return -1; 
	}
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(input_packet->msg_head.msg_id, account->client_id, input_packet->msg_head.msg_len, input_packet->getBody());

	ManagerClientSession::instance()->handleOutputStream(account->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();
	delete pack;

	return 0;
}

int ManageTrSession::handleMsgCancelMatchAck(InterPacket * input_packet, const MsgCancelMatchAck * input_msg)
{
	DEF_LOG_DEBUG("recv MsgCancelMatchAck, guid is <%llu>", input_packet->msg_head.guid);

	Account * account = ManageAccount::GetInstance()->getAccount(input_packet->msg_head.guid);
	if (NULL == account)
	{
		DEF_LOG_ERROR("do not find accout by account id <%llu>", input_packet->msg_head.guid);
		return -1;
	}
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(input_packet->msg_head.msg_id, account->client_id, input_packet->msg_head.msg_len, input_packet->getBody());

	ManagerClientSession::instance()->handleOutputStream(account->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();
	delete pack;

	return 0;
}

int ManageTrSession::handleMsgMatchEnterBattle(InterPacket * input_packet, const MsgMatchEnterBattle * input_msg)
{
	DEF_LOG_DEBUG("recv MsgMatchEnterBattle, guid is <%llu>", input_packet->msg_head.guid);

	Account * account = ManageAccount::GetInstance()->getAccount(input_packet->msg_head.guid);
	if (NULL == account)
	{
		DEF_LOG_ERROR("do not find accout by account id <%llu>", input_packet->msg_head.guid);
		return -1;
	}
	InterPacket * pack = new InterPacket();
	
	pack->setMsgInfo(input_packet->msg_head.msg_id, account->client_id, input_packet->msg_head.msg_len, input_packet->getBody());

	ManagerClientSession::instance()->handleOutputStream(account->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();
	delete pack;

	return 0;
}

int ManageTrSession::handleSMsgBattleEndNotifyVr(InterPacket * input_packet, const SMsgBattleEndNotifyVr * input_msg)
{
	return 0;
}

void ManageTrSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	m_tr_info.session_id = session_id;
	DEF_LOG_INFO("get new tr session <%x>", session_id);

	this->registerVrToTr();
}

void ManageTrSession::handleCloseConnection(SessionId session_id)
{
	DEF_LOG_INFO("get close tr session <%x>", session_id);
	if (session_id == m_tr_info.session_id)
	{
		m_tr_info.session_id = NULL;
		DEF_LOG_INFO("clear tr session ");
	}
}

void ManageTrSession::registerVrToTr()
{
	InterPacket * pack = MessageMaker::makeSMsgVrRegisterTr(m_tr_info.session_id, VrConfig::instance()->getVrCfg().area_id, VrConfig::instance()->getVrCfg().server_id);
	m_message_pump->handleOutputStream(m_tr_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
}
