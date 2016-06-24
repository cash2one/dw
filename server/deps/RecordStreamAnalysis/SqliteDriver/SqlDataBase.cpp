#include "SqlDataBase.h"

SqlDataBase::SqlDataBase(void)
:db_(0)
{
}

SqlDataBase::~SqlDataBase(void)
{
	Close();
}

bool SqlDataBase::Open(const char* file_name)
{
	if(isOpened())
		Close();

	return SQLITE_OK == sqlite3_open(file_name, &db_);
}

void SqlDataBase::Close()
{
	if(!isOpened())
		return;

	sqlite3_close(db_);
	db_ = 0;
}

SqlReader SqlDataBase::ExecuteQuery(const char* sql)
{
	if( !isOpened())
		return SqlReader();
	
	sqlite3_stmt *stmt = 0;
	sqlite3_prepare_v2(db_, sql, -1, &stmt, 0);
		
	return SqlReader(stmt);
}

bool SqlDataBase::ExecuteNonQuery(const char* sql)
{	
	if( !isOpened())
		return false;

	return SQLITE_OK == sqlite3_exec(db_, sql, 0, 0, 0);
}

bool SqlDataBase::isOpened()
{
	return 0 != db_;
}
