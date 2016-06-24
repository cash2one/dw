// tr.cpp : Defines the entry point for the console application.
//

#include <malloc.h>
#include "ManageBattleSession.h"
#include "ManageLogger.h"
#include "ManageSingleton.h"
#include "ManageVrSession.h"
#include "Redirect.h"
#include "TrConfig.h"
#include "ManageMatch.h"

int startupService(int argc, char * argv[]);

int main(int argc, char * argv[])
{
	int ret = startupService(argc, argv);
	if (0 != ret)
	{
		return 1;
	}
	return 0;
}


int startupService(int argc, char * argv[])
{
	CHECK_INPUT_PARAM(argc, argv);

	START_DAEMONIZE_CMD();

	IGNORE_LINUX_SINGLE();

	ManageSingleton::instance();
	ManageLogger::instance()->init();

	TrConfig::instance()->init();

	ManageMatch::instance()->init();

	ManageVrSession::instance()->init(TrConfig::instance()->getVrCfg());
	ManageBattleSession::instance()->init(TrConfig::instance()->getBattleCfg());

	time_t last_maloc_trim_time = getSystemMilliSeconds();

	while (true)
	{
		ManageVrSession::instance()->update();
		ManageBattleSession::instance()->update();

		ManageMatch::instance()->update();

		boost::system_time sleep_time = boost::get_system_time() + boost::posix_time::millisec(1);
		boost::thread::sleep(sleep_time);


#ifndef WIN32
		time_t curr_time = getSystemMilliSeconds();
		if (curr_time - last_maloc_trim_time > 60000)
		{
			malloc_trim(0);
			last_maloc_trim_time = curr_time;
		}
#endif
	}

	return 0;
}