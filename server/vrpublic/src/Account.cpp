
#include "Account.h"

Account::Account()
	: guid(0)
	, language_type(0)
	, summoner_id(0)
{
	client_id = 0;
}

AccountServerInfo::AccountServerInfo()
	: guid(0)
	, area_id(0)
	, gs_index(0)
	, first_enter_time(0)
	, last_enter_time(0)
{
}
