#include "make_framecmd_c2c.h"

InterPacket * makeMsgBody(int msg_id, uint64_t guid, const std::vector<unknown> & data)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data_off = builder.CreateVector(data);
	flatbuffers::Offset<MsgBody> off_set = CreateMsgBody(builder ,data_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeIntVec3(int msg_id, uint64_t guid, int x, int y, int z)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<IntVec3> off_set = CreateIntVec3(builder ,x, y, z);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandChangeTargetingMode(int msg_id, uint64_t guid, uint8_t mode)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandChangeTargetingMode> off_set = CreateMsgCommandChangeTargetingMode(builder ,mode);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandPlayerMoveDirection(int msg_id, uint64_t guid, int16_t sDegree, uint8_t bytMoveSeq)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandPlayerMoveDirection> off_set = CreateMsgCommandPlayerMoveDirection(builder ,sDegree, bytMoveSeq);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandStopMove(int msg_id, uint64_t guid, int8_t chReserve)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandStopMove> off_set = CreateMsgCommandStopMove(builder ,chReserve);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandAttackActor(int msg_id, uint64_t guid, uint32_t uObjectId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandAttackActor> off_set = CreateMsgCommandAttackActor(builder ,uObjectId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandLearnSkill(int msg_id, uint64_t guid, uint32_t uHeroId, uint8_t bytSlotType, uint8_t bytSkillLevel)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandLearnSkill> off_set = CreateMsgCommandLearnSkill(builder ,uHeroId, bytSlotType, bytSkillLevel);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandUseCurveTrackSkill(int msg_id, uint64_t guid, int16_t sNoUse, int8_t chSlotType, uint32_t uSkillId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandUseCurveTrackSkill> off_set = CreateMsgCommandUseCurveTrackSkill(builder ,sNoUse, chSlotType, uSkillId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandUseCommonAttack(int msg_id, uint64_t guid, int8_t chStart, uint32_t uObjectId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandUseCommonAttack> off_set = CreateMsgCommandUseCommonAttack(builder ,chStart, uObjectId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandSwitchAI(int msg_id, uint64_t guid, int8_t chIsAutoAI)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandSwitchAI> off_set = CreateMsgCommandSwitchAI(builder ,chIsAutoAI);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandSwitchCaptain(int msg_id, uint64_t guid, uint32_t uObjectId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandSwitchCaptain> off_set = CreateMsgCommandSwitchCaptain(builder ,uObjectId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandSwitchSuperKiller(int msg_id, uint64_t guid, int8_t chIsSuperKiller)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandSwitchSuperKiller> off_set = CreateMsgCommandSwitchSuperKiller(builder ,chIsSuperKiller);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandSwitchGodMode(int msg_id, uint64_t guid, int8_t chIsGodMode)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandSwitchGodMode> off_set = CreateMsgCommandSwitchGodMode(builder ,chIsGodMode);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandLearnTalent(int msg_id, uint64_t guid, uint32_t uHeroId, int8_t chTalentLevel, uint32_t uTalentId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandLearnTalent> off_set = CreateMsgCommandLearnTalent(builder ,uHeroId, chTalentLevel, uTalentId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandTestCmdDelay(int msg_id, uint64_t guid, uint32_t uDelayMs)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandTestCmdDelay> off_set = CreateMsgCommandTestCmdDelay(builder ,uDelayMs);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandPlayerRunAway(int msg_id, uint64_t guid, int8_t chReserve)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandPlayerRunAway> off_set = CreateMsgCommandPlayerRunAway(builder ,chReserve);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandPlayerDisconnect(int msg_id, uint64_t guid, int8_t chReserve)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandPlayerDisconnect> off_set = CreateMsgCommandPlayerDisconnect(builder ,chReserve);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandPlayerReconnect(int msg_id, uint64_t guid, int8_t chReserve)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandPlayerReconnect> off_set = CreateMsgCommandPlayerReconnect(builder ,chReserve);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandPlayerAttackTargetMode(int msg_id, uint64_t guid, int8_t chAttackTargetMode)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandPlayerAttackTargetMode> off_set = CreateMsgCommandPlayerAttackTargetMode(builder ,chAttackTargetMode);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandSvrNTFCheckFrameLater(int msg_id, uint64_t guid, uint8_t bytFrameLaterNum)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandSvrNTFCheckFrameLater> off_set = CreateMsgCommandSvrNTFCheckFrameLater(builder ,bytFrameLaterNum);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandAssistStateChange(int msg_id, uint64_t guid, int8_t bytChgType, uint32_t uAiPlayerObjID, uint32_t uMasterObjectID)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandAssistStateChange> off_set = CreateMsgCommandAssistStateChange(builder ,bytChgType, uAiPlayerObjID, uMasterObjectID);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandChangeAutoAI(int msg_id, uint64_t guid, uint8_t bytAutoAIType, uint32_t uPlayerObjectId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandChangeAutoAI> off_set = CreateMsgCommandChangeAutoAI(builder ,bytAutoAIType, uPlayerObjectId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandPlayerBuyEquip(int msg_id, uint64_t guid, uint16_t usEquipId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandPlayerBuyEquip> off_set = CreateMsgCommandPlayerBuyEquip(builder ,usEquipId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandPlayerSellEquip(int msg_id, uint64_t guid, uint8_t bytEquipIndex)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandPlayerSellEquip> off_set = CreateMsgCommandPlayerSellEquip(builder ,bytEquipIndex);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandPlayerAddGoldCoinInBattle(int msg_id, uint64_t guid, uint32_t uAddedValue)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandPlayerAddGoldCoinInBattle> off_set = CreateMsgCommandPlayerAddGoldCoinInBattle(builder ,uAddedValue);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandSetSkillLevel(int msg_id, uint64_t guid, uint8_t bytSkillSlot, uint8_t bytSkillLevel)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandSetSkillLevel> off_set = CreateMsgCommandSetSkillLevel(builder ,bytSkillSlot, bytSkillLevel);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandPlayerCommonAttackMode(int msg_id, uint64_t guid, uint8_t bytCommonAttackMode)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandPlayerCommonAttackMode> off_set = CreateMsgCommandPlayerCommonAttackMode(builder ,bytCommonAttackMode);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandLockAttackTarget(int msg_id, uint64_t guid, uint32_t uLockedTargetId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandLockAttackTarget> off_set = CreateMsgCommandLockAttackTarget(builder ,uLockedTargetId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgCommandSignalMiniMapTarget(int msg_id, uint64_t guid, uint8_t bytSignalId, uint8_t bytType, uint32_t uTargetObjectId)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<MsgCommandSignalMiniMapTarget> off_set = CreateMsgCommandSignalMiniMapTarget(builder ,bytSignalId, bytType, uTargetObjectId);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeMsgFrameCommandC2C(int msg_id, uint64_t guid, uint64_t uObjectId, uint8_t bytCmdType, const std::vector<unknown> & aData)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::Vector<uint8_t>> aData_off = builder.CreateVector(aData);
	flatbuffers::Offset<MsgFrameCommandC2C> off_set = CreateMsgFrameCommandC2C(builder ,uObjectId, bytCmdType, aData_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

