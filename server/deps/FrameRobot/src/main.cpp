// FrameRobot.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "ManageLogger.h"
#include "Logger.h"
#include "Redirect.h"
#include "ManageRobotConfig.h"
#include "ManageLoginRobot.h"
#include "ManageVrRobot.h"
#include "ManageBattleRobot.h"
#include "ManageSingleton.h"
#include "Robot.h"

#include "Single.hpp"
#include "pluginDepot.h"
#include "FRPlugin.h"
#include "FunCallCount.hpp"

int startRobot(int argc, char * argv[]);

int loadRobotCfg();
int main(int argc, char * argv[])
{
	int ret = startRobot(argc, argv);

	boost::system_time sleep_time = boost::get_system_time() + boost::posix_time::millisec(3000);
	boost::thread::sleep(sleep_time);

	return 0;
}

int startRobot(int argc, char * argv[])
{
	if (argc < 4)
	{
		std::cout << "player input ip_address, port, net type" << std::endl;
		return 0;
	}

	IGNORE_LINUX_SINGLE();

	ManageSingleton::instance();

	ManageLogger::instance()->init();

	if (argc >= 5)
	{
		int server_id = atoi(argv[4]);
		Robot::setChooseServerId(server_id);
	}

	string ip_str = argv[1];
	int port = atoi(argv[2]);
	int net_type = atoi(argv[3]);

	ManageRobotConfig::instance()->init(ip_str, port, net_type);

	RobotCfg robot_cfg;
	boost::posix_time::milliseconds empty_sleep_time(1000);
	while (true)
	{
		if (ManageRobotConfig::instance()->isGotRobotCfg())
		{
			robot_cfg = ManageRobotConfig::instance()->getRobotCfg();
			break;
		}

		DEF_LOG_INFO("waiting for robot config server replay");

		boost::this_thread::sleep(empty_sleep_time);
	}


	//
	Single<FunCallCount>::Instance();

	//std::cout << "ip is        : " << robot_cfg.bs_ip_addr.c_str() << std::endl;
	//std::cout << "port is      : " << port << std::endl;
	//std::cout << "robot num is : " << robot_num << std::endl;
	DEF_LOG_INFO("get vrpublic server info is <%s:%d>, net type is <%d>", robot_cfg.ip_addr.c_str(), robot_cfg.port, robot_cfg.net_type);
	DEF_LOG_INFO("robot start id is <%d>", robot_cfg.robot_start_index);
	DEF_LOG_INFO("robot count is <%d>", robot_cfg.robot_num);

	Single<RunningRobot>::Instance();
	Single<PluginDepot>::Instance();
	Single<PluginDepot>::GetInstance()->addPlugin(Single<FRPlugin>::Instance());
	Single<Sender>::Instance();

	Single<Sender>::GetInstance()->cl2vr_cb = std::bind(&ManageVrRobot::handleOutputPacket,
		ManageVrRobot::instance(), std::placeholders::_1);

	Single<Sender>::GetInstance()->cl2vrp_cb = std::bind(&ManageLoginRobot::handleOutputPacket,
		ManageLoginRobot::instance(), std::placeholders::_1);

	Single<Sender>::GetInstance()->cl2battle_cb = std::bind(&ManageBattleRobot::handleOutputPacket,
		ManageBattleRobot::instance(), std::placeholders::_1);


	// 在pump之前初始化
	PluginDepotCfg plugindepot_cfg;
	plugindepot_cfg.logger = g_logger_instance;
	//plugindepot_cfg.pool = Single<Pool>::GetInstance();
	plugindepot_cfg.sender = Single<Sender>::GetInstance();
	if (-1 == Single<PluginDepot>::GetInstance()->init(plugindepot_cfg))
	{
		DEF_LOG_ERROR("failed to init plugin depot");
		return -1;
	}

	if (-1 == loadRobotCfg())
		return -1;

	ManageLoginRobot::instance()->init(robot_cfg.ip_addr, robot_cfg.port, robot_cfg.robot_start_index, robot_cfg.robot_num, robot_cfg.net_type);
	ManageVrRobot::instance()->init();
	ManageBattleRobot::instance()->init();

	while (true)
	{
		ManageLoginRobot::instance()->update();
		ManageVrRobot::instance()->update();
		ManageBattleRobot::instance()->update();

		boost::system_time sleep_time = boost::get_system_time() + boost::posix_time::millisec(1000);
		boost::thread::sleep(sleep_time);
	}

	return 0;
}

int loadRobotCfg()
{
	PluginParam* param = Single<PluginDepot>::GetInstance()->getPluginParam("robot");
	if (NULL == param)
	{
		DEF_LOG_ERROR("NOT define robot param in config.xml");
		return -1;
	}

	Single<RobotCfg>::Instance();
	bool res = true;
	res = param->getStr("account_name_prefix", Single<RobotCfg>::GetInstance()->account_name_prefix) && res;
	res = param->getInt32("account_index_begin", Single<RobotCfg>::GetInstance()->account_index_begin) && res;
	res = param->getInt32("account_index_end", Single<RobotCfg>::GetInstance()->account_index_end) && res;

	res = param->getInt32("login_once_num", Single<RobotCfg>::GetInstance()->login_once_num) && res;
	res = param->getInt32("login_interval", Single<RobotCfg>::GetInstance()->login_interval) && res;
	res = param->getInt32("logoff_once_num", Single<RobotCfg>::GetInstance()->logoff_once_num) && res;
	res = param->getInt32("logoff_interval", Single<RobotCfg>::GetInstance()->logoff_interval) && res;
	res = param->getInt32("random_create_name_begin_index", Single<RobotCfg>::GetInstance()->random_create_name_begin_index) && res;
	res = param->getInt32("random_create_name_end_index", Single<RobotCfg>::GetInstance()->random_create_name_end_index) && res;
	res = param->getInt32("search_friend_times", Single<RobotCfg>::GetInstance()->search_friend_times) && res;
	res = param->getInt32("confirm_add_prob", Single<RobotCfg>::GetInstance()->confirm_add_prob) && res;
	if (!res)
	{
		DEF_LOG_ERROR("LOST define robot param in config.xml");
		return -1;
	}

	return 0;
}

