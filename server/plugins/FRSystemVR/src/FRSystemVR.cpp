#include "FRSystemVR.h"
#include "get_entity_macro.h"
#include "account.pb.h"
#include "check_macro.h"
#include "friend_generated.h"
#include "sender.h"
#include "pool.h"
#include "pluginDepot.h"

using namespace MobaGo::FlatBuffer;

FRSystemVR::FRSystemVR()
{
	m_plugin_name = "FRSystemVR";
}

FRSystemVR::~FRSystemVR()
{
}

// vr handle msg from client
INPUT_FUN_DEFINE(FRSystemVR, Opcode_E_MsgSearchFriendReq, MobaGo::FlatBuffer::MsgSearchFriendReq)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	string friend_name = input_msg->friend_name() != NULL ? input_msg->friend_name()->str() : "";
	if (friend_name.empty() || friend_name.find(" ", 0) != friend_name.npos)
	{
		InterPacket * pack = new InterPacket();

		flatbuffers::FlatBufferBuilder build;
		flatbuffers::Offset<MobaGo::FlatBuffer::MsgError> req_off = MobaGo::FlatBuffer::CreateMsgError(build, MobaGo::FlatBuffer::Error_IllegalChar);
		build.Finish(req_off);

		pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgError, summoner->gate_guid(), build.GetSize(), (char *)build.GetBufferPointer());
		m_plugin_cfg.sender->sendMsg2Client(pack);

		return 0;
	}

	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> off_friend_name = builder.CreateString(friend_name);
	flatbuffers::Offset<SMsgVRNotifySearchFriend> off_search_friend = CreateSMsgVRNotifySearchFriend(builder, summoner->guid(), off_friend_name);
	builder.Finish(off_search_friend);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgNotifySearchFriend, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Friend(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, Opcode_E_MsgAddFriendReq, MobaGo::FlatBuffer::MsgAddFriendReq)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	uint64_t summoner_id = summoner->guid();
	uint64_t friend_id = input_msg->friend_id();

	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRNotifyAddFriendReq> off_add_friend = CreateSMsgVRNotifyAddFriendReq(builder, summoner->guid(), input_msg->friend_id());
	builder.Finish(off_add_friend);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRNotifyAddFriendReq, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Friend(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, Opcode_E_MsgDelFriendReq, MobaGo::FlatBuffer::MsgDelFriendReq)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRNotifyDelFriendReq> off_del_friend = CreateSMsgVRNotifyDelFriendReq(builder, summoner->guid(), input_msg->friend_id());
	builder.Finish(off_del_friend);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRNotifyDelFriendReq, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Friend(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, Opcode_E_MsgConfirmAddFriendReq, MobaGo::FlatBuffer::MsgConfirmAddFriendReq)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	uint64_t summoner_id = summoner->guid();
	uint64_t friend_id = input_msg->friend_id();

	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRNotifyConfirmAddFriendReq> off_add_friend = CreateSMsgVRNotifyConfirmAddFriendReq(builder, summoner->guid(), input_msg->friend_id());
	builder.Finish(off_add_friend);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRNotifyConfirmAddFriendReq, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Friend(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, Opcode_E_MsgAddCancelFriendReq, MobaGo::FlatBuffer::MsgAddCancelFriendReq)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRNotifyAddCancelFriendReq> off_add_cancel_friend = CreateSMsgVRNotifyAddCancelFriendReq(builder, summoner->guid(), input_msg->friend_id());
	builder.Finish(off_add_cancel_friend);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRNotifyAddCancelFriendReq, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Friend(pack);
	
	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, Opcode_E_MsgFriendListReq, EmptyMsg)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	InterPacket * pack = new InterPacket();
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgFriendListReq> off_friend_list_req = CreateSMsgFriendListReq(builder, summoner->guid());
	builder.Finish(off_friend_list_req);

	pack->setMsgInfo(E_SMsgFriendListReq, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Friend(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, Opcode_E_MsgApplyForFriendListReq, EmptyMsg)\
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	InterPacket * pack = new InterPacket();
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgApplyForFriendListReq> off_apply_for_friend = CreateSMsgApplyForFriendListReq(builder, summoner->guid());
	builder.Finish(off_apply_for_friend);

	pack->setMsgInfo(E_SMsgApplyForFriendListReq, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Friend(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, Opcode_E_MsgFriendDetailInfoReq, MobaGo::FlatBuffer::MsgFriendDetailInfoReq)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	InterPacket * pack = new InterPacket();
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgFriendDetailInfoReq> off_friend_detail_req = CreateSMsgFriendDetailInfoReq(builder, summoner->guid(), input_msg->friend_id());
	builder.Finish(off_friend_detail_req);

	pack->setMsgInfo(E_SMsgVRFriendDetailInfoReq, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Friend(pack);

	return 0;
}


INPUT_FUN_DEFINE(FRSystemVR, E_SMsgInterError, MobaGo::FlatBuffer::MsgError)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	int inter_error_code = input_msg->error_code();

	InterPacket * pack = new InterPacket();
	
	flatbuffers::FlatBufferBuilder build;
	switch (inter_error_code)
	{
	case MobaGo::FlatBuffer::InterError_SummonerNameIsEmpty:
		{
			flatbuffers::Offset<MobaGo::FlatBuffer::MsgError> req_off = MobaGo::FlatBuffer::CreateMsgError(build, Error_SummonerNameIsEmpty);
			build.Finish(req_off);
			break;
		}
	case MobaGo::FlatBuffer::InterError_FriendSvrHasNoSummoner:
		{
			flatbuffers::Offset<MobaGo::FlatBuffer::MsgError> req_off = MobaGo::FlatBuffer::CreateMsgError(build, Error_FriendSvrHasNoSummoner);
			build.Finish(req_off);
			break;
		}
	case MobaGo::FlatBuffer::InterError_AddMyselfAsFriend:
		{
			flatbuffers::Offset<MobaGo::FlatBuffer::MsgError> req_off = MobaGo::FlatBuffer::CreateMsgError(build, Error_AddMyselfAsFriend);
			build.Finish(req_off);
			break;
		}
	}
	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgError, summoner->gate_guid(), build.GetSize(), (char *)build.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Client(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, E_SMsgNotifySearchFriendAck, MobaGo::FlatBuffer::SMsgNotifySearchFriendAck)
{
	uint64_t friend_id = input_msg->friend_id();
	int friend_head_id = input_msg->friend_head_id();

	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	InterPacket * pack = new InterPacket();

	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MobaGo::FlatBuffer::FriendInfo> off_friend_info = MobaGo::FlatBuffer::CreateFriendInfo(builder, friend_id, friend_head_id, 0);
	flatbuffers::Offset<MobaGo::FlatBuffer::MsgSearchFriendAck> off_search_friend_ack = MobaGo::FlatBuffer::CreateMsgSearchFriendAck(builder, off_friend_info);
	builder.Finish(off_search_friend_ack);
	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgSearchFriendAck, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());

	m_plugin_cfg.sender->sendMsg2Client(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, E_SMsgVRFriendNotifyFriendOperateReq, MobaGo::FlatBuffer::SMsgVRFNotifyFriendOperateReq)
{
	uint64_t friend_id = input_msg->friend_id();
	PackInfo req_pack(R_GetGateGuidBySummonerId, friend_id, NULL); // friend_id: friend_guid --> friend_gate_guid
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(input_msg->friend_id())
	CK_NULL_RETURN(summoner, -1)							// 实际上summoner是friend信息，宏定义命名的关系， 容易混淆
	
	InterPacket * pack = new InterPacket();

	uint64_t friend_guid = input_msg->friend_id();
	uint64_t summoner_id = input_msg->summoner_id();
	int oper_type = input_msg->operate_type();			// to be deleted

	flatbuffers	::FlatBufferBuilder builder;
	flatbuffers::Offset<MobaGo::FlatBuffer::MsgVRFriendOperateReq> off_vr_friend_oper_ack = MobaGo::FlatBuffer::CreateMsgVRFriendOperateReq(builder, input_msg->summoner_id(), (friend_operate_type)input_msg->operate_type());
	builder.Finish(off_vr_friend_oper_ack);

	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgVRFriendOperateReq, req_pack.guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Client(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, E_SMsgVRFriendNotifyFriendOperateAck, MobaGo::FlatBuffer::SMsgVRFNotifyFriendOperateAck)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	InterPacket * pack = new InterPacket();
	flatbuffers::FlatBufferBuilder builder;

	if (input_msg->operate_type() == friend_oper_type::friend_oper_type_add_friend)
	{
		flatbuffers::Offset<MobaGo::FlatBuffer::MsgAddFriendAck> off_add_friend_ack = MobaGo::FlatBuffer::CreateMsgAddFriendAck(builder, MobaGo::FlatBuffer::FriendRelationOperResult_FROR_SUCCESS);
		builder.Finish(off_add_friend_ack);

		pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgAddFriendAck, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	}

	if (input_msg->operate_type() == friend_oper_type::friend_oper_type_del_friend)
	{
		flatbuffers::Offset<MobaGo::FlatBuffer::MsgDelFriendAck> off_del_friend_ack = MobaGo::FlatBuffer::CreateMsgDelFriendAck(builder, MobaGo::FlatBuffer::FriendRelationOperResult_FROR_SUCCESS);
		builder.Finish(off_del_friend_ack);

		pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgDelFriendAck, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	}

	if (input_msg->operate_type() == friend_oper_type::friend_oper_type_add_confirm)
	{
		// to be deleted
		// ManageSummoner::GetInstance()->addFriend(pSummoner, stcFriendInfo);
		flatbuffers::Offset<MobaGo::FlatBuffer::FriendInfo> off_friend_info = MobaGo::FlatBuffer::CreateFriendInfo(builder, input_msg->friend_id(), 0, 0);
		flatbuffers::Offset<MobaGo::FlatBuffer::MsgConfirmAddFriendAck> off_add_friend_ack = MobaGo::FlatBuffer::CreateMsgConfirmAddFriendAck(builder, off_friend_info);
		builder.Finish(off_add_friend_ack);

		pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgConfirmAddFriendAck, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	}

	if (input_msg->operate_type() == friend_oper_type::friend_oper_type_add_cancel)
	{
		flatbuffers::Offset<MobaGo::FlatBuffer::MsgAddCancelFriendAck> off_cancel_friend_ack = MobaGo::FlatBuffer::CreateMsgAddCancelFriendAck(builder, MobaGo::FlatBuffer::FriendRelationOperResult_FROR_SUCCESS);
		builder.Finish(off_cancel_friend_ack);

		pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgAddCancelFriendAck, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	}

	m_plugin_cfg.sender->sendMsg2Client(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, E_SMsgFriendListAck, MobaGo::FlatBuffer::SMsgFriendListAck)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	const flatbuffers::Vector<flatbuffers::Offset<FriendDetail>> * vec_friend_detail = input_msg->friend_info();
	flatbuffers::FlatBufferBuilder builder;
	std::vector<flatbuffers::Offset<FriendInfo>> vec_friend_info;
	for (unsigned int i = 0; i < vec_friend_detail->size(); ++i)
	{
		const FriendDetail *friend_detail = vec_friend_detail->Get(i);
		flatbuffers::Offset<FriendInfo> friend_info = CreateFriendInfo(builder, friend_detail->friend_id(), friend_detail->head_id(), friend_detail->status());
		vec_friend_info.push_back(friend_info);
	}

	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<FriendInfo>>> off_vec_friend_info = builder.CreateVector(vec_friend_info);
	flatbuffers::Offset<MsgFriendListAck> off_friend_list_ack = CreateMsgFriendListAck(builder, off_vec_friend_info);
	builder.Finish(off_friend_list_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgFriendListAck, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Client(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, E_SMsgApplyForFriendListAck, MobaGo::FlatBuffer::SMsgApplyForFriendListAck)
{
	PackInfo req_pack(R_GetSummonerGuidByGateGuid, input_packet->msg_head.guid, NULL);
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(req_pack.guid)
	CK_NULL_RETURN(summoner, -1)

	const flatbuffers::Vector<flatbuffers::Offset<FriendDetail>> * vec_friend_detail = input_msg->friend_info();
	flatbuffers::FlatBufferBuilder builder;
	std::vector<flatbuffers::Offset<FriendInfo>> vec_friend_info;
	for (unsigned int i = 0; i < vec_friend_detail->size(); ++i)
	{
		const FriendDetail *friend_detail = vec_friend_detail->Get(i);
		flatbuffers::Offset<FriendInfo> friend_info = CreateFriendInfo(builder, friend_detail->friend_id(), friend_detail->head_id(), friend_detail->status());
		vec_friend_info.push_back(friend_info);
	}

	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<FriendInfo>>> off_vec_friend_info = builder.CreateVector(vec_friend_info);
	flatbuffers::Offset<MsgApplyForFriendListAck> off_friend_list_ack = CreateMsgApplyForFriendListAck(builder, off_vec_friend_info);
	builder.Finish(off_friend_list_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgApplyForFriendListAck, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Client(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, E_SMsgFSFriendDetailInfoReq, MobaGo::FlatBuffer::SMsgFriendDetailInfoReq)
{
	Get_Entity_Summoner(input_packet->msg_head.guid);
	if (NULL != summoner)
	{
		// process
	}
	else
	{
		// load from db, callback
		//m_plugin_cfg.pool->load(input_packet->msg_head.guid, std::bind())
	}

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, E_SMsgFSFriendDetailInfoAck, MobaGo::FlatBuffer::SMsgFriendDetailInfoAck)
{
	Get_Entity_Summoner(input_packet->msg_head.guid)
	CK_NULL_RETURN(summoner, -1)

	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MobaGo::FlatBuffer::FriendInfo> off_friend_info = MobaGo::FlatBuffer::CreateFriendInfo(builder, input_msg->friend_id(), input_msg->friend_head_id(), 1);
	flatbuffers::Offset<MsgFriendDetailInfoAck> off_friend_detail_ack = MobaGo::FlatBuffer::CreateMsgFriendDetailInfoAck(builder, off_friend_info);
	builder.Finish(off_friend_detail_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgFriendDetailInfoAck, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Client(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, E_SMsgFriendStatusAck, MobaGo::FlatBuffer::SMsgFriendStatusAck)
{
	Get_Entity_Summoner(input_packet->msg_head.guid)
	CK_NULL_RETURN(summoner, -1)
	
	flatbuffers::FlatBufferBuilder builder;
	vector<flatbuffers::Offset<FriendStatus>> off_vec_friend_status;
	const flatbuffers::Vector<flatbuffers::Offset<SMsgFriendStatus>> * temp_vec_stutus = input_msg->friend_status();
	for (unsigned int i = 0; i < temp_vec_stutus->size(); ++i)
	{
		const SMsgFriendStatus *smsg_friend_status = temp_vec_stutus->Get(i);
		flatbuffers::Offset<FriendStatus> off_friend_status = CreateFriendStatus(builder, smsg_friend_status->friend_id(), (SummonerStatus)smsg_friend_status->friend_status());
		off_vec_friend_status.push_back(off_friend_status);
	}
	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<FriendStatus>>>  vec_friend_status = builder.CreateVector(off_vec_friend_status);
	flatbuffers::Offset<MobaGo::FlatBuffer::MsgFriendStatusAck> off_status_ack = CreateMsgFriendStatusAck(builder, vec_friend_status);
	builder.Finish(off_status_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgFriendsStatusAck, summoner->gate_guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Client(pack);

	return 0;
}

INPUT_FUN_DEFINE(FRSystemVR, E_SMsgNotifySummonerStatusChange, MobaGo::FlatBuffer::SMsgNotifySummonerStatusToFriend)
{
	uint64_t friend_id = input_msg->friend_id();
	PackInfo req_pack(R_GetGateGuidBySummonerId, friend_id, NULL); // friend_id: friend_guid --> friend_gate_guid
	m_plugin_cfg.depot->request(req_pack);

	Get_Entity_Summoner(input_msg->friend_id())
	CK_NULL_RETURN(summoner, -1)							// 实际上summoner是friend信息，宏定义命名的关系， 容易混淆
	InterPacket * pack = new InterPacket();

	uint64_t summoner_id = input_msg->summoner_id();
	int summmoner_status = input_msg->summoner_status();

	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MobaGo::FlatBuffer::FriendStatus> off_summoner_status = MobaGo::FlatBuffer::CreateFriendStatus(builder, summoner_id,(SummonerStatus)input_msg->summoner_status());
	builder.Finish(off_summoner_status);

	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgFriendStatusChange, req_pack.guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Client(pack);

	return 0;
}

NOTIFY_FUN_DEFINE(FRSystemVR, N_CreateSummoner, EmptyMsg /*to be more specific*/)
{
	Get_Entity_Summoner(ntf.guid)
	CK_NULL_RETURN(summoner, -1)

	//uint64_t summoner_id = summoner->guid();

	//通知好友服
	flatbuffers::FlatBufferBuilder builder;
	string sum_name = summoner->sum_name();
	flatbuffers::Offset<flatbuffers::String> off_sum_name = builder.CreateString(sum_name);
	flatbuffers::Offset<SMsgVRNotifyCreateSummonerToVRF> off_friend_info = CreateSMsgVRNotifyCreateSummonerToVRF(builder, summoner->guid(), off_sum_name, 7); // todo list, hard code
	builder.Finish(off_friend_info);

	InterPacket * friend_pack = new InterPacket();
	friend_pack->setMsgInfo(E_SMsgVRNotifyCreateSummonerToVRF, summoner->guid(), builder.GetSize(), (char *)builder.GetBufferPointer());
	m_plugin_cfg.sender->sendMsg2Friend(friend_pack);

	//通知client
	InterPacket * client_pack = new InterPacket();
	client_pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgCreateSummonerAck, summoner->gate_guid() , 0, (const char *)0);
	m_plugin_cfg.sender->sendMsg2Client(client_pack);

	return 0;
}

NOTIFY_FUN_DEFINE(FRSystemVR, N_PlayerOnline, EmptyMsg)
{
	Get_Entity_Summoner(ntf.guid)
	CK_NULL_RETURN(summoner, -1)

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRNotifySummonerLoginToVRF, summoner->guid(), 0, (char *)NULL);
	m_plugin_cfg.sender->sendMsg2Friend(pack);

	return 0;
}

NOTIFY_FUN_DEFINE(FRSystemVR, N_PlayerOffline, EmptyMsg)
{
	m_plugin_cfg.pool->remove(ntf.guid);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRNotifySummonerLogoffToVRF, ntf.guid, 0, (char *)NULL);
	m_plugin_cfg.sender->sendMsg2Friend(pack);

	return 0;
}


