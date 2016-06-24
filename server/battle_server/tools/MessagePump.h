
#ifndef MESSAGE_PUMP_HPP__
#define MESSAGE_PUMP_HPP__

#include <unordered_map>
#include "SessionHandle.h"
#include "SessionServer.h"
#include "Packet_.h"
#include "PackMsg.h"
#include "PacketHandle.h"
#include "opcode.h"

template<class _PackType>
class MessageServerPump;

class MessagePumpHandle
{
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session) = 0;
	virtual void handleCloseConnection(SessionId session_id) = 0;
};

template<class _PackType>
class MessageServerPump
{
public:
	virtual int init(MessagePumpHandle * msg_pump_handle, int net_type) = 0;
	virtual int listen(const string & ip_addr, int port) = 0;
	virtual int connect(const string & ip_addr, int port, bool auto_reconnect = false) = 0;
	virtual bool addMsgHandle(int msg_id, PacketHandleBase<_PackType> * msg_handle) = 0;
	virtual int update(size_t & dispatch_pack_num, size_t & dispatch_pack_bytes) = 0;
	virtual int handleOutputStream(SessionId session_id, char * buffer, size_t buf_len) = 0;
	static MessageServerPump<_PackType> * createMessageServerPump();

	typedef std::function< int(int, _PackType*, char*, int) > DefaultMsgHandler;
	virtual bool setDefaultMsgHandler(DefaultMsgHandler cb) = 0;
	virtual void * getSS() = 0;
	virtual void closeSession(SessionId session_id) = 0;
};

template<class _PackType>
class MessageServerPumpImp : public InputStreamHandle, public MessageServerPump<_PackType>
{
public:
	MessageServerPumpImp();
	~MessageServerPumpImp();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session_id, int error_code);
	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len);

public:
	virtual int init(MessagePumpHandle * msg_pump_handle, int net_type);
	virtual int listen(const string & ip_addr, int port);
	virtual int connect(const string & ip_addr, int port, bool auto_reconnect = false);
	virtual bool addMsgHandle(int msg_id, PacketHandleBase<_PackType> * msg_handle);
	virtual int update(size_t & dispatch_pack_num, size_t & dispatch_pack_bytes);
	virtual int handleOutputStream(SessionId session_id, char * buffer, size_t buf_len);
	virtual bool setDefaultMsgHandler(typename MessageServerPump<_PackType>::DefaultMsgHandler cb) override { m_default_msg_cb = cb;  return true; }
	virtual void * getSS() { return m_server_session; }
	virtual void closeSession(SessionId session_id) { m_server_session->closeSession(session_id); }

protected:
	typedef vector<_PackType *> PacketVec_t;

	void handleInputPacket(vector<_PackType *> & pack_vec);

	void handleInputPacket(_PackType * packet);

	void getInputPacket(PacketVec_t & pack_vec);
private:
	typedef unordered_map<int, PacketHandleBase<_PackType> *> PacketHandleMap_t;

	SessionServer * m_server_session;

	MessagePumpHandle * m_msg_pump_handle;

	PacketHandleMap_t m_msg_handle_map;

	PacketVec_t m_input_packet;

	boost::mutex m_input_packet_mtx;

	typename MessageServerPump<_PackType>::DefaultMsgHandler m_default_msg_cb;
};

template<class _PackType>
MessageServerPump<_PackType> * MessageServerPump<_PackType>::createMessageServerPump()
{
	return new MessageServerPumpImp<_PackType>();
}

template<class _PackType>
MessageServerPumpImp<_PackType>::MessageServerPumpImp()
	: m_server_session(NULL)
	, m_msg_pump_handle(NULL)
{

}

template<class _PackType>
MessageServerPumpImp<_PackType>::~MessageServerPumpImp()
{

}


template<class _PackType>
void MessageServerPumpImp<_PackType>::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	_PackType * packet = NULL;
	makeEmptyMsgPacket(&packet, MSG_NEW_CLIENT_SESSION);
	packet->session_id = session_id;

	this->handleInputPacket(packet);
}

template<class _PackType>
void MessageServerPumpImp<_PackType>::handleNewConnectionFailed()
{

}

template<class _PackType>
void MessageServerPumpImp<_PackType>::handleCloseConnection(SessionId session_id, int error_code)
{
	_PackType * packet = NULL;
	makeEmptyMsgPacket(&packet, MSG_CLOSE_CLIENT_SESSION);
	packet->session_id = session_id;

	this->handleInputPacket(packet);
}

template<class _PackType>
size_t MessageServerPumpImp<_PackType>::handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	vector<_PackType *> pack_vec;
	size_t ret = parsePacketFromStream(session_id, buffer, buf_len, pack_vec);
	this->handleInputPacket(pack_vec);
	return ret;
}

template<class _PackType>
int MessageServerPumpImp<_PackType>::init(MessagePumpHandle * msg_pump_handle, int net_type)
{
	m_msg_pump_handle = msg_pump_handle;

	m_server_session = SessionServer::createSessionServer((NetProtocolType)net_type);
	m_server_session->init(this);
	return 0;
}

template<class _PackType>
int MessageServerPumpImp<_PackType>::listen(const string & ip_addr, int port)
{
	m_server_session->listen(ip_addr, port);
	return 0;
}

template<class _PackType>
int MessageServerPumpImp<_PackType>::connect(const string & ip_addr, int port, bool auto_reconnect)
{
	m_server_session->connect(ip_addr, port, auto_reconnect);
	return 0;
}

template<class _PackType>
bool MessageServerPumpImp<_PackType>::addMsgHandle(int msg_id, PacketHandleBase<_PackType> * msg_handle)
{
	auto it = m_msg_handle_map.find(msg_id);
	if (it != m_msg_handle_map.end())
	{
		// error
		DEF_LOG_ERROR("add reduplication msg handle <%d>", msg_id);
		return 1;
	}

	m_msg_handle_map.insert(std::make_pair(msg_id, msg_handle));
	return 0;
}

template<class _PackType>
int MessageServerPumpImp<_PackType>::update(size_t & dispatch_pack_num, size_t & dispatch_pack_bytes)
{
	PacketVec_t input_packet;
	this->getInputPacket(input_packet);
	for (auto pack : input_packet)
	{
		DEF_LOG_DEBUG("recv msg : <%hd>, len is <%hd>", pack->msg_head.msg_id, pack->msg_head.msg_len);

		auto msg_it = m_msg_handle_map.find(pack->msg_head.msg_id);
		if (msg_it != m_msg_handle_map.end())
		{
			// 
			PacketHandleBase<_PackType> * pack_handle = msg_it->second;
			pack_handle->handlePacketMsg(pack, pack->getBody(), pack->msg_head.msg_len);
		}
		else if (MSG_NEW_CLIENT_SESSION == pack->msg_head.msg_id)
		{
			m_msg_pump_handle->handleNewConnection(pack->session_id, "", 0, m_server_session);
		}
		else if (MSG_CLOSE_CLIENT_SESSION == pack->msg_head.msg_id)
		{
			m_msg_pump_handle->handleCloseConnection(pack->session_id);
		}
		else
		{	
			if (m_default_msg_cb)
				m_default_msg_cb(pack->msg_head.msg_id, pack, pack->getBody(), pack->msg_head.msg_len);
			else
				DEF_LOG_ERROR("do not handle message, msg id is <%hd>, msg len is <%hd>", pack->msg_head.msg_id, pack->msg_head.msg_len);
		}

		delete pack;
	}
	return 0;

}

template<class _PackType>
int MessageServerPumpImp<_PackType>::handleOutputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	m_server_session->handleOutputStream(session_id, buffer, buf_len);
	return 0;
}

template<class _PackType>
void MessageServerPumpImp<_PackType>::handleInputPacket(vector<_PackType *> & pack_vec)
{
	boost::mutex::scoped_lock lock(m_input_packet_mtx);
	std::copy(pack_vec.begin(), pack_vec.end(), std::back_inserter(m_input_packet));
}

template<class _PackType>
void MessageServerPumpImp<_PackType>::handleInputPacket(_PackType * packet)
{
	boost::mutex::scoped_lock lock(m_input_packet_mtx);
	m_input_packet.push_back(packet);
}

template<class _PackType>
void MessageServerPumpImp<_PackType>::getInputPacket(PacketVec_t & pack_vec)
{
	boost::mutex::scoped_lock lock(m_input_packet_mtx);
	std::copy(m_input_packet.begin(), m_input_packet.end(), std::back_inserter(pack_vec));
	m_input_packet.clear();
}

#endif
