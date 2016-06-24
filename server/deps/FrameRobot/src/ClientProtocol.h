
#ifndef CLIENT_PROTOCOL_HPP__
#define CLIENT_PROTOCOL_HPP__

#include <string>
#include "common_generated.h"
#include "framecmd_c2c_generated.h"
#include "framecmd_c2s_generated.h"
#include "framesync_generated.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "Packet_.h"
#include "login_generated.h"
#include "match_generated.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;
using namespace flatbuffers;

class ClientProtocol
{
public:
	static ClientPacket * makeMsgChooseServerReq(int area_id);

	static ClientPacket * makeMsgLobbyJoinGameReq(const string & account, int hero_skill_id);

	static ClientPacket * makeMsgCombatPlayerReadyReq(uint64_t player_id);

	static ClientPacket * makeMsgClientFrameSyncReqMove(int player_id, int x, int y, int z);

	static void makeC2cMoveBuffer(int player_id, int x, int y, int z, string & output_buffer);

	static ClientPacket * makeClientPacket(int msg_id, uint8_t * buffer, int buf_size);

	// vrpublic
	static ClientPacket * makeMsgLoginReq(const string & account_name);

	static ClientPacket * makeMsgLoginReq(SessionId session_id, const string & user_name);

	static ClientPacket * makeMsgChooseServerReq(SessionId session_id, int server_id);

	// vr
	static ClientPacket * makeMsgVRLoginReq(SessionId session_id, const string & user_name, uint64_t user_id, const string & session_key);

	static ClientPacket * makeMsgEnterBattle(SessionId session_id, uint64_t user_id);

	static ClientPacket * makeMsgReEnterBattleReq(SessionId session_id, uint64_t user_id);

	static ClientPacket * makeMsgReqSummonerInfoReq(SessionId session_id);

	static ClientPacket* makeDemoSystemTestReq(SessionId id);

	static ClientPacket * makeMsgCreateSummonerReq(SessionId session_id, const string & account_name, const string & summoner_name);

	static ClientPacket * makeMsgMatchReq(SessionId session_id, int team_type, int match_type);

	static ClientPacket * makeMsgSearchFriendReq(SessionId session_id, string friend_name);
	static ClientPacket * makeMsgAddFriendReq(SessionId session_id, uint64_t friend_id);
	static ClientPacket * makeMsgDelFriendReq(SessionId session_id, uint64_t friend_id);
	static ClientPacket * makeMsgConnfirmAddFriendReq(SessionId session_id, uint64_t friend_id);
	static ClientPacket * makeMsgAddCancelFriendReq(SessionId session_id, uint64_t friend_id);
	static ClientPacket * makeMsgFriendListReq(SessionId session_id);
	static ClientPacket * makeMsgApplyForFriendListReq(SessionId session_id);
	static ClientPacket * makeMsgFriendDetailInfoReq(SessionId session_id, uint64_t friend_id);

	static ClientPacket * makeMsgSelectHeroReq(SessionId session_id, int hero_id);

	static ClientPacket * makeMsgConfirmHeroReq(SessionId session_id, int hero_id);

	// battle
	static ClientPacket * makeMsgReportBattleResultReq();
};

#endif
