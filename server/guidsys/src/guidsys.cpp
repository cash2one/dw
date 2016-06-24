// guidsys.cpp : 定义控制台应用程序的入口点。
//

//#ifdef WIN32
//#include "winsock2.h"
//#endif // WIN32


#include <boost/thread.hpp>
#include "Redirect.h"
#include "ManageLogger.h"
#include "ManageConfig.h"
#include "ManageSession.h"


void initSystemSocket();

int main(int argc, char * argv[])
{
	CHECK_INPUT_PARAM(argc, argv);

	START_DAEMONIZE_CMD();

	IGNORE_LINUX_SINGLE();

	ManageLogger::instance()->init();
	ManageConfig::instance()->init();
	ManageSession manage_session;

	if (manage_session.init() == -1)
	{
		DEF_LOG_ERROR("failed to init manage session\n");
		return 1;
	}

	while (true)
	{
		manage_session.update();

		boost::system_time sleep_time = boost::get_system_time() + boost::posix_time::millisec(1);
		boost::thread::sleep(sleep_time);

	}

	return 0;
}
