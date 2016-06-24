
#ifndef MANAGE_VR_SESSION_HPP__
#define MANAGE_VR_SESSION_HPP__

#include <unordered_map>
#include "Packet_.h"
#include "MessagePump.h"
#include "Singleton.h"
#include "FriendConfig.h"
#include "inter_generated.h"
#include "MessageMaker.h"
#include "EmptyMsg.h"
#include "friend_generated.h"

typedef struct _VrSession
{
	_VrSession() : 
		area_id(0),
		gs_id(0)
	{
	}

	int area_id;
	int gs_id;
	SessionId session_id;
}VrSession;

typedef unordered_map<int, VrSession *>	VrServerInfoMap_t;
typedef unordered_map<SessionId, VrSession *> VrSessionMap_t;

class ManageVrSession : public MessagePumpHandle
{
public:
	ManageVrSession();
	~ManageVrSession();
	static ManageVrSession * instance();
public:
	int init(const FriendCfg & friend_cfg);

	int update();

	virtual  void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);

	// register handler
	int handleSMsgVrRegisterVRFriend(InterPacket *inter_packet, const MobaGo::FlatBuffer::SMsgVrRegisterVRFriend * msg);

	// 
	int handleSMsgVRNotifyCreateSummonerToVRF(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRNotifyCreateSummonerToVRF * msg);
	int handleVRNotifySummonerLoginToVRF(InterPacket * inter_packet, const EmptyMsg *msg);
	int handleVRNotifySummonerLogoffToVRF(InterPacket * inter_packet, const EmptyMsg *msg);

	int handleSMsgNotifySearchFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRNotifySearchFriend * msg);

	int handleSMsgVRAddFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRAddFriendReq * msg);
	int handleSMsgVRDelFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRDelFriendReq * msg);
	int handleSMsgVRConfirmAddFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRConfirmAddFriendReq * msg);
	int handleSMsgVRAddCancelFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRAddCancelFriendReq * msg);

	int handleSMsgFriendListReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgFriendListReq * msg);
	int handleSMsgApplyForFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgApplyForFriendListReq * msg);

	int handleSMsgFriendDetailInfoReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgFriendDetailInfoReq * msg);
	int handleSMsgFriendDetailInfoAck(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgFriendDetailInfoAck * msg);

	void notifySummonerStatusToFriends(uint64_t summoner_id, MobaGo::FlatBuffer::SummonerStatus sum_status);
	//
	int handleOutputStream(SessionId session_id, char * buffer, size_t buf_len);

protected:
	void registerMsgHandle();
	void addVR(SessionId session_id, int area_id, int gs_id);
	void delVR(SessionId session_id);
	VrSession * getVR(SessionId session_id);
	VrSession * getVR(int gs_id);

private:
	MessageServerPump<InterPacket> * m_message_pump;
	VrServerInfoMap_t m_vr_server_map;		// 根据gs_id查找vr
	VrSessionMap_t m_vr_session_map;		// 根据session_id查找vr
};

#endif
