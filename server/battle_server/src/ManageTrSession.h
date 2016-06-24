
#ifndef MANAGE_TR_SESSION_HPP__
#define MANAGE_TR_SESSION_HPP__

#include "BattleConfig.h"
#include "MessagePump.h"
#include "battle_generated.h"
#include "match_generated.h"
#include "protocol_generated.h"
#include "inter_generated.h"
#include "Packet_.h"
#include "EmptyMsg.h"

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;
using namespace flatbuffers;

struct TrInfo 
{
	TrInfo()
		: session_id(NULL)
	{}

	SessionId session_id;

};

class ManageTrSession : public MessagePumpHandle
{
public:
	ManageTrSession();
	~ManageTrSession();
	static ManageTrSession * instance();
public:
	int init(const TrCfg & tr_cfg);

	int update();

	void updateGateInfo(const string & gate_ip, int gate_port);

	void handleOutputPacket(InterPacket * packet);
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);

	int handleSMsgNotifyBattleMatchResult(InterPacket * input_pack, const MobaGo::FlatBuffer::SMsgNotifyBattleMatchResult * input_msg);

	int handleSMsgVrReciveBattleReportNotifyBattle(InterPacket * input_pack, const EmptyMsg * input_msg);

protected:
	void registerMsgHandle();
	void registerToTr();

	InterPacket * createSMsgBattleRegisterTr(int battle_id, const string & ip_addr, int port);
private:
	TrCfg m_tr_cfg;

	TrInfo m_tr_info;

	MessageServerPump<InterPacket> * m_message_pump;

	string m_gate_ip;

	int m_gate_port;
};

#endif
