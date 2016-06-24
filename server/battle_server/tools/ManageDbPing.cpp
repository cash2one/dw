
#include "ManageDbPing.h"
#include "Logger.h"

ManageDbPing::ManageDbPing()
	: m_db_connection(NULL)
	, m_ping_time_interval(60)
	, m_last_ping_time(0)
{
	m_last_ping_time = getSystemMilliSeconds();
}

ManageDbPing::~ManageDbPing()
{

}

void ManageDbPing::init(mysqlpp::Connection * db_connection, const DbInfo & db_info, time_t ping_time_interval)
{
	m_db_connection = db_connection;
	m_db_info = db_info;

	m_last_ping_time = getSystemMilliSeconds();
}


void ManageDbPing::update()
{
	time_t curr_time = getSystemMilliSeconds();
	if (curr_time - m_last_ping_time >= m_ping_time_interval * 1000)
	{
		this->pingDb();
		m_last_ping_time = curr_time;
	}
}

void ManageDbPing::pingDb()
{
	if (NULL == m_db_connection)
	{
		return;
	}

	if (!m_db_connection->connected())
	{
		this->connectToDb();
	}

	if (m_db_connection->connected())
	{
		mysqlpp::Query query = m_db_connection->query();
		query << "SELECT * FROM ping";

		query.store();

		DEF_LOG_DEBUG("success to ping the db server <%s:%d -- %s>", m_db_info.ip_addr.c_str(), m_db_info.port, m_db_info.db_name.c_str());
	}
}

void ManageDbPing::connectToDb()
{
	if (!m_db_connection->connect(m_db_info.db_name.c_str(), m_db_info.ip_addr.c_str(), m_db_info.user_name.c_str(), m_db_info.passwd.c_str(), m_db_info.port))
	{
		DEF_LOG_ERROR("failed to connect data center database <%s:%p>, <%s:%s>", m_db_info.ip_addr.c_str(), m_db_info.port, m_db_info.user_name.c_str(), m_db_info.passwd.c_str());
		return ;
	}

	DEF_LOG_INFO("success to connect the db server <%s:%d -- %s>", m_db_info.ip_addr.c_str(), m_db_info.port, m_db_info.db_name.c_str());
}

