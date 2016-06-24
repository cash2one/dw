
#ifndef BATTLE_RESULT_HPP__
#define BATTLE_RESULT_HPP__

#include <stdint.h>
#include <unordered_map>
#include "framesync_generated.h"
#include "Player.h"

using namespace std;
using namespace MobaGo::FlatBuffer;

struct PlayerBattleResultInfo 
{
	PlayerBattleResultInfo()
		: player(NULL)
		, report_win_team(0)
		, vr_recived_battle_result(false)
	{}

	Player * player;
	int report_win_team;
	bool vr_recived_battle_result;
};

typedef unordered_map<uint64_t, PlayerBattleResultInfo *> PlayerBattleResultInfoMap_t;

class BattleResult
{
public:
	BattleResult();
	~BattleResult();
public:
	int init(int one_team_member_number);
	void playerReportBattleResult(Player * player, const MsgReportBattleResultReq * msg);
	bool isBattleEnd();
	bool isBattleEndNotifyClient();
	void isBattleEndNotifyClient(bool is_notified);
	void vrReciveBattleReportNotifyBattle(Player * player);
	int getWinTeamId();

	PlayerBattleResultInfo * getPlayerBattleResultInfo(uint64_t user_id);
protected:
	bool checkBattleResult();
private:
	int m_one_team_member_number;
	bool m_is_battle_end;
	time_t m_report_battle_end_time;
	int m_win_team_id;
	bool m_is_battle_end_notified_client;

	PlayerBattleResultInfoMap_t m_player_battle_result_info_map;
};

#endif
