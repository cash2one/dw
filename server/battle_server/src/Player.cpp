
#include "Player.h"

//int ONE_TEAM_MEMBER_NUMBER = 5;

Player::Player()
	: m_session_id(0)
	, m_player_id(0)
	, m_gate_guid(0)
	, m_hero_id(0)
	, m_team_id(0)
	, m_player_battle(NULL)
	, m_battle_player_status(BPS_PREPARE)
	, m_is_confirmed_hero(false)
{

}

Player::~Player()
{

}

int Player::init(uint64_t session_id, PlayerId_t player_id, uint64_t gate_guid, int hero_id, const string & name, Battle * battle)
{
	m_session_id = session_id;
	m_player_id = player_id;
	m_gate_guid = gate_guid;
	m_hero_id = hero_id;
	m_player_name = name;
	m_player_battle = battle;

	//m_player_reconnect_info.player_status = PS_ONLINE;

	return 0;
}

void Player::setGateGuid(uint64_t gate_guid)
{
	m_gate_guid = gate_guid;
}

uint64_t Player::getGateGuid()
{
	return m_gate_guid;
}

int Player::playerClientLoading()
{
	m_battle_player_status = BPS_CLIENT_LOADING;
	return 0;
}

int Player::playerClientReady()
{
	m_battle_player_status = BPS_READY;
	return 0;
}

int Player::playerEnterBattle()
{
	m_battle_player_status = BPS_START_BATTLE;

	return 0;
}

void Player::setSession(uint64_t sessiond_id)
{
	m_session_id = sessiond_id;
}

Battle * Player::getBattle()
{
	return m_player_battle;
}

BattlePlayerStatus & Player::getStatus()
{
	return m_battle_player_status;
}

PlayerReconnectInfo & Player::getReconnectInfo()
{
	return m_player_reconnect_info;
}

uint64_t Player::getSession()
{
	return m_session_id;
}

PlayerId_t Player::getId()
{
	return m_player_id;
}

int Player::getTeamId()
{
	return m_team_id;
}

int Player::getHeroId()
{
	return m_hero_id;
}

void Player::randomHeroForBattle()
{
	// todo
	m_hero_id = 704;
}

const string & Player::getName()
{
	return m_player_name;
}

void Player::setTeamId(int team_id)
{
	m_team_id = team_id;
}

void Player::enterBattle()
{
	m_player_reconnect_info.player_status = PS_ONLINE;
}

bool Player::isOffline()
{
	return PS_OFFLINE == m_player_reconnect_info.player_status;
}

void Player::setHeroId(int hero_id)
{
	m_hero_id = hero_id;
}

void Player::confirmHero(int hero_id)
{
	this->setHeroId(hero_id);

	m_is_confirmed_hero = true;
}

bool Player::isConfirmedHero()
{
	return m_is_confirmed_hero;
}

