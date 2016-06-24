#include "SqlReader.h"

SqlReader::SqlReader(sqlite3_stmt *stmt)
:stmt_(stmt)
{
}

SqlReader::SqlReader()
:stmt_(0)
{

}

SqlReader::~SqlReader(void)
{
	Close();
}

bool SqlReader::Read()
{
	if( !IsOpened())
		return false;

	return SQLITE_ROW == sqlite3_step(stmt_);
}

void SqlReader::Close()
{
	if( !IsOpened())
		return;

	sqlite3_finalize(stmt_);
	stmt_ = 0;
}

int SqlReader::ColumnCount()
{	
	if( !IsOpened())
		return -1;

	return sqlite3_column_count(stmt_);
}

int SqlReader::ValueType(int col)
{
	if( !IsOpened())
		return -1;

	return sqlite3_column_type(stmt_, col);
}

const char* SqlReader::StringValue(int col)
{
	if( !IsOpened())
		return 0;

	return (const char*)sqlite3_column_text(stmt_, col);
}

int SqlReader::IntValue(int col)
{
	 if( !IsOpened())
		return 0;
	
	 return sqlite3_column_int(stmt_, col);
}


__int64 SqlReader::Int64Value(int col)
{
	if( !IsOpened())
		return 0;

	return sqlite3_column_int64(stmt_, col);
}

double SqlReader::DoubleValue(int col)
{
	 if( !IsOpened())
		 return 0;

	 return sqlite3_column_double(stmt_, col);
}


const void* SqlReader::BlobValue(int col, int& len)
{
	if( !IsOpened())
		return 0;
	
	len = sqlite3_column_bytes(stmt_, col);
	return sqlite3_column_blob(stmt_, col);
}

bool SqlReader::IsOpened()
{
	return 0 != stmt_;
}
