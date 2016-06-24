#include "make_login.h"

InterPacket * makeMsgLoginReq(int msg_id, uint64_t guid, uint32_t channel_type, const string & client_version, uint8_t is_play, uint32_t language, const string & account_name, const string & device_name, const string & password, uint32_t platform_id, const string & session_code)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> client_version_off = builder.CreateString(client_version);
	flatbuffers::Offset<flatbuffers::String> account_name_off = builder.CreateString(account_name);
	flatbuffers::Offset<flatbuffers::String> device_name_off = builder.CreateString(device_name);
	flatbuffers::Offset<flatbuffers::String> password_off = builder.CreateString(password);
	flatbuffers::Offset<flatbuffers::String> session_code_off = builder.CreateString(session_code);
	flatbuffers::Offset<MsgLoginReq> off_set = CreateMsgLoginReq(builder ,channel_type, client_version_off, is_play, language, account_name_off, device_name_off, password_off, platform_id, session_code_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeServerInfo_t(int msg_id, uint64_t guid, uint32_t server_id, const string & server_name, int8_t is_logined, uint32_t area_index, int8_t stauts)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> server_name_off = builder.CreateString(server_name);
	flatbuffers::Offset<ServerInfo_t> off_set = CreateServerInfo_t(builder ,server_id, server_name_off, is_logined, area_index, stauts);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgChooseServerReq(int msg_id, uint64_t guid, uint32_t server_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgChooseServerReq> off_set = CreateMsgChooseServerReq(builder ,server_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgChooseServerAck(int msg_id, uint64_t guid, uint64_t user_id, const string & ip, uint32_t port, const string & session_key, const string & announcement)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> ip_off = builder.CreateString(ip);
	flatbuffers::Offset<flatbuffers::String> session_key_off = builder.CreateString(session_key);
	flatbuffers::Offset<flatbuffers::String> announcement_off = builder.CreateString(announcement);
	flatbuffers::Offset<MsgChooseServerAck> off_set = CreateMsgChooseServerAck(builder ,user_id, ip_off, port, session_key_off, announcement_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgVRLoginReq(int msg_id, uint64_t guid, const string & session_key, uint64_t account_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> session_key_off = builder.CreateString(session_key);
	flatbuffers::Offset<MsgVRLoginReq> off_set = CreateMsgVRLoginReq(builder ,session_key_off, account_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgVRLoginAck(int msg_id, uint64_t guid, int8_t is_in_battle)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgVRLoginAck> off_set = CreateMsgVRLoginAck(builder ,is_in_battle);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgReqSummonerInfoAck(int msg_id, uint64_t guid, const string & summoner_name)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> summoner_name_off = builder.CreateString(summoner_name);
	flatbuffers::Offset<MsgReqSummonerInfoAck> off_set = CreateMsgReqSummonerInfoAck(builder ,summoner_name_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCreateSummonerReq(int msg_id, uint64_t guid, const string & summoner_name, int8_t head_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> summoner_name_off = builder.CreateString(summoner_name);
	flatbuffers::Offset<MsgCreateSummonerReq> off_set = CreateMsgCreateSummonerReq(builder ,summoner_name_off, head_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

