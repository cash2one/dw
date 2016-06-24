
#include "MessageMaker.h"

InterPacket * MessageMaker::makeMsgMatchEnterBattle(MyGuid_t user_id, SessionId session_id, const string & server_ip, int server_port)
{
	FlatBufferBuilder builder;
	Offset<String> off_ip = builder.CreateString(server_ip);
	Offset<String> off_key = builder.CreateString("test_key");
	Offset<MsgMatchEnterBattle> off_enter = CreateMsgMatchEnterBattle(builder, off_ip, server_port, off_key);
	builder.Finish(off_enter);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_MsgMatchEnterBattle_Copy, user_id, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;
	return pack;
}

InterPacket * MessageMaker::makeSMsgNotifyBattleMatchResult(SessionId session_id, uint64_t match_battle_id, MatchCellVec_t & match_cell_vec)
{
	FlatBufferBuilder builder;
	vector<Offset<PPlayerInfo>> off_player_info_vec;
	for (auto it = match_cell_vec.begin(); it != match_cell_vec.end(); ++it)
	{
		MatchCell * cell = *it;
		Offset<PPlayerInfo> off_pi = CreatePPlayerInfo(builder, cell->getUserId());
		off_player_info_vec.push_back(off_pi);
	}

	Offset<Vector<Offset<PPlayerInfo>>> off_pi = builder.CreateVector(off_player_info_vec);
	Offset<SMsgNotifyBattleMatchResult> off_match_result_set = CreateSMsgNotifyBattleMatchResult(builder, match_battle_id, 1, 1, off_pi);
	builder.Finish(off_match_result_set);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgNotifyBattleMatchResult, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;
	return pack;
}

InterPacket * MessageMaker::makeMsgMatchAck(SessionId session_id, uint64_t account_id, int result)
{
	FlatBufferBuilder builder;
	Offset<MsgMatchAck> off_ack = CreateMsgMatchAck(builder, result);
	builder.Finish(off_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_MsgMatchAck_Copy, account_id, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

InterPacket * MessageMaker::makeMsgCancelMatchAck(SessionId session_id, uint64_t account_id, int result)
{
	FlatBufferBuilder builder;
	Offset<MsgCancelMatchAck> off_ack = CreateMsgCancelMatchAck(builder, result);
	builder.Finish(off_ack);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_MsgCancelMatchAck_Copy, account_id, builder.GetSize(), builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}


