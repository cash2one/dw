
#include "ManageClientPacket.h"
#include "ClientMessageManager.h"
#include "opcode.h"
#include "ManageVrpublicMessage.h"
#include "MessageMaker.h"
#include "ManageVrpublicSession.h"
#include "ManageTrSession.h"
#include "ManageSummoner.h"
#include "MakeGuid.h"
#include "BaseFunc.h"
#include "ManageFriendSession.h"
#include "common_define.h"
#include "opcode.h"
#include "inter_proto.pb.h"
#include "Singleton.h"

using namespace std;
using namespace MobaGo::FlatBuffer;

ManageClientPacket::ManageClientPacket()
{

}

ManageClientPacket::~ManageClientPacket()
{

}

ManageClientPacket * ManageClientPacket::instance()
{
	return Singleton<ManageClientPacket>::instance();
}

int ManageClientPacket::init()
{
	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Protocol_Base_E_PROTOCOL_KEEPALIVE,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgLobbyKeepAliveReq>(this, NULL, &ManageClientPacket::handleMsgLobbyKeepAliveReq));

	ClientMessageManager::instance()->addPlayerMsgHandle(E_SMsgGateRegister,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::SMsgGateRegister>(this, NULL, &ManageClientPacket::handleSMsgGateRegister));

	ClientMessageManager::instance()->addPlayerMsgHandle(E_MSG_PLAYER_CLOSE_CONNECTION,
		new PackMsg<ManageClientPacket, EmptyMsg>(this, NULL, &ManageClientPacket::handleClientCloseConnection));

	ClientMessageManager::instance()->addPlayerMsgHandle(E_SMsgVrpublicNotifyVrLoginInfo,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::SMsgVrpublicNotifyVrLoginInfo>(this, NULL, &ManageClientPacket::handleSMsgVrpublicNotifyVrLoginInfo));

	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgVRLoginReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgVRLoginReq>(this, NULL, &ManageClientPacket::handleMsgVRLoginReq));

	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgCreateSummonerReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgCreateSummonerReq>(this, NULL, &ManageClientPacket::handleMsgCreateSummonerReq));

	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgMatchReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgMatchReq>(this, NULL, &ManageClientPacket::handleMsgMatchReq));

	// 处理好友
	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgSearchFriendReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgSearchFriendReq>(this, NULL, &ManageClientPacket::handleMsgSearchFriendReq));

	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgAddFriendReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgAddFriendReq>(this, NULL, &ManageClientPacket::handleMsgAddFriendReq));
	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgDelFriendReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgDelFriendReq>(this, NULL, &ManageClientPacket::handleMsgDelFriendReq));
	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgConfirmAddFriendReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgConfirmAddFriendReq>(this, NULL, &ManageClientPacket::handleMsgConfirmAddFriendReq));
	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgAddCancelFriendReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgAddCancelFriendReq>(this, NULL, &ManageClientPacket::handleMsgAddCancelFriendReq));

	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgFriendListReq,
		new PackMsg<ManageClientPacket, EmptyMsg>(this, NULL, &ManageClientPacket::handleMsgFriendListReq));

	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgApplyForFriendListReq,
		new PackMsg<ManageClientPacket, EmptyMsg>(this, NULL, &ManageClientPacket::handleMsgApplyForFriendListReq));

	ClientMessageManager::instance()->addPlayerMsgHandle(MobaGo::Protocol::Opcode_E_MsgFriendDetailInfoReq,
		new PackMsg<ManageClientPacket, MobaGo::FlatBuffer::MsgFriendDetailInfoReq>(this, NULL, &ManageClientPacket::handleMsgFriendDetailInfoReq));
	return 0;
}

int ManageClientPacket::handleMsgLobbyKeepAliveReq(InterPacket * packet, const MsgLobbyKeepAliveReq * msg)
{
	return 0;
}

int ManageClientPacket::handleSMsgGateRegister(InterPacket * packet, const SMsgGateRegister * msg)
{
	InterPacket * inter_pack = MessageMaker::makeSMsgGateRegister(0, msg->ip_addr() != NULL ? msg->ip_addr()->c_str() : "", msg->port());

	ManageVrpublicMessage::instance()->handleInputPacket(inter_pack);

	return 0;
}

int ManageClientPacket::handleClientCloseConnection(InterPacket * packet, const EmptyMsg * msg)
{
	DEF_LOG_DEBUG("recv client close connection, client gate guid is <%llu>", packet->msg_head.guid);
	Account * account = ManageAccount::GetInstance()->findAccount(packet->msg_head.guid);
	if (NULL != account)
	{
		this->notifyClientCloseConnection(account);
	}

	ManageAccount::GetInstance()->delAccount(packet->msg_head.guid);
	return 0;
}

int ManageClientPacket::handleSMsgVrpublicNotifyVrLoginInfo(InterPacket * packet, const SMsgVrpublicNotifyVrLoginInfo * msg)
{
	string user_name = msg->user_name() != NULL ? msg->user_name()->c_str() : "";
	string login_key = msg->login_key() != NULL ? msg->login_key()->c_str() : "";

	ManageAccount::GetInstance()->newSessionCode(msg->user_guid(), user_name, login_key);

	flatbuffers::FlatBufferBuilder builder;
	InterPacket * ack_pack = new InterPacket();

	string gate_ip = msg->gate_ip() != NULL ? msg->gate_ip()->c_str() : "";
	flatbuffers::Offset<flatbuffers::String> off_gate_ip = builder.CreateString(gate_ip);
	string announcement = msg->announcement() != NULL ? msg->announcement()->c_str() : "";
	flatbuffers::Offset<flatbuffers::String> off_announcement = builder.CreateString(announcement);

	flatbuffers::Offset<SMsgVrpublicNotifyVrLoginInfoAck> off_notify = CreateSMsgVrpublicNotifyVrLoginInfoAck(builder, msg->gs_id(), off_gate_ip, msg->gate_port(), off_announcement);
	builder.Finish(off_notify);


	ack_pack->setMsgInfo(E_SMsgVrpublicNotifyVrLoginInfoAck, packet->msg_head.guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	ack_pack->session_id = packet->session_id;

	ManageVrpublicSession::instance()->handleOutputPacket(ack_pack);

	return 0;
	
}

int ManageClientPacket::handleMsgVRLoginReq(InterPacket * packet, const MsgVRLoginReq * msg)
{
	string session_code = msg->session_key() != NULL ? msg->session_key()->str() : "";
	if (!ManageAccount::GetInstance()->checkSessionCode(msg->account_id(), session_code))
	{
		MessageMaker::sendErrorPacket(packet->session_id, packet->msg_head.guid, MobaGo::FlatBuffer::Error_VRNoAccount);	
		return 0; 
	}

	Account * pAccount = NULL;
	Summoner * pSummoner = NULL;
	// process re login
	pAccount = ManageAccount::GetInstance()->getAccount(msg->account_id());
	if (NULL != pAccount)
	{
		pAccount->client_id = packet->msg_head.guid;
		pAccount->session_id = packet->session_id;

		if (pAccount->getSummoner() == NULL)
		{
			pSummoner = this->checkSummonerByLogin(pAccount);
			if (NULL == pSummoner)
			{
				return 0;
			}

			pAccount->setSummoner(pSummoner);
			pSummoner->setAccount(pAccount);
			pSummoner->client_id = packet->msg_head.guid;
			pSummoner->session_id = packet->session_id;
			ManageSummoner::GetInstance()->addMemSummonerById(pSummoner->m_nId, pSummoner);
		}
		else
		{
			pSummoner = pAccount->getSummoner();
		}

		MessageMaker::sendReqSummonerInfoAck(packet->session_id, packet->msg_head.guid, pSummoner);
	}
	else
	{
		pAccount = new Account();
		pAccount->account_id = msg->account_id();
		pAccount->session_id = packet->session_id;
		pAccount->client_id = packet->msg_head.guid;

		// client第一次连接vr,存入map_account
		uint64_t client_id = packet->msg_head.guid;
		ManageAccount::GetInstance()->addAccount(client_id, pAccount);

		pSummoner = this->checkSummonerByLogin(pAccount);

		if (NULL == pSummoner)
		{
			return 0;
		}

		pAccount->setSummoner(pSummoner);
		pSummoner->setAccount(pAccount);
		pSummoner->client_id = packet->msg_head.guid;
		pSummoner->session_id = packet->session_id;
		ManageSummoner::GetInstance()->addMemSummonerById(pSummoner->m_nId, pSummoner);

		MessageMaker::sendReqSummonerInfoAck(packet->session_id, packet->msg_head.guid, pSummoner);
	}

	// todo , check if player is in battle
	//账号id name入数据库
	// map_account 加入session_id client_id
	// to do list
	// 获取资源下载地址、推送邮件信息提示、老用户送签到英雄、长时间不打天梯降分处理、各个大厅地址推送
	// ...各种处理
	MessageMaker::sendVRLoginAck(packet->session_id, packet->msg_head.guid, 0);
	
	//通知好友服上线
	notifySummonerLoginToVRF(pSummoner->m_nId);
	
	return 0;
}

int  ManageClientPacket::handleMsgCreateSummonerReq(InterPacket * packet, const MsgCreateSummonerReq * msg)
{
	string summoner_name = msg->summoner_name() != NULL ? msg->summoner_name()->c_str() : "";
	int head_id = msg->head_id();

	uint64_t client_id = packet->msg_head.guid;
	Account * pAccount = ManageAccount::GetInstance()->findAccount(client_id);
	
	if (!BaseFunc::IllegalWordCheck::IsLegalString(summoner_name))
	{
		MessageMaker::sendErrorPacket(packet->session_id, packet->msg_head.guid, MobaGo::FlatBuffer::Error_IllegalChar);
		return 0;
	}


	if (pAccount == NULL)
	{
		MessageMaker::sendErrorPacket(packet->session_id, packet->msg_head.guid, MobaGo::FlatBuffer::Error_VRNoAccount);
		return 0;
	}

	Summoner * pSummoner = pAccount->getSummoner();
	if (pSummoner != NULL)
	{
		MessageMaker::sendErrorPacket(packet->session_id, packet->msg_head.guid, MobaGo::FlatBuffer::Error_DuplicateSummoner);
		return 0;
	}


	// to do list
	// 长度检查（常见宏定义）、脏字检查、数据库检查名字是否被占用
	if (summoner_name.empty() || summoner_name.find(" ", 0) != summoner_name.npos)
	{
		MessageMaker::sendErrorPacket(packet->session_id, packet->msg_head.guid, MobaGo::FlatBuffer::Error_IllegalChar);
		return 0;
	}


	// to do list
	// summoner创建
	MyGuid_t sum_id = 0;
	MakeGuid::instance()->get_guid(ET_SUMMONER, sum_id);
	if (!MakeGuid::instance()->get_guid(ET_SUMMONER, sum_id))
	{
		MessageMaker::sendErrorPacket(packet->session_id, packet->msg_head.guid, MobaGo::FlatBuffer::Error_MakeGuidFail);
		return 0;
	}

	dbass::Asssummoner ass_summoner(sum_id);
	Entity::summoner* e_summoner = (Entity::summoner *)ass_summoner.data();
	e_summoner->set_account_id(pAccount->account_id);
	e_summoner->set_sum_name(summoner_name.c_str());

	mysqlpp::Connection  * db_connection = ManageDB::GetInstance()->getMysqlConnection();
	mysqlpp::Query query = db_connection->query();
	ass_summoner.insert(query);

	query.store();

	pSummoner = new Summoner(sum_id);
	pSummoner->m_strSumName = summoner_name;

	pAccount->setSummoner(pSummoner);
	pSummoner->setAccount(pAccount);
	pSummoner->client_id = client_id;
	pSummoner->session_id = packet->session_id;

	//在线summoner添加到内存中
	ManageSummoner::GetInstance()->addMemSummonerById(pSummoner->m_nId, pSummoner);

	MessageMaker::sendCreateSummonerAck(packet->session_id, packet->msg_head.guid, sum_id);
	ManageFriendSession::instance()->sendVRNotifyCreateSummonerToVRF(sum_id, summoner_name, VrConfig::instance()->getVrCfg().server_id);

	//通知好友服上线
	notifySummonerLoginToVRF(pSummoner->m_nId);

	return 0;
}

int ManageClientPacket::handleMsgMatchReq(InterPacket * packet, const MobaGo::FlatBuffer::MsgMatchReq * msg)
{
	Account * account = ManageAccount::GetInstance()->findAccount(packet->msg_head.guid);
	if (NULL == account)
	{
		// error
		return -1;
	}

	this->updatePlayerInfoToTr(account);

	vector<uint64_t> group_member;
	group_member.push_back(account->account_id);
	InterPacket * pack = MessageMaker::makeSMsgMatchReq(account->account_id, msg->team_type(), msg->match_type(), group_member);
	ManageTrSession::instance()->handleOutputPacket(pack);
	return 0;
}

int ManageClientPacket::handleMsgCancelMatchReq(InterPacket * packet, const EmptyMsg * msg)
{
	Account * account = ManageAccount::GetInstance()->findAccount(packet->msg_head.guid);
	if (NULL == account)
	{
		// error
		return -1;
	}

	InterPacket * pack = MessageMaker::makeMsgCancelMatchReq(account->account_id);
	ManageTrSession::instance()->handleOutputPacket(pack);

	return 0;
}

Summoner * ManageClientPacket::checkSummonerByLogin(Account * account)
{
	Summoner * pSummoner = NULL;

	//uint64_t sum_id = ManageAccount::GetInstance()->querySumIdByAccountId(account->account_id);
	uint64_t sum_id = account->account_id;

	// sum_id为 0 说明该账号没有召唤师，需创建
	if (!sum_id)
	{
		MessageMaker::sendErrorPacket(account->session_id, account->client_id, MobaGo::FlatBuffer::Error_NeedCreateSummoner);
	}
	else
	{
		// sum_id不为空，设置召唤师信息
		pSummoner = ManageSummoner::GetInstance()->findSummoner(sum_id);
		if (NULL == pSummoner)
		{
			// should never happen
			//MessageMaker::sendVRLoginAck(account->session_id, account->client_id, 0);
		}
		else
		{

		}
	}

	return pSummoner;
}

// 好友相关
int ManageClientPacket::handleMsgSearchFriendReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgSearchFriendReq * msg)
{
	string friend_name = msg->friend_name() != NULL ? msg->friend_name()->str() : "";
	if (friend_name.empty() || friend_name.find(" ", 0) != friend_name.npos)
	{
		MessageMaker::sendErrorPacket(packet->session_id, packet->msg_head.guid, MobaGo::FlatBuffer::Error_IllegalChar);
		return 0;
	}

	CHECKACCOUNT(packet->msg_head.guid)
	CHECKSUMMONER(packet->msg_head.guid)

	uint64_t summoner_id = pSummoner->m_nId;
	
	ManageFriendSession::instance()->sendSMsgNotifySearchFriend(packet->msg_head.guid, summoner_id, friend_name);

	return 0;
}

int ManageClientPacket::handleMsgAddFriendReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgAddFriendReq * msg)
{
	CHECKACCOUNT(packet->msg_head.guid)
	CHECKSUMMONER(packet->msg_head.guid)

	uint64_t summoner_id = pSummoner->m_nId;
	ManageFriendSession::instance()->sendSMsgVRAddFriendReq(packet->msg_head.guid, summoner_id, msg->friend_id());

	return 0;
}

int ManageClientPacket::handleMsgDelFriendReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgDelFriendReq * msg)
{
	CHECKACCOUNT(packet->msg_head.guid)
	CHECKSUMMONER(packet->msg_head.guid)

	uint64_t summoner_id = pSummoner->m_nId;
	ManageFriendSession::instance()->sendSMsgVRDelFriendReq(packet->msg_head.guid, summoner_id, msg->friend_id());

	return 0;
}

int ManageClientPacket::handleMsgConfirmAddFriendReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgConfirmAddFriendReq * msg)
{
	CHECKACCOUNT(packet->msg_head.guid)
	CHECKSUMMONER(packet->msg_head.guid)

	uint64_t summoner_id = pSummoner->m_nId;
	ManageFriendSession::instance()->sendSMsgVRConfirmAddFriendReq(packet->msg_head.guid, summoner_id, msg->friend_id());

	return 0;
}

int ManageClientPacket::handleMsgAddCancelFriendReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgAddCancelFriendReq * msg)
{
	CHECKACCOUNT(packet->msg_head.guid)
	CHECKSUMMONER(packet->msg_head.guid)

	uint64_t summoner_id = pSummoner->m_nId;
	ManageFriendSession::instance()->sendSMsgVRAddCancelFriendReq(packet->msg_head.guid, summoner_id, msg->friend_id());

	return 0;
}

void ManageClientPacket::updatePlayerInfoToTr(Account * account)
{
	Proto::Player player;
	player.set_account_id(account->account_id);
	player.set_summoner_name(account->getSummoner()->m_strSumName);
	string str_buf = player.SerializeAsString();

	InterPacket * pack = new InterPacket();

	pack->setMsgInfo(E_proto_Player, account->account_id, str_buf.size(), str_buf.data());

	ManageTrSession::instance()->handleOutputPacket(pack);
}

void ManageClientPacket::notifyClientCloseConnection(Account * account)
{
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_MSG_PLAYER_CLOSE_CONNECTION, account->account_id, 0, (char *)NULL);
	ManageTrSession::instance()->handleOutputPacket(pack);

	if (NULL == account->m_summoner)
	{
		return;
	}

	uint64_t summoner_id = account->m_summoner->m_nId;
	InterPacket * friend_svr_packet = new InterPacket();
	friend_svr_packet->setMsgInfo(E_SMsgVRNotifySummonerLogoffToVRF, summoner_id, 0, (char *)NULL);
	ManageFriendSession::instance()->handleOutputStream(friend_svr_packet);
}

void ManageClientPacket::notifySummonerLoginToVRF(uint64_t summoner_id)
{
	InterPacket * packet = new InterPacket();
	packet->setMsgInfo(E_SMsgVRNotifySummonerLoginToVRF, summoner_id, 0, (char *)NULL);
	ManageFriendSession::instance()->handleOutputStream(packet);

}

int ManageClientPacket::handleMsgFriendListReq(InterPacket * packet, const EmptyMsg * msg)
{
	CHECKACCOUNT(packet->msg_head.guid)
	CHECKSUMMONER(packet->msg_head.guid)

	uint64_t summoner_id = pSummoner->m_nId;
	ManageFriendSession::instance()->sendSMsgFriendListReq(packet->msg_head.guid, summoner_id);

	return 0;
}

int ManageClientPacket::handleMsgApplyForFriendListReq(InterPacket * packet, const EmptyMsg * msg)
{
	CHECKACCOUNT(packet->msg_head.guid)
	CHECKSUMMONER(packet->msg_head.guid)

	uint64_t summoner_id = pSummoner->m_nId;
	ManageFriendSession::instance()->sendMsgApplyForFriendListReq(packet->msg_head.guid, summoner_id);

	return 0;
}

int ManageClientPacket::handleMsgFriendDetailInfoReq(InterPacket *packet, const MobaGo::FlatBuffer::MsgFriendDetailInfoReq * msg)
{
	CHECKACCOUNT(packet->msg_head.guid)
	CHECKSUMMONER(packet->msg_head.guid)

	uint64_t summoner_id = pSummoner->m_nId;
	uint64_t friend_id = msg->friend_id();

	ManageFriendSession::instance()->sendMsgFriendDetailInfoReq(packet->msg_head.guid, summoner_id, friend_id);

	return 0;
}