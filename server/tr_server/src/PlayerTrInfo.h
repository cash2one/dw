
#ifndef PLAYER_TR_INFO_HPP__
#define PLAYER_TR_INFO_HPP__

#include <vector>
#include <stdint.h>
#include "inter_proto.pb.h"
#include "SessionHandle.h"

using namespace std;

enum PlayerTrState
{
	PTS_base = 0,
	PTS_matching,
	PTS_is_confirming,
	PTS_is_confirmed,
	PTS_in_battle,
};

enum PlayerTrEvent
{
	PTE_base = 0,
	PTE_request_match,
	PTE_cancel_match,
	PTE_match_success,
	PTE_match_confirmed,
	PTE_all_confirmed,
	PTE_battle_finish,
	PTE_online,
	PTE_offline,
};

class MatchCell;

struct MatchInfo 
{
	MatchInfo()
		: team_type(0)
		, match_type(0)
	{}

	int team_type;
	int match_type;
	vector<uint64_t> team_member;
};

class PlayerTrInfo
{
public:
	PlayerTrInfo();
	~PlayerTrInfo();
public:
	void setAccountId(uint64_t account_id);
	uint64_t getAccountId();
	Proto::Player & getPlayer();

	PlayerTrState getState();
	void setState(PlayerTrState player_state);

	void setMatchCell(MatchCell * match_cell);
	MatchCell * getMatchCell();

	void setSessionId(SessionId session_id);
	SessionId getSessionId();

	void setBattleSession(SessionId session_id);
	SessionId getBattleSession();

	MatchInfo & getMatchInfo();
protected:
private:
	uint64_t m_account_id;
	PlayerTrState m_player_state;
	MatchCell * m_match_cell;
	SessionId m_session_id;		// vr session
	SessionId m_battle_session;
	Proto::Player m_player;
	MatchInfo m_match_info;
};

#endif

