
#include "ManageVrpublicMsgPump.h"
#include "ManageClientMsgPump.h"
#include "Single.hpp"
#include "plugin.h"
#include "pluginDepot.h"
#include "opcode.h"

ManageVrpublicMsgPump::ManageVrpublicMsgPump()
	: m_message_pump(NULL)
	, m_session_id(NULL)
{

}

ManageVrpublicMsgPump::~ManageVrpublicMsgPump()
{

}

ManageVrpublicMsgPump * ManageVrpublicMsgPump::instance()
{
	return Singleton<ManageVrpublicMsgPump>::instance();
}

void ManageVrpublicMsgPump::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	m_session_id = session_id;

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_VrModuleVrpConnected, 0);
	pack->session_id = session_id;

	Single<PluginDepot>::GetInstance()->input(pack->msg_head.msg_id, pack, NULL, 0);

	delete pack;
}

void ManageVrpublicMsgPump::handleCloseConnection(SessionId session_id)
{
	if (session_id == m_session_id)
	{
		m_session_id = NULL;
	}
}

int ManageVrpublicMsgPump::init(const VrpublicCfg & vrp_cfg)
{
	m_message_pump = MessageServerPump<InterPacket>::createMessageServerPump();
	m_message_pump->init(this, NPT_TCP);
	m_message_pump->connect(vrp_cfg.ip_addr, vrp_cfg.port, true);

	m_message_pump->setDefaultMsgHandler(std::bind(&PluginDepot::input, Single<PluginDepot>::GetInstance(),
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	this->notifyAreaIdToPlugin();

	return 0;
}

int ManageVrpublicMsgPump::update()
{
	size_t dispatch_pack_num = 0;
	size_t dispatch_pack_bytes = 0;
	m_message_pump->update(dispatch_pack_num, dispatch_pack_bytes);

	return (int)dispatch_pack_num;
}

bool ManageVrpublicMsgPump::handleOutputPacket(InterPacket * pack)
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

void ManageVrpublicMsgPump::notifyAreaIdToPlugin()
{
	VrCfg vr_cfg = VrConfig::instance()->getVrCfg();

	uint32_t area_id = vr_cfg.area_id;
	uint32_t server_id = vr_cfg.server_id;
	uint64_t guid;
	memcpy(&guid, &area_id, sizeof(area_id));
	memcpy((char *)&guid + sizeof(area_id), &server_id, sizeof(server_id));
	PackInfo nty_pack(N_VrModuleNotifyAreaId, guid, NULL);
	Single<PluginDepot>::GetInstance()->notify(nty_pack);
}

