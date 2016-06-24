
#ifndef MANAGE_BATTLE_SESSION_HPP__
#define MANAGE_BATTLE_SESSION_HPP__

#include <unordered_map>
#include "Singleton.h"
#include "TrConfig.h"
#include "MessagePump.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "inter_generated.h"
#include "opcode.h"

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
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);
public:
	int handleSMsgBattleRegisterTr(InterPacket * input_packet, const SMsgBattleRegisterTr * input_msg);
	int handleSMsgBattleEndNotifyVr(InterPacket * input_packet, const SMsgBattleEndNotifyVr * input_msg);
	int handleSMsgNotifyBattleMatchResultAck(InterPacket * input_packet, const SMsgNotifyBattleMatchResultAck * input_msg);
protected:
	void registerMsgHandle();

	void addRegisterBattle(SessionId session_id, int battle_server_id, int battle_gate_port, const string & battle_gate_ip);

	void removeBattle(int battle_server_id);
private:
	BattleCfg m_battle_cfg;

	MessageServerPump<InterPacket> * m_manage_server_pump;

	BattleInfoMap_t m_battle_info_map;

	BattleSessionMap_t m_battle_session_map;
};

#endif

