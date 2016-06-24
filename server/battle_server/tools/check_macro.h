
#ifndef CHECK_MACRO_HPP__
#define CHECK_MACRO_HPP__

#include "Logger.h"

#define CK_EQUAL_RETURN(first_value, second_value, ret)	\
	if ((first_value) == (second_value))	\
	{	\
		DEF_LOG_ERROR("failed to check the value <%s>, <%s>", #first_value, #second_value);	\
		return ret;	\
	}

#define CK_NOT_EQUAL_RETURN(first_value, second_value, ret)	\
	if ((first_value) != (second_value))	\
			{	\
		DEF_LOG_ERROR("failed to check the value <%s>, <%s>", #first_value, #second_value);	\
		return ret;	\
			}

#define CK_NULL_RETURN(check_value, ret)	\
	if (NULL == (check_value))	\
	{	\
		DEF_LOG_ERROR("check failed the value (NULL) <%s>", #check_value);	\
		return ret;	\
	}

#define CK_NULL_CMD(check_value, ret)	\
	if (NULL == (check_value))	\
			{	\
		DEF_LOG_ERROR("check failed the value (NULL) <%s>", #check_value);	\
		ret;	\
			}

#define CK_FALSE_RETURN(check_value, ret)	\
	if (!(check_value))	\
	{	\
		DEF_LOG_ERROR("check failed the value (false) <%s>", #check_value);	\
		return ret;	\
	}

#define CK_TRUE_RETURN(check_value, ret)	\
	if (check_value)	\
			{	\
		DEF_LOG_ERROR("check failed the value (true) <%s>", #check_value);	\
		return ret;	\
			}

#define CK_TRUE_PRINT(check_value)	\
	if (check_value)	\
					{	\
		DEF_LOG_ERROR("error occcur while check the value (true) <%s>", #check_value);	\
					}

#define CHECK_INIT_VALUE(ret)	\
	if ((ret) == -1)	\
					{	\
		DEF_LOG_ERROR("failed to init <%s>", #ret);	\
		return 1;	\
					}

#define CK_TRUE_CMD(check_value, cmd)	\
		if (check_value)	\
		{	\
			DEF_LOG_ERROR("check failed the value (true) <%s>", #check_value);	\
			cmd;	\
		}



#endif