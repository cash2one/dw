
#include "StatCount.h"

StatCount::StatCount()
	: m_new_count(0)
	, m_delete_count(0)
{
}

StatCount::~StatCount()
{

}

uint32 StatCount::addNewCount()
{
	boost::mutex::scoped_lock lock(m_mutex);
	++m_new_count;
	return m_new_count;
}

uint32 StatCount::addRemoveCount()
{
	boost::mutex::scoped_lock lock(m_mutex);
	++m_delete_count;

	return m_delete_count;
}

