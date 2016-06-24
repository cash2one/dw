// gate_server.cpp : Defines the entry point for the console application.
//

#include <malloc.h>
#include "ManageLogger.h"
#include "ManageClientSession.h"
#include "ManageServerSession.h"
#include "Redirect.h"
#include "GateConfig.h"

int serverStartup(int argc, char * argv[])
{
	CHECK_INPUT_PARAM(argc, argv);

	START_DAEMONIZE_CMD();

	IGNORE_LINUX_SINGLE();

	ManageLogger::instance()->init();

	if (GateConfig::instance()->init() == -1)
	{
		DEF_LOG_ERROR("failed to load gate config");
		return 1;
	}

	ClientCfg & client_cfg = GateConfig::instance()->getClientCfg();
	ServerCfg & server_cfg = GateConfig::instance()->getServerCfg();

	int ret = ManageClientSession::instance()->init(client_cfg.ip_addr, client_cfg.port, client_cfg.net_type);

	ret = ManageServerSession::instance()->init(server_cfg.ip_addr, server_cfg.port);

	time_t last_maloc_trim_time = getSystemMilliSeconds();

	while (true)
	{
		int pack_num = ManageClientSession::instance()->update();

		if (0 == pack_num)
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

int main(int argc, char * argv[])
{
	int ret = serverStartup(argc, argv);
	if (ret != 0)
	{
		boost::system_time sleep_time = boost::get_system_time() + boost::posix_time::millisec(3000);
		boost::thread::sleep(sleep_time);
	}

	return ret;
}

