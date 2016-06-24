
#include "RaknetMessagePump.h"

RaknetMessagePump::RaknetMessagePump()
	: m_msg_pump_handle(NULL)
{

}

RaknetMessagePump::~RaknetMessagePump()
{

}

int RaknetMessagePump::init(MessagePumpHandle * msg_pump_handle, int net_type)
{
	m_msg_pump_handle = msg_pump_handle;

	return 0;
}

int RaknetMessagePump::listen(const string & ip_addr, int port)
{
	return 0;
}

int RaknetMessagePump::connect(const string & ip_addr, int port, bool auto_reconnect)
{
	_PumpType * message_pump = _PumpType::createMessageServerPump();

	message_pump->init(this, NPT_RAKNET);

	message_pump->connect(ip_addr, port, false);

	message_pump->setDefaultMsgHandler(m_cb);

	ConnectInfo * connect_info = new ConnectInfo(message_pump, ip_addr, port);

	m_connect_info_raknet_map.insert(std::make_pair(message_pump->getSS(), connect_info));

	return 0;
}

bool RaknetMessagePump::addMsgHandle(int msg_id, PacketHandleBase<ServerPacket> * msg_handle)
{
	return false;
}

int RaknetMessagePump::update(size_t & dispatch_pack_num, size_t & dispatch_pack_bytes)
{
	for (auto it : m_connect_info_raknet_map)
	{
		ConnectInfo * connect_info = it.second;
		connect_info->message_pump->update(dispatch_pack_num, dispatch_pack_bytes);
	}

	return 0;
}

int RaknetMessagePump::handleOutputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	ConnectInfo * connect_info = this->getConnectInfoBySession(session_id);
	if (NULL == connect_info)
	{
		return -1;
	}

	return connect_info->message_pump->handleOutputStream(session_id, buffer, buf_len);
}

bool RaknetMessagePump::setDefaultMsgHandler(_PumpType::DefaultMsgHandler cb)
{
	m_cb = cb;
	return true;
}

void * RaknetMessagePump::getSS()
{
	return NULL;
}

void RaknetMessagePump::closeSession(SessionId session_id)
{
	return;
}

void RaknetMessagePump::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	//MessageServerPumpImp<ServerPacket> * msg_pump = dynamic_cast<MessageServerPumpImp<ServerPacket> *>(reinterpret_cast<_PumpType *>(raknet_session));

	ConnectInfo * connect_info = this->getConnectInfoByRaknet(raknet_session);
	if (NULL == connect_info)
	{
		// error
		return;
	}

	connect_info->session_id = session_id;

	m_connect_info_session_map.insert(std::make_pair(session_id, connect_info));

	m_msg_pump_handle->handleNewConnection(session_id, remote_ip_addr, local_port, raknet_session);
}

void RaknetMessagePump::handleCloseConnection(SessionId session_id)
{
	m_msg_pump_handle->handleCloseConnection(session_id);
}

MessageServerPump<ServerPacket> * RaknetMessagePump::createMessagePump(int net_type)
{
	if (NPT_RAKNET == net_type)
	{
		return new RaknetMessagePump();
	}
	else
	{
		return MessageServerPump<ServerPacket>::createMessageServerPump();
	}
}

RaknetMessagePump::ConnectInfo * RaknetMessagePump::getConnectInfoByRaknet(SessionId raknet_session)
{
	auto it = m_connect_info_raknet_map.find(raknet_session);
	if (it != m_connect_info_raknet_map.end())
	{
		return it->second;
	}

	return NULL;
}

RaknetMessagePump::ConnectInfo * RaknetMessagePump::getConnectInfoBySession(SessionId session_id)
{
	auto it = m_connect_info_session_map.find(session_id);
	if (it != m_connect_info_session_map.end())
	{
		return it->second;
	}

	return NULL;
}


