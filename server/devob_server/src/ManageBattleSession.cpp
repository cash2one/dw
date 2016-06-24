
#include "ManageBattleSession.h"
#include "Logger.h"

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

	std::string buf("maxnum");
	std::ifstream max_file(comm_dir + buf);
	if (max_file)
	{
		max_file >> m_self_inc;
	}

	
	for (uint32 i = 1; i < m_self_inc; i++)
	{
		ObInfo *info = new ObInfo;
		if (!info->serialize(comm_dir + numtoStr(i)))
			continue;

		m_info.insert(std::make_pair(i, info));
	}
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

void ManageBattleSession::handleNewConnection(SessionId session_id)
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

#define  REGMSG(msgid, msgname, handler) \
	m_manage_server_pump->addMsgHandle(msgid, new PacketHandle<ManageBattleSession, InterPacket, msgname>(this, &ManageBattleSession::##handler))

void ManageBattleSession::registerMsgHandle()
{
	REGMSG(E_SMsgRegisterToDevOb, SMsgRegisterToDevOb, onSMsgRegisterToDevOb);
	REGMSG(E_SMsgRegisterOneBattle, SMsgRegisterOneBattle, onSMsgRegisterOneBattle);
	REGMSG(E_SMsgUpdateBattleFrame, SMsgUpdateBattleFrame, onSMsgUpdateBattleFrame);
	REGMSG(E_SMsgUpdateBattleStatus, SMsgUpdateBattleStatus, onSMsgUpdateBattleStatus);
	REGMSG(E_SMsgRequestObListReq, EmptyMsg, onSMsgRequestObListReq);
}


int ManageBattleSession::onSMsgRequestObListReq(InterPacket * input_packet, const EmptyMsg * input_msg)
{
	FlatBufferBuilder builder;

	std::vector<Offset<PObInfo>> ob_list;
	
	
	for (auto i : m_info)
	{
		auto j = *i.second;
		PObInfoBuilder sub(builder);
		sub.add_ob_id(j.ob_id);
		sub.add_one_team_num(j.one_team_num);
		sub.add_status(j.status);
		std::string showTime;
		formatTimeToString(getSystemMilliSeconds(), showTime);
		sub.add_start_time(builder.CreateString(showTime));
		ob_list.push_back(sub.Finish());
	}
	auto w = builder.CreateVector(ob_list);
	Offset<SMsgRequestObListAck> ack = CreateSMsgRequestObListAck(builder, w);
	builder.Finish(ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgRequestObListAck, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = input_packet->session_id;
	ManageBattleSession::instance()->handleOutputPacket(pack);
	return 0;
}

int ManageBattleSession::onSMsgUpdateBattleStatus(InterPacket * input_packet, const SMsgUpdateBattleStatus * input_msg)
{
	for (auto i : m_info)
	{
		if (i.second->status != 0)
			continue;

		i.second->status = input_msg->status();

		i.second->serialize(comm_dir + numtoStr(i.first));
	}

	return 0;
}

int ManageBattleSession::onSMsgUpdateBattleFrame(InterPacket * input_packet, const SMsgUpdateBattleFrame * input_msg)
{

	for (auto i : m_info)
	{
		if (i.second->battle_id != input_msg->battle_id() ||
			i.second->status != 0)
			continue;

		auto j = input_msg->frame_stream();
		std::vector<int8_t> new_update;
		new_update.resize(j->size());
		std::copy(j->begin(), j->end(), new_update.begin());
		i.second->ob_updates.push_back(std::make_pair(input_msg->frame_no(), new_update));
	}

	return 0;
}

int ManageBattleSession::Inc()
{
	return m_self_inc++;
}

std::string ManageBattleSession::numtoStr(uint32 i)
{
	std::stringstream str;
	str << i;
	return str.str();
}

int ManageBattleSession::onSMsgRegisterOneBattle(InterPacket * input_packet, const SMsgRegisterOneBattle * input_msg)
{	
	bool found = false;
	for (auto i : m_info)
	{
		if (i.second->battle_id != input_msg->battle_id() ||
			i.second->status != 0)
			continue;

		found = true;
	}

	ObInfo *info = new ObInfo;
	info->ob_id = Inc();
	info->battle_id = input_msg->battle_id();
	info->one_team_num = input_msg->one_team_num();
	

	m_info.insert(std::make_pair(info->ob_id, info));

	std::string buf("maxnum");
	std::ofstream max_file(comm_dir + buf);
	if (max_file)
	{
		max_file << m_self_inc;
	}

	return 0;
}


int ManageBattleSession::onSMsgRegisterToDevOb(InterPacket * input_packet, const SMsgRegisterToDevOb * input_msg)
{	
	FlatBufferBuilder builder;
	Offset<String> off_ip = builder.CreateString(TrConfig::instance()->getVrCfg().ip_addr);
	Offset<SMsgObReportGateInfo> ack = CreateSMsgObReportGateInfo(builder, off_ip, TrConfig::instance()->getVrCfg().port);
	builder.Finish(ack);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgObReportGateInfo, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = input_packet->session_id;
	ManageBattleSession::instance()->handleOutputPacket(pack);
	return 0;
}



void ManageBattleSession::addRegisterBattle(SessionId session_id, int battle_server_id, int battle_gate_port, const string & battle_gate_ip)
{
	auto it = m_battle_info_map.find(battle_server_id);
	if (it != m_battle_info_map.end())
	{
		// error
		DEF_LOG_ERROR("get reduplication battle id <%d>", battle_server_id);
		this->removeBattle(battle_server_id);
		m_battle_session_map.erase(it->second->session_id);
		delete it->second;
		m_battle_info_map.erase(it);
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
