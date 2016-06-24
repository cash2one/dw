#include "make_match.h"

InterPacket * makeMsgMatchReq(int msg_id, uint64_t guid, uint8_t team_type, uint8_t match_type)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgMatchReq> off_set = CreateMsgMatchReq(builder ,team_type, match_type);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgMatchAck(int msg_id, uint64_t guid, uint8_t match_result)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgMatchAck> off_set = CreateMsgMatchAck(builder ,match_result);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCancelMatchAck(int msg_id, uint64_t guid, uint8_t result)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCancelMatchAck> off_set = CreateMsgCancelMatchAck(builder ,result);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgMatchEnterBattle(int msg_id, uint64_t guid, const string & server_ip, int server_port, const string & session_key)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> server_ip_off = builder.CreateString(server_ip);
	flatbuffers::Offset<flatbuffers::String> session_key_off = builder.CreateString(session_key);
	flatbuffers::Offset<MsgMatchEnterBattle> off_set = CreateMsgMatchEnterBattle(builder ,server_ip_off, server_port, session_key_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgReconnectBattle(int msg_id, uint64_t guid, const string & server_ip, int server_port, const string & session_key)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> server_ip_off = builder.CreateString(server_ip);
	flatbuffers::Offset<flatbuffers::String> session_key_off = builder.CreateString(session_key);
	flatbuffers::Offset<MsgReconnectBattle> off_set = CreateMsgReconnectBattle(builder ,server_ip_off, server_port, session_key_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

