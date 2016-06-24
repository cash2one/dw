#ifndef _BASE_FUNC_H_
#define _BASE_FUNC_H_

#include <string.h>
#include <memory>
#include <string>
#include <map>
#include <vector>

#define UTF8_FILTER

namespace BaseFunc
{

// 从自由1导入
class StringHandleImpFromZYZZ
{
public:
	StringHandleImpFromZYZZ();
	~StringHandleImpFromZYZZ();
	// 替换字串
	static bool StrReplace(char* szSource, int nLen, const char* szOldstring, const char* szNewstring);
	static void Trim(char* szBuf);
private:

};


class IllegalWordCheck
{
public:
	IllegalWordCheck();
	~IllegalWordCheck();

	enum
	{
		PACKET_LEN					= 120,
	};

	//-------------------------------- 脏字符串处理 -----------------------------------
	//初始化脏字
	static bool InitMark();
	static bool ReadFileSimple(std::vector<std::string>& vecString, const std::string& strFileName);
	static int _3BytesUtf8ToUnicode(unsigned char* p);
	static int CopySkipSpecialChar(const char* data, size_t nLen, char* output);
	static bool IsValidUtf8Encode(const char* data, size_t nLen);
	static bool	IsLegalString(const std::string & strInfo);

	static std::vector<std::string>	m_vecStringForbidden;

#ifdef UTF8_FILTER
	typedef struct _testNode
	{
		bool flag;
		int child[256];
		void testinit() 
		{ 
			memset(child, -1, sizeof(child)); 
			flag = false;
		};
	}testNode;

	static std::vector<testNode> vecIllegalWord;
	static int topVecIllegalWord;
	
	static void TestInsert(std::vector<testNode> &test, const char * s, int &top);
	static int TestFind(const std::vector<testNode> &test, const char * s);
	static void TestCheck(const std::vector<testNode> &test, char * s);
	static bool TestIsNotAllowed(const std::vector<testNode> &test, const char * s);

#endif

	static bool m_bIsEnglish;
private:
	//	供字符串vector按照长度排序用的函数对象
	struct longer
	{
		bool operator()(const std::string& _Left, const std::string& _Right) const
		{
			return _Left.size() > _Right.size();
		}
	};
};
}

#endif