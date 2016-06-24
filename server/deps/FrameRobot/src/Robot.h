
#ifndef ROBOT_HPP__
#define ROBOT_HPP__

#include <unordered_map>
#include <vector>
#include <list>
#include "common_generated.h"
#include "framecmd_c2c_generated.h"
#include "framecmd_c2s_generated.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "framesync_generated.h"
#include "Handle.h"
#include "PackMsg.h"
#include "Packet_.h"
#include "SessionHandle.h"
#include "login_generated.h"
#include "match_generated.h"
#include "battle_generated.h"
#include "msg_error_generated.h"
#include "EmptyMsg.h"
#include "friend_generated.h"
#include "Single.hpp"
#include "pluginDepot.h"

using namespace std;

using namespace MobaGo::FlatBuffer;

struct PlayerInfo;
class Robot;

typedef vector<PlayerInfo *> PlayerInfoVec_t;
typedef list<PlayerInfo *> PlayerInfoList_t;
typedef unordered_map<uint64_t, PlayerInfo *> PlayerInfoMap_t;
typedef list<Robot *> RobotList_t;

enum RobotStatus
{
	RS_Base						= 0,
	RS_Connected_vrpublic		= 1,
	RS_Sent_vrpublic_login_msg,
	RS_Enter_vrpublic,

	RS_Connected_vr,
	RS_sent_vr_login_msg,
	RS_sent_request_summer,
	RS_sent_create_summer,
	RS_Enter_vr,

	RS_Connected_battle,
	RS_sent_battle_login_msg,
	RS_Enter_battle,

	RS_Logout,
};

struct PlayerInfo 
{
	PlayerInfo()
		: user_id(0)
		, account_id(0)
		, hero_skin_id(0)
		, is_logined(false)
		, is_battle_started(false)
	{}

	uint64_t user_id;
	uint64_t account_id;
	int hero_skin_id;
	string account;
	bool is_logined;
	bool is_battle_started;
};

class RobotMessageHandle
{
public:
	virtual void handleOutputPacket(ClientPacket * packet) = 0;
};

class Robot
{
public:
	Robot();
	~Robot();
public:
	int init(SessionId session_id, RobotMessageHandle * robot_msg_handle);

	void setRobotStatus(RobotStatus robot_status);

	bool addPlayerMsgHandle(int msg_id, MsgHandleBase * msg_handle);

	int handlePacket(ServerPacket * packet);

	void update();

	PlayerInfo * getPlayerInfo();

	void setSession(SessionId session_id, RobotMessageHandle * robot_msg_handle);

	SessionId getSession();

	void setVrServerInfo(const string & ip_addr, int port, const string & session_key);

	const string & getVrIp();
	int getVrPort();
	const string & getVrSessionKey();

	void setBattleServerInfo(const string & ip_addr, int port, const string & session_key, bool is_reconnect_battle = false);
	const string & getBattleIp();
	int getBattlePort();
	bool isReconnectBattle();
	const string & getBattleSessionKey();

public:
	void connectedVrpublic();

	void connectedVr();

	void connectedBattle();

	void setAccountId(MyGuid_t account_id);

	static void setChooseServerId(int server_id);

public:
	// vrpublic
	int handleMsgServerListAck(session_t session_id, const MsgServerListAck * input_msg);

	int handleMsgChooseServerAck(session_t session_id, const MsgChooseServerAck * msg);

	// vr 
	int handleMsgVRLoginAck(session_t session_id, const MsgVRLoginAck * input_msg);
	int handleMsgMatchEnterBattle(session_t session_id, const MsgMatchEnterBattle * input_msg);

	int handleMsgReconnectBattle(session_t session_id, const MsgReconnectBattle * input_msg);

	int handleMsgCancelMatchAck(session_t session_id, const MsgCancelMatchAck * input_msg);

	int handleMsgMatchAck(session_t session_id, const MsgMatchAck * input_msg);

	// vr friend
	int handleMsgSearchFriendAck(session_t session_id, const MobaGo::FlatBuffer::MsgSearchFriendReq * msg);

	int handleMsgAddFriendAck(session_t session_id, const MobaGo::FlatBuffer::MsgAddFriendAck * msg);
	int handleMsgDelFriendAck(session_t session_id, const MobaGo::FlatBuffer::MsgDelFriendAck * msg);
	int handleMsgConfirmAddFriendAck(session_t session_id, const MobaGo::FlatBuffer::MsgConfirmAddFriendAck * msg);
	int handleMsgAddCancelFriendAck(session_t session_id, const MobaGo::FlatBuffer::MsgAddCancelFriendAck * msg);

	int handleMsgFriendListAck(session_t session_id, const MsgFriendListAck * msg);
	int handleMsgApplyForFriendListAck(session_t session_id, const MsgApplyForFriendListAck * msg);
	int handleMsgFriendDetailInfoAck(session_t session_id, const MobaGo::FlatBuffer::MsgFriendDetailInfoAck * msg);

	// battle
	int handleMsgLobbyJoinGameAck(session_t session_id, const MsgLobbyJoinGameAck * msg);

	int handleMsgLobbyGameStartAck(session_t session_id, const MsgLobbyGameStartAck * msg);

	int handleMsgCombatPlayerReadyAck(session_t session_id, const MsgCombatPlayerReadyAck * msg);

	int handleMsgCombatBattleStartAck(session_t session_id, const MsgCombatBattleStartAck * msg);

	int handleMsgFrameSyncDetail(session_t session_id, const MsgFrameSyncDetail * msg);

	int handleMsgReconnectionAck(session_t session_id, const MsgReconnectionAck * msg);

	int handleMsgError(session_t session_id, const MsgError * msg);

	int handleMsgReqSummonerInfoAck(session_t session_id, const MsgReqSummonerInfoAck * msg);

	int handleMsgCreateSummonerAck(session_t session_id, const EmptyMsg * msg);

	int handleMsgSearchFriendAck(session_t sesssion_id, const MsgSearchFriendAck * msg);

	void sendMsg(ClientPacket * pack);

protected:
	void initMsgHandler();

	MsgHandleBase * getMsgHandle(int msg_id);

	void loginVrpublic();

	void loginVr();
	
	void loginBattle();

	void logout();
private:
	SessionId m_session;

	typedef unordered_map<int, MsgHandleBase *> MessageHandleMap_t;

	MessageHandleMap_t m_msg_handle_map;

	//;ManagerRobot * m_manager_robot;
	friend class ManageLoginRobot;
	PlayerInfo * m_player_info;

	uint32_t m_packet_count;

	RobotMessageHandle * m_robot_msg_handle;

	string m_vr_ip_addr;
	int m_vr_port;
	string m_vr_session_key;

	string m_battle_ip_addr;
	int m_battle_port;
	string m_battle_session_key;
	bool m_is_reconnect_battle;

	RobotStatus m_robot_status;

	static int m_choose_server_id;

};

struct RunningRobot
{
	std::map<SessionId, Robot*> running;

	int packet_input(int msg_id, ServerPacket* packet, char * buffer, int buffer_size)
	{	
		auto it = running.find(packet->session_id);
		if (it == running.end())
			return -1;

		Robot * r = it->second;
		if (NULL == r)
			return -1;

		if (0 == r->handlePacket(packet))
			return 0;

		std::unique_ptr<InterPacket> dest(new InterPacket);
		dest->session_id = packet->session_id;
		dest->setMsgInfo(msg_id, 0, buffer_size, buffer);

		return Single<PluginDepot>::GetInstance()->input(msg_id, dest.get(), buffer, buffer_size);
	}
};

#endif
