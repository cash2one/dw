
#ifndef MANAGER_ROBOT_HPP__
#define MANAGER_ROBOT_HPP__

//#define BOOST_ASIO_HAS_MOVE 1

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "SessionHandle.h"
#include "SessionServer.h"
#include "Robot.h"
#include "Packet_.h"
#include "SessionServer.h"
#include "ManageStat.h"

using namespace std;

class ManagerRobot : public InputStreamHandle
{
public:
	ManagerRobot();
	~ManagerRobot();
public:
	int init(const string & ip_str, int port, int robot_start_index, int robot_num, int net_type);

	int svc();

	void handleInputPacket(ServerPacket * packet);

	void getPacketVec(ServerPacketVec_t & packet_vec);

	Robot * getRobot(SessionId session);
public:
	size_t handleOutputPacket(SessionId session, ClientPacket * packet);
public:
	virtual void handleNewConnection(SessionId session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session, int error_code);
	virtual size_t handleInputStream(SessionId session, char * buffer, size_t buf_len);
protected:
	void initRobot(int robot_num);

	void initRobot(SessionId session);

	void addRobot(Robot * robot, SessionId session);

	string getRobotAccount();

	void processNewSession();

	void processCloseSession();

	void removeRobot(SessionId session);
private:
	typedef unordered_map<SessionId, Robot *> SessionRobotMap_t;

	SessionServer * m_session_server;

	int m_port;

	string m_ip_str;

	int m_robot_number;

	// only this.svc thread add, remove, find this map
	SessionRobotMap_t m_session_robot_map;

	//boost::mutex m_session_robot_map_mtx;

	boost::thread * m_thread;

	typedef vector<ServerPacket *> ServerPacketVec_t;

	ServerPacketVec_t m_server_packet_vec;

	boost::mutex m_server_packet_vec_mtx;

	time_t m_last_update_robot_time;

	uint32_t m_robot_name_index;

	typedef unordered_set<SessionId> SessionSet_t;

	SessionSet_t m_new_session_set;
	SessionSet_t m_close_session_set;

	boost::mutex m_session_mutex;

	ManageStat m_manage_stat;
}; 

#endif
