#ifndef ACCOUNT_HPP__
#define ACCOUNT_HPP__

#include <unordered_map>
#include <string>
#include <list>
#include "MyGuidDef.h"
#include "Packet_.h"

using namespace std;

class Summoner;

class Account
{
public:
	Account();

	Summoner * getSummoner();
	void setSummoner(Summoner *pSum);


public:
	Summoner * m_summoner;

	MyGuid_t account_id;
	string account_name;
	uint64_t client_id;			// 对应client_id, 和session相关，用来查找client
	session_t session_id;	// 对应vr gate

	int language_type;
	int channel_type;
	string client_version;
	int is_play;
	string device_name;
	string password;
	int platform_id;
};

#endif
