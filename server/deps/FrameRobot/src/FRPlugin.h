#ifndef _FR_PLUGIN_H_
#define _FR_PLUGIN_H_

#include <unordered_map>
#include <iostream>
#include "typedef.h"
#include <flatbuffers/flatbuffers.h>
#include "plugin.h"
#include "friend_generated.h"
#include "protocol_generated.h"
#include "Robot.h"

using namespace MobaGo::Protocol;

class FRPlugin : public Plugin
{
public:
	FRPlugin();
	~FRPlugin();

public:
	BEGIN_INPUT_MSG_MAP()
		// client handle msg from vr
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgSearchFriendAck, MobaGo::FlatBuffer::MsgSearchFriendAck)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgAddFriendAck, MobaGo::FlatBuffer::MsgAddFriendAck)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgDelFriendAck, MobaGo::FlatBuffer::MsgDelFriendAck)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgConfirmAddFriendAck, MobaGo::FlatBuffer::MsgConfirmAddFriendAck)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgAddCancelFriendAck, MobaGo::FlatBuffer::MsgAddCancelFriendAck)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgFriendListAck, MobaGo::FlatBuffer::MsgFriendListAck)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgApplyForFriendListAck, MobaGo::FlatBuffer::MsgApplyForFriendListAck)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgFriendDetailInfoAck, MobaGo::FlatBuffer::MsgFriendDetailInfoAck)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgError, MobaGo::FlatBuffer::MsgError)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgVRFriendOperateReq, MobaGo::FlatBuffer::MsgVRFriendOperateReq)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgFriendsStatusAck, MobaGo::FlatBuffer::MsgFriendStatusAck)
		INPUT_HANDLE_MSG(FRPlugin, Opcode_E_MsgFriendStatusChange, MobaGo::FlatBuffer::FriendStatus)
	END_INPUT_MSG_MAP()

public:
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgSearchFriendAck, MobaGo::FlatBuffer::MsgSearchFriendAck);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgAddFriendAck, MobaGo::FlatBuffer::MsgAddFriendAck);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgDelFriendAck, MobaGo::FlatBuffer::MsgDelFriendAck);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgConfirmAddFriendAck, MobaGo::FlatBuffer::MsgConfirmAddFriendAck);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgAddCancelFriendAck, MobaGo::FlatBuffer::MsgAddCancelFriendAck);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgFriendListAck, MobaGo::FlatBuffer::MsgFriendListAck);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgApplyForFriendListAck, MobaGo::FlatBuffer::MsgApplyForFriendListAck);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgFriendDetailInfoAck, MobaGo::FlatBuffer::MsgFriendDetailInfoAck);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgError, MobaGo::FlatBuffer::MsgError);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgVRFriendOperateReq, MobaGo::FlatBuffer::MsgVRFriendOperateReq);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgFriendsStatusAck, MobaGo::FlatBuffer::MsgFriendStatusAck);
	INPUT_FUN_DECLARE(FRPlugin, Opcode_E_MsgFriendStatusChange, MobaGo::FlatBuffer::FriendStatus);

public:
	string randomCreateRobotName(int start_index, int end_index);

	bool isConfirmAdd(int prob);
	void notifyCreateAccount(uint64_t account_id, string account_name);
	bool getAvailableAccount(string account_name);
	void notifyCreateVRRobot(SessionId session_id, Robot * robot);
	void notifyRemoveVRRobot(SessionId session_id);
	Robot * getRobot(SessionId session_id);

private:
	typedef unordered_map<SessionId, Robot *> RobotSessionMap_t;
	RobotSessionMap_t m_robot_session_map;
	map<string, uint64_t> m_available_account;
};

#endif