
#include <memory>
#include "TrConfig.h"
#include "Singleton.h"
#include "Logger.h"

#define CHECK_NULL_PTR_RETURN(check_ptr, ret)	\
	if (NULL == check_ptr)	\
    {	\
		DEF_LOG_ERROR(#check_ptr);	\
		return ret;	\
	}

TrConfig::TrConfig()
{

}

TrConfig::~TrConfig()
{

}

TrConfig * TrConfig::instance()
{
	return Singleton<TrConfig>::instance();
}

int TrConfig::init()
{
	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (!doc->load_file("tr_config.xml"))
	{
		DEF_LOG_ERROR("failed to load tr_config.xml");
		return -1;
	}

	bool ret = true;
	ret = this->loadTrCfg(doc.get()) && ret;
	ret = this->loadBattleCfg(doc.get()) && ret;

	return ret ? 0 : -1;
}

const VrCfg & TrConfig::getVrCfg()
{
	return m_vr_cfg;
}

const BattleCfg & TrConfig::getBattleCfg()
{
	return m_battle_cfg;
}

bool TrConfig::loadTrCfg(Document * doc)
{
	Element * ele = doc->get_root()->get_element("vr");

	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = NULL;
	
	attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_vr_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_vr_cfg.port = attr->as_int();

	return true;
}

bool TrConfig::loadBattleCfg(Document * doc)
{
	Element * ele = doc->get_root()->get_element("battle");

	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = NULL;

	attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_battle_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_battle_cfg.port = attr->as_int();

	return true;
}
