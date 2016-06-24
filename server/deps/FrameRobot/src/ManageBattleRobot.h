
#ifndef MANAGE_BATTLE_ROBOT_HPP__
#define MANAGE_BATTLE_ROBOT_HPP__

#include "Packet_.h"
#include "MessagePump.h"
#include "Robot.h"
#include "Singleton.h"
#include "protocol_generated.h"
#include "battle_generated.h"
#include "inter_generated.h"
#include "lobby_generated.h"
#include "match_generated.h"
#include "RaknetMessagePump.h"

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;
using namespace std;
using namespace flatbuffers;

class ManageBattleRobot : public MessagePumpHandle, public RobotMessageHandle
{
public:
	ManageBattleRobot();
	~ManageBattleRobot();
	static ManageBattleRobot * instance();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);
public:
	int init();

	void update();

	void handleRobot(Robot * robot);

	virtual void handleOutputPacket(ClientPacket * packet);

	int handleMsgEnterBattleAck(ServerPacket * input_packet, const EmptyMsg * input_msg);

	int handleMsgReEnterBattleAck(ServerPacket * input_packet, const MsgReEnterBattleAck * input_msg);

	int handleMsgSelectHeroAck(ServerPacket * input_packet, const MsgSelectHeroAck * input_msg);

	int handleMsgConfirmHeroAck(ServerPacket * input_packet, const MsgConfirmHeroAck * input_msg);


	int handleMsgLobbyGameStartAck(ServerPacket * input_packet, const MsgLobbyGameStartAck * input_msg);

	int handleMsgCombatPlayerReadyAck(ServerPacket * input_packet, const MsgCombatPlayerReadyAck * input_msg);

	int handleMsgFrameSyncDetail(ServerPacket * input_packet, const MsgFrameSyncDetail * input_msg);

	// 消息格式转换
	int clientInput(int msg_id, ServerPacket* packet, char * buffer, int buffer_size);

protected:
	Robot * getAvilableRobot();

	void updateRobot();

	void addRobot(Robot * robot);
	Robot * getRobot(SessionId session_id);
private:
	MessageServerPump<ServerPacket> * m_message_pump;

	RobotList_t m_avilable_robot_list;

	RobotList_t m_request_connection_robot_list;

	RobotList_t m_enable_robot_list;

	typedef unordered_map<SessionId, Robot *> RobotSessionMap_t;

	RobotSessionMap_t m_robot_session_map;

};

#endif
