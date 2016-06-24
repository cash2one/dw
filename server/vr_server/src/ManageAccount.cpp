#include "Logger.h"
#include "ManageAccount.h"
#include "ManageDB.h"

ManageAccount * ManageAccount::m_instance = NULL;

ManageAccount::ManageAccount()
{
}

ManageAccount::~ManageAccount()
{
	auto it = map_account.begin();
	for (; it != map_account.end(); ++it)
	{
		delete it->second;
	}
	map_account.clear();

	if (m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
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

bool ManageAccount::checkSessionCode(MyGuid_t account_id, string session_code)
{
	MAP_VERIFYING_ACCOUNT::iterator itFind = map_verifying_account.find(account_id);
	if (itFind == map_verifying_account.end())
	{
		return false;
	}

	if (itFind->second.session_code == session_code)
	{
		return true;
	}
	
	return true;
}

// 
bool ManageAccount::newSessionCode(MyGuid_t account_id, string account_name, string session_code)
{
	MAP_VERIFYING_ACCOUNT::iterator itFind = map_verifying_account.find(account_id);
	if (itFind != map_verifying_account.end())
	{
		itFind->second.account_id = account_id;
		itFind->second.account_name = account_name;
		itFind->second.session_code = session_code;

		return true;
	}

	VERIFYING_ACCOUNT_INFO & account = map_verifying_account[account_id];
	account.account_id = account_id;
	account.account_name = account_name;
	account.session_code = session_code;

	return true;
}

uint64_t ManageAccount::querySumIdByAccountId(uint64_t account_id)
{
	uint64_t sum_id = 0;

	mysqlpp::Connection * m_db_connection = ManageDB::GetInstance()->getMysqlConnection();
	mysqlpp::Query query = m_db_connection->query();
	query << "SELECT guid, account_id FROM summoner where account_id=";
	query << account_id;

	const ::mysqlpp::StoreQueryResult& res = query.store();

	if (res.size() == 0)
	{
		return sum_id;
	}

	mysqlpp::Row row = *res.begin();
	sum_id = atoll(row[0]);

	return sum_id;
}

// account处理
Account * ManageAccount::addAccount(uint64_t client_id, Account *account)
{
	Account *temp;
	temp = findAccount(client_id);
	if (temp != NULL)
	{
		// 账号已存在
		return temp;
	}

	if (account != NULL)
	{
		DEF_LOG_DEBUG("add new account , account id is <%llu>, client id is <%llu>", account->account_id, account->client_id);
		map_account[client_id] = account;
	}

	return account;
}

void ManageAccount::delAccount(uint64_t client_id)
{
	Account *account;
	account = findAccount(client_id);
	if (account != NULL)
	{
		DEF_LOG_DEBUG("delete account, account is <%llu>, client id is <%llu>", account->account_id, account->client_id);
		delete account;
	}
	map_account.erase(client_id);
}

Account * ManageAccount::findAccount(uint64_t client_id)
{
	map<uint64_t, Account *>::iterator itFind = map_account.find(client_id);
	if (itFind != map_account.end())
	{
		return itFind->second;
	}

	return NULL;
}

Account * ManageAccount::getAccount(uint64_t account_id)
{
	// todo
	for (auto it : map_account)
	{
		if (account_id == it.second->account_id)
		{
			return it.second;
		}
	}

	return NULL;
}

