#ifndef MANAGE_SUMMONER_HPP__
#define MANAGE_SUMMONER_HPP__

#ifdef WIN32
#include <WinSock2.h>
#endif // WIN32

#include <string>
#include <mysql++/mysql++.h>
#include "Summoner.h"
#include "ManageDB.h"

using namespace std;

typedef map<uint64_t, Summoner *> MAP_SUMMONER; //’ŸªΩ ¶¡–±Ì

class ManageSummoner
{
public:
	ManageSummoner();
	~ManageSummoner();

	static ManageSummoner * GetInstance();

	Summoner * findSummoner(uint64_t sum_id, bool add_to_mem = true);
	Summoner * DBLoadSumByID(uint64_t sum_id);
	void addMemSummonerById(uint64_t sum_id, Summoner * pSummoner);
	void addFriend(Summoner * pSummoner, STC_FRIEND_INFO & stcFriendInfo);

public:
	static ManageSummoner * m_instance;

private:
	MAP_SUMMONER map_summoner;
};

#endif