

#ifndef MANAGE_FRIEND_SESSION_HPP__
#define MANAGE_FRIEND_SESSION_HPP__

#include <string>
#include "MessagePump.h"
#include "Packet_.h"
#include "Singleton.h"
#include "ManageSingleton.h"
#include "inter_generated.h"
#include "msg_error_generated.h"
#include "friend_generated.h"

using namespace std;

struct FriendSvrInfo
{
	FriendSvrInfo() : session_id(NULL)
	{}
	SessionId session_id;
};

class ManageFriendSession : public MessagePumpHandle
{
public:
	ManageFriendSession();
	~ManageFriendSession();
	static ManageFriendSession * instance();
public:
	int init(const string & ip_addr, int port);

	int update();

	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);

public:
	void handleOutputStream(InterPacket * inter_packet);

public:
	void sendVRNotifyCreateSummonerToVRF(uint64_t sum_id, string sum_name, int server_id);	
	void sendSMsgNotifySearchFriend(uint64_t guid, uint64_t sum_id, string friend_name);
	void sendSMsgVRAddFriendReq(uint64_t guid, uint64_t summoner_id, uint64_t friend_id);
	void sendSMsgVRDelFriendReq(uint64_t guid, uint64_t summoner_id, uint64_t friend_id);
	void sendSMsgVRConfirmAddFriendReq(uint64_t guid, uint64_t summoner_id, uint64_t friend_id);
	void sendSMsgVRAddCancelFriendReq(uint64_t guid, uint64_t summoner_id, uint64_t friend_id);
	int sendSMsgFriendListReq(uint64_t guid, uint64_t summoner_id);
	int sendMsgApplyForFriendListReq(uint64_t guid, uint64_t summoner_id);
	void sendMsgFriendDetailInfoReq(uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

protected:
	void registerVrToVRFriend();

private:
	MessageServerPump<InterPacket> * m_message_pump;
	FriendSvrInfo m_server_info;
	static ManageFriendSession * m_instance;
};
#endif