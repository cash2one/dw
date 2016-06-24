
#ifndef MATCH_QUEUE_HPP__
#define MATCH_QUEUE_HPP__

#include <unordered_map>
#include "MyGuidDef.h"
#include "Packet_.h"
#include "MatchCell.h"
#include "MatchQueue.h"
#include "MatchBattle.h"

struct BattleInfo;
class MatchQueue;

using namespace std;

typedef unordered_map<MyGuid_t, MatchCell *> MatchCellMap_t;
typedef unordered_map<int, MatchQueue *> MatchQueueMap_t;

class MatchQueue
{
public:
	MatchQueue();
	~MatchQueue();
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

	void notifyBattleMatchResult(MatchCellVec_t & match_cell_vec, MatchBattle * match_battle, BattleInfo * battle_info);
private:
	int m_one_team_member_number;
	MatchCellMap_t m_match_cell_map;
};

#endif

