
#ifndef MANAGE_SNIFFER_HPP__
#define MANAGE_SNIFFER_HPP__

#include <stdint.h>
#include <string>
#include <pcap.h>
#include "MsgBlock.h"

#pragma pack(1)

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

#pragma pack(8)

using namespace std;

class ManageSniffer
{
public:
	ManageSniffer();
	~ManageSniffer();
public:
	bool init(const string & dev_str, const string & filter_str);

	bool update();
protected:
	void parseStream(const char * buffer, int len);
private:
	pcap_t * m_handle;

	MsgBlock m_msg_block;
};

#endif
