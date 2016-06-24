

#include "PlayerTrInfo.h"

PlayerTrInfo::PlayerTrInfo()
	: m_account_id(0)
	, m_player_state(PTS_base)
	, m_match_cell(NULL)
	, m_session_id(NULL)
	, m_battle_session(NULL)
{

}

PlayerTrInfo::~PlayerTrInfo()
{

}

void PlayerTrInfo::setAccountId(uint64_t account_id)
{
	m_account_id = account_id;
}

uint64_t PlayerTrInfo::getAccountId()
{
	return m_account_id;
}

Proto::Player & PlayerTrInfo::getPlayer()
{
	return m_player;
}

PlayerTrState PlayerTrInfo::getState()
{
	return m_player_state;
}

void PlayerTrInfo::setState(PlayerTrState player_state)
{
	m_player_state = player_state;
}

void PlayerTrInfo::setMatchCell(MatchCell * match_cell)
{
	m_match_cell = match_cell;
}

MatchCell * PlayerTrInfo::getMatchCell()
{
	return m_match_cell;
}

void PlayerTrInfo::setSessionId(SessionId session_id)
{
	m_session_id = session_id;
}

SessionId PlayerTrInfo::getSessionId()
{
	return m_session_id;
}

void PlayerTrInfo::setBattleSession(SessionId session_id)
{
	m_battle_session = session_id;
}

SessionId PlayerTrInfo::getBattleSession()
{
	return m_battle_session;
}

MatchInfo & PlayerTrInfo::getMatchInfo()
{
	return m_match_info;
}

