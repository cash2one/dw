
#ifndef MESSAGE_MAKER_HPP__
#define MESSAGE_MAKER_HPP__

#include <string>
#include "ManagerClientSession.h"
#include "SessionHandle.h"
#include "Packet_.h"
#include "inter_generated.h"
#include "protocol_generated.h"
#include "opcode.h"
#include "msg_error_generated.h"
#include "login_generated.h"
#include "Summoner.h"
#include "match_generated.h"
#include "friend_generated.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;

class MessageMaker
{
public:
	static InterPacket * makeSMsgVrRegisterVrpublic(SessionId session_id, int area_id, int server_id, const string & gate_ip_addr, int gate_port);

	static InterPacket * makeSMsgGateRegister(SessionId session_id, const string & gate_ip, int gate_port);


	static void sendErrorPacket(session_t session_id, uint64_t guid, MobaGo::FlatBuffer::Error error_code);
	static void sendVRLoginAck(session_t session_id, uint64_t guid, int nReconnectBattle);

	static void sendReqSummonerInfoAck(session_t session_id, uint64_t guid, Summoner * pSummoner);

	static void sendCreateSummonerAck(SessionId session_id, uint64_t guid, uint64_t sum_id);

	static void sendMsgFriendListAck(SessionId session_id, uint64_t guid, MAP_FRIEND map_friend);

	static InterPacket * makeSmsgVRNotifyCreateSummonerToVRF(SessionId session_id, uint64_t sum_id, string sum_name, int server_id);

	static InterPacket * makeSMsgNotifySearchFriend(SessionId session_id, uint64_t guid, uint64_t sum_id, string friend_name);
	static InterPacket * makeSMsgNotifySearchFriendAck(SessionId session_id, uint64_t guid, uint64_t sum_id, Summoner *pSummoner);

	static InterPacket * makeSMsgVRAddFriendReq(SessionId session_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);
	static InterPacket * makeSMsgVRDelFriendReq(SessionId session_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);
	static InterPacket * makeSMsgVRConfirmAddFriendReq(SessionId session_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);
	static InterPacket * makeSMsgVRAddCancelFriendReq(SessionId session_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

	static InterPacket * makeSMsgVrRegisterTr(SessionId session_id, int area_id, int gs_id);

	static InterPacket * makeSMsgVrRegisterVRFriend(SessionId session_id, int area_id, int gs_id);

	// match
	static InterPacket * makeSMsgMatchReq(MyGuid_t user_guid, int team_type, int match_type, vector<MyGuid_t> & group_member);

	static InterPacket * makeMsgCancelMatchReq(MyGuid_t user_guid);

	//friend
	static InterPacket * makeSMsgFriendListReq(SessionId session_id, MyGuid_t guid, uint64_t summoner_id);
	static InterPacket * makeSMsgApplyForFriendListReq(SessionId session_id, MyGuid_t guid, uint64_t summoner_id);
	static InterPacket * makeSMsgFriendDetailInfoReq(SessionId session_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);
	static InterPacket * makeSMsgFriendDetailInfoAck(SessionId session_id, uint64_t summoner_id, Summoner * pFriend);
	static InterPacket * makeSMsgFriendStatusReq(SessionId session_id, uint64_t guid, uint64_t summoner_id);
};

#endif
