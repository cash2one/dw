#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "msg_error_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeMsgError(int msg_id, uint64_t guid, uint32_t error_code);


