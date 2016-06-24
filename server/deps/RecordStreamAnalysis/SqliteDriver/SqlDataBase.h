#pragma once
#include <boost/noncopyable.hpp>
#include "sqlite3.h"
#include "SqlReader.h"

class SqlDataBase :boost::noncopyable
{
public:
	SqlDataBase(void);
	~SqlDataBase(void);

public:
	bool Open(const char* file_name);
	void Close();

	//> 执行查询请求
	SqlReader ExecuteQuery(const char* sql);

	//> 执行非查询请求
	bool ExecuteNonQuery(const char* sql);

private:
	bool isOpened();

private:
	sqlite3	*db_;
};
