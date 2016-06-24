
#ifndef MANAGE_SESSION_HPP
#define MANAGE_SESSION_HPP

#include <boost/thread.hpp>

#include "Packet_.h"
#include "AllocGuid.h"
#include "SessionServer.h"
#include "SessionHandle.h"
#include "Packet_.h"
#include "guid_generated.h"
#include "protocol_generated.h"
#include "opcode.h"

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;

class ManageSession : public InputStreamHandle
{
public:
	ManageSession();

	~ManageSession();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session_id, int error_code);
	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len);

	void update();
public:
	virtual int init();

	virtual int stop();

	virtual int finit();
protected:
	void handleInputPacket(InterPacket * pack);

private:
	SessionServer * m_session_server;

	AllocGuid m_alloc_guid;

	InterPacketVec_t m_input_pack_vec;

	boost::mutex m_input_pack_vec_mtx;
};

#endif