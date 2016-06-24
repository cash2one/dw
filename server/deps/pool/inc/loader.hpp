#pragma  once
#include "actor.hpp"
#include "dynamicparse.h"
#include "Single.hpp"

namespace google
{
	namespace protobuf
	{
		class Message;
	}
}
enum DB_OP
{
	DB_LOAD,
	DB_UPDATE,
	DB_INSERT,
};

struct RequestInfost;
typedef std::function<void(RequestInfost*)> RequestCallBack;

struct RequestInfost
{	
	DB_OP op = DB_LOAD;
	google::protobuf::Message * as = NULL;
	uint64_t guid = 0;
	uint32_t id = 0;
	RequestCallBack cb;
	bool res = false;
};

class Loader
	:public Actor<RequestInfost*>
{
public:
	virtual int DoOnce(mysqlpp::Connection& con, RequestInfost*& task) override
	{	
		CacheAssistantx assist;
		assist.msg = task->as;
		assist.myguid = task->guid;
		assist.parser = Single<DynamicParser>::GetInstance();
		mysqlpp::Query query = con.query();
		switch (task->op)
		{
		case DB_LOAD:
			task->res = assist.load(query);
			break;

		case DB_UPDATE:
			task->res = assist.update(query);
			break;

		case DB_INSERT:
			task->res = assist.insert(query);
			break;
		}
		
		task->cb(task);
		return 0;
	};

	virtual int DoOthers() override
	{
		return -1;
	};
};