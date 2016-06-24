#include "make_battle.h"

InterPacket * makeMsgEnterBattleReq(int msg_id, uint64_t guid, uint64_t user_id, const string & session_key)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> session_key_off = builder.CreateString(session_key);
	flatbuffers::Offset<MsgEnterBattleReq> off_set = CreateMsgEnterBattleReq(builder ,user_id, session_key_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeBattlePlayerInfo(int msg_id, uint64_t guid, uint64_t player_id, const string & player_name)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> player_name_off = builder.CreateString(player_name);
	flatbuffers::Offset<BattlePlayerInfo> off_set = CreateBattlePlayerInfo(builder ,player_id, player_name_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgReEnterBattleReq(int msg_id, uint64_t guid, uint64_t user_id, const string & session_key)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> session_key_off = builder.CreateString(session_key);
	flatbuffers::Offset<MsgReEnterBattleReq> off_set = CreateMsgReEnterBattleReq(builder ,user_id, session_key_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgReEnterBattleAck(int msg_id, uint64_t guid, int8_t result)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgReEnterBattleAck> off_set = CreateMsgReEnterBattleAck(builder ,result);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgSelectHeroReq(int msg_id, uint64_t guid, uint32_t hero_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgSelectHeroReq> off_set = CreateMsgSelectHeroReq(builder ,hero_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgSelectHeroAck(int msg_id, uint64_t guid, uint64_t player_id, uint32_t hero_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgSelectHeroAck> off_set = CreateMsgSelectHeroAck(builder ,player_id, hero_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgConfirmHeroReq(int msg_id, uint64_t guid, uint32_t hero_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgConfirmHeroReq> off_set = CreateMsgConfirmHeroReq(builder ,hero_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgConfirmHeroAck(int msg_id, uint64_t guid, uint64_t player_id, uint32_t hero_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgConfirmHeroAck> off_set = CreateMsgConfirmHeroAck(builder ,player_id, hero_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

