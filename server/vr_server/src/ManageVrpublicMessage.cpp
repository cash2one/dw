
#include "ManageVrpublicMessage.h"
#include "opcode.h"
#include "ClientMessageManager.h"

ManageVrpublicMessage * ManageVrpublicMessage::m_instance = NULL;

ManageVrpublicMessage::ManageVrpublicMessage()
{

}

ManageVrpublicMessage::~ManageVrpublicMessage()
{

}

ManageVrpublicMessage * ManageVrpublicMessage::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageVrpublicMessage();
	}

	return m_instance;
}

int ManageVrpublicMessage::update()
{
	int ret = 0;
	InterPacketVec_t input_pack_vec;
	this->getInputPacket(input_pack_vec);

	ret = input_pack_vec.size();

	for (auto pack : input_pack_vec)
	{
		if (E_SMsgVrpublicNotifyVrLoginInfo == pack->msg_head.msg_id)
		{
			ClientMessageManager::instance()->handlePlayerPacket(pack);
		}
		else
		{
			this->handleInputMsgHandle(pack->session_id, pack);

			delete pack;
		}
	}

	return ret;
}

void ManageVrpublicMessage::handleInputPacket(InterPacket * packet)
{
	boost::mutex::scoped_lock lock(m_input_packet_vec_mtx);
	m_input_packet_vec.push_back(packet);
}

void ManageVrpublicMessage::handleInputPacket(InterPacketVec_t & pack_vec)
{
	boost::mutex::scoped_lock lock(m_input_packet_vec_mtx);
	std::copy(pack_vec.begin(), pack_vec.end(), std::back_inserter(m_input_packet_vec));
}

void ManageVrpublicMessage::getInputPacket(InterPacketVec_t & pack_vec)
{
	boost::mutex::scoped_lock lock(m_input_packet_vec_mtx);
	if (m_input_packet_vec.size() > 0)
	{
		std::copy(m_input_packet_vec.begin(), m_input_packet_vec.end(), std::back_inserter(pack_vec));
		m_input_packet_vec.clear();
	}
}

bool ManageVrpublicMessage::addInputMsgHandle(int msg_id, MsgHandleBase * msg_handle)
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

void ManageVrpublicMessage::handleInputMsgHandle(session_t session_id, InterPacket * packet)
{
	MessageHandleMap_t::iterator it = m_msg_handle_map.find(packet->msg_head.msg_id);
	if (it != m_msg_handle_map.end())
	{
		MsgHandleBase * msg_handle = it->second;
		//if (packet->msg_head.msg_id < MSG_PLAYER_CLIENT_START_MSG)
		//{
		//	//msg_handle->handleServerMsg(session_id, packet->getBody(), packet->msg_head.msg_len);
		//}
		//else
		//{
		//	//msg_handle->handle(session_id, packet->getBody(), packet->msg_head.msg_len);
		//}

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
