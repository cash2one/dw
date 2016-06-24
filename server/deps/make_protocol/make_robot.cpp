#include "make_robot.h"

InterPacket * makeRequestRobotConfigAck(int msg_id, uint64_t guid, const string & ip_addr, int port, int start_id, int run_robot_num, uint8_t net_type)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> ip_addr_off = builder.CreateString(ip_addr);
	flatbuffers::Offset<RequestRobotConfigAck> off_set = CreateRequestRobotConfigAck(builder ,ip_addr_off, port, start_id, run_robot_num, net_type);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeDemoSystemTestReq(int msg_id, uint64_t guid, int you, const string & her)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::String> her_off = builder.CreateString(her);
	flatbuffers::Offset<DemoSystemTestReq> off_set = CreateDemoSystemTestReq(builder ,you, her_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

InterPacket * makeDemoSystemTestAck(int msg_id, uint64_t guid, uint8_t me, const std::vector<unknown> & one)
{
	flatbuffers::FlatBufferBuilder builder;
	flatbuffers::Offset<flatbuffers::Vector<const string &>> one_off = builder.CreateVector(one);
	flatbuffers::Offset<DemoSystemTestAck> off_set = CreateDemoSystemTestAck(builder ,me, one_off);
	builder.Finish(off_set);
	InterPacket * pack = new InterPacket();
	pack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());
	return pack;
};

