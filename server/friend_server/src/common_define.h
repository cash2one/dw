#ifndef COMMON_DEFINE_H_
#define COMMON_DEFINE_H_

#define  MYSQL_GET_QUERY \
mysqlpp::Connection  * db_connection = ManageDB::GetInstance()->getMysqlConnection(); \
mysqlpp::Query query = db_connection->query();

//请求者召唤师数据
#define ASS_SUMMONER_INFO \
dbass::Assfriend_player_info ass_summoner(summoner_id); \
Entity::friend_player_info* e_summoner = (Entity::friend_player_info *)ass_summoner.data();\
if (!ass_summoner.load(query)) \
{ \
	MessageMaker::sendVRErrorPacket(inter_packet->session_id, inter_packet->msg_head.guid, InterError_FriendSvrHasNoSummoner, this); \
	return -1; \
}

//请求者召唤师关系数据
#define ASS_SUMMONER_RELATION \
dbass::Assfriend_player_relation ass_summoner_relation(summoner_id); \
Entity::friend_player_relation* e_summoner_relation = (Entity::friend_player_relation *)ass_summoner_relation.data(); \
if (!ass_summoner_relation.load(query)) \
{ \
	MessageMaker::sendVRErrorPacket(inter_packet->session_id, inter_packet->msg_head.guid, InterError_FriendSvrHasNoSummoner, this); \
	return -1; \
}

// 好友召唤师数据
#define ASS_FRIEND_INFO \
dbass::Assfriend_player_info ass_friend(friend_id); \
Entity::friend_player_info* e_friend = (Entity::friend_player_info *)ass_friend.data();\
if (!ass_friend.load(query)) \
{ \
	MessageMaker::sendVRErrorPacket(inter_packet->session_id, inter_packet->msg_head.guid, InterError_FriendSvrHasNoSummoner, this); \
	return -1; \
}

//好友召唤师关系数据
#define ASS_FRIEND_RELATION \
dbass::Assfriend_player_relation ass_friend_relation(friend_id); \
Entity::friend_player_relation* e_friend_relation = (Entity::friend_player_relation *)ass_friend_relation.data(); \
if (!ass_friend_relation.load(query)) \
{ \
	MessageMaker::sendVRErrorPacket(inter_packet->session_id, inter_packet->msg_head.guid, InterError_FriendSvrHasNoSummoner, this); \
	return -1; \
}

#endif

