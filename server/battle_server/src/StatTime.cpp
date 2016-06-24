
#include "StatTime.h"
#include "Logger.h"
#include "BattleLog.h"

StatTime::StatTime(const string & key_word, int check_time_value)
	: m_start_check_time(0)
	, m_check_time_value(check_time_value)
	, m_key_word(key_word)
{
	m_start_check_time = getSystemMilliSeconds();
}

StatTime::~StatTime()
{
	this->stat();
}

void StatTime::reset()
{
	m_start_check_time = getSystemMilliSeconds();
}

void StatTime::stat()
{
	int diff_time = int(getSystemMilliSeconds() - m_start_check_time);
	if (diff_time >= m_check_time_value)
	{
		BATTLE_LOG_WARNING("stat time <%s>, diff time is <%d>, check time is <%d>", m_key_word.c_str(), diff_time, m_check_time_value);
	}
}
