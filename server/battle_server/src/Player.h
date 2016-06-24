
#ifndef PLAYER_HPP__
#define PLAYER_HPP__

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "typedef.h"
#include "Packet_.h"
#include "BattleType.h"

using namespace std;

class Battle;

//extern int ONE_TEAM_MEMBER_NUMBER;

class Player;

enum BattlePlayerStatus
{
	BPS_BASE				= 0,
	BPS_PREPARE,
	BPS_COUNT_DOWN,
	BPS_IS_RECONNECTING,
	BPS_CLIENT_LOADING,
	BPS_READY,
	BPS_START_BATTLE,
};


typedef unordered_map<PlayerId_t, Player *> PlayerMap_t;

typedef unordered_set<Player *> PlayerSet_t;

//typedef unordered_map<uint32, Player *> PlayerIdMap_t;

enum PlayerStatus
{
	PS_BASE,
	PS_ONLINE,
	PS_IS_RECONNECTING,
	PS_OFFLINE,
};

struct PlayerReconnectInfo
{
	PlayerReconnectInfo()
		: player_status(PS_BASE)
		, last_send_frame_index(0)
	{}

	PlayerStatus player_status;
	uint32 last_send_frame_index;
};

class Player
{
public:
	Player();
	virtual ~Player();
public:
	int init(uint64_t session_id, PlayerId_t player_id, uint64_t gate_guid, int hero_id, const string & name, Battle * battle);

	void setGateGuid(uint64_t gate_guid);

	uint64_t getGateGuid();

	int playerClientLoading();

	int playerClientReady();

	int playerEnterBattle();

	void setSession(uint64_t sessiond_id);

	Battle * getBattle();

	BattlePlayerStatus & getStatus();

	PlayerReconnectInfo & getReconnectInfo();

	uint64_t getSession();

	PlayerId_t getId();

	int getTeamId();

	int getHeroId();

	void randomHeroForBattle();

	const string & getName();

	void setTeamId(int team_id);

	void enterBattle();

	bool isOffline();

	void setHeroId(int hero_id);

	void confirmHero(int hero_id);

	bool isConfirmedHero();

	void reportBattleEnd();
private:
	uint64_t m_session_id;
	PlayerId_t m_player_id;
	uint64_t m_gate_guid;
	int m_hero_id;
	int m_team_id;
	string m_player_name;
	Battle * m_player_battle;
	bool m_is_confirmed_hero;
	BattlePlayerStatus m_battle_player_status;
	PlayerReconnectInfo m_player_reconnect_info;
	
};

#endif
