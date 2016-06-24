#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "match_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeMsgMatchReq(int msg_id, uint64_t guid, uint8_t team_type, uint8_t match_type);

InterPacket * makeMsgMatchAck(int msg_id, uint64_t guid, uint8_t match_result);

InterPacket * makeMsgCancelMatchAck(int msg_id, uint64_t guid, uint8_t result);

InterPacket * makeMsgMatchEnterBattle(int msg_id, uint64_t guid, const string & server_ip, int server_port, const string & session_key);

InterPacket * makeMsgReconnectBattle(int msg_id, uint64_t guid, const string & server_ip, int server_port, const string & session_key);


