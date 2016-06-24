// UnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include "testPool.hpp"
#include "ManageLogger.h"
#include "LoggerFactory.h"
#include "testFunctionTraits.hpp"
#include "testPlugin.hpp"
#include "testfsm.hpp"
#include "testRemoveNote.hpp"

int _tmain(int argc, _TCHAR* argv[])
{	
	ManageLogger::instance();
	ManageLogger::instance()->init();

	//TestPool::run();

	//TestFunctionTraits::run();
	TestPlugin::run();
	//testfsm::run();
	//system("pause");
	//TestRemoveNote::run("F:\\backup\\oold");
	return 0;
}

