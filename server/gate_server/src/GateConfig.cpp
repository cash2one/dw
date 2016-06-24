
#include "GateConfig.h"
#include "Logger.h"

#define CHECK_NULL_PTR_RETURN(check_ptr, ret)	\
	if (NULL == check_ptr)	\
			{	\
		DEF_LOG_ERROR(#check_ptr);	\
		return ret;	\
			}

GateConfig * GateConfig::m_instance = NULL;

GateConfig::GateConfig()
{

}

GateConfig::~GateConfig()
{

}

GateConfig * GateConfig::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new GateConfig();
	}

	return m_instance;
}

int GateConfig::init()
{
	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (!doc->load_file("gate_config.xml"))
	{
		DEF_LOG_ERROR("failed to load gate_config.xml");
		return -1;
	}

	bool load_ret = true;
	load_ret = this->loadClientInfo(doc.get()) && load_ret;
	load_ret = this->loadServerInfo(doc.get()) && load_ret;

	return load_ret ? 0 : -1;
}

ClientCfg & GateConfig::getClientCfg()
{
	return m_client_cfg;
}

ServerCfg & GateConfig::getServerCfg()
{
	return m_server_cfg;
}

bool GateConfig::loadClientInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("client");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);

	m_client_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);

	m_client_cfg.port = attr->as_int();

	attr = ele->get_attribute("net_type");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_client_cfg.net_type = attr->as_int();

	attr = ele->get_attribute("ping_interval");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_client_cfg.ping_interval = attr->as_int();

	return true;

}

bool GateConfig::loadServerInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("server");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);

	m_server_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);

	m_server_cfg.port = attr->as_int();

	return true;

}
