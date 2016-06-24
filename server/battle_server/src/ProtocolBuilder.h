
#ifndef PROTOCOL_BUILDER_HPP__
#define PROTOCOL_BUILDER_HPP__

#include "common_generated.h"
#include "framecmd_c2c_generated.h"
#include "framecmd_c2s_generated.h"
#include "framesync_generated.h"
#include "lobby_generated.h"
#include "protocol_generated.h"
#include "PackMsg.h"
#include "Packet_.h"

class ProtocolBuilder
{
public:
	static InterPacket * createMsgPing(uint64_t client_start_time, uint64_t gate_recv_time, uint64_t server_recv_time);
};

#endif
