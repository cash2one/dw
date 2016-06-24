
#ifndef SERVE_INFO_HPP__
#define SERVE_INFO_HPP__

#include <unordered_map>
#include <string>
#include <SessionServer.h>

using namespace std;

struct ServerInfo;
struct LanguageInfo;

typedef unordered_map<int, ServerInfo *> ServerInfoMap_t;
typedef unordered_map<int, LanguageInfo *> LanguageInfoMap_t;

enum ServerStatus
{
	SS_BASE			= 0,
	SS_OFFLINE,
	SS_ONLINE,
};

struct ServerVrInfo 
{
	ServerVrInfo()
		: gs_id(0)
		, gate_port(0)
		, session_id(0)
	{}

	int gs_id;
	int gate_port;
	SessionId session_id;
	string gate_ip_addr;
};

typedef unordered_map<int, ServerVrInfo *> ServerVrInfoMap_t;

struct ServerInfo
{
	ServerInfo();
	~ServerInfo();

	void updateStatus(ServerStatus server_status);

	void updateGateInfo(const string & ip_addr, int port);

	void updatePlayerNumber(int add_player_num);

	int area_id;
	string area_name;
	ServerStatus status;
	int version_low;
	int version_high;
	int version_update_notify;
	time_t start_time;
	int player_num;
	string notice_stop;
	string notice_login;
	int is_show;
	string avatar_path;
	int resource_version;
	string resource_url;
	int area_index;
	string gate_ip;
	int gate_port;

	ServerVrInfoMap_t server_vr_map;
	int is_match_area;  // 有玩家能才进大区
};

struct LanguageInfo 
{
	LanguageInfo();
	~LanguageInfo();

	int area_id;
	int language_type;
	string area_name;
	string notice_login;
	string notice_stop;
};

#endif
