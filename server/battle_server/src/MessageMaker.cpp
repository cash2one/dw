
#include "MessageMaker.h"
#include "battle_generated.h"
#include "opcode.h"

InterPacket * MessageMaker::makeMsgEnterBattleAck(SessionId session_id, uint64_t guid, PlayerMap_t mapPlayer)
{
	InterPacket * pack = new InterPacket();
	FlatBufferBuilder builder;
	vector<Offset<BattlePlayerInfo>> player_info_vec;
	for (auto it : mapPlayer)
	{
		Offset<flatbuffers::String> off_user_name = builder.CreateString(it.second->getName());
		Offset<BattlePlayerInfo> off_player_info = CreateBattlePlayerInfo(builder, it.second->getId(), off_user_name);
		player_info_vec.push_back(off_player_info);
	}
	
	Offset<Vector<Offset<BattlePlayerInfo>>> off_player_info_vec = builder.CreateVector(player_info_vec);
	Offset<MobaGo::FlatBuffer::MsgEnterBattleAck> off_enter_battle_ack = CreateMsgEnterBattleAck(builder, off_player_info_vec);
	builder.Finish(off_enter_battle_ack);

	pack->setMsgInfo(Opcode_E_MsgEnterBattleAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeMsgReEnterBattleAck(SessionId session_id, uint64_t guid, int result)
{
	FlatBufferBuilder builder;
	Offset<MsgReEnterBattleAck> off_ack = CreateMsgReEnterBattleAck(builder, result);
	builder.Finish(off_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgReEnterBattleAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeMsgSelectHeroAck(SessionId session_id, uint64_t guid, int hero_id)
{
	FlatBufferBuilder builder;
	Offset<MsgSelectHeroAck> off_ack = CreateMsgSelectHeroAck(builder, guid, hero_id);
	builder.Finish(off_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgSelectHeroAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeMsgConfirmHeroAck(SessionId session_id, uint64_t guid, int hero_id)
{
	FlatBufferBuilder builder;
	Offset<MsgConfirmHeroAck> off_ack = CreateMsgConfirmHeroAck(builder, guid, hero_id);
	builder.Finish(off_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgConfirmHeroAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeMsgBeforeBattleParamNty(uint64_t guid, const BattleSetting & battle_setting)
{
	FlatBufferBuilder builder;
	Offset<MsgBeforeBattleParamNty> off_msg = CreateMsgBeforeBattleParamNty(builder, battle_setting.select_hero_timeout, battle_setting.buy_skin_timeout);
	builder.Finish(off_msg);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgBeforeBattleParamNty, guid, builder.GetSize(), builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeMsgBattleParamNty(uint64_t guid, const BattleSetting & battle_setting, int random_seed)
{
	FlatBufferBuilder builder;
	Offset<MsgBattleParamNty> off_msg = CreateMsgBattleParamNty(builder, battle_setting.frame_delta, battle_setting.pre_act_frames, random_seed);
	builder.Finish(off_msg);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgBattleParamNty, guid, builder.GetSize(), builder.GetBufferPointer());

	return pack;
}

InterPacket * MessageMaker::makeMsgReportBattleResultAck(uint64_t guid)
{
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgReportBattleResultAck, guid);

	return pack;
}

InterPacket * MessageMaker::makeMsgBattleEndNty(uint64_t guid)
{
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_MsgBattleEndNty, guid);

	return pack;
}

InterPacket * MessageMaker::makeSMsgNotifyBattleMatchResultAck(uint64_t match_battle_id, int result)
{
	FlatBufferBuilder builder;
	Offset<SMsgNotifyBattleMatchResultAck> off_ack = CreateSMsgNotifyBattleMatchResultAck(builder, match_battle_id, result);
	builder.Finish(off_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgNotifyBattleMatchResultAck, 0, builder.GetSize(), builder.GetBufferPointer());

	return pack;
}
