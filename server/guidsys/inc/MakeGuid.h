
#ifndef REQUEST_GUID_HPP
#define REQUEST_GUID_HPP

#include <boost/thread.hpp>
#include <flatbuffers/flatbuffers.h>
#include "typedef.h"
#include "Packet_.h"
#include "SessionServer.h"
#include "SessionHandle.h"
#include "protocol_generated.h"
#include "guid_generated.h"
#include "MyGuidDef.h"
#include "opcode.h"

using namespace MobaGo::Protocol;
using namespace MobaGo::FlatBuffer;

enum EntityType
{
	ET_BASE = 0,
	ET_PLAYER,
	ET_PLAYER_SERVER,
	ET_SUMMONER,
};

struct GuidInfo 
{
	GuidInfo()
	: seed(0)
	, guid_no(0)
	, guid_rest(0)
	, index(0)
	{}
	GuidInfo(uint32 sv, uint32 gnv, uint32 grv, uint64 iv)
	: seed(sv)
	, guid_no(gnv)
	, guid_rest(grv)
	, index(iv)
	{}

	uint32 seed;
	uint32 guid_no;
	uint32 guid_rest;
	uint64 index;
};

struct ConnectionInfo
{
	ConnectionInfo()
		: session_id(0)
		, port(0)
	{}

	time_t last_request_connection_time;
	SessionId session_id;
	string ip_addr;
	int port;
};

struct GuidRequestInfo 
{
	GuidRequestInfo()
		: last_request_time(0)
		, is_requesting(false)
	{}

	time_t last_request_time;
	bool is_requesting;
};

class MakeGuid : public InputStreamHandle
{
public:
	MakeGuid();
	~MakeGuid();
public:
	bool get_guid(EntityType entity_type, uint64 & guid);

	static MakeGuid * instance();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session_id, int error_code);
	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len);

public:
	int init(const string & guid_srv_addr, int port, uint32 once_request_no = 10000u);

	int stop();

	int svc();

protected:

	int connectSrv();

	bool getGuidInfo(GuidInfo & guid_info);

	void checkConnection();

	void checkGuidInfo();

	void doGuidRequest();

	void addInputPacket(InterPacket * pack);

	void handleInputPacket(InterPacket * pack);

	void handleGuidRequest(InterPacket * pack);

	void handleNewConnPack(InterPacket * pack);

	void handleCloseConnPack(InterPacket * pack);

private:
	typedef deque<GuidInfo> GuidInfoDeque_t;

	uint32 m_request_no;

	GuidInfo m_guid_info;

	bool m_is_stop;

	GuidInfoDeque_t m_guid_info_que;

	SessionServer * m_session_server;

	ConnectionInfo m_connection_info;

	boost::thread * m_thread;

	InterPacketVec_t m_input_pack_vec;
	boost::mutex m_input_pack_vec_mtx;

	GuidRequestInfo m_guid_request_info;

	static MakeGuid * m_instance;
};
#endif