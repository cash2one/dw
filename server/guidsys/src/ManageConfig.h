
#ifndef MANAGE_CONFIG_HPP
#define MANAGE_CONFIG_HPP

#include "SimpleXML.h"

struct GuidCfg
{
	GuidCfg()
		: seed_value(0)
		, index(0)
		, port(0)
	{}

	uint32 seed_value;
	uint64 index;
	string addr;
	int port;
};

class ManageConfig
{
public:
	ManageConfig();

	~ManageConfig();
public:
	static ManageConfig * instance()
	{
		if (NULL == m_instance)
		{
			m_instance = new ManageConfig();
		}

		return m_instance;
	}
public:
	virtual int init();

	virtual int stop();

	virtual int finit();
public:
	const GuidCfg & getGuidCfg();

	bool writeGuidCfg(uint32 seed_value, uint64 index);
protected:
	bool loadConfig(const string & cfg_path);

private:
	Document * m_config_doc;

	Attribute * m_seed_attr;

	Attribute * m_index_attr;

	GuidCfg m_guid_cfg;

	static ManageConfig * m_instance;
};
#endif