
#ifndef VRCONFIG_HPP__
#define VRCONFIG_HPP__

#include <string>
#include "SimpleXML.h"

using namespace std;

struct ClientCfg 
{
	ClientCfg()
		: port(0)
		, net_type(1)
	{}

	int port;
	string ip_addr;
	int net_type;
};

struct VrCfg 
{
	VrCfg()
		: area_id(0)
		, server_id(0)
	{}

	int area_id;
	int server_id;
};

struct TrCfg 
{
	TrCfg()
		: port(0)
	{}

	int port;
	string ip_addr;
};

struct VrpublicCfg
{
	VrpublicCfg()
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

struct GuidCfg
{
	GuidCfg()
		: port(0)
	{}

	string ip_addr;
	int port;
};

struct FriendCfg 
{
	FriendCfg()
		: port(0)
	{}
	string ip_addr;
	int port;
};

class VrConfig
{
public:
	VrConfig();
	~VrConfig();
public:
	static VrConfig * instance();

	int init();

	VrCfg & getVrCfg();

	ClientCfg & getClientCfg();

	TrCfg & getTrCfg();

	VrpublicCfg & getVrpublicCfg();

	DbInfo & getDBAccountInfo();

	GuidCfg & getGuidCfg();

	FriendCfg & getFriendCfg();

	std::string getProtoCfg();

protected:
	bool loadVrInfo(Document * doc);

	bool loadClientInfo(Document * doc);

	bool loadTrInfo(Document * doc);

	bool loadVrpublicInfo(Document * doc);
	bool loadDBInfo(Document * doc);
	bool loadguidCfg(Document * doc);
	bool loadFriendCfg(Document * doc);
	bool loadProtoCfg(Document* doc);
private:

	ClientCfg m_client_cfg;

	TrCfg m_tr_cfg;

	VrpublicCfg m_vrpublic_cfg;

	VrCfg m_vr_cfg;

	DbInfo m_data_acount_info;
	
	GuidCfg m_guid_cfg;

	FriendCfg m_friend_cfg;

	std::string m_proto_path;

	static VrConfig * m_instance;
};

#endif
