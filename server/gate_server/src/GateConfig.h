
#ifndef GATE_CONFIG_HPP__
#define GATE_CONFIG_HPP__

#include <string>
#include "SimpleXml.h"

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
	int ping_interval;
};

struct ServerCfg
{
	ServerCfg()
		: port(0)
	{}

	int port;
	string ip_addr;
};

class GateConfig
{
public:
	GateConfig();
	~GateConfig();
public:
	static GateConfig * instance();

	int init();

	ClientCfg & getClientCfg();

	ServerCfg & getServerCfg();
protected:
	bool loadClientInfo(Document * doc);

	bool loadServerInfo(Document * doc);
private:
	ClientCfg m_client_cfg;
	ServerCfg m_server_cfg;

	static GateConfig * m_instance;
};

#endif
