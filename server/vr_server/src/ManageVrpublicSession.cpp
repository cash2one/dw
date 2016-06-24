
#include "ManageVrpublicSession.h"
#include "opcode.h"
#include "ManageVrpublicMessage.h"

ManageVrpublicSession * ManageVrpublicSession::m_instance = NULL;

ManageVrpublicSession::ManageVrpublicSession()
	: m_session_server(NULL)
{

}

ManageVrpublicSession::~ManageVrpublicSession()
{

}

ManageVrpublicSession * ManageVrpublicSession::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageVrpublicSession();
	}

	return m_instance;
}

int ManageVrpublicSession::init(const VrpublicCfg & vrpublic_cfg)
{
	m_vrpublic_cfg = vrpublic_cfg;

	m_session_server = SessionServer::createSessionServer(NPT_TCP);

	m_session_server->init(this);

	m_session_server->connect(vrpublic_cfg.ip_addr, vrpublic_cfg.port, true);

	return 0;
}

void ManageVrpublicSession::handleOutputPacket(InterPacket * packet)
{
	m_session_server->handleOutputStream(m_session_id, packet->getStream(), packet->getStreamSize());

	packet->cleanStream();

	delete packet;
}

void ManageVrpublicSession::getNewSession(SessionId session_id)
{
	m_session_id = session_id;
}

void ManageVrpublicSession::getCloseSession(SessionId session_id)
{
	if (session_id == m_session_id)
	{
		m_session_id = NULL;
	}
}

void ManageVrpublicSession::handleNewConnection(SessionId session_id, const string & remote_ip_addr, int local_port, SessionId raknet_session)
{
	//m_session_id = session_id;

	InterPacket * inter_pack = new InterPacket();
	inter_pack->setMsgInfo(MSG_NEW_CLIENT_SESSION, 0, 0, (char *)NULL);
	inter_pack->session_id = session_id;

	ManageVrpublicMessage::instance()->handleInputPacket(inter_pack);
}

void ManageVrpublicSession::handleNewConnectionFailed()
{
}

void ManageVrpublicSession::handleCloseConnection(SessionId session_id, int error_code)
{
	InterPacket * inter_pack = new InterPacket();
	inter_pack->setMsgInfo(MSG_CLOSE_CLIENT_SESSION, 0, 0, (char *)NULL);
	inter_pack->session_id = session_id;

	ManageVrpublicMessage::instance()->handleInputPacket(inter_pack);
}
	
size_t ManageVrpublicSession::handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
{
	InterPacketVec_t pack_vec;
	size_t pos = parseInterPacketFromStream(session_id, buffer, buf_len, pack_vec);

	for (auto pack : pack_vec)
	{
		ManageVrpublicMessage::instance()->handleInputPacket(pack);
	}

	return pos;
}
