#include "make_framecmd_c2s.h"

InterPacket * makeMsgBody(int msg_id, uint64_t guid, const std::vector<unknown> & data)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data_off = builder.CreateVector(data);
	flatbuffers::Offset<MsgBody> off_set = CreateMsgBody(builder ,data_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeIntVec3(int msg_id, uint64_t guid, int x, int y, int z)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<IntVec3> off_set = CreateIntVec3(builder ,x, y, z);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandUseObjectiveSkill(int msg_id, uint64_t guid, uint32_t uObjectId, int8_t chSlotType, uint32_t uSkillId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandUseObjectiveSkill> off_set = CreateMsgCommandUseObjectiveSkill(builder ,uObjectId, chSlotType, uSkillId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgFrameCommandC2S(int msg_id, uint64_t guid, uint64_t uObjectId, uint8_t bytSyncType, const std::vector<unknown> & aData)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::Vector<uint8_t>> aData_off = builder.CreateVector(aData);
	flatbuffers::Offset<MsgFrameCommandC2S> off_set = CreateMsgFrameCommandC2S(builder ,uObjectId, bytSyncType, aData_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

