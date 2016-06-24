#include "make_inter.h"

InterPacket * makeSMsgGateRegister(int msg_id, uint64_t guid, const string & ip_addr, int port)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> ip_addr_off = builder.CreateString(ip_addr);
	flatbuffers::Offset<SMsgGateRegister> off_set = CreateSMsgGateRegister(builder ,ip_addr_off, port);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVrRegisterVrpublic(int msg_id, uint64_t guid, int area_id, int server_id, const string & gate_ip_addr, int gate_port)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> gate_ip_addr_off = builder.CreateString(gate_ip_addr);
	flatbuffers::Offset<SMsgVrRegisterVrpublic> off_set = CreateSMsgVrRegisterVrpublic(builder ,area_id, server_id, gate_ip_addr_off, gate_port);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVrpublicNotifyVrLoginInfo(int msg_id, uint64_t guid, const string & user_name, uint64_t user_guid, const string & login_key, uint32_t channel_type, const string & client_version, uint8_t is_play, uint32_t language, const string & account_name, const string & device_name, const string & password, uint32_t platform_id, uint32_t gs_id, const string & gate_ip, uint32_t gate_port, const string & announcement)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> user_name_off = builder.CreateString(user_name);
	flatbuffers::Offset<flatbuffers::String> login_key_off = builder.CreateString(login_key);
	flatbuffers::Offset<flatbuffers::String> client_version_off = builder.CreateString(client_version);
	flatbuffers::Offset<flatbuffers::String> account_name_off = builder.CreateString(account_name);
	flatbuffers::Offset<flatbuffers::String> device_name_off = builder.CreateString(device_name);
	flatbuffers::Offset<flatbuffers::String> password_off = builder.CreateString(password);
	flatbuffers::Offset<flatbuffers::String> gate_ip_off = builder.CreateString(gate_ip);
	flatbuffers::Offset<flatbuffers::String> announcement_off = builder.CreateString(announcement);
	flatbuffers::Offset<SMsgVrpublicNotifyVrLoginInfo> off_set = CreateSMsgVrpublicNotifyVrLoginInfo(builder ,user_name_off, user_guid, login_key_off, channel_type, client_version_off, is_play, language, account_name_off, device_name_off, password_off, platform_id, gs_id, gate_ip_off, gate_port, announcement_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVrpublicNotifyVrLoginInfoAck(int msg_id, uint64_t guid, uint32_t gs_id, const string & gate_ip, uint32_t gate_port, const string & session_code, const string & announcement)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> gate_ip_off = builder.CreateString(gate_ip);
	flatbuffers::Offset<flatbuffers::String> session_code_off = builder.CreateString(session_code);
	flatbuffers::Offset<flatbuffers::String> announcement_off = builder.CreateString(announcement);
	flatbuffers::Offset<SMsgVrpublicNotifyVrLoginInfoAck> off_set = CreateSMsgVrpublicNotifyVrLoginInfoAck(builder ,gs_id, gate_ip_off, gate_port, session_code_off, announcement_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRNotifyCreateSummonerToVRF(int msg_id, uint64_t guid, uint64_t sum_id, const string & sum_name, uint32_t server_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> sum_name_off = builder.CreateString(sum_name);
	flatbuffers::Offset<SMsgVRNotifyCreateSummonerToVRF> off_set = CreateSMsgVRNotifyCreateSummonerToVRF(builder ,sum_id, sum_name_off, server_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVrRegisterTr(int msg_id, uint64_t guid, int area_id, int server_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVrRegisterTr> off_set = CreateSMsgVrRegisterTr(builder ,area_id, server_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVrRegisterVRFriend(int msg_id, uint64_t guid, uint32_t area_id, uint32_t server_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVrRegisterVRFriend> off_set = CreateSMsgVrRegisterVRFriend(builder ,area_id, server_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgBattleRegisterTr(int msg_id, uint64_t guid, int battle_server_id, const string & battle_gate_ip, int battle_gate_port)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> battle_gate_ip_off = builder.CreateString(battle_gate_ip);
	flatbuffers::Offset<SMsgBattleRegisterTr> off_set = CreateSMsgBattleRegisterTr(builder ,battle_server_id, battle_gate_ip_off, battle_gate_port);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgMatchReq(int msg_id, uint64_t guid, uint8_t team_type, uint8_t match_type, const std::vector<unknown> & user_list)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::Vector<uint64_t>> user_list_off = builder.CreateVector(user_list);
	flatbuffers::Offset<SMsgMatchReq> off_set = CreateSMsgMatchReq(builder ,team_type, match_type, user_list_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makePPlayerInfo(int msg_id, uint64_t guid, uint64_t user_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<PPlayerInfo> off_set = CreatePPlayerInfo(builder ,user_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgNotifyBattleMatchResultAck(int msg_id, uint64_t guid, uint64_t match_id, uint32_t result)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgNotifyBattleMatchResultAck> off_set = CreateSMsgNotifyBattleMatchResultAck(builder ,match_id, result);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makePPlayerBattleResult(int msg_id, uint64_t guid, uint64_t account_id, int team_id, int kill_value, int death_value, int assist_value)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<PPlayerBattleResult> off_set = CreatePPlayerBattleResult(builder ,account_id, team_id, kill_value, death_value, assist_value);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRAddFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRAddFriendReq> off_set = CreateSMsgVRAddFriendReq(builder ,summoner_id, friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRDelFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRDelFriendReq> off_set = CreateSMsgVRDelFriendReq(builder ,summoner_id, friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRConfirmAddFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRConfirmAddFriendReq> off_set = CreateSMsgVRConfirmAddFriendReq(builder ,summoner_id, friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRAddCancelFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRAddCancelFriendReq> off_set = CreateSMsgVRAddCancelFriendReq(builder ,summoner_id, friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRNotifyAddFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRNotifyAddFriendReq> off_set = CreateSMsgVRNotifyAddFriendReq(builder ,summoner_id, friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRNotifyDelFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRNotifyDelFriendReq> off_set = CreateSMsgVRNotifyDelFriendReq(builder ,summoner_id, friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRNotifyConfirmAddFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRNotifyConfirmAddFriendReq> off_set = CreateSMsgVRNotifyConfirmAddFriendReq(builder ,summoner_id, friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRNotifyAddCancelFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRNotifyAddCancelFriendReq> off_set = CreateSMsgVRNotifyAddCancelFriendReq(builder ,summoner_id, friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRFNotifyFriendOperateReq(int msg_id, uint64_t guid, uint64_t friend_id, uint8_t operate_type, uint64_t summoner_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRFNotifyFriendOperateReq> off_set = CreateSMsgVRFNotifyFriendOperateReq(builder ,friend_id, operate_type, summoner_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRFNotifyFriendOperateAck(int msg_id, uint64_t guid, uint64_t friend_id, uint8_t operate_type, uint64_t summoner_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRFNotifyFriendOperateAck> off_set = CreateSMsgVRFNotifyFriendOperateAck(builder ,friend_id, operate_type, summoner_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRNotifySearchFriend(int msg_id, uint64_t guid, uint64_t summoner_id, const string & friend_name)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> friend_name_off = builder.CreateString(friend_name);
	flatbuffers::Offset<SMsgVRNotifySearchFriend> off_set = CreateSMsgVRNotifySearchFriend(builder ,summoner_id, friend_name_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgNotifySearchFriendAck(int msg_id, uint64_t guid, uint64_t friend_id, uint32_t friend_head_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgNotifySearchFriendAck> off_set = CreateSMsgNotifySearchFriendAck(builder ,friend_id, friend_head_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeFriendDetail(int msg_id, uint64_t guid, uint64_t friend_id, uint32_t head_id, uint8_t status)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<FriendDetail> off_set = CreateFriendDetail(builder ,friend_id, head_id, status);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgFriendListReq(int msg_id, uint64_t guid, uint64_t summoner_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgFriendListReq> off_set = CreateSMsgFriendListReq(builder ,summoner_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgApplyForFriendListReq(int msg_id, uint64_t guid, uint64_t summoner_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgApplyForFriendListReq> off_set = CreateSMsgApplyForFriendListReq(builder ,summoner_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgVRFriendDetailInfoReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgVRFriendDetailInfoReq> off_set = CreateSMsgVRFriendDetailInfoReq(builder ,summoner_id, friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgFriendDetailInfoReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgFriendDetailInfoReq> off_set = CreateSMsgFriendDetailInfoReq(builder ,summoner_id, friend_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgFriendDetailInfoAck(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id, uint32_t friend_head_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgFriendDetailInfoAck> off_set = CreateSMsgFriendDetailInfoAck(builder ,summoner_id, friend_id, friend_head_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgFriendStatusReq(int msg_id, uint64_t guid, uint64_t summoner_id)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgFriendStatusReq> off_set = CreateSMsgFriendStatusReq(builder ,summoner_id);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgNotifySummonerStatusToFriend(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id, uint8_t summoner_status)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgNotifySummonerStatusToFriend> off_set = CreateSMsgNotifySummonerStatusToFriend(builder ,summoner_id, friend_id, summoner_status);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeSMsgFriendStatus(int msg_id, uint64_t guid, uint64_t friend_id, int8_t friend_status)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<SMsgFriendStatus> off_set = CreateSMsgFriendStatus(builder ,friend_id, friend_status);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

