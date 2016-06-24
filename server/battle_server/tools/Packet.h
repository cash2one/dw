
#ifndef PACKET_HPP__
#define PACKET_HPP__

#include <stdlib.h>
#include "typedef.h"
#pragma pack(push)
#pragma pack(1)

struct MsgHead
{
	MsgHead()
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

#pragma pack(8)

struct Packet
{
public:
	Packet()
		: msg_buffer(NULL)
	{}

	char * getBody()
	{
		if (NULL == msg_buffer)
		{
			// error
			return NULL;
		}

		return msg_buffer + pack_head_size;
	}

	PackHead msg_head;
	char * msg_buffer;

	static int pack_head_size;
};
#pragma pack(pop)
#endif
