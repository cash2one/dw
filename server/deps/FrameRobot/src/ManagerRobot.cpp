
#include "ManagerRobot.h"
#include <boost/thread.hpp>
#include "Packet_.h"
#include "SessionServer.h"

ManagerRobot::ManagerRobot()
	: m_session_server(NULL)
	, m_port(0)
	, m_robot_number(100)
	, m_thread(NULL)
	, m_last_update_robot_time(0)
	, m_robot_name_index(0)
{

}

ManagerRobot::~ManagerRobot()
{
	if (NULL != m_thread)
	{
		delete m_thread;
		m_thread = NULL;
	}
}

int ManagerRobot::init(const string & ip_str, int port, int robot_start_index, int robot_num, int net_type)
{
	m_ip_str = ip_str;
	m_port = port;
	m_robot_name_index = robot_start_index;
	m_robot_number = robot_num;
	m_session_server = SessionServer::createSessionServer((NetProtocolType)net_type);
	m_session_server->init(this);

	this->initRobot(robot_num);

	boost::function0<int> f = boost::bind(&ManagerRobot::svc, this);
	m_thread = new boost::thread(f);

	return 0;
}

int ManagerRobot::svc()
{
	ServerPacketVec_t packet_vec;
	while (true)
	{
		this->processNewSession();
		this->processCloseSession();
		this->getPacketVec(packet_vec);
		for (ServerPacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
		{
			ServerPacket * pack = *it;
			Robot * robot = this->getRobot((SessionId)pack->session_id);
			if (NULL != robot)
			{
				robot->handlePacket(pack);
			}

			m_manage_stat.addIncomingPacket(1, pack->getStreamSize());

			delete pack;
		}
		packet_vec.clear();

		boost::posix_time::time_duration curr_time = boost::posix_time::microsec_clock::local_time().time_of_day();
		if (curr_time.total_milliseconds() > m_last_update_robot_time + 50)
		{
			m_last_update_robot_time = curr_time.total_milliseconds();
			for (auto it : m_session_robot_map)
			{
				Robot * robot = it.second;
				robot->update();
			}
		}
		else
		{
			boost::system_time sleep_time = boost::get_system_time() + boost::posix_time::millisec(10);
			boost::thread::sleep(sleep_time);
		}

		m_manage_stat.update();
	}
	return 0;
}

void ManagerRobot::handleInputPacket(ServerPacket * packet)
{
	boost::mutex::scoped_lock lock(m_server_packet_vec_mtx);
	m_server_packet_vec.push_back(packet);
}

void ManagerRobot::getPacketVec(ServerPacketVec_t & packet_vec)
{
	boost::mutex::scoped_lock lock(m_server_packet_vec_mtx);
	std::copy(m_server_packet_vec.begin(), m_server_packet_vec.end(), std::back_inserter(packet_vec));
	m_server_packet_vec.clear();
}

Robot * ManagerRobot::getRobot(SessionId session)
{
	//boost::mutex::scoped_lock lock(m_session_robot_map_mtx);
	SessionRobotMap_t::iterator it = m_session_robot_map.find(session);
	if (it != m_session_robot_map.end())
	{
		return it->second;
	}

	return NULL;
}

size_t ManagerRobot::handleOutputPacket(SessionId session, ClientPacket * packet)
{
	m_manage_stat.addOutgoingPacket(1, packet->getStreamSize());

	m_session_server->handleOutputStream(session, packet->getStream(), packet->getStreamSize());
	packet->cleanStream();
	delete packet;
	return 0;
}

void ManagerRobot::handleNewConnection(SessionId session)
{
	DEF_LOG_INFO("success to connect <%s:%d>", m_ip_str.c_str(), m_port);
	//this->initRobot(session);
	boost::mutex::scoped_lock lock(m_session_mutex);

	m_new_session_set.insert(session);
}

void ManagerRobot::handleNewConnectionFailed()
{
	DEF_LOG_ERROR("failed to connect <%s:%d>", m_ip_str.c_str(), m_port);
}

void ManagerRobot::handleCloseConnection(SessionId session, int error_code)
{
	boost::mutex::scoped_lock lock(m_session_mutex);
	m_close_session_set.insert(session);
}

size_t ManagerRobot::handleInputStream(SessionId session, char * buffer, size_t buf_len)
{
	PackHead msg_head;
	ServerPacket * packet;
	size_t pos = 0;
	while (ServerPacket::pack_head_size <= (int)(buf_len - pos))
	{
		memcpy(&msg_head, buffer + pos, ServerPacket::pack_head_size);

		if (msg_head.msg_len > 10000)
		{
			DEF_LOG_ERROR("get too big msg head, msg is <%hd>, msg len is <%hd>", msg_head.msg_id, msg_head.msg_len);
		}
		if (msg_head.msg_len + ServerPacket::pack_head_size <= (int)(buf_len - pos))
		{
			packet = new ServerPacket();
			// make packet
			packet->setMsgInfo(msg_head, buffer + pos + ServerPacket::pack_head_size);
			pos += ServerPacket::pack_head_size + msg_head.msg_len;
			packet->session_id = session;
			//MessageManager::instance()->handlePlayerMsg((int)session, packet);
			//MessageManager::instance()->handlePlayerPacket(packet);
			this->handleInputPacket(packet);
		}
		else
		{
			// 
			break;
		}
	}

	return pos;
}

void ManagerRobot::initRobot(int robot_num)
{
	for (int i = 0; i < robot_num; ++i)
	{
		m_session_server->connect(m_ip_str, m_port);
		//SessionId session = 
		//if (NULL != session)
		//{
		//	Robot * robot = new Robot();
		//	robot->init(this->getRobotAccount(), 211, session, this);
		//	m_session_robot_map[session] = robot;
		//}
		//else
		//{
		//	DEF_LOG_ERROR("failed to connect <%s:%d>", m_ip_str.c_str(), m_port);
		//}
	}
}

void ManagerRobot::initRobot(SessionId session)
{
	//Robot * robot = new Robot();
	//robot->init(this->getRobotAccount(), 211, session, this);
	//this->addRobot(robot, session);
}

void ManagerRobot::addRobot(Robot * robot, SessionId session)
{
	//boost::mutex::scoped_lock lock(m_session_robot_map_mtx);
	m_session_robot_map[session] = robot;
}

string ManagerRobot::getRobotAccount()
{
	char name[100];
	sprintf(name, "robot_%d", m_robot_name_index);
	++m_robot_name_index;
	return name;
}

void ManagerRobot::processNewSession()
{
	boost::mutex::scoped_lock lock(m_session_mutex);
	for (auto session : m_new_session_set)
	{
		this->initRobot(session);
	}

	m_new_session_set.clear();
}

void ManagerRobot::processCloseSession()
{
	boost::mutex::scoped_lock lock(m_session_mutex);
	for (auto session : m_close_session_set)
	{
		this->removeRobot(session);
	}

	m_close_session_set.clear();
}

void ManagerRobot::removeRobot(SessionId session)
{
	SessionRobotMap_t::iterator it = m_session_robot_map.find(session);
	if (it != m_session_robot_map.end())
	{
		delete it->second;
		m_session_robot_map.erase(it);
	}
}
