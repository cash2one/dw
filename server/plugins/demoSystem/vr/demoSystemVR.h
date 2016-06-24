#pragma  once
#include <unordered_map>
#include <iostream>
#include "typedef.h"
#include <flatbuffers/flatbuffers.h>
#include "inter_generated.h"
#include "plugin.h"
#include "msg_demo.pb.h"
#include "robot_generated.h"
#include "protocol_generated.h"
#include "sender.h"

using namespace MobaGo::FlatBuffer;
using namespace MobaGo::Protocol;

class DemoSystemVR
	:public Plugin
{	
public:
	DemoSystemVR() {}
	~DemoSystemVR(){}


	BEGIN_INPUT_MSG_MAP()
		INPUT_HANDLE_MSG(DemoSystemVR, Opcode_E_DemoSystemTestReq, DemoSystemTestReq)
	END_INPUT_MSG_MAP()


	BEGIN_REQUEST_MSG_MAP()
		REQUEST_HANDLE_MSG(DemoSystemVR, 2, xx2demo_req_item_buy)
	END_REQUEST_MSG_MAP()
	

	BEGING_NOTIFY_MSG_MAP()
		NOTIFY_HANDLE_MSG(DemoSystemVR, 3, demo2xx_ntf_item_reduce)
	END_NOTIFY_MSG_MAP()


	INPUT_FUN_DECLARE(DemoSystemVR, Opcode_E_DemoSystemTestReq, DemoSystemTestReq);

	REQUEST_FUN_DECLARE(DemoSystemVR, 2, xx2demo_req_item_buy);

	NOTIFY_FUN_DECLARE(DemoSystemVR, 3, demo2xx_ntf_item_reduce);
};