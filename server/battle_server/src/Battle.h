
#ifndef BATTLE_HPP__
#define BATTLE_HPP__

#include <unordered_map>
#include "common_generated.h"
#include "framecmd_c2c_generated.h"
#include "framecmd_c2s_generated.h"
#include "framesync_generated.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "typedef.h"
#include "Player.h"
#include "BattleTeam.h"
#include "Packet_.h"
#include "ManageClientValidation.h"
#include "MessageMaker.h"
#include "BattleResult.h"

using namespace std;

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;
using namespace flatbuffers;

class BattleManager;

enum BattleStatus
{
	BS_PREPARE			= 0,	// ban, pick, choose hero
	BS_COUNT_DOWN,				// all players confirm hero, count down, can buy skin, weapon
	BS_CLIENT_LOADING,
	BS_START_BATTLE,
};

struct BattleTimeoutInfo
{
	BattleTimeoutInfo()
		: battle_create_time(0)
		, all_player_confirmed_hero_time(0)
		, all_player_start_loading_time(0)
	{}

	time_t battle_create_time;
	time_t all_player_confirmed_hero_time;
	time_t all_player_start_loading_time;
};

struct BattleFrameInfo
{
	BattleFrameInfo()
		: last_frame_time(0)
		, frame_index(0)
	{}

	uint64 last_frame_time;
	uint32 frame_index;
};

struct PlayerFrameInfo
{
	PlayerFrameInfo()
		: player_action_bytes(NULL)
		, player_id(0)
		, action_type(0)
		, frame_index(0)
	{}
	PlayerFrameInfo(vector<uint8> * action_bytes, PlayerId_t playerid, int actiontype, uint32 frameindex)
		: player_action_bytes(action_bytes)
		, player_id(playerid)
		, action_type(actiontype)
		, frame_index(frameindex)
	{}

	~PlayerFrameInfo()
	{
		if (NULL != player_action_bytes)
		{
			delete player_action_bytes;
		}
	}

	vector<uint8> * player_action_bytes;
	PlayerId_t player_id;
	int action_type;
	uint32 frame_index;

};

struct FramePacketInfo
{
	FramePacketInfo()
		: frame_index(0)
		, packet(NULL)
	{}

	FramePacketInfo(uint32 frameindex, InterPacket * pack)
		: frame_index(frameindex)
		, packet(pack)
	{}

	uint32 frame_index;
	InterPacket * packet;
};

typedef vector<FramePacketInfo *> PacketFrameInfoVec_t;
typedef unordered_map<uint32, FramePacketInfo *> PacketFrameInfoMap_t;

class Battle
{
public:
	Battle();
	~Battle();
public:
	typedef vector<PlayerFrameInfo *> PlayerFrameInfoVec_t;

	int init(BattleManager * battle_manager, int one_team_number, bool del_frame_pack_for_press_test);

	void update(time_t current_time);

	void makeCurrentFrame(uint32 frame_index, time_t current_time);

	void broadcastPacket(InterPacket * packet, int team_id, bool all_team = false);

	void broadcastPacketFrame(InterPacket * packet);

	bool isTimeout(time_t current_time);

	bool isBattleEnd(time_t current_time);

	bool isMemberFull();

	void playerReconnect(Player * player, session_t session_id);

	void playerOffline(Player * player);

	void playerEnterBattle(Player * player);

	void playerReEnterBattle(Player * player);

	void playerSelectHero(Player * player, int hero_id);

	void playerConfirmHero(Player * player, int hero_id);
public:
	Player * addPlayer(session_t session_id, PlayerId_t player_id, uint64_t gate_guid, int hero_id, const string & player_name);

	void removePlayer(Player * player);

	void playerReadyForBattle(Player * player);

	void playerReadyForReconnection(Player * player);

	void playerReportBattleResult(Player * player, const MsgReportBattleResultReq * msg);

	// vr msg
	void vrReciveBattleReportNotifyBattle(Player * player);

	//Player * getPlayerBySession(session_t session_id);

	bool isBattleCreate();
	bool isBattleStart();

	PlayerMap_t & getPlayerMap();

	void changePlayerSession(Player * player, session_t session);

public:
	void processClientFramePack(Player * player, const MsgClientFrameSyncReq * cc_msg);

	void processClientToClientMsg(Player * player, const MsgFrameCommandC2C * cc_msg);

	void processClientToServerMsg(Player * player, const MsgFrameCommandC2S * cs_msg);

	void processMsgRepairFrameReq(Player * player, const MsgRepairFrameReq * msg);

	void processMsgReconnectionReq(Player * player, const MsgReconnectionReq * msg);

	void handleClientValidation(int msg_id, PlayerId_t player_id, uint32_t frame_no, const string & validation_value);

protected:
	void startBattle();

	void changePlayerStatus(BattlePlayerStatus bps_value);

	void changePlayerStatus(Player * player, BattlePlayerStatus bps_value);
	
	void notifyClientCountDown();

	void notifyClientLoading();

	void sendNotifyClientCountDown();

	void sendNotifyClientLoadingMsg();

	void notifyClientJoinGame(Player * player, PlayerId_t player_id, uint32 team_id, uint64_t gate_guid);

	void notifyClientEnterBattle();

	void notifyClientBattleEnd();

	void notifyTrBattleEnd();

	//void sendReqFrameToPlayer(session_t session_id, const Vector<uint32> & frame_req);

	FramePacketInfo * getFramePacketInfo(uint32 frame_index);

	void playerLoginWithLogout(Player * player);

	void playerLoginWithoutLogout(Player * player);

	void clearFrameAction(PlayerFrameInfoVec_t & frame_action_vec);

	bool isAllPlayerConfirmedHero();

	void updateInterPacketGuid(InterPacket * packet, uint64_t guid);

	void checkBeforeBattleTimeout();

	void timeoutOfSelectHero();

	// timeout of count down
	void timeoutOfBuySkin();

	void timeoutOfClientLoading();

protected:
	InterPacket * createMsgLobbyGameStartAck(int count_down);
	InterPacket * createMsgBattleCountDownNty(int count_down);
	InterPacket * createMsgLobbyJoinGameAck(int is_success, PlayerId_t player_id, uint64_t gate_guid, int team_id);
	InterPacket * createMsgCombatBattleStartAck(uint64_t gate_guid);
	InterPacket * createMsgFrameSyncPack(uint64_t gate_guid);
	InterPacket * createMsgReconnectionAck(uint32 start_frome_index, uint32 frame_count, bool is_reach_end, uint64_t gate_guid);
	InterPacket * createMsgRepairAck(const flatbuffers::Vector<uint32_t> & frame_index_vec, uint64_t gate_guid);
	InterPacket * createMsgCombatPlayerReadyAck(int battle_status, uint64_t gate_guid);

	InterPacket * makeSMsgBattleEndNotifyVr();

private:
	BattleManager * m_battle_manager;

	uint64 m_battle_start_time;

	PlayerMap_t m_player_map;

	PlayerSet_t m_offline_player_set;

	BattleStatus m_battle_status;

	time_t m_time_out_time;

	BattleTeam m_battle_team;

	PlayerFrameInfoVec_t m_current_frame_action;

	PlayerFrameInfoVec_t m_all_frame_action;

	BattleFrameInfo m_battle_frame_info;

	PacketFrameInfoMap_t m_frame_packet_info_map;

	bool m_del_frame_pack_for_press_test;

	int m_one_team_number;

	ManageClientValidation m_manage_client_validation;

	int m_random_seed;

	BattleTimeoutInfo m_battle_timeout_info;

	BattleResult m_battle_result;
};

#endif