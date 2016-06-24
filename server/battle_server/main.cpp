// zyzr_frame.cpp : Defines the entry point for the console application.
//

#include <ace/OS.h>
#include <ace/ace.h>
#include <ace/Init_ACE.h>
#include <ace/Reactor.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include <ace/SOCK_Dgram.h>
#include "BattleManager.h"
#include "ManagerSession.h"
#include "MessageManager.h"
#include "ManageLogger.h"
#include "Singleton.h"
#include "ManageSingleton.h"
#include "StatTime.h"
#include "BattleLog.h"

extern int ONE_TEAM_MEMBER_NUMBER;

int main(int argc, char * argv[])
{
	int ret = ACE::init();

	if (argc >= 2)
	{
		ONE_TEAM_MEMBER_NUMBER = atoi(argv[1]);
	}

	ManageLogger::instance()->init();

	MessageManager::instance();
	BattleManager::instance()->init();
	ManagerSession::instance()->init("0.0.0.0:7777");

	ACE_Reactor * reactor = NULL;
#ifdef WIN32
	reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(100000), true);
#endif

	StatTime stat_time_msg("stat MessageManager", 1);
	StatTime stat_battle_manager("stat BattleManager", 5);

	BATTLE_LOG_INFO(" team number is <%d.", ONE_TEAM_MEMBER_NUMBER);

	BATTLE_LOG_DEBUG("test log <%d> <%s>", 324, " good !");

	while (true)
	{
		//ManagerSession::instance()->update();
		stat_time_msg.reset();
		MessageManager::instance()->update();
		stat_time_msg.stat();

		stat_battle_manager.reset();
		BattleManager::instance()->update();
		stat_battle_manager.stat();

		ACE_Time_Value sleep_time(0, 1000);
		ACE_OS::sleep(sleep_time);
	}

	//reactor->run_reactor_event_loop();

	ACE::fini();
	return 0;
}

