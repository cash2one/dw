
#ifndef MANAGE_ACCOUNT_HPP__
#define MANAGE_ACCOUNT_HPP__

#ifdef WIN32
#include <WinSock2.h>
#endif // WIN32

#include <string>
#include <mysql++/mysql++.h>
#include "VrPublicConfig.h"
#include "account.pb.h"
#include "cache_auto_assistant.h"
#include "Packet_.h"
#include "Account.h"
#include "login_generated.h"
#include "ManageDbPing.h"

using namespace std;



class ManageAccount
{
public:
	typedef map<uint64_t, Account *> MAP_ACCOUNT;

public:
	ManageAccount();
	~ManageAccount();
public:
	
	static ManageAccount * GetInstance();

	int init(const DbInfo & data_center_cfg);

	void update();

	void accountSetClientInfo(uint64_t client_id, session_t session_id);

	void makeUserName(int platform_id, int channel_id, const string & account, string & user_name);

	Account * queryAccountByName(const string & user_name, int channel_type);

	Account * createAccount(int byLang, const string & account_name, const string & password, const string & device_name, MobaGo::FlatBuffer::Login login_type, int channel);

	bool getAccountServerInfoList(MyGuid_t guid, AccountServerInfoList_t & server_info_list);

	void updateLastLoginServer(MyGuid_t guid);

	void addNewServerInfo(MyGuid_t user_guid, int area_id, int gs_id);

	// account¥¶¿Ì
	Account * addAccount(uint64_t account_id, session_t session_id, Account * pAccount);
	void delAccount(uint64_t account_id);
	Account *findAccount(uint64_t account_id);

	void removeAccountByOffline(uint64_t gate_guid);

protected:
public:
	static ManageAccount * m_instance;

private:
	mysqlpp::Connection * m_db_connection;
	DbInfo m_data_center_cfg;

	MAP_ACCOUNT map_account;

	ManageDbPing m_manage_db_ping;
};

#endif
