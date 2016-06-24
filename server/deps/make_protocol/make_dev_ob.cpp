#include "make_dev_ob.h"

InterPacket * makeSMsgRegisterToDevOb(int msg_id, uint64_t guid, uint32_t gs_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgRegisterToDevOb> off_set = CreateSMsgRegisterToDevOb(builder ,gs_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgObReportGateInfo(int msg_id, uint64_t guid, const string & gate_ip, int gate_port)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> gate_ip_off = builder.CreateString(gate_ip);
	flatbuffers::Offset<SMsgObReportGateInfo> off_set = CreateSMsgObReportGateInfo(builder ,gate_ip_off, gate_port);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makePObPlayerInfo(int msg_id, uint64_t guid, uint64_t uPlayerUid, uint32_t uHeroSkinId, uint8_t bytCamp)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<PObPlayerInfo> off_set = CreatePObPlayerInfo(builder ,uPlayerUid, uHeroSkinId, bytCamp);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgUpdateBattleFrame(int msg_id, uint64_t guid, uint32_t battle_id, uint32_t frame_no, const std::vector<unknown> & frame_stream)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::Vector<int8_t>> frame_stream_off = builder.CreateVector(frame_stream);
	flatbuffers::Offset<SMsgUpdateBattleFrame> off_set = CreateSMsgUpdateBattleFrame(builder ,battle_id, frame_no, frame_stream_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgUpdateBattleStatus(int msg_id, uint64_t guid, uint32_t battle_id, uint32_t status, uint32_t is_end)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgUpdateBattleStatus> off_set = CreateSMsgUpdateBattleStatus(builder ,battle_id, status, is_end);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makePObInfo(int msg_id, uint64_t guid, uint32_t ob_id, const string & start_time, uint32_t one_team_num, uint32_t status)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> start_time_off = builder.CreateString(start_time);
	flatbuffers::Offset<PObInfo> off_set = CreatePObInfo(builder ,ob_id, start_time_off, one_team_num, status);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgSelectObReq(int msg_id, uint64_t guid, uint32_t ob_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgSelectObReq> off_set = CreateMsgSelectObReq(builder ,ob_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgSelectObAck(int msg_id, uint64_t guid, const string & ob_ip_addr, int ob_port)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> ob_ip_addr_off = builder.CreateString(ob_ip_addr);
	flatbuffers::Offset<MsgSelectObAck> off_set = CreateMsgSelectObAck(builder ,ob_ip_addr_off, ob_port);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgEnterDevObReq(int msg_id, uint64_t guid, uint32_t ob_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgEnterDevObReq> off_set = CreateMsgEnterDevObReq(builder ,ob_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

