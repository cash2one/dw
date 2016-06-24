
#ifndef MANAGE_DB_PING_HPP__
#define MANAGE_DB_PING_HPP__

#ifdef WIN32
#include <windows.h>
#endif

#include <string>
#include <mysql++.h>

using namespace std;

class ManageDbPing
{
public:
	ManageDbPing();
	~ManageDbPing();

	struct DbInfo 
	{
		DbInfo()
			: port(0)
		{}

		DbInfo(int _port, const string & _ip_addr, const string & _db_name, const string & _user_name, const string & _passwd)
			: port(_port)
			, ip_addr(_ip_addr)
			, db_name(_db_name)
			, user_name(_user_name)
			, passwd(_passwd)
		{}

		int port;
		string ip_addr;
		string db_name;
		string user_name;
		string passwd;
	};
public:
	void init(mysqlpp::Connection * db_connection, const DbInfo & db_info, time_t ping_time_interval = 60);

	void update();
protected:
	void pingDb();
	void connectToDb();
private:
	mysqlpp::Connection * m_db_connection;
	DbInfo m_db_info;
	time_t m_ping_time_interval;
	time_t m_last_ping_time;
};

#endif
