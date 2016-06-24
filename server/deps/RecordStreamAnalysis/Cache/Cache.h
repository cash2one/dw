#pragma once

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include "..\include\type.h"

class Cache :boost::noncopyable
{
public:
	Cache(void);
	~Cache(void);
	bool Add(Recordst& record);
	bool GetObj(Guid object_guid, Date start_date, Date end_date, HistoryRecord& record);
	bool GetOwner(Guid owner_guid, Date start_date, Date end_date, HistoryRecord& record);
	void Clean();
	
private:
	boost::mutex mu_;
	GuidCache  cache_;
};
