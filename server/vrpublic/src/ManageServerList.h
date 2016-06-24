
#ifndef MANAGE_SERVER_LIST_HPP__
#define MANAGE_SERVER_LIST_HPP__

#ifdef WIN32
#include <WinSock2.h>
#endif // WIN32


#include <mysql++/mysql++.h>
#include "VrPublicConfig.h"
#include "ServerInfo.h"
#include "Packet_.h"

class ManageServerList
{
public:
	ManageServerList();
	~ManageServerList();
public:
	static ManageServerList * instance();

	int init(const DbInfo & douwu_db_info);

	void vrIsOnline(SessionId session_id, int area_id, int gs_id, const string & gate_ip_addr, int gate_port);

	void vrIsOffline(int area_id, int gs_id);

	ServerVrInfo * GetVrServerByIndex(int area_id, int gs_index);
	ServerVrInfo * RandGetVrServer(int area_id);
	string makeUserSessionKey(const string & user_name);
protected:
	bool initDb();

	bool loadServerList();

	bool loadLanguageInfo();

	int getLanguageValue(int area_id, int language_type);

	LanguageInfo * getLanguageInfo(int area_id, int language_type);


public:
	ServerInfoMap_t & getServerInfoMap();

private:
	mysqlpp::Connection * m_db_connection;

	DbInfo m_douwu_db_info;

	ServerInfoMap_t m_server_map;

	LanguageInfoMap_t m_language_info_map;

	static ManageServerList * m_instance;
};

#endif
