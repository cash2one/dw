#include "fbparse.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>

#include "flatbuffers/util.h"


using namespace flatbuffers;

int FBParser::init(const std::string proto_path)
{
	boost::filesystem::path fsPath(proto_path);
	boost::filesystem::directory_iterator beg_iter(fsPath);
	boost::filesystem::directory_iterator end_iter;

	try
	{
		for (; beg_iter != end_iter; ++beg_iter)
		{	
			string str_name = beg_iter->path().filename().generic_string();
			auto findIt = str_name.find(".fbs");
			if (findIt == std::string::npos)
				continue;

			std::string schemafile;
			std::string file_path(proto_path);
			file_path += "//";
			file_path += str_name;

			if (!flatbuffers::LoadFile(file_path.c_str(), false, &schemafile))
			{
				std::cout << "failed to load file: " << file_path << std::endl;
				return -1;
			}

			using namespace flatbuffers;
			const char *include_directories[] = { ".", nullptr };
			if (!m_parser.Parse(schemafile.c_str(), include_directories))
			{
				std::cout << "failed to parse fille: " << file_path << ", error: " << m_parser.error_ << "\n";
				return -1;
			}

		}
	}
	catch (const boost::filesystem::filesystem_error& e)
	{
		std::cout << "FBParser::init filesystem_error: " << e.what() << std::endl;
		return -1;
	}
	catch (const std::exception& e)
	{
		std::cout << "FBParser::init exception: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}

int FBParser::setRootType(const std::string type)
{
	if (!m_parser.SetRootType(type.c_str()))
	{
		std::cout << "failed to set root type: " << type <<" ,err: " << m_parser.error_ << "\n";
		return -1;
	}
	return 0;
}

int FBParser::parse(const std::string& a_bin, std::string& json)
{	
	GenerateText(m_parser, a_bin.c_str(), &json);
	return 0;
}

int FBParser::setField(const std::string name, const std::string v)
{	
	auto root = m_parser.root_struct_def_;
	if (NULL == root)
	{
		std::cout << "NOT set root type" << std::endl;
		return -1;
	}
	
	bool existed = false;
	for (FieldDef* x : root->fields.vec)
	{	
		if (x->name != name)
			continue;

		existed = true;
		break;
	}

	if (!existed)
	{
		std::cout << "field NOT exist, name: " << name << " root type: " << root->name << std::endl;
		return -1;
	}

	for (Fieldst& f : m_buffer)
	{
		if (f.name == name)
		{
			f.v = v;
			return 0;
		}
	}

	Fieldst f;
	f.name = name;
	f.v = v;
	m_buffer.push_back(f);

	return 0;
}

int FBParser::finish(std::string& a_out)
{	
	std::stringstream ss;
	ss << "{";
	
	size_t index = 0;
	for (auto& e : m_buffer)
	{	
		index++;
		ss << e.name << ":" << e.v;
		
		if (index != m_buffer.size())
			ss << ",";
	}

	ss << "}";

	const char *include_directories[] = { ".", nullptr };
	if (!m_parser.Parse(ss.str().c_str(), include_directories))
	{
		std::cout << "failed to parse : " << ss.str() << " err: " << m_parser.error_ << "\n";
		return -1;
	}

	auto bufferpointer =
		reinterpret_cast<const char *>(m_parser.builder_.GetBufferPointer());
	a_out.assign(bufferpointer, bufferpointer + m_parser.builder_.GetSize());
	return 0;
}

const char *const kTypeNames2[] = {
#define FLATBUFFERS_TD(ENUM, IDLTYPE, CTYPE, JTYPE, GTYPE, NTYPE, PTYPE) \
    IDLTYPE,
	FLATBUFFERS_GEN_TYPES(FLATBUFFERS_TD)
#undef FLATBUFFERS_TD
	nullptr
};

int FBParser::getFields(const std::string type, std::vector<Fieldst>& fields)
{	
	StructDef* vec = m_parser.structs_.Lookup(type);
	if (NULL == vec)
	{
		std::cout << "NOT get the type: " << type << std::endl;
		return -1;
	}

	for (FieldDef* x : vec->fields.vec)
	{	
		Fieldst st;
		st.name = x->name;
		st.type = kTypeNames2[x->value.type.base_type];
		fields.push_back(st);
	}
	
	return 0;
}