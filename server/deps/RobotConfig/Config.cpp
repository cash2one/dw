
#include <memory>
#include "Config.h"

Config::Config()
	: m_robot_start_id(0)
	, m_client_run_robots_num(1000)
	, m_net_type(1)
	, m_port(0)
{

}

Config::~Config()
{

}

int Config::load()
{
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		return -1;
	}

	if (!auto_doc->load_file("robot_cfg.xml"))
	{
		return -1;
	}

	this->loadServerInfo(auto_doc.get());
	this->loadBattleServerInfo(auto_doc.get());
	this->loadRobotInfo(auto_doc.get());

	return 0;
}

int Config::loadServerInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("server");
	m_port = ele->get_attribute("port")->as_int();
	m_ip_addr = ele->get_attribute("addr")->get_value();
	return 0;
}

int Config::loadBattleServerInfo(Document * doc)
{
	Element * svrs_ele = doc->get_root()->get_element("battle_servers");
	Elements svr_eles = svrs_ele->get_elements("server");
	for (auto svr_ele : svr_eles)
	{
		string ip_addr = svr_ele->get_attribute("addr")->get_value();
		int port = svr_ele->get_attribute("port")->as_int();
		BattleConfig * bc = new BattleConfig();
		bc->init(ip_addr, port);
		m_battle_config_que.push(bc);
	}

	return 0;
}

int Config::loadRobotInfo(Document * doc)
{
	Element * robot_ele = doc->get_root()->get_element("robot");

	m_robot_start_id = robot_ele->get_attribute("start_id")->as_int();
	m_client_run_robots_num = robot_ele->get_attribute("client_run_robots_num")->as_int();
	m_net_type = robot_ele->get_attribute("net_type")->as_int();

	return 0;
}
