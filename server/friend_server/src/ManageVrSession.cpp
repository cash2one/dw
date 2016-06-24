
#include "ManageVrSession.h"

#include "cache_auto_assistant.h"
#include "ManageDB.h"
#include <mysql++.h>
#include "MessageMaker.h"
#include "msg_error_generated.h"
#include "common_define.h"
#include "ManageSummoner.h"

using namespace MobaGo::FlatBuffer;

ManageVrSession::ManageVrSession()
	: m_message_pump(NULL)
{

}

ManageVrSession::~ManageVrSession()
{

}

ManageVrSession * ManageVrSession::instance()
{
	return Singleton<ManageVrSession>::instance();
}

int ManageVrSession::init(const FriendCfg & friend_cfg)
{
	m_message_pump = MessageServerPump<InterPacket>::createMessageServerPump();
	m_message_pump->init(this, NPT_TCP);

	m_message_pump->listen(friend_cfg.ip_addr, friend_cfg.port);

	registerMsgHandle();

	return 0;
}

int ManageVrSession::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_message_pump->update(dispatch_pack_num, dispatch_pack_bytes);


	return (int)dispatch_pack_num;
}

void ManageVrSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	DEF_LOG_INFO("manage_vr_session handle new connection");
}

void ManageVrSession::handleCloseConnection(SessionId session_id)
{
	DEF_LOG_INFO("manage_vr_session handle close connection");
	delVR(session_id);
}

int ManageVrSession::handleOutputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	return m_message_pump->handleOutputStream(session_id, buffer, buf_len);
}

void ManageVrSession::registerMsgHandle()
{
	m_message_pump->addMsgHandle(E_SMsgVrRegisterVRFriend, new PacketHandle<ManageVrSession, InterPacket, SMsgVrRegisterVRFriend>(this, &ManageVrSession::handleSMsgVrRegisterVRFriend));
	m_message_pump->addMsgHandle(E_SMsgVRNotifyCreateSummonerToVRF, new PacketHandle<ManageVrSession, InterPacket, SMsgVRNotifyCreateSummonerToVRF>(this, &ManageVrSession::handleSMsgVRNotifyCreateSummonerToVRF));
	m_message_pump->addMsgHandle(E_SMsgVRNotifySummonerLogoffToVRF, new PacketHandle<ManageVrSession, InterPacket, EmptyMsg>(this, &ManageVrSession::handleVRNotifySummonerLogoffToVRF));
	m_message_pump->addMsgHandle(E_SMsgVRNotifySummonerLoginToVRF, new PacketHandle<ManageVrSession, InterPacket, EmptyMsg>(this, &ManageVrSession::handleVRNotifySummonerLoginToVRF));

	m_message_pump->addMsgHandle(E_SMsgNotifySearchFriend, new PacketHandle<ManageVrSession, InterPacket, SMsgVRNotifySearchFriend>(this, &ManageVrSession::handleSMsgNotifySearchFriendReq));
	m_message_pump->addMsgHandle(E_SMsgVRNotifyAddFriendReq, new PacketHandle<ManageVrSession, InterPacket, SMsgVRAddFriendReq>(this, &ManageVrSession::handleSMsgVRAddFriendReq));
	m_message_pump->addMsgHandle(E_SMsgVRNotifyDelFriendReq, new PacketHandle<ManageVrSession, InterPacket, SMsgVRDelFriendReq>(this, &ManageVrSession::handleSMsgVRDelFriendReq));
	m_message_pump->addMsgHandle(E_SMsgVRNotifyConfirmAddFriendReq, new PacketHandle<ManageVrSession, InterPacket, SMsgVRConfirmAddFriendReq>(this, &ManageVrSession::handleSMsgVRConfirmAddFriendReq));
	m_message_pump->addMsgHandle(E_SMsgVRNotifyAddCancelFriendReq, new PacketHandle<ManageVrSession, InterPacket, SMsgVRAddCancelFriendReq>(this, &ManageVrSession::handleSMsgVRAddCancelFriendReq));

	m_message_pump->addMsgHandle(E_SMsgFriendListReq, new PacketHandle<ManageVrSession, InterPacket, SMsgFriendListReq>(this, &ManageVrSession::handleSMsgFriendListReq));
	m_message_pump->addMsgHandle(E_SMsgApplyForFriendListReq, new PacketHandle<ManageVrSession, InterPacket, SMsgApplyForFriendListReq>(this, &ManageVrSession::handleSMsgApplyForFriendReq));
	
	m_message_pump->addMsgHandle(E_SMsgVRFriendDetailInfoReq, new PacketHandle<ManageVrSession, InterPacket, SMsgFriendDetailInfoReq>(this, &ManageVrSession::handleSMsgFriendDetailInfoReq));
	m_message_pump->addMsgHandle(E_SMsgVRFriendDetailInfoAck, new PacketHandle<ManageVrSession, InterPacket, SMsgFriendDetailInfoAck>(this, &ManageVrSession::handleSMsgFriendDetailInfoAck));
}

int ManageVrSession::handleSMsgVrRegisterVRFriend(InterPacket *inter_packet, const SMsgVrRegisterVRFriend * msg)
{
	addVR(inter_packet->session_id, msg->area_id(), msg->server_id());

	return 0;
}

int ManageVrSession::handleSMsgVRNotifyCreateSummonerToVRF(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRNotifyCreateSummonerToVRF * msg)
{
	uint64_t sum_id = msg->sum_id();
	int server_id = msg->server_id();
	string sum_name = msg->sum_name() != NULL ? msg->sum_name()->str() : "";
	if (sum_name.empty())
	{
		DEF_LOG_ERROR("notify create summoner error, summoner name is empty, sum_id:%llu, server_id:%d ", sum_id, server_id);
		MessageMaker::sendVRErrorPacket(inter_packet->session_id, inter_packet->msg_head.guid, InterError_SummonerNameIsEmpty, this);
		return -1;
	}

	mysqlpp::Connection  * db_connection = ManageDB::GetInstance()->getMysqlConnection();
	mysqlpp::Query query = db_connection->query();
	// 创建好友信息
	dbass::Assfriend_player_info ass_summoner(sum_id);
	Entity::friend_player_info * e_summoner = (Entity::friend_player_info *)ass_summoner.data();
	e_summoner->set_player_name(sum_name);
	e_summoner->set_vr_id(server_id);

	ass_summoner.insert(query);

	// 创建好友关系
	dbass::Assfriend_player_relation ass_summoner_ralation(sum_id);
	Entity::friend_player_relation * e_summoner_relation = (Entity::friend_player_relation *)ass_summoner_ralation.data();
	ass_summoner_ralation.insert(query);

	return 0;
}

int ManageVrSession::handleVRNotifySummonerLoginToVRF(InterPacket * inter_packet, const EmptyMsg *msg)
{
	uint64_t summoner_id = inter_packet->msg_head.guid;

	//将自己的状态广播给所有的玩家
	notifySummonerStatusToFriends(summoner_id, SummonerStatus_Idle);
	
	//将好友的状态推送给自己
	MYSQL_GET_QUERY
	ASS_SUMMONER_RELATION
	google::protobuf::RepeatedField< ::google::protobuf::uint64 > summoner_friend_list = e_summoner_relation->friend_list();
	InterPacket * pack = MessageMaker::makeSMsgNotifyFriendsStatusToSummoner(inter_packet->session_id, inter_packet->msg_head.guid, summoner_id, summoner_friend_list);
	m_message_pump->handleOutputStream(inter_packet->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;

	Summoner * pSummoner = ManageSummoner::GetInstance()->findSummoner(summoner_id);
	if (pSummoner == NULL)
	{
		pSummoner = new Summoner();
		pSummoner->m_nId = summoner_id;
		pSummoner->setStatus(SummonerStatus_Idle);
		ManageSummoner::GetInstance()->addSummoner(summoner_id, pSummoner);
		
		return 0;
	}

	pSummoner->setStatus(SummonerStatus_Idle);
	
	

	return 0;
}

int ManageVrSession::handleVRNotifySummonerLogoffToVRF(InterPacket * inter_packet, const EmptyMsg *msg)
{
	uint64_t summoner_id = inter_packet->msg_head.guid;
	ManageSummoner::GetInstance()->delSummoner(summoner_id);
	notifySummonerStatusToFriends(summoner_id, SummonerStatus_Offline);

	return 0;
}

int ManageVrSession::handleSMsgNotifySearchFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRNotifySearchFriend * msg)
{
	uint64 summoner_id = msg->summoner_id();
	string friend_name = msg->friend_name() != NULL ? msg->friend_name()->str() : "";
	if (friend_name.empty())
	{
		DEF_LOG_ERROR("vr search friend error: friend_name is empty");
		MessageMaker::sendVRErrorPacket(inter_packet->session_id, inter_packet->msg_head.guid, InterError_SummonerNameIsEmpty, this);
		return -1;
	}

	mysqlpp::Connection * m_db_connection = ManageDB::GetInstance()->getMysqlConnection();
	mysqlpp::Query query = m_db_connection->query();
	query << "SELECT guid, player_name, vr_id FROM friend_player_info where player_name=";
	query << "'";
	query << friend_name;
	query << "'";

	const ::mysqlpp::StoreQueryResult& res = query.store();

	if (res.size() == 0)
	{
		MessageMaker::sendVRErrorPacket(inter_packet->session_id, inter_packet->msg_head.guid, InterError_FriendSvrHasNoSummoner, this);
		return -1;
	}

	mysqlpp::Row row = *res.begin();
	uint64 friend_id = atoll(row[0]);
	int friend_server_id = atoi(row[2]);

	// 好友粗略数据存数据库，不用查询VR
	InterPacket * pack = MessageMaker::makeSMsgVRFriendNotifySearchFriendAck(inter_packet->session_id, inter_packet->msg_head.guid, friend_id, friend_name, friend_server_id);
	m_message_pump->handleOutputStream(inter_packet->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();
	delete pack;

	return 0;
}

int ManageVrSession::handleSMsgVRAddFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRAddFriendReq * msg)
{
	uint64_t summoner_id = msg->summoner_id();
	uint64_t friend_id = msg->friend_id();
	uint64_t  sum_guid = inter_packet->msg_head.guid;

	if (summoner_id == friend_id)
	{
		MessageMaker::sendVRErrorPacket(inter_packet->session_id, inter_packet->msg_head.guid, InterError_AddMyselfAsFriend, this);
		return -1;
	}

	MYSQL_GET_QUERY
	ASS_FRIEND_RELATION

	// 将B的好友关系数据更新
	bool bAlreadyAdd = false;
	google::protobuf::RepeatedField< ::google::protobuf::uint64 > friend_apply_list = e_friend_relation->apply_list();
	for (int i = 0; i < friend_apply_list.size(); ++i)
	{
		if (friend_apply_list.Get(i) == summoner_id)
		{
			bAlreadyAdd = true;
			break;
		}
	}
	if (!bAlreadyAdd)
	{
		e_friend_relation->add_apply_list(summoner_id);
	}
	ass_friend_relation.update(query);
	query.store();

	// 通知申请者消息已发送
	InterPacket * pack = MessageMaker::makeSMsgNotifyFriendOperateAck(inter_packet->session_id, inter_packet->msg_head.guid, summoner_id, friend_id, MobaGo::FlatBuffer::friend_oper_type_add_friend);
	m_message_pump->handleOutputStream(inter_packet->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;

	// 通知被申请的VR
	// 好友召唤师数据
	// 判断是否在线
	Summoner * pFriend = ManageSummoner::GetInstance()->findSummoner(friend_id);
	if (pFriend == NULL)
	{
		return 0;
	}

	ASS_FRIEND_INFO

	int friend_gs_id = e_friend->vr_id();
	VrSession * pFriendVR = getVR(friend_gs_id);
	if (pFriendVR == NULL)
		return -1;
	InterPacket * friend_pack = MessageMaker::makeSMsgNotifyFriendOperateReq(pFriendVR->session_id, summoner_id, friend_id, MobaGo::FlatBuffer::friend_oper_type_add_friend);
	m_message_pump->handleOutputStream(pFriendVR->session_id, friend_pack->getStream(), friend_pack->getStreamSize());
	friend_pack->cleanStream();

	delete friend_pack;

	return 0;
}

int ManageVrSession::handleSMsgVRDelFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRDelFriendReq * msg)
{
	uint64_t summoner_id = msg->summoner_id();
	uint64_t friend_id = msg->friend_id();
	uint64_t  sum_guid = inter_packet->msg_head.guid;

	MYSQL_GET_QUERY
	ASS_SUMMONER_RELATION
	ASS_FRIEND_INFO
	ASS_FRIEND_RELATION

	// 删除本人好友列表
	google::protobuf::RepeatedField< ::google::protobuf::uint64 > summoner_friend_list = e_summoner_relation->friend_list();
	e_summoner_relation->clear_friend_list();
	for (int i = 0; i < summoner_friend_list.size(); ++i)
	{
		if (summoner_friend_list.Get(i) != summoner_id)
		{
			e_summoner_relation->add_friend_list(summoner_friend_list.Get(i));
		}
	}
	ass_summoner_relation.update(query);
	query.store();

	//删除对方好友列表
	google::protobuf::RepeatedField< ::google::protobuf::uint64 > friend_friend_list = e_friend_relation->friend_list();
	e_friend_relation->clear_friend_list();
	for (int i = 0; i < friend_friend_list.size(); ++i)
	{
		if (friend_friend_list.Get(i) != summoner_id)
		{
			e_friend_relation->add_friend_list(friend_friend_list.Get(i));
		}
	}
	ass_friend_relation.update(query);
	query.store();

	// 通知申请者消息已发送
	InterPacket * pack = MessageMaker::makeSMsgNotifyFriendOperateAck(inter_packet->session_id, inter_packet->msg_head.guid, summoner_id, friend_id, MobaGo::FlatBuffer::friend_oper_type_del_friend);
	m_message_pump->handleOutputStream(inter_packet->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;

	// 通知被申请的VR
	// 判断是否在线
	Summoner * pFriend = ManageSummoner::GetInstance()->findSummoner(friend_id);
	if (pFriend == NULL)
	{
		return 0;
	}

	int friend_gs_id = e_friend->vr_id();
	VrSession * pFriendVR = getVR(friend_gs_id);
	if (pFriendVR == NULL)
		return -1;
	InterPacket * friend_pack = MessageMaker::makeSMsgNotifyFriendOperateReq(pFriendVR->session_id, summoner_id, friend_id, MobaGo::FlatBuffer::friend_oper_type_del_friend);
	m_message_pump->handleOutputStream(pFriendVR->session_id, friend_pack->getStream(), friend_pack->getStreamSize());
	friend_pack->cleanStream();

	delete friend_pack;

	return 0;
}

int ManageVrSession::handleSMsgVRConfirmAddFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRConfirmAddFriendReq * msg)
{
	uint64_t summoner_id = msg->summoner_id();
	uint64_t friend_id = msg->friend_id();
	uint64_t  sum_guid = inter_packet->msg_head.guid;

	MYSQL_GET_QUERY
	ASS_SUMMONER_RELATION
	ASS_FRIEND_INFO
	ASS_FRIEND_RELATION

	// 从申请名单中移去并添加到好友名单中
	bool bIsInApplyList = false;
	google::protobuf::RepeatedField< ::google::protobuf::uint64> summoner_apply_list = e_summoner_relation->apply_list();
	e_summoner_relation->clear_apply_list();
	for (int i = 0; i < summoner_apply_list.size(); ++i)
	{
		if (summoner_apply_list.Get(i) != friend_id)
		{
			e_summoner_relation->add_apply_list(summoner_apply_list.Get(i));
		}
		else
		{
			bIsInApplyList = true;
		}
	}
	if (!bIsInApplyList)
	{
		DEF_LOG_ERROR("confirm add error: not even in my apply list");
		return -1;
	}

	// 保证玩家好友列表中没有重复好友
	bool bSummonerAlreadyAddFriend = false;
	google::protobuf::RepeatedField<google::protobuf::uint64> summoner_friend_list = e_summoner_relation->friend_list();
	for (int i = 0; i < summoner_friend_list.size(); ++i)
	{
		if (summoner_friend_list.Get(i) == friend_id)
		{
			bSummonerAlreadyAddFriend = true;
			break;
		}
	}

	if (!bSummonerAlreadyAddFriend)
	{
		e_summoner_relation->add_friend_list(friend_id);
		ass_summoner_relation.update(query);
	}

	//申请者处理：添加自己到好友名单
	google::protobuf::RepeatedField< ::google::protobuf::uint64> friend_friend_list = e_friend_relation->friend_list();
	bool bFriendAlreadyAddSummoner = false;
	for (int i = 0; i < friend_friend_list.size(); ++i)
	{
		if (friend_friend_list.Get(i) == summoner_id)
		{
			bFriendAlreadyAddSummoner = true;
		}
	}
	if (!bFriendAlreadyAddSummoner)
	{
		e_friend_relation->add_friend_list(summoner_id);
		ass_friend_relation.update(query);
	}
	
	// 通知申请者消息已发送
	InterPacket * pack = MessageMaker::makeSMsgNotifyFriendOperateAck(inter_packet->session_id, inter_packet->msg_head.guid, summoner_id, friend_id, MobaGo::FlatBuffer::friend_oper_type_add_confirm);
	m_message_pump->handleOutputStream(inter_packet->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;

	// 通知被申请的VR
	// 判断是否在线
	Summoner * pFriend = ManageSummoner::GetInstance()->findSummoner(friend_id);
	if (pFriend == NULL)
	{
		return 0;
	}

	int friend_gs_id = e_friend->vr_id();
	VrSession * pFriendVR = getVR(friend_gs_id);
	if (pFriendVR == NULL)
		return -1;
	InterPacket * friend_pack = MessageMaker::makeSMsgNotifyFriendOperateReq(pFriendVR->session_id, summoner_id, friend_id, MobaGo::FlatBuffer::friend_oper_type_add_confirm);
	m_message_pump->handleOutputStream(pFriendVR->session_id, friend_pack->getStream(), friend_pack->getStreamSize());
	friend_pack->cleanStream();

	delete friend_pack;

	return 0;
}

int ManageVrSession::handleSMsgVRAddCancelFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgVRAddCancelFriendReq * msg)
{
	uint64_t summoner_id = msg->summoner_id();
	uint64_t friend_id = msg->friend_id();
	uint64_t  sum_guid = inter_packet->msg_head.guid;
	MYSQL_GET_QUERY
	ASS_SUMMONER_RELATION
	
	// 从申请名单中移去
	google::protobuf::RepeatedField< ::google::protobuf::uint64 > summoner_apply_list = e_summoner_relation->apply_list();
	e_summoner_relation->clear_apply_list();
	for (int i = 0; i < summoner_apply_list.size(); ++i)
	{
		if (summoner_apply_list.Get(i) != friend_id)
		{
			e_summoner_relation->add_apply_list(summoner_apply_list.Get(i));
		}
	}
	ass_summoner_relation.update(query);
	
	// 通知申请者消息已发送
	InterPacket * pack = MessageMaker::makeSMsgNotifyFriendOperateAck(inter_packet->session_id, inter_packet->msg_head.guid, summoner_id, friend_id, MobaGo::FlatBuffer::friend_oper_type_add_cancel);
	m_message_pump->handleOutputStream(inter_packet->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;

	return 0;
}

void ManageVrSession::addVR(SessionId session_id, int area_id, int gs_id)
{
	VrSession * pVrsession = NULL;
	auto itSession = m_vr_session_map.find(session_id);
	if (itSession != m_vr_session_map.end())
	{
		pVrsession = itSession->second;
		pVrsession->area_id = area_id;
		pVrsession->gs_id = gs_id;
		return;
	}

	pVrsession = new VrSession();
	m_vr_session_map[session_id] = pVrsession;
	pVrsession->session_id = session_id;
	pVrsession->area_id = area_id;
	pVrsession->gs_id = gs_id;

	// put into m_vr_server_map
	auto itServer = m_vr_server_map.find(gs_id);
	if (itServer != m_vr_server_map.end())
	{
		itServer->second = pVrsession;
		return;
	}

	m_vr_server_map[gs_id] = pVrsession;
}

void ManageVrSession::delVR(SessionId session_id)
{
	auto itSession = m_vr_session_map.find(session_id);
	int gs_id = -1;
	if (itSession != m_vr_session_map.end())
	{
		gs_id = itSession->second->gs_id;
		delete itSession->second;
		m_vr_session_map.erase(itSession);
	}

	auto itServer = m_vr_server_map.find(gs_id);
	if (itServer != m_vr_server_map.end())
	{
		m_vr_server_map.erase(itServer);
	}
}

VrSession * ManageVrSession::getVR(SessionId session_id)
{
	auto it = m_vr_session_map.find(session_id);
	if (it != m_vr_session_map.end())
	{
		return it->second;
	}
	
	return NULL;
}

VrSession * ManageVrSession::getVR(int gs_id)
{
	auto it = m_vr_server_map.find(gs_id);
	if (it != m_vr_server_map.end())
	{
		return it->second;
	}

	return NULL;
}

int ManageVrSession::handleSMsgFriendListReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgFriendListReq * msg)
{
	uint64_t summoner_id = msg->summoner_id();

	mysqlpp::Connection  * db_connection = ManageDB::GetInstance()->getMysqlConnection();
	mysqlpp::Query query = db_connection->query();

	dbass::Assfriend_player_relation ass_friend_relation(summoner_id);
	Entity::friend_player_relation* e_friend_relation = (Entity::friend_player_relation *)ass_friend_relation.data();

	if (!ass_friend_relation.load(query))
	{
		MessageMaker::sendVRErrorPacket(inter_packet->session_id, inter_packet->msg_head.guid, InterError_FriendSvrHasNoSummoner, this);
		return -1;
	}

	query.store();

	const ::google::protobuf::RepeatedField<::google::protobuf::uint64 >& friend_list = e_friend_relation->friend_list();

	flatbuffers::FlatBufferBuilder builder;
	std::vector<flatbuffers::Offset<FriendDetail>> vec_friend;
	for (int i = 0; i < friend_list.size(); ++i)
	{
		uint64_t friend_id = friend_list.Get(i);

		dbass::Assfriend_player_info ass_friend(friend_id);
		Entity::friend_player_info* e_friend = (Entity::friend_player_info *)ass_friend.data();
		if (!ass_friend.load(query))
		{
			continue;
		}
		query.store();

		flatbuffers::Offset<FriendDetail> off_friend = CreateFriendDetail(builder, e_friend->guid(), 0, 0);
		vec_friend.push_back(off_friend);
	}
	
	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<FriendDetail>>> friend_vec = builder.CreateVector(vec_friend);
	flatbuffers::Offset<SMsgFriendListAck> off_friend_list_ok = CreateSMsgFriendListAck(builder, summoner_id, friend_vec);
	builder.Finish(off_friend_list_ok);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgFriendListAck, inter_packet->msg_head.guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = inter_packet->session_id;

	m_message_pump->handleOutputStream(inter_packet->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;

	return 0;
}


int ManageVrSession::handleSMsgApplyForFriendReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgApplyForFriendListReq * msg)
{
	uint64_t summoner_id = msg->summoner_id();

	mysqlpp::Connection  * db_connection = ManageDB::GetInstance()->getMysqlConnection();
	mysqlpp::Query query = db_connection->query();

	dbass::Assfriend_player_relation ass_friend_relation(summoner_id);
	Entity::friend_player_relation* e_friend_relation = (Entity::friend_player_relation *)ass_friend_relation.data();

	if (!ass_friend_relation.load(query))
	{
		MessageMaker::sendVRErrorPacket(inter_packet->session_id, inter_packet->msg_head.guid, InterError_FriendSvrHasNoSummoner, this);
		return -1;
	}

	query.store();

	const ::google::protobuf::RepeatedField<::google::protobuf::uint64 >& apply_friend_list = e_friend_relation->apply_list();

	flatbuffers::FlatBufferBuilder builder;
	std::vector<flatbuffers::Offset<FriendDetail>> vec_friend;
	for (int i = 0; i < apply_friend_list.size(); ++i)
	{
		uint64_t friend_id = apply_friend_list.Get(i);

		dbass::Assfriend_player_info ass_friend(friend_id);
		Entity::friend_player_info* e_friend = (Entity::friend_player_info *)ass_friend.data();
		if (!ass_friend.load(query))
		{
			continue;
		}

		query.store();

		flatbuffers::Offset<FriendDetail> off_friend = CreateFriendDetail(builder, e_friend->guid(), 0, 0);

		vec_friend.push_back(off_friend);
	}

	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<FriendDetail>>> friend_vec = builder.CreateVector(vec_friend);
	flatbuffers::Offset<SMsgFriendListAck> off_apply_list_ok = CreateSMsgFriendListAck(builder, summoner_id, friend_vec);
	builder.Finish(off_apply_list_ok);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgApplyForFriendListAck, inter_packet->msg_head.guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = inter_packet->session_id;

	m_message_pump->handleOutputStream(inter_packet->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
	return 0;
}

int ManageVrSession::handleSMsgFriendDetailInfoReq(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgFriendDetailInfoReq * msg)
{
	uint64_t summoner_id = msg->summoner_id();
	uint64_t friend_id = msg->friend_id();

	MYSQL_GET_QUERY
	ASS_FRIEND_INFO

	int friend_gs_id = e_friend->vr_id();
	VrSession * pVrSession = getVR(friend_gs_id);
	if (pVrSession == NULL)
	{
		//error log
		return -1;
	}

	InterPacket * friend_pack = MessageMaker::makeSMsgFriendDetailInfoReq(pVrSession->session_id, summoner_id, friend_id);
	m_message_pump->handleOutputStream(pVrSession->session_id, friend_pack->getStream(), friend_pack->getStreamSize());
	friend_pack->cleanStream();

	delete friend_pack;


	return 0;
}

int ManageVrSession::handleSMsgFriendDetailInfoAck(InterPacket * inter_packet, const MobaGo::FlatBuffer::SMsgFriendDetailInfoAck * msg)
{
	uint64_t summoner_id = msg->summoner_id();
	uint64_t friend_id = msg->friend_id();
	uint64_t friend_head_id = msg->friend_head_id();

	MYSQL_GET_QUERY
	ASS_SUMMONER_INFO

	int sum_svr_id = e_summoner->vr_id();

	VrSession * pVrSession = getVR(sum_svr_id);
	if (pVrSession == NULL)
	{
		return -1;
	}

	InterPacket * pack = MessageMaker::makeSMsgFriendDetailInfoAck(pVrSession->session_id, summoner_id, friend_id, friend_head_id);
	m_message_pump->handleOutputStream(pVrSession->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;

	return 0;
}

void ManageVrSession::notifySummonerStatusToFriends(uint64_t summoner_id, SummonerStatus sum_status)
{
	MYSQL_GET_QUERY

	dbass::Assfriend_player_relation ass_summoner_relation(summoner_id); 
	Entity::friend_player_relation* e_summoner_relation = (Entity::friend_player_relation *)ass_summoner_relation.data(); 
	if (!ass_summoner_relation.load(query)) 
	{ 
		return;
	}

	google::protobuf::RepeatedField< ::google::protobuf::uint64 > summoner_friend_list = e_summoner_relation->friend_list();
	for (int i = 0; i < summoner_friend_list.size(); ++i)
	{
		uint64_t friend_id = summoner_friend_list.Get(i);
		DEF_LOG_INFO("check friend list: summoner_id:%llu, friend_id:%llu", summoner_id, friend_id);
		dbass::Assfriend_player_info ass_friend(friend_id); 
		Entity::friend_player_info* e_friend = (Entity::friend_player_info *)ass_friend.data(); 
		if (!ass_friend.load(query)) 
		{ 
			continue;
		}
		// online check
		Summoner * pSummoner = ManageSummoner::GetInstance()->findSummoner(friend_id);
		if (pSummoner == NULL)
		{
			continue;
		}

		int friend_server_id = e_friend->vr_id();
		VrSession * pVrSession = getVR(friend_server_id);
		if (pVrSession == NULL)
		{
			continue;;
		}

		InterPacket * pack = MessageMaker::makeSMsgNotifySummonerStatusToFriends(pVrSession->session_id, summoner_id, friend_id, sum_status);
		m_message_pump->handleOutputStream(pVrSession->session_id, pack->getStream(), pack->getStreamSize());
		pack->cleanStream();

		delete pack;
	}
}