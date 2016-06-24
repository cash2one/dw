#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "typedef.h"
#include <boost/thread.hpp>
#include "logger_def.h"
#include "Report.h"

#define LOG_DEBUG(logger, log_info, ...)		logger->log(LL_DEBUG, log_info, ##__VA_ARGS__);
#define LOG_INFO(logger, log_info, ...)			logger->log(LL_INFO, log_info, ##__VA_ARGS__);
#define LOG_WARNING(logger, log_info, ...)		logger->log(LL_WARNING, log_info, ##__VA_ARGS__);
#define LOG_TRACE(logger, log_info, ...)		logger->log(LL_TRACE, log_info, ##__VA_ARGS__);
#define LOG_ERROR(logger, log_info, ...)		logger->log(LL_ERROR, log_info, ##__VA_ARGS__);
#define LOG_FATAL(logger, log_info, ...)		logger->log(LL_FATAL, log_info, ##__VA_ARGS__);

//#define DEF_LOG_DEBUG(log_info, ...)			g_logger_instance->log(LL_DEBUG, log_info, ##__VA_ARGS__);
//#define DEF_LOG_INFO(log_info, ...)				g_logger_instance->log(LL_INFO, log_info, ##__VA_ARGS__);
//#define DEF_LOG_WARNING(log_info, ...)			g_logger_instance->log(LL_WARNING, log_info, ##__VA_ARGS__);
//#define DEF_LOG_TRACE(log_info, ...)			g_logger_instance->log(LL_TRACE, log_info, ##__VA_ARGS__);
//#define DEF_LOG_ERROR(log_info, ...)			g_logger_instance->log(LL_ERROR, log_info, ##__VA_ARGS__);
//#define DEF_LOG_FATAL(log_info, ...)			g_logger_instance->log(LL_FATAL, log_info, ##__VA_ARGS__);

#define DEF_LOG_DEBUG(log_info, ...)		g_logger_instance->log(LL_DEBUG, log_info " -- file <%s : %d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
#define DEF_LOG_INFO(log_info, ...)			g_logger_instance->log(LL_INFO, log_info " -- file <%s : %d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
#define DEF_LOG_WARNING(log_info, ...)		g_logger_instance->log(LL_WARNING, log_info " -- file <%s : %d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
#define DEF_LOG_TRACE(log_info, ...)		g_logger_instance->log(LL_TRACE, log_info " -- file <%s : %d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
#define DEF_LOG_ERROR(log_info, ...)		g_logger_instance->log(LL_ERROR, log_info " -- file <%s : %d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
#define DEF_LOG_FATAL(log_info, ...)		g_logger_instance->log(LL_FATAL, log_info " -- file <%s : %d> \n", ##__VA_ARGS__, __FILE__, __LINE__);

enum LOGGER_LEVEL
{
	LL_DEBUG	= 0x1,
	LL_INFO		= 0x2,
	LL_WARNING	= 0x4,
	LL_TRACE	= 0x8,
	LL_ERROR	= 0x10,
	LL_FATAL	= 0x20,
};

time_t LOGGER_EXOPRT getSystemMilliSeconds();

void LOGGER_EXOPRT formatTimeToString(time_t time_in_millsec, string & formated_time);

time_t LOGGER_EXOPRT formatStringToTime(const string & str_time);

class Logger;

LOGGER_EXOPRT extern Logger * g_logger_instance;

LOGGER_LEVEL extractLogLevel(const string & log_level_str);

class LOGGER_EXOPRT Logger
{
public:
	Logger();

	virtual ~Logger();

public:
	static void setFilter(uint32 filter_mask);

	static void setBufferLength(int buffer_length);

	static int getBufferLength();
public:
	void setSynchronization(bool sync);

	bool getSynchronization();

	void setName(const string & logger_name);

	const string & getName();

	//void setPrefix(const string & module_name, const string & class_name);

	void log(LOGGER_LEVEL log_level, const char * log_format, ...);

	void syncLog(char * log_buffer);

	virtual void doLogging(const char * log_buffer) = 0;

	virtual void flush();

protected:
	static uint32 m_filter_mask;

	static int m_buffer_length;

	bool m_synchronization;

	string m_logger_name;

	//string m_module_name;

	//string m_class_name;

	boost::mutex m_sync_logger_mutex;
};
#endif