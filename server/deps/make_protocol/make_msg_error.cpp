#include "make_msg_error.h"

InterPacket * makeMsgError(int msg_id, uint64_t guid, uint32_t error_code)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgError> off_set = CreateMsgError(builder ,error_code);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

