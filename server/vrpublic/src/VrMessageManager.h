
#ifndef VR_MESSAGE_MANAGER_HPP__
#define VR_MESSAGE_MANAGER_HPP__

#include <unordered_map>
#include "Handle.h"
#include "PackMsg.h"
#include "Packet_.h"
#include "Player.h"

class VrMessageManager
{
public:
	VrMessageManager();
	~VrMessageManager();
public:
	static VrMessageManager * instance();
public:
	int update(int & incoming_packet_num, int & incoming_bytes);

	void handleVrPacket(InterPacketVec_t & pack_vec);

	void handleVrPacket(InterPacket * packet);

	void getVrPacket(InterPacketVec_t & vr_packet_vec);

	bool addVrMsgHandle(int msg_id, MsgHandleBase * message);

	void handleVrMsg(session_t session_id, InterPacket * packet);
protected:
private:
	typedef unordered_map<int, MsgHandleBase *> MessageHandleMap_t;

	MessageHandleMap_t m_msg_handle_map;

	InterPacketVec_t m_vr_packet_vec;

	boost::mutex m_vr_packet_vec_mtx;

	static VrMessageManager * m_instance;
};

#endif
