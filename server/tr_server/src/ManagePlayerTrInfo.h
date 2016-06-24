
#ifndef MANAGE_PLAYER_TR_INFO_HPP__
#define MANAGE_PLAYER_TR_INFO_HPP__

#include <stdint.h>
#include <unordered_map>
#include "PlayerTrInfo.h"
#include "Singleton.h"

using namespace std;

class ManagePlayerTrInfo
{
public:
	ManagePlayerTrInfo();
	~ManagePlayerTrInfo();
	static ManagePlayerTrInfo * instance();
public:
	PlayerTrInfo * getPlayer(uint64_t account_id);

	void addPlayer(PlayerTrInfo * player);

protected:
private:
	typedef unordered_map<uint64_t, PlayerTrInfo *> PlayerTrInfoMap_t;

	PlayerTrInfoMap_t m_player_info_map;
};

#endif

