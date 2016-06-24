
#include "ManageTrSession.h"
#include "opcode.h"
#include "Singleton.h"
#include "inter_generated.h"
#include "BattleManager.h"

ManageTrSession::ManageTrSession()
	: m_message_pump(NULL)
	, m_gate_port(0)
{

}

ManageTrSession::~ManageTrSession()
{

}

ManageTrSession * ManageTrSession::instance()
{
	return Singleton<ManageTrSession>::instance();
}

int ManageTrSession::init(const TrCfg & tr_cfg)
{
	m_message_pump = MessageServerPump<InterPacket>::createMessageServerPump();
	m_message_pump->init(this, NPT_TCP);
	m_message_pump->connect(tr_cfg.ip_addr, tr_cfg.port, true);

	this->registerMsgHandle();

	return 0;
}

int ManageTrSession::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_message_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	return dispatch_pack_num;
}

void ManageTrSession::updateGateInfo(const string & gate_ip, int gate_port)
{
	m_gate_ip = gate_ip;
	m_gate_port = gate_port;

	this->registerToTr();
}

void ManageTrSession::handleOutputPacket(InterPacket * packet)
{
	if (NULL != m_tr_info.session_id)
	{
		m_message_pump->handleOutputStream(m_tr_info.session_id, packet->getStream(), packet->getStreamSize());
		packet->cleanStream();
	}
	else
	{
		DEF_LOG_ERROR("tr is not avilable");
	}

	delete packet;
}

void ManageTrSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	m_tr_info.session_id = session_id;
	this->registerToTr();
}

void ManageTrSession::handleCloseConnection(SessionId session_id)
{
	if (session_id == m_tr_info.session_id)
	{
		m_tr_info.session_id = NULL;
	}
}

int ManageTrSession::handleSMsgNotifyBattleMatchResult(InterPacket * input_pack, const MobaGo::FlatBuffer::SMsgNotifyBattleMatchResult * input_msg)
{
	// todo
	BattleManager::instance()->handleSMsgNotifyBattleMatchResult(input_pack, input_msg);
	return 0;
}

int ManageTrSession::handleSMsgVrReciveBattleReportNotifyBattle(InterPacket * input_pack, const EmptyMsg * input_msg)
{
	BattleManager::instance()->handleSMsgVrReciveBattleReportNotifyBattle(input_pack, input_msg);

	return 0;
}

void ManageTrSession::registerMsgHandle()
{
	//m_message_pump->addMsgHandle(E_Msg)
	m_message_pump->addMsgHandle(E_SMsgNotifyBattleMatchResult, new PacketHandle<ManageTrSession, InterPacket, SMsgNotifyBattleMatchResult>(this, &ManageTrSession::handleSMsgNotifyBattleMatchResult));

	m_message_pump->addMsgHandle(E_SMsgVrReciveBattleReportNotifyBattle, new PacketHandle<ManageTrSession, InterPacket, EmptyMsg>(this, &ManageTrSession::handleSMsgVrReciveBattleReportNotifyBattle));

	
}

void ManageTrSession::registerToTr()
{
	if (m_gate_ip.size() == 0)
	{
		return;
	}

	InterPacket * pack = this->createSMsgBattleRegisterTr(BattleConfig::instance()->getBattleCfg().battle_id, m_gate_ip, m_gate_port);

	m_message_pump->handleOutputStream(m_tr_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
}

InterPacket * ManageTrSession::createSMsgBattleRegisterTr(int battle_id, const string & ip_addr, int port)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> off_ip = builder.CreateString(ip_addr);
	flatbuffers::Offset<MobaGo::FlatBuffer::SMsgBattleRegisterTr> off_reg = MobaGo::FlatBuffer::CreateSMsgBattleRegisterTr(builder, battle_id, off_ip, port);

	builder.Finish(off_reg);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgBattleRegisterTr, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = m_tr_info.session_id;

	return pack;
}
