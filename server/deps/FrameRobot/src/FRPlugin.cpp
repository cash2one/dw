#include "FRPlugin.h"
#include "flatbuffers/flatbuffers.h"
#include <random>
#include "ClientProtocol.h"
#include "ManageRobotConfig.h"
#include "FunCallCount.hpp"

using namespace flatbuffers;

#define GET_ROBOT_AND_RETURN() \
	auto it = Single<RunningRobot>::GetInstance()->running.find(input_packet->session_id); \
	if (it == Single<RunningRobot>::GetInstance()->running.end()) \
		{ \
		return -1; \
		} \
	Robot * robot = it->second; 

FRPlugin::FRPlugin()
{
	m_plugin_name = "FRPlugin";
}

FRPlugin::~FRPlugin()
{
}

// client handle msg from vr
INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgSearchFriendAck, MobaGo::FlatBuffer::MsgSearchFriendAck)
{
	GET_ROBOT_AND_RETURN();

	DEF_LOG_INFO("handleMsgSearchFriendAck: friend_id:%llu, friend_head_id:%d, friend_status:%d, ", input_msg->friend_info()->friend_id(), input_msg->friend_info()->head_id(), input_msg->friend_info()->status());
	uint64_t friend_id = input_msg->friend_info()->friend_id();
	
	FlatBufferBuilder builder;
	Offset<MsgAddFriendReq> off_req = CreateMsgAddFriendReq(builder, friend_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgAddFriendReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = input_packet->session_id;
	m_plugin_cfg.sender->sendMsg2VR(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgAddFriendAck, MobaGo::FlatBuffer::MsgAddFriendAck)
{
	GET_ROBOT_AND_RETURN();

	DEF_LOG_INFO("add friend ack ok, %llu", input_packet->msg_head.guid);

	/*FlatBufferBuilder builder;
	Offset<MsgDelFriendReq> off_req = CreateMsgDelFriendReq(builder, 216454257098274205);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgDelFriendReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = input_packet->session_id;
	m_plugin_cfg.sender->sendMsg2VR(pack);*/

	return 0;
}

INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgDelFriendAck, MobaGo::FlatBuffer::MsgDelFriendAck)
{
	GET_ROBOT_AND_RETURN();

	FlatBufferBuilder builder;
	Offset<MsgConfirmAddFriendReq> off_req = CreateMsgConfirmAddFriendReq(builder, 216454257098274205);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgConfirmAddFriendReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = input_packet->session_id;
	m_plugin_cfg.sender->sendMsg2VR(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgConfirmAddFriendAck, MobaGo::FlatBuffer::MsgConfirmAddFriendAck)
{
	GET_ROBOT_AND_RETURN();

	DEF_LOG_INFO("confirm add friend ack, summoner_id:%llu, friend_id:%llu", robot->getPlayerInfo()->account_id, input_msg->friend_info()->friend_id());
	
	/*uint64_t friend_id = input_msg->friend_info()->friend_id();
	FlatBufferBuilder builder;
	Offset<MsgAddCancelFriendReq> off_req = CreateMsgAddCancelFriendReq(builder, friend_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgAddCancelFriendReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = input_packet->session_id;
	m_plugin_cfg.sender->sendMsg2VR(pack);*/

	return 0;
}

INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgAddCancelFriendAck, MobaGo::FlatBuffer::MsgAddCancelFriendAck)
{
	GET_ROBOT_AND_RETURN();

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgFriendListReq, 0, (char *)NULL);
	pack->session_id = input_packet->session_id;
	m_plugin_cfg.sender->sendMsg2VR(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgFriendListAck, MobaGo::FlatBuffer::MsgFriendListAck)
{
	GET_ROBOT_AND_RETURN();

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgApplyForFriendListReq, 0, (char *)NULL);
	pack->session_id = input_packet->session_id;
	m_plugin_cfg.sender->sendMsg2VR(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgApplyForFriendListAck, MobaGo::FlatBuffer::MsgApplyForFriendListAck)
{
	GET_ROBOT_AND_RETURN();

	FlatBufferBuilder builder;
	Offset<MsgFriendDetailInfoReq> off_req = CreateMsgFriendDetailInfoReq(builder, 216454257098274205);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgFriendDetailInfoReq, builder.GetSize(), (char *)builder.GetBufferPointer());

	pack->session_id = input_packet->session_id;
	m_plugin_cfg.sender->sendMsg2VR(pack);
	return 0;
}

INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgFriendDetailInfoAck, MobaGo::FlatBuffer::MsgFriendDetailInfoAck)
{
	GET_ROBOT_AND_RETURN();

	DEF_LOG_INFO("handleMsgFriendListAck: friend_id:%llu", input_msg->friend_info()->friend_id());
	
	return 0;
}

INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgFriendsStatusAck, MobaGo::FlatBuffer::MsgFriendStatusAck)
{
	GET_ROBOT_AND_RETURN();

	const flatbuffers::Vector<flatbuffers::Offset<FriendStatus>> * temp_vec_stutus = input_msg->friend_status();
	if (temp_vec_stutus != NULL )
	{
		for (unsigned int i = 0; i < temp_vec_stutus->size(); ++i)
		{
			DEF_LOG_INFO("friends status ack: summoner_id:%llu, friend_id:%llu, friend_status:%d", robot->getPlayerInfo()->account_id, input_msg->friend_status()->Get(i)->friend_id(),
				input_msg->friend_status()->Get(i)->status());
		}
	}

	return 0;
}

INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgFriendStatusChange, MobaGo::FlatBuffer::FriendStatus)
{
	GET_ROBOT_AND_RETURN();

	DEF_LOG_INFO("friends status change: summoner_id:%llu, friend_id:%llu, friend_status:%d", robot->getPlayerInfo()->account_id, input_msg->friend_id(), input_msg->status());

	return 0;
}

// 所有下一步的处理都放在这里处理，所谓的下一步处理
// 比如： A add B, B 收到 A加好友的请求，B confirm_add A就是下一步的处理
INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgVRFriendOperateReq, MobaGo::FlatBuffer::MsgVRFriendOperateReq)
{
	GET_ROBOT_AND_RETURN();

	uint64_t summoner_id = robot->getPlayerInfo()->account_id;
	uint64_t friend_id = input_msg->friend_id();
	switch (input_msg->operate_type())
	{
	case friend_operate_type_add_friend:
	{
		// add confirm
		DEF_LOG_INFO("operate friend req---> add: summoner_id:%llu, friend_id:%llu", summoner_id, friend_id);
		ClientPacket * pack = ClientProtocol::makeMsgConnfirmAddFriendReq(input_packet->session_id, friend_id);
		m_plugin_cfg.sender->sendMsg2VR(pack);

		Single<FunCallCount>::GetInstance()->addConfirmAddFriendCount();

		break;
	}
	case friend_operate_type_del_friend:
	{
		DEF_LOG_INFO("operate friend req---> del: summoner_id:%llu, friend_id:%llu", summoner_id, friend_id);
		break;
	}
	case friend_operate_type_add_confirm:
	{
		// del to do list
		if (isConfirmAdd(Single<RobotCfg>::GetInstance()->confirm_add_prob))
		{
			DEF_LOG_INFO("operate friend req---> add confirm: summoner_id:%llu, friend_id:%llu", summoner_id, friend_id);
			ClientPacket * pack = ClientProtocol::makeMsgDelFriendReq(input_packet->session_id, friend_id);
			m_plugin_cfg.sender->sendMsg2VR(pack);

			Single<FunCallCount>::GetInstance()->addDelFriendCount();
		}
		
		break;
	}
	case friend_operate_type_add_cancel:
	{
		DEF_LOG_INFO("operate friend req---> add cancel: summoner_id:%llu, friend_id:%llu", summoner_id, friend_id);
		break;
	}
	default:
		break;
	}

	return 0;
}

bool FRPlugin::isConfirmAdd(int prob)
{
	if (prob >= 100)
		return true;
	if (prob <= 0)
		return false;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 100);

	if (dis(gen) > prob)
		return false;
	else
		return true;
}

INPUT_FUN_DEFINE(FRPlugin, Opcode_E_MsgError, MobaGo::FlatBuffer::MsgError)
{
	GET_ROBOT_AND_RETURN();
	int msg_error = input_msg->error_code();
	DEF_LOG_INFO("handleMsgFriendListAck: friend_id:%llu", msg_error);

	return 0;
}

void FRPlugin::notifyCreateVRRobot(SessionId session_id, Robot * robot)
{
	auto it = m_robot_session_map.find(session_id);
	if (it != m_robot_session_map.end())
	{
		return;
	}

	m_robot_session_map[session_id] = robot;
}

void FRPlugin::notifyRemoveVRRobot(SessionId session_id)
{
	auto it = m_robot_session_map.find(session_id);
	if (it != m_robot_session_map.end())
	{
		delete it->second;
		m_robot_session_map.erase(it);
	}
}

Robot * FRPlugin::getRobot(SessionId session_id)
{
	auto it = m_robot_session_map.find(session_id);
	if (it != m_robot_session_map.end())
	{
		return it->second;
	}

	return NULL;
}

string FRPlugin::randomCreateRobotName(int start_index, int end_index)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(start_index, end_index);
	int robot_id = dis(gen);
	char robot_name[100];
	sprintf(robot_name, "robot_%d", robot_id);

	return robot_name;
}

void FRPlugin::notifyCreateAccount(uint64_t account_id, string account_name)
{
	auto it = m_available_account.find(account_name);
	if (it == m_available_account.end())
	{	
		m_available_account[account_name] = account_id;
	}
}

bool FRPlugin::getAvailableAccount(string account_name)
{
	auto it = m_available_account.find(account_name);
	if (it == m_available_account.end())
	{
		return true;

	}

	return false;
}
