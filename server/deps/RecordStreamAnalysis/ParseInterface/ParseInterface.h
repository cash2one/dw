#pragma once
#include <string>
#include "dynamicparse.h"

class ParseInterface
{	
public:
	ParseInterface();
	~ParseInterface();

	MessageMap* Parse(uint64_t guid, const char* buf, int buf_size, std::string& type_name);
	void	DelMapMsg(MessageMap*& map_content);

private:
	CacheAssistantx* obj_;
	DynamicParser parser_;
};

