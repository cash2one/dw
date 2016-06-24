
#ifndef BATTLE_CONFIG_HPP__
#define BATTLE_CONFIG_HPP__

#include <string>
#include "SimpleXML.h"
#include "Singleton.h"
#include "BattleSetting.h"

using namespace std;

struct GateCfg 
{
	GateCfg()
		: port(0)
	{}

	int port;
	string ip_addr;
};

struct TrCfg 
{
	TrCfg()
	{}

	int port;
	string ip_addr;
};

struct BattleCfg 
{
	BattleCfg()
		: battle_id(0)
		, delete_frame_pack(0)
	{}

	int battle_id;
	int delete_frame_pack;
};

class BattleConfig
{
public:
	BattleConfig();
	~BattleConfig();
	static BattleConfig * instance();
public:
	int init();

	GateCfg & getGateCfg();

	TrCfg & getTrCfg();

	BattleCfg & getBattleCfg();

	static bool loadBattleSetting(BattleSetting & battle_setting);
protected:
	bool loadBattleCfg(Document * doc);
	bool loadGateCfg(Document * doc);
	bool loadTrCfg(Document * doc);

private:
	GateCfg m_gate_cfg;
	TrCfg m_tr_cfg;
	BattleCfg m_battle_cfg;
};

#endif
