
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

Summoner * ManageSummoner::findSummoner(uint64_t sum_id)
{
	auto it = map_summoner.find(sum_id);
	if (it != map_summoner.end())
	{
		return it->second;
	}

	return NULL;

}
void ManageSummoner::delSummoner(uint64_t sum_id)
{
	auto it = map_summoner.find(sum_id);
	if (it != map_summoner.end())
	{
		delete it->second;
		map_summoner.erase(it);
	}
}

void ManageSummoner::addSummoner(uint64_t sum_id, Summoner * pSummoner)
{
	auto it = map_summoner.find(sum_id);
	if (it != map_summoner.end())
	{
		it->second->setStatus(pSummoner->sum_status);
		return;
	}

	map_summoner[sum_id] = pSummoner;
}