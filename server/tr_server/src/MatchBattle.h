
#ifndef MATCH_BATTLE_HPP__
#define MATCH_BATTLE_HPP__

#include <unordered_map>
#include <stdint.h>
#include "MatchCell.h"

using namespace std;

struct BattleInfo;
class MatchBattle;

typedef unordered_map<uint64_t, MatchBattle *> MatchBattleMat_t;

class MatchBattle
{
public:
	MatchBattle();
	~MatchBattle();
public:
	void init(uint64_t match_id, BattleInfo * battle_info, const MatchCellVec_t & match_cell_vec);

	uint64_t getMatchId() const;

	BattleInfo * getBattleInfo() const;

	const MatchCellVec_t & getMatchCellVec() const;
protected:
private:
	uint64_t m_match_id;
	BattleInfo * m_battle_info;

	MatchCellVec_t m_match_cell_vec;
};

#endif
