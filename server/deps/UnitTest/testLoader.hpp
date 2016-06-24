#pragma  once
#include "loader.hpp"
class TestLoader
{
	void testLoader()
	{
		class Test
		{
		public:
			void  finish(RequestInfost *st)
			{

			}
		};


		Loader a;
		Test test;
		a.Init(4, "127.0.0.1", "root", "123456", 3306, "game");
		RequestInfost * st = new RequestInfost;
		st->op = DB_INSERT;
		dbass::Assaccount * account = new dbass::Assaccount;
		Entity::account* e_account = dynamic_cast<Entity::account*>(account->data());
		e_account->set_guid(3);
		st->as = account;
		st->cb = std::bind(&Test::finish, &test, std::placeholders::_1);
		a.AddTask(0, st);
		a.Stop();
	}
};