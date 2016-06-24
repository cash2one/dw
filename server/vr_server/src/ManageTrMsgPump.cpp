
#include "ManageTrMsgPump.h"
#include "ManageClientMsgPump.h"
#include "Single.hpp"
#include "plugin.h"
#include "pluginDepot.h"

ManageTrMsgPump::ManageTrMsgPump()
	: m_message_pump(NULL)
	, m_session_id(NULL)
{

}

ManageTrMsgPump::~ManageTrMsgPump()
{

}

ManageTrMsgPump * ManageTrMsgPump::instance()
{
	return Singleton<ManageTrMsgPump>::instance();
}

void ManageTrMsgPump::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	m_session_id = session_id;
}

void ManageTrMsgPump::handleCloseConnection(SessionId session_id)
{
	if (session_id == m_session_id)
	{
		m_session_id = NULL;
	}
}

int ManageTrMsgPump::init(const TrCfg & cfg_value)
{
	m_message_pump = MessageServerPump<InterPacket>::createMessageServerPump();
	m_message_pump->init(this, NPT_TCP);
	m_message_pump->connect(cfg_value.ip_addr, cfg_value.port, true);

	m_message_pump->setDefaultMsgHandler(std::bind(&PluginDepot::input, Single<PluginDepot>::GetInstance(),
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	//// 这里添加插件
	//Single<PluginDepot>::GetInstance()->addPlugin(Single<DemoSystemVR>::Instance());
	return 0;
}

int ManageTrMsgPump::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_message_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	return (int)dispatch_pack_num;
}

bool ManageTrMsgPump::handleOutputPacket(InterPacket * pack)
{
	bool ret = false;
	if (NULL != m_session_id)
	{
		m_message_pump->handleOutputStream(m_session_id, pack->getStream(), pack->getStreamSize());
		pack->cleanStream();
		ret = true;
	}
	else
	{
		DEF_LOG_ERROR("session is NULL while send packet <%hd>", pack->msg_head.msg_id);
	}

	delete pack;

	return ret;
}