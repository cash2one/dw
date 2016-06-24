
#ifndef PACK_MSG_HPP__
#define PACK_MSG_HPP__

#include "flatbuffers/flatbuffers.h"
#include "Handle.h"
#include "Packet_.h"
#include "Logger.h"
#include "BattleLog.h"

class MsgHandleBase
{
public:
	MsgHandleBase();
	virtual ~MsgHandleBase();
	// server interval msg
	virtual bool handleServerMsg(session_t session_id, char * buffer, int buffer_size) = 0;
	// client msg
	virtual bool handle(session_t session_id, char * buffer, int buffer_size) = 0;

	virtual bool handleInterMsg(InterPacket * packet, char * buffer, int buffer_size) = 0;

	int msg_id;
};

template<class T, class MsgType>
class PackMsg : public MsgHandleBase
{
public:
	typedef MsgType _MsgType;
	typedef int (T::*MsgFunc)(session_t, const MsgType*);
	typedef int (T::*InterMsgFunc)(InterPacket *, const MsgType*);

	PackMsg(T * call_ins, MsgFunc fun, InterMsgFunc inter_fun = NULL)
		: m_msg_id(0)
		, m_call_ins(call_ins)
		, m_fun(fun)
		, m_inter_fun(inter_fun)
	{}

	bool handle(session_t session_id, char * buffer, int buffer_size)
	{
		if (NULL == m_fun)
		{
			BATTLE_LOG_ERROR("the function is NULL");
			return false;
		}
		if (NULL == buffer)
		{
			BATTLE_LOG_ERROR("get null point of client msg, msg is is <%d> len is <%d>", buffer_size, buffer_size);
			return false;
		}
		char * buf = buffer;
		const MsgType * msg = flatbuffers::GetRoot<MsgType>(buf);
		//MsgType * msg = flatbuffers::GetRoot<MsgType>(buffer);
		flatbuffers::Verifier verifier((uint8_t *)buffer, buffer_size);
		if (NULL != msg && msg->Verify(verifier))
		{
			(m_call_ins->*m_fun)(session_id, msg);
			return true;
		}
		else
		{
			ClientHead head;
			memcpy(&head, buffer - ClientPacket::pack_head_size, ClientPacket::pack_head_size);
			if (NULL == msg)
			{
				BATTLE_LOG_ERROR("get NULL msg, msg id is <%hd>, len is <%hd>", head.msg_id, head.msg_len);
			}
			else
			{
				BATTLE_LOG_ERROR("get msg, verify failed, msg id is <%hd>, len is <%hd>", head.msg_id, head.msg_len);
			}
			// error
		}
		return false;
	}

	bool handleServerMsg(session_t session_id, char * buffer, int buffer_size)
	{
		if (NULL == m_fun)
		{
			BATTLE_LOG_ERROR("the function is NULL");
			return false;
		}

		if (NULL == buffer)
		{
			(m_call_ins->*m_fun)(session_id, NULL);
			return true;
		}

		char * buf = buffer;
		const MsgType * msg = flatbuffers::GetRoot<MsgType>(buf);
		flatbuffers::Verifier verifier((uint8_t *)buffer, buffer_size);
		if (NULL != msg && msg->Verify(verifier))
		{
			(m_call_ins->*m_fun)(session_id, msg);
			return true;
		}
		else
		{
			ClientHead head;
			memcpy(&head, buffer - ClientPacket::pack_head_size, ClientPacket::pack_head_size);
			if (NULL == msg)
			{
				BATTLE_LOG_ERROR("get NULL msg, msg id is <%d>, len is <%d>", head.msg_id, head.msg_len);
			}
			else
			{
				BATTLE_LOG_ERROR("get msg, verify failed, msg id is <%d>, len is <%d>", head.msg_id, head.msg_len);
			}
			// error
			return false;
		}
	}

	virtual bool handleInterMsg(InterPacket * packet, char * buffer, int buffer_size)
	{
		if (NULL == m_inter_fun)
		{
			BATTLE_LOG_ERROR("the function is NULL");
			return false;
		}
		if (NULL == buffer)
		{
			BATTLE_LOG_ERROR("get null point of client msg, msg is is <%d> len is <%d>", buffer_size, buffer_size);
			return false;
		}
		char * buf = buffer;
		const MsgType * msg = flatbuffers::GetRoot<MsgType>(buf);
		//MsgType * msg = flatbuffers::GetRoot<MsgType>(buffer);
		flatbuffers::Verifier verifier((uint8_t *)buffer, buffer_size);
		if (NULL != msg && msg->Verify(verifier))
		{
			(m_call_ins->*m_inter_fun)(packet, msg);
			return true;
		}
		else
		{
			if (NULL == msg)
			{
				BATTLE_LOG_ERROR("get NULL msg, msg id is <%hd>, len is <%hd>", packet->msg_head.msg_id, packet->msg_head.msg_len);
			}
			else
			{
				BATTLE_LOG_ERROR("get msg, verify failed, msg id is <%hd>, len is <%hd>", packet->msg_head.msg_id, packet->msg_head.msg_len);
			}
			// error
		}
		return false;
	}
public:
	int m_msg_id;
	T * m_call_ins;
	MsgFunc m_fun;
	InterMsgFunc m_inter_fun;
};

#endif
