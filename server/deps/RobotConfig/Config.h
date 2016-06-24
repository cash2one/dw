
#ifndef CONFIG_HPP__
#define CONFIG_HPP__

#include <string>
#include "SimpleXML.h"
#include "BattleConfig.h"

using namespace std;

class BattleConfigCmp
{
public:
	bool operator ()(const BattleConfig * first, const BattleConfig * second)
	{
		return first->getRobotNum() > second->getRobotNum();
	}
};

class Config
{
public:
	Config();
	~Config();
public:
	int load();
	int loadServerInfo(Document * doc);

	int loadBattleServerInfo(Document * doc);

	int loadRobotInfo(Document * doc);

public:
	int m_robot_start_id;
	int m_client_run_robots_num;
	int m_net_type;
	int m_port;
	string m_ip_addr;

	typedef priority_queue<BattleConfig *, vector<BattleConfig *>, BattleConfigCmp> BattleConfigQue_t;

	BattleConfigQue_t m_battle_config_que;

};

#endif
