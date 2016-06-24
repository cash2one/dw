
#ifndef MANAGER_CONFIG_HPP__
#define MANAGER_CONFIG_HPP__

#include <queue>
#include <boost/thread.hpp>
#include <flatbuffers/flatbuffers.h>
#include "robot_generated.h"
#include "SessionHandle.h"
#include "SessionServer.h"
#include "Packet_.h"
#include "Config.h"
#include "BattleConfig.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;

class ManagerConfig : public InputStreamHandle
{
public:
	ManagerConfig();
	~ManagerConfig();
public:
	int init();

	int svc();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session_id, int error_code);
	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len);

protected:
	void handleInputPacket(ClientPacket * packet);

	void handlePacket(ClientPacket * packet);

	void handleRequestRobotConfig(ClientPacket * packet);
private:
	SessionServer * m_session_server;
	Config m_cfg;
	boost::thread * m_thread;
	ClientPacketVec_t m_input_packet;
	boost::mutex m_input_packet_mutex;

	int m_assign_robot_amount;

};

#endif
