#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "framecmd_c2s_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeMsgBody(int msg_id, uint64_t guid, const std::vector<unknown> & data);

InterPacket * makeIntVec3(int msg_id, uint64_t guid, int x, int y, int z);

InterPacket * makeMsgCommandUseObjectiveSkill(int msg_id, uint64_t guid, uint32_t uObjectId, int8_t chSlotType, uint32_t uSkillId);

InterPacket * makeMsgFrameCommandC2S(int msg_id, uint64_t guid, uint64_t uObjectId, uint8_t bytSyncType, const std::vector<unknown> & aData);


