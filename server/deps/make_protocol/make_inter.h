#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "inter_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeSMsgGateRegister(int msg_id, uint64_t guid, const string & ip_addr, int port);

InterPacket * makeSMsgVrRegisterVrpublic(int msg_id, uint64_t guid, int area_id, int server_id, const string & gate_ip_addr, int gate_port);

InterPacket * makeSMsgVrpublicNotifyVrLoginInfo(int msg_id, uint64_t guid, const string & user_name, uint64_t user_guid, const string & login_key, uint32_t channel_type, const string & client_version, uint8_t is_play, uint32_t language, const string & account_name, const string & device_name, const string & password, uint32_t platform_id, uint32_t gs_id, const string & gate_ip, uint32_t gate_port, const string & announcement);

InterPacket * makeSMsgVrpublicNotifyVrLoginInfoAck(int msg_id, uint64_t guid, uint32_t gs_id, const string & gate_ip, uint32_t gate_port, const string & session_code, const string & announcement);

InterPacket * makeSMsgVRNotifyCreateSummonerToVRF(int msg_id, uint64_t guid, uint64_t sum_id, const string & sum_name, uint32_t server_id);

InterPacket * makeSMsgVrRegisterTr(int msg_id, uint64_t guid, int area_id, int server_id);

InterPacket * makeSMsgVrRegisterVRFriend(int msg_id, uint64_t guid, uint32_t area_id, uint32_t server_id);

InterPacket * makeSMsgBattleRegisterTr(int msg_id, uint64_t guid, int battle_server_id, const string & battle_gate_ip, int battle_gate_port);

InterPacket * makeSMsgMatchReq(int msg_id, uint64_t guid, uint8_t team_type, uint8_t match_type, const std::vector<unknown> & user_list);

InterPacket * makePPlayerInfo(int msg_id, uint64_t guid, uint64_t user_id);

InterPacket * makeSMsgNotifyBattleMatchResultAck(int msg_id, uint64_t guid, uint64_t match_id, uint32_t result);

InterPacket * makePPlayerBattleResult(int msg_id, uint64_t guid, uint64_t account_id, int team_id, int kill_value, int death_value, int assist_value);

InterPacket * makeSMsgVRAddFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

InterPacket * makeSMsgVRDelFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

InterPacket * makeSMsgVRConfirmAddFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

InterPacket * makeSMsgVRAddCancelFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

InterPacket * makeSMsgVRNotifyAddFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

InterPacket * makeSMsgVRNotifyDelFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

InterPacket * makeSMsgVRNotifyConfirmAddFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

InterPacket * makeSMsgVRNotifyAddCancelFriendReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

InterPacket * makeSMsgVRFNotifyFriendOperateReq(int msg_id, uint64_t guid, uint64_t friend_id, uint8_t operate_type, uint64_t summoner_id);

InterPacket * makeSMsgVRFNotifyFriendOperateAck(int msg_id, uint64_t guid, uint64_t friend_id, uint8_t operate_type, uint64_t summoner_id);

InterPacket * makeSMsgVRNotifySearchFriend(int msg_id, uint64_t guid, uint64_t summoner_id, const string & friend_name);

InterPacket * makeSMsgNotifySearchFriendAck(int msg_id, uint64_t guid, uint64_t friend_id, uint32_t friend_head_id);

InterPacket * makeFriendDetail(int msg_id, uint64_t guid, uint64_t friend_id, uint32_t head_id, uint8_t status);

InterPacket * makeSMsgFriendListReq(int msg_id, uint64_t guid, uint64_t summoner_id);

InterPacket * makeSMsgApplyForFriendListReq(int msg_id, uint64_t guid, uint64_t summoner_id);

InterPacket * makeSMsgVRFriendDetailInfoReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

InterPacket * makeSMsgFriendDetailInfoReq(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id);

InterPacket * makeSMsgFriendDetailInfoAck(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id, uint32_t friend_head_id);

InterPacket * makeSMsgFriendStatusReq(int msg_id, uint64_t guid, uint64_t summoner_id);

InterPacket * makeSMsgNotifySummonerStatusToFriend(int msg_id, uint64_t guid, uint64_t summoner_id, uint64_t friend_id, uint8_t summoner_status);

InterPacket * makeSMsgFriendStatus(int msg_id, uint64_t guid, uint64_t friend_id, int8_t friend_status);


