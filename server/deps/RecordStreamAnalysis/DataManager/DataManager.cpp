#include "DataManager.h"
#include <boost/bind.hpp>
#include <algorithm>

#include "..\SqliteDriver\SqlDataBase.h"

#define QUERY_ALL_TABLE	"select * from record"
#define RECORD_COLUMN_COUNT 12

DataManager::DataManager(void)
:work_pool(1)
,listen_pool(1)
{
	listen_pool.schedule( boost::bind(&DataManager::listen, this));
}

DataManager::~DataManager(void)
{
	Stop();
}

bool DataManager::GetObject(Guid object_guid, Date start_date, Date end_date, HistoryRecord& record)
{
	return cache_.GetObj(object_guid, start_date, end_date, record);
}

bool DataManager::GetOwner(Guid owner_guid, Date start_date, Date end_date, HistoryRecord& record)
{
	return cache_.GetOwner(owner_guid, start_date, end_date, record);
}

void DataManager::Clean()
{
	cache_.Clean();
}

bool DataManager::LoadDatabase(const char* db)
{
	boost::mutex::scoped_lock lock(mu_);

	std::set<std::string>::const_iterator quque_it = db_queued_.find(db);
	std::set<std::string>::const_iterator set_it = db_loaded_.find(db);
	
	if( quque_it != db_queued_.end() || set_it != db_loaded_.end())
		return true;
	 
	db_queued_.insert(db);
	evt_.SetEvent();

	return false;
}


void DataManager::listen()
{	
	HANDLE handles[2];
	handles[0] = evt_;
	handles[1] = close_evt_;
	do 
	{
		DWORD  wait = ::WaitForMultipleObjects(2, handles, FALSE, INFINITE);
		if( WAIT_OBJECT_0 == wait)
		{
			boost::mutex::scoped_lock lock(mu_);
			std::set<std::string>::const_iterator quque_it = db_queued_.begin();
			for(; quque_it != db_queued_.end(); ++quque_it)
			{
				work_pool.schedule( boost::bind(&DataManager::loadSingleDB, this, *quque_it) );
				db_loaded_.insert(*quque_it);
			}

			db_queued_.clear();
		}
		else
			break;

	} while (TRUE);

}

void DataManager::Wait()
{
	work_pool.wait();
}

void DataManager::Stop()
{
	close_evt_.SetEvent();
}

bool DataManager::QueryDatabase(const char* db, const char* sql, RecordArray& record_array)
{
	SqlDataBase sqlite_db;
	if( !sqlite_db.Open(db))
		return false;

	SqlReader reader = sqlite_db.ExecuteQuery(sql);

	if( !reader.IsOpened())
		return false;

	if( RECORD_COLUMN_COUNT != reader.ColumnCount())
		return false;

	while(reader.Read())
	{	
		boost::shared_ptr< Recordst>st(new Recordst);
		for( int i = 0; i < reader.ColumnCount(); i++)
		{	
			switch(i)
			{
			case 0:
				st->guid = reader.Int64Value(i);
				break;

			case 1:
				st->object_guid = reader.Int64Value(i);
				break;

			case 2:
				st->owner_guid = reader.Int64Value(i);
				break;

			case 3:
				st->opcode = reader.IntValue(i);
				break;

			case 4:
				st->stream_size = reader.IntValue(i);
				break;

			case 5:
				{
					int len = 0;
					const void * p = reader.BlobValue(i, len);
					if( NULL == p)
						break;

					st->stream.resize(len);
					memcpy_s(&(st->stream[0]), len, p, len);
				}
				break;

			case 6:
				st->optype = reader.IntValue(i);
				break;

			case 7:
				st->last_modify_time = reader.Int64Value(i);
				break;

			case 8:
				st->trans_index = reader.IntValue(i);
				break;

			case 9:
				st->trans_num = reader.IntValue(i);
				break;

			case 10:
				st->trans_id = reader.IntValue(i);
				break;

			case 11:
				st->line_id = reader.IntValue(i);
				break;
			} // end of switch

		} // end of for
		
		record_array.push_back(st);
	}

	reader.Close();	
	sqlite_db.Close();

	return true;
}

void DataManager::loadSingleDB(std::string db)
{
	SqlDataBase sqlite_db;
	if( !sqlite_db.Open(db.c_str()))
		return;

	SqlReader reader = sqlite_db.ExecuteQuery(QUERY_ALL_TABLE);
	
	do 
	{
		if( !reader.IsOpened())
			break;
		
		if( RECORD_COLUMN_COUNT != reader.ColumnCount())
			break;

		while(reader.Read())
		{		
			Recordst st;
			for( int i = 0; i < reader.ColumnCount(); i++)
			{	
				switch(i)
				{
				case 0:
					st.guid = reader.Int64Value(i);
					break;

				case 1:
					st.object_guid = reader.Int64Value(i);
					break;

				case 2:
					st.owner_guid = reader.Int64Value(i);
					break;

				case 3:
					st.opcode = reader.IntValue(i);
					break;

				case 4:
					st.stream_size = reader.IntValue(i);
					break;

				case 5:
					{
						int len = 0;
						const void * p = reader.BlobValue(i, len);
						if( NULL == p)
							break;
					
						st.stream.resize(len, '\0');
						memcpy_s(&st.stream[0], len, p, len);
					}
					break;

				case 6:
					st.optype = reader.IntValue(i);
					break;

				case 7:
					st.last_modify_time = reader.Int64Value(i);
					break;

				case 8:
					st.trans_index = reader.IntValue(i);
					break;

				case 9:
					st.trans_num = reader.IntValue(i);
					break;

				case 10:
					st.trans_id = reader.IntValue(i);
					break;

				case 11:
					st.line_id = reader.IntValue(i);
					break;
				} // end of switch

			} // end of for

			cache_.Add(st);
		}
		
		reader.Close();	
	} while (false);
	
	sqlite_db.Close();
}
