
#include "ManageClientPing.h"

ManageClientPing::ManageClientPing()
	: m_thread(NULL)
	, m_max_ping_time(10000)
	, m_check_last_ping_time(0)
	, m_timeout_notify(NULL)
{
}

ManageClientPing::~ManageClientPing()
{

}

int ManageClientPing::init(ClientPingTimeoutNotify * timeout_notify, int max_ping_time)
{
	m_timeout_notify = timeout_notify;

	m_max_ping_time = max_ping_time;

	m_check_last_ping_time = getSystemMilliSeconds();

	boost::function0<int> f = boost::bind(&ManageClientPing::svc, this);
	m_thread = new boost::thread(f);

	return 0;
}

int ManageClientPing::svc()
{
	return 0;

	boost::posix_time::milliseconds empty_sleep_time(3);

	while (true)
	{
		processAddRemoveClient();
		checkClientPing();

		boost::this_thread::sleep(empty_sleep_time);
	}
	return 0;
}

void ManageClientPing::addClient(uint64_t client_guid)
{
	boost::mutex::scoped_lock lock(m_client_mtx);
	m_add_client_set.insert(client_guid);
}

void ManageClientPing::removeClient(uint64_t client_guid)
{
	boost::mutex::scoped_lock lock(m_client_mtx);
	m_delete_client_set.insert(client_guid);
}

void ManageClientPing::updatePint(uint64_t client_guid)
{
	boost::mutex::scoped_lock lock(m_client_mtx);
	m_update_client_set.insert(client_guid);
}

void ManageClientPing::getClientOpInfo(ClientGuidSet_t & add_client_set, ClientGuidSet_t & delete_client_set, ClientGuidSet_t & update_client_set)
{
	boost::mutex::scoped_lock lock(m_client_mtx);

	add_client_set = m_add_client_set;
	m_add_client_set.clear();

	delete_client_set = m_delete_client_set;
	m_delete_client_set.clear();

	update_client_set = m_update_client_set;
	m_update_client_set.clear();
}

void ManageClientPing::processAddRemoveClient()
{
	ClientGuidSet_t add_client_set;
	ClientGuidSet_t delete_client_set;
	ClientGuidSet_t update_client_set;

	this->getClientOpInfo(add_client_set, delete_client_set, update_client_set);

	for (auto client_guid : add_client_set)
	{
		auto it = m_client_ping_info_map.find(client_guid);
		if (it != m_client_ping_info_map.end())
		{
			// error
			DEF_LOG_ERROR("add client ping, but find it in map <%llu>", client_guid);
			continue;
		}

		DEF_LOG_DEBUG("add client ping info <%llu>", client_guid);

		ClientPingInfo * info = new ClientPingInfo(client_guid);
		info->last_ping_time = getSystemMilliSeconds();
		m_client_ping_info_map.insert(std::make_pair(info->client_guid, info));
	}

	for (auto client_guid : delete_client_set)
	{
		auto it = m_client_ping_info_map.find(client_guid);
		if (m_client_ping_info_map.end() == it)
		{
			// error
			DEF_LOG_ERROR("remove ping info, but did not find it in map <%llu>", client_guid);
			continue;
		}

		DEF_LOG_DEBUG("delete client ping info <%llu>", client_guid);

		delete it->second;
		m_client_ping_info_map.erase(it);

		this->removeFromCheckMap(client_guid);
	}

	for (auto client_guid : update_client_set)
	{
		auto it = m_client_ping_info_map.find(client_guid);
		if (m_client_ping_info_map.end() == it)
		{
			DEF_LOG_ERROR("update client ping, but did not find it", client_guid);
			continue;
		}

		ClientPingInfo * info = it->second;
		info->last_ping_time = getSystemMilliSeconds();

		this->removeFromCheckMap(client_guid);
	}

}

void ManageClientPing::checkClientPing()
{
	time_t curr_time = getSystemMilliSeconds();
	if (curr_time - m_check_last_ping_time < m_max_ping_time / 2)
	{
		return;
	}

	m_check_last_ping_time = curr_time;

	ClientGuidSet_t timeout_of_ping_client_guid;
	for (auto it : m_check_client_ping_info_map)
	{
		ClientPingInfo * info = it.second;
		if (curr_time - info->last_ping_time >= m_max_ping_time)
		{
			// timeout of ping
			timeout_of_ping_client_guid.insert(info->client_guid);
		}
	}

	if (timeout_of_ping_client_guid.size() > 0)
	{
		m_timeout_notify->pingTimeoutNotify(timeout_of_ping_client_guid);
	}

	m_check_client_ping_info_map = m_client_ping_info_map;
}

void ManageClientPing::removeFromCheckMap(uint64_t client_guid)
{
	auto it = m_check_client_ping_info_map.find(client_guid);
	if (it != m_check_client_ping_info_map.end())
	{
		m_check_client_ping_info_map.erase(it);
	}
}


