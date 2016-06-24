
#ifndef MANAGE_VR_SESSION_HPP__
#define MANAGE_VR_SESSION_HPP__

#include <unordered_map>
#include "Singleton.h"
#include "ObConfig.h"
#include "MessagePump.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "inter_generated.h"
#include "match_generated.h"
#include "opcode.h"
#include "EmptyMsg.h"
#include "ManageMatch.h"

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;
using namespace flatbuffers;

struct VrInfo
{
	VrInfo()
		: area_id(0)
		, gs_id(0)
	{}

	int area_id;
	int gs_id;
	SessionId session_id;

};

typedef unordered_map<int, VrInfo *> VrInfoMap_t;
typedef unordered_map<SessionId, VrInfo *> VrSessionMap_t;

struct AreaInfo 
{
	AreaInfo()
		: area_id(0)
	{}

	int area_id;
	VrInfoMap_t vr_info_map;
};

typedef unordered_map<int, AreaInfo *> AreaInfoMap_t;

typedef unordered_map<int, ManageMatch *> ManageMatchMap_t;

class ManageClientSession : public MessagePumpHandle
{
public:
	ManageClientSession();
	~ManageClientSession();
public:
	static ManageClientSession * instance();
public:
	int init(const VrCfg & vr_cfg);

	int update();

	void handleOutputPacket(InterPacket * packet);
public:
	virtual void handleNewConnection(SessionId session_id);
	virtual void handleCloseConnection(SessionId session_id);
public:
	int handleSMsgVrRegisterTr(InterPacket * input_packet, const SMsgVrRegisterTr * input_msg);
	int handleSMsgMatchReq(InterPacket * input_packet, const SMsgMatchReq * input_msg);
	int handleMsgCancelMatchReq(InterPacket * input_packet, const SMsgMatchReq * input_msg);
protected:
	void registerMsgHandle();

	void addVr(SessionId session_id, int area_id, int gs_id);

	void removeVr(int area_id, int gs_id);

	VrInfo * getVr(SessionId session_id);

	ManageMatch * getManageMatch(int team_type, int match_type);

	void updateMatch();
private:
	VrCfg m_vr_cfg;

	MessageServerPump<InterPacket> * m_manage_server_pump;

	AreaInfoMap_t m_area_info_map;
	VrSessionMap_t m_vr_session_map;

	ManageMatchMap_t m_manage_match_map;
};

#endif

