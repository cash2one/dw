

#include "PlayerManager.h"
#include "Logger.h"
#include "BattleLog.h"

#define ADD_PLAYER(add_key, player, player_map)	\
	auto it = player_map.find(add_key);	\
	if (player_map.end() == it)	\
	{	\
		player_map.insert(std::make_pair(add_key, player));	\
		return;	\
	}	\
	BATTLE_LOG_ERROR("add reduplicate player by <%s>, name is <%s>, id is <%llu>, session is <%x>", #add_key, player->getName().c_str(), player->getId(), player->getSession());

#define REMOVE_PLAYER(remove_key, player, player_map)	\
	auto it = player_map.find(remove_key);	\
	if (player_map.end() != it)	\
	{	\
		player_map.erase(it);	\
		return;	\
	}	\
	BATTLE_LOG_ERROR("not find player while remove player by <%s>, name is <%s>, id is <%d>, session is <%x>", #remove_key, player->getName().c_str(), player->getId(), player->getSession());

#define SEARCH_PLAYER_BY_STR(search_key, player_map)	\
	auto it = player_map.find(search_key);	\
	if (it != player_map.end())	\
	{ \
		return it->second;	\
	}	\
	if (report_error)	\
		BATTLE_LOG_ERROR("search player by name <%s>, but not find", search_key.c_str());	\
	return NULL;

#define SEARCH_PLAYER_BY_NUM(search_key, player_map)	\
	auto it = player_map.find(search_key);	\
	if (it != player_map.end())	\
	{ \
		return it->second;	\
	}	\
	BATTLE_LOG_ERROR("search player by <%s> <%llu>, but not find", #search_key, search_key);	\
	return NULL;

PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{

}

void PlayerManager::addPlayer(Player * player)
{
	//this->addPlayerBySession(player);
	this->addPlayerById(player);
	//this->addPlayerByAccount(player);
}

void PlayerManager::removePlayer(Player * player)
{
	this->removePlayerById(player);
	this->removePlayerBySession(player);
}

void PlayerManager::connectPlayerSession(Player * player, uint64_t session)
{
	player->setSession(session);
	this->addPlayerBySession(player);
}
	
Player * PlayerManager::getPlayerBySession(uint64_t session_id, bool report_error)
{
	auto it = m_player_session_map.find(session_id);
	if (it != m_player_session_map.end())
	{
		return it->second;
	}
	if (report_error)
	{
		BATTLE_LOG_ERROR("search player by <sesison id> <%x>, but not find", session_id);
	}

	return NULL;
}

//Player * PlayerManager::getPlayerByName(const string & name, bool report_error)
//{
//	SEARCH_PLAYER_BY_STR(name, this->m_player_account_map);
//}

Player * PlayerManager::getPlayerById(PlayerId_t player_id)
{
	SEARCH_PLAYER_BY_NUM(player_id, this->m_player_id_map);
}

void PlayerManager::changePlayerSession(Player * player, uint64_t session_id)
{
	//player->setSession(session_id);
	//this->addPlayerBySession(player);
}

void PlayerManager::addPlayerBySession(Player * player)
{
	ADD_PLAYER(player->getSession(), player, m_player_session_map);

	//PlayerSessionMap_t::iterator it = m_player_session_map.find(player->m_session_id);
	//if (it != m_player_session_map.end())
	//{
	//	m_player_session_map.insert(std::make_pair(player->m_session_id, player));
	//	return;
	//}

	//BATTLE_LOG_ERROR("add reduplicate player by session, name is <%s>, id is <%d>, session is <%d>", player->m_player_name.c_str(), player->m_player_id, player->m_session_id);
}

void PlayerManager::addPlayerById(Player * player)
{
	ADD_PLAYER(player->getId(), player, m_player_id_map);
}

//void PlayerManager::addPlayerByAccount(Player * player)
//{
//	ADD_PLAYER(player->getName(), player, m_player_account_map);
//}

//void PlayerManager::removePlayerByName(Player * player)
//{
//	REMOVE_PLAYER(player->getName(), player, m_player_account_map);
//}

void PlayerManager::removePlayerBySession(Player * player)
{
	PlayerSessionMap_t::iterator it = m_player_session_map.find(player->getSession());
	if (it != m_player_session_map.end())
	{
		m_player_session_map.erase(it);
	}
	else
	{
		if (!player->isOffline())
		{
			BATTLE_LOG_ERROR("not find while remove player by session, name <%s> player id <%d> , sesseion <%x>", player->getName().c_str(), player->getId(), player->getSession());
		}
	}
}

bool PlayerManager::isInBattle(PlayerId_t player_id)
{
	// todo
	return false;
}

void PlayerManager::removePlayerById(Player * player)
{
	REMOVE_PLAYER(player->getId(), player, m_player_id_map);
}


