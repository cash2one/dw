
#ifndef MANAGE_VRPUBLIC_MESSAGE_HPP__
#define MANAGE_VRPUBLIC_MESSAGE_HPP__

#include <unordered_map>
#include "Handle.h"
#include "PackMsg.h"
#include "Packet_.h"
#include "Player.h"

using namespace std;

class ManageVrpublicMessage
{
public:
	ManageVrpublicMessage();
	~ManageVrpublicMessage();
public:
	static ManageVrpublicMessage * instance();
public:
	int update();

	void handleInputPacket(InterPacket * packet);

	void handleInputPacket(InterPacketVec_t & pack_vec);

	void getInputPacket(InterPacketVec_t & pack_vec);

	bool addInputMsgHandle(int msg_id, MsgHandleBase * msg_handle);

	void handleInputMsgHandle(session_t session_id, InterPacket * packet);

	void setDefaultMsgHandler(std::function< int(int, InterPacket*, char*, int) > cb) { m_default_cb = cb; }

protected:
private:
	typedef unordered_map<int, MsgHandleBase *> MessageHandleMap_t;

	MessageHandleMap_t m_msg_handle_map;

	InterPacketVec_t m_input_packet_vec;

	boost::mutex m_input_packet_vec_mtx;

	static ManageVrpublicMessage * m_instance;
	
	std::function< int(int, InterPacket*, char*, int) > m_default_cb;
};
#endif
