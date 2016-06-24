#include "sender.h"
#include "Logger.h"
#include "Packet_.h"

void Sender::sendMsg2Client(InterPacket* p)
{
	if (!client_cb)
	{
		DEF_LOG_ERROR("NOT register sender callback");
		return;
	}

	client_cb(p);
}

void Sender::sendMsg2VR(InterPacket* p)
{
	if (!vr_cb)
	{
		DEF_LOG_ERROR("NOT register sender callback");
		return;
	}

	vr_cb(p);
}

void Sender::sendMsg2VRPublic(InterPacket* p)
{
	if (!vrpublic_cb)
	{
		DEF_LOG_ERROR("NOT register sender callback");
		return;
	}

	vrpublic_cb(p);
}

void Sender::sendMsg2TR(InterPacket* p)
{
	if (!tr_cb)
	{
		DEF_LOG_ERROR("NOT register sender callback");
		return;
	}

	tr_cb(p);
}

void Sender::sendMsg2Friend(InterPacket* p)
{
	if (!friend_cb)
	{
		DEF_LOG_ERROR("NOT register sender callback");
		return;
	}

	friend_cb(p);
}


void Sender::sendMsg2VR(ClientPacket* p)
{
	if (!cl2vr_cb)
	{
		DEF_LOG_ERROR("NOT register sender callback");
		return;
	}

	cl2vr_cb(p);
}

void Sender::sendMsg2VRPublic(ClientPacket* p)
{
	if (!cl2vrp_cb)
	{
		DEF_LOG_ERROR("NOT register sender callback");
		return;
	}

	cl2vrp_cb(p);
}

void Sender::sendMsg2TR(ClientPacket* p)
{
	if (!cl2tr_cb)
	{
		DEF_LOG_ERROR("NOT register sender callback");
		return;
	}

	cl2tr_cb(p);
}

void Sender::sendMsg2Friend(ClientPacket* p)
{
	if (!cl2friend_cb)
	{
		DEF_LOG_ERROR("NOT register sender callback");
		return;
	}

	cl2friend_cb(p);
}

void Sender::sendMsg2Battle(ClientPacket* p)
{
	if (!cl2battle_cb)
	{
		DEF_LOG_ERROR("NOT register sender callback");
		return;
	}

	cl2battle_cb(p);
}
