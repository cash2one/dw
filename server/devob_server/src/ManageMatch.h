

#ifndef MANAGE_MATCH_HPP__
#define MANAGE_MATCH_HPP__

#include <unordered_map>
#include "MyGuidDef.h"
#include "Packet_.h"
#include "MatchCell.h"

struct BattleInfo;

using namespace std;

typedef unordered_map<MyGuid_t, MatchCell *> MatchCellMap_t;

class ManageMatch
{
public:
	ManageMatch();
	~ManageMatch();
	//static ManageMatch * instance();
public:
	int init(int one_team_member_num);

	int update();

	void handleMatch(MyGuid_t user_guid, SessionId session_id);

	void handleMatchCancel(MyGuid_t user_guid);
protected:
	void addMatch(MyGuid_t user_guid, SessionId session_id);

	void removeMatch(MyGuid_t user_guid);

	void matchSuccess(MatchCellVec_t & match_cell_vec, BattleInfo * battle_info);

	void notifyPlayerEnterBattle(MatchCell * match_cell, const string & server_ip, int server_port);

	void notifyBattleMatchResult(MatchCellVec_t & match_cell_vec, BattleInfo * battle_info);
private:
	int m_one_team_member_number;
	MatchCellMap_t m_match_cell_map;
};

#endif
