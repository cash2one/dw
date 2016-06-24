
#ifndef Protocol_Maker_hpp__
#define Protocol_Maker_hpp__

#include <vector>
#include <string>
#include <iostream>
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/idl.h"
#include "flatbuffers/util.h"

using namespace std;
using namespace flatbuffers;

class ProtocolMaker
{
public:
	ProtocolMaker();
	~ProtocolMaker();

	struct CppInfo
	{
		CppInfo()
		{}

		string msg_name;
		string msg_c_h;
		string msg_c_cpp;
	};
	typedef vector<CppInfo *> CppInfoVec_t;
public:
	void init(const string & source_path, const string & output_path);
protected:
	void getFileList(const string & file_path, std::vector<string> & file_list);
	void makeProtocolCpp(const string & protocol_file_name);
	bool canMakeProtocolCpp(StructDef & struct_def);
	string makeProtocolH(StructDef & struct_def);
	string makeProtocolCpp(StructDef & struct_def);

	string getCppType(FieldDef * field_def);

	string getCppType(BaseType base_type);

	void saveProtocolCpp(const string & file_name);

	void saveProtocolCppH(const string & name);

	void saveProtocolCppCpp(const string & name);

	void saveFile(const string & file_name, const string & file_content);
private:
	string m_source_path;
	string m_output_path;

	CppInfoVec_t m_cpp_info_vec;
};
#endif