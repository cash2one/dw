
#include <memory>
#include "Logger.h"
#include "VrPublicConfig.h"

#define CHECK_NULL_PTR_RETURN(check_ptr, ret)	\
	if (NULL == check_ptr)	\
	{	\
		DEF_LOG_ERROR(#check_ptr);	\
		return ret;	\
	}

VrPublicConfig * VrPublicConfig::m_instance = nullptr;

VrPublicConfig::VrPublicConfig()
{

}

VrPublicConfig::~VrPublicConfig()
{

}

VrPublicConfig * VrPublicConfig::instance()
{
	if (nullptr == m_instance)
	{
		m_instance = new VrPublicConfig();
	}

	return m_instance;
}

int VrPublicConfig::init()
{
	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (!doc->load_file("vrpublic_config.xml"))
	{
		return -1;
	}

	bool load_ret = true;
	load_ret = this->loadGateInfo(doc.get()) && load_ret;
	load_ret = this->loadVrInfo(doc.get()) && load_ret;
	load_ret = this->loadDataCenterDbInfo(doc.get()) && load_ret;
	load_ret = this->loadDouwuDbInfo(doc.get()) && load_ret;
	load_ret = this->loadguidCfg(doc.get()) && load_ret;

	return load_ret ? 0 : -1;
}

GateCfg & VrPublicConfig::getGateCfg()
{
	return m_gate_cfg;
}

VrCfg & VrPublicConfig::getVrCfg()
{
	return m_vr_cfg;
}

DbInfo & VrPublicConfig::getDataCenterCfg()
{
	return m_data_center_info;
}

DbInfo & VrPublicConfig::getDataDouwuCfg()
{
	return m_data_douwu_info;
}

GuidCfg & VrPublicConfig::getGuidCfg()
{
	return m_guid_cfg;
}

bool VrPublicConfig::loadGateInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("gate");
	Attribute * attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);

	m_gate_cfg.ip_addr = attr->get_value();
	
	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);

	m_gate_cfg.port = attr->as_int();

	return true;
}

bool VrPublicConfig::loadVrInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("vr");
	Attribute * attr = ele->get_attribute("ip_addr");

	CHECK_NULL_PTR_RETURN(attr, false);
	m_vr_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_vr_cfg.port = attr->as_int();

	return true;
}

bool VrPublicConfig::loadDataCenterDbInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("data_center");
	Attribute * attr = ele->get_attribute("ip_addr");

	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_center_info.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_center_info.port = attr->as_int();

	attr = ele->get_attribute("db_name");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_center_info.db_name = attr->get_value();

	attr = ele->get_attribute("user_name");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_center_info.user_name = attr->get_value();

	attr = ele->get_attribute("passwd");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_center_info.passwd = attr->get_value();

	return true;
}

bool VrPublicConfig::loadDouwuDbInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("data_douwu");
	Attribute * attr = ele->get_attribute("ip_addr");

	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_douwu_info.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_douwu_info.port = attr->as_int();

	attr = ele->get_attribute("db_name");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_douwu_info.db_name = attr->get_value();

	attr = ele->get_attribute("user_name");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_douwu_info.user_name = attr->get_value();

	attr = ele->get_attribute("passwd");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_douwu_info.passwd = attr->get_value();

	return true;
}

bool VrPublicConfig::loadguidCfg(Document * doc)
{
	Element * ele = doc->get_root()->get_element("guid");
	Attribute * attr = ele->get_attribute("ip_addr");

	CHECK_NULL_PTR_RETURN(attr, false);
	m_guid_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_guid_cfg.port = attr->as_int();

	return true;
}
