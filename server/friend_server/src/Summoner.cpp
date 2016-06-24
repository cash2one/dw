#include "Summoner.h"

using namespace MobaGo::FlatBuffer;

Summoner::Summoner(uint64_t sum_id)
	: m_nId(sum_id)
	, sum_status(SummonerStatus_Offline)
{
}

Summoner::Summoner()
	: m_nId(0)
	, sum_status(SummonerStatus_Offline)
{
}

Summoner::~Summoner()
{
}

void Summoner::setStatus(MobaGo::FlatBuffer::SummonerStatus status)
{
	sum_status = status;
}