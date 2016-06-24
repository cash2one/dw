
#include "MatchQueue.h"

#include "Singleton.h"
#include "ManageBattleSession.h"
#include "MessageMaker.h"
#include "ManageVrSession.h"
#include "MatchBattle.h"

MatchQueue::MatchQueue()
	: m_one_team_member_number(1)
{

}

MatchQueue::~MatchQueue()
{

}

//MatchQueue * MatchQueue::instance()
//{
//	return Singleton<MatchQueue>::instance();
//}

int MatchQueue::init(int one_team_member_num)
{
	m_one_team_member_number = one_team_member_num;
	return 0;
}

int MatchQueue::update()
{
	if (m_match_cell_map.size() >= m_one_team_member_number * 2)
	{
		BattleInfo * battle_info = ManageBattleSession::instance()->getAvilableBattle();
		if (NULL != battle_info)
		{
			MatchCellVec_t cell_vec;
			for (auto it = m_match_cell_map.begin(); it != m_match_cell_map.end();)
			{
				//
				cell_vec.push_back(it->second);
				m_match_cell_map.erase(it++);

				if (cell_vec.size() == m_one_team_member_number * 2)
				{
					break;
				}
			}

			this->matchSuccess(cell_vec, battle_info);
		}
	}
	return 0;
}

void MatchQueue::handleMatch(MyGuid_t user_guid, SessionId session_id)
{
	this->addMatch(user_guid, session_id);
}

void MatchQueue::handleMatchCancel(MyGuid_t user_guid)
{
	this->removeMatch(user_guid);
}

void MatchQueue::addMatch(MyGuid_t user_guid, SessionId session_id)
{
	auto it = m_match_cell_map.find(user_guid);
	if (it != m_match_cell_map.end())
	{
		// should never happen
		this->removeMatch(user_guid);
	}

	MatchCell * cell = new MatchCell();
	cell->init(user_guid, session_id);

	m_match_cell_map.insert(std::make_pair(user_guid, cell));

	ManageMatch::instance()->addMatchCell(cell);

	InterPacket * pack = MessageMaker::makeMsgMatchAck(session_id, user_guid, 1);
	ManageVrSession::instance()->handleOutputPacket(pack);
}

void MatchQueue::removeMatch(MyGuid_t user_guid)
{
	auto it = m_match_cell_map.find(user_guid);
	if (it != m_match_cell_map.end())
	{
		MatchCell * match_cell = it->second;

		ManageMatch::instance()->removeMatchCell(match_cell);

		m_match_cell_map.erase(it);

		delete match_cell;
	}
	else
	{
		// 
	}
}

void MatchQueue::matchSuccess(MatchCellVec_t & match_cell_vec, BattleInfo * battle_info)
{
	DEF_LOG_DEBUG("match success, team member number is <%d>", m_one_team_member_number);

	MatchBattle * match_battle = ManageMatch::instance()->matchSuccess(battle_info, match_cell_vec);

	//for (auto it = match_cell_vec.begin(); it != match_cell_vec.end(); ++it)
	//{
	//	MatchCell * cell = *it;
	//	this->notifyPlayerEnterBattle(cell, battle_info->battle_gate_ip, battle_info->battle_gate_port);
	//	DEF_LOG_DEBUG("team member is <%llu>", cell->getUserId());
	//}

	this->notifyBattleMatchResult(match_cell_vec, match_battle, battle_info);
}

void MatchQueue::notifyPlayerEnterBattle(MatchCell * match_cell, const string & server_ip, int server_port)
{
	InterPacket * pack = MessageMaker::makeMsgMatchEnterBattle(match_cell->getUserId(), match_cell->getSessionId(), server_ip, server_port);
	ManageVrSession::instance()->handleOutputPacket(pack);
}

void MatchQueue::notifyBattleMatchResult(MatchCellVec_t & match_cell_vec, MatchBattle * match_battle, BattleInfo * battle_info)
{
	InterPacket * pack = MessageMaker::makeSMsgNotifyBattleMatchResult(battle_info->session_id, match_battle->getMatchId(), match_cell_vec);
	ManageBattleSession::instance()->handleOutputPacket(pack);
}
