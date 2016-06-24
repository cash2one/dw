
#ifndef BATTLE_LOG_HPP__
#define BATTLE_LOG_HPP__

#include "Logger.h"

#define ENABLE_LOG 1

#ifdef ENABLE_LOG
#ifdef WIN32
	#define BATTLE_LOG_DEBUG(log_info, ...)		g_logger_instance->log(LL_DEBUG, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
	#define BATTLE_LOG_INFO(log_info, ...)		g_logger_instance->log(LL_INFO, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
	#define BATTLE_LOG_WARNING(log_info, ...)	g_logger_instance->log(LL_WARNING, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
	#define BATTLE_LOG_TRACE(log_info, ...)		g_logger_instance->log(LL_TRACE, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
	#define BATTLE_LOG_ERROR(log_info, ...)		g_logger_instance->log(LL_ERROR, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
	#define BATTLE_LOG_FATAL(log_info, ...)		g_logger_instance->log(LL_FATAL, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
#else
	#define BATTLE_LOG_DEBUG(log_info, ...)		g_logger_instance->log(LL_DEBUG, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
	#define BATTLE_LOG_INFO(log_info, ...)		g_logger_instance->log(LL_INFO, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
	#define BATTLE_LOG_WARNING(log_info, ...)	g_logger_instance->log(LL_WARNING, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
	#define BATTLE_LOG_TRACE(log_info, ...)		g_logger_instance->log(LL_TRACE, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
	#define BATTLE_LOG_ERROR(log_info, ...)		g_logger_instance->log(LL_ERROR, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
	#define BATTLE_LOG_FATAL(log_info, ...)		g_logger_instance->log(LL_FATAL, log_info " -- file <%s>, line <%d> \n", ##__VA_ARGS__, __FILE__, __LINE__);
#endif
#else
	#define BATTLE_LOG_DEBUG(log_info, ...)		
	#define BATTLE_LOG_INFO(log_info, ...)		
	#define BATTLE_LOG_WARNING(log_info, ...)	
	#define BATTLE_LOG_TRACE(log_info, ...)		
	#define BATTLE_LOG_ERROR(log_info, ...)		
	#define BATTLE_LOG_FATAL(log_info, ...)		
#endif // 1

#endif