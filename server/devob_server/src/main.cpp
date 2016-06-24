// tr.cpp : Defines the entry point for the console application.
//

#include <malloc.h>
#include "ManageBattleSession.h"
#include "ManageLogger.h"
#include "ManageSingleton.h"
#include "ManageClientSession.h"
#include "Redirect.h"
#include "ObConfig.h"

int startupService();

int main(int argc, char * argv[])
{
	int ret = startupService();
	if (0 != ret)
	{

	}
	return 0;
}


int startupService()
{
	IGNORE_LINUX_SINGLE();

	ManageSingleton::instance();
	ManageLogger::instance()->init();

	TrConfig::instance()->init();

	ManageClientSession::instance()->init(TrConfig::instance()->getVrCfg());
	ManageBattleSession::instance()->init(TrConfig::instance()->getBattleCfg());

	time_t last_maloc_trim_time = getSystemMilliSeconds();

	while (true)
	{
		ManageClientSession::instance()->update();
		ManageBattleSession::instance()->update();

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