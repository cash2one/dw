// RobotConfig.cpp : Defines the entry point for the console application.
//

#include <boost/thread.hpp>
#include <string>
#include <iostream>
#include "ManageLogger.h"
#include "ManagerConfig.h"
#include "Redirect.h"

using namespace std;

int main(int argc, char * argv[])
{
	CHECK_INPUT_PARAM(argc, argv);

	START_DAEMONIZE_CMD();

	IGNORE_LINUX_SINGLE();

	ManageLogger::instance()->init();

	ManagerConfig manage_config;
	if (0 != manage_config.init())
	{
		std::cout << "failed to init ManagerConfig" << std::endl;
		return 1;
	}

	boost::posix_time::milliseconds empty_sleep_time(1000);
	while (true)
	{
		boost::this_thread::sleep(empty_sleep_time);
	}
	return 0;

}

