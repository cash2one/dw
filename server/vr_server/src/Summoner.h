#ifndef SUMMONER_H_
#define SUMMONER_H_

#include "Logger.h"
#include "Packet_.h"

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

	void setAccount(Account * pAccount);
	void addFriend();

public:
	Account * m_account;

	uint64_t m_nId;
	string m_strSumName;

	uint64_t client_id;		// ��Ӧclient_id, ��session��أ���������client
	session_t session_id;	// ��Ӧvr gate

	MAP_FRIEND map_friend;
};


#endif