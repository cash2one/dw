
#ifndef ACCOUNT_HPP__
#define ACCOUNT_HPP__

#include <unordered_map>
#include <string>
#include <list>
#include "MyGuidDef.h"
#include "Packet_.h"

using namespace std;

struct Account;
struct AccountServerInfo;

typedef unordered_map<MyGuid_t, Account *> AccountGuidMap_t;
typedef unordered_map<string, Account *> AccountNameMap_t;

typedef list<AccountServerInfo *> AccountServerInfoList_t;

struct Account
{
	Account();

	MyGuid_t guid;      // account 唯一id
	string user_name;
	int language_type;
	int channel_type;
	string client_version;
	int is_play;
	string account_name;
	string device_name;
	string password;
	int platform_id;
	string session_code;
	uint64_t client_id;			// 对应client_id, 和session相关，用来查找client
	session_t session_id;	// 对应vrpublic gate
	uint64_t summoner_id;
};

struct AccountServerInfo 
{
	AccountServerInfo();

	MyGuid_t guid;
	int area_id;
	int gs_index;
	time_t first_enter_time;
	time_t last_enter_time;
};

#endif
