
#include "Logger.h"
#include "ManageLoginRobot.h"
#include "ManageVrRobot.h"
#include "Single.hpp"
#include "pluginDepot.h"
#include "RaknetMessagePump.h"

#define GET_ROBOT_AND_RETURN()	\
	Robot * robot = this->getRobot(input_packet->session_id);	\
	if (NULL == robot)	\
	{	\
		return -1;	\
	}

ManageLoginRobot::ManageLoginRobot()
	: m_message_pump(NULL)
{

}

ManageLoginRobot::~ManageLoginRobot()
{

}

ManageLoginRobot * ManageLoginRobot::instance()
{
	return Singleton<ManageLoginRobot>::instance();
}

void ManageLoginRobot::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	this->initOneRobot(session_id);
}

void ManageLoginRobot::handleCloseConnection(SessionId session_id)
{

}

void ManageLoginRobot::handleOutputPacket(ClientPacket * packet)
{
	m_message_pump->handleOutputStream(packet->session_id, packet->getStream(), packet->getStreamSize());
	packet->cleanStream();
	delete packet;
}

int ManageLoginRobot::init(const string & ip_str, int port, int robot_start_index, int robot_num, int net_type)
{
	m_robot_cfg.ip_addr = ip_str;
	m_robot_cfg.port = port;
	m_robot_cfg.robot_start_index = robot_start_index;
	m_robot_cfg.robot_num = robot_num;
	m_robot_cfg.net_type = net_type;

	//m_message_pump = MessageServerPump<ServerPacket>::createMessageServerPump();
	m_message_pump = RaknetMessagePump::createMessagePump(ManageRobotConfig::instance()->getRobotCfg().net_type);
	m_message_pump->init(this, m_robot_cfg.net_type);

	this->registerMsgHandle();

	this->initRobot();

	m_message_pump->setDefaultMsgHandler(std::bind(&RunningRobot::packet_input, Single<RunningRobot>::GetInstance(),
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	return 0;
}

void ManageLoginRobot::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_message_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	this->updateRobot();
}

int ManageLoginRobot::handleMsgServerListAck(ServerPacket * input_packet, const MsgServerListAck * input_msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgServerListAck(input_packet->session_id, input_msg);

	return 0;
}

int ManageLoginRobot::handleMsgError(ServerPacket * input_packet, const MsgError * input_msg)
{
	GET_ROBOT_AND_RETURN();

	DEF_LOG_ERROR("get error msg, user id is <%llu>, error code is <%d>", robot->getPlayerInfo()->user_id, input_msg->error_code());
	return 0;
}

int ManageLoginRobot::clientInput(int msg_id, ServerPacket* packet, char * buffer, int buffer_size)
{
	std::unique_ptr<InterPacket> dest(new InterPacket);
	dest->session_id = packet->session_id;
	dest->setMsgInfo(msg_id, 0, buffer_size, buffer);

	return Single<PluginDepot>::GetInstance()->input(msg_id, dest.get(), buffer, buffer_size);
}

int ManageLoginRobot::handleMsgChooseServerAck(ServerPacket * input_packet, const MsgChooseServerAck * input_msg)
{
	GET_ROBOT_AND_RETURN();
	string server_ip = input_msg->ip() != NULL ? input_msg->ip()->c_str() : "";
	string session_key = input_msg->session_key() != NULL ? input_msg->session_key()->c_str() : "";

	this->removeRobot(input_packet->session_id);

	robot->setAccountId(input_msg->user_id());
	uint64 user_id = input_msg->user_id();
	robot->setVrServerInfo(input_msg->ip() != NULL ? input_msg->ip()->c_str() : "", input_msg->port(), input_msg->session_key() != NULL ? input_msg->session_key()->c_str() : "");

	robot->setRobotStatus(RS_Connected_vr);

	ManageVrRobot::instance()->handleRobot(robot);
	return 0;
}


void ManageLoginRobot::initRobot()
{	
	auto cfg = Single<RobotCfg>::GetInstance();
	for (int i = cfg->account_index_end; i >= cfg->account_index_begin; i--)
	{
		PlayerInfo * player_info = new PlayerInfo();
		char name[100] = { 0 };
		sprintf(name, "robot_%d", i);
		player_info->account = name;

		Robot * robot = new Robot();
		robot->m_player_info = player_info;
		m_tobeLogin.push_back(robot);
	}
}

void ManageLoginRobot::registerMsgHandle()
{
	m_message_pump->addMsgHandle(Opcode_E_MsgServerListAck, new PacketHandle<ManageLoginRobot, ServerPacket, MsgServerListAck>(this, &ManageLoginRobot::handleMsgServerListAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgError, new PacketHandle<ManageLoginRobot, ServerPacket, MsgError>(this, &ManageLoginRobot::handleMsgError));
	m_message_pump->addMsgHandle(Opcode_E_MsgChooseServerAck, new PacketHandle<ManageLoginRobot, ServerPacket, MsgChooseServerAck>(this, &ManageLoginRobot::handleMsgChooseServerAck));
}

string ManageLoginRobot::getRobotName(int robot_id)
{
	char name[100];
	sprintf(name, "robot_%d", robot_id);
	return name;
}

void ManageLoginRobot::addAvilablePlayerInfo(PlayerInfo * player_info)
{
	m_avilable_robot.push_back(player_info);
}

void ManageLoginRobot::initOneRobot(SessionId session_id)
{	
	if (m_tobeLogin.empty())
		return;

	Robot * robot = m_tobeLogin.back();
	m_tobeLogin.pop_back();

	this->addRobot(session_id, robot);
	Single<RunningRobot>::GetInstance()->running[session_id] = robot;

	robot->init(session_id, this);
	robot->setRobotStatus(RS_Connected_vrpublic);
}

PlayerInfo * ManageLoginRobot::getAvilablePlayerInfo()
{
	PlayerInfo * player_info = NULL;
	if (m_avilable_robot.size() > 0)
	{
		player_info = m_avilable_robot.front();
		m_avilable_robot.pop_front();
	}

	return player_info;
}

void ManageLoginRobot::addRobot(SessionId session_id, Robot * robot)
{
	m_robot_session_map.insert(std::make_pair(session_id, robot));
}

Robot * ManageLoginRobot::getRobot(SessionId session_id)
{
	auto it = m_robot_session_map.find(session_id);
	if (it != m_robot_session_map.end())
	{
		return it->second;
	}

	return NULL;
}

void ManageLoginRobot::removeRobot(SessionId session_id)
{
	auto it = m_robot_session_map.find(session_id);
	if (it != m_robot_session_map.end())
	{
		m_robot_session_map.erase(it);
	}
}

void ManageLoginRobot::updateRobot()
{
	for (auto it : m_robot_session_map)
	{
		Robot * robot = it.second;
		robot->update();
	}

	auto cfg = Single<RobotCfg>::GetInstance();
	if (m_time_point.elapsed_second() > cfg->login_interval)
	{

		int num = std::min((int)cfg->login_once_num, (int)m_tobeLogin.size());
		for (int i = 0; i < num; i++)
			m_message_pump->connect(m_robot_cfg.ip_addr, m_robot_cfg.port);

		m_time_point.reset();
	}
}

