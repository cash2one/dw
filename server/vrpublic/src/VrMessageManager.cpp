
#include "VrMessageManager.h"
#include "opcode.h"
#include "ClientMessageManager.h"

VrMessageManager * VrMessageManager::m_instance = NULL;

VrMessageManager::VrMessageManager()
{

}

VrMessageManager::~VrMessageManager()
{


}

VrMessageManager * VrMessageManager::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new VrMessageManager();
	}

	return m_instance;
}

int VrMessageManager::update(int & incoming_packet_num, int & incoming_bytes)
{
	incoming_packet_num = 0;
	incoming_bytes = 0;

	InterPacketVec_t pack_vec;
	this->getVrPacket(pack_vec);

	incoming_packet_num = (int)pack_vec.size();

	for (auto pack : pack_vec)
	{
		incoming_bytes += pack->getStreamSize();

		if (E_SMsgVrpublicNotifyVrLoginInfoAck == pack->msg_head.msg_id)
		{
			ClientMessageManager::instance()->handlePlayerMsg(pack->session_id, pack);
		}
		else
		{
			this->handleVrMsg(pack->session_id, pack);
			delete pack;
		}
	}
	return incoming_packet_num;
}

void VrMessageManager::handleVrPacket(InterPacketVec_t & pack_vec)
{
	boost::mutex::scoped_lock lock(m_vr_packet_vec_mtx);

	std::copy(pack_vec.begin(), pack_vec.end(), std::back_inserter(m_vr_packet_vec));
}

void VrMessageManager::handleVrPacket(InterPacket * packet)
{
	boost::mutex::scoped_lock lock(m_vr_packet_vec_mtx);
	m_vr_packet_vec.push_back(packet);
}

void VrMessageManager::getVrPacket(InterPacketVec_t & vr_packet_vec)
{
	boost::mutex::scoped_lock lock(m_vr_packet_vec_mtx);
	std::copy(m_vr_packet_vec.begin(), m_vr_packet_vec.end(), std::back_inserter(vr_packet_vec));
	m_vr_packet_vec.clear();
}

bool VrMessageManager::addVrMsgHandle(int msg_id, MsgHandleBase * message)
{
	auto it = m_msg_handle_map.find(msg_id);
	if (it != m_msg_handle_map.end())
	{
		// error
		return false;
	}

	m_msg_handle_map.insert(std::make_pair(msg_id, message));

	return true;
}

void VrMessageManager::handleVrMsg(session_t session_id, InterPacket * packet)
{
	MessageHandleMap_t::iterator it = m_msg_handle_map.find(packet->msg_head.msg_id);
	if (it != m_msg_handle_map.end())
	{
		MsgHandleBase * msg_handle = it->second;
		msg_handle->handleInterMsg(packet, packet->getBody(), packet->msg_head.msg_len);
	}
	else
	{
		DEF_LOG_ERROR("failed to get the message handle <%hd, %hd>", packet->msg_head.msg_id, packet->msg_head.msg_len);
	}
}
