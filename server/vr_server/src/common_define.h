#ifndef COMMON_DEFINE_H_
#define COMMON_DEFINE_H_

// 在已登录VR之后用
#define CHECKACCOUNT(guid) \
	Account * pAccount = ManageAccount::GetInstance()->findAccount(guid); \
	if (pAccount == NULL) { \
		MessageMaker::sendErrorPacket(packet->session_id, guid, MobaGo::FlatBuffer::Error_VRNoAccount); \
		return 0; \
	} 

#define CHECKSUMMONER(guid) \
	Summoner * pSummoner = pAccount->getSummoner(); \
	if (pSummoner == NULL) \
	{ \
		MessageMaker::sendErrorPacket(packet->session_id, guid, MobaGo::FlatBuffer::Error_NeedCreateSummoner); \
		return 0; \
	} 

#define  MYSQL_GET_QUERY \
mysqlpp::Connection  * db_connection = ManageDB::GetInstance()->getMysqlConnection(); \
mysqlpp::Query query = db_connection->query();

#endif

