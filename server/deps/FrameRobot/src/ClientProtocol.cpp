
#include "BattleLog.h"
#include "ClientProtocol.h"
#include "battle_generated.h"
#include "robot_generated.h"
#include "friend_generated.h"
#include "Single.hpp"
#include "FRPlugin.h"

ClientPacket * ClientProtocol::makeMsgChooseServerReq(int area_id)
{
	FlatBufferBuilder builder;
	auto off_req = CreateMsgChooseServerReq(builder, area_id);
	builder.Finish(off_req);

	return makeClientPacket(Opcode_E_MsgChooseServerReq, builder.GetBufferPointer(), builder.GetSize());
}

ClientPacket * ClientProtocol::makeMsgLobbyJoinGameReq(const string & account, int hero_skill_id)
{
	FlatBufferBuilder builder;

	Offset <String> off_account = builder.CreateString(account);

	Offset<MsgLobbyJoinGameReq> off_set = CreateMsgLobbyJoinGameReq(builder, off_account, hero_skill_id);
	builder.Finish(off_set);

	return makeClientPacket(Protocol_Lobby_E_PROTOCOL_LOBBY_JOIN_GAME_REQ, builder.GetBufferPointer(), builder.GetSize());
}

ClientPacket * ClientProtocol::makeMsgCombatPlayerReadyReq(uint64_t player_id)
{
	FlatBufferBuilder builder;
	Offset<MsgCombatPlayerReadyReq> off_set = CreateMsgCombatPlayerReadyReq(builder, player_id);
	builder.Finish(off_set);

	return makeClientPacket(Protocol_Combat_E_PROTOCOL_COMBAT_PLAYER_READY_REQ, builder.GetBufferPointer(), builder.GetSize());
}

ClientPacket * ClientProtocol::makeMsgClientFrameSyncReqMove(int player_id, int x, int y, int z)
{
	string c2c_move;
	ClientProtocol::makeC2cMoveBuffer(player_id, x, y, z, c2c_move);

	FlatBufferBuilder frame_sync_builder;
	Offset<Vector<uint8_t> > action_data_off = frame_sync_builder.CreateVector((uint8_t *)c2c_move.data(), c2c_move.size());
	Offset<MsgFrameSyncAction> action_off = CreateMsgFrameSyncAction(frame_sync_builder, 1, action_data_off);

	vector<Offset<MsgFrameSyncAction> > action_off_vec;
	action_off_vec.push_back(action_off);

	Offset<MsgClientFrameSyncReq> sync_off = CreateMsgClientFrameSyncReq(frame_sync_builder, 1, action_off);
	frame_sync_builder.Finish(sync_off);

	ClientPacket * pack = makeClientPacket(Protocol_Combat_E_PROTOCOL_COMBAT_MSG_CLIENT_FRAME_SYNC_REQ, frame_sync_builder.GetBufferPointer(), frame_sync_builder.GetSize());

	//const MsgFrameSyncDetail * test = flatbuffers::GetRoot<MsgFrameSyncDetail>(pack->getBody());

	//flatbuffers::Verifier verifier((uint8_t *)pack->getBody(), pack->msg_head.msg_len);
	//if (test->Verify(verifier))
	//{
	//	BATTLE_LOG_DEBUG("");
	//}

	return pack;
}

void ClientProtocol::makeC2cMoveBuffer(int player_id, int x, int y, int z, string & output_buffer)
{
	IntVec3 vec3(x, y, z);
	FlatBufferBuilder builder;
	Offset<MsgCommandPlayerMove> off_set = CreateMsgCommandPlayerMove(builder, &vec3);
	builder.Finish(off_set);

	FlatBufferBuilder c2cBuilder;
	Offset<Vector<uint8_t> > data_off = c2cBuilder.CreateVector(builder.GetBufferPointer(), builder.GetSize());
	Offset<MsgFrameCommandC2C> c2c_off = CreateMsgFrameCommandC2C(c2cBuilder, player_id, 1, data_off);
	c2cBuilder.Finish(c2c_off);
	output_buffer = string((char *)c2cBuilder.GetBufferPointer(), c2cBuilder.GetSize());
}

ClientPacket * ClientProtocol::makeClientPacket(int msg_id, uint8_t * buffer, int buf_size)
{
	ClientPacket * pack = new ClientPacket();
	pack->msg_head.msg_id = msg_id;
	pack->msg_head.msg_len = buf_size;
	pack->setMsgInfo(pack->msg_head, (char *)buffer);

	return pack;
}

ClientPacket * ClientProtocol::makeMsgLoginReq(const string & account_name)
{
	FlatBufferBuilder builder;
	Offset<String> off_name = builder.CreateString(account_name);
	Offset<String> off_dev = builder.CreateString(account_name);
	Offset<MsgLoginReq> off_login = CreateMsgLoginReq(builder, 1, 0, 0, 1, off_name, off_dev, 0, 1, 0);
	builder.Finish(off_login);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgLoginReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	return pack;
}


ClientPacket * ClientProtocol::makeMsgLoginReq(SessionId session_id, const string & user_name)
{
	FlatBufferBuilder builder;
	Offset<String> off_name = builder.CreateString(user_name);
	Offset<MsgLoginReq> off_req = CreateMsgLoginReq(builder, 1, 0, 1, 1, off_name, 0, 0, 1, 0);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgLoginReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgChooseServerReq(SessionId session_id, int server_id)
{
	FlatBufferBuilder builder;
	Offset<MsgChooseServerReq> off_req = CreateMsgChooseServerReq(builder, server_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgChooseServerReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgVRLoginReq(SessionId session_id, const string & user_name, uint64_t user_id, const string & session_key)
{
	FlatBufferBuilder builder;
	Offset<String> off_name = builder.CreateString(user_name);
	Offset<String> off_key = builder.CreateString(session_key);
	Offset<MsgVRLoginReq> off_req = CreateMsgVRLoginReq(builder, off_key, user_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgVRLoginReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgEnterBattle(SessionId session_id, uint64_t user_id)
{
	FlatBufferBuilder builder;
	Offset<MsgEnterBattleReq> off_battle = CreateMsgEnterBattleReq(builder, user_id);
	builder.Finish(off_battle);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgEnterBattleReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgReEnterBattleReq(SessionId session_id, uint64_t user_id)
{
	FlatBufferBuilder builder;
	Offset<MsgReEnterBattleReq> off_battle = CreateMsgReEnterBattleReq(builder, user_id);
	builder.Finish(off_battle);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgReEnterBattleReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgReqSummonerInfoReq(SessionId session_id)
{
	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgReqSummonerInfoReq, 0, NULL);
	pack->session_id = session_id;

	return pack;
}

ClientPacket* ClientProtocol::makeDemoSystemTestReq(SessionId id)
{	
	ClientPacket * pack = new ClientPacket();
	FlatBufferBuilder builder;
	auto it = CreateDemoSystemTestReq(builder, 1);
	builder.Finish(it);

	pack->setMsgInfo(Opcode_E_DemoSystemTestReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgCreateSummonerReq(SessionId session_id, const string & account_name, const string & summoner_name)
{
	Single<FRPlugin>::GetInstance()->notifyCreateAccount(111111, account_name); // todo list

	FlatBufferBuilder builder;
	Offset<String> off_acc = builder.CreateString(account_name);
	Offset<String> off_summ = builder.CreateString(summoner_name);

	Offset<MsgCreateSummonerReq> off_req = CreateMsgCreateSummonerReq(builder, off_summ, 1);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgCreateSummonerReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgMatchReq(SessionId session_id, int team_type, int match_type)
{
	FlatBufferBuilder builder;
	Offset<MsgMatchReq> off_req = CreateMsgMatchReq(builder, team_type, match_type);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgMatchReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgSelectHeroReq(SessionId session_id, int hero_id)
{
	FlatBufferBuilder builder;
	Offset<MsgSelectHeroReq> off_req = CreateMsgSelectHeroReq(builder, hero_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgSelectHeroReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgConfirmHeroReq(SessionId session_id, int hero_id)
{
	FlatBufferBuilder builder;
	Offset<MsgConfirmHeroReq> off_req = CreateMsgConfirmHeroReq(builder, hero_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgConfirmHeroReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgReportBattleResultReq()
{
	FlatBufferBuilder builder;
	Offset<MsgReportBattleResultReq> off_req = CreateMsgReportBattleResultReq(builder, 1);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgReportBattleResultReq, builder.GetSize(), (char *)builder.GetBufferPointer());

	return pack;
}

ClientPacket * ClientProtocol::makeMsgSearchFriendReq(SessionId session_id, string friend_name)
{
	FlatBufferBuilder builder;
	Offset<String> off_friend_name = builder.CreateString(friend_name);
	Offset<MsgSearchFriendReq> off_req = CreateMsgSearchFriendReq(builder, off_friend_name);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgSearchFriendReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgAddFriendReq(SessionId session_id, uint64_t friend_id)
{
	FlatBufferBuilder builder;
	Offset<MsgAddFriendReq> off_req = CreateMsgAddFriendReq(builder, friend_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgAddFriendReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgDelFriendReq(SessionId session_id, uint64_t friend_id)
{
	FlatBufferBuilder builder;
	Offset<MsgDelFriendReq> off_req = CreateMsgDelFriendReq(builder, friend_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgDelFriendReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgConnfirmAddFriendReq(SessionId session_id, uint64_t friend_id)
{
	FlatBufferBuilder builder;
	Offset<MsgConfirmAddFriendReq> off_req = CreateMsgConfirmAddFriendReq(builder, friend_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgConfirmAddFriendReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgAddCancelFriendReq(SessionId session_id, uint64_t friend_id)
{
	FlatBufferBuilder builder;
	Offset<MsgAddCancelFriendReq> off_req = CreateMsgAddCancelFriendReq(builder, friend_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgAddCancelFriendReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgFriendListReq(SessionId session_id)
{
	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgFriendListReq, 0, (char *)NULL);
	pack->session_id = session_id;

	return pack;
}
ClientPacket * ClientProtocol::makeMsgApplyForFriendListReq(SessionId session_id)
{
	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgApplyForFriendListReq, 0, (char *)NULL);
	pack->session_id = session_id;

	return pack;
}

ClientPacket * ClientProtocol::makeMsgFriendDetailInfoReq(SessionId session_id, uint64_t friend_id)
{
	FlatBufferBuilder builder;
	Offset<MsgFriendDetailInfoReq> off_req = CreateMsgFriendDetailInfoReq(builder, friend_id);
	builder.Finish(off_req);

	ClientPacket * pack = new ClientPacket();
	pack->setMsgInfo(Opcode_E_MsgFriendDetailInfoReq, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	return pack;
}


