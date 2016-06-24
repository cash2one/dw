
#ifndef MANAGE_VR_SESSION_HPP__
#define MANAGE_VR_SESSION_HPP__

#include <string>
#include <unordered_map>
#include <boost/thread.hpp>
#include "SessionServer.h"
#include "Packet_.h"
#include "StatCount.h"

using namespace std;

struct VrSession
{
	VrSession()
		: area_id(0)
		, gs_index(0)
		, gate_port(0)
		, session_id(0)
	{}

	int area_id;
	int gs_index;
	string gate_ip_addr;
	int gate_port;
	SessionId session_id;
};

struct AreaInfo 
{
	AreaInfo()
	{}

	int area_id;
	typedef unordered_map<int, VrSession *> VrSessionMap_t;
	VrSessionMap_t vr_session_map;
};

class ManageVrSession : public InputStreamHandle
{
public:
	ManageVrSession();
	~ManageVrSession();
public:
	static ManageVrSession * instance();
public:
	int init(const string & ip_addr, int port);

	VrSession * RandGetVrServer();

	VrSession * GetVrServerByIndex(int area_id, int gs_index);

	void addNewVr(SessionId session_id, int area_id, int gs_id, const string & gate_ip_addr, int gate_port);

	void removeVr(SessionId session_id);
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session_id, int error_code);
	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len);

	virtual void handleOutputPacket(InterPacket * pack);
protected:
private:

	SessionServer * m_session_server;

	InterPacketVec_t m_vr_packet_vec;

	boost::mutex m_vr_packet_vec_mtx;

	typedef unordered_map<int, AreaInfo *> AreaInfoMap_t;

	typedef unordered_map<SessionId, VrSession *> VrSessionInfoMap_t;

	AreaInfoMap_t m_area_info_map;
	VrSessionInfoMap_t m_vr_session_info_map;
	//boost::mutex m_vr_session_info_map_mtx;

	static ManageVrSession * m_instance;
};

#endif
