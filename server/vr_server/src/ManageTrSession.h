
#ifndef MANAGE_TR_SESSION_HPP__
#define MANAGE_TR_SESSION_HPP__

#include "MessagePump.h"
#include "VrConfig.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "match_generated.h"
#include "inter_generated.h"

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;
using namespace std;
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

	void handleOutputPacket(InterPacket * pack);

	int handleMsgLobbyKeepAliveReq(InterPacket * input_packet, const MsgLobbyKeepAliveReq * input_msg);

	int handleMsgMatchAck(InterPacket * input_packet, const MsgMatchAck * input_msg);

	int handleMsgCancelMatchAck(InterPacket * input_packet, const MsgCancelMatchAck * input_msg);

	int handleMsgMatchEnterBattle(InterPacket * input_packet, const MsgMatchEnterBattle * input_msg);

	int handleSMsgBattleEndNotifyVr(InterPacket * input_packet, const SMsgBattleEndNotifyVr * input_msg);
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);

	void setDefaultMsgHandler(std::function< int(int, InterPacket*, char*, int) > cb) { m_message_pump->setDefaultMsgHandler(cb); }
protected:
	void registerVrToTr();
private:
	MessageServerPump<InterPacket> * m_message_pump;

	TrInfo m_tr_info;

	static ManageTrSession * m_instance;
};

#endif
