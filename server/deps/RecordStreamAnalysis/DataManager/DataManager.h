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
	
	//>��ѯcache�������
	bool GetObject(Guid object_guid, Date start_date, Date end_date, HistoryRecord& record);
	bool GetOwner(Guid owner_guid, Date start_date, Date end_date, HistoryRecord& record);
	void Clean();
	
	//>����db��cache, �Ѽ��ط���true, �����ط���false
	bool LoadDatabase(const char* db);
	//> �ȴ�db�������
	void Wait();
	
	void Stop();

	//>ֱ�Ӳ�ѯsqlite�ļ�
	bool QueryDatabase(const char* db, const char* sql, RecordArray& record_array);	

private:
	void listen();
	void loadSingleDB(std::string db);

private:
	Cache cache_;

	boost::mutex mu_; //>����db����,��ѯ��ز���
	std::set<std::string>	db_queued_;
	std::set<std::string> db_loaded_;
	CEvent evt_;
	CEvent close_evt_;
	pool work_pool;
	pool listen_pool;
};
