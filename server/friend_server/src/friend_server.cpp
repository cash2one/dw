// friend_server.cpp : Defines the entry point for the console application.
//

#include <malloc.h>
#include "ManageLogger.h"
#include "ManageSingleton.h"
#include "Redirect.h"
#include "FriendConfig.h"
#include "ManageVrSession.h"
#include "ManageDB.h"

#define CHECK_INIT_VALUE(ret)	\
	if (ret == -1)	\
					{	\
		DEF_LOG_ERROR("failed to init <%s>", #ret);	\
		return 1;	\
					}

int startupService();

int main(int argc, char * argv[])
{
	int ret = startupService();
	if (0 != ret)
	{
		return 1;
	}
	return 0;
}

int startupService()
{
	IGNORE_LINUX_SINGLE();

	ManageSingleton::instance();
	CHECK_INIT_VALUE(ManageLogger::instance()->init());

	CHECK_INIT_VALUE(FriendConfig::instance()->init());

	CHECK_INIT_VALUE(ManageVrSession::instance()->init(FriendConfig::instance()->getFriendCfg()));

	CHECK_INIT_VALUE(ManageDB::GetInstance()->init(FriendConfig::instance()->getDBConfig()));

	time_t last_maloc_trim_time = getSystemMilliSeconds();

	while (true)
	{
		int dispatch_pack_num = ManageVrSession::instance()->update();

		if (0 == dispatch_pack_num)
		{
			boost::system_time sleep_time = boost::get_system_time() + boost::posix_time::millisec(1);
			boost::thread::sleep(sleep_time);
		}

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