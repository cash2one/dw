#include "ParseInterface.h"

ParseInterface::ParseInterface()
:obj_(NULL)
{

}

ParseInterface::~ParseInterface()
{
	if(NULL != obj_)
	{
		delete obj_;
	}
}

MessageMap* ParseInterface::Parse(uint64_t guid, const char* buf, int buf_size, std::string& type_name)
{
	// todo 通过guid 获取 type name

	MessageMap* map_msg = NULL;
	parser_.getMessageContent(type_name, buf, buf_size, map_msg);

	return map_msg;
}

void ParseInterface::DelMapMsg(MessageMap*& map_content)
{
	parser_.deleteMessageContent(map_content);
}
