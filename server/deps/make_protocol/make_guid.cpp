#include "make_guid.h"

InterPacket * makeSMsgRequestGuidReq(int msg_id, uint64_t guid, uint32_t request_number)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgRequestGuidReq> off_set = CreateSMsgRequestGuidReq(builder ,request_number);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgRequestGuidAck(int msg_id, uint64_t guid, uint32_t seed_index, uint64_t start_index, uint32_t create_number)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgRequestGuidAck> off_set = CreateSMsgRequestGuidAck(builder ,seed_index, start_index, create_number);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

