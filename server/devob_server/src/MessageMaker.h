

#ifndef MESSAGE_MAKER_HPP__
#define MESSAGE_MAKER_HPP__

#include <string>
#include "Packet_.h"
#include "opcode.h"
#include "inter_generated.h"
#include "match_generated.h"
#include "protocol_generated.h"
#include "MatchCell.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;
using namespace flatbuffers;

class MessageMaker
{
public:
	static InterPacket * makeMsgMatchEnterBattle(MyGuid_t user_id, SessionId session_id, const string & server_ip, int server_port);

	static InterPacket * makeSMsgNotifyBattleMatchResult(SessionId session_id, MatchCellVec_t & match_cell_vec);

	static InterPacket * makeMsgMatchAck(SessionId session_id, uint64_t account_id, int result);
};

#endif
