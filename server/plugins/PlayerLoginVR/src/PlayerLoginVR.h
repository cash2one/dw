
#ifndef PlayerLoginVR_hpp__
#define PlayerLoginVR_hpp__

#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include "typedef.h"
#include <flatbuffers/flatbuffers.h>
#include "inter_generated.h"
#include "plugin.h"
//#include "msg_demo.pb.h"
#include "robot_generated.h"
#include "protocol_generated.h"
#include "match_generated.h"
#include "opcode.h"
#include "EmptyMsg.h"
#include "login_generated.h"
#include "inter_generated.h"
#include <google/protobuf/message.h>
#include "pool.h"
#include "msg_error_generated.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;


class PlayerLoginVR : public Plugin
{
public:
	PlayerLoginVR();
	~PlayerLoginVR();
public:
	enum PlayerStatus
	{
		PS_base		= 0,
		PS_loading,
		PS_online,
		PS_offline,
	};

	struct PlayerLoginInfo
	{
		PlayerLoginInfo()
			: summoner_id(0)
			, player_status(PS_base)
			, gate_guid(0)
		{}

		PlayerLoginInfo(uint64_t _summoner_id, string _user_name, string _login_key)
			: summoner_id(_summoner_id)
			, user_name(_user_name)
			, login_key(_login_key)
			, player_status(PS_base)
			, gate_guid(0)
		{}

		uint64_t summoner_id;
		string user_name;
		string login_key;
		PlayerStatus player_status;
		uint64_t gate_guid;
	};

	typedef unordered_map<uint64_t, PlayerLoginInfo *> PlayerLoginInfoMap_t;
public:

	BEGIN_INPUT_MSG_MAP()

		INPUT_HANDLE_MSG(PlayerLoginVR, E_SMsgGateRegister, SMsgGateRegister)
		INPUT_HANDLE_MSG(PlayerLoginVR, E_SMsgVrpublicNotifyVrLoginInfo, SMsgVrpublicNotifyVrLoginInfo)

		INPUT_HANDLE_MSG(PlayerLoginVR, E_VrModuleVrpConnected, EmptyMsg)
		INPUT_HANDLE_MSG(PlayerLoginVR, E_VrModuleVrpConnectionClosed, EmptyMsg)

		INPUT_HANDLE_MSG(PlayerLoginVR, E_MSG_PLAYER_CLOSE_CONNECTION, EmptyMsg)
		//
		INPUT_HANDLE_MSG(PlayerLoginVR, Opcode_E_MsgVRLoginReq, MsgVRLoginReq)
		INPUT_HANDLE_MSG(PlayerLoginVR, Opcode_E_MsgCreateSummonerReq, MsgCreateSummonerReq)
		//INPUT_HANDLE_MSG(PlayerLoginVR, Opcode_E_MsgMatchReq, MsgMatchReq)

	END_INPUT_MSG_MAP()

	BEGIN_REQUEST_MSG_MAP()
		REQUEST_HANDLE_MSG(PlayerLoginVR, R_GetSummonerGuidByGateGuid, EmptyMsg)
		REQUEST_HANDLE_MSG(PlayerLoginVR, R_GetGateGuidBySummonerId, EmptyMsg)
	END_REQUEST_MSG_MAP()

	BEGING_NOTIFY_MSG_MAP()
		NOTIFY_HANDLE_MSG(PlayerLoginVR, N_VrModuleNotifyAreaId, EmptyMsg)
	END_NOTIFY_MSG_MAP()

public:
	// input msg
	INPUT_FUN_DECLARE(PlayerLoginVR, E_SMsgGateRegister, SMsgGateRegister);
	
	INPUT_FUN_DECLARE(PlayerLoginVR, E_SMsgVrpublicNotifyVrLoginInfo, SMsgVrpublicNotifyVrLoginInfo);
	
	INPUT_FUN_DECLARE(PlayerLoginVR, E_VrModuleVrpConnected, EmptyMsg);
	
	INPUT_FUN_DECLARE(PlayerLoginVR, E_VrModuleVrpConnectionClosed, EmptyMsg);

	INPUT_FUN_DECLARE(PlayerLoginVR, E_MSG_PLAYER_CLOSE_CONNECTION, EmptyMsg);

	INPUT_FUN_DECLARE(PlayerLoginVR, Opcode_E_MsgVRLoginReq, MsgVRLoginReq);

	INPUT_FUN_DECLARE(PlayerLoginVR, Opcode_E_MsgCreateSummonerReq, MsgCreateSummonerReq);

	// request msg
	REQUEST_FUN_DECLARE(PlayerLoginVR, R_GetSummonerGuidByGateGuid, EmptyMsg);
	REQUEST_FUN_DECLARE(PlayerLoginVR, R_GetGateGuidBySummonerId, EmptyMsg);

	// notify msg
	NOTIFY_FUN_DECLARE(PlayerLoginVR, N_VrModuleNotifyAreaId, EmptyMsg);

public:
	void loadDataCallback(uint32_t req_id, uint64_t entity_guid, MSG_TYPE * entity_msg);
	
protected:
	void notifyCreateSummoner(uint64_t summoner_guid);
	void notifyPlayerOnline(uint64_t summoner_guid);
	void notifyPlayerOffline(uint64_t summoner_guid);

	template<class _MsgType>
	void notifyPluginMsg(int msg_id, uint64_t summoner_guid, _MsgType * notify_msg);
protected:
	void registerGateInfoToVrpublic();

	void addPlayerLoginInfo(uint64_t account_id, const string & user_name, const string & login_key);
	PlayerLoginInfo * getPlayerLoginInfo(uint64_t account_id);

	void addGateGuid(uint64_t gate_guid, PlayerLoginInfo * player_login_info);
	PlayerLoginInfo * getPlayerByGateGuid(uint64_t gate_guid);

	InterPacket * makeSMsgVrRegisterVrpublic(SessionId session_id, int area_id, int server_id, const string & gate_ip_addr, int gate_port);
	InterPacket * makeMsgError(uint64_t gate_guid, int error_code);
	InterPacket * makeMsgReqSummonerInfoAck(uint64_t gate_guid, const string & summoner_name);
private:
	SessionId m_vrp_session_id;
	string m_gate_ip;
	int m_gate_port;
	int m_gate_net_type;

	int m_area_id;
	int m_server_id;

	PlayerLoginInfoMap_t m_player_login_info_map;

	PlayerLoginInfoMap_t m_player_login_info_gate_guid_map;
};

#endif
