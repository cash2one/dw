

#include "ManageBattleRobot.h"
#include "ManageRobotConfig.h"
#include "Logger.h"
#include "ClientProtocol.h"
#include "Single.hpp"
#include "pluginDepot.h"
#include "RaknetMessagePump.h"

ManageBattleRobot::ManageBattleRobot()
	: m_message_pump(NULL)
{

}

ManageBattleRobot::~ManageBattleRobot()
{

}

ManageBattleRobot * ManageBattleRobot::instance()
{
	return Singleton<ManageBattleRobot>::instance();
}

void ManageBattleRobot::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	Robot * robot = this->getAvilableRobot();
	if (NULL == robot)
	{
		DEF_LOG_ERROR("failed to get avilable battle robot");
	}

	robot->setSession(session_id, this);
	robot->setRobotStatus(RS_Connected_battle);

	m_enable_robot_list.push_back(robot);


}

void ManageBattleRobot::handleCloseConnection(SessionId session_id)
{

}

int ManageBattleRobot::init()
{
	//m_message_pump = MessageServerPump<ServerPacket>::createMessageServerPump();
	m_message_pump = RaknetMessagePump::createMessagePump(ManageRobotConfig::instance()->getRobotCfg().net_type);
	m_message_pump->init(this, ManageRobotConfig::instance()->getRobotCfg().net_type);

	m_message_pump->setDefaultMsgHandler(std::bind(&RunningRobot::packet_input, Single<RunningRobot>::GetInstance(),
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	m_message_pump->addMsgHandle(Opcode_E_MsgEnterBattleAck, new PacketHandle<ManageBattleRobot, ServerPacket, EmptyMsg>(this, &ManageBattleRobot::handleMsgEnterBattleAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgReEnterBattleAck, new PacketHandle<ManageBattleRobot, ServerPacket, MsgReEnterBattleAck>(this, &ManageBattleRobot::handleMsgReEnterBattleAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgSelectHeroAck, new PacketHandle<ManageBattleRobot, ServerPacket, MsgSelectHeroAck>(this, &ManageBattleRobot::handleMsgSelectHeroAck));
	m_message_pump->addMsgHandle(Opcode_E_MsgConfirmHeroAck, new PacketHandle<ManageBattleRobot, ServerPacket, MsgConfirmHeroAck>(this, &ManageBattleRobot::handleMsgConfirmHeroAck));

	m_message_pump->addMsgHandle(Protocol_Lobby_E_PROTOCOL_LOBBY_GAME_START_ACK, new PacketHandle<ManageBattleRobot, ServerPacket, MsgLobbyGameStartAck>(this, &ManageBattleRobot::handleMsgLobbyGameStartAck));

	m_message_pump->addMsgHandle(Protocol_Combat_E_PROTOCOL_COMBAT_BATTLE_START_ACK, new PacketHandle<ManageBattleRobot, ServerPacket, MsgCombatPlayerReadyAck>(this, &ManageBattleRobot::handleMsgCombatPlayerReadyAck));

	m_message_pump->addMsgHandle(Protocol_Combat_E_PROTOCOL_COMBAT_MSG_SERVER_FRAME_SYNC_ACK, new PacketHandle<ManageBattleRobot, ServerPacket, MsgFrameSyncDetail>(this, &ManageBattleRobot::handleMsgFrameSyncDetail));

	

	return 0;
}

void ManageBattleRobot::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_message_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	this->updateRobot();
}

void ManageBattleRobot::handleRobot(Robot * robot)
{

	m_avilable_robot_list.push_back(robot);

	m_message_pump->connect(robot->getBattleIp(), robot->getBattlePort());

}

void ManageBattleRobot::handleOutputPacket(ClientPacket * packet)
{
	m_message_pump->handleOutputStream(packet->session_id, packet->getStream(), packet->getStreamSize());
	packet->cleanStream();
	delete packet;
}

int ManageBattleRobot::handleMsgEnterBattleAck(ServerPacket * input_packet, const EmptyMsg * input_msg)
{
	DEF_LOG_INFO("recv enter battle msg");

	{
		ClientPacket * pack = ClientProtocol::makeMsgSelectHeroReq(input_packet->session_id, 704);
		pack->session_id = input_packet->session_id;

		m_message_pump->handleOutputStream(input_packet->session_id, pack->getStream(), pack->getStreamSize());

		pack->cleanStream();
		delete pack;
	}

	{
		ClientPacket * pack = ClientProtocol::makeMsgConfirmHeroReq(input_packet->session_id, 704);
		pack->session_id = input_packet->session_id;

		m_message_pump->handleOutputStream(input_packet->session_id, pack->getStream(), pack->getStreamSize());

		pack->cleanStream();
		delete pack;
	}

	return 0;
}

int ManageBattleRobot::handleMsgReEnterBattleAck(ServerPacket * input_packet, const MsgReEnterBattleAck * input_msg)
{
	DEF_LOG_INFO("recv re enter battle msg");
	return 0;
}

int ManageBattleRobot::handleMsgSelectHeroAck(ServerPacket * input_packet, const MsgSelectHeroAck * input_msg)
{
	DEF_LOG_INFO("recv select hero msg");
	return 0;
}

int ManageBattleRobot::handleMsgConfirmHeroAck(ServerPacket * input_packet, const MsgConfirmHeroAck * input_msg)
{
	DEF_LOG_INFO("recv confirm hero msg");
	return 0;
}

int ManageBattleRobot::handleMsgLobbyGameStartAck(ServerPacket * input_packet, const MsgLobbyGameStartAck * input_msg)
{
	DEF_LOG_INFO("recv lobby game start ack");

	ClientPacket * pack = ClientProtocol::makeMsgCombatPlayerReadyReq(0);
	m_message_pump->handleOutputStream(input_packet->session_id, pack->getStream(), pack->getStreamSize());
	pack->cleanStream();
	delete pack;

	return 0;
}

int ManageBattleRobot::handleMsgCombatPlayerReadyAck(ServerPacket * input_packet, const MsgCombatPlayerReadyAck * input_msg)
{
	DEF_LOG_INFO("recv combat player ready ack");
	return 0;
}

int ManageBattleRobot::clientInput(int msg_id, ServerPacket* packet, char * buffer, int buffer_size)
{
	std::unique_ptr<InterPacket> dest(new InterPacket);
	dest->session_id = packet->session_id;
	dest->setMsgInfo(msg_id, 0, buffer_size, buffer);

	return Single<PluginDepot>::GetInstance()->input(msg_id, dest.get(), buffer, buffer_size);
}

int ManageBattleRobot::handleMsgFrameSyncDetail(ServerPacket * input_packet, const MsgFrameSyncDetail * input_msg)
{
	//ClientPacket * pack = ClientProtocol::makeMsgReportBattleResultReq();
	//m_message_pump->handleOutputStream(input_packet->session_id, pack->getStream(), pack->getStreamSize());
	//pack->cleanStream();

	//delete pack;

	return 0;
}

Robot * ManageBattleRobot::getAvilableRobot()
{
	if (m_request_connection_robot_list.size() == 0)
	{
		return NULL;
	}

	Robot * ret = m_request_connection_robot_list.front();
	m_request_connection_robot_list.pop_front();
	return ret;
}

void ManageBattleRobot::updateRobot()
{
	for (auto robot : m_enable_robot_list)
	{
		robot->update();
	}

	if (m_avilable_robot_list.size() > 0)
	{
		for (auto robot : m_avilable_robot_list)
		{
			m_request_connection_robot_list.push_back(robot);

			m_message_pump->connect(robot->getBattleIp(), robot->getBattlePort());

		}

		m_avilable_robot_list.clear();
	}
}

void ManageBattleRobot::addRobot(Robot * robot)
{
	m_robot_session_map[robot->getSession()] = robot;
}

Robot * ManageBattleRobot::getRobot(SessionId session_id)
{
	auto it = m_robot_session_map.find(session_id);
	if (it != m_robot_session_map.end())
	{
		return it->second;
	}
	
	return NULL;
}

