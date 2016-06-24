#pragma once
#include <time.h>
#include <string>

std::wstring Utf8ToUnicode(std::string& str);
std::string UnicodeToUtf8(std::wstring& str);


std::string  UnicodeToMbcs(std::wstring& str);
std::wstring MbcsToUnicode(std::string& str);



std::string MbcsToUtf8(std::string& str);
std::string Utf8ToMbcs(std::string& str);



void FormatTime(time_t& t, TCHAR *output);