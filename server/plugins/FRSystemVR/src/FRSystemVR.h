#ifndef _FR_SYSTEM_VR_H_
#define _FR_SYSTEM_VR_H_

#include <unordered_map>
#include <iostream>
#include "typedef.h"
#include <flatbuffers/flatbuffers.h>
#include "inter_generated.h"
#include "plugin.h"
//#include "msg_demo.pb.h"
#include "robot_generated.h"
#include "protocol_generated.h"
#include "match_generated.h"
#include "opcode.h"
#include "msg_error_generated.h"
#include "friend_generated.h"
#include "EmptyMsg.h"

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;

class FRSystemVR : public Plugin
{
public:
	FRSystemVR();
	~FRSystemVR();

public:
	BEGIN_INPUT_MSG_MAP()
		
		// vr handle msg from client
		INPUT_HANDLE_MSG(FRSystemVR, Opcode_E_MsgSearchFriendReq, MobaGo::FlatBuffer::MsgSearchFriendReq)
		INPUT_HANDLE_MSG(FRSystemVR, Opcode_E_MsgAddFriendReq, MobaGo::FlatBuffer::MsgAddFriendReq)
		INPUT_HANDLE_MSG(FRSystemVR, Opcode_E_MsgDelFriendReq, MobaGo::FlatBuffer::MsgDelFriendReq)
		INPUT_HANDLE_MSG(FRSystemVR, Opcode_E_MsgConfirmAddFriendReq, MobaGo::FlatBuffer::MsgConfirmAddFriendReq)
		INPUT_HANDLE_MSG(FRSystemVR, Opcode_E_MsgAddCancelFriendReq, MobaGo::FlatBuffer::MsgAddCancelFriendReq)
		INPUT_HANDLE_MSG(FRSystemVR, Opcode_E_MsgFriendListReq, EmptyMsg)
		INPUT_HANDLE_MSG(FRSystemVR, Opcode_E_MsgApplyForFriendListReq, EmptyMsg)
		INPUT_HANDLE_MSG(FRSystemVR, Opcode_E_MsgFriendDetailInfoReq, MobaGo::FlatBuffer::MsgFriendDetailInfoReq)

		// vr handle msg from friend
		INPUT_HANDLE_MSG(FRSystemVR, E_SMsgInterError, MobaGo::FlatBuffer::MsgError)
		INPUT_HANDLE_MSG(FRSystemVR, E_SMsgNotifySearchFriendAck, MobaGo::FlatBuffer::SMsgNotifySearchFriendAck)
		INPUT_HANDLE_MSG(FRSystemVR, E_SMsgVRFriendNotifyFriendOperateReq, MobaGo::FlatBuffer::SMsgVRFNotifyFriendOperateReq)
		INPUT_HANDLE_MSG(FRSystemVR, E_SMsgVRFriendNotifyFriendOperateAck, MobaGo::FlatBuffer::SMsgVRFNotifyFriendOperateAck)
		INPUT_HANDLE_MSG(FRSystemVR, E_SMsgFriendListAck, MobaGo::FlatBuffer::SMsgFriendListAck)
		INPUT_HANDLE_MSG(FRSystemVR, E_SMsgApplyForFriendListAck, MobaGo::FlatBuffer::SMsgApplyForFriendListAck)
		INPUT_HANDLE_MSG(FRSystemVR, E_SMsgFSFriendDetailInfoReq, MobaGo::FlatBuffer::SMsgFriendDetailInfoReq)
		INPUT_HANDLE_MSG(FRSystemVR, E_SMsgFSFriendDetailInfoAck, MobaGo::FlatBuffer::SMsgFriendDetailInfoAck)
		INPUT_HANDLE_MSG(FRSystemVR, E_SMsgFriendStatusAck, MobaGo::FlatBuffer::SMsgFriendStatusAck)
		INPUT_HANDLE_MSG(FRSystemVR, E_SMsgNotifySummonerStatusChange, MobaGo::FlatBuffer::SMsgNotifySummonerStatusToFriend)

	END_INPUT_MSG_MAP()

	BEGING_NOTIFY_MSG_MAP()
		NOTIFY_HANDLE_MSG(FRSystemVR, N_CreateSummoner, MobaGo::FlatBuffer::)
		NOTIFY_HANDLE_MSG(FRSystemVR, N_PlayerOnline, EmptyMsg)
		NOTIFY_HANDLE_MSG(FRSystemVR, N_PlayerOffline, EmptyMsg)
	END_NOTIFY_MSG_MAP()
public:

	// vr handle msg from client
	INPUT_FUN_DECLARE(FRSystemVR, Opcode_E_MsgSearchFriendReq, MobaGo::FlatBuffer::MsgSearchFriendReq);
	INPUT_FUN_DECLARE(FRSystemVR, Opcode_E_MsgAddFriendReq, MobaGo::FlatBuffer::MsgAddFriendReq);
	INPUT_FUN_DECLARE(FRSystemVR, Opcode_E_MsgDelFriendReq, MobaGo::FlatBuffer::MsgDelFriendReq);
	INPUT_FUN_DECLARE(FRSystemVR, Opcode_E_MsgConfirmAddFriendReq, MobaGo::FlatBuffer::MsgConfirmAddFriendReq);
	INPUT_FUN_DECLARE(FRSystemVR, Opcode_E_MsgAddCancelFriendReq, MobaGo::FlatBuffer::MsgAddCancelFriendReq);
	INPUT_FUN_DECLARE(FRSystemVR, Opcode_E_MsgFriendListReq, EmptyMsg);
	INPUT_FUN_DECLARE(FRSystemVR, Opcode_E_MsgApplyForFriendListReq, EmptyMsg);
	INPUT_FUN_DECLARE(FRSystemVR, Opcode_E_MsgFriendDetailInfoReq, MobaGo::FlatBuffer::MsgFriendDetailInfoReq);

	//
	INPUT_FUN_DECLARE(FRSystemVR, E_SMsgInterError, MobaGo::FlatBuffer::MsgError);
	INPUT_FUN_DECLARE(FRSystemVR, E_SMsgNotifySearchFriendAck, MobaGo::FlatBuffer::SMsgNotifySearchFriendAck);
	INPUT_FUN_DECLARE(FRSystemVR, E_SMsgVRFriendNotifyFriendOperateReq, MobaGo::FlatBuffer::SMsgVRFNotifyFriendOperateReq);
	INPUT_FUN_DECLARE(FRSystemVR, E_SMsgVRFriendNotifyFriendOperateAck, MobaGo::FlatBuffer::SMsgVRFNotifyFriendOperateAck);
	INPUT_FUN_DECLARE(FRSystemVR, E_SMsgFriendListAck, MobaGo::FlatBuffer::SMsgFriendListAck);
	INPUT_FUN_DECLARE(FRSystemVR, E_SMsgApplyForFriendListAck, MobaGo::FlatBuffer::SMsgApplyForFriendListAck);
	INPUT_FUN_DECLARE(FRSystemVR, E_SMsgFSFriendDetailInfoReq, MobaGo::FlatBuffer::SMsgFriendDetailInfoReq);
	INPUT_FUN_DECLARE(FRSystemVR, E_SMsgFSFriendDetailInfoAck, MobaGo::FlatBuffer::SMsgFriendDetailInfoAck);
	INPUT_FUN_DECLARE(FRSystemVR, E_SMsgFriendStatusAck, MobaGo::FlatBuffer::SMsgFriendStatusAck);
	INPUT_FUN_DECLARE(FRSystemVR, E_SMsgNotifySummonerStatusChange, MobaGo::FlatBuffer::SMsgNotifySummonerStatusToFriend);

	// notify
	NOTIFY_FUN_DECLARE(FRSystemVR, N_CreateSummoner, EmptyMsg);
	NOTIFY_FUN_DECLARE(FRSystemVR, N_PlayerOnline, EmptyMsg);
	NOTIFY_FUN_DECLARE(FRSystemVR, N_PlayerOffline, EmptyMsg);
protected:

private:

};

#endif