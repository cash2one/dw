
#ifndef VR_PUBLIC_CONFIG_HPP__
#define VR_PUBLIC_CONFIG_HPP__

#include <string>
#include "SimpleXML.h"

using namespace std;

struct GateCfg 
{
	GateCfg()
		: port(0)
		, net_type(1)
	{}

	int port;
	int net_type;
	string ip_addr;
};

struct VrCfg
{
	VrCfg()
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

class VrPublicConfig
{
public:
	VrPublicConfig();
	~VrPublicConfig();
public:
	static VrPublicConfig * instance();

	int init();

	GateCfg & getGateCfg();

	VrCfg & getVrCfg();

	DbInfo & getDataCenterCfg();

	DbInfo & getDataDouwuCfg();

	GuidCfg & getGuidCfg();

protected:
	bool loadGateInfo(Document * doc);
	bool loadVrInfo(Document * doc);
	bool loadDataCenterDbInfo(Document * doc);
	bool loadDouwuDbInfo(Document * doc);
	bool loadguidCfg(Document * doc);
private:
	GateCfg m_gate_cfg;
	VrCfg m_vr_cfg;
	DbInfo m_data_center_info;
	DbInfo m_data_douwu_info;
	GuidCfg m_guid_cfg;

	static VrPublicConfig * m_instance;
};

#endif
