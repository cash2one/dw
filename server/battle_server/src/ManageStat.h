
#ifndef MANAGE_STAT_HPP__
#define MANAGE_STAT_HPP__

#include <boost/thread.hpp>
#include <boost/cstdint.hpp>

struct StatPackInfo 
{
	StatPackInfo()
		: stat_num(0)
		, stat_bytes(0)
		, start_time(0)
	{}

	boost::uint64_t stat_num;
	boost::uint64_t stat_bytes;
	time_t start_time;
};

class ManageStat
{
public:
	ManageStat();
	~ManageStat();
public:
	void addIncomingPacket(int pack_num, int stream_bytes);
	void addOutgoingPacket(int pack_num, int stream_bytes);

	void update();

private:
	StatPackInfo m_incoming_pack_info;
	StatPackInfo m_outgoing_pack_info;

	time_t m_last_record_time;
	time_t m_start_time;
};

#endif