
#include "Packet_.h"

//StatCount ServerPacket::m_stat_count;

int ClientPacket::pack_head_size = sizeof(ClientHead);

int ServerPacket::pack_head_size = sizeof(PackHead);

int InterPacket::pack_head_size = sizeof(InterHead);


InterPacket * makeInterPacket(MyGuid_t guid, ClientPacket * client_pack)
{
	InterPacket * pack = new InterPacket();
	InterHead head;
	memcpy(&head, &client_pack->msg_head, sizeof(client_pack->msg_head));
	head.guid = guid;
	pack->setMsgInfo(head, client_pack->getBody());

	return pack;
}

ClientPacket * makeClientPacket(InterPacket * inter_pack)
{
	ClientPacket * pack = new ClientPacket();
	memcpy(&pack->msg_head, &inter_pack->msg_head, sizeof(pack->msg_head));
	pack->setMsgInfo(pack->msg_head, inter_pack->getBody());
	return pack;
}

ServerPacket * makeServerPacket(InterPacket * inter_pack)
{
	ServerPacket * pack = new ServerPacket();
	memcpy(&pack->msg_head, &inter_pack->msg_head, ServerPacket::pack_head_size);
	pack->setMsgInfo(pack->msg_head, inter_pack->getBody());
	return pack;
}

size_t parsePacketFromStream(SessionId session_id, char * buffer, size_t buf_len, ClientPacketVec_t & pack_vec)
{
	return parseClientPacketFromStream(session_id, buffer, buf_len, pack_vec);
}

size_t parsePacketFromStream(SessionId session_id, char * buffer, size_t buf_len, InterPacketVec_t & pack_vec)
{
	return parseInterPacketFromStream(session_id, buffer, buf_len, pack_vec);
}

size_t parsePacketFromStream(SessionId session_id, char * buffer, size_t buf_len, ServerPacketVec_t & pack_vec)
{
	PackHead msg_head;
	ServerPacket * packet;
	size_t pos = 0;
	while (ServerPacket::pack_head_size <= (int)(buf_len - pos))
	{
		memcpy(&msg_head, buffer + pos, ServerPacket::pack_head_size);
		if (msg_head.msg_len + ServerPacket::pack_head_size <= (int)(buf_len - pos))
		{
			packet = new ServerPacket();
			// make packet
			packet->setMsgInfo(msg_head, buffer + pos + ServerPacket::pack_head_size);
			pos += ServerPacket::pack_head_size + msg_head.msg_len;
			packet->session_id = session_id;

			pack_vec.push_back(packet);
		}
		else
		{
			// 
			break;
		}
	}

	return pos;
}

size_t parseClientPacketFromStream(SessionId session, char * buffer, size_t buf_len, ClientPacketVec_t & pack_vec)
{
	ClientHead msg_head;
	ClientPacket * packet;
	size_t pos = 0;
	while (ClientPacket::pack_head_size <= (int)(buf_len - pos))
	{
		memcpy(&msg_head, buffer + pos, ClientPacket::pack_head_size);
		if (msg_head.msg_len + ClientPacket::pack_head_size <= (int)(buf_len - pos))
		{
			packet = new ClientPacket();
			// make packet
			packet->setMsgInfo(msg_head, buffer + pos + ClientPacket::pack_head_size);
			pos += ClientPacket::pack_head_size + msg_head.msg_len;
			packet->session_id = session;
			
			pack_vec.push_back(packet);
		}
		else
		{
			// 
			break;
		}
	}

	return pos;
}

size_t parseInterPacketFromStream(SessionId session, char * buffer, size_t buf_len, InterPacketVec_t & pack_vec)
{
	InterHead msg_head;
	InterPacket * packet;
	size_t pos = 0;
	while (InterPacket::pack_head_size <= (int)(buf_len - pos))
	{
		memcpy(&msg_head, buffer + pos, InterPacket::pack_head_size);
		if (msg_head.msg_len + InterPacket::pack_head_size <= (int)(buf_len - pos))
		{
			packet = new InterPacket();
			// make packet
			packet->setMsgInfo(msg_head, buffer + pos + InterPacket::pack_head_size);
			packet->session_id = session;
			pos += InterPacket::pack_head_size + msg_head.msg_len;

			pack_vec.push_back(packet);
		}
		else
		{
			// 
			break;
		}
	}

	return pos;
}

// todo
int convertVersionToInt(const char * str_version)
{
	return 0;
}

void makeEmptyMsgPacket(ClientPacket ** packet, int msg_id)
{
	*packet = new ClientPacket();
	(*packet)->setMsgInfo(msg_id, 0, (char *)NULL);
}

void makeEmptyMsgPacket(InterPacket ** packet, int msg_id)
{
	*packet = new InterPacket();
	(*packet)->setMsgInfo(msg_id, 0, 0, (const char *)NULL);
}

void makeEmptyMsgPacket(ServerPacket ** packet, int msg_id)
{
	*packet = new ServerPacket();
	PackHead head;
	head.msg_id = msg_id;
	head.msg_len = 0;
	(*packet)->setMsgInfo(head, (char *)NULL);
}
