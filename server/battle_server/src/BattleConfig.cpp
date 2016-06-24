
#include <memory>
#include "BattleConfig.h"
#include "Logger.h"
#include "check_macro.h"

#define CHECK_NULL_PTR_RETURN(check_ptr, ret)	\
	if (NULL == check_ptr)	\
		    {	\
		DEF_LOG_ERROR(#check_ptr);	\
		return ret;	\
			}

BattleConfig::BattleConfig()
{

}

BattleConfig::~BattleConfig()
{

}

int BattleConfig::init()
{
	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (!doc->load_file("battle_config.xml"))
	{
		DEF_LOG_ERROR("failed to load battle_config.xml");
		return -1;
	}

	bool ret = true;
	ret = this->loadBattleCfg(doc.get()) && ret;
	ret = this->loadGateCfg(doc.get()) && ret;
	ret = this->loadTrCfg(doc.get()) && ret;

	return ret ? 0 : -1;
}

GateCfg & BattleConfig::getGateCfg()
{
	return m_gate_cfg;
}

TrCfg & BattleConfig::getTrCfg()
{
	return m_tr_cfg;
}

BattleCfg & BattleConfig::getBattleCfg()
{
	return m_battle_cfg;
}

#define LOAD_BATTLE_SETTING_PARAM_INT(param_name)	\
	ele = root_ele->get_element(#param_name);	\
	if (NULL != ele)	\
		{	\
		attr = ele->get_attribute("");	\
		battle_setting.param_name = attr->as_int() * 1000;	\
		DEF_LOG_INFO("load battle setting param <%s>, value is <%d>", #param_name, battle_setting.param_name);	\
		}	\
		else	\
	{	\
		ret = false;	\
		DEF_LOG_ERROR("failed to load battle setting param <%s>", #param_name);	\
	}

bool BattleConfig::loadBattleSetting(BattleSetting & battle_setting)
{
	return true;

	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (!doc->load_file("battle_setting.xml"))
	{
		DEF_LOG_ERROR("failed to load battle_setting.xml");
		return false;
	}

	Element * root_ele = doc->get_root();
	Element * ele = NULL;
	Attribute * attr = NULL;

	CK_NULL_RETURN(root_ele, false);

	bool ret = true;

	LOAD_BATTLE_SETTING_PARAM_INT(ban_timeout);
	LOAD_BATTLE_SETTING_PARAM_INT(select_hero_timeout);
	LOAD_BATTLE_SETTING_PARAM_INT(buy_skin_timeout);
	LOAD_BATTLE_SETTING_PARAM_INT(loading_timeout);
	LOAD_BATTLE_SETTING_PARAM_INT(battle_timeout);
	LOAD_BATTLE_SETTING_PARAM_INT(all_offline_battle_timeout);
	LOAD_BATTLE_SETTING_PARAM_INT(frame_delta);
	LOAD_BATTLE_SETTING_PARAM_INT(pre_act_frames);

	return ret;
}

BattleConfig * BattleConfig::instance()
{
	return Singleton<BattleConfig>::instance();
}

bool BattleConfig::loadBattleCfg(Document * doc)
{
	Element * ele = doc->get_root()->get_element("battle");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = NULL;
	
	attr = ele->get_attribute("battle_id");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_battle_cfg.battle_id = attr->as_int();

	attr = ele->get_attribute("delete_frame_pack");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_battle_cfg.delete_frame_pack = attr->as_int();

	return true;
}

bool BattleConfig::loadGateCfg(Document * doc)
{
	Element * ele = doc->get_root()->get_element("gate");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = NULL;

	attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_gate_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_gate_cfg.port = attr->as_int();

	return true;

}

bool BattleConfig::loadTrCfg(Document * doc)
{
	Element * ele = doc->get_root()->get_element("tr");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = NULL;

	attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_tr_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_tr_cfg.port = attr->as_int();

	return true;
}
