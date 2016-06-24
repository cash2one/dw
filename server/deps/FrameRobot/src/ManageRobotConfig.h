
#ifndef MANAGE_ROBOT_CONFIG_HPP__
#define MANAGE_ROBOT_CONFIG_HPP__

#include <flatbuffers/flatbuffers.h>
#include "SessionServer.h"
#include "SessionHandle.h"
#include "Packet_.h"
#include "robot_generated.h"

using namespace MobaGo::FlatBuffer;

struct RobotCfg
{
	RobotCfg()
		: port(0)
		, robot_start_index(0)
		, robot_num(0)
		, net_type(0)
	{}

	string ip_addr;
	int port;
	int robot_start_index;
	int robot_num;
	int net_type;

	// 以下通过配置表加载
	std::string account_name_prefix;
	int account_index_begin = 0;
	int account_index_end = 0;
	int login_once_num = 0;
	int login_interval = 0;
	int logoff_once_num = 0;
	int logoff_interval = 0;
	int random_create_name_begin_index = 0;
	int random_create_name_end_index = 0;
	int search_friend_times = 0;
	int confirm_add_prob = 0;
};

class ManageRobotConfig : public InputStreamHandle
{
public:
	ManageRobotConfig();
	~ManageRobotConfig();
	static ManageRobotConfig * instance();
public:
	virtual void handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session);
	virtual void handleNewConnectionFailed();
	virtual void handleCloseConnection(SessionId session_id, int error_code);
	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len);

public:
	void init(const string & ip_addr, int port, int net_type);

	bool isGotRobotCfg();

	RobotCfg & getRobotCfg();
protected:
	void handleInputPacket(ClientPacket * packet);

	void handleReqCfgAck(ClientPacket * packet);
private:
	SessionServer * m_session_server;

	RobotCfg m_robot_cfg;

	bool m_get_robot_cfg;
};

#endif
