#include "MessagePacker.h"


MessagePacker * MessagePacker::m_instance = NULL;

MessagePacker::MessagePacker()
{}

MessagePacker::~MessagePacker()
{
	if (m_instance != NULL)
	{
		delete m_instance;
	}
	m_instance = NULL;
}

MessagePacker * MessagePacker::instance()
{
	if (m_instance == NULL)
	{
		m_instance = new MessagePacker();
	}

	return m_instance;
}

void MessagePacker::sendErrorPacket(session_t session_id, uint64_t guid, MobaGo::FlatBuffer::Error error_code)
{
	InterPacket * pack = new InterPacket();

	flatbuffers::FlatBufferBuilder build;
	flatbuffers::Offset<MobaGo::FlatBuffer::MsgError> req_off = MobaGo::FlatBuffer::CreateMsgError(build, error_code);
	build.Finish(req_off);

	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgError, guid, build.GetSize(), (char *)build.GetBufferPointer());
	if (pack)
	{
		ManagerClientSession::instance()->handleOutputStream(session_id, (char *)pack->getStream(), pack->getStreamSize());
		pack->cleanStream();
		delete pack;
	}
}

void MessagePacker::sendAreaList(session_t session_id, uint64_t guid, const ServerInfoMap_t & all_server_info_map, const AccountServerInfoList_t & account_server_info_list)
{
	// begin
	flatbuffers::FlatBufferBuilder builder;
	InterPacket *pack = new InterPacket();
	std::vector<flatbuffers::Offset<MobaGo::FlatBuffer::ServerInfo_t>> vec_off_server;

	ServerInfoMap_t::const_iterator itAll = all_server_info_map.begin();
	for (; itAll != all_server_info_map.end(); ++itAll)
	{
		const ServerInfo *stcServerInfo = itAll->second;
		auto it = find_if(account_server_info_list.begin(), account_server_info_list.end(), 
			[&](const AccountServerInfo *account_server)
			{
				return account_server->area_id == itAll->second->area_id;
			}
		);

		if (stcServerInfo->is_match_area && it == account_server_info_list.end())
		{
			continue;
		}

		flatbuffers::Offset<flatbuffers::String> server_name = builder.CreateString(itAll->second->area_name);
		flatbuffers::Offset<MobaGo::FlatBuffer::ServerInfo_t> off_server_info = MobaGo::FlatBuffer::CreateServerInfo_t(builder,
			itAll->second->area_id, server_name, 1, itAll->second->area_index, itAll->second->status);
		vec_off_server.push_back(off_server_info);
	}

	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ServerInfo_t>>> fb_vec_server = builder.CreateVector(vec_off_server);
	flatbuffers::Offset<MsgServerListAck> off_ack = MobaGo::FlatBuffer::CreateMsgServerListAck(builder, fb_vec_server);
	builder.Finish(off_ack);

	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgServerListAck, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	if (pack)
	{
		ManagerClientSession::instance()->handleOutputStream(session_id, (char *)pack->getStream(), pack->getStreamSize());
		pack->cleanStream();
		delete pack;
	}
	
}

void MessagePacker::sendChooseAreaAck(session_t session_id, uint64_t guid, MyGuid_t account_id, int gs_id, string gate_ip, int gate_port, string session_code, string annoucement)
{
	InterPacket * pack = new InterPacket();

	flatbuffers::FlatBufferBuilder build;
	flatbuffers::Offset<flatbuffers::String> off_gate_ip = build.CreateString(gate_ip);
	flatbuffers::Offset<flatbuffers::String> off_session_code = build.CreateString(session_code);
	flatbuffers::Offset<flatbuffers::String> off_announcement = build.CreateString(annoucement);
	flatbuffers::Offset<MsgChooseServerAck> req_off = MobaGo::FlatBuffer::CreateMsgChooseServerAck(build, account_id, off_gate_ip, gate_port, off_session_code, off_announcement);
	build.Finish(req_off);

	pack->setMsgInfo(MobaGo::Protocol::Opcode_E_MsgChooseServerAck, guid, build.GetSize(), (char *)build.GetBufferPointer());
	if (pack)
	{
		ManagerClientSession::instance()->handleOutputStream(session_id, (char *)pack->getStream(), pack->getStreamSize());
		pack->cleanStream();
		delete pack;
	}
}

void MessagePacker::SendSMsgVrpublicNotifyVrLoginInfo(SessionId session_id, uint64_t guid, Account * account, ServerVrInfo * pVrServer)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> account_name = builder.CreateString(account->user_name);
	flatbuffers::Offset<flatbuffers::String> client_version = builder.CreateString(account->client_version);
	flatbuffers::Offset<flatbuffers::String> session_code = builder.CreateString(account->session_code);
	flatbuffers::Offset<flatbuffers::String> device_name = builder.CreateString(account->device_name);
	flatbuffers::Offset<flatbuffers::String> password = builder.CreateString(account->password);

	flatbuffers::Offset<flatbuffers::String> gate_ip = builder.CreateString(pVrServer->gate_ip_addr);
	flatbuffers::Offset<flatbuffers::String> announcement = builder.CreateString("");

	flatbuffers::Offset<SMsgVrpublicNotifyVrLoginInfo> off_notify = CreateSMsgVrpublicNotifyVrLoginInfo(builder, account_name, account->summoner_id, session_code, account->channel_type, client_version,
		account->is_play, account->language_type, account_name, device_name, password, account->platform_id, pVrServer->gs_id, gate_ip, pVrServer->gate_port, announcement);
	builder.Finish(off_notify);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(E_SMsgVrpublicNotifyVrLoginInfo, guid, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = session_id;

	ManageVrSession::instance()->handleOutputPacket(pack);
	delete pack;
}
