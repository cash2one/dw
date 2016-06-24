
#include "MessageMaker.h"

//
//ClientPacket * MessageMaker::makeMsgLoginReq(SessionId session_id, const string & user_name)
//{
//	FlatBufferBuilder builder;
//	Offset<String> off_name = builder.CreateString(user_name);
//	Offset<MsgLoginReq> off_req = CreateMsgLoginReq(builder, 1, 0, 1, 1, off_name, 0, 0, 1, 0);
//	builder.Finish(off_req);
//
//	ClientPacket * pack = new ClientPacket();
//	pack->setMsgInfo(Opcode_E_MsgLoginReq, builder.GetSize(), (char *)builder.GetBufferPointer());
//	pack->session_id = session_id;
//
//	return pack;
//}
//
//ClientPacket * MessageMaker::makeMsgChooseServerReq(SessionId session_id, int server_id)
//{
//	FlatBufferBuilder builder;
//	Offset<MsgChooseServerReq> off_req = CreateMsgChooseServerReq(builder, server_id);
//	builder.Finish(off_req);
//
//	ClientPacket * pack = new ClientPacket();
//	pack->setMsgInfo(Opcode_E_MsgChooseServerReq, builder.GetSize(), (char *)builder.GetBufferPointer());
//	pack->session_id = session_id;
//
//	return pack;
//}
//
//ClientPacket * MessageMaker::makeMsgVRLoginReq(SessionId session_id, const string & user_name, const string & session_key)
//{
//	FlatBufferBuilder builder;
//	Offset<String> off_name = builder.CreateString(user_name);
//	Offset<String> off_key = builder.CreateString(session_key);
//	Offset<MsgVRLoginReq> off_req = CreateMsgVRLoginReq(builder, off_name, off_key);
//	builder.Finish(off_req);
//
//	ClientPacket * pack = new ClientPacket();
//	pack->setMsgInfo(Opcode_E_MsgVRLoginReq, builder.GetSize(), (char *)builder.GetBufferPointer());
//
//	return pack;
//}

