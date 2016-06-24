#include "Summoner.h"
#include "Account.h"

Summoner::Summoner(uint64_t sum_id)
	: m_nId(sum_id)
	, m_account(NULL)
	, client_id(0)
	, session_id(NULL)
{
}

Summoner::Summoner()
	: m_nId(0)
	, m_account(NULL)
{
}

Summoner::~Summoner()
{
}


void Summoner::setAccount(Account * pAccount)
{
	m_account = pAccount;
}