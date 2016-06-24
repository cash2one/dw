
#ifndef STAT_TIME_HPP__
#define STAT_TIME_HPP__

#include <string>
#include "typedef.h"

using namespace std;

class StatTime
{
public:
	StatTime(const string & key_word, int check_time_value);
	~StatTime();
	void reset();
	void stat();
protected:
private:
	time_t m_start_check_time;
	int m_check_time_value;
	string m_key_word;
};

#endif
