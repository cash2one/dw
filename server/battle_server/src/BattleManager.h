
#ifndef BATTLE_MANAGER_HPP__
#define BATTLE_MANAGER_HPP__

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "PackMsg.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "Battle.h"
#include "framesync_generated.h"
#include "check_generated.h"
#include "EmptyMsg.h"
#include "typedef.h"
#include "PlayerManager.h"
#include "ManageStat.h"
#include "battle_generated.h"
#include "inter_generated.h"
#include "BattleSetting.h"

using namespace std;

using namespace MobaGo::FlatBuffer;

struct PlayerAccountInfo 
{
	PlayerAccountInfo()
		: player_id(0)
	{}

	PlayerAccountInfo(uint32 _player_id)
		: player_id(_player_id)
	{}

	uint32 player_id;
};

struct PlayerBattleInfo 
{
	PlayerBattleInfo()
		: player(0)
		, battle(0)
	{}

	Player * player;
	Battle * battle;
};

typedef unordered_map<session_t, PlayerBattleInfo *> PlayerBattleInfoSessionMap_t;

typedef unordered_map<uint32, PlayerBattleInfo *> PlayerBattleInfoIdMap_t;

typedef unordered_set<Battle *> PlayerBattleInfoSet_t;

class BattleManager
{
public:
	BattleManager();
	~BattleManager();
	static BattleManager * instance();
public:
	int init(bool del_frame_pack_for_press_test);

	void update();
public:
	//void connectClosed(session_t session_id);

	uint64 getSystemTime();
public:

	// protocol message
	int handleMsgPing(InterPacket * inter_pack, const MsgPing * msg);

	int handleMsgEnterBattleReq(InterPacket * inter_pack, const MsgEnterBattleReq * msg);
	int handleMsgReEnterBattleReq(InterPacket * inter_pack, const MsgReEnterBattleReq * msg);

	int handleMsgSelectHeroReq(InterPacket * inter_pack, const MsgSelectHeroReq * msg);
	int handleMsgConfirmHeroReq(InterPacket * inter_pack, const MsgConfirmHeroReq * msg);
	int handleMsgReportBattleResultReq(InterPacket * inter_pack, const MsgReportBattleResultReq * msg);

	int handleSMsgNotifyBattleMatchResult(InterPacket * inter_pack, const SMsgNotifyBattleMatchResult * msg);
	int handleSMsgVrReciveBattleReportNotifyBattle(InterPacket * inter_pack, const EmptyMsg * msg);

	int handleMsgLobbyKeepAliveReq(InterPacket * inter_pack, const MsgLobbyKeepAliveReq * msg);

	int handleMsgLobbyJoinGameReq(InterPacket * inter_pack, const MsgLobbyJoinGameReq * msg);

	int handleMsgCombatPlayerReadyReq(InterPacket * inter_pack, const MsgCombatPlayerReadyReq * msg);

	int handleMsgClientFrameSyncReq(InterPacket * inter_pack, const MsgClientFrameSyncReq * msg);

	int handleMsgRepairFrameReq(InterPacket * inter_pack, const MsgRepairFrameReq * msg);

	int handleMsgReconnectionReq(InterPacket * inter_pack, const MsgReconnectionReq * msg);

	int handlePlayerCloseConnection(InterPacket * inter_pack, const EmptyMsg *);

	int handleClientValidation(InterPacket * inter_pack, const MsgClientValidation * msg);

public:
	void sendPlayerMsg(Player * player, InterPacket * packet, bool is_delete_packet);

	void battleIsStarting(Battle * battle);

	ManageStat & getStat();

	bool isPlayerCanReciveMsg(Player * player);

	const BattleSetting & getBattleSetting();

	int getRandomValue();
protected:
	void sendPlayerMsg(uint64_t session_id, InterPacket * packet, bool is_delete_packet);

	void addPlayer(Player * player);

	void connectPlayerSession(Player * player, uint64_t session);

	void removePlayer(Player * player);

	void removeBattle(Battle * battle);

	void checkPlayerSessionByRelogin(Player * player);

	bool isPlayerLoginAgainWithoutLogout(Player * player);

	void addBattle(Battle * battle);
private:

	int m_player_index;

	Battle * m_current_batte;

	time_t m_last_update_battle_time;

	PlayerBattleInfoSet_t m_battle_set;

	PlayerManager m_player_manager;

	ManageStat m_manage_stat;

	bool m_del_frame_pack_for_press_test;

	BattleSetting m_battle_setting_cfg;
};

#endif
