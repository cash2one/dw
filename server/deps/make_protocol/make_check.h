#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "check_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeMsgClientValidation(int msg_id, uint64_t guid, uint32_t frame_no, const string & check_value);


