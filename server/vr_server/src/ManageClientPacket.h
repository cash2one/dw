
#ifndef MANAGECLIENTPACKET_HPP__
#define MANAGECLIENTPACKET_HPP__

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "PackMsg.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "inter_generated.h"
#include "login_generated.h"
#include "ManageAccount.h"
#include "msg_error_generated.h"
#include "match_generated.h"
#include "Summoner.h"
#include "EmptyMsg.h"
#include "entity/vr/account.pb.h"
#include "entity/vr/cache_auto_assistant.h"
#include "MakeGuid.h"
#include "friend_generated.h"

class ManageClientPacket
{
public:
	ManageClientPacket();
	~ManageClientPacket();
public:
	static ManageClientPacket * instance();
public:
	int init();
public:
	int handleMsgLobbyKeepAliveReq(InterPacket * packet, const MobaGo::FlatBuffer::MsgLobbyKeepAliveReq * msg);

	int handleSMsgGateRegister(InterPacket * packet, const MobaGo::FlatBuffer::SMsgGateRegister * msg);

	int handleClientCloseConnection(InterPacket * packet, const EmptyMsg * msg);

	int handleSMsgVrpublicNotifyVrLoginInfo(InterPacket * packet, const MobaGo::FlatBuffer::SMsgVrpublicNotifyVrLoginInfo * msg);

	int handleMsgVRLoginReq(InterPacket * packet, const MobaGo::FlatBuffer::MsgVRLoginReq * msg);
	int handleMsgCreateSummonerReq(InterPacket * packet, const MobaGo::FlatBuffer::MsgCreateSummonerReq * msg);
	int handleMsgMatchReq(InterPacket * packet, const MobaGo::FlatBuffer::MsgMatchReq * msg);
	int handleMsgCancelMatchReq(InterPacket * packet, const EmptyMsg * msg);
	
	// ∫√”—œ‡πÿ
	void notifySummonerLoginToVRF(uint64_t summoner_id);
	int handleMsgSearchFriendReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgSearchFriendReq * msg);

	int handleMsgAddFriendReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgAddFriendReq * msg);
	int handleMsgDelFriendReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgDelFriendReq * msg);
	int handleMsgConfirmAddFriendReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgConfirmAddFriendReq * msg);
	int handleMsgAddCancelFriendReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgAddCancelFriendReq * msg);

	int handleMsgFriendListReq(InterPacket * packet, const EmptyMsg * msg);
	int handleMsgApplyForFriendListReq(InterPacket * packet, const EmptyMsg * msg);
	int handleMsgFriendDetailInfoReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgFriendDetailInfoReq * msg);
	
protected:
	Summoner * checkSummonerByLogin(Account * account);

	void updatePlayerInfoToTr(Account * account);

	void notifyClientCloseConnection(Account * account);
private:
	
};
#endif