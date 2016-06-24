
#include "BaseFunc.h"
#include <algorithm>
#include <fstream>

using namespace std;
using namespace BaseFunc;

bool IllegalWordCheck::m_bIsEnglish = false;

#ifdef UTF8_FILTER
vector<IllegalWordCheck::testNode> IllegalWordCheck::vecIllegalWord;
int IllegalWordCheck::topVecIllegalWord;;
#endif

std::vector<std::string>	IllegalWordCheck::m_vecStringForbidden;


StringHandleImpFromZYZZ::StringHandleImpFromZYZZ()
{
}

StringHandleImpFromZYZZ::~StringHandleImpFromZYZZ()
{
}

// 替换字串
bool StringHandleImpFromZYZZ::StrReplace(char* szSource, int nLen, const char* szOldstring, const char* szNewstring)
{
	return false;
}

void StringHandleImpFromZYZZ::Trim(char* szBuf)
{

}

IllegalWordCheck::IllegalWordCheck()
{
}

IllegalWordCheck::~IllegalWordCheck()
{
}

//-------------------------------- 脏字符串处理 -----------------------------------
//初始化脏字
bool IllegalWordCheck::InitMark()
{
	// 读配置表来判断是否英文
	bool bIsEnglish = false;

	m_vecStringForbidden.clear();

	if (!ReadFileSimple(m_vecStringForbidden, "markWord.txt")) // to do list
	{
		return false;
	}

	/*
	if (m_bIsEnglish)
	{
		for (DWORD dw = 0; dw < m_vecStringForbidden.size(); dw++)
		{
			std::transform(m_vecStringForbidden[dw].begin(), m_vecStringForbidden[dw].end(), m_vecStringForbidden[dw].begin(), ::tolower);
		}
	}
	*/

	//服务器的一些禁止符号
	vector<string> vecForbiddenSign;
	if (!ReadFileSimple(vecForbiddenSign, "forbiddenSign.txt"))
	{
		return false;
	}

	m_vecStringForbidden.insert(m_vecStringForbidden.end(), vecForbiddenSign.begin(), vecForbiddenSign.end());

	//	删除列表内重复的条目
	if (m_vecStringForbidden.begin() == m_vecStringForbidden.end())
	{
		return false;
	}

	for (vector<string>::iterator iter = m_vecStringForbidden.begin(); iter != m_vecStringForbidden.end() - 1; ++iter)
	{
		for (vector<string>::iterator iter2 = iter + 1; iter2 != m_vecStringForbidden.end();)
		{
			if (*iter2 == *iter)
			{
				iter2 = m_vecStringForbidden.erase(iter2);
			}
			else
			{
				++iter2;
			}
		}
	}

	string strSpace;
	strSpace = ' ';
	m_vecStringForbidden.push_back(strSpace);
	strSpace = "　";
	m_vecStringForbidden.push_back(strSpace);

	m_vecStringForbidden.push_back("\x0d");
	m_vecStringForbidden.push_back("\x0a");
	m_vecStringForbidden.push_back("\t");

	//按照长度排序，长的在前
	sort(m_vecStringForbidden.begin(), m_vecStringForbidden.end(), longer());

#ifdef UTF8_FILTER
	testNode tmp;
	tmp.testinit();
	vecIllegalWord.clear();
	vecIllegalWord.push_back(tmp);
	topVecIllegalWord = 1;
	for (vector<string>::iterator iter = m_vecStringForbidden.begin(); iter != m_vecStringForbidden.end(); ++iter)
	{
		TestInsert(vecIllegalWord, iter->c_str(), topVecIllegalWord);
	}
	
#endif

	return false;
}

bool IllegalWordCheck::ReadFileSimple(vector<string>& vecString, const string& strFileName)
{
	std::ifstream fin(strFileName);
	if (!fin)
	{
		return false;
	}

	vecString.clear();
	char szBuf[2048];
	while (fin.getline(szBuf, sizeof(szBuf)))
	{
		//StringHandleImpFromZYZZ::Trim(szBuf);
		string temp(szBuf);
		if (!temp.empty())
		{
			vecString.push_back(temp);
		}

	}
	fin.close();

	return true;
}

static bool strheader(register const char* str, register const char* header)
{
	while (*header != '\0')
	{
		if (*str != *header)
			return false;
		++str;
		++header;
	}
	return true;
}

//判断有没有脏字filter
static bool HaveMarkCStr(const char * str, int nLen, const char * filter)
{
	if (!filter)
		return false;

	const char *p = str;
	for (int i = 0; i < nLen;)
	{
		if (strheader(p, filter))
		{
			return true;
		}
		else
		{
			if ((unsigned char)*p >= 0x80)
			{
				p += 2;
				i += 2;
			}
			else
			{
				++p;
				++i;
			}
		}
	}
	return false;
}

int IllegalWordCheck::_3BytesUtf8ToUnicode(unsigned char* p)
{

	int c1 = (int)(*p);
	int c2 = (int)(*(p + 1));
	int c3 = (int)(*(p + 2));

	int c = ((c1 & 0x0f) << 12) | ((c2 & 63) << 6) | (c3 & 63);	//1110 xxxx	10xx xxxx		10xx xxxx

	return c;
}

int IllegalWordCheck::CopySkipSpecialChar(const char* data, size_t nLen, char* output)
{
	unsigned char* p = (unsigned char*)data;
	unsigned char* q = (unsigned char*)output;
	while (true)
	{
		if ((*p >> 3) == 30)		//1111 0000
		{
			if (nLen < 4)
				break;

			nLen -= 4;

			//不是汉字，不处理
			p += 4;

			continue;
		}

		if ((*p >> 4) == 14)		//1110 0000
		{
			if (nLen < 3)
				break;

			nLen -= 3;

			int utf8Int = 0;
			memcpy(((unsigned char*)&utf8Int) + 1, p, 3);

			int c = _3BytesUtf8ToUnicode(p);
			if (c >= 0x4e00 && c <= 0x9fa5)
			{
				memcpy(q, p, 3);
				q += 3;
			}

			p += 3;
			continue;
		}

		if ((*p >> 5) == 6)		//1100 0000
		{
			if (nLen < 2)
				break;

			nLen -= 2;

			//不是汉字，不处理	
			p += 2;
			continue;
		}

		if (nLen < 1)
			break;

		//1000 0000		output从utf8 字符中间传过来，不处理
		//0000 0000		ascii
		//不是汉字，不处理	

		nLen -= 1;
		p++;

	}

	return (int)(q - (unsigned char*)output);
}

#define IS_START_WITH_10(c)	( ( (c)  >> 6 ) ==  2 )
bool IllegalWordCheck::IsValidUtf8Encode(const char* data, size_t nLen)
{
	unsigned char* p = (unsigned char*)data;

	while (nLen)
	{
		if ((p[0] >> 3) == 30)		//1111 0000
		{
			if (nLen < 4)
				return false;

			if (!IS_START_WITH_10(p[1]) || !IS_START_WITH_10(p[2]) || !IS_START_WITH_10(p[3]))
				return false;

			nLen -= 4;
			p += 4;
			continue;
		}

		if ((p[0] >> 4) == 14)		//1110 0000
		{
			if (nLen < 3)
				return false;

			if (!IS_START_WITH_10(p[1]) || !IS_START_WITH_10(p[2]))
				return false;

			nLen -= 3;
			p += 3;
			continue;
		}

		if ((p[0] >> 5) == 6)		//1100 0000
		{
			if (nLen < 2)
				return false;

			if (!IS_START_WITH_10(p[1]))
				return false;

			nLen -= 2;
			p += 2;
			continue;
		}

		if (p[0] < 128)
		{
			if (nLen < 1)
				return false;

			nLen -= 1;
			p++;
			continue;
		}

		return false;

	}

	return true;
}

bool IllegalWordCheck::IsLegalString(const string& strInfo)
{
	if (strInfo.empty())
		return false;

#ifdef UTF8_FILTER

	if (!IsValidUtf8Encode(strInfo.data(), strInfo.size()))
		return false;

	//assert( strInfo.size() < 512 );

	if (!m_bIsEnglish)
	{
		bool leagal = !TestIsNotAllowed(vecIllegalWord, strInfo.c_str());
		if (!leagal)
			return false;

		//如果合法，去除标点，再检查一次
		char output[512] = { 0 };
		int dstLen = CopySkipSpecialChar(strInfo.data(), strInfo.size(), output);
		output[dstLen] = 0;

		return !TestIsNotAllowed(vecIllegalWord, output);
	}
	else
	{
		string strInfoCopy = strInfo;
		std::transform(strInfoCopy.begin(), strInfoCopy.end(), strInfoCopy.begin(), ::tolower);

		bool leagal = !TestIsNotAllowed(vecIllegalWord, strInfoCopy.c_str());
		if (!leagal)
			return false;

		//如果合法，去除标点，再检查一次
		char output[512] = { 0 };
		int dstLen = CopySkipSpecialChar(strInfoCopy.data(), strInfoCopy.size(), output);
		output[dstLen] = 0;

		return !TestIsNotAllowed(vecIllegalWord, output);
	}
	//	return !TestIsNotAllowed(test,strInfo.c_str());

#else
	for (vector<string>::iterator iter = m_vecStringForbidden.begin(); iter != m_vecStringForbidden.end(); ++iter)
	{
		if (HaveMarkCStr(strInfo.data(), strInfo.size(), iter->c_str()))
		{
			return false;
		}
	}
	return true;
#endif
}


#ifdef UTF8_FILTER
void IllegalWordCheck::TestInsert(vector<testNode> &test, const char * s, int &top)
{
	int pnow = 0, i, id;
	testNode tmp;
	for (i = 0; s[i]; ++i)
	{
		id = (unsigned char)s[i];
		if (test[pnow].child[id] == -1)
		{
			test[pnow].child[id] = top;
			tmp.testinit();
			test.push_back(tmp);
			top++;
		}
		pnow = test[pnow].child[id];
	}
	test[pnow].flag = true;
}

int IllegalWordCheck::TestFind(const vector<testNode> &test, const char * s)
{
	int i, id, pnow = 0;
	int ans = -1;
	for (i = 0; s[i]; ++i)
	{
		id = (unsigned char)s[i];
		if (test[pnow].child[id] == -1) return ans + 1;
		pnow = test[pnow].child[id];
		if (test[pnow].flag) ans = i;
	}

	return ans + 1;
}

void IllegalWordCheck::TestCheck(const vector<testNode> &test, char * s)
{
	char *p = s;
	while (*p)
	{
		int ret = TestFind(test, p);
		if (ret == 0)
		{
			if ((unsigned char)*p < 0x80)
			{
				p++;
			}
			else if ((unsigned char)*p < 0xE0)
			{
				p += 2;
			}
			else if ((unsigned char)*p < 0xF0)
			{
				p += 3;
			}
			else
			{
				p += 4;
			}
		}
		else
		{
			//脏字替换为同数量的* 
			int nNum = 0;
			char* y = new char[PACKET_LEN];
			memset(y, 0, PACKET_LEN);
			memcpy(y, p, ret);
			for (int i = 0; i < ret;)
			{
				if ((unsigned char)*p < 0x80)
				{
					p++;
					i++;
					nNum++;
				}
				else if ((unsigned char)*p < 0xE0)
				{
					p += 2;
					i += 2;
					nNum++;
				}
				else if ((unsigned char)*p < 0xF0)
				{
					p += 3;
					i += 3;
					nNum++;
				}
				else
				{
					p += 4;
					i += 4;
					nNum++;
				}
			}

			string strReplace;
			for (int i = 0; i < nNum; ++i)
			{
				strReplace += "*";
			}
			StringHandleImpFromZYZZ::StrReplace(s, PACKET_LEN - 1, y, strReplace.data());
			delete[] y;
		}
	}
}
bool IllegalWordCheck::TestIsNotAllowed(const vector<testNode> &test, const char * s)
{
	if (test.empty())
	{
		return false;
	}
	const char *p = s;
	while (*p)
	{
		int ret = TestFind(test, p);
		if (ret == 0)
		{
			if ((unsigned char)*p < 0x80)
			{
				p++;
			}
			else if ((unsigned char)*p < 0xE0)
			{
				p += 2;
			}
			else if ((unsigned char)*p < 0xF0)
			{
				p += 3;
			}
			else
			{
				p += 4;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}

#endif
