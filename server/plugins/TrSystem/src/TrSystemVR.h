
#ifndef TrSystemVR_hpp__
#define TrSystemVR_hpp__

#include <unordered_map>
#include <iostream>
#include "typedef.h"
#include <flatbuffers/flatbuffers.h>
#include "inter_generated.h"
#include "plugin.h"
//#include "msg_demo.pb.h"
#include "robot_generated.h"
#include "protocol_generated.h"
#include "match_generated.h"
#include "opcode.h"
#include "EmptyMsg.h"
#include "pluginDepot.h"
#include "inter_proto.pb.h"
#include "account.pb.h"
#include "login_generated.h"

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;
using namespace flatbuffers;
using namespace std;

enum MatchStatus
{
	MS_base		= 0,
	MS_matching_req,
	MS_matching,
	MS_match_success,
	MS_match_confirmed,
	MS_in_battle,

	// MS_canceling_match,	 equal to MS_matching
};

class TrSystemVR : public Plugin
{
public:
	TrSystemVR();
	~TrSystemVR();
public:
	struct PlayerInfo 
	{
		PlayerInfo()
			: summoner_guid(0)
			, match_status(MS_base)
			, is_online(false)
			, battle_server_port(0)
		{}

		uint64_t summoner_guid;
		MatchStatus match_status;
		bool is_online;
		int battle_server_port;
		string battle_server_ip;
		string battle_session_key;
	};
public:
	BEGIN_INPUT_MSG_MAP()
		// client input msg
		INPUT_HANDLE_MSG(TrSystemVR, Opcode_E_MsgMatchReq, MsgMatchReq)
		INPUT_HANDLE_MSG(TrSystemVR, Opcode_E_MsgCancelMatchReq, EmptyMsg)

		// tr ack message
		INPUT_HANDLE_MSG(TrSystemVR, E_MsgMatchAck_Copy, MsgMatchAck)
		INPUT_HANDLE_MSG(TrSystemVR, E_MsgCancelMatchAck_Copy, MsgCancelMatchAck)
		INPUT_HANDLE_MSG(TrSystemVR, E_MsgMatchEnterBattle_Copy, MsgMatchEnterBattle)
		INPUT_HANDLE_MSG(TrSystemVR, E_SMsgBattleEndNotifyVr, SMsgBattleEndNotifyVr)
	END_INPUT_MSG_MAP()

	BEGING_NOTIFY_MSG_MAP()
		NOTIFY_HANDLE_MSG(TrSystemVR, N_CreateSummoner, EmptyMsg)
		NOTIFY_HANDLE_MSG(TrSystemVR, N_PlayerOnline, EmptyMsg)
		NOTIFY_HANDLE_MSG(TrSystemVR, N_PlayerOffline, EmptyMsg)
	END_NOTIFY_MSG_MAP()
public:
	// client input msg
	INPUT_FUN_DECLARE(TrSystemVR, Opcode_E_MsgMatchReq, MsgMatchReq);
	INPUT_FUN_DECLARE(TrSystemVR, Opcode_E_MsgCancelMatchReq, EmptyMsg);
		
	// tr ack msg
	INPUT_FUN_DECLARE(TrSystemVR, E_MsgMatchAck_Copy, MsgMatchAck);
	INPUT_FUN_DECLARE(TrSystemVR, E_MsgCancelMatchAck_Copy, MsgCancelMatchAck);
	INPUT_FUN_DECLARE(TrSystemVR, E_MsgMatchEnterBattle_Copy, MsgMatchEnterBattle);
	INPUT_FUN_DECLARE(TrSystemVR, E_SMsgBattleEndNotifyVr, SMsgBattleEndNotifyVr);

	// notify msg
	NOTIFY_FUN_DECLARE(TrSystemVR, N_CreateSummoner, EmptyMsg);
	NOTIFY_FUN_DECLARE(TrSystemVR, N_PlayerOnline, EmptyMsg);
	NOTIFY_FUN_DECLARE(TrSystemVR, N_PlayerOffline, EmptyMsg);

protected:
	InterPacket * makeSMsgMatchReq(MyGuid_t gate_guid, int team_type, int match_type, vector<MyGuid_t> & group_member);
	InterPacket * makeMsgReconnectBattle(MyGuid_t gate_guid, int battle_port, const string & battle_ip, const string & battle_session_key);
	InterPacket * makeMsgVRLoginAck(MyGuid_t gate_guid, int battle_status);
protected:
	uint64_t getSummonerGuidByGateGuid(uint64_t gate_guid);


	void updateSummonerToTr(Entity::summoner * summoner);

	PlayerInfo * getPlayerInfo(uint64_t summoner_guid);
	bool addPlayerInfo(PlayerInfo * player_info);

	void notifyClientReEnterBattle(PlayerInfo * player_info);

protected:
	void eventPlayerOnline(uint64_t summoner_guid);
	void eventPlayerOffline(uint64_t summoner_guid);

	InterPacket * makePlayerOfflineMsg(uint64_t summoner_guid);
private:
	typedef unordered_map<uint64_t, PlayerInfo *> PlayerInfoMap_t;

	PlayerInfoMap_t m_player_info_map;
};

#endif
