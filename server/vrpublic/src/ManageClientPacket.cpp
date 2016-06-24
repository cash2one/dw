
#include "ManageClientPacket.h"
#include "ClientMessageManager.h"
#include "ManageAccount.h"
#include "ManagerClientSession.h"
#include "MessagePacker.h"
#include "MyGuidDef.h"
#include "MakeGuid.h"

ManageClientPacket * ManageClientPacket::m_instance = NULL;

ManageClientPacket::ManageClientPacket()
{

}

ManageClientPacket::~ManageClientPacket()
{

}

ManageClientPacket * ManageClientPacket::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageClientPacket();
	}

	return m_instance;
}

int ManageClientPacket::init()
{
	DEF_LOG_INFO("init ManageClientPacket");

	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgLoginReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgLoginReq>(this, NULL, &ManageClientPacket::handleMsgLoginReq));

	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgChooseServerReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgChooseServerReq>(this, NULL, &ManageClientPacket::handleMsgChooseServerReq));

	ClientMessageManager::instance()->addPlayerMsgHandle(E_SMsgVrpublicNotifyVrLoginInfoAck,
		new PackMsg<ManageClientPacket, EmptyMsg>(this, NULL, &ManageClientPacket::handleSMsgVrpublicNotifyVrLoginInfoAck));

	ClientMessageManager::instance()->addPlayerMsgHandle(E_MSG_PLAYER_CLOSE_CONNECTION,
		new PackMsg<ManageClientPacket, EmptyMsg>(this, NULL, &ManageClientPacket::handleSMsgClientClosedGateNotifyServer));

	

	return 0;
}


int ManageClientPacket::handleMsgLoginReq(InterPacket * input_pack, const MsgLoginReq * msg)
{
	bool bNewAccount = false;

	DEF_LOG_DEBUG("get player login msg, gate guid is <%llu>, accoaunt name is <%s>, device name is <%s>, client version is <%s>", input_pack->msg_head.guid, 
		msg->account_name() != NULL ? msg->account_name()->c_str() : "", msg->account_name() != NULL ? msg->account_name()->c_str() : "", msg->client_version() != NULL ? msg->client_version()->c_str() : "");

	//InterPacket pp;
	//pp.msg_head = input_pack->msg_head;
	//pp.msg_head.msg_len = 0;

	//pp.setMsgInfo(pp.msg_head, NULL);

	//ManagerClientSession::instance()->handleOutputStream(input_pack->session_id, pp.getStream(), pp.getStreamSize());
	//pp.cleanStream();

	//return 0;
	
	// 判断用户名是否重复
	Account *pAccount = NULL;
	string account_name = msg->account_name() != NULL ? msg->account_name()->c_str() : "";
	string passwd = msg->password() != NULL ? msg->password()->c_str() : "";
	string device_name = msg->device_name() != NULL ? msg->device_name()->c_str() : "";
	if (msg->channel_type() == Login_Device) // 以后会加上Login_Device
	{
		if (msg->device_name()->str().empty())
		{
			MessagePacker::instance()->sendErrorPacket(input_pack->session_id, input_pack->msg_head.guid, MobaGo::FlatBuffer::Error_EmptyAccount);
			return 0;
		}
		pAccount = ManageAccount::GetInstance()->queryAccountByName(device_name, msg->channel_type()); // device有其它的数据库表
	}

	if (pAccount != NULL)
	{
		// 是否有其它session登录
		uint64_t client_id = pAccount->client_id;
		if (client_id != 0)
		{
			if (client_id == input_pack->msg_head.guid)
			{
				// 已经登录过了
				return 0;
			}
			else
			{
				// 有其他Session登录 其他地方不要绑定Session  // 被踢下线
				// 通知client强制关闭客户端, sendErrorPacket Error_ForceQuitClient
			}
		}

		// 
		ManageAccount::GetInstance()->accountSetClientInfo(input_pack->msg_head.guid, input_pack->session_id);
	}
	else
	{
		if (msg->channel_type() == Login_Device)
		{
			//MAC地址登陆，允许自动创建
			// 自动创建
			// 创建账户
			pAccount = ManageAccount::GetInstance()->createAccount(msg->language(), account_name, passwd, device_name, Login_Device, 0);
			if (!pAccount)
			{
				// 创建账户失败
				DEF_LOG_ERROR("failed to create account, accouant name is <%s>, divice name is <%s>", account_name.c_str(), device_name.c_str());

				MessagePacker::instance()->sendErrorPacket(input_pack->session_id, input_pack->msg_head.guid, MobaGo::FlatBuffer::Error_CreateAccount);

				return -1;
			}
			else
			{
				DEF_LOG_DEBUG("create new account, gate guid is <%llu>, account id <%llu>", input_pack->msg_head.guid, pAccount->guid);
				bNewAccount = true; // 需要告诉客户端吗？
			}
		}
	}
	
	if (pAccount)
	{
		AccountServerInfoList_t server_info_list;
		ManageAccount::GetInstance()->getAccountServerInfoList(pAccount->guid, server_info_list);
		ServerInfoMap_t & allServerInfoMap = ManageServerList::instance()->getServerInfoMap();
		if (!allServerInfoMap.empty())
		{
			MessagePacker::instance()->sendAreaList(input_pack->session_id, input_pack->msg_head.guid, allServerInfoMap, server_info_list);
		}
		ManageAccount::GetInstance()->addAccount(input_pack->msg_head.guid, input_pack->session_id, pAccount);
	}
	else
	{
		MessagePacker::instance()->sendErrorPacket(input_pack->session_id, input_pack->msg_head.guid, MobaGo::FlatBuffer::Error_NoAccount);
	}
	
	return 0;
}

int ManageClientPacket::handleMsgChooseServerReq(InterPacket * input_pack, const MsgChooseServerReq * msg)
{
	DEF_LOG_DEBUG("get choose msg, server id is <%d>", msg->server_id());

	Account * pAccount = ManageAccount::GetInstance()->findAccount(input_pack->msg_head.guid);
	if (!pAccount)
	{
		// account不存在
		DEF_LOG_ERROR("do not find account where choose server");
		return 0;
	}

	//
	AccountServerInfoList_t account_server_info_list;
	ManageAccount::GetInstance()->getAccountServerInfoList(pAccount->guid, account_server_info_list);
	auto it = find_if(account_server_info_list.begin(), account_server_info_list.end(),
		[=](AccountServerInfo * areaServer)
		{
			return areaServer->area_id == msg->server_id();  // server_id ---> area_id 名字容易混淆
		}
	);

	ServerVrInfo * vrServer = NULL;
	if (it != account_server_info_list.end())
	{
		vrServer = ManageServerList::instance()->GetVrServerByIndex((*it)->area_id, (*it)->gs_index);
	}
	else
	{
		// 随机一个vr
		vrServer = ManageServerList::instance()->RandGetVrServer(msg->server_id());
		if (vrServer)
		{
			ManageAccount::GetInstance()->addNewServerInfo(pAccount->guid, msg->server_id(), vrServer->gs_id);
		}
		else
		{
			MessagePacker::instance()->sendErrorPacket(input_pack->session_id, input_pack->msg_head.guid, MobaGo::FlatBuffer::Error_NoAvailableVR);
			return 0;
		}
	}

	// 向vr发登录消息
	string user_session_key = ManageServerList::instance()->makeUserSessionKey(pAccount->user_name);
	pAccount->session_code = user_session_key;
	
	if (!vrServer)
	{
		MessagePacker::instance()->sendErrorPacket(input_pack->session_id, input_pack->msg_head.guid, MobaGo::FlatBuffer::Error_NoAvailableVR);
		return 0;
	}
	else
	{
		MessagePacker::instance()->SendSMsgVrpublicNotifyVrLoginInfo(vrServer->session_id, input_pack->msg_head.guid, pAccount, vrServer);
	}
	

	return 0;	
}

int ManageClientPacket::handleSMsgVrpublicNotifyVrLoginInfoAck(InterPacket * input_pack, const EmptyMsg * msg)
{
	// to do list
	// 用以确认sesson_code已生成, 告诉client可以登录vr了
	MyGuid_t user_guid = input_pack->msg_head.guid;
	//MyGuid_t user_guid = SWITCH_GUID_TYPE(ET_PLAYER, input_pack->msg_head.guid);
	Account * pAccount = ManageAccount::GetInstance()->findAccount(user_guid);
	if (!pAccount)
	{
		// account不存在
		return 0;
	}

	const SMsgVrpublicNotifyVrLoginInfoAck * ack_msg = (SMsgVrpublicNotifyVrLoginInfoAck *)msg;
	MyGuid_t account_id = pAccount->guid;
	session_t session_id = pAccount->session_id;
	string gate_ip = ack_msg->gate_ip() != NULL ? ack_msg->gate_ip()->str() : "";
	int gate_port = ack_msg->gate_port();
	string announcement = ack_msg->announcement() != NULL ? ack_msg->announcement()->str() : "";
	string session_code = pAccount->session_code;

	// send client ack
	MessagePacker::instance()->sendChooseAreaAck(session_id, user_guid, pAccount->summoner_id, ack_msg->gs_id(), gate_ip, gate_port, session_code, announcement);

	return 0;
}

int ManageClientPacket::handleSMsgClientClosedGateNotifyServer(InterPacket * input_pack, const EmptyMsg * msg)
{
	// client connection closed
	DEF_LOG_DEBUG("get client closed msg, gate guid is <%llu>", input_pack->msg_head.guid);
	ManageAccount::GetInstance()->removeAccountByOffline(input_pack->msg_head.guid);
	return 0;
}

