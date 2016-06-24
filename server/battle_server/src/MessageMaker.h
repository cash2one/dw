
#ifndef MESSAGE_MAKER_HPP__
#define MESSAGE_MAKER_HPP__

#include "inter_generated.h"
#include "battle_generated.h"
#include "match_generated.h"
#include "lobby_generated.h"
#include "login_generated.h"
#include "protocol_generated.h"
#include "Packet_.h"
#include "SessionHandle.h"
#include "Battle.h"
#include "BattleSetting.h"

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;
using namespace flatbuffers;

class MessageMaker
{
public:
	static InterPacket * makeMsgEnterBattleAck(SessionId session_id, uint64_t guid, PlayerMap_t mapPlayer);

	static InterPacket * makeMsgReEnterBattleAck(SessionId session_id, uint64_t guid, int result);

	static InterPacket * makeMsgSelectHeroAck(SessionId session_id, uint64_t guid, int hero_id);

	static InterPacket * makeMsgConfirmHeroAck(SessionId session_id, uint64_t guid, int hero_id);

	static InterPacket * makeMsgBeforeBattleParamNty(uint64_t guid, const BattleSetting & battle_setting);

	static InterPacket * makeMsgBattleParamNty(uint64_t guid, const BattleSetting & battle_setting, int random_seed);

	static InterPacket * makeMsgReportBattleResultAck(uint64_t guid);

	static InterPacket * makeMsgBattleEndNty(uint64_t guid);

	static InterPacket * makeSMsgNotifyBattleMatchResultAck(uint64_t match_battle_id, int result);
};

#endif
