#include "make_common.h"

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

