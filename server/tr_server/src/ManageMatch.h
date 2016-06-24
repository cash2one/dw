

#ifndef MANAGE_MATCH_HPP__
#define MANAGE_MATCH_HPP__

#include <unordered_map>
#include "MyGuidDef.h"
#include "Packet_.h"
#include "MatchCell.h"
#include "MatchQueue.h"
#include "match_generated.h"
#include "protocol_generated.h"
#include "inter_generated.h"
#include "ManageState.h"
#include "PlayerTrInfo.h"
#include "ManagePlayerTrInfo.h"
#include "MatchBattle.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;


struct BattleInfo;

typedef unordered_map<MyGuid_t, MatchCell *> MatchCellMap_t;

class ManageMatch
{
public:
	ManageMatch();
	~ManageMatch();
	static ManageMatch * instance();
public:
	// player request match
	int handleMatch(InterPacket * input_packet, const SMsgMatchReq * input_msg);

	// player cancel match
	int handleMatchCancel(InterPacket * input_packet);

	// player offline 
	int handlePlayerOffline(InterPacket * input_packet);

	// battle finish
	void playerBattleFinish(PlayerTrInfo * player);

	void handleSMsgNotifyBattleMatchResultAck(uint64_t match_battle_id, int result);
public:
	int init();

	int update();

	void addMatchCell(MatchCell * match_cell);

	void removeMatchCell(MatchCell * match_cell);

	void removeMatchCellAndDelete(PlayerTrInfo * player_info);

	MatchBattle * matchSuccess(BattleInfo * battle_info, MatchCellVec_t & match_cell_vec);
public:
	bool stateRequestMatch(PlayerTrInfo * player_info);

	//bool stateMatchingRequestMatch(PlayerTrInfo * player_info);

	bool stateMatchSuccess(PlayerTrInfo * player_info);

	bool stateCancelMatch(PlayerTrInfo * player_info);

	bool stateMatchingPlayerOffline(PlayerTrInfo * player_info);

	bool stateBattleFinish(PlayerTrInfo * player_info);

protected:

	MatchQueue * getMatchQueue(int match_type);

	MatchBattle * getMatchBattle(uint64_t match_battle_id);
	void addMatchBattle(MatchBattle * match_battle);
	void removeMatchBattle(uint64_t match_battle_id);

	bool cancelMatching(PlayerTrInfo * player_info);
private:
	uint64_t m_match_id_index;

	MatchCellMap_t m_match_cell_map;

	MatchQueueMap_t m_match_queue_map;

	MatchBattleMat_t m_match_battle_map;

	ManageState<PlayerTrState, PlayerTrEvent, PlayerTrInfo> m_manage_player_state;
};

#endif

