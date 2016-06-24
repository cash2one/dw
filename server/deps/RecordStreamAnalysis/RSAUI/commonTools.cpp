#include "StdAfx.h"
#include "commonTools.h"

std::wstring Utf8ToUnicode(std::string& str)
{	
	std::wstring return_buf;
	int size = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
	if( 0 == size)
		return return_buf;
	
	wchar_t*  pBuf = new wchar_t[size + 1];
	::wmemset(pBuf, '\0', size + 1);
	if(NULL == pBuf)
		return return_buf;

	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pBuf, size + 1);
	return_buf = pBuf;
	delete []pBuf;
	
	return return_buf;
}


std::string UnicodeToUtf8(std::wstring& str)
{
	std::string return_buf;
	int size = ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0, NULL, NULL);
	if( 0 == size)
		return return_buf;

	char*  pBuf = new char[size + 1];
	::memset(pBuf, '\0', size + 1);
	if(NULL == pBuf)
		return return_buf;

	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), pBuf, size + 1, NULL, NULL);
	return_buf = pBuf;
	delete []pBuf;

	return return_buf;
}

std::string  UnicodeToMbcs(std::wstring& str)
{
	std::string return_buf;
	int size = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.length(), NULL, 0, NULL, NULL);
	if( 0 == size)
		return return_buf;

	char*  pBuf = new char[size + 1];
	::memset(pBuf, 0, size + 1);
	if(NULL == pBuf)
		return return_buf;

	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.length(), pBuf, size + 1, NULL, NULL);
	return_buf = pBuf;
	delete []pBuf;

	return return_buf;
}


std::wstring MbcsToUnicode(std::string& str)
{
	std::wstring return_buf;
	int size = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), NULL, 0);
	if( 0 == size)
		return return_buf;

	wchar_t*  pBuf = new wchar_t[size + 1];
	::wmemset(pBuf, 0, size + 1);
	if(NULL == pBuf)
		return return_buf;


	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pBuf, size + 1);
	return_buf = pBuf;
	delete []pBuf;

	return return_buf;
}


std::string MbcsToUtf8(std::string& str)
{
	return UnicodeToUtf8(MbcsToUnicode(str));

}

std::string Utf8ToMbcs(std::string& str)
{
	return UnicodeToMbcs(Utf8ToUnicode(str));
}

void FormatTime(time_t& t, TCHAR *output)
{
    struct tm tm1;
    tm1 = *localtime(&t);
    _stprintf(output, _T("%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d"),
        tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, 
        tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
}