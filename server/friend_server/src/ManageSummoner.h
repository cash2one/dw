#ifndef MANAGE_SUMMONER_HPP__
#define MANAGE_SUMMONER_HPP__

#include <string>
#include "Summoner.h"


using namespace std;

typedef map<uint64_t, Summoner *> MAP_SUMMONER; //’ŸªΩ ¶¡–±Ì

class ManageSummoner
{
public:
	ManageSummoner();
	~ManageSummoner();

	static ManageSummoner * GetInstance();

	Summoner * findSummoner(uint64_t sum_id);
	void delSummoner(uint64_t sum_id);
	void addSummoner(uint64_t sum_id, Summoner * pSummoner);


public:
	static ManageSummoner * m_instance;

private:
	MAP_SUMMONER map_summoner;
};

#endif