#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "framesync_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeMsgBody(int msg_id, uint64_t guid, const std::vector<unknown> & data);

InterPacket * makeIntVec3(int msg_id, uint64_t guid, int x, int y, int z);

InterPacket * makeMsgFrameSyncAction(int msg_id, uint64_t guid, uint8_t bytType, const std::vector<unknown> & aData);

InterPacket * makeMsgRepairFrameReq(int msg_id, uint64_t guid, const std::vector<unknown> & aFrames);

InterPacket * makeMsgReconnectionReq(int msg_id, uint64_t guid, uint8_t bytIsNew);

InterPacket * makeMsgReconnectionAck(int msg_id, uint64_t guid, uint8_t bytIsEOF, const std::vector<unknown> & oFrameData);

InterPacket * makePBattleEndReportPlayer(int msg_id, uint64_t guid, uint64_t account_id, uint16_t kill_num, uint16_t death_num, uint16_t assist_num);

InterPacket * makeMsgBeforeBattleParamNty(int msg_id, uint64_t guid, uint8_t select_hero_timeout, uint8_t buy_skin_timeout);

InterPacket * makeMsgBattleParamNty(int msg_id, uint64_t guid, uint8_t frame_delta, uint8_t pre_act_frames, uint32_t random_seed);

InterPacket * makeMsgBattleCountDownNty(int msg_id, uint64_t guid, uint8_t count_down_time);


