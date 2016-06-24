#pragma once
#include "typedef.h"
#include <string>
#include "flatbuffers/idl.h"

struct Fieldst 
{
	std::string name;
	std::string type;
	std::string v;
};


struct FBParser
{
	int init(const std::string proto_path);
	int setRootType(const std::string type);
	int getFields(const std::string type, std::vector<Fieldst>& fields);

	int parse(const std::string& a_bin, std::string& json);

	int setField(const std::string name, const std::string v);
	int finish(std::string& a_out);

	
private:
	flatbuffers::Parser m_parser;
	std::vector<Fieldst> m_buffer;
};