
#include "ManageBattleSession.h"
#include "Logger.h"
#include "check_macro.h"
#include "ManagePlayerTrInfo.h"
#include "ManageVrSession.h"

ManageBattleSession::ManageBattleSession()
	: m_manage_server_pump(NULL)
{

}

ManageBattleSession::~ManageBattleSession()
{

}

ManageBattleSession * ManageBattleSession::instance()
{
	return Singleton<ManageBattleSession>::instance();
}

int ManageBattleSession::init(const BattleCfg & vr_cfg)
{
	m_battle_cfg = vr_cfg;

	m_manage_server_pump = MessageServerPump<InterPacket>::createMessageServerPump();
	m_manage_server_pump->init(this, NPT_TCP);
	m_manage_server_pump->listen(m_battle_cfg.ip_addr, m_battle_cfg.port);

	this->registerMsgHandle();

	return 0;
}

int ManageBattleSession::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_manage_server_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	return dispatch_pack_num;
}

BattleInfo * ManageBattleSession::getAvilableBattle()
{
	if (m_battle_info_map.size() == 0)
	{
		return NULL;
	}

	return m_battle_info_map.begin()->second;
}

void ManageBattleSession::handleOutputPacket(InterPacket * packet)
{
	m_manage_server_pump->handleOutputStream(packet->session_id, packet->getStream(), packet->getStreamSize());
	packet->cleanStream();
	delete packet;
}

void ManageBattleSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{


}

void ManageBattleSession::handleCloseConnection(SessionId session_id)
{
	auto it = m_battle_session_map.find(session_id);
	if (it != m_battle_session_map.end())
	{
		this->removeBattle(it->second->battle_server_id);
	}
}

int ManageBattleSession::handleSMsgBattleRegisterTr(InterPacket * input_packet, const SMsgBattleRegisterTr * input_msg)
{
	string battle_gate_ip = input_msg->battle_gate_ip() != NULL ? input_msg->battle_gate_ip()->c_str() : "";
	this->addRegisterBattle(input_packet->session_id, input_msg->battle_server_id(), input_msg->battle_gate_port(), battle_gate_ip);
	return 0;
}

int ManageBattleSession::handleSMsgBattleEndNotifyVr(InterPacket * input_packet, const SMsgBattleEndNotifyVr * input_msg)
{
	CK_TRUE_RETURN(input_msg->player_battle_result_list() == 0, -1);

	DEF_LOG_DEBUG("recv SMsgBattleEndNotifyVr, win team id is <%d>, player size is <%d>", input_msg->win_team_id(), input_msg->player_battle_result_list()->size());

	for (int i = 0; i < input_msg->player_battle_result_list()->size(); ++i)
	{
		const PPlayerBattleResult * player_battle_result = input_msg->player_battle_result_list()->Get(i);
		PlayerTrInfo * player_tr_info = ManagePlayerTrInfo::instance()->getPlayer(player_battle_result->account_id());
		CK_TRUE_CMD(NULL == player_tr_info, continue);
		
		InterPacket * pack = input_packet->clone();
		pack->updateGuid(player_tr_info->getAccountId());
		pack->session_id = player_tr_info->getSessionId();
		ManageVrSession::instance()->handleOutputPacket(pack);

		ManageMatch::instance()->playerBattleFinish(player_tr_info);
	}

	return 0;
}

int ManageBattleSession::handleSMsgNotifyBattleMatchResultAck(InterPacket * input_packet, const SMsgNotifyBattleMatchResultAck * input_msg)
{
	ManageMatch::instance()->handleSMsgNotifyBattleMatchResultAck(input_msg->match_id(), input_msg->result());
	return 0;
}

void ManageBattleSession::registerMsgHandle()
{
	m_manage_server_pump->addMsgHandle(E_SMsgBattleRegisterTr, new PacketHandle<ManageBattleSession, InterPacket, SMsgBattleRegisterTr>(this, &ManageBattleSession::handleSMsgBattleRegisterTr));
	m_manage_server_pump->addMsgHandle(E_SMsgBattleEndNotifyVr, new PacketHandle<ManageBattleSession, InterPacket, SMsgBattleEndNotifyVr>(this, &ManageBattleSession::handleSMsgBattleEndNotifyVr));

	m_manage_server_pump->addMsgHandle(E_SMsgNotifyBattleMatchResultAck, new PacketHandle<ManageBattleSession, InterPacket, SMsgNotifyBattleMatchResultAck>(this, &ManageBattleSession::handleSMsgNotifyBattleMatchResultAck));
}

void ManageBattleSession::addRegisterBattle(SessionId session_id, int battle_server_id, int battle_gate_port, const string & battle_gate_ip)
{
	auto it = m_battle_info_map.find(battle_server_id);
	if (it != m_battle_info_map.end())
	{
		// error
		DEF_LOG_ERROR("get reduplication battle id <%d>", battle_server_id);
		this->removeBattle(battle_server_id);
		//m_battle_session_map.erase(it->second->session_id);
		//delete it->second;
		//m_battle_info_map.erase(it);
	}

	DEF_LOG_INFO("get new battle <%d>, gate info is <%s:%d>", battle_server_id, battle_gate_ip.c_str(), battle_gate_port);

	BattleInfo * battle_info = new BattleInfo();
	battle_info->battle_server_id = battle_server_id;
	battle_info->battle_gate_ip = battle_gate_ip;
	battle_info->battle_gate_port = battle_gate_port;
	battle_info->session_id = session_id;

	m_battle_info_map[battle_info->battle_server_id] = battle_info;
	m_battle_session_map[battle_info->session_id] = battle_info;
}

void ManageBattleSession::removeBattle(int battle_server_id)
{
	auto it = m_battle_info_map.find(battle_server_id);
	if (it != m_battle_info_map.end())
	{
		// 
		DEF_LOG_INFO("remove battle info <%d>", battle_server_id);
		BattleInfo * battle_info = it->second;
		DEF_LOG_ERROR("remove battle <%d>", battle_server_id);
		m_battle_session_map.erase(battle_info->session_id);
		m_battle_info_map.erase(it);

		delete battle_info;
	}
	else
	{
		DEF_LOG_ERROR("do not find battle <%d> while remove the battle", battle_server_id);
	}
}
