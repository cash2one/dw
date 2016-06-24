
#ifndef SESSION_SERVER_RAKNET_HPP__
#define SESSION_SERVER_RAKNET_HPP__

#include <unordered_set>
#include <boost/thread.hpp>
#include <boost/unordered_map.hpp>
#include "SessionServer.h"
#include "RakPeerInterface.h"
#include "MsgBlock.h"

class SessionServerRaknet : public SessionServer
{
public:
	SessionServerRaknet();
	~SessionServerRaknet();
public:
	virtual int init(InputStreamHandle * input_stream_handle);

	virtual int listen(const string & ip_str, int port);

	virtual void connect(const string & ip_str, int port, bool auto_reconnect = false);

	virtual void handleOutputStream(SessionId session, char * buffer, size_t buf_len);

	virtual void closeSession(SessionId session_id);

public:
	int svc();
protected:
	SessionId getSessionId(RakNet::Packet * packet);

	void connectionNew(RakNet::Packet * packet);

	void connectionClose(RakNet::Packet * packet);

	void addSession(SessionId session_id, RakNet::RakNetGUID & guid);

	void removeSession(SessionId session_id);

	bool getGuid(SessionId session_id, RakNet::RakNetGUID & guid);
private:
	RakNet::RakPeerInterface * m_peer;

	boost::thread * m_thread;

	InputStreamHandle * m_input_stream_handle;

	boost::unordered_map<SessionId, RakNet::RakNetGUID> m_session_map;

	MsgBlock m_msg_block;

	char m_game_msg_id;

	int m_connection_num;

	int m_connection_index;

	vector<RakNet::SocketDescriptor> m_startup_for_connect_sock_des_vec;

	std::unordered_set<SessionId> m_is_closing_session_set;

	boost::mutex m_is_closing_session_set_mtx;

	bool m_is_connecting;
};

#endif
