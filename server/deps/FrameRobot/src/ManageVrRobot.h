
#ifndef MANAGE_VR_ROBOT_HPP__
#define MANAGE_VR_ROBOT_HPP__

#include "Singleton.h"
#include "Robot.h"
#include "MessagePump.h"
#include "protocol_generated.h"
#include "login_generated.h"
#include "match_generated.h"
#include "EmptyMsg.h"
#include "friend_generated.h"
#include "RaknetMessagePump.h"
#include "timer.hpp"

using namespace MobaGo::Protocol;
using namespace MobaGo::FlatBuffer;

class ManageVrRobot : public MessagePumpHandle, public RobotMessageHandle
{
public:
	ManageVrRobot();
	~ManageVrRobot();
	static ManageVrRobot * instance();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);

public:
	int init();

	void update();

	void handleRobot(Robot * robot);

	virtual void handleOutputPacket(ClientPacket * packet);
public:
	//int handleMsgVRLoginAck(ServerPacket * input_packet, const MsgVRLoginAck * input_msg);

	int handleMsgMatchAck(ServerPacket * input_packet, const MsgMatchAck * input_msg);

	int handleMsgMatchEnterBattle(ServerPacket * input_packet, const MsgMatchEnterBattle * input_msg);

	int handleMsgReconnectBattle(ServerPacket * input_packet, const MsgReconnectBattle * input_msg);

	int handleMsgError(ServerPacket * input_packet, const MsgError * input_msg);

	int handleMsgVRLoginAck(ServerPacket * input_packet, const MsgVRLoginAck * input_msg);

	int handleMsgReqSummonerInfoAck(ServerPacket * input_packet, const MsgReqSummonerInfoAck * input_msg);

	int handleMsgCreateSummonerAck(ServerPacket * input_packet, const EmptyMsg * input_msg);

	int handleMsgCancelMatchAck(ServerPacket * input_packet, const MsgCancelMatchAck * input_msg);

	// friend相关
	int handleMsgSearchFriendAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgSearchFriendAck * msg);

	int handleMsgAddFriendAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgAddFriendAck * msg);
	int handleMsgDelFriendAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgDelFriendAck * msg);
	int handleMsgConfirmAddFriendAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgConfirmAddFriendAck * msg);
	int handleMsgAddCancelFriendAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgAddCancelFriendAck * msg);

	int handleMsgFriendListAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgFriendListAck * msg);
	int handleMsgApplyForFriendListAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgApplyForFriendListAck * msg);
	int handleMsgFriendDetailInfoAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgFriendDetailInfoAck * msg);


	// 消息格式转换
	int clientInput(int msg_id, ServerPacket* packet, char * buffer, int buffer_size);

protected:
	void registerMsgHandle();

	Robot * getAvilableRobot();

	void enableRobot(Robot * robot);

	void updateRobot();

	Robot * getRobot(SessionId session_id);

	void removeRobot(Robot * robot);
private:
	MessageServerPump<ServerPacket> * m_message_pump;

	RobotList_t m_avilable_robot_list;

	RobotList_t m_request_connection_robot_list;

	RobotList_t m_enable_robot_list;

	typedef unordered_map<SessionId, Robot *> RobotSessionMap_t;

	RobotSessionMap_t m_robot_session_map;
	Timer m_time_point;
};

#endif
