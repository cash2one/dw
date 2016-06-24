
#include <boost/thread.hpp>
#include "MessageIdentifiers.h"
#include "SessionServerRaknet.h"
#include "Logger.h"

#define ID_GAME_MESSAGE_1 ID_USER_PACKET_ENUM + 1

SessionServerRaknet::SessionServerRaknet()
	: m_peer(NULL)
	, m_thread(NULL)
	, m_game_msg_id((char)ID_GAME_MESSAGE_1)
	, m_connection_num(1000)
	, m_connection_index(0)
	, m_is_connecting(false)
{
}

SessionServerRaknet::~SessionServerRaknet()
{

}

int SessionServerRaknet::init(InputStreamHandle * input_stream_handle)
{
	m_input_stream_handle = input_stream_handle;

	m_peer = RakNet::RakPeerInterface::GetInstance();
	m_msg_block.init(10240000);

	boost::function0<int> f = boost::bind(&SessionServerRaknet::svc, this);
	m_thread = new boost::thread(f);

	return 0;
}

int SessionServerRaknet::listen(const string & ip_str, int port)
{
	RakNet::SocketDescriptor sd(port, ip_str.c_str());
	m_peer->Startup(60000, &sd, 1);
	m_peer->SetMaximumIncomingConnections((unsigned short)60000);

	return 0;
}

void SessionServerRaknet::connect(const string & ip_str, int port, bool auto_reconnect)
{
	m_is_connecting = true;

	RakNet::SocketDescriptor sd;
	m_peer->Startup(1000, &sd, 1);

	m_peer->Connect(ip_str.c_str(), port, 0, 0);
}

void SessionServerRaknet::handleOutputStream(SessionId session, char * buffer, size_t buf_len)
{
	RakNet::RakNetGUID guid;
	if (this->getGuid(session, guid))
	{
		if (m_msg_block.space() < buf_len + 1)
		{
			int re_init_size = int(m_msg_block.size() + buf_len + 1);
			m_msg_block.init(re_init_size);
		}
		memcpy(m_msg_block.wtPtr(), &m_game_msg_id, sizeof(m_game_msg_id));
		m_msg_block.wtPtr(sizeof(m_game_msg_id));
		memcpy(m_msg_block.wtPtr(), buffer, buf_len);
		m_msg_block.wtPtr(buf_len);
		
		m_peer->Send(m_msg_block.rdPtr(), (int)m_msg_block.size(), IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 1, guid, false);
		m_msg_block.clear();
	}

	delete [] buffer;
}

void SessionServerRaknet::closeSession(SessionId session_id)
{
	boost::mutex::scoped_lock lock(m_is_closing_session_set_mtx);
	m_is_closing_session_set.insert(session_id);

	//RakNet::RakNetGUID guid;
	//if (this->getGuid(session_id, guid))
	//{
	//	RakNet::AddressOrGUID addr(guid);
	//	m_peer->CloseConnection(addr, true);
	//	this->removeSession(session_id);
	//}
}

int SessionServerRaknet::svc()
{
	RakNet::Packet *packet = NULL;
	bool has_packet = false;
	boost::posix_time::milliseconds empty_sleep_time(1);

	std::unordered_set<SessionId> is_closing_session_set;

	while (true)
	{
		has_packet = false;
		for (packet = m_peer->Receive(); packet; m_peer->DeallocatePacket(packet), packet = m_peer->Receive())
		{
			has_packet = true;
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				this->connectionClose(packet);
				break;
			case ID_REMOTE_CONNECTION_LOST:
				this->connectionClose(packet);
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				this->connectionNew(packet);
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				this->connectionNew(packet);
				break;
			case ID_NEW_INCOMING_CONNECTION:
				this->connectionNew(packet);
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				m_input_stream_handle->handleNewConnectionFailed();
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				this->connectionNew(packet);
				break;
			case ID_CONNECTION_LOST:
				this->connectionClose(packet);
				break;
			case ID_GAME_MESSAGE_1:
				m_input_stream_handle->handleInputStream(this->getSessionId(packet), (char *)(packet->data + 1), packet->length - 1);
				break;
			default:
				DEF_LOG_ERROR("not handle message with identifier %i has arrived.", packet->data[0]);
				break;
			}
		}

		{
			boost::mutex::scoped_lock lock(m_is_closing_session_set_mtx);
			if (m_is_closing_session_set.size() > 0)
			{
				for (auto session_id : m_is_closing_session_set)
				{
					is_closing_session_set.insert(session_id);
				}

				m_is_closing_session_set.clear();
			}
		}

		if (is_closing_session_set.size() > 0)
		{
			for (auto session_id : is_closing_session_set)
			{
				RakNet::RakNetGUID guid;
				if (this->getGuid(session_id, guid))
				{
					RakNet::AddressOrGUID addr(guid);
					m_peer->CloseConnection(addr, true);
					this->removeSession(session_id);
				}
			}
			is_closing_session_set.clear();
		}

		if (!has_packet)
		{
			boost::this_thread::sleep(empty_sleep_time);
		}
	}
}

SessionId SessionServerRaknet::getSessionId(RakNet::Packet * packet)
{
	if (m_is_connecting)
	{
		return this;
	}
	else
	{
		return (SessionId)packet->guid.g;
	}
}

void SessionServerRaknet::connectionNew(RakNet::Packet * packet)
{
	const char * ip_str = packet->systemAddress.ToString(false);
	string ip_addr = NULL == ip_str ? "" : ip_str;
	m_input_stream_handle->handleNewConnection(this->getSessionId(packet), ip_addr, packet->systemAddress.GetPort(), this);
	this->addSession(this->getSessionId(packet), packet->guid);
}

void SessionServerRaknet::connectionClose(RakNet::Packet * packet)
{
	m_input_stream_handle->handleCloseConnection(this->getSessionId(packet), 0);
	this->removeSession(this->getSessionId(packet));
}

void SessionServerRaknet::addSession(SessionId session_id, RakNet::RakNetGUID & guid)
{
	m_session_map[session_id] = guid;
}

void SessionServerRaknet::removeSession(SessionId session_id)
{
	auto it = m_session_map.find(session_id);
	if (it != m_session_map.end())
	{
		m_session_map.erase(it);
	}
}

bool SessionServerRaknet::getGuid(SessionId session_id, RakNet::RakNetGUID & guid)
{
	auto it = m_session_map.find(session_id);
	if (it != m_session_map.end())
	{
		guid = it->second;
		return true;
	}

	return false;
}
