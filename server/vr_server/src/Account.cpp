
#include "Account.h"
#include "Summoner.h"

Account::Account()
	: m_summoner(NULL)
	, client_id(0)
	, account_id(0)
	, session_id(0)
	, language_type(0)
{
}

Summoner * Account::getSummoner()
{
	return m_summoner;
}

void Account::setSummoner(Summoner *pSum)
{
	m_summoner = pSum;
}

