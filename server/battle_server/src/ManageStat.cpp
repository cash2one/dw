
#include "Logger.h"
#include "ManageStat.h"

ManageStat::ManageStat()
{
	m_last_record_time = getSystemMilliSeconds();
	m_start_time = getSystemMilliSeconds();
}

ManageStat::~ManageStat()
{

}

void ManageStat::addIncomingPacket(int pack_num, int stream_bytes)
{
	m_incoming_pack_info.stat_num += pack_num;
	m_incoming_pack_info.stat_bytes += stream_bytes;
}

void ManageStat::addOutgoingPacket(int pack_num, int stream_bytes)
{
	m_outgoing_pack_info.stat_num += pack_num;
	m_outgoing_pack_info.stat_bytes += stream_bytes;
}

void ManageStat::update()
{
	time_t curr_time = getSystemMilliSeconds();
	if (curr_time - m_last_record_time >= 60000)
	{
		m_last_record_time = curr_time;
		double diff_time_sec = (curr_time - m_start_time) / 1000.0;
		
		double in_pack_num = m_incoming_pack_info.stat_num / 10000.0;
		double in_pack_speed = m_incoming_pack_info.stat_num / 10000.0 / diff_time_sec;

		double in_bytes = m_incoming_pack_info.stat_bytes / 1000.0 / 1000.0;
		double in_byte_speed = m_incoming_pack_info.stat_bytes / 1000.0 / diff_time_sec;

		double out_pack_num = m_outgoing_pack_info.stat_num / 10000.0;
		double out_pack_speed = m_outgoing_pack_info.stat_num / 10000.0 / diff_time_sec;

		double out_bytes = m_outgoing_pack_info.stat_bytes / 1000.0 / 1000.0;
		double out_byte_speed = m_outgoing_pack_info.stat_bytes / 1000.0 / diff_time_sec;

		DEF_LOG_INFO("stat , incoming pack speed, pack, bytes speed, bytes <%.2f w/s, %.2f w>, <%.2f k/s, %.2f M>", in_pack_speed, in_pack_num, in_byte_speed, in_bytes);
		DEF_LOG_INFO("stat , outgoing pack speed, pack, bytes speed, bytes <%.2f w/s, %.2f w>, <%.2f k/s, %.2f M>", out_pack_speed, out_pack_num, out_byte_speed, out_bytes);
	}
}

