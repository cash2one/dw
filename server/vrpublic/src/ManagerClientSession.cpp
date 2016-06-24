
#include "Logger.h"
#include "ManagerClientSession.h"
#include "ClientMessageManager.h"
#include "Packet_.h"
#include "opcode.h"
#include "BattleLog.h"

ManagerClientSession * ManagerClientSession::m_instance = NULL;

ManagerClientSession::ManagerClientSession()
	: m_session_server(NULL)
{

}

ManagerClientSession::~ManagerClientSession()
{

}

ManagerClientSession * ManagerClientSession::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManagerClientSession();
	}

	return m_instance;
}

void ManagerClientSession::update()
{
	
}

void ManagerClientSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	DEF_LOG_INFO("get new gate session : <%x>", session_id);
	m_stat_connection_count.addNewCount();
}

void ManagerClientSession::handleNewConnectionFailed()
{

}

void ManagerClientSession::handleCloseConnection(SessionId session, int error_code)
{
	DEF_LOG_INFO("close gate session <%x>", session);

	//BATTLE_LOG_INFO("ManagerSession connection close : <%x>\n", session);
	//m_stat_connection_count.addRemoveCount();
	////BattleManager::instance()->connectClosed(session);
	//
	//ClientPacket * packet = new ClientPacket();
	//packet->msg_head.msg_id = MSG_PLAYER_CLOSE_CONNECTION;
	//packet->msg_head.msg_len = 0;
	//packet->session_id = session;
	//ClientMessageManager::instance()->handlePlayerPacket(packet);
}

size_t ManagerClientSession::handleInputStream(SessionId session, char * buffer, size_t buf_len)
{
	InterPacketVec_t pack_vec;
	size_t pos = parseInterPacketFromStream(session, buffer, buf_len, pack_vec);
	if (pack_vec.size() > 0)
	{
		ClientMessageManager::instance()->handlePlayerPacket(pack_vec);
	}

	return pos;
	//ClientHead msg_head; 
	//ClientPacket * packet;
	//size_t pos = 0;
	//while (ClientPacket::pack_head_size <= (int)(buf_len - pos))
	//{
	//	memcpy(&msg_head, buffer + pos, ClientPacket::pack_head_size);
	//	if (msg_head.msg_len + ClientPacket::pack_head_size <= (int)(buf_len - pos))
	//	{
	//		packet = new ClientPacket();
	//		// make packet
	//		packet->setMsgInfo(msg_head, buffer + pos + ClientPacket::pack_head_size);
	//		pos += ClientPacket::pack_head_size + msg_head.msg_len;
	//		packet->session_id = session;
	//		ClientMessageManager::instance()->handlePlayerPacket(packet);
	//	}
	//	else
	//	{
	//		// 
	//		break;
	//	}
	//}

	//return pos;
}


//void ManagerClientSession::newConnection(netstream::Session_t session)
//{
//	BATTLE_LOG_INFO("get new connection : <%x>", session);
//}
//
//void ManagerClientSession::connectionClosed(netstream::Session_t session, int trigger_source)
//{
//	//BattleManager::instance()->connectClosed(session);
//
//	ClientPacket * packet = new ClientPacket();
//	packet->msg_head.msg_id = MSG_PLAYER_CLOSE_CONNECTION;
//	packet->msg_head.msg_len = 0;
//	packet->session_id = session;
//	MessageManager::instance()->handlePlayerPacket(packet);
//}
//
//void ManagerClientSession::handleInputStream(netstream::Session_t session, ACE_Message_Block & msg_block)
//{
//	ClientHead msg_head;
//	ClientPacket * packet;
//	while (ClientPacket::pack_head_size <= (int)msg_block.length())
//	{
//		memcpy(&msg_head, msg_block.rd_ptr(), ClientPacket::pack_head_size);
//		if (msg_head.msg_len + ClientPacket::pack_head_size <= (int)msg_block.length())
//		{
//			packet = new ClientPacket();
//			// make packet
//			packet->setMsgInfo(msg_head, msg_block.rd_ptr() + ClientPacket::pack_head_size);
//			msg_block.rd_ptr(ClientPacket::pack_head_size + msg_head.msg_len);
//			packet->session_id = session;
//			//MessageManager::instance()->handlePlayerMsg((int)session, packet);
//			MessageManager::instance()->handlePlayerPacket(packet);
//		}
//		else
//		{
//			// 
//			break;
//		}
//	}
//}

int ManagerClientSession::init(const string & addr, int port, int net_type)
{
	DEF_LOG_INFO("init gate session, ip is <%s>, port is <%d>, net type is <%d>", addr.c_str(), port, net_type);

	m_session_server = SessionServer::createSessionServer(NPT_TCP);
	m_session_server->init(this);

	m_session_server->connect(addr, port, true);

	return 0;
}

void ManagerClientSession::handleOutputStream(SessionId session, char * buffer, size_t buf_len)
{
	m_session_server->handleOutputStream(session, buffer, buf_len);
}

void ManagerClientSession::removeSession(SessionId session)
{

}

//netstream::SessionPool * ManagerClientSession::getNet()
//{
//	return m_session_pool;
//}

//void ManagerClientSession::removeSession(netstream::Session_t session)
//{
//	m_session_pool->removeSession(session);
//}
