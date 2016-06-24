
#include "ManageVrRobot.h"
#include "ManageBattleRobot.h"
#include "ManageRobotConfig.h"
#include "ClientProtocol.h"
#include "pluginDepot.h"
#include "Single.hpp"
#include "FRPlugin.h"

#include "RaknetMessagePump.h"

#define GET_ROBOT_AND_RETURN()	\
	Robot * robot = this->getRobot(input_packet->session_id);	\
	if (NULL == robot)	\
			{	\
		return -1;	\
			}

ManageVrRobot::ManageVrRobot()
	: m_message_pump(NULL)
{
	
}

ManageVrRobot::~ManageVrRobot()
{


}

ManageVrRobot * ManageVrRobot::instance()
{
	return Singleton<ManageVrRobot>::instance();
}

void ManageVrRobot::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	Robot * robot = this->getAvilableRobot();
	if (NULL == robot)
	{
		return;
	}

	robot->setSession(session_id, this);
	robot->setRobotStatus(RS_Connected_vr);
	Single<FRPlugin>::GetInstance()->notifyCreateVRRobot(session_id, robot);
	Single<RunningRobot>::GetInstance()->running[session_id] = robot;
	this->enableRobot(robot);
}

void ManageVrRobot::handleCloseConnection(SessionId session_id)
{
	Single<FRPlugin>::GetInstance()->notifyRemoveVRRobot(session_id);
}


int ManageVrRobot::init()
{
	//m_message_pump = MessageServerPump<ServerPacket>::createMessageServerPump();
	m_message_pump = RaknetMessagePump::createMessagePump(ManageRobotConfig::instance()->getRobotCfg().net_type);
	m_message_pump->init(this, ManageRobotConfig::instance()->getRobotCfg().net_type);

	//this->registerMsgHandle();

	m_message_pump->setDefaultMsgHandler(std::bind(&RunningRobot::packet_input, Single<RunningRobot>::GetInstance(),
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	return 0;
}

void ManageVrRobot::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_message_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	this->updateRobot();
}

void ManageVrRobot::handleRobot(Robot * robot)
{
	m_avilable_robot_list.push_back(robot);
}

void ManageVrRobot::handleOutputPacket(ClientPacket * packet)
{
	m_message_pump->handleOutputStream(packet->session_id, packet->getStream(), packet->getStreamSize());
	packet->cleanStream();
	delete packet;
}

int ManageVrRobot::handleMsgVRLoginAck(ServerPacket * input_packet, const MsgVRLoginAck * input_msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgVRLoginAck(input_packet->session_id, input_msg);

	return 0;
}

int ManageVrRobot::handleMsgMatchAck(ServerPacket * input_packet, const MsgMatchAck * input_msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgMatchAck(input_packet->session_id, input_msg);
	return 0;
}

int ManageVrRobot::handleMsgMatchEnterBattle(ServerPacket * input_packet, const MsgMatchEnterBattle * input_msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgMatchEnterBattle(input_packet->session_id, input_msg);

	this->removeRobot(robot);

	ManageBattleRobot::instance()->handleRobot(robot);
	return 0;
}

int ManageVrRobot::handleMsgReconnectBattle(ServerPacket * input_packet, const MsgReconnectBattle * input_msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgReconnectBattle(input_packet->session_id, input_msg);

	this->removeRobot(robot);

	ManageBattleRobot::instance()->handleRobot(robot);
	return 0;
}

int ManageVrRobot::handleMsgError(ServerPacket * input_packet, const MsgError * input_msg)
{
	GET_ROBOT_AND_RETURN();
	DEF_LOG_ERROR("get error message, user id is <%llu>, error code is <%d>", robot->getPlayerInfo()->user_id, input_msg->error_code());

	robot->handleMsgError(input_packet->session_id, input_msg);
	return 0;
}

//int ManageVrRobot::handleMsgVRLoginAck(ServerPacket * input_packet, const MsgVRLoginAck * input_msg)
//{
//	GET_ROBOT_AND_RETURN();
//
//	robot->handleMsgVRLoginAck(input_packet->session_id, input_msg);
//	return 0;
//}

int ManageVrRobot::handleMsgReqSummonerInfoAck(ServerPacket * input_packet, const MsgReqSummonerInfoAck * input_msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgReqSummonerInfoAck(input_packet->session_id, input_msg);
	return 0;
}

int ManageVrRobot::handleMsgCreateSummonerAck(ServerPacket * input_packet, const EmptyMsg * input_msg)
{
	GET_ROBOT_AND_RETURN();

	ClientPacket * pack = ClientProtocol::makeDemoSystemTestReq(input_packet->session_id);
	robot->sendMsg(pack);

	robot->handleMsgCreateSummonerAck(input_packet->session_id, input_msg);
	return 0;
}

int ManageVrRobot::handleMsgCancelMatchAck(ServerPacket * input_packet, const MsgCancelMatchAck * input_msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgCancelMatchAck(input_packet->session_id, input_msg);
	return 0;
}

int ManageVrRobot::handleMsgSearchFriendAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgSearchFriendAck * msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgSearchFriendAck(input_packet->session_id, msg);
	return 0;
}

int ManageVrRobot::handleMsgAddFriendAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgAddFriendAck * msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgAddFriendAck(input_packet->session_id, msg);
	return 0;
}
int ManageVrRobot::handleMsgDelFriendAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgDelFriendAck * msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgDelFriendAck(input_packet->session_id, msg);
	return 0;
}
int ManageVrRobot::handleMsgConfirmAddFriendAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgConfirmAddFriendAck * msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgConfirmAddFriendAck(input_packet->session_id, msg);
	return 0;
}
int ManageVrRobot::handleMsgAddCancelFriendAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgAddCancelFriendAck * msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgAddCancelFriendAck(input_packet->session_id, msg);
	return 0;
}

int ManageVrRobot::handleMsgFriendListAck(ServerPacket * input_packet, const MsgFriendListAck * msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgFriendListAck(input_packet->session_id, msg);
	return 0;
}

int ManageVrRobot::handleMsgApplyForFriendListAck(ServerPacket * input_packet, const MsgApplyForFriendListAck * msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgApplyForFriendListAck(input_packet->session_id, msg);
	return 0;
}
int ManageVrRobot::handleMsgFriendDetailInfoAck(ServerPacket * input_packet, const MobaGo::FlatBuffer::MsgFriendDetailInfoAck * msg)
{
	GET_ROBOT_AND_RETURN();

	robot->handleMsgFriendDetailInfoAck(input_packet->session_id, msg);
	return 0;
}

int ManageVrRobot::clientInput(int msg_id, ServerPacket* packet, char * buffer, int buffer_size)
{	
	std::unique_ptr<InterPacket> dest(new InterPacket);
	dest->session_id = packet->session_id;
	dest->setMsgInfo(msg_id, 0, buffer_size, buffer);
	
	return Single<PluginDepot>::GetInstance()->input(msg_id, dest.get(), buffer, buffer_size);
}

void ManageVrRobot::registerMsgHandle()
{
	//m_message_pump->addMsgHandle(Opcode_E_MsgVRLoginReq, new PacketHandle<ManageVrRobot, ServerPacket, MsgVRLoginAck>(this, &ManageVrRobot::handleMsgVRLoginAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgError, new PacketHandle<ManageVrRobot, ServerPacket, MsgError>(this, &ManageVrRobot::handleMsgError));

	m_message_pump->addMsgHandle(Opcode_E_MsgVRLoginAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgVRLoginAck>(this, &ManageVrRobot::handleMsgVRLoginAck));

	m_message_pump->addMsgHandle(Opcode_E_MsgReqSummonerInfoAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgReqSummonerInfoAck>(this, &ManageVrRobot::handleMsgReqSummonerInfoAck));

	m_message_pump->addMsgHandle(Opcode_E_MsgCreateSummonerAck, new PacketHandle<ManageVrRobot, ServerPacket, EmptyMsg>(this, &ManageVrRobot::handleMsgCreateSummonerAck));

	m_message_pump->addMsgHandle(Opcode_E_MsgMatchAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgMatchAck>(this, &ManageVrRobot::handleMsgMatchAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgCancelMatchAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgCancelMatchAck>(this, &ManageVrRobot::handleMsgCancelMatchAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgMatchEnterBattle, new PacketHandle<ManageVrRobot, ServerPacket, MsgMatchEnterBattle>(this, &ManageVrRobot::handleMsgMatchEnterBattle));

	m_message_pump->addMsgHandle(Opcode_E_MsgSearchFriendAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgSearchFriendAck>(this, &ManageVrRobot::handleMsgSearchFriendAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgAddFriendAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgAddFriendAck>(this, &ManageVrRobot::handleMsgAddFriendAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgDelFriendAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgDelFriendAck>(this, &ManageVrRobot::handleMsgDelFriendAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgConfirmAddFriendAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgConfirmAddFriendAck>(this, &ManageVrRobot::handleMsgConfirmAddFriendAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgAddCancelFriendAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgAddCancelFriendAck>(this, &ManageVrRobot::handleMsgAddCancelFriendAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgFriendListAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgFriendListAck>(this, &ManageVrRobot::handleMsgFriendListAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgApplyForFriendListAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgApplyForFriendListAck>(this, &ManageVrRobot::handleMsgApplyForFriendListAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgFriendDetailInfoAck, new PacketHandle<ManageVrRobot, ServerPacket, MsgFriendDetailInfoAck>(this, &ManageVrRobot::handleMsgFriendDetailInfoAck));

	m_message_pump->addMsgHandle(Opcode_E_MsgReconnectBattle, new PacketHandle<ManageVrRobot, ServerPacket, MsgReconnectBattle>(this, &ManageVrRobot::handleMsgReconnectBattle));
		

}

Robot * ManageVrRobot::getAvilableRobot()
{
	Robot * ret = NULL;
	if (m_request_connection_robot_list.size() > 0)
	{
		ret = m_request_connection_robot_list.front();
		m_request_connection_robot_list.pop_front();
	}

	return ret;
}

void ManageVrRobot::enableRobot(Robot * robot)
{
	m_enable_robot_list.push_back(robot);

	m_robot_session_map.insert(std::make_pair(robot->getSession(), robot));
}

void ManageVrRobot::updateRobot()
{
	for (auto it : m_enable_robot_list)
	{
		Robot * robot = it;
		robot->update();
	}

	if (m_avilable_robot_list.size() > 0)
	{
		for (auto it : m_avilable_robot_list)
		{
			Robot * robot = it;
			m_message_pump->connect(robot->getVrIp(), robot->getVrPort());

			m_request_connection_robot_list.push_back(robot);
		}

		m_avilable_robot_list.clear();
	}
}

Robot * ManageVrRobot::getRobot(SessionId session_id)
{
	auto it = m_robot_session_map.find(session_id);
	if (it != m_robot_session_map.end())
	{
		return it->second;
	}

	return NULL;
}

void ManageVrRobot::removeRobot(Robot * robot)
{
	auto it = m_robot_session_map.find(robot->getSession());
	if (it != m_robot_session_map.end())
	{
		m_robot_session_map.erase(it);
	}

	for (auto it = m_enable_robot_list.begin(); it != m_enable_robot_list.end(); ++it)
	{
		if (robot == *it)
		{
			m_enable_robot_list.erase(it);
			break;
		}
	}
}

