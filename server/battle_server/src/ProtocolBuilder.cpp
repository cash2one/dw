
#include "ProtocolBuilder.h"

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;

InterPacket * ProtocolBuilder::createMsgPing(uint64_t client_start_time, uint64_t gate_recv_time, uint64_t server_recv_time)
{
	return NULL;

	//MsgPing msg_ping(client_start_time, gate_recv_time, server_recv_time);

	//ServerPacket * pack = new ServerPacket();
	//pack->msg_head.msg_id = Protocol_Base_E_PROTOCOL_PING;
	//pack->msg_head.msg_len = sizeof(msg_ping);
	//pack->setMsgInfo(pack->msg_head, (char *)&msg_ping);

	//return pack;
}
