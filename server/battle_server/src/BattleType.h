
#ifndef BATTLE_TYPE_HPP__
#define BATTLE_TYPE_HPP__

#include "typedef.h"
#include "BattleLog.h"

#define CHECK_RETURN(check_value, ret)	\
	if (!(check_value))	\
	{  \
		BATTLE_LOG_ERROR("check value failed <%s>", #check_value);	\
		return ret;	\
	}

typedef uint64_t PlayerId_t;

#endif