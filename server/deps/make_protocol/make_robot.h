#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "robot_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeRequestRobotConfigAck(int msg_id, uint64_t guid, const string & ip_addr, int port, int start_id, int run_robot_num, uint8_t net_type);

InterPacket * makeDemoSystemTestReq(int msg_id, uint64_t guid, int you, const string & her);

InterPacket * makeDemoSystemTestAck(int msg_id, uint64_t guid, uint8_t me, const std::vector<unknown> & one);


