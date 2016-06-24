
#include <boost/lexical_cast.hpp>
#include "Logger.h"
#include "ManageAccount.h"
#include "MakeGuid.h"
#include "cache_auto_assistant.h"
#include "Logger.h"
#include "MyGuidDef.h"

ManageAccount * ManageAccount::m_instance = NULL;

ManageAccount::ManageAccount()
	: m_db_connection(NULL)
{

}

ManageAccount::~ManageAccount()
{
	if (NULL != m_db_connection)
	{
		delete m_db_connection;
		m_db_connection = NULL;
	}
}

ManageAccount * ManageAccount::GetInstance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageAccount();
	}

	return m_instance;
}

int ManageAccount::init(const DbInfo & data_center_cfg)
{
	DEF_LOG_INFO("init manage account, db ip is <%s>, port is <%d>, db name is <%s>, user name is <%s>", data_center_cfg.ip_addr.c_str(), data_center_cfg.port, data_center_cfg.db_name.c_str(), data_center_cfg.user_name.c_str());

	m_data_center_cfg = data_center_cfg;

	m_db_connection = new mysqlpp::Connection(false);
	if (!m_db_connection->connect(m_data_center_cfg.db_name.c_str(), m_data_center_cfg.ip_addr.c_str(), m_data_center_cfg.user_name.c_str(), m_data_center_cfg.passwd.c_str(), m_data_center_cfg.port))
	{
		DEF_LOG_ERROR("failed to connect data center database <%s:%p>, <%s:%s>", m_data_center_cfg.ip_addr.c_str(), m_data_center_cfg.port, m_data_center_cfg.user_name.c_str(), m_data_center_cfg.passwd.c_str());
		return -1;
	}

	DEF_LOG_INFO("success init manage account");

	m_manage_db_ping.init(m_db_connection, ManageDbPing::DbInfo(m_data_center_cfg.port, m_data_center_cfg.ip_addr, m_data_center_cfg.db_name, m_data_center_cfg.user_name, m_data_center_cfg.passwd));

	return 0;
}

void ManageAccount::update()
{
	m_manage_db_ping.update();
}

void ManageAccount::accountSetClientInfo(uint64_t client_id, session_t session_id)
{
	MAP_ACCOUNT::iterator it = map_account.find(client_id);
	if (it != map_account.end())
	{
		it->second->client_id = client_id;
		it->second->session_id = session_id;
	}
}

void ManageAccount::makeUserName(int platform_id, int channel_id, const string & account, string & user_name)
{
	user_name.resize(100);
	sprintf((char *)user_name.data(), "%d_%d_%s", platform_id, channel_id, account.c_str());
}

Account * ManageAccount::queryAccountByName(const string & user_name, int channel_type)
{
	mysqlpp::Query query = m_db_connection->query();
	query << "SELECT guid, user_name, sum_id FROM account where user_name=";
	query << "'";
	query << user_name.c_str();
	query << "'";

	const ::mysqlpp::StoreQueryResult& res = query.store();
	//if (!res)
	//{
	//	DEF_LOG_ERROR("failed to query account");
	//	return NULL;
	//}

	if (res.size() == 0)
	{
		return NULL;
	}

	Account * ret = new Account();

	mysqlpp::Row row = *res.begin();

	ret->guid = atoll(row[0]);
	ret->user_name = row[1].c_str();
	ret->summoner_id = atoll(row[2]);
	ret->channel_type = channel_type;

	return ret;
}

Account * ManageAccount::createAccount(int byLang, const string & account_name, const string & password, const string & device_name, MobaGo::FlatBuffer::Login login_type, int channel)
{
	if (login_type != Login_Account && login_type != Login_Device)
	{
		return NULL;
	}

	Account * ret = new Account();

	MyGuid_t guid = 0;
	MyGuid_t summoner_id = 0;
	if (!MakeGuid::instance()->get_guid(ET_PLAYER, guid))
	{
		DEF_LOG_ERROR("failed to make guid");
		delete ret;
		return NULL;
	}

	summoner_id = SWITCH_GUID_TYPE(ET_SUMMONER, guid);

	DEF_LOG_DEBUG("new account guid is <%llu>, summoner guid is <%llu>", guid, summoner_id);

	ret->guid = guid;
	ret->user_name = device_name;
	ret->summoner_id = summoner_id;

	dbass::Assaccount ass_account(guid);
	Entity::account * entity_account = (Entity::account *)ass_account.data();
	entity_account->set_user_name(device_name);
	entity_account->set_channel_type(channel);
	entity_account->set_login_type(login_type);
	entity_account->set_sum_id(summoner_id);

	time_t curr_time = getSystemMilliSeconds();
	string time_str;
	formatTimeToString(curr_time, time_str);

	entity_account->set_create_tim(time_str);
	mysqlpp::Query query = m_db_connection->query();

	ass_account.insert(query);

	return ret;
}

bool ManageAccount::getAccountServerInfoList(MyGuid_t guid, AccountServerInfoList_t & server_info_list)
{
	mysqlpp::Query query = m_db_connection->query();
	query << "SELECT guid, area_id, gs_id, user_id, last_enter_time FROM role_server WHERE user_id=";
	query << guid;

	const ::mysqlpp::StoreQueryResult& res = query.store();
	if (!res)
	{
		DEF_LOG_ERROR("failed to load player server info list");
		return false;
	}

	for (mysqlpp::StoreQueryResult::const_iterator it = res.begin(); it != res.end(); ++it)
	{
		mysqlpp::Row row = *it;
		AccountServerInfo * server_info = new AccountServerInfo();
		server_info->area_id = atoi(row[1]);
		server_info->gs_index = atoi(row[2]);
		server_info->last_enter_time = atoll(row[4]);

		server_info_list.push_back(server_info);
	}

	return true;
}

void ManageAccount::updateLastLoginServer(MyGuid_t guid)
{
	time_t curr_time = getSystemMilliSeconds();
	string time_str;
	formatTimeToString(curr_time, time_str);

	mysqlpp::Query query = m_db_connection->query();
	query << "UPDATE role_server SET last_enter_time=";
	query << "'" << time_str.c_str() << "'";
	query << " WHERE guid=" << guid;

	query.store();
}

void ManageAccount::addNewServerInfo(MyGuid_t user_guid, int area_id, int gs_id)
{
	time_t curr_time = getSystemMilliSeconds();
	string time_str;
	formatTimeToString(curr_time, time_str);

	uint64_t guid_value;
	MakeGuid::instance()->get_guid(ET_PLAYER_SERVER, guid_value);

	dbass::Assrole_server ass_role_server(guid_value);

	Entity::role_server * role_server = (Entity::role_server *)ass_role_server.data();
	role_server->set_area_id(area_id);
	role_server->set_gs_id(gs_id);
	role_server->set_user_id(user_guid);
	role_server->set_first_enter_time(time_str);
	role_server->set_last_enter_time(time_str);

	mysqlpp::Query query = m_db_connection->query();
	ass_role_server.insert(query);

	query.store();

	//query << "INSERT INTO role_server SET ";
	//query << " guid=" << guid_value;
	//query << ",";
	//query << " area_id=" << area_id;
	//query << ",";
	//query << " gs_id=" << gs_id;
	//query << ",";
	//query << " user_id=" << user_guid;
	//query << ",";
	//query << "first_enter_time=" << ::mysqlpp::quote << time_str;
	//query << ",";
	//query << "last_enter_time=" << ::mysqlpp::quote << time_str;

	//query.store();
}

// account处理
Account * ManageAccount::addAccount(uint64_t client_it, session_t session_id, Account * pAccount)
{
	Account *account;
	account = findAccount(client_it);
	if (account != NULL)
	{
		// 账号已存在
		account->guid = pAccount->guid;
		account->user_name = pAccount->user_name;
		account->language_type = pAccount->language_type;
		account->session_id = session_id;
		return account;
	}

	map_account[client_it] = pAccount;
	pAccount->client_id = client_it;
	pAccount->session_id = session_id;

	return pAccount;
}

void ManageAccount::delAccount(uint64_t client_it)
{
	Account *account;
	account = findAccount(client_it);
	if (account != NULL)
	{
		DEF_LOG_DEBUG("start to remove account, gate guid is <%llu>, account id is <%llu>", client_it, account->guid);
		delete account;
	}
	else
	{
		DEF_LOG_ERROR("do not find account where remove it, gate guid is <%llu>", client_it);
	}

	map_account.erase(client_it);
}

Account *ManageAccount::findAccount(uint64_t client_it)
{
	MAP_ACCOUNT::iterator itFind = map_account.find(client_it);
	if (itFind != map_account.end())
	{
		return itFind->second;
	}

	return NULL;
}

void ManageAccount::removeAccountByOffline(uint64_t gate_guid)
{
	this->delAccount(gate_guid);
}

