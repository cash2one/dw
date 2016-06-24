#include "make_lobby.h"

InterPacket * makeMsgPing(int msg_id, uint64_t guid, uint64_t client_start_time, uint64_t gate_recv_time, uint64_t server_recv_time)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgPing> off_set = CreateMsgPing(builder ,client_start_time, gate_recv_time, server_recv_time);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgLobbyKeepAliveReq(int msg_id, uint64_t guid, uint32_t uReserved)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgLobbyKeepAliveReq> off_set = CreateMsgLobbyKeepAliveReq(builder ,uReserved);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgLobbyJoinGameReq(int msg_id, uint64_t guid, const string & sUserName, uint32_t uHeroSkinId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> sUserName_off = builder.CreateString(sUserName);
	flatbuffers::Offset<MsgLobbyJoinGameReq> off_set = CreateMsgLobbyJoinGameReq(builder ,sUserName_off, uHeroSkinId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgLobbyJoinGameAck(int msg_id, uint64_t guid, uint8_t bytIsSuccess, uint64_t uPlayerUid, uint8_t bytCamp)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgLobbyJoinGameAck> off_set = CreateMsgLobbyJoinGameAck(builder ,bytIsSuccess, uPlayerUid, bytCamp);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgGamePlayerInfo(int msg_id, uint64_t guid, uint64_t uPlayerUid, uint32_t uHeroSkinId, uint8_t bytCamp)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgGamePlayerInfo> off_set = CreateMsgGamePlayerInfo(builder ,uPlayerUid, uHeroSkinId, bytCamp);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCombatPlayerReadyReq(int msg_id, uint64_t guid, uint64_t uPlayerUid)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCombatPlayerReadyReq> off_set = CreateMsgCombatPlayerReadyReq(builder ,uPlayerUid);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCombatPlayerReadyAck(int msg_id, uint64_t guid, uint8_t bytIsAlreadyRunning)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCombatPlayerReadyAck> off_set = CreateMsgCombatPlayerReadyAck(builder ,bytIsAlreadyRunning);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCombatBattleStartAck(int msg_id, uint64_t guid, uint32_t uReserved)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCombatBattleStartAck> off_set = CreateMsgCombatBattleStartAck(builder ,uReserved);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgLoadingProcessReport(int msg_id, uint64_t guid, uint32_t loading_process)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgLoadingProcessReport> off_set = CreateMsgLoadingProcessReport(builder ,loading_process);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgLoadingProcessNotify(int msg_id, uint64_t guid, uint64_t account_id, uint32_t loading_process)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgLoadingProcessNotify> off_set = CreateMsgLoadingProcessNotify(builder ,account_id, loading_process);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

