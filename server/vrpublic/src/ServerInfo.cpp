
#include "ServerInfo.h"

ServerInfo::ServerInfo()
	: area_id(0)
	, status(SS_OFFLINE)
	, version_low(0)
	, version_high(0)
	, start_time(0)
	, player_num(0)
	, is_show(0)
	, resource_version(0)
	, area_index(0)
	, gate_port(0)
{

}

ServerInfo::~ServerInfo()
{

}

void ServerInfo::updateStatus(ServerStatus server_status)
{
	this->status = server_status;
}

void ServerInfo::updateGateInfo(const string & ip_addr, int port)
{
	gate_ip = ip_addr;
	gate_port = port;
}

void ServerInfo::updatePlayerNumber(int add_player_num)
{
	this->player_num += add_player_num;
}

LanguageInfo::LanguageInfo()
	: area_id(0)
	, language_type(0)
{

}

LanguageInfo::~LanguageInfo()
{

}