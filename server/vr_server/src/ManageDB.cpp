#include "ManageDB.h"

ManageDB * ManageDB::m_instance = NULL;

ManageDB::ManageDB()
{
}

ManageDB::~ManageDB()
{
	if (m_instance != NULL)
	{
		delete m_instance;
	}
}

ManageDB * ManageDB::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new ManageDB();
	}

	return m_instance;
}

int ManageDB::init(const DbInfo & data_db)
{
	DEF_LOG_INFO("init manage db, db ip is <%s>, port is <%d>, db name is <%s>, user name is <%s>", data_db.ip_addr.c_str(), data_db.port, data_db.db_name.c_str(), data_db.user_name.c_str());

	db_info = data_db;

	m_db_connection = new mysqlpp::Connection(false);
	if (!m_db_connection->connect(db_info.db_name.c_str(), db_info.ip_addr.c_str(), db_info.user_name.c_str(), db_info.passwd.c_str(), db_info.port))
	{
		DEF_LOG_ERROR("failed to connect data center database <%s:%p>, <%s:%s>", db_info.ip_addr.c_str(), db_info.port, db_info.user_name.c_str(), db_info.passwd.c_str());
		return -1;
	}

	m_manage_db_ping.init(m_db_connection, ManageDbPing::DbInfo(db_info.port, db_info.ip_addr, db_info.db_name, db_info.user_name, db_info.passwd));

	return 0;
}

void ManageDB::update()
{
	m_manage_db_ping.update();
}

mysqlpp::Connection * ManageDB::getMysqlConnection()
{
	return m_db_connection;
}
