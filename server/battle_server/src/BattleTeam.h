
#ifndef BATTLE_TEAM_HPP__
#define BATTLE_TEAM_HPP__

#include "Player.h"

enum TeamID
{
	TeamID_RED = 1,
	TeamID_BLUE,
};

class BattleTeam
{
public:
	BattleTeam();
	~BattleTeam();
public:
	int init(int one_team_number);
	int addPlayer(Player * player);

	int removePlayer(Player * player);

	PlayerMap_t * getPlayerTeam(Player * player);

	int getAllMemberNumber();

	int getAllMemberReadyNumber();

	bool isAllReady();

	void startBattle();
	
	PlayerMap_t getTeamById(int team_id);

protected:
private:
	int m_one_team_number;
	PlayerMap_t m_first_team;

	PlayerMap_t m_second_team;

	PlayerMap_t m_all_team_member;
};
#endif