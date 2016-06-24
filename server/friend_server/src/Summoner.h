#ifndef SUMMONER_H_
#define SUMMONER_H_

#include "Logger.h"
#include "Packet_.h"
#include "friend_generated.h"

using namespace std;

class Account;

typedef struct _STC_FRIEND_INFO
{
	uint64_t m_nId;
	//
}STC_FRIEND_INFO;

typedef std::map<uint64_t, STC_FRIEND_INFO> MAP_FRIEND;

class Summoner
{
public:
	Summoner(uint64_t sum_id);
	Summoner();
	~Summoner();

	void setStatus(MobaGo::FlatBuffer::SummonerStatus status);

public:

	uint64_t m_nId;
	MobaGo::FlatBuffer::SummonerStatus sum_status;
};


#endif