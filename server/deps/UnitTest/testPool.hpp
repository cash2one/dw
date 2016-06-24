#pragma once

#include "pool.h"
#include "Single.hpp"
#include "dynamicparse.h"
#include "loader.hpp"
#include "account.pb.h"

class TestPool
{
public:

	void loaded(uint32_t id, uint64_t guid, MSG_TYPE* msg)
	{
		Entity::summoner* q = dynamic_cast<Entity::summoner *>(msg);
		q;

	}

	static void run()
	{	
		Single<DynamicParser>::Instance();
		Single<DynamicParser>::GetInstance()->init("F:\\zyzr_server\\trunk\\deps\\entity\\proto\\vr");

		TableColumnsMap cols;
		Single<DynamicParser>::GetInstance()->querydbDesc("127.0.0.1", 3306, "root", "123456", "game", cols);
		std::vector<std::string> changed;
		Single<DynamicParser>::GetInstance()->checkEntity(cols, "game", changed);
		Single<DynamicParser>::GetInstance()->updatedbChanged("127.0.0.1", 3306, "root", "123456", "game", changed);

		Single<Loader>::Instance();
		Single<Loader>::GetInstance()->Init(4, "127.0.0.1", "root", "123456", 3306, "game");

		Single<Pool>::Instance();

		Entity::summoner *s = new Entity::summoner;
		s->set_guid(6);
		s->set_account_id(5);
		s->set_sum_name("xx");
		Single<Pool>::GetInstance()->add(6, s);
		TestPool test;
		LoadCallBack cb = std::bind(&TestPool::loaded, &test, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		Single<Pool>::GetInstance()->load(5, cb);

		while (true)
		{
			Single<Pool>::GetInstance()->update();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}	

	}
};