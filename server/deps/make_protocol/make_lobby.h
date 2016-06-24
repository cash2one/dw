#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "lobby_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeMsgPing(int msg_id, uint64_t guid, uint64_t client_start_time, uint64_t gate_recv_time, uint64_t server_recv_time);

InterPacket * makeMsgLobbyKeepAliveReq(int msg_id, uint64_t guid, uint32_t uReserved);

InterPacket * makeMsgLobbyJoinGameReq(int msg_id, uint64_t guid, const string & sUserName, uint32_t uHeroSkinId);

InterPacket * makeMsgLobbyJoinGameAck(int msg_id, uint64_t guid, uint8_t bytIsSuccess, uint64_t uPlayerUid, uint8_t bytCamp);

InterPacket * makeMsgGamePlayerInfo(int msg_id, uint64_t guid, uint64_t uPlayerUid, uint32_t uHeroSkinId, uint8_t bytCamp);

InterPacket * makeMsgCombatPlayerReadyReq(int msg_id, uint64_t guid, uint64_t uPlayerUid);

InterPacket * makeMsgCombatPlayerReadyAck(int msg_id, uint64_t guid, uint8_t bytIsAlreadyRunning);

InterPacket * makeMsgCombatBattleStartAck(int msg_id, uint64_t guid, uint32_t uReserved);

InterPacket * makeMsgLoadingProcessReport(int msg_id, uint64_t guid, uint32_t loading_process);

InterPacket * makeMsgLoadingProcessNotify(int msg_id, uint64_t guid, uint64_t account_id, uint32_t loading_process);


