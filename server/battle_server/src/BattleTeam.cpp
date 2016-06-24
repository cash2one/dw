
#include "BattleTeam.h"

BattleTeam::BattleTeam()
	: m_one_team_number(0)
{

}

BattleTeam::~BattleTeam()
{

}

int BattleTeam::init(int one_team_number)
{
	m_one_team_number = one_team_number;

	return 0;
}

int BattleTeam::addPlayer(Player * player)
{
	if (0 != m_one_team_number && m_all_team_member.size() >= (uint32)m_one_team_number * 2)
	{
		return -1;
	}

	PlayerMap_t::iterator it = m_all_team_member.find(player->getId());
	if (it != m_all_team_member.end())
	{
		// error
		return -1;
	}

	m_all_team_member[player->getId()] = player;

	if (m_first_team.size() <= m_second_team.size())
	{
		m_first_team[player->getId()] = player;
		player->setTeamId(1);
	}
	else
	{
		m_second_team[player->getId()] = player;
		player->setTeamId(2);
	}

	return 0;
}

int BattleTeam::removePlayer(Player * player)
{
	PlayerMap_t::iterator it = m_all_team_member.find(player->getId());
	if (m_all_team_member.end() == it)
	{
		return -1;
	}

	m_all_team_member.erase(it);

	PlayerMap_t * player_team = getPlayerTeam(player);
	if (NULL == player_team)
	{
		return -1;
	}

	PlayerMap_t::iterator sub_it = player_team->find(player->getId());
	if (sub_it != player_team->end())
	{
		player_team->erase(sub_it);
	}

	return 0;
}

PlayerMap_t * BattleTeam::getPlayerTeam(Player * player)
{
	if (1 == player->getTeamId())
	{
		return &m_first_team;
	}
	else if (2 == player->getTeamId())
	{
		return &m_second_team;
	}

	return NULL;
}

int BattleTeam::getAllMemberNumber()
{
	return m_all_team_member.size();
}

int BattleTeam::getAllMemberReadyNumber()
{
	int ret = 0;
	for (PlayerMap_t::iterator it = m_all_team_member.begin(); it != m_all_team_member.end(); ++it)
	{
		Player * player = it->second;
		if (player->getStatus() == BPS_READY)
		{
			++ret;
		}
	}

	return ret;
}

bool BattleTeam::isAllReady()
{
	bool is_all_ready = true;
	for (PlayerMap_t::iterator it = m_all_team_member.begin(); it != m_all_team_member.end(); ++it)
	{
		Player * player = it->second;
		if (player->getStatus() != BPS_READY)
		{
			is_all_ready = false;
			break;
		}
	}

	return is_all_ready;
}

void BattleTeam::startBattle()
{
}

PlayerMap_t BattleTeam::getTeamById(int team_id)
{
	if (1 == team_id)
	{
		return m_first_team;
	}
	else if (2 == team_id)
	{
		return m_second_team;
	}
}

