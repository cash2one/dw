
#include "Logger.h"
#include "ManageServerList.h"
#include "Logger.h"

ManageServerList * ManageServerList::m_instance = NULL;

ManageServerList::ManageServerList()
	: m_db_connection(NULL)
{

}

ManageServerList::~ManageServerList()
{

}

ManageServerList * ManageServerList::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageServerList();
	}

	return m_instance;
}

int ManageServerList::init(const DbInfo & douwu_db_info)
{
	DEF_LOG_INFO("init manage server list, db ip is <%s>, port is <%d>, db name is <%s>, user name is <%s>", douwu_db_info.ip_addr.c_str(), douwu_db_info.port, douwu_db_info.db_name.c_str(), douwu_db_info.user_name.c_str());

	m_douwu_db_info = douwu_db_info;

	if (!initDb())
	{
		return -1;
	}

	if (!this->loadServerList())
	{
		DEF_LOG_ERROR("failed to load server list info");
		return -1;
	}

	if (!this->loadLanguageInfo())
	{
		DEF_LOG_ERROR("failed to load language info");
		return -1;
	}

	DEF_LOG_INFO("success to init server list");

	m_db_connection->disconnect();

	return 0;
}

bool ManageServerList::initDb()
{
	m_db_connection = new mysqlpp::Connection(false);
	if (!m_db_connection->connect(m_douwu_db_info.db_name.c_str(), m_douwu_db_info.ip_addr.c_str(), m_douwu_db_info.user_name.c_str(), m_douwu_db_info.passwd.c_str(), m_douwu_db_info.port))
	{
		DEF_LOG_ERROR("failed to connect to db <%s:%d--%s>, user pwd is <%s:%s>", m_douwu_db_info.ip_addr.c_str(), m_douwu_db_info.port, m_douwu_db_info.db_name.c_str(), m_douwu_db_info.user_name.c_str(), m_douwu_db_info.passwd.c_str());
		return false;
	}

	return true;
}

bool ManageServerList::loadServerList()
{
	mysqlpp::Query query = m_db_connection->query();
	query << "SELECT area_id, area_name, version_low, version_high, version_update_noti, notice_stop, notice_login, visible, avatar_path, resource_version, resource_url, area_index, is_match_area FROM dw_area";
	mysqlpp::StoreQueryResult res = query.store();
	if (!res)
	{
		DEF_LOG_ERROR("failed to load server list");
		return false;
	}

	for (mysqlpp::StoreQueryResult::const_iterator it = res.begin(); it != res.end(); ++it)
	{
		mysqlpp::Row row = *it;
		ServerInfo * server_info = new ServerInfo();
		server_info->area_id = atoi(row[0]);
		server_info->area_name = row[1].c_str();
		server_info->version_low = convertVersionToInt(row[2]);
		server_info->version_high = convertVersionToInt(row[3]);
		server_info->version_update_notify = convertVersionToInt(row[4]);
		server_info->notice_stop = row[5].c_str();
		server_info->notice_login = row[6].c_str();
		server_info->is_show = atoi(row[7]);
		server_info->avatar_path = row[8].c_str();
		server_info->resource_version = atoi(row[9]);
		server_info->resource_url = row[10].c_str();
		server_info->area_index = atoi(row[11]);
		server_info->is_match_area = atoi(row[12]);

		DEF_LOG_INFO("load server info, area id is <%d>, area name is <%s>, is show is <%d>", server_info->area_id, server_info->area_name.c_str(), server_info->is_show);

		m_server_map.insert(std::make_pair(server_info->area_id, server_info));
	}

	DEF_LOG_INFO("get server info list size is <%d>", m_server_map.size());

	return m_server_map.size() > 0;
}

bool ManageServerList::loadLanguageInfo()
{
	mysqlpp::Query query = m_db_connection->query();
	query << "SELECT area_id, area_name, language, login, stop FROM dw_notice";
	mysqlpp::StoreQueryResult res = query.store();
	if (!res)
	{
		DEF_LOG_ERROR("failed to load server language info");
		return false;
	}

	for (mysqlpp::StoreQueryResult::const_iterator it = res.begin(); it != res.end(); ++it)
	{
		mysqlpp::Row row = *it;
		LanguageInfo * info = new LanguageInfo();
		info->area_id = atoi(row[0]);
		info->area_name = row[1].c_str();
		info->language_type = atoi(row[2]);
		info->notice_login = row[3].c_str();
		info->notice_stop = row[4].c_str();

		int value = this->getLanguageValue(info->area_id, info->language_type);
		m_language_info_map.insert(std::make_pair(value, info));
	}

	DEF_LOG_INFO("get language size is <%d>", m_language_info_map.size());

	return true;
}


int ManageServerList::getLanguageValue(int area_id, int language_type)
{
	return area_id * 10000 + language_type;
}

LanguageInfo * ManageServerList::getLanguageInfo(int area_id, int language_type)
{
	int value = this->getLanguageValue(area_id, language_type);
	auto it = m_language_info_map.find(value);
	if (it != m_language_info_map.end())
	{
		return it->second;
	}

	return NULL;
}

void ManageServerList::vrIsOnline(SessionId session_id, int area_id, int gs_id, const string & gate_ip_addr, int gate_port)
{
	auto it = m_server_map.find(area_id);
	if (m_server_map.end() == it)
	{
		DEF_LOG_ERROR("failed to get area id info, area id is <%d>, gs id is <%d>, gate ip is <%s>, gate port is <%d>", area_id, gs_id, gate_ip_addr.c_str(), gate_port);
		return;
	}

	ServerInfo * server_info = it->second;
	auto gs_it = server_info->server_vr_map.find(gs_id);
	if (server_info->server_vr_map.end() == gs_it)
	{
		// new vr 
		ServerVrInfo * server_vr_info = new ServerVrInfo();
		server_vr_info->gs_id = gs_id;
		server_vr_info->gate_port = gate_port;
		server_vr_info->gate_ip_addr = gate_ip_addr;
		server_vr_info->session_id = session_id;

		server_info->server_vr_map[gs_id] = server_vr_info;

		DEF_LOG_INFO("get new vr register info, area id is <%d>, gs id is <%d>, gate ip is <%s>, gate port is <%d>", area_id, gs_id, gate_ip_addr.c_str(), gate_port);
	}
	else
	{
		// update vr info, should never happen
		ServerVrInfo * server_vr_info = gs_it->second;
		server_vr_info->gs_id = gs_id;
		server_vr_info->gate_port = gate_port;
		server_vr_info->gate_ip_addr = gate_ip_addr;
		server_vr_info->session_id = session_id;

		DEF_LOG_ERROR("should never happen, get new vr register info, area id is <%d>, gs id is <%d>, gate ip is <%s>, gate port is <%d>", area_id, gs_id, gate_ip_addr.c_str(), gate_port);
	}
}

void ManageServerList::vrIsOffline(int area_id, int gs_id)
{
	auto it = m_server_map.find(area_id);
	if (m_server_map.end() == it)
	{
		DEF_LOG_ERROR("failed to get area id info, area id is <%d>, gs id is <%d>", area_id, gs_id);
		return;
	}

	ServerInfo * server_info = it->second;
	auto gs_it = server_info->server_vr_map.find(gs_id);
	if (server_info->server_vr_map.end() == gs_it)
	{
		DEF_LOG_ERROR("failed to get vr info, area id is <%d>, gs id is <%d>", area_id, gs_id);
		return;
	}

	DEF_LOG_INFO("vr is off line, area id is <%d>, gs id is <%d>", area_id, gs_id);

	delete gs_it->second;
	server_info->server_vr_map.erase(gs_it);
}

ServerVrInfo * ManageServerList::GetVrServerByIndex(int area_id, int gs_index)
{
	ServerVrInfo * server = NULL;
	auto it = m_server_map.find(area_id);
	if (m_server_map.end() == it)
	{
		return NULL;
	}

	ServerInfo * server_info = it->second;
	auto gs_it = server_info->server_vr_map.find(gs_index);
	if (server_info->server_vr_map.end() == gs_it)
	{
		return NULL;
	}
	else
	{
		// update vr info, should never happen
		server = gs_it->second;
	}

	return server;
}

ServerVrInfo * ManageServerList::RandGetVrServer(int area_id)
{
	ServerVrInfo * server = NULL;
	auto it = m_server_map.find(area_id);
	if (m_server_map.end() == it)
	{
		return NULL;
	}

	ServerInfo * server_info = it->second;
	if (server_info->server_vr_map.empty())
	{
		return NULL;
	}
	else
	{
		server = server_info->server_vr_map.begin()->second;
	}

	return server;
}

string ManageServerList::makeUserSessionKey(const string & user_name)
{
	// todo
	return user_name + "_key";
}

ServerInfoMap_t & ManageServerList::getServerInfoMap()
{
	return m_server_map;
}
