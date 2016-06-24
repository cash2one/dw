#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "dev_ob_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeSMsgRegisterToDevOb(int msg_id, uint64_t guid, uint32_t gs_id);

InterPacket * makeSMsgObReportGateInfo(int msg_id, uint64_t guid, const string & gate_ip, int gate_port);

InterPacket * makePObPlayerInfo(int msg_id, uint64_t guid, uint64_t uPlayerUid, uint32_t uHeroSkinId, uint8_t bytCamp);

InterPacket * makeSMsgUpdateBattleFrame(int msg_id, uint64_t guid, uint32_t battle_id, uint32_t frame_no, const std::vector<unknown> & frame_stream);

InterPacket * makeSMsgUpdateBattleStatus(int msg_id, uint64_t guid, uint32_t battle_id, uint32_t status, uint32_t is_end);

InterPacket * makePObInfo(int msg_id, uint64_t guid, uint32_t ob_id, const string & start_time, uint32_t one_team_num, uint32_t status);

InterPacket * makeMsgSelectObReq(int msg_id, uint64_t guid, uint32_t ob_id);

InterPacket * makeMsgSelectObAck(int msg_id, uint64_t guid, const string & ob_ip_addr, int ob_port);

InterPacket * makeMsgEnterDevObReq(int msg_id, uint64_t guid, uint32_t ob_id);


