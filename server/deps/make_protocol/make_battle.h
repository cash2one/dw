#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "battle_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeMsgEnterBattleReq(int msg_id, uint64_t guid, uint64_t user_id, const string & session_key);

InterPacket * makeBattlePlayerInfo(int msg_id, uint64_t guid, uint64_t player_id, const string & player_name);

InterPacket * makeMsgReEnterBattleReq(int msg_id, uint64_t guid, uint64_t user_id, const string & session_key);

InterPacket * makeMsgReEnterBattleAck(int msg_id, uint64_t guid, int8_t result);

InterPacket * makeMsgSelectHeroReq(int msg_id, uint64_t guid, uint32_t hero_id);

InterPacket * makeMsgSelectHeroAck(int msg_id, uint64_t guid, uint64_t player_id, uint32_t hero_id);

InterPacket * makeMsgConfirmHeroReq(int msg_id, uint64_t guid, uint32_t hero_id);

InterPacket * makeMsgConfirmHeroAck(int msg_id, uint64_t guid, uint64_t player_id, uint32_t hero_id);


