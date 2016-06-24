#pragma once
#include "sqlite3.h"

class SqlReader
{
public:
	SqlReader(sqlite3_stmt* stmt);
	SqlReader();
	~SqlReader(void);

	bool IsOpened();
	
	//> 读取一行
	bool Read();
	void Close();
	
	//>行数
	int ColumnCount();
	//>列的数据类型
	int ValueType(int col);

	const char* StringValue(int col);
	int IntValue(int col);
	__int64 Int64Value(int col);
	double	DoubleValue(int col);
	const void* BlobValue(int col, int& len);


private:
	sqlite3_stmt *stmt_;
};
