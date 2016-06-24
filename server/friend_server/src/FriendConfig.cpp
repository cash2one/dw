
#include <memory>
#include "FriendConfig.h"
#include "Logger.h"

#define CHECK_NULL_PTR_RETURN(check_ptr, ret)	\
	if (NULL == check_ptr)	\
		    {	\
		DEF_LOG_ERROR(#check_ptr);	\
		return ret;	\
			}

FriendConfig::FriendConfig()
{

}

FriendConfig::~FriendConfig()
{

}

FriendConfig * FriendConfig::instance()
{
	return Singleton<FriendConfig>::instance();
}

int FriendConfig::init()
{
	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (!doc->load_file("friend_config.xml"))
	{
		DEF_LOG_ERROR("failed to load friend_config.xml");
		return -1;
	}

	bool ret = true;
	ret = this->loadFriendCfg(doc.get()) && ret;
	ret = this->loadDBFriendCfg(doc.get()) && ret;

	return ret ? 0 : -1;
}

const FriendCfg & FriendConfig::getFriendCfg()
{
	return m_friend_cfg;
}

const DbInfo & FriendConfig::getDBConfig()
{
	return m_db_friend_info;
}

bool FriendConfig::loadFriendCfg(Document * doc)
{
	Element * ele = doc->get_root()->get_element("friend");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = NULL;

	attr = ele->get_attribute("ip_addr");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_friend_cfg.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_friend_cfg.port = attr->as_int();

	return true;
}

bool FriendConfig::loadDBFriendCfg(Document * doc)
{
	Element * ele = doc->get_root()->get_element("db");
	CHECK_NULL_PTR_RETURN(ele, false);

	Attribute * attr = ele->get_attribute("ip_addr");

	CHECK_NULL_PTR_RETURN(attr, false);
	m_db_friend_info.ip_addr = attr->get_value();

	attr = ele->get_attribute("port");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_db_friend_info.port = attr->as_int();

	attr = ele->get_attribute("db_name");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_db_friend_info.db_name = attr->get_value();

	attr = ele->get_attribute("user_name");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_db_friend_info.user_name = attr->get_value();

	attr = ele->get_attribute("passwd");
	CHECK_NULL_PTR_RETURN(attr, false);
	m_db_friend_info.passwd = attr->get_value();

	return true;
}
