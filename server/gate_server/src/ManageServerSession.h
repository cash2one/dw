
#ifndef MANAGE_SERVER_SESSION_HPP__
#define MANAGE_SERVER_SESSION_HPP__

#include <string>
#include <boost/thread.hpp>
#include "SessionServer.h"
#include "SessionHandle.h"
#include "Packet_.h"
#include "protocol_generated.h"
#include "inter_generated.h"
#include "opcode.h"

using namespace std;
using namespace MobaGo::Protocol;
using namespace flatbuffers;
using namespace MobaGo::FlatBuffer;

struct ServerInfo 
{
	ServerInfo()
		: session_id(NULL)
		, is_validation(false)
	{}

	SessionId session_id;
	bool is_validation;
};

class ManageServerSession : public InputStreamHandle
{
public:
	ManageServerSession();
	~ManageServerSession();
public:
	static ManageServerSession * instance();
public:
	int init(const string & ip_addr, int port);

	void handleClientPacket(InterPacket * inter_pack);

	void handleClientPacket(InterPacketVec_t & pack_vec);

	void notifyServerSocketClosed(uint64 gate_guid);

	int svc();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session_id, int error_code);
	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len);

protected:
	void handlePacket(InterPacketVec_t & pack_vec);

	void getClientPacket(InterPacketVec_t & pack_vec, InterPacketVec_t & server_event_pack_vec);

	void doHandleServerEventPacket(InterPacket * packet);

	void doHandleClientPacket(InterPacket * packet);

	void registerGate(SessionId session_id);
private:
	SessionServer * m_session_server;

	boost::thread * m_thread;

	InterPacketVec_t m_client_packet_vec;

	boost::mutex m_client_packet_vec_mtx;

	ServerInfo m_server_info;

	InterPacketVec_t m_server_event_packet_vec;

	static ManageServerSession * m_instance;

};

#endif
