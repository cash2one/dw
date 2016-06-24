
#include "Logger.h"
#include "ManagePlayerTrInfo.h"

ManagePlayerTrInfo::ManagePlayerTrInfo()
{

}

ManagePlayerTrInfo::~ManagePlayerTrInfo()
{

}

ManagePlayerTrInfo * ManagePlayerTrInfo::instance()
{
	return Singleton<ManagePlayerTrInfo>::instance();
}

PlayerTrInfo * ManagePlayerTrInfo::getPlayer(uint64_t account_id)
{
	auto it = m_player_info_map.find(account_id);
	if (it != m_player_info_map.end())
	{
		return it->second;
	}

	return NULL;
}

void ManagePlayerTrInfo::addPlayer(PlayerTrInfo * player)
{
	auto it = m_player_info_map.find(player->getAccountId());
	if (it != m_player_info_map.end())
	{
		DEF_LOG_ERROR("add reduplication player <%llu>", player->getAccountId());
		delete player;
		return;
	}

	m_player_info_map.insert(std::make_pair(player->getAccountId(), player));
}


