
#ifndef MATCH_CELL_HPP__
#define MATCH_CELL_HPP__

#include <unordered_map>
#include <vector>
#include "MyGuidDef.h"
#include "Packet_.h"

using namespace std;



class MatchCell;

typedef vector<MatchCell *> MatchCellVec_t;

class MatchCell
{
public:
	MatchCell();
	~MatchCell();
public:
	void init(MyGuid_t user_guid, SessionId session_id);

	MyGuid_t getUserId();
	SessionId getSessionId();
	
protected:
private:
	MyGuid_t m_user_guid;
	SessionId m_session_id;
	
};

#endif
