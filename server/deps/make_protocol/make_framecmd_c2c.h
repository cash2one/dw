#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "framecmd_c2c_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeMsgBody(int msg_id, uint64_t guid, const std::vector<unknown> & data);

InterPacket * makeIntVec3(int msg_id, uint64_t guid, int x, int y, int z);

InterPacket * makeMsgCommandChangeTargetingMode(int msg_id, uint64_t guid, uint8_t mode);

InterPacket * makeMsgCommandPlayerMoveDirection(int msg_id, uint64_t guid, int16_t sDegree, uint8_t bytMoveSeq);

InterPacket * makeMsgCommandStopMove(int msg_id, uint64_t guid, int8_t chReserve);

InterPacket * makeMsgCommandAttackActor(int msg_id, uint64_t guid, uint32_t uObjectId);

InterPacket * makeMsgCommandLearnSkill(int msg_id, uint64_t guid, uint32_t uHeroId, uint8_t bytSlotType, uint8_t bytSkillLevel);

InterPacket * makeMsgCommandUseCurveTrackSkill(int msg_id, uint64_t guid, int16_t sNoUse, int8_t chSlotType, uint32_t uSkillId);

InterPacket * makeMsgCommandUseCommonAttack(int msg_id, uint64_t guid, int8_t chStart, uint32_t uObjectId);

InterPacket * makeMsgCommandSwitchAI(int msg_id, uint64_t guid, int8_t chIsAutoAI);

InterPacket * makeMsgCommandSwitchCaptain(int msg_id, uint64_t guid, uint32_t uObjectId);

InterPacket * makeMsgCommandSwitchSuperKiller(int msg_id, uint64_t guid, int8_t chIsSuperKiller);

InterPacket * makeMsgCommandSwitchGodMode(int msg_id, uint64_t guid, int8_t chIsGodMode);

InterPacket * makeMsgCommandLearnTalent(int msg_id, uint64_t guid, uint32_t uHeroId, int8_t chTalentLevel, uint32_t uTalentId);

InterPacket * makeMsgCommandTestCmdDelay(int msg_id, uint64_t guid, uint32_t uDelayMs);

InterPacket * makeMsgCommandPlayerRunAway(int msg_id, uint64_t guid, int8_t chReserve);

InterPacket * makeMsgCommandPlayerDisconnect(int msg_id, uint64_t guid, int8_t chReserve);

InterPacket * makeMsgCommandPlayerReconnect(int msg_id, uint64_t guid, int8_t chReserve);

InterPacket * makeMsgCommandPlayerAttackTargetMode(int msg_id, uint64_t guid, int8_t chAttackTargetMode);

InterPacket * makeMsgCommandSvrNTFCheckFrameLater(int msg_id, uint64_t guid, uint8_t bytFrameLaterNum);

InterPacket * makeMsgCommandAssistStateChange(int msg_id, uint64_t guid, int8_t bytChgType, uint32_t uAiPlayerObjID, uint32_t uMasterObjectID);

InterPacket * makeMsgCommandChangeAutoAI(int msg_id, uint64_t guid, uint8_t bytAutoAIType, uint32_t uPlayerObjectId);

InterPacket * makeMsgCommandPlayerBuyEquip(int msg_id, uint64_t guid, uint16_t usEquipId);

InterPacket * makeMsgCommandPlayerSellEquip(int msg_id, uint64_t guid, uint8_t bytEquipIndex);

InterPacket * makeMsgCommandPlayerAddGoldCoinInBattle(int msg_id, uint64_t guid, uint32_t uAddedValue);

InterPacket * makeMsgCommandSetSkillLevel(int msg_id, uint64_t guid, uint8_t bytSkillSlot, uint8_t bytSkillLevel);

InterPacket * makeMsgCommandPlayerCommonAttackMode(int msg_id, uint64_t guid, uint8_t bytCommonAttackMode);

InterPacket * makeMsgCommandLockAttackTarget(int msg_id, uint64_t guid, uint32_t uLockedTargetId);

InterPacket * makeMsgCommandSignalMiniMapTarget(int msg_id, uint64_t guid, uint8_t bytSignalId, uint8_t bytType, uint32_t uTargetObjectId);

InterPacket * makeMsgFrameCommandC2C(int msg_id, uint64_t guid, uint64_t uObjectId, uint8_t bytCmdType, const std::vector<unknown> & aData);


