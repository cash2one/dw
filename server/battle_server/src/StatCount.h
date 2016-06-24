
#ifndef STAT_COUNT_HPP__
#define STAT_COUNT_HPP__

#include <boost/thread.hpp>
#include "typedef.h"

class StatCount
{
public:
	StatCount();
	~StatCount();
public:
	uint32 addNewCount();
	uint32 addRemoveCount();
protected:
private:
	uint32 m_new_count;
	uint32 m_delete_count;
	boost::mutex m_mutex;
};

#endif
