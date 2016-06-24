#pragma  once

#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
typedef __int64  Integer;


struct  Recordst 
{	
	Recordst()
		:guid(0)
		,object_guid(0)
		,owner_guid(0)
		,opcode(0)
		,stream_size(0)
		,optype(0)
		,last_modify_time(0)
		,trans_index(0)
		,trans_num(0)
		,trans_id(0)
		,line_id(0)
	{	
		//> 必须先有内容才能resize
		stream.push_back('\0');
	}


	Integer guid;
	Integer object_guid;
	Integer owner_guid;
	Integer opcode;
	Integer stream_size;
	std::vector<char> stream;
	Integer optype;
	Integer last_modify_time;
	Integer trans_index;
	Integer trans_num;
	Integer trans_id;
	Integer line_id;
};

typedef Integer Date;
typedef std::multimap<Date,  boost::shared_ptr<Recordst> > HistoryRecord;

typedef Integer Guid;
typedef std::map<Guid, boost::shared_ptr<HistoryRecord> > GuidCache;
typedef std::vector< boost::shared_ptr<Recordst> > RecordArray;