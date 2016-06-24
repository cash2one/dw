
#include <boost/lexical_cast.hpp>
#include "AllocGuid.h"
#include "Logger.h"
#include "opcode.h"
#include "ManageConfig.h"

#define MAX_GUID_INDEX uint64(0x0000FFFFFFFFFFFF)

AllocGuid::AllocGuid()
: m_seed(0)
, m_index(0)
{
}

AllocGuid::~AllocGuid()
{
}

int AllocGuid::init(uint32 seed_value, uint64 last_index)
{
	m_seed = seed_value;
	m_index = last_index;

	DEF_LOG_INFO("get sedd : <%d>, last index : <%s>\n", seed_value, boost::lexical_cast<string>(last_index).c_str());
	return 0;
}

InterPacket * AllocGuid::processRequest(InterPacket * packet)
{
	const MobaGo::FlatBuffer::SMsgRequestGuidReq * guid_req = flatbuffers::GetRoot<MobaGo::FlatBuffer::SMsgRequestGuidReq>(packet->getBody());
	flatbuffers::Verifier verifier((uint8_t *)packet->getBody(), packet->msg_head.msg_len);

	if (NULL != guid_req && guid_req->Verify(verifier))
	{
		uint32 generate_no = 0;
		uint64 start_index = 0;
		if (generateGuid(guid_req->request_number(), generate_no, start_index))
		{
			flatbuffers::FlatBufferBuilder build;
			flatbuffers::Offset<MobaGo::FlatBuffer::SMsgRequestGuidAck> guid_ack_off = MobaGo::FlatBuffer::CreateSMsgRequestGuidAck(build, m_seed, start_index, generate_no);
			build.Finish(guid_ack_off);

			InterPacket * ret = new InterPacket();
			ret->setMsgInfo(E_SMsgRequestGuidAck, 0llu, (int)build.GetSize(), (char *)build.GetBufferPointer());
			return ret;
		}
		else
		{
			DEF_LOG_ERROR("failed to generate guid\n");
		}
	}
	else
	{
		DEF_LOG_ERROR("failed to parse the RequestGuid packet\n");
	}

	return NULL;
}

bool AllocGuid::generateGuid(uint32 request_no, uint32 & generate_no, uint64 & start_index)
{
	start_index = m_index;
	
	if ((m_index + request_no) < MAX_GUID_INDEX)
	{
		DEF_LOG_INFO("guid request, seed is <%d>, request no is <%d>, index is <%s>\n", m_seed, request_no, boost::lexical_cast<string>(m_index).c_str());
		generate_no = request_no;
		m_index += request_no;
	}
	else
	{
		DEF_LOG_INFO("current seed is finish : <%d>, ++it, current request no : <%d>, start index <%s>\n", m_seed, request_no, boost::lexical_cast<string>(m_index).c_str());

		m_index = 1;

		generate_no = request_no;
		m_index += request_no;
		++m_seed;
	}

	return ManageConfig::instance()->writeGuidCfg(m_seed, m_index);
}