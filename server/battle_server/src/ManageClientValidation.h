
#ifndef MANAGE_CLIENT_VALIDATION_HPP__
#define MANAGE_CLIENT_VALIDATION_HPP__

#include <string>
#include <stdint.h>
#include <unordered_map>
#include "BattleLog.h"

using namespace std;

struct ClientValidationInfo 
{
	ClientValidationInfo()
		: frame_no(0)
		, msg_id(0)
	{}

	bool add(uint64_t player_id, const string & check_validation_value)
	{
		PlayerValidationMap_t::iterator it = player_validation_map.find(player_id);
		if (it != player_validation_map.end())
		{
			return false;
		}

		player_validation_map[player_id] = check_validation_value;

		return true;
	}

	typedef unordered_map<uint64_t, string> PlayerValidationMap_t;
	uint32 frame_no;
	int msg_id;
	PlayerValidationMap_t player_validation_map;
};

class ManageClientValidation
{
public:
	ManageClientValidation();
	~ManageClientValidation();
public:
	void init(int player_number);
	void handleValidation(uint32_t server_frame_no, uint32_t client_frame_no, int msg_id, uint64_t player_id, const string & check_validation_value);

	uint64_t makeIndex(int msg_id, uint32_t client_frame_no);

	bool check(ClientValidationInfo & client_validation_info);

	bool isCheckSuccess();

	void update(uint32_t curr_frame_index);
protected:
private:
	int m_player_number;

	bool m_is_check_success;

	// first : msg_id * 1000000 + client_frame_no
	typedef unordered_map<int, ClientValidationInfo> ClientValidationInfoMap_t;

	ClientValidationInfoMap_t m_client_validation_info_map;
};

#endif
