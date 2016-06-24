#include "make_friend.h"

InterPacket * makeFriendInfo(int msg_id, uint64_t guid, uint64_t friend_id, uint32_t head_id, uint8_t status)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<FriendInfo> off_set = CreateFriendInfo(builder ,friend_id, head_id, status);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgAddFriendReq(int msg_id, uint64_t guid, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgAddFriendReq> off_set = CreateMsgAddFriendReq(builder ,friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgAddFriendAck(int msg_id, uint64_t guid, uint8_t add_friend_result)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgAddFriendAck> off_set = CreateMsgAddFriendAck(builder ,add_friend_result);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgDelFriendReq(int msg_id, uint64_t guid, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgDelFriendReq> off_set = CreateMsgDelFriendReq(builder ,friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgDelFriendAck(int msg_id, uint64_t guid, uint8_t del_friend_result)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgDelFriendAck> off_set = CreateMsgDelFriendAck(builder ,del_friend_result);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgConfirmAddFriendReq(int msg_id, uint64_t guid, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgConfirmAddFriendReq> off_set = CreateMsgConfirmAddFriendReq(builder ,friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgAddCancelFriendReq(int msg_id, uint64_t guid, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgAddCancelFriendReq> off_set = CreateMsgAddCancelFriendReq(builder ,friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgAddCancelFriendAck(int msg_id, uint64_t guid, uint8_t confirm_cancel_result)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgAddCancelFriendAck> off_set = CreateMsgAddCancelFriendAck(builder ,confirm_cancel_result);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgSearchFriendReq(int msg_id, uint64_t guid, const string & friend_name)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> friend_name_off = builder.CreateString(friend_name);
	flatbuffers::Offset<MsgSearchFriendReq> off_set = CreateMsgSearchFriendReq(builder ,friend_name_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgVRFriendOperateReq(int msg_id, uint64_t guid, uint64_t friend_id, uint8_t operate_type)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgVRFriendOperateReq> off_set = CreateMsgVRFriendOperateReq(builder ,friend_id, operate_type);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgFriendDetailInfoReq(int msg_id, uint64_t guid, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgFriendDetailInfoReq> off_set = CreateMsgFriendDetailInfoReq(builder ,friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeFriendStatus(int msg_id, uint64_t guid, uint64_t friend_id, int8_t status)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<FriendStatus> off_set = CreateFriendStatus(builder ,friend_id, status);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

