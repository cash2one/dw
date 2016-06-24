
#include "MessageMaker.h"
#include "ManageFriendSession.h"
#include "friend_generated.h"
#include "inter_generated.h"
#include "opcode.h"

InterPacket * MessageMaker::makeSMsgVrRegisterVrpublic(SessionId session_id, int area_id, int server_id, const string & gate_ip_addr, int gate_port)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> off_ip_addr = builder.CreateString(gate_ip_addr);
	flatbuffers::Offset<SMsgVrRegisterVrpublic> off_set = CreateSMsgVrRegisterVrpublic(builder, area_id, server_id, off_ip_addr, gate_port);
	builder.Finish(off_set);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVrRegisterVrpublic, 0, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeSMsgGateRegister(SessionId session_id, const string & gate_ip, int gate_port)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<String> ip_addr_off = builder.CreateString(gate_ip);

	Offset<SMsgGateRegister> off_reg = CreateSMsgGateRegister(builder, ip_addr_off, gate_port);

	builder.Finish(off_reg);

	InterPacket * inter_pack = new InterPacket();
	inter_pack->setMsgInfo(E_SMsgGateRegister, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	inter_pack->session_id = session_id;

	return inter_pack;
}

void MessageMaker::sendErrorPacket(session_t session_id, uint64_t guid, MobaGo::FlatBuffer::Error error_code)
{
	InterPacket * pack = new InterPacket();

	flatbuffers::FlatBufferBuilder build;
	flatbuffers::Offset<MobaGo::FlatBuffer::MsgError> req_off = MobaGo::FlatBuffer::CreateMsgError(build, error_code);
	build.Finish(req_off);

	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgError, guid, build.GetSize(), (char *)build.GetBufferPointer());
	if (pack)
	{
		ManagerClientSession::instance()->handleOutputStream(session_id, (char *)pack->getStream(), pack->getStreamSize());
		pack->cleanStream();
		delete pack;
	}
}


void MessageMaker::sendVRLoginAck(session_t session_id, uint64_t guid, int nReconnectBattle)
{
	InterPacket * pack = new InterPacket();

	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MobaGo::FlatBuffer::MsgVRLoginAck> off_login_ack = MobaGo::FlatBuffer::CreateMsgVRLoginAck(builder, nReconnectBattle);
	builder.Finish(off_login_ack);

	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgVRLoginAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	if (pack)
	{
		ManagerClientSession::instance()->handleOutputStream(session_id, (char *)pack->getStream(), pack->getStreamSize());
		pack->cleanStream();
		delete pack;
	}
}

void MessageMaker::sendReqSummonerInfoAck(session_t session_id, uint64_t guid, Summoner * pSummoner)
{
	InterPacket * pack = new InterPacket();

	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> off_summoner_name = builder.CreateString(pSummoner->m_strSumName);
	flatbuffers::Offset<MobaGo::FlatBuffer::MsgReqSummonerInfoAck> off_login_ack = MobaGo::FlatBuffer::CreateMsgReqSummonerInfoAck(builder, off_summoner_name);
	builder.Finish(off_login_ack);

	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgReqSummonerInfoAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	if (pack)
	{
		ManagerClientSession::instance()->handleOutputStream(session_id, (char *)pack->getStream(), pack->getStreamSize());
		pack->cleanStream();
		delete pack;
	}
}
void MessageMaker::sendCreateSummonerAck(session_t session_id, uint64_t guid, uint64_t sum_id)
{
	InterPacket * pack = new InterPacket();

	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgCreateSummonerAck, guid, 0, (const char *)0);
	if (pack)
	{
		ManagerClientSession::instance()->handleOutputStream(session_id, (char *)pack->getStream(), pack->getStreamSize());
		pack->cleanStream();
		delete pack;
	}
}

void MessageMaker::sendMsgFriendListAck(SessionId session_id, uint64_t guid, MAP_FRIEND map_friend)
{
	InterPacket * pack = new InterPacket();
	flatbuffers::FlatBufferBuilder builder;

	std::vector<Offset<FriendInfo>> vec_off_friend;
	vec_off_friend.clear();
	MAP_FRIEND::iterator it = map_friend.begin();
	for (; it != map_friend.end(); ++it)
	{
		STC_FRIEND_INFO & stcFriend = it->second;
		Offset<FriendInfo> off_friend = CreateFriendInfo(builder, stcFriend.m_nId, 0, 0); // ¡Ÿ ±÷µ
		vec_off_friend.push_back(off_friend);
	}

	Offset<Vector<Offset<FriendInfo>>> friend_vec = builder.CreateVector(vec_off_friend);
	Offset<MsgFriendListAck> off_friend_list_ack = CreateMsgFriendListAck(builder, friend_vec);
	builder.Finish(off_friend_list_ack);

	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgFriendListAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	if (pack)
	{
		ManagerClientSession::instance()->handleOutputStream(session_id, (char *)pack->getStream(), pack->getStreamSize());
		pack->cleanStream();
		delete pack;
	}
}

InterPacket * MessageMaker::makeSmsgVRNotifyCreateSummonerToVRF(SessionId session_id, uint64_t sum_id, string sum_name, int server_id)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<String> off_sum_name = builder.CreateString(sum_name);
	Offset<SMsgVRNotifyCreateSummonerToVRF> off_friend_info = CreateSMsgVRNotifyCreateSummonerToVRF(builder, sum_id, off_sum_name, server_id);
	builder.Finish(off_friend_info);

	InterPacket * pack = new InterPacket(); 
	pack->setMsgInfo(E_SMsgVRNotifyCreateSummonerToVRF, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeSMsgNotifySearchFriend(SessionId session_id, uint64_t guid, uint64_t sum_id, string friend_name)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<String> off_friend_name = builder.CreateString(friend_name);
	Offset<SMsgVRNotifySearchFriend> off_search_friend = CreateSMsgVRNotifySearchFriend(builder, sum_id, off_friend_name);
	builder.Finish(off_search_friend);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgNotifySearchFriend, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeSMsgNotifySearchFriendAck(SessionId session_id, uint64_t guid, uint64_t sum_id, Summoner *pFriend)
{
	flatbuffers::FlatBufferBuilder builder;
	uint64_t friend_id = pFriend->m_nId;
	Offset<SMsgNotifySearchFriendAck> off_search_friend_ack = CreateSMsgNotifySearchFriendAck(builder, friend_id, 0); // 0 ---> head_id
	builder.Finish(off_search_friend_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgNotifySearchFriendAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeSMsgVRAddFriendReq(SessionId session_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgVRNotifyAddFriendReq> off_add_friend = CreateSMsgVRNotifyAddFriendReq(builder, summoner_id, friend_id);
	builder.Finish(off_add_friend);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRNotifyAddFriendReq, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeSMsgVRDelFriendReq(SessionId session_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgVRNotifyDelFriendReq> off_del_friend = CreateSMsgVRNotifyDelFriendReq(builder, summoner_id, friend_id);
	builder.Finish(off_del_friend);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRNotifyDelFriendReq, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}


InterPacket * MessageMaker::makeSMsgVRConfirmAddFriendReq(SessionId session_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgVRNotifyConfirmAddFriendReq> off_add_friend = CreateSMsgVRNotifyConfirmAddFriendReq(builder, summoner_id, friend_id);
	builder.Finish(off_add_friend);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRNotifyConfirmAddFriendReq, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}
InterPacket * MessageMaker::makeSMsgVRAddCancelFriendReq(SessionId session_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgVRNotifyAddCancelFriendReq> off_add_cancel_friend = CreateSMsgVRNotifyAddCancelFriendReq(builder, summoner_id, friend_id);
	builder.Finish(off_add_cancel_friend);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRNotifyAddCancelFriendReq, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeSMsgVrRegisterTr(SessionId session_id, int area_id, int gs_id)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgVrRegisterTr> off_reg = CreateSMsgVrRegisterTr(builder, area_id, gs_id);
	builder.Finish(off_reg);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVrRegisterTr, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeSMsgVrRegisterVRFriend(SessionId session_id, int area_id, int gs_id)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgVrRegisterVRFriend> off_reg = CreateSMsgVrRegisterVRFriend(builder, area_id, gs_id);
	builder.Finish(off_reg);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVrRegisterVRFriend, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeSMsgMatchReq(MyGuid_t user_guid, int team_type, int match_type, vector<MyGuid_t> & group_member)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<Vector<uint64_t>> off_group_mem = builder.CreateVector(group_member);
	Offset<SMsgMatchReq> off_req = CreateSMsgMatchReq(builder, team_type, match_type, off_group_mem);
	builder.Finish(off_req);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgMatchReq, user_guid, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeMsgCancelMatchReq(MyGuid_t user_guid)
{
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgCancelMatchReq, user_guid, 0, (const char *)0);
	return pack;
}

InterPacket * MessageMaker::makeSMsgFriendListReq(SessionId session_id, MyGuid_t guid, uint64_t summoner_id)
{
	InterPacket * pack = new InterPacket();
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgFriendListReq> off_friend_list_req = CreateSMsgFriendListReq(builder, summoner_id);
	builder.Finish(off_friend_list_req);

	pack->setMsgInfo(E_SMsgFriendListReq, guid, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}
InterPacket * MessageMaker::makeSMsgApplyForFriendListReq(SessionId session_id, MyGuid_t guid, uint64_t summoner_id)
{
	InterPacket * pack = new InterPacket();
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgApplyForFriendListReq> off_apply_for_friend = CreateSMsgApplyForFriendListReq(builder, summoner_id);
	builder.Finish(off_apply_for_friend);

	pack->setMsgInfo(E_SMsgApplyForFriendListReq, guid, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeSMsgFriendDetailInfoReq(SessionId session_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	InterPacket * pack = new InterPacket();
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgFriendDetailInfoReq> off_friend_detail_req = CreateSMsgFriendDetailInfoReq(builder, summoner_id, friend_id);
	builder.Finish(off_friend_detail_req);

	pack->setMsgInfo(E_SMsgVRFriendDetailInfoReq, guid, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeSMsgFriendDetailInfoAck(SessionId session_id, uint64_t summoner_id, Summoner * pFriend)
{
	InterPacket * pack = new InterPacket();
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgFriendDetailInfoAck> off_friend_detail_ack = CreateSMsgFriendDetailInfoAck(builder, summoner_id, pFriend->m_nId, 0);
	builder.Finish(off_friend_detail_ack);

	pack->setMsgInfo(E_SMsgVRFriendDetailInfoAck, 0, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeSMsgFriendStatusReq(SessionId session_id, uint64_t guid, uint64_t summoner_id)
{
	InterPacket * packet = new InterPacket();
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgFriendStatusReq> off_friend_status_req = CreateSMsgFriendStatusReq(builder, summoner_id);
	builder.Finish(off_friend_status_req);

	packet->setMsgInfo(E_SMsgFriendStatusReq, guid, builder.GetSize(), (char *)builder.GetBufferPointer());

	return packet;

}