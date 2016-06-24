
#include <memory>
#include "Logger.h"
#include "VrConfig.h"

#define CHECK_NULL_PTR_RETURN(check_ptr, ret)	\
	if (NULL == check_ptr)	\
			{	\
		DEF_LOG_ERROR(#check_ptr);	\
		return ret;	\
			}

VrConfig * VrConfig::m_instance = NULL;

VrConfig::VrConfig()
{

}

VrConfig::~VrConfig()
{

}

VrConfig * VrConfig::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new VrConfig();
	}

	return m_instance;
}

int VrConfig::init()
{
	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (!doc->load_file("vr_config.xml"))
	{
		return -1;
	}

	bool load_ret = true;

	load_ret = this->loadVrInfo(doc.get()) && load_ret;
	load_ret = this->loadClientInfo(doc.get()) && load_ret;
	load_ret = this->loadTrInfo(doc.get()) && load_ret;
	load_ret = this->loadVrpublicInfo(doc.get()) && load_ret;
	load_ret = this->loadDBInfo(doc.get()) && load_ret;
	load_ret = this->loadguidCfg(doc.get()) && load_ret;
	load_ret = this->loadFriendCfg(doc.get()) && load_ret;
	load_ret = this->loadProtoCfg(doc.get()) && load_ret;

	return load_ret ? 0 : -1;
}

VrCfg & VrConfig::getVrCfg()
{
	return m_vr_cfg;
}

ClientCfg & VrConfig::getClientCfg()
{
	return m_client_cfg;
}

TrCfg & VrConfig::getTrCfg()
{
	return m_tr_cfg;
}

VrpublicCfg & VrConfig::getVrpublicCfg()
{
	return m_vrpublic_cfg;
}

DbInfo & VrConfig::getDBAccountInfo()
{
	return m_data_acount_info;
}

GuidCfg & VrConfig::getGuidCfg()
{
	return m_guid_cfg;
}

FriendCfg & VrConfig::getFriendCfg()
{
	return m_friend_cfg;
}

std::string VrConfig::getProtoCfg()
{
	return m_proto_path;
}

bool VrConfig::loadVrInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("vr");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = ele->get_attribute("area_id");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_vr_cfg.area_id = attr->as_int();

	attr = ele->get_attribute("server_id");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_vr_cfg.server_id = attr->as_int();

	return true;
}

bool VrConfig::loadClientInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("gate");
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

	return true;
}

bool VrConfig::loadTrInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("tr");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_tr_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_tr_cfg.port = attr->as_int();

	return true;
}

bool VrConfig::loadVrpublicInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("vrpublic");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_vrpublic_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_vrpublic_cfg.port = attr->as_int();

	return true;

}

bool VrConfig::loadDBInfo(Document * doc)
{
	Element * ele = doc->get_root()->get_element("db");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = ele->get_attribute("ip_addr");

	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_acount_info.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_acount_info.port = attr->as_int();

	attr = ele->get_attribute("db_name");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_acount_info.db_name = attr->get_value();

	attr = ele->get_attribute("user_name");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_acount_info.user_name = attr->get_value();

	attr = ele->get_attribute("passwd");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_data_acount_info.passwd = attr->get_value();

	return true;
}

bool VrConfig::loadguidCfg(Document * doc)
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

bool VrConfig::loadFriendCfg(Document * doc)
{
	Element * ele = doc->get_root()->get_element("friend");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_friend_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_friend_cfg.port = attr->as_int();

	return true;
}

bool VrConfig::loadProtoCfg(Document* doc)
{
	Element * ele = doc->get_root()->get_element("proto");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = ele->get_attribute("path");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_proto_path = attr->get_value();

	return true;
}
