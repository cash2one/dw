
#include "ManageSummoner.h"

ManageSummoner * ManageSummoner::m_instance = NULL;

ManageSummoner::ManageSummoner()
{
}

ManageSummoner::~ManageSummoner()
{
	auto it = map_summoner.begin();
	for (; it != map_summoner.end(); ++it)
	{
		delete it->second;
	}
	map_summoner.clear();

	if (m_instance != NULL)
	{
		delete m_instance;
	}
}

ManageSummoner * ManageSummoner::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new ManageSummoner();
	}

	return m_instance;
}

Summoner * ManageSummoner::findSummoner(uint64_t sum_id, bool add_to_mem)
{
	MAP_SUMMONER::iterator itFind = map_summoner.find(sum_id);
	if (itFind != map_summoner.end())
	{
		return itFind->second;
	}
	if (!add_to_mem)
	{
		return NULL;
	}

	//从数据库读表
	Summoner * pSummoner = DBLoadSumByID(sum_id);
	if (!pSummoner)
	{
		return NULL;
	}

	return pSummoner;
}

Summoner * ManageSummoner::DBLoadSumByID(uint64_t sum_id)
{
	mysqlpp::Connection * m_db_connection = ManageDB::GetInstance()->getMysqlConnection();
	mysqlpp::Query query = m_db_connection->query();
	query << "SELECT guid, account_id, sum_name FROM summoner where guid=";
	query << sum_id;
	query << " limit 1";

	const ::mysqlpp::StoreQueryResult& res = query.store();

	if (res.size() == 0)
	{
		return NULL;
	}

	Summoner * pSummoner = new Summoner();

	mysqlpp::Row row = *res.begin();
	pSummoner->m_nId = atoll(row[0]);
	pSummoner->m_strSumName = row[2].c_str();

	map_summoner.insert(make_pair(pSummoner->m_nId, pSummoner));
	
	return pSummoner;
}

void ManageSummoner::addMemSummonerById(uint64_t sum_id, Summoner * pSummoner)
{
	auto it = map_summoner.find(sum_id);
	if (it != map_summoner.end())
	{
		if (it->second != pSummoner)
		{
			delete it->second;
			it->second = pSummoner;
		}
		return;
	}

	map_summoner[sum_id] = pSummoner;
}
void ManageSummoner::addFriend(Summoner * pSummoner, STC_FRIEND_INFO & stcFriendInfo)
{
	if (pSummoner == NULL)
	{
		return;
	}
	auto it = pSummoner->map_friend.find(stcFriendInfo.m_nId);
	if (it == pSummoner->map_friend.end())
	{
		pSummoner->map_friend.insert(make_pair(stcFriendInfo.m_nId, stcFriendInfo));
		// to do list confirm_add, add
	}
}

