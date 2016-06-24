#include "plugin.h"

Plugin::Plugin()
{
	
}

int Plugin::init(const PluginCfg plugin_cfg)
{
	m_plugin_cfg = plugin_cfg;
	input();
	request();
	notify();
	gm();
	return 0;
}

void PluginParam::append(const std::string param_name, const std::string param_value)
{
	auto p = pairs.insert(std::make_pair(param_name, param_value));
	if (!p.second)
	{
		DEF_LOG_ERROR("Failed to append param name and value, name is <%s>, value is <%s>\n", param_name.c_str(), param_value.c_str());
	}
}

bool PluginParam::getStr(const std::string param_name, std::string & param_value)
{
	auto it = pairs.find(param_name);
	if (it != pairs.end())
	{
		param_value = it->second;
		return true;
	}
	
	return false;
}

bool PluginParam::getInt32(const std::string param_name, int32 & param_value)
{
	string pv;
	if (getStr(param_name, pv))
	{
		try
		{
			param_value = boost::lexical_cast<int32>(pv);
			return true;
		}
		catch (...)
		{
		}
	}

	return false;
}

bool PluginParam::getUint64(const std::string param_name, uint64 & param_value)
{
	string pv;
	if (getStr(param_name, pv))
	{
		try
		{
			param_value = boost::lexical_cast<uint64>(pv);
			return true;
		}
		catch (...)
		{
		}
	}

	return false;
}