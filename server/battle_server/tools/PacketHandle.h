
#ifndef PACKET_HANDLE_HPP__
#define PACKET_HANDLE_HPP__

#include "Logger.h"
#include "Packet_.h"

template<class _PackType>
class PacketHandleBase
{
public:
	virtual bool handlePacketMsg(_PackType * packet, char * buffer, int buffer_size) = 0;
};

template <class _InsType, class _PackType, class _MsgType>
class PacketHandle : public PacketHandleBase<_PackType>
{
public:
	typedef int (_InsType::*HandlePacketMsgFun)(_PackType *, const _MsgType *);

	PacketHandle(_InsType * call_ins, HandlePacketMsgFun msg_fun)
		: m_call_ins(call_ins)
		, m_fun(msg_fun)
	{}

	~PacketHandle()
	{}

	virtual bool handlePacketMsg(_PackType * packet, char * buffer, int buffer_size)
	{
		if (NULL == m_fun)
		{
			DEF_LOG_ERROR("the function is NULL");
			return false;
		}

		if (buffer_size > 0 && NULL == buffer)
		{
			DEF_LOG_ERROR("get null point of client msg, msg is is <%d> len is <%d>", buffer_size, buffer_size);
			return false;
		}

		if (buffer_size > 0)
		{
			const _MsgType * msg = flatbuffers::GetRoot<_MsgType>(buffer);
			flatbuffers::Verifier verifier((uint8_t *)buffer, buffer_size);
			if (NULL != msg && msg->Verify(verifier))
			{
				(m_call_ins->*m_fun)(packet, msg);
				return true;
			}
			else
			{
				DEF_LOG_ERROR("get NULL msg, msg id is <%hd>, len is <%hd>", packet->msg_head.msg_id, packet->msg_head.msg_len);
				return false;
			}
		}
		else
		{
			// empty message entity
			(m_call_ins->*m_fun)(packet, NULL);
			return true;
		}
	}

private:
	_InsType * m_call_ins;
	HandlePacketMsgFun m_fun;
};

#endif
