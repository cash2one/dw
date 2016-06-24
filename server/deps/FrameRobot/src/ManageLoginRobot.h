
#ifndef MANAGE_LOGIN_ROBOT_HPP__
#define MANAGE_LOGIN_ROBOT_HPP__

#include <string>
#include "Packet_.h"
#include "MessagePump.h"
#include "Robot.h"
#include "login_generated.h"
#include "protocol_generated.h"
#include "PacketHandle.h"
#include "Singleton.h"
#include "ManageRobotConfig.h"
#include "RaknetMessagePump.h"
#include "timer.hpp"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;

class ManageLoginRobot : public MessagePumpHandle, public RobotMessageHandle
{
public:
	ManageLoginRobot();
	~ManageLoginRobot();
	static ManageLoginRobot * instance();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);
public:
	void handleOutputPacket(ClientPacket * packet);
public:
	int init(const string & ip_str, int port, int robot_start_index, int robot_num, int net_type);

	void update();

public:
	int handleMsgServerListAck(ServerPacket * input_packet, const MsgServerListAck * input_msg);
	int handleMsgError(ServerPacket * input_packet, const MsgError * input_msg);
	int handleMsgChooseServerAck(ServerPacket * input_packet, const MsgChooseServerAck * input_msg);

	// 消息格式转换
	int clientInput(int msg_id, ServerPacket* packet, char * buffer, int buffer_size);
	
	void removeRobot(SessionId session_id);
protected:
	void initRobot();

	void registerMsgHandle();

	string getRobotName(int robot_id);

	void addAvilablePlayerInfo(PlayerInfo * player_info);

	void initOneRobot(SessionId session_id);

	PlayerInfo * getAvilablePlayerInfo();

	void addRobot(SessionId session_id, Robot * robot);

	Robot * getRobot(SessionId session_id);

	

	void updateRobot();
private:
	MessageServerPump<ServerPacket> * m_message_pump;

	RobotCfg m_robot_cfg;

	PlayerInfoList_t m_avilable_robot;

	typedef unordered_map<SessionId, Robot *> RobotMap_t;

	RobotMap_t m_robot_session_map;

	Timer m_time_point;
	std::vector<Robot *> m_tobeLogin;
};

#endif
