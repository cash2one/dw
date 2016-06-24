// zyzr_frame.cpp : Defines the entry point for the console application.
//

#include <malloc.h>
#include <iostream>
#include <boost/thread.hpp>
#include "BattleManager.h"
#include "ManagerSession.h"
#include "ManageLogger.h"
#include "StatTime.h"
#include "BattleLog.h"
#include "Redirect.h"
#include "ManageStat.h"
#include "BattleConfig.h"
#include "ManageTrSession.h"
#include "check_macro.h"



//extern int ONE_TEAM_MEMBER_NUMBER;

int startServer(int argc, char * argv[]);

int main(int argc, char * argv[])
{
	int ret = startServer(argc, argv);

	boost::system_time sleep_time = boost::get_system_time() + boost::posix_time::millisec(3000);
	boost::thread::sleep(sleep_time);

	return ret;
}

int startServer(int argc, char * argv[])
{
#ifndef WIN32
	//mallopt(M_MMAP_MAX, 0);         // 禁止malloc调用mmap分配内存
	//mallopt(M_TRIM_THRESHOLD, -1);  // 禁止内存紧缩
#endif // !WIN32

	//int ret = ACE::init();

	//if (argc < 4)
	//{
	//	std::cout << "please enter parameter : listen port, one team member number, net type" << std::endl;
	//	return 0;
	//}

	//int del_frame_pack = 0;
	//if (argc > 4)
	//{
	//	del_frame_pack = atoi(argv[4]);;
	//}

	//int port = atoi(argv[1]);
	//ONE_TEAM_MEMBER_NUMBER = atoi(argv[2]);
	//int net_type = atoi(argv[3]);

	CHECK_INPUT_PARAM(argc, argv);

	START_DAEMONIZE_CMD();

	IGNORE_LINUX_SINGLE();

	CHECK_INIT_VALUE(ManageLogger::instance()->init());

	//Redirect redirect;
	BattleConfig::instance()->init();

	GateCfg & gate_cfg = BattleConfig::instance()->getGateCfg();
	TrCfg & tr_cfg = BattleConfig::instance()->getTrCfg();

	CHECK_INIT_VALUE(BattleConfig::instance()->init());

	CHECK_INIT_VALUE(ManagerSession::instance()->init(gate_cfg.ip_addr, gate_cfg.port));

	CHECK_INIT_VALUE(BattleManager::instance()->init(1 == BattleConfig::instance()->getBattleCfg().delete_frame_pack));

	CHECK_INIT_VALUE(ManageTrSession::instance()->init(tr_cfg));

	StatTime stat_time_msg("stat ManagerSession", 50);
	StatTime stat_battle_manager("stat BattleManager", 50);

	time_t last_maloc_trim_time = getSystemMilliSeconds();

	//BATTLE_LOG_INFO(" team number is <%d.", ONE_TEAM_MEMBER_NUMBER);

	int incoming_packet_num = 0;
	int incoming_bytes = 0;

	while (true)
	{
		//ManagerSession::instance()->update();
		stat_time_msg.reset();
		ManagerSession::instance()->update();
		stat_time_msg.stat();

		BattleManager::instance()->getStat().addIncomingPacket(incoming_packet_num, incoming_bytes);

		stat_battle_manager.reset();
		BattleManager::instance()->update();
		stat_battle_manager.stat();

		ManageTrSession::instance()->update();

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