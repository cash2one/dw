#pragma  once
#include "plugin.h"

class Pool;
class Logger;
struct Sender;
struct PluginDepotCfg
{
	Pool* pool = NULL;
	Logger* logger = NULL;
	Sender *sender = NULL;
	std::string plugin_param_path = "plugin.xml";
};

struct   PluginDepot
{
	int init(const PluginDepotCfg & plugin_depot_cfg);

	int input(int msg_id, InterPacket* packet, char * buffer, int buffer_size);
	int notify(PackInfo & pack_info);
	int request(PackInfo & pack_info);
	int gmcmd(const string & gm_name, const vector<string> & gm_param, uint64 target_guid);

	bool isStartupSuccess();
	bool isShutdownSuccess();

	int addPlugin(Plugin * p);
	PluginParam* getPluginParam(const std::string plugin_name);

private:
	bool loadPluginParam(const std::string& path);
protected:
	PluginDepotCfg	m_plugin_depot_cfg;
	std::map<std::string, Plugin *>	m_plugin_map;
	std::map<std::string, PluginParam*> m_plugin_param;
};