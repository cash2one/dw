#include "MessageMaker.h"
#include "cache_auto_assistant.h"
#include "protocol_generated.h"
#include "ManageVrSession.h"


using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;

void MessageMaker::sendVRErrorPacket(session_t session_id, uint64_t guid, MobaGo::FlatBuffer::InterError error_code, ManageVrSession *pSession)
{
	InterPacket * pack = new InterPacket();

	flatbuffers::FlatBufferBuilder build;
	flatbuffers::Offset<MobaGo::FlatBuffer::MsgError> req_off = MobaGo::FlatBuffer::CreateMsgError(build, error_code);
	build.Finish(req_off);

	pack->setMsgInfo(E_SMsgInterError, guid, build.GetSize(), (char *)build.GetBufferPointer());

	pSession->handleOutputStream(session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
}

InterPacket * MessageMaker::makeSMsgVRFriendNotifySearchFriendAck(SessionId session_id, uint64_t guid, uint64_t friend_id, string friend_name, int gs_id) // 
{

	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgNotifySearchFriendAck> off_search_friend_ack = CreateSMsgNotifySearchFriendAck(builder, friend_id, 0);
	builder.Finish(off_search_friend_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgNotifySearchFriendAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeSMsgNotifySearchFriendAck(SessionId session_id, uint64_t guid, uint64_t sum_id, uint64_t friend_id, int friend_head_id) //注释掉
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgNotifySearchFriendAck> off_search_friend_ack = CreateSMsgNotifySearchFriendAck(builder, friend_id, friend_head_id);
	builder.Finish(off_search_friend_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgNotifySearchFriendAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeSMsgNotifyFriendOperateReq(SessionId session_id, uint64_t sum_id, uint64_t friend_id, friend_oper_type oper_type)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgVRFNotifyFriendOperateReq> off_friend_oper_req = CreateSMsgVRFNotifyFriendOperateReq(builder, friend_id, oper_type, sum_id);
	builder.Finish(off_friend_oper_req);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRFriendNotifyFriendOperateReq, friend_id, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}
InterPacket * MessageMaker::makeSMsgNotifyFriendOperateAck(SessionId session_id, uint64_t guid, uint64_t sum_id, uint64_t friend_id, friend_oper_type oper_type)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgVRFNotifyFriendOperateAck> off_friend_oper_ack = CreateSMsgVRFNotifyFriendOperateAck(builder, friend_id, oper_type, sum_id);
	builder.Finish(off_friend_oper_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVRFriendNotifyFriendOperateAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeSMsgFriendDetailInfoReq(SessionId session_id, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgFriendDetailInfoReq> off_friend_detail_req = CreateSMsgFriendDetailInfoReq(builder, summoner_id, friend_id);
	builder.Finish(off_friend_detail_req);

	InterPacket * packet = new InterPacket();
	packet->setMsgInfo(E_SMsgFSFriendDetailInfoReq, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	packet->session_id = session_id;

	return packet;
}

InterPacket * MessageMaker::makeSMsgFriendDetailInfoAck(SessionId session_id, uint64_t summoner_id, uint64_t friend_id, int friend_head_id)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgFriendDetailInfoAck> off_friend_detail_ack = CreateSMsgFriendDetailInfoAck(builder, summoner_id, friend_id, friend_head_id);
	builder.Finish(off_friend_detail_ack);

	InterPacket * packet = new InterPacket();
	packet->setMsgInfo(E_SMsgFSFriendDetailInfoAck, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	packet->session_id = session_id;

	return packet;
}

InterPacket * MessageMaker::makeSMsgNotifySummonerStatusToFriends(SessionId session_id, uint64_t summmoner_id, uint64_t friend_id, MobaGo::FlatBuffer::SummonerStatus sum_status)
{
	flatbuffers::FlatBufferBuilder builder;
	Offset<SMsgNotifySummonerStatusToFriend> off_summoner_status_notify = CreateSMsgNotifySummonerStatusToFriend(builder, summmoner_id, friend_id, sum_status);
	builder.Finish(off_summoner_status_notify);

	InterPacket * packet = new InterPacket();
	packet->setMsgInfo(E_SMsgNotifySummonerStatusChange, friend_id, builder.GetSize(), (char *)builder.GetBufferPointer()); // 把自己的状态推给friend， 所以接收消息的是friend_id
	packet->session_id = session_id;

	return packet;
}

InterPacket * MessageMaker::makeSMsgNotifyFriendsStatusToSummoner(SessionId session_id, uint64_t guid, uint64_t summoner_id, google::protobuf::RepeatedField< ::google::protobuf::uint64 > friend_list)
{
	flatbuffers::FlatBufferBuilder builder;
	std::vector<flatbuffers::Offset<SMsgFriendStatus>> std_vec_friend_status;

	for (int i = 0; i < friend_list.size(); ++i)
	{
		flatbuffers::Offset<SMsgFriendStatus> off_friend_status;
		uint64_t friend_id = friend_list.Get(i);
		Summoner * pSummmoner = ManageSummoner::GetInstance()->findSummoner(friend_id);
		if (pSummmoner != NULL)
		{
			off_friend_status = CreateSMsgFriendStatus(builder, friend_id, pSummmoner->sum_status);
		}
		else
		{
			off_friend_status = CreateSMsgFriendStatus(builder, friend_id, SummonerStatus_Offline);
		}
		std_vec_friend_status.push_back(off_friend_status);
	}

	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<SMsgFriendStatus>>>  off_vec_friend_status = builder.CreateVector(std_vec_friend_status);
	flatbuffers::Offset<SMsgFriendStatusAck> off_friend_status_ack = CreateSMsgFriendStatusAck(builder, summoner_id, off_vec_friend_status);
	builder.Finish(off_friend_status_ack);

	InterPacket * packet = new InterPacket();
	packet->setMsgInfo(E_SMsgFriendStatusAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	packet->session_id = session_id;

	return packet;
}