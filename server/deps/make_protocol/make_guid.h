#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "guid_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeSMsgRequestGuidReq(int msg_id, uint64_t guid, uint32_t request_number);

InterPacket * makeSMsgRequestGuidAck(int msg_id, uint64_t guid, uint32_t seed_index, uint64_t start_index, uint32_t create_number);


