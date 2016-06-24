
#include <fstream>
#include <boost/filesystem.hpp>
#include "ProtocolMaker.h"


using namespace boost::filesystem;

ProtocolMaker::ProtocolMaker()
{

}

ProtocolMaker::~ProtocolMaker()
{

}

void ProtocolMaker::init(const string & source_path, const string & output_path)
{
	m_source_path = source_path;
	m_output_path = output_path;

	std::vector<string> file_list;
	this->getFileList(source_path, file_list);

	for (auto file_name : file_list)
	{
		std::cout << "get fsb file : " << file_name << std::endl;
		makeProtocolCpp(file_name);
		saveProtocolCpp(file_name);
	}
}

void ProtocolMaker::getFileList(const string & file_path, std::vector<string> & file_list)
{
	path parent_path(file_path);
	if (!is_directory(parent_path))
	{
		std::cout << "the file path is not valid : " << file_path.c_str() << std::endl;
		return;
	}

	string file_ext(".fbs");

	directory_iterator end_it;
	for (directory_iterator it(parent_path); it != end_it; ++it)
	{
		if (!is_directory(*it))
		{
			if (it->path().extension() == file_ext)
			{
				// find one
				file_list.push_back(it->path().filename().string());
			}
		}
	}
}

void ProtocolMaker::makeProtocolCpp(const string & protocol_file_name)
{
	std::string schema_file;
	string full_file_path = m_source_path + "/" + protocol_file_name;
	bool ok = flatbuffers::LoadFile(full_file_path.c_str(), false, &schema_file);
	if (!ok)
	{
		std::cout << "failed to load file : " << full_file_path << std::endl;
		return;
	}

	flatbuffers::Parser parser;
	const char * include_dir[2];
	include_dir[0] = m_source_path.c_str();
	include_dir[1] = nullptr;

	ok = parser.Parse(schema_file.c_str(), include_dir);

	if (!ok)
	{
		std::cout << "failed to parse schema file : " << schema_file << std::endl;
		return;
	}

	for (auto one_schema : parser.structs_.vec)
	{
		StructDef & struct_def = *one_schema;
		if (canMakeProtocolCpp(struct_def))
		{
			CppInfo * cpp_info = new CppInfo();
			cpp_info->msg_name = struct_def.name;
			cpp_info->msg_c_h = makeProtocolH(struct_def);
			cpp_info->msg_c_cpp = makeProtocolCpp(struct_def);

			m_cpp_info_vec.push_back(cpp_info);
		}
	}

}

bool ProtocolMaker::canMakeProtocolCpp(StructDef & struct_def)
{
	for (auto field_def : struct_def.fields.vec)
	{
		if (BASE_TYPE_UNION == field_def->value.type.base_type || BASE_TYPE_STRUCT == field_def->value.type.base_type)
		{
			return false;
		}

		if ((BASE_TYPE_VECTOR == field_def->value.type.base_type) && (BASE_TYPE_UNION == field_def->value.type.element || BASE_TYPE_STRUCT == field_def->value.type.element))
		{
			return false;
		}
	}

	return true;
}

string packet_type = "InterPacket *";

string ProtocolMaker::makeProtocolH(StructDef & struct_def)
{
	string ret;
	ret += packet_type + " make" + struct_def.name + "(int msg_id, uint64_t guid, ";
	bool is_first = true;
	for (auto field_def : struct_def.fields.vec)
	{
		if (is_first)
		{
			is_first = false;
		}
		else
		{
			ret += ", ";
		}

		ret += getCppType(field_def) + " " + field_def->name;
	}
	ret += ")";
	return ret;
}

string ProtocolMaker::makeProtocolCpp(StructDef & struct_def)
{
	string ret;
	ret = makeProtocolH(struct_def);
	ret += "\n{\n";
	ret += "\tflatbuffers::FlatBufferBuilder builder;\n";
	for (auto field_def : struct_def.fields.vec)
	{
		if (BASE_TYPE_STRING == field_def->value.type.base_type)
		{
			ret += "\tflatbuffers::Offset<flatbuffers::String> " + field_def->name + "_off = builder.CreateString(" + field_def->name + ");\n";
		}
		else if (BASE_TYPE_VECTOR == field_def->value.type.base_type)
		{
			ret += "\tflatbuffers::Offset<flatbuffers::Vector<" + getCppType(field_def->value.type.element) + ">> " + field_def->name + "_off = builder.CreateVector(" + field_def->name + ");\n";
		}
	}

	ret += "\tflatbuffers::Offset<" + struct_def.name + "> off_set = Create" + struct_def.name + "(builder ,";

	bool is_first = true;
	for (auto field_def : struct_def.fields.vec)
	{
		if (is_first)
		{
			is_first = false;
		}
		else
		{
			ret += ", ";
		}
		
		if (BASE_TYPE_STRING == field_def->value.type.base_type || BASE_TYPE_VECTOR == field_def->value.type.base_type)
		{
			ret += field_def->name + "_off";
		}
		else
		{
			ret += field_def->name;
		}
	}
	ret += ");\n";
	
	ret += "\tbuilder.Finish(off_set);\n";

	ret += "\tInterPacket * pack = new InterPacket();\n";
	ret += "\tpack->setMsgInfo(msg_id, guid, builder.GetSize(), builder.GetBufferPointer());\n";
	ret += "\treturn pack;\n";

	ret += "}";
	return ret;
}

string ProtocolMaker::getCppType(FieldDef * field_def)
{
	string ret;
	if (BASE_TYPE_UNION == field_def->value.type.base_type || BASE_TYPE_STRUCT == field_def->value.type.base_type)
	{
		ret = "unknown";
	}
	else if (BASE_TYPE_VECTOR == field_def->value.type.base_type)
	{
		ret = "const std::vector<" + getCppType(field_def->value.type.base_type) + "> &";
	}
	else
	{
		ret = getCppType(field_def->value.type.base_type);
	}

	return ret;
}

string ProtocolMaker::getCppType(BaseType base_type)
{
	string ret;
	switch (base_type)
	{
	case BASE_TYPE_NONE:
		ret = "uint8_t";
		break;
	case BASE_TYPE_UTYPE:
		ret = "uint8_t";
		break;
	case BASE_TYPE_CHAR:
		ret = "int8_t";
		break;
	case BASE_TYPE_UCHAR:
		ret = "uint8_t";
		break;
	case BASE_TYPE_SHORT:
		ret = "int16_t";
		break;
	case BASE_TYPE_USHORT:
		ret = "uint16_t";
		break;
	case BASE_TYPE_INT:
		ret = "int";
		break;
	case BASE_TYPE_BOOL:
		ret = "bool";
		break;
	case BASE_TYPE_UINT:
		ret = "uint32_t";
		break;
	case BASE_TYPE_LONG:
		ret = "int64_t";
	case BASE_TYPE_ULONG:
		ret = "uint64_t";
		break;
	case BASE_TYPE_FLOAT:
	case BASE_TYPE_DOUBLE:
		ret = "double";
		break;
	case BASE_TYPE_STRING:
		ret = "const string &";
		break;
	default:
		ret = "unknown";
		break;
	}

	return ret;
}

void ProtocolMaker::saveProtocolCpp(const string & file_name)
{
	if (m_cpp_info_vec.size() == 0)
	{
		std::cout << "do not process file : " << file_name << std::endl;
		return;
	}

	string name = file_name;
	if (name.size() > 4)
	{
		name[name.size() - 4] = '\0';
	}

	this->saveProtocolCppH(name.c_str());
	this->saveProtocolCppCpp(name.c_str());

	for (auto cpp_info : m_cpp_info_vec)
	{
		delete cpp_info;
	}
	m_cpp_info_vec.clear();
}

void ProtocolMaker::saveProtocolCppH(const string & name)
{
	string cpp_h_name = "make_" + name + ".h";

	string file_content;
	file_content += "#include <string>\n";
	file_content += "#include <vector>\n";
	file_content += "#include <flatbuffers/flatbuffers.h>";
	file_content += "#include \"" + name + "_generated.h\"\n";
	file_content += "#include \"Packaet_.h\"\n";
	file_content += "\n";
	file_content += "using namespace std;\n";
	file_content += "using namespace MobaGo::FlatBuffer;\n";
	file_content += "using namespace flatbuffers;\n";

	file_content += "\n";
	file_content += "\n";

	for (auto cpp_info : m_cpp_info_vec)
	{
		file_content += cpp_info->msg_c_h + ";\n\n";
	}

	file_content += "\n";
	this->saveFile(cpp_h_name, file_content);
}

void ProtocolMaker::saveProtocolCppCpp(const string & name)
{
	string cpp_cpp_name = "make_" + name + ".cpp";

	string file_content;
	file_content += "#include \"make_" + name + ".h\"\n";
	file_content += "\n";

	for (auto cpp_info : m_cpp_info_vec)
	{
		file_content += cpp_info->msg_c_cpp + ";\n\n";
	}

	this->saveFile(cpp_cpp_name, file_content);
}

void ProtocolMaker::saveFile(const string & file_name, const string & file_content)
{
	string full_file_name = m_output_path + "\\" + file_name;
	std::fstream f(full_file_name, std::ios_base::out | std::ios_base::trunc);

	f.write(file_content.c_str(), file_content.size());

	f.close();
}

