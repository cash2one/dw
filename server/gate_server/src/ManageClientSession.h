
#ifndef MANAGE_CLIENT_SESSION_HPP__
#define MANAGE_CLIENT_SESSION_HPP__

#include <boost/thread.hpp>
#include <unordered_map>
#include "SessionServer.h"
#include "SessionHandle.h"
#include "Packet_.h"
#include "StatCount.h"
#include "Logger.h"
#include "ManageClientPing.h"

using namespace std;

struct ClientSessionInfo 
{
	ClientSessionInfo()
		: session_id(0)
		, session_guid(0)
		, last_ping_time(0)
		, send_msg_to_gs(false)
	{
		this->last_ping_time = getSystemMilliSeconds();
	}

	ClientSessionInfo(SessionId _session_id, MyGuid_t _session_guid)
		: session_id(_session_id)
		, session_guid(_session_guid)
		, last_ping_time(0)
		, send_msg_to_gs(false)
	{
		this->last_ping_time = getSystemMilliSeconds();
	}

	SessionId session_id;
	MyGuid_t session_guid;
	time_t last_ping_time;
	bool send_msg_to_gs;
};

class ManageClientSession : public InputStreamHandle, public ClientPingTimeoutNotify
{
public:
	ManageClientSession();
	~ManageClientSession();
public:
	static ManageClientSession * instance();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session_id, int error_code);
	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len);
public:
	int init(const string & addr, int port, int net_type);

	int update();

	void handleServerPacket(InterPacketVec_t & pack_vec);

	virtual void pingTimeoutNotify(ClientGuidSet_t & client_guid_set);
protected:
	void handleClientPingTimeout(InterPacket * packet);
	
	void handleServerPacket(InterPacket * packet);

	void handleClientPacket(ClientPacketVec_t & pack_vec);

	void handleClientPacket(ClientPacket * packet);

	void getServerPacket(InterPacketVec_t & pack_vec);

	void getClientPacket(ClientPacketVec_t & pack_vec);

	MyGuid_t getSessionGuid(SessionId session_id);

	ClientSessionInfo * getSessionInfo(SessionId session_id);

	SessionId getSessionIdByGuid(MyGuid_t guid);

	void doHandleNewConnection(ClientPacket * pack);

	void doHandleCloseConnection(ClientPacket * pack);

	void addClientSession(const ClientSessionInfo & cs_info);

	void removeClientSession(SessionId session_id);

private:
	SessionServer * m_session_server;

	ClientPacketVec_t m_client_packet_vec;

	boost::mutex m_client_packet_vec_mtx;

	typedef unordered_map<SessionId, ClientSessionInfo *> ClientSessionInfoMap_t;

	typedef unordered_map<MyGuid_t, ClientSessionInfo *> ClientGuidInfoMap_t;

	ClientSessionInfoMap_t m_client_session_info_map;

	ClientGuidInfoMap_t m_client_guid_info_map;

	InterPacketVec_t m_server_pack_vec;

	boost::mutex m_server_pack_vec_mtx;

	MyGuid_t m_client_guid_index;

	ManageClientPing m_manage_client_ping;

	//ClientGuidSet_t m_ping_timeout_guid_set;

	//boost::mutex m_ping_timeout_guid_set_mtx;

	static ManageClientSession * m_instance;
};
#endif