
#include "ManageMatch.h"
#include "Singleton.h"
#include "ManageBattleSession.h"
#include "MessageMaker.h"
#include "ManageClientSession.h"

ManageMatch::ManageMatch()
	: m_one_team_member_number(1)
{

}

ManageMatch::~ManageMatch()
{

}

//ManageMatch * ManageMatch::instance()
//{
//	return Singleton<ManageMatch>::instance();
//}

int ManageMatch::init(int one_team_member_num)
{
	m_one_team_member_number = one_team_member_num;
	return 0;
}

int ManageMatch::update()
{
	if (m_match_cell_map.size() >= m_one_team_member_number * 2)
	{
		BattleInfo * battle_info = ManageBattleSession::instance()->getAvilableBattle();
		if (NULL != battle_info)
		{
			MatchCellVec_t cell_vec;
			for (auto it = m_match_cell_map.begin(); it != m_match_cell_map.end(); )
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

void ManageMatch::handleMatch(MyGuid_t user_guid, SessionId session_id)
{
	this->addMatch(user_guid, session_id);
}

void ManageMatch::handleMatchCancel(MyGuid_t user_guid)
{
	this->removeMatch(user_guid);
}

void ManageMatch::addMatch(MyGuid_t user_guid, SessionId session_id)
{
	auto it = m_match_cell_map.find(user_guid);
	if (it != m_match_cell_map.end())
	{
		this->removeMatch(user_guid);
	}

	MatchCell * cell = new MatchCell();
	cell->init(user_guid, session_id);

	m_match_cell_map.insert(std::make_pair(user_guid, cell));

	InterPacket * pack = MessageMaker::makeMsgMatchAck(session_id, user_guid, 1);
	ManageClientSession::instance()->handleOutputPacket(pack);
}

void ManageMatch::removeMatch(MyGuid_t user_guid)
{
	auto it = m_match_cell_map.find(user_guid);
	if (it != m_match_cell_map.end())
	{
		delete it->second;
		m_match_cell_map.erase(it);
	}
	else
	{
		// 
	}
}

void ManageMatch::matchSuccess(MatchCellVec_t & match_cell_vec, BattleInfo * battle_info)
{
	for (auto it = match_cell_vec.begin(); it != match_cell_vec.end(); ++it)
	{
		MatchCell * cell = *it;
		this->notifyPlayerEnterBattle(cell, battle_info->battle_gate_ip, battle_info->battle_gate_port);
	}

	this->notifyBattleMatchResult(match_cell_vec, battle_info);
}

void ManageMatch::notifyPlayerEnterBattle(MatchCell * match_cell, const string & server_ip, int server_port)
{
	InterPacket * pack = MessageMaker::makeMsgMatchEnterBattle(match_cell->getUserId(), match_cell->getSessionId(), server_ip, server_port);
	ManageClientSession::instance()->handleOutputPacket(pack);
}

void ManageMatch::notifyBattleMatchResult(MatchCellVec_t & match_cell_vec, BattleInfo * battle_info)
{
	InterPacket * pack = MessageMaker::makeSMsgNotifyBattleMatchResult(battle_info->session_id, match_cell_vec);
	ManageBattleSession::instance()->handleOutputPacket(pack);
}

