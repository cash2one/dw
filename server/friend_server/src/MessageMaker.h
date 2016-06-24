
#ifndef MESSAGE_MAKER_HPP__
#define MESSAGE_MAKER_HPP__

#include <string>
#include "Packet_.h"
#include "inter_generated.h"
#include "protocol_generated.h"
#include "opcode.h"
#include "msg_error_generated.h"
#include "friend_generated.h"
#include "ManageSummoner.h"
#include "google/protobuf/repeated_field.h"

class ManageVrSession;

class MessageMaker
{
public:

	static void sendVRErrorPacket(session_t session_id, uint64_t guid, MobaGo::FlatBuffer::InterError error_code, ManageVrSession * pVrSession);

	static InterPacket * makeSMsgVRFriendNotifySearchFriendAck(SessionId session_id, uint64_t guid, uint64_t friend_id, string friend_name, int gs_id);
	static InterPacket * makeSMsgNotifySearchFriendAck(SessionId session_id, uint64_t guid, uint64_t sum_id, uint64_t friend_id, int friend_head_id);

	//req发到另外一个VR, ack发到原来的VR
	static InterPacket * makeSMsgNotifyFriendOperateReq(SessionId session_id, uint64_t sum_id, uint64_t friend_id, MobaGo::FlatBuffer::friend_oper_type oper_type);
	static InterPacket * makeSMsgNotifyFriendOperateAck(SessionId session_id, uint64_t guid, uint64_t sum_id, uint64_t friend_id, MobaGo::FlatBuffer::friend_oper_type oper_type);

	static InterPacket * makeSMsgFriendDetailInfoReq(SessionId session_id, uint64_t summoner_id, uint64_t friend_id);
	static InterPacket * makeSMsgFriendDetailInfoAck(SessionId session_id, uint64_t summoner_id, uint64_t friend_id, int friend_head_id);

	static InterPacket * makeSMsgNotifySummonerStatusToFriends(SessionId session_id, uint64_t summmoner_id, uint64_t friend_id, MobaGo::FlatBuffer::SummonerStatus sum_status);

	// 将好友的状态推送给自己
	static InterPacket * makeSMsgNotifyFriendsStatusToSummoner(SessionId session_id, uint64_t guid, uint64_t summoner_id, google::protobuf::RepeatedField<::google::protobuf::uint64 > friend_list);
};

#endif
