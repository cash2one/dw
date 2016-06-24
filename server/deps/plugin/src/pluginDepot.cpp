#include "pluginDepot.h"
#include "simplexml.h"

int PluginDepot::init(const PluginDepotCfg & plugin_depot_cfg)
{
	m_plugin_depot_cfg = plugin_depot_cfg;

	if (!loadPluginParam(m_plugin_depot_cfg.plugin_param_path))
	{
		DEF_LOG_ERROR("failed to load plugin param:%s", m_plugin_depot_cfg.plugin_param_path.c_str());
		return -1;
	}

	PluginCfg cfg;
	cfg.depot = this;
	cfg.logger = m_plugin_depot_cfg.logger;
	cfg.pool = m_plugin_depot_cfg.pool;
	cfg.sender = m_plugin_depot_cfg.sender;
	for (auto& it : m_plugin_map)
	{
		Plugin* p = it.second;
		cfg.params = m_plugin_param[p->name()];
		if (NULL == cfg.params)
		{	
			// 没有则默认提供一个空的
			cfg.params = new PluginParam;
		}

		if (0 != p->init(cfg))
		{
			DEF_LOG_ERROR("failed to init plugin :%s", p->name().c_str());
			return -1;
		}
	}

	return 0;
}

int PluginDepot::input(int msg_id, InterPacket* packet, char * buffer, int buffer_size)
{	
	for (auto& it : m_plugin_map)
	{
		Plugin* p = it.second;

		auto findIt = p->m_input_msg_type_map.find(msg_id);
		if ( findIt == p->m_input_msg_type_map.end())
			continue;

		PacketHandleBase<InterPacket> * pack_handle = findIt->second;
		pack_handle->handlePacketMsg(packet, buffer, buffer_size);

		return 0;
	}

	return -1;
}

int PluginDepot::notify(PackInfo & pack_info)
{	
	int res = -1;
	for (auto& it : m_plugin_map)
	{
		Plugin* p = it.second;

		auto findIt = p->m_notify_msg_type_map.find(pack_info.opcode);
		if (findIt == p->m_notify_msg_type_map.end())
			continue;

		const vector<MsgHandler>& handlers = findIt->second;
		for (auto& handler : handlers)
		{
			handler(pack_info);
		}
		
		res = 0;
	}

	return res;
}

int PluginDepot::request(PackInfo & pack_info)
{
	for (auto& it : m_plugin_map)
	{
		Plugin* p = it.second;

		auto findIt = p->m_request_msg_type_map.find(pack_info.opcode);
		if (findIt == p->m_request_msg_type_map.end())
			continue;

		auto& handler = findIt->second;
		handler(pack_info);

		return 0;
	}

	return -1;
}

int PluginDepot::gmcmd(const string & gm_name, const vector<string> & gm_param, uint64 target_guid)
{
	for (auto& it : m_plugin_map)
	{
		Plugin* p = it.second;

		auto findIt = p->m_gmcmd_msg_type_map.find(gm_name);
		if (findIt == p->m_gmcmd_msg_type_map.end())
			continue;

		auto& handler = findIt->second;
		handler(gm_param, target_guid);

		return 0;
	}

	return -1;
}


bool PluginDepot::isStartupSuccess()
{	
	bool success = true;
	for (auto& it : m_plugin_map)
	{
		Plugin* p = it.second;
		if ( !p->isStartupSuccess())
		{
			DEF_LOG_INFO("waiting for plugin %s startup:%s", p->name().c_str());
			success = false;
			break;
		}
	}

	return success;
}

bool PluginDepot::isShutdownSuccess()
{
	return 0;
}

PluginParam* PluginDepot::getPluginParam(const std::string plugin_name)
{
	auto it = m_plugin_param.find(plugin_name);
	if (it != m_plugin_param.end())
		return it->second;

	return NULL;
}

int PluginDepot::addPlugin(Plugin * p)
{
	auto it = m_plugin_map.find(p->name());
	if (it != m_plugin_map.end())
	{
		DEF_LOG_ERROR("reduplicate plugin, name:%s", p->name().c_str());
		return -1;
	}

	m_plugin_map.insert(std::make_pair(p->name(), p));
	return 0;
}

bool PluginDepot::loadPluginParam(const std::string& path)
{
	std::unique_ptr<Document> xml_doc(XMLFactory::create_document());
	try
	{	
		auto path = m_plugin_depot_cfg.plugin_param_path;
		if (!xml_doc->load_file(path))
		{
			DEF_LOG_ERROR("failed to load config by xml document : <%s>\n", path.c_str());
			return false;
		}

		Element * root = xml_doc->get_root();
		Elements plugins = root->get_elements();
		for (Elements::iterator it = plugins.begin(); it != plugins.end(); ++it)
		{	

			PluginParam *plugin_param = new PluginParam;
			Elements params = (*it)->get_elements();
			Attribute * plugin_name = (*it)->get_attribute("name");
			if (NULL == plugin_name)
			{
				DEF_LOG_ERROR("failed to get plugin attribute name");
				return false;
			}

			for (Elements::iterator inner = params.begin(); inner != params.end(); ++inner)
			{ 
				Attribute * attr_name = (*inner)->get_attribute("name");
				if (NULL == attr_name)
				{
					DEF_LOG_ERROR("failed to get name attribute");
					return false;
				}

				Attribute * attr_value = (*inner)->get_attribute("value");
				if (NULL == attr_value)
				{
					DEF_LOG_ERROR("failed to get value attribute");
					return false;
				}

				plugin_param->append(attr_name->get_value(), attr_value->get_value());
			}
			
			m_plugin_param.emplace(plugin_name->get_value(), plugin_param);
		}

		return true;
	}
	catch (...)
	{
		DEF_LOG_ERROR("unknown exception");
	}

	return false;
}

