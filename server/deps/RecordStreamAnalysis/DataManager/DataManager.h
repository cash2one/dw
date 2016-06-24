#pragma once
#include <queue>
#include <set>
#include <string>
#include <boost/thread/mutex.hpp>
#include "..\Cache\Cache.h"
#include "..\include\threadpool.hpp"
#define _WIN32_WINNT 0x0501
#include <afxmt.h>

using namespace boost::threadpool;

class DataManager :boost::noncopyable
{
public:
	DataManager(void);
	~DataManager(void);
	
	//>查询cache里的数据
	bool GetObject(Guid object_guid, Date start_date, Date end_date, HistoryRecord& record);
	bool GetOwner(Guid owner_guid, Date start_date, Date end_date, HistoryRecord& record);
	void Clean();
	
	//>加载db进cache, 已加载返回true, 待加载返回false
	bool LoadDatabase(const char* db);
	//> 等待db加载完毕
	void Wait();
	
	void Stop();

	//>直接查询sqlite文件
	bool QueryDatabase(const char* db, const char* sql, RecordArray& record_array);	

private:
	void listen();
	void loadSingleDB(std::string db);

private:
	Cache cache_;

	boost::mutex mu_; //>用于db加载,查询相关操作
	std::set<std::string>	db_queued_;
	std::set<std::string> db_loaded_;
	CEvent evt_;
	CEvent close_evt_;
	pool work_pool;
	pool listen_pool;
};
