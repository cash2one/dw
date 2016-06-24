
#ifndef PACKET__HPP__
#define PACKET__HPP__

#include <vector>
#include <stdlib.h>
#include "typedef.h"
//#include "StatCount.h"
#include "SessionHandle.h"

using namespace std;

typedef uint64_t MyGuid_t;

struct ClientPacket;

typedef vector<ClientPacket *> ClientPacketVec_t;

struct ServerPacket;

typedef vector<ServerPacket *> ServerPacketVec_t;

struct InterPacket;

typedef vector<InterPacket *> InterPacketVec_t;

typedef void * session_t;

#pragma pack(push, 1)

struct ClientHead
{
	ClientHead()
		: msg_id(0)
		, msg_len(0)
	{}
	uint16_t msg_id;
	uint16_t msg_len;
};

struct PackHead 
{
	PackHead()
		: msg_id(0)
		, msg_len(0)
		, server_time(0)
		, version(0)
		, pack_seq(0)
	{}
	uint16_t msg_id;
	uint16_t msg_len;
	uint64_t server_time;
	uint32_t version;
	uint32_t pack_seq;
};

struct InterHead 
{
	InterHead()
		: msg_id(0)
		, msg_len(0)
		, server_time(0)
		, version(0)
		, pack_seq(0)
		, guid(0)
	{}

	uint16_t msg_id;
	uint16_t msg_len;
	uint64_t server_time;
	uint32_t version;
	uint32_t pack_seq;
	uint64_t guid;
};

#pragma pack(pop)

InterPacket * makeInterPacket(MyGuid_t guid, ClientPacket * client_pack);

ClientPacket * makeClientPacket(InterPacket * inter_pack);

ServerPacket * makeServerPacket(InterPacket * inter_pack);

size_t parsePacketFromStream(SessionId session_id, char * buffer, size_t buf_len, ClientPacketVec_t & pack_vec);

size_t parsePacketFromStream(SessionId session_id, char * buffer, size_t buf_len, InterPacketVec_t & pack_vec);

size_t parsePacketFromStream(SessionId session_id, char * buffer, size_t buf_len, ServerPacketVec_t & pack_vec);

size_t parseClientPacketFromStream(SessionId session, char * buffer, size_t buf_len, ClientPacketVec_t & pack_vec);

size_t parseInterPacketFromStream(SessionId session, char * buffer, size_t buf_len, InterPacketVec_t & pack_vec);



int convertVersionToInt(const char * str_version);

void makeEmptyMsgPacket(ClientPacket ** packet, int msg_id);

void makeEmptyMsgPacket(InterPacket ** packet, int msg_id);

void makeEmptyMsgPacket(ServerPacket ** packet, int msg_id);

//size_t parseServerPacketFromStream(char * buffer, size_t buf_len, ClientPacketVec_t & pack_vec);

struct ServerPacket
{
public:
	ServerPacket()
		: msg_buffer(NULL)
		, session_id(NULL)
	{
		//m_stat_count.addNewCount();
	}

	~ServerPacket()
	{
		//m_stat_count.addRemoveCount();
		if (NULL != msg_buffer)
		{
			delete[] msg_buffer;
		}
	}

	template<class _T>
	static int appendNumber(char * buffer, _T value)
	{
		memcpy(buffer, &value, sizeof(_T));
		return sizeof(_T);
	}

	static int appendUint8(char * buffer, uint8 value)
	{
		return ServerPacket::appendNumber<uint8>(buffer, value);
	}

	static int appendUint16(char * buffer, uint16 value)
	{
		return ServerPacket::appendNumber<uint16>(buffer, value);
	}

	static int appendUint32(char * buffer, uint32 value)
	{
		return ServerPacket::appendNumber<uint32>(buffer, value);
	}

	static int appendUint64(char * buffer, uint64 value)
	{
		return ServerPacket::appendNumber<uint64>(buffer, value);
	}


	char * getBody()
	{
		if (NULL == msg_buffer)
		{
			// error
			return NULL;
		}

		return msg_buffer + pack_head_size;
	}

	void setMsgInfo(const PackHead & pack_head, const char * buffer_body)
	{
		msg_head = pack_head;
		msg_buffer = new char[pack_head_size + pack_head.msg_len];
		memcpy(msg_buffer, &msg_head, pack_head_size);
		memcpy(msg_buffer + pack_head_size, buffer_body, msg_head.msg_len);
	}

	char * getStream()
	{
		return msg_buffer;
	}

	int getStreamSize()
	{
		return pack_head_size + msg_head.msg_len;
	}

	void * getSession()
	{
		return session_id;
	}

	ServerPacket * clone()
	{
		ServerPacket * pk = new ServerPacket();
		pk->setMsgInfo(this->msg_head, this->getBody());
		return pk;
	}

	void cleanStream()
	{
		memset(&msg_head, 0, sizeof(msg_head));
		msg_buffer = NULL;
	}

	PackHead msg_head;
	char * msg_buffer;
	session_t session_id;

	static int pack_head_size;
	//static StatCount m_stat_count;
};

struct ClientPacket
{
public:
	ClientPacket()
		: msg_buffer(NULL)
		, session_id(0)
	{}

	~ClientPacket()
	{
		if (NULL != msg_buffer)
		{
			delete[] msg_buffer;
		}
	}

	char * getBody()
	{
		if (NULL == msg_buffer)
		{
			// error
			return NULL;
		}

		return msg_buffer + pack_head_size;
	}

	void setMsgInfo(const ClientHead & pack_head, const char * buffer_body)
	{
		msg_head = pack_head;
		msg_buffer = new char[pack_head_size + pack_head.msg_len];
		memcpy(msg_buffer, &msg_head, pack_head_size);
		memcpy(msg_buffer + pack_head_size, buffer_body, msg_head.msg_len);
	}

	void setMsgInfo(int msg_id, int body_len, const char * buffer_body)
	{
		msg_head.msg_id = (uint16_t)msg_id;
		msg_head.msg_len = (uint16_t)body_len;
		msg_buffer = new char[pack_head_size + body_len];
		memcpy(msg_buffer, &msg_head, pack_head_size);
		if (body_len > 0)
		{
			memcpy(msg_buffer + pack_head_size, buffer_body, msg_head.msg_len);
		}
	}

	char * getStream()
	{
		return msg_buffer;
	}

	int getStreamSize()
	{
		return pack_head_size + msg_head.msg_len;
	}

	void * getSession()
	{
		return session_id;
	}

	void cleanStream()
	{
		memset(&msg_head, 0, sizeof(msg_head));	
		msg_buffer = NULL;
		session_id = 0;
	}

	ClientHead msg_head;
	char * msg_buffer;
	session_t session_id;

	static int pack_head_size;
};

struct InterPacket
{
public:
	InterPacket()
		: msg_buffer(NULL)
		, session_id(NULL)
	{
	}

	InterPacket(int msg_id, MyGuid_t guid)
		: msg_buffer(NULL)
		, session_id(NULL)
	{
		this->setMsgInfo(msg_id, guid);
	}

	InterPacket(int msg_id, MyGuid_t guid, int body_len, const char * buffer_body)
		: msg_buffer(NULL)
		, session_id(NULL)
	{
		this->setMsgInfo(msg_id, guid, body_len, buffer_body);
	}

	InterPacket(int msg_id, MyGuid_t guid, int body_len, const uint8_t * buffer_body)
		: msg_buffer(NULL)
		, session_id(NULL)
	{
		this->setMsgInfo(msg_id, guid, body_len, buffer_body);
	}

	~InterPacket()
	{
		if (NULL != msg_buffer)
		{
			delete[] msg_buffer;
		}
	}

	char * getBody()
	{
		if (NULL == msg_buffer)
		{
			// error
			return NULL;
		}

		return msg_buffer + pack_head_size;
	}

	void setMsgInfo(const InterHead & pack_head, const char * buffer_body)
	{
		msg_head = pack_head;
		msg_buffer = new char[pack_head_size + pack_head.msg_len];
		memcpy(msg_buffer, &msg_head, pack_head_size);
		if (msg_head.msg_len > 0)
		{
			memcpy(msg_buffer + pack_head_size, buffer_body, msg_head.msg_len);
		}
	}

	void setMsgInfo(int msg_id, MyGuid_t guid)
	{
		this->setMsgInfo(msg_id, guid, 0, (char *)NULL);
	}

	void setMsgInfo(int msg_id, MyGuid_t guid, int body_len, const char * buffer_body)
	{
		msg_head.msg_id = (uint16_t)msg_id;
		msg_head.msg_len = (uint16_t)body_len;
		msg_head.guid = guid;
		msg_buffer = new char[pack_head_size + body_len];
		memcpy(msg_buffer, &msg_head, pack_head_size);
		if (body_len > 0)
		{
			memcpy(msg_buffer + pack_head_size, buffer_body, msg_head.msg_len);
		}
	}

	void setMsgInfo(int msg_id, MyGuid_t guid, int body_len, const uint8_t * buffer_body)
	{
		setMsgInfo(msg_id, guid, body_len, (const char *) buffer_body);
	}

	InterPacket * clone()
	{
		InterPacket * pk = new InterPacket();
		pk->setMsgInfo(this->msg_head, this->getBody());

		return pk;
	}

	char * getStream()
	{
		return msg_buffer;
	}

	int getStreamSize()
	{
		return pack_head_size + msg_head.msg_len;
	}

	void cleanStream()
	{
		memset(&msg_head, 0, sizeof(msg_head));
		msg_buffer = NULL;
	}

	void updateGuid(uint64_t guid)
	{
		msg_head.guid = guid;
		memcpy(msg_buffer, &msg_head, pack_head_size);
	}

	void updateHead(uint64_t guid, int msg_id)
	{
		msg_head.guid = guid;
		msg_head.msg_id = msg_id;
		memcpy(msg_buffer, &msg_head, pack_head_size);
	}

	InterHead msg_head;
	char * msg_buffer;
	SessionId session_id;
	static int pack_head_size;
};

#endif
