
#include "Logger.h"
#include "ManageRobotConfig.h"
#include "Singleton.h"

ManageRobotConfig::ManageRobotConfig()
	: m_get_robot_cfg(false)
{

}

ManageRobotConfig::~ManageRobotConfig()
{

}

ManageRobotConfig * ManageRobotConfig::instance()
{
	return Singleton<ManageRobotConfig>::instance();
}

void ManageRobotConfig::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	ClientHead head;
	head.msg_id = Robot_Protocol_Msg_RequestRobotConfig;
	head.msg_len = 0;

	char * buf = new char[sizeof(head)];
	memcpy(buf, &head, sizeof(head));
	m_session_server->handleOutputStream(session_id, buf, sizeof(head));
}

void ManageRobotConfig::handleNewConnectionFailed()
{
	DEF_LOG_INFO("connected to robot config server");
}

void ManageRobotConfig::handleCloseConnection(SessionId session_id, int error_code)
{

}

size_t ManageRobotConfig::handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
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


void ManageRobotConfig::init(const string & ip_addr, int port, int net_type)
{
	m_session_server = SessionServer::createSessionServer((NetProtocolType)net_type);
	m_session_server->init(this);
	m_session_server->connect(ip_addr, port);
}

bool ManageRobotConfig::isGotRobotCfg()
{
	return m_get_robot_cfg;
}

RobotCfg & ManageRobotConfig::getRobotCfg()
{
	return m_robot_cfg;
}

void ManageRobotConfig::handleInputPacket(ClientPacket * packet)
{
	if (Robot_Protocol_Msg_RequestRobotConfigAck == packet->msg_head.msg_id)
	{
		this->handleReqCfgAck(packet);
	}
}

void ManageRobotConfig::handleReqCfgAck(ClientPacket * packet)
{
	const RequestRobotConfigAck * req_cfg_ack = flatbuffers::GetRoot<RequestRobotConfigAck>(packet->getBody());

	flatbuffers::Verifier verifier((uint8_t *)packet->getBody(), packet->msg_head.msg_len);

	if (!req_cfg_ack->Verify(verifier))
	{
		DEF_LOG_ERROR("failed to verfiy the message : RequestRobotConfigAck, len is <%hd>", packet->msg_head.msg_len);
		return;
	}

	m_robot_cfg.ip_addr = req_cfg_ack->ip_addr()->c_str();
	m_robot_cfg.port = req_cfg_ack->port();
	m_robot_cfg.net_type = req_cfg_ack->net_type();
	m_robot_cfg.robot_start_index = req_cfg_ack->start_id();
	m_robot_cfg.robot_num = req_cfg_ack->run_robot_num();

	m_get_robot_cfg = true;
}
