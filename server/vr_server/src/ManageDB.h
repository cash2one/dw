#ifndef MANAGE_DB_HPP__
#define MANAGE_DB_HPP__

#ifdef WIN32
#include <WinSock2.h>
#endif // WIN32

#include "VrConfig.h"
#include <mysql++/mysql++.h>
#include "Logger.h"
#include "ManageDbPing.h"

class ManageDB
{
private:
	ManageDB();
public:
	~ManageDB();

	static ManageDB * GetInstance();

	int init(const DbInfo & data_account_db);

	void update();
	
	mysqlpp::Connection * getMysqlConnection();
	

public:
	static ManageDB * m_instance;

private:
	mysqlpp::Connection * m_db_connection;
	DbInfo db_info;
	ManageDbPing m_manage_db_ping;
};


#endif 