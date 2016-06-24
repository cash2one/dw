
#include "cache_auto_assistant.h"
#include "ManageFriendSession.h"
#include "MessageMaker.h"
#include "VrConfig.h"
#include "Summoner.h"
#include "ManageSummoner.h"
#include "protocol_generated.h"
#include "opcode.h"
#include "ManageAccount.h"
#include "protocol_generated.h"
#include "common_define.h"
#include "ManageDB.h"
#include "mysql++.h"

using namespace MobaGo::FlatBuffer;

ManageFriendSession::ManageFriendSession()
	: m_message_pump(NULL)
{

}

ManageFriendSession::~ManageFriendSession()
{

}

ManageFriendSession * ManageFriendSession::instance()
{
	return Singleton<ManageFriendSession>::instance();
}

int ManageFriendSession::init(const string & ip_addr, int port)
{
	m_message_pump = MessageServerPump<InterPacket>::createMessageServerPump();
	m_message_pump->init(this, NPT_TCP);
	m_message_pump->connect(ip_addr, port, true);

	return 0;
}

int ManageFriendSession::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_message_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	return dispatch_pack_num;
}

void ManageFriendSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	m_server_info.session_id = session_id;
	DEF_LOG_INFO("get new friend session <%x>", session_id);

	this->registerVrToVRFriend();
}

void ManageFriendSession::handleCloseConnection(SessionId session_id)
{
	DEF_LOG_INFO("get close friend session <%x>", session_id);
	if (session_id == m_server_info.session_id)
	{
		m_server_info.session_id = NULL;
		DEF_LOG_INFO("clear friend session ");
	}
}

void ManageFriendSession::sendVRNotifyCreateSummonerToVRF(uint64_t sum_id, string sum_name, int server_id)
{
	InterPacket * pack = MessageMaker::makeSmsgVRNotifyCreateSummonerToVRF(m_server_info.session_id, sum_id, sum_name, server_id);
	m_message_pump->handleOutputStream(m_server_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;

}

void ManageFriendSession::handleOutputStream(InterPacket *inter_packet)
{
	m_message_pump->handleOutputStream(m_server_info.session_id, inter_packet->getStream(), inter_packet->getStreamSize());
	inter_packet->cleanStream();

	delete inter_packet;
}

void ManageFriendSession::sendSMsgNotifySearchFriend(uint64 guid, uint64 sum_id, string friend_name)
{
	InterPacket * pack = MessageMaker::makeSMsgNotifySearchFriend(m_server_info.session_id, guid, sum_id, friend_name);
	m_message_pump->handleOutputStream(m_server_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
}

void ManageFriendSession::sendSMsgVRAddFriendReq(uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	InterPacket * pack = MessageMaker::makeSMsgVRAddFriendReq(m_server_info.session_id, guid, summoner_id, friend_id);
	m_message_pump->handleOutputStream(m_server_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
}

void ManageFriendSession::sendSMsgVRDelFriendReq(uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	InterPacket * pack = MessageMaker::makeSMsgVRDelFriendReq(m_server_info.session_id, guid, summoner_id, friend_id);
	m_message_pump->handleOutputStream(m_server_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
}

void ManageFriendSession::sendSMsgVRConfirmAddFriendReq(uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	InterPacket * pack = MessageMaker::makeSMsgVRConfirmAddFriendReq(m_server_info.session_id, guid, summoner_id, friend_id);
	m_message_pump->handleOutputStream(m_server_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
}

void ManageFriendSession::sendSMsgVRAddCancelFriendReq(uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	InterPacket * pack = MessageMaker::makeSMsgVRAddCancelFriendReq(m_server_info.session_id, guid, summoner_id, friend_id);
	m_message_pump->handleOutputStream(m_server_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
}

void ManageFriendSession::registerVrToVRFriend()
{
	InterPacket * pack = MessageMaker::makeSMsgVrRegisterVRFriend(m_server_info.session_id, VrConfig::instance()->getVrCfg().area_id, VrConfig::instance()->getVrCfg().server_id);
	m_message_pump->handleOutputStream(m_server_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
}

int ManageFriendSession::sendSMsgFriendListReq(uint64_t guid, uint64_t summoner_id)
{
	InterPacket * pack = MessageMaker::makeSMsgFriendListReq(m_server_info.session_id, guid, summoner_id);
	m_message_pump->handleOutputStream(m_server_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;

	return 0;
}

int ManageFriendSession::sendMsgApplyForFriendListReq(uint64_t guid, uint64_t summoner_id)
{
	InterPacket * pack = MessageMaker::makeSMsgApplyForFriendListReq(m_server_info.session_id, guid, summoner_id);
	m_message_pump->handleOutputStream(m_server_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;

	return 0;
}

void ManageFriendSession::sendMsgFriendDetailInfoReq(uint64_t guid, uint64_t summoner_id, uint64_t friend_id)
{
	InterPacket * pack = MessageMaker::makeSMsgFriendDetailInfoReq(m_server_info.session_id, guid, summoner_id, friend_id);
	m_message_pump->handleOutputStream(m_server_info.session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();

	delete pack;
}
