#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "login_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeMsgLoginReq(int msg_id, uint64_t guid, uint32_t channel_type, const string & client_version, uint8_t is_play, uint32_t language, const string & account_name, const string & device_name, const string & password, uint32_t platform_id, const string & session_code);

InterPacket * makeServerInfo_t(int msg_id, uint64_t guid, uint32_t server_id, const string & server_name, int8_t is_logined, uint32_t area_index, int8_t stauts);

InterPacket * makeMsgChooseServerReq(int msg_id, uint64_t guid, uint32_t server_id);

InterPacket * makeMsgChooseServerAck(int msg_id, uint64_t guid, uint64_t user_id, const string & ip, uint32_t port, const string & session_key, const string & announcement);

InterPacket * makeMsgVRLoginReq(int msg_id, uint64_t guid, const string & session_key, uint64_t account_id);

InterPacket * makeMsgVRLoginAck(int msg_id, uint64_t guid, int8_t is_in_battle);

InterPacket * makeMsgReqSummonerInfoAck(int msg_id, uint64_t guid, const string & summoner_name);

InterPacket * makeMsgCreateSummonerReq(int msg_id, uint64_t guid, const string & summoner_name, int8_t head_id);


