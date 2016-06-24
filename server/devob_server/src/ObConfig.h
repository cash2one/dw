

#ifndef TR_CONFIG_HPP__
#define TR_CONFIG_HPP__

#include <string>
#include "SimpleXML.h"

using namespace std;

struct VrCfg 
{
	VrCfg()
		: port(0)
	{}

	string ip_addr;
	int port;
};

struct BattleCfg 
{
	BattleCfg()
		: port(0)
	{}

	string ip_addr;
	int port;
};

class TrConfig
{
public:
	TrConfig();
	~TrConfig();
public:
	static TrConfig * instance();
public:
	int init();

	const VrCfg & getVrCfg();

	const BattleCfg & getBattleCfg();
protected:
	bool loadTrCfg(Document * doc);
	bool loadBattleCfg(Document * doc);
private:
	VrCfg m_vr_cfg;
	BattleCfg m_battle_cfg;
};

#endif
