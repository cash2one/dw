#pragma  once
#include <functional>
#include "SessionHandle.h"

struct InterPacket;
struct ClientPacket;

struct Sender
{	
	// server -> server
	void sendMsg2Client(InterPacket* p);
	void sendMsg2VR(InterPacket* p);
	void sendMsg2VRPublic(InterPacket* p);
	void sendMsg2TR(InterPacket* p);
	void sendMsg2Friend(InterPacket* p);

	// client -> server
	void sendMsg2VR(ClientPacket* p);
	void sendMsg2VRPublic(ClientPacket* p);
	void sendMsg2TR(ClientPacket* p);
	void sendMsg2Friend(ClientPacket* p);
	void sendMsg2Battle(ClientPacket* p);

	typedef std::function< bool(InterPacket *)> ServerCallback;
	ServerCallback client_cb;
	ServerCallback vr_cb;
	ServerCallback vrpublic_cb;
	ServerCallback tr_cb;
	ServerCallback friend_cb;

	typedef std::function< void(ClientPacket *)> ClientCallback;
	ClientCallback cl2vr_cb;
	ClientCallback cl2vrp_cb;
	ClientCallback cl2tr_cb;
	ClientCallback cl2friend_cb;
	ClientCallback cl2battle_cb;
};