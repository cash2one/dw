
#ifndef BATTLE_CONFIG_HPP__
#define BATTLE_CONFIG_HPP__

#include <string>

using namespace std;

class BattleConfig
{
public:
	BattleConfig();
	~BattleConfig();
public:
	void init(const string & ip_addr, int port);

	void addRobotNum(int robot_num);

	int getRobotNum() const;

	int getPort() const;

	string getIpAddr() const;
protected:
private:
	int m_robot_num;

	int m_port;

	string m_ip_addr;
};

#endif
