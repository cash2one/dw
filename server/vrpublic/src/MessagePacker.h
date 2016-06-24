#ifndef MESSAGE_PACKER_HPP__
#define MESSAGE_PACKER_HPP__

#include "ManagerClientSession.h"
#include "logger.h"
#include "ManageClientPacket.h"
#include "msg_error_generated.h"
#include "Account.h"
#include "inter_generated.h"
#include "opcode.h"

using namespace MobaGo::FlatBuffer;

class MessagePacker
{
public:
	MessagePacker();
	~MessagePacker();

public:
	static MessagePacker * instance();

public:
	void sendErrorPacket(session_t session_id, uint64_t guid, MobaGo::FlatBuffer::Error error_code);
	void sendAreaList(session_t session_id, uint64_t guid, const ServerInfoMap_t & all_server_info_map, const AccountServerInfoList_t & account_server_info_list);

	void sendChooseAreaAck(session_t session_id, uint64_t guid, MyGuid_t account_id, int gs_id, string gate_ip, int gate_port, string session_code, string annoucement);

	void SendSMsgVrpublicNotifyVrLoginInfo(SessionId session_id, uint64_t guid, Account * account, ServerVrInfo * pVrServer);

public:
	static MessagePacker * m_instance;
};

#endif