
#include "MatchCell.h"

MatchCell::MatchCell()
	: m_user_guid(0)
	, m_session_id(NULL)
{

}

MatchCell::~MatchCell()
{

}

void MatchCell::init(MyGuid_t user_guid, SessionId session_id)
{
	m_user_guid = user_guid;
	m_session_id = session_id;
}

MyGuid_t MatchCell::getUserId()
{
	return m_user_guid;
}

SessionId MatchCell::getSessionId()
{
	return m_session_id;
}

