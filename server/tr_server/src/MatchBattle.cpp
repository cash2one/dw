
#include "MatchBattle.h"

MatchBattle::MatchBattle()
	: m_match_id(0)
	, m_battle_info(NULL)
{

}

MatchBattle::~MatchBattle()
{

}

void MatchBattle::init(uint64_t match_id, BattleInfo * battle_info, const MatchCellVec_t & match_cell_vec)
{
	m_match_id = match_id;
	m_battle_info = battle_info;
	m_match_cell_vec = match_cell_vec;
}

uint64_t MatchBattle::getMatchId() const
{
	return m_match_id;
}

BattleInfo * MatchBattle::getBattleInfo() const
{
	return m_battle_info;
}

const MatchCellVec_t & MatchBattle::getMatchCellVec() const
{
	return m_match_cell_vec;
}

