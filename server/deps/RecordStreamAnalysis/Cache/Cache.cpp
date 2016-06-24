#include "Cache.h"
#include <utility>

Cache::Cache(void)
{
}

Cache::~Cache(void)
{
}

bool Cache::Add(Recordst& record)
{
	boost::mutex::scoped_lock lock(mu_);

	GuidCache::iterator it = cache_.find(record.object_guid);
	if(it == cache_.end())
	{	
		boost::shared_ptr<HistoryRecord> history_record(new HistoryRecord);
		boost::shared_ptr<Recordst> current_record(new Recordst);
		*current_record = record;
		history_record->insert(std::make_pair(current_record->last_modify_time, current_record));
		
		cache_.insert(std::make_pair(current_record->object_guid, history_record));
	}
	else
	{
		boost::shared_ptr<HistoryRecord> history_record = it->second;
		boost::shared_ptr<Recordst> current_record(new Recordst);
		*current_record = record;
		history_record->insert(std::make_pair(current_record->last_modify_time, current_record));
		cache_.insert(std::make_pair(current_record->object_guid, history_record));
	}

	return true;
}

bool Cache::GetObj(Guid object_guid, Date start_date, Date end_date, HistoryRecord& record)
{
	boost::mutex::scoped_lock lock(mu_);

	GuidCache::iterator it = cache_.find(object_guid);
	if(it == cache_.end())
		return false;

	boost::shared_ptr<HistoryRecord> history_record = it->second;
	HistoryRecord::const_iterator start_it;
    if( 0 != start_date)
        start_it = history_record->lower_bound(start_date);
    else
        start_it = history_record->begin();

	if( start_it == history_record->end())
		return false;

	HistoryRecord::const_iterator end_it;
    if( 0 != end_date)
        end_it = history_record->lower_bound(end_date);
    else
        end_it = history_record->end();

	
	for( ; start_it != end_it; ++start_it)
	{
		boost::shared_ptr<Recordst> source_record = start_it->second;
		boost::shared_ptr<Recordst> target_record(new Recordst);

		*target_record = *source_record;

		record.insert(std::make_pair(start_it->first, target_record));
	}
	
	return true;
}

void Cache::Clean()
{
	boost::mutex::scoped_lock lock(mu_);
	cache_.clear();
}

bool Cache::GetOwner(Guid owner_guid, Date start_date, Date end_date, HistoryRecord& record)
{
	boost::mutex::scoped_lock lock(mu_);

	GuidCache::const_iterator it = cache_.begin();
	for( ; it != cache_.end(); ++it)
	{
		boost::shared_ptr<HistoryRecord> history_record = it->second;
		HistoryRecord::const_iterator start_it;
        if( 0 != start_date)
            start_it = history_record->lower_bound(start_date);
        else
            start_it = history_record->begin();

		if( start_it == history_record->end())
			continue;

		HistoryRecord::const_iterator end_it;
        if( 0 != end_date)
            end_it = history_record->lower_bound(end_date);
        else
            end_it = history_record->end();

		for( ; start_it != end_it; ++start_it)
		{	

			boost::shared_ptr<Recordst> source_record = start_it->second;

			if(source_record->owner_guid == owner_guid)
			{
				boost::shared_ptr<Recordst> target_record(new Recordst);
				*target_record = *source_record;
				record.insert(std::make_pair(start_it->first, target_record));
			}
		}

	}

	return !record.empty();
}
