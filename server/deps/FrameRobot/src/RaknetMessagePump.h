
#ifndef RAKNET_MESSAGE_PUMP_HPP__
#define RAKNET_MESSAGE_PUMP_HPP__

#include "map"
#include <string>
#include "MessagePump.h"
#include "Packet_.h"

using namespace std;

class RaknetMessagePump : public MessagePumpHandle, MessageServerPump<ServerPacket>
{
public:
	RaknetMessagePump();
	~RaknetMessagePump();

	typedef MessageServerPump<ServerPacket> _PumpType;

	struct ConnectInfo
	{
		ConnectInfo()
			: message_pump(NULL)
			, port(0)
			, session_id(NULL)
		{}

		ConnectInfo(_PumpType * _message_pump, const string & _ip_addr, int _port)
			: message_pump(_message_pump)
			, ip_addr(_ip_addr)
			, port(_port)
		{}

		_PumpType * message_pump;
		string ip_addr;
		int port;
		SessionId session_id;
	};

	typedef map<SessionId, ConnectInfo *> ConnectInfoSessionMap_t;
	

public:
	virtual int init(MessagePumpHandle * msg_pump_handle, int net_type);

	virtual int listen(const string & ip_addr, int port);

	virtual int connect(const string & ip_addr, int port, bool auto_reconnect = false);
	virtual bool addMsgHandle(int msg_id, PacketHandleBase<ServerPacket> * msg_handle);

	virtual int update(size_t & dispatch_pack_num, size_t & dispatch_pack_bytes);
	virtual int handleOutputStream(SessionId session_id, char * buffer, size_t buf_len);
	virtual bool setDefaultMsgHandler(_PumpType::DefaultMsgHandler cb);
	virtual void * getSS();
	virtual void closeSession(SessionId session_id);
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleCloseConnection(SessionId session_id);
public:
	static MessageServerPump<ServerPacket> * createMessagePump(int net_type);
protected:
	ConnectInfo * getConnectInfoByRaknet(SessionId raknet_session);
	ConnectInfo * getConnectInfoBySession(SessionId session_id);
private:
	MessagePumpHandle * m_msg_pump_handle;

	_PumpType::DefaultMsgHandler m_cb;

	ConnectInfoSessionMap_t m_connect_info_session_map;
	ConnectInfoSessionMap_t m_connect_info_raknet_map;
};

#endif
