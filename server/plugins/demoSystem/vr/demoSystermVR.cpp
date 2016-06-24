#include "demoSystemVR.h"
#include <flatbuffers/flatbuffers.h>

INPUT_FUN_DEFINE(DemoSystemVR, Opcode_E_DemoSystemTestReq, DemoSystemTestReq)
{	
	auto it = input_msg->you();
	flatbuffers::FlatBufferBuilder builder;
	auto out = CreateDemoSystemTestAck(builder);
	builder.Finish(out);

	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(Opcode_E_DemoSystemTestAck, 0, builder.GetSize(), (char *)builder.GetBufferPointer());
	pack->session_id = input_packet->session_id;

	m_plugin_cfg.sender->sendMsg2Client(pack);
	return 0;
}

REQUEST_FUN_DEFINE(DemoSystemVR, 2, xx2demo_req_item_buy)
{
	return 0;
}

NOTIFY_FUN_DEFINE(DemoSystemVR, 3, demo2xx_ntf_item_reduce)
{	
	demo2xx_ntf_item_reduce *r = dynamic_cast<demo2xx_ntf_item_reduce *>(ntf.msg);
	r->id();
	r->num();
	return 0;
}

