
#ifndef PLAYER_MANAGER_HPP__
#define PLAYER_MANAGER_HPP__

#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include "Player.h"
#include "Packet_.h"
#include "BattleType.h"
#include "BattleType.h"

typedef boost::unordered_map<string, Player *> PlayerAccountMap_t;

typedef boost::unordered_map<PlayerId_t, Player *> PlayerIdMap_t;

typedef boost::unordered_map<uint64_t, Player *> PlayerSessionMap_t;

using namespace std;

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();
public:
	void addPlayer(Player * player);
	void removePlayer(Player * player);
	void connectPlayerSession(Player * player, uint64_t session);

	Player * getPlayerBySession(uint64_t session_id, bool report_error = true);
	//Player * getPlayerByName(const string & name, bool report_error = true);
	Player * getPlayerById(PlayerId_t player_id);

	void changePlayerSession(Player * player, uint64_t session_id);

	void removePlayerBySession(Player * player);

	bool isInBattle(PlayerId_t player_id);
protected:
	void addPlayerBySession(Player * player);
	void addPlayerById(Player * player);
	//void addPlayerByAccount(Player * player);

	//void removePlayerByName(Player * player);
	void removePlayerById(Player * player);

private:
	//PlayerAccountMap_t m_player_account_map;
	PlayerIdMap_t m_player_id_map;
	PlayerSessionMap_t m_player_session_map;
};

#endif
