
#include "ManageClientValidation.h"

ManageClientValidation::ManageClientValidation()
	: m_player_number(0)
	, m_is_check_success(true)
{

}

ManageClientValidation::~ManageClientValidation()
{

}

void ManageClientValidation::init(int player_number)
{
	if (0 == m_player_number)
	{
		m_player_number = player_number;
	}
}

void ManageClientValidation::handleValidation(uint32_t server_frame_no, uint32_t client_frame_no, int msg_id, uint64_t player_id, const string & check_validation_value)
{
	if (!m_is_check_success)
	{
		return;
	}

	uint64 index = makeIndex(msg_id, client_frame_no);

	m_client_validation_info_map[index].msg_id = msg_id;
	m_client_validation_info_map[index].frame_no = client_frame_no;

	if (!m_client_validation_info_map[index].add(player_id, check_validation_value))
	{
		BATTLE_LOG_ERROR("get reduplicate client validation msg, frame <%d>, msg <%d>, player is <%llu", client_frame_no, msg_id, player_id);
	}

	if (this->check(m_client_validation_info_map[index]) && m_client_validation_info_map[index].player_validation_map.size() == m_player_number)
	{
		m_client_validation_info_map.erase(index);
	}

	if (m_client_validation_info_map.size() > 10)
	{
		//BATTLE_LOG_INFO("m_client_validation_info_map size is <%d>", m_client_validation_info_map.size());
	}
}

uint64_t ManageClientValidation::makeIndex(int msg_id, uint32_t client_frame_no)
{
	uint64 ret = msg_id;
	ret = ret * 10000000 + client_frame_no;

	return ret;
}

bool ManageClientValidation::check(ClientValidationInfo & client_validation_info)
{
	bool check_success = true;
	string check_value;
	bool is_setting = false;
	for (auto it : client_validation_info.player_validation_map)
	{
		if (!is_setting)
		{
			is_setting = true;
			check_value = it.second;
		}
		else
		{
			if (check_value != it.second)
			{
				// error
				check_success = false;
			}
		}
	}

	if (!check_success)
	{
		// error
		m_is_check_success = false;
		BATTLE_LOG_ERROR("check failed ---------------------------------------------");
		for (auto it : client_validation_info.player_validation_map)
		{
			BATTLE_LOG_ERROR("player is <%llu>, validation string is <%s>", it.first, it.second.c_str());
		}

		return false;
	}

	return true;
}

bool ManageClientValidation::isCheckSuccess()
{
	return m_is_check_success;
}

void ManageClientValidation::update(uint32_t curr_frame_index)
{
	//for (auto it : m_client_validation_info_map)
	//{
	//	if (curr_frame_index - it.second.frame_no > 20)
	//	{
	//		//BATTLE_LOG_ERROR("get uncomplate client validation , curr frame index is <%d>, client frame no is <%d>", curr_frame_index, it.second.frame_no);
	//		for (auto player_it : it.second.player_validation_map)
	//		{
	//			//BATTLE_LOG_INFO("frame is <%d>, player id is <%llu>, check value string is <%s>", it.second.frame_no, player_it.first, player_it.second);
	//		}
	//		//BATTLE_LOG_INFO("--------------------------------------------------------------------------------------");
	//	}
	//}
}
