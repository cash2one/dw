
#include "Logger.h"
#include "ClientMessageManager.h"
#include "opcode.h"

ClientMessageManager * ClientMessageManager::m_instance = NULL;

ClientMessageManager::ClientMessageManager()
{

}

ClientMessageManager::~ClientMessageManager()
{

}

ClientMessageManager * ClientMessageManager::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ClientMessageManager();
	}

	return m_instance;
}

int ClientMessageManager::update(int & incoming_packet_num, int & incoming_bytes)
{
	incoming_packet_num = 0;
	incoming_bytes = 0;

	InterPacketVec_t client_packet_vec;
	this->getClientPacket(client_packet_vec);
	incoming_packet_num = client_packet_vec.size();
	for (InterPacketVec_t::iterator it = client_packet_vec.begin(); it != client_packet_vec.end(); ++it)
	{
		InterPacket * packet = *it;
		this->handlePlayerMsg(packet->session_id, packet);

		incoming_bytes += packet->getStreamSize();
		delete packet;
	}

	return incoming_packet_num;
}

void ClientMessageManager::handlePlayerPacket(InterPacket * packet)
{
	//DEF_LOG_INFO("recv packet, msg id is <%d> len is <%d>\n", packet->msg_head.msg_id, packet->msg_head.msg_len);

	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_packet_mutex, );
	boost::mutex::scoped_lock lock(m_client_packet_mutex);
	m_client_packet.push_back(packet);
}

void ClientMessageManager::handlePlayerPacket(InterPacketVec_t & pack_vec)
{
	boost::mutex::scoped_lock lock(m_client_packet_mutex);
	std::copy(pack_vec.begin(), pack_vec.end(), std::back_inserter(m_client_packet));
}

void ClientMessageManager::getClientPacket(InterPacketVec_t & client_packet_vec)
{
	boost::mutex::scoped_lock lock(m_client_packet_mutex);
	std::copy(m_client_packet.begin(), m_client_packet.end(), std::back_inserter(client_packet_vec));
	m_client_packet.clear();
}

bool ClientMessageManager::addPlayerMsgHandle(int msg_id, MsgHandleBase * msg_handle)
{
	MessageHandleMap_t::iterator it = m_msg_handle_map.find(msg_id);
	if (it != m_msg_handle_map.end())
	{
		// error
		return false;
	}

	msg_handle->msg_id = msg_id;
	m_msg_handle_map.insert(std::make_pair(msg_id, msg_handle));

	return true;
}

void ClientMessageManager::handlePlayerMsg(session_t session_id, InterPacket * packet)
{
	MessageHandleMap_t::iterator it = m_msg_handle_map.find(packet->msg_head.msg_id);
	if (it != m_msg_handle_map.end())
	{
		DEF_LOG_DEBUG("handle client packet, msg id is <%hd>, msg len is <%hd>", packet->msg_head.msg_id, packet->msg_head.msg_len);

		MsgHandleBase * msg_handle = it->second;
		if (packet->msg_head.msg_id < MSG_PLAYER_CLIENT_START_MSG)
		{
			if (packet->msg_head.msg_id == E_MSG_PLAYER_CLOSE_CONNECTION)
				DEF_LOG_INFO("check what i found now!");
			//msg_handle->handleServerMsg(session_id, packet->getBody(), packet->msg_head.msg_len);
		}
		else
		{
			//msg_handle->handle(session_id, packet->getBody(), packet->msg_head.msg_len);
		}

		msg_handle->handleInterMsg(packet, packet->getBody(), packet->msg_head.msg_len);
	}
	else
	{
		// error
		if (!m_default_cb)
			return;

		m_default_cb(packet->msg_head.msg_id, packet, packet->getBody(), packet->msg_head.msg_len);
	}
}
