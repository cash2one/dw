
#ifndef CLIENT_MESSAGE_MANAGER_HPP__
#define CLIENT_MESSAGE_MANAGER_HPP__

#include <unordered_map>
#include "Handle.h"
#include "PackMsg.h"
#include "Packet_.h"
#include "Player.h"

using namespace std;

class ClientMessageManager
{
public:
	ClientMessageManager();
	~ClientMessageManager();
	static ClientMessageManager * instance();

public:
	int update(int & incoming_packet_num, int & incoming_bytes);

	void handlePlayerPacket(InterPacket * packet);

	void handlePlayerPacket(InterPacketVec_t & pack_vec);

	void getClientPacket(InterPacketVec_t & client_packet_vec);

	bool addPlayerMsgHandle(int msg_id, MsgHandleBase * message);

	void handlePlayerMsg(session_t session_id, InterPacket * packet);

	void setDefaultMsgHandler(std::function< int(int, InterPacket*, char*, int) > cb) { m_default_cb = cb; }
protected:
private:
	static ClientMessageManager * m_instance;

	typedef unordered_map<int, MsgHandleBase *> MessageHandleMap_t;

	MessageHandleMap_t m_msg_handle_map;

	InterPacketVec_t m_client_packet;

	boost::mutex m_client_packet_mutex;

	std::function< int(int, InterPacket*, char*, int) > m_default_cb;
};

#endif
