
#ifndef MANAGE_BATTLE_SESSION_HPP__
#define MANAGE_BATTLE_SESSION_HPP__

#include <unordered_map>
#include "Singleton.h"
#include "ObConfig.h"
#include "MessagePump.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "inter_generated.h"
#include "dev_ob_generated.h"
#include "EmptyMsg.h"
#include "opcode.h"
#include <fstream>

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;
using namespace flatbuffers;
using namespace std;

struct BattleInfo 
{
	BattleInfo()
		: battle_server_id(0)
		, battle_gate_port(0)
		, session_id(NULL)
	{}

	int battle_server_id;
	int battle_gate_port;
	string battle_gate_ip;
	SessionId session_id;
};

typedef unordered_map<int, BattleInfo *> BattleInfoMap_t;
typedef unordered_map<SessionId, BattleInfo *> BattleSessionMap_t;

struct ObPlayerInfo
{
	uint64_t id = 0;
	uint32_t hero_skill_id = 0;
	uint8_t byt_camp = 0;
};

struct ObInfo
{
	uint32 ob_id = 0;
	uint32 battle_id = 0;
	std::string start_time;
	uint32 one_team_num = 0;
	uint32 status = 0;
	std::vector< std::pair<uint32, std::vector<int8_t>> > ob_updates;
	std::vector<ObPlayerInfo> players;

	bool serialize(const std::string file_name)
	{	
		std::ofstream w(file_name);
		if (!w)
			return false;

		w << ob_id;
		w << battle_id;
		w << start_time;
		w << one_team_num;
		w << status;
		uint32 sum = ob_updates.size();
		w << sum;
		for (uint32 i = 0; i < sum; i++)
		{
			w << ob_updates[i].first;
			uint32 len = ob_updates[i].second.size();
			w << len;
			for (uint32 j = 0; j < len; j++)
			{
				w << ob_updates[i].second[j];
			}

		}
	}

	bool deserialize(const std::string file_name)
	{
		std::ifstream r(file_name);
		if (!r)
			return false;

		r >> ob_id;
		r >> battle_id;
		r >> start_time;
		r >> one_team_num;
		r >> status;
		uint32 sum = 0; 
		r >> sum;
		for (uint32 i = 0; i < sum; i++)
		{
			uint32 frame_Id = 0;
			r >> frame_Id;
			uint32 len = 0;
			r >> len;
			std::vector<int8_t> frames;
			for (uint32 j = 0; j < len; j++)
			{
				int8_t temp = 0; 
				r >> temp;
				frames.push_back(temp);
			}
			
		}
	}
};

class ManageBattleSession : public MessagePumpHandle
{
public:
	ManageBattleSession();
	~ManageBattleSession();
public:
	static ManageBattleSession * instance();
public:
	int init(const BattleCfg & vr_cfg);

	int update();

	BattleInfo * getAvilableBattle();

	void handleOutputPacket(InterPacket * packet);
public:
	virtual void handleNewConnection(SessionId session_id);
	virtual void handleCloseConnection(SessionId session_id);
public:
	int handleSMsgBattleRegisterTr(InterPacket * input_packet, const SMsgBattleRegisterTr * input_msg);
	int onSMsgRegisterToDevOb(InterPacket * input_packet, const SMsgRegisterToDevOb * input_msg);
	int onSMsgRegisterOneBattle(InterPacket * input_packet, const SMsgRegisterOneBattle * input_msg);
	int onSMsgUpdateBattleFrame(InterPacket * input_packet, const SMsgUpdateBattleFrame * input_msg);
	int onSMsgUpdateBattleStatus(InterPacket * input_packet, const SMsgUpdateBattleStatus * input_msg);
	int onSMsgRequestObListReq(InterPacket * input_packet, const EmptyMsg * input_msg);

private:
	int onUpdateBattleStatus(uint32_t status);
	int onRegisterOneBattle(uint32_t battle_id, uint32_t one_team_num);
	int Inc();
	std::string numtoStr(uint32 i);
protected:
	void registerMsgHandle();

	void addRegisterBattle(SessionId session_id, int battle_server_id, int battle_gate_port, const string & battle_gate_ip);

	void removeBattle(int battle_server_id);
private:
	BattleCfg m_battle_cfg;

	MessageServerPump<InterPacket> * m_manage_server_pump;

	BattleInfoMap_t m_battle_info_map;

	BattleSessionMap_t m_battle_session_map;

	std::map<int, ObInfo *> m_info;
	int m_self_inc = 1;
	std::string comm_dir = "./caches/";
};

#endif

