#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "common_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeMsgBody(int msg_id, uint64_t guid, const std::vector<unknown> & data);

InterPacket * makeIntVec3(int msg_id, uint64_t guid, int x, int y, int z);


