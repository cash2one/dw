

#include "Logger.h"
#include "BattleResult.h"
#include "check_macro.h"

BattleResult::BattleResult()
	: m_one_team_member_number(0)
	, m_is_battle_end(false)
	, m_win_team_id(1)
	, m_is_battle_end_notified_client(false)
{

}

BattleResult::~BattleResult()
{

}

int BattleResult::init(int one_team_member_number)
{
	m_one_team_member_number = one_team_member_number;

	return 0;
}

void BattleResult::playerReportBattleResult(Player * player, const MsgReportBattleResultReq * msg)
{
	auto it = m_player_battle_result_info_map.find(player->getId());
	if (m_player_battle_result_info_map.end() != it)
	{
		DEF_LOG_ERROR("get reduplication battle result, player is <%llu>", player->getId());
		return;
	}

	// todo
	m_win_team_id = msg->win_team();

	PlayerBattleResultInfo * battle_result_info = new PlayerBattleResultInfo();
	battle_result_info->player = player;
	battle_result_info->report_win_team = msg->win_team();

	m_report_battle_end_time = getSystemMilliSeconds();

	m_player_battle_result_info_map.insert(std::make_pair(player->getId(), battle_result_info));

	m_is_battle_end = checkBattleResult();
}

bool BattleResult::isBattleEnd()
{
	return m_is_battle_end;
}

bool BattleResult::isBattleEndNotifyClient()
{
	return m_is_battle_end_notified_client;
}

void BattleResult::isBattleEndNotifyClient(bool is_notified)
{
	m_is_battle_end_notified_client = is_notified;
}

void BattleResult::vrReciveBattleReportNotifyBattle(Player * player)
{
	PlayerBattleResultInfo * player_battle_result = this->getPlayerBattleResultInfo(player->getId());
	CK_NULL_RETURN(player_battle_result, );

	player_battle_result->vr_recived_battle_result = true;
}

int BattleResult::getWinTeamId()
{
	return m_win_team_id;
}

PlayerBattleResultInfo * BattleResult::getPlayerBattleResultInfo(uint64_t user_id)
{
	auto it = m_player_battle_result_info_map.find(user_id);
	if (m_player_battle_result_info_map.end() != it)
	{
		return it->second;
	}

	return NULL;
}

bool BattleResult::checkBattleResult()
{
	// todo
	return true;
}


