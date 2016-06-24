
#ifndef FRIEND_CONFIG_HPP__
#define FRIEND_CONFIG_HPP__

#include <string>
#include "SimpleXML.h"
#include "Singleton.h"

using namespace std;

struct FriendCfg 
{
	FriendCfg()
		: port(0)
	{}

	int port;
	string ip_addr;
};

struct DbInfo
{
	DbInfo()
		: port(0)
	{}

	string ip_addr;
	string db_name;
	int port;
	string user_name;
	string passwd;
};

class FriendConfig
{
public:
	FriendConfig();
	~FriendConfig();
	static FriendConfig * instance();
public:
	int init();
	const FriendCfg & getFriendCfg();
	const DbInfo & getDBConfig();
protected:
	bool loadFriendCfg(Document * doc);
	bool loadDBFriendCfg(Document * doc);
private:
	FriendCfg m_friend_cfg;
	DbInfo m_db_friend_info;
};
#endif
