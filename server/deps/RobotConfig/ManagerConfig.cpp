
#include "Logger.h"
#include "ManagerConfig.h"

ManagerConfig::ManagerConfig()
	: m_session_server(NULL)
	, m_thread(NULL)
	, m_assign_robot_amount(0)
{

}

ManagerConfig::~ManagerConfig()
{

}

int ManagerConfig::init()
{
	if (m_cfg.load() == -1)
	{
		return -1;
	}

	m_session_server = SessionServer::createSessionServer();
	m_session_server->init(this);
	m_session_server->listen(m_cfg.m_ip_addr, m_cfg.m_port);

	DEF_LOG_INFO("start at <%s:%d>", m_cfg.m_ip_addr.c_str(), m_cfg.m_port);

	boost::function0<int> f = boost::bind(&ManagerConfig::svc, this);
	m_thread = new boost::thread(f);

	return 0;
}

int ManagerConfig::svc()
{
	boost::posix_time::milliseconds empty_sleep_time(10);
	ClientPacketVec_t packet_vec;
	while (true)
	{
		{
			boost::mutex::scoped_lock lock(m_input_packet_mutex);
			std::copy(m_input_packet.begin(), m_input_packet.end(), std::back_inserter(packet_vec));
			m_input_packet.clear();
		}

		for (auto pack : packet_vec)
		{
			this->handlePacket(pack);
		}
		packet_vec.clear();

		boost::this_thread::sleep(empty_sleep_time);
	}
	return 0;
}

void ManagerConfig::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{

}

void ManagerConfig::handleNewConnectionFailed()
{

}

void ManagerConfig::handleCloseConnection(SessionId session_id, int error_code)
{

}

size_t ManagerConfig::handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	ClientHead msg_head;
	ClientPacket * packet;
	size_t pos = 0;
	while (ClientPacket::pack_head_size <= (int)(buf_len - pos))
	{
		memcpy(&msg_head, buffer + pos, ClientPacket::pack_head_size);
		if (msg_head.msg_len + ClientPacket::pack_head_size <= (int)(buf_len - pos))
		{
			packet = new ClientPacket();
			// make packet
			packet->setMsgInfo(msg_head, buffer + pos + ClientPacket::pack_head_size);
			pos += ClientPacket::pack_head_size + msg_head.msg_len;
			packet->session_id = session_id;
			//MessageManager::instance()->handlePlayerMsg((int)session, packet);
			this->handleInputPacket(packet);
		}
		else
		{
			// 
			break;
		}
	}

	return pos;
}

void ManagerConfig::handleInputPacket(ClientPacket * packet)
{
	boost::mutex::scoped_lock lock(m_input_packet_mutex);
	m_input_packet.push_back(packet);
}

void ManagerConfig::handlePacket(ClientPacket * packet)
{
	if (Robot_Protocol_Msg_RequestRobotConfig == packet->msg_head.msg_id)
	{
		this->handleRequestRobotConfig(packet);
	}

}

void ManagerConfig::handleRequestRobotConfig(ClientPacket * packet)
{
	BattleConfig * battle_config = m_cfg.m_battle_config_que.top();
	m_cfg.m_battle_config_que.pop();
	
	battle_config->addRobotNum(m_cfg.m_client_run_robots_num);
	m_cfg.m_battle_config_que.push(battle_config);

	flatbuffers::FlatBufferBuilder builder;
	Offset<String> off_ip_addr = builder.CreateString(battle_config->getIpAddr().c_str());
	Offset<RequestRobotConfigAck> req_cfg_ack = CreateRequestRobotConfigAck(builder, off_ip_addr, battle_config->getPort(), m_cfg.m_robot_start_id + m_assign_robot_amount, m_cfg.m_client_run_robots_num, m_cfg.m_net_type);
	builder.Finish(req_cfg_ack);

	ClientPacket output_pack;
	output_pack.msg_head.msg_id = Robot_Protocol_Msg_RequestRobotConfigAck;
	output_pack.msg_head.msg_len = builder.GetSize();
	output_pack.setMsgInfo(output_pack.msg_head, (char *)builder.GetBufferPointer());

	m_session_server->handleOutputStream(packet->getSession(), output_pack.getStream(), output_pack.getStreamSize());

	output_pack.cleanStream();

	DEF_LOG_INFO("get request, ip is <%s> port is <%d>, robot start number is <%d>, client run robot number is <%d>", battle_config->getIpAddr().c_str(), battle_config->getPort(), m_cfg.m_robot_start_id + m_assign_robot_amount, m_cfg.m_client_run_robots_num);

	m_assign_robot_amount += m_cfg.m_client_run_robots_num;
}
