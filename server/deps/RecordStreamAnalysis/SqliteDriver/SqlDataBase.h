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

	//> ִ�в�ѯ����
	SqlReader ExecuteQuery(const char* sql);

	//> ִ�зǲ�ѯ����
	bool ExecuteNonQuery(const char* sql);

private:
	bool isOpened();

private:
	sqlite3	*db_;
};
