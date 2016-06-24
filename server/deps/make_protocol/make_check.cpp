#include "make_check.h"

InterPacket * makeMsgClientValidation(int msg_id, uint64_t guid, uint32_t frame_no, const string & check_value)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> check_value_off = builder.CreateString(check_value);
	flatbuffers::Offset<MsgClientValidation> off_set = CreateMsgClientValidation(builder ,frame_no, check_value_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

