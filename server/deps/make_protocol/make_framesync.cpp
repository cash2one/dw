#include "make_framesync.h"

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

InterPacket * makeMsgFrameSyncAction(int msg_id, uint64_t guid, uint8_t bytType, const std::vector<unknown> & aData)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::Vector<uint8_t>> aData_off = builder.CreateVector(aData);
	flatbuffers::Offset<MsgFrameSyncAction> off_set = CreateMsgFrameSyncAction(builder ,bytType, aData_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgRepairFrameReq(int msg_id, uint64_t guid, const std::vector<unknown> & aFrames)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::Vector<uint32_t>> aFrames_off = builder.CreateVector(aFrames);
	flatbuffers::Offset<MsgRepairFrameReq> off_set = CreateMsgRepairFrameReq(builder ,aFrames_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgReconnectionReq(int msg_id, uint64_t guid, uint8_t bytIsNew)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgReconnectionReq> off_set = CreateMsgReconnectionReq(builder ,bytIsNew);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgReconnectionAck(int msg_id, uint64_t guid, uint8_t bytIsEOF, const std::vector<unknown> & oFrameData)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::Vector<uint8_t>> oFrameData_off = builder.CreateVector(oFrameData);
	flatbuffers::Offset<MsgReconnectionAck> off_set = CreateMsgReconnectionAck(builder ,bytIsEOF, oFrameData_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makePBattleEndReportPlayer(int msg_id, uint64_t guid, uint64_t account_id, uint16_t kill_num, uint16_t death_num, uint16_t assist_num)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<PBattleEndReportPlayer> off_set = CreatePBattleEndReportPlayer(builder ,account_id, kill_num, death_num, assist_num);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgBeforeBattleParamNty(int msg_id, uint64_t guid, uint8_t select_hero_timeout, uint8_t buy_skin_timeout)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgBeforeBattleParamNty> off_set = CreateMsgBeforeBattleParamNty(builder ,select_hero_timeout, buy_skin_timeout);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgBattleParamNty(int msg_id, uint64_t guid, uint8_t frame_delta, uint8_t pre_act_frames, uint32_t random_seed)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgBattleParamNty> off_set = CreateMsgBattleParamNty(builder ,frame_delta, pre_act_frames, random_seed);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgBattleCountDownNty(int msg_id, uint64_t guid, uint8_t count_down_time)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgBattleCountDownNty> off_set = CreateMsgBattleCountDownNty(builder ,count_down_time);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

