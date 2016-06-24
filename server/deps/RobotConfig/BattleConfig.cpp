
#include "BattleConfig.h"

BattleConfig::BattleConfig()
	: m_robot_num(0)
	, m_port(0)
{
}

BattleConfig::~BattleConfig()
{

}

void BattleConfig::init(const string & ip_addr, int port)
{
	m_ip_addr = ip_addr;
	m_port = port;
}

void BattleConfig::addRobotNum(int robot_num)
{
	m_robot_num += robot_num;
}

int BattleConfig::getRobotNum() const
{
	return m_robot_num;
}

int BattleConfig::getPort() const
{
	return m_port;
}

string BattleConfig::getIpAddr() const
{
	return m_ip_addr;
}
