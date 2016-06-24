
#ifndef MANAGE_CLIENT_PING_HPP__
#define MANAGE_CLIENT_PING_HPP__

#include <unordered_map>
#include <unordered_set>
#include <stdint.h>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "Packet_.h"
#include "Singleton.h"
#include "Logger.h"

using namespace std;

struct ClientPingInfo 
{
	ClientPingInfo()
		: client_guid(0)
		, last_ping_time(0)
	{}

	ClientPingInfo(uint64_t _client_guid)
		: client_guid(_client_guid)
		, last_ping_time(0)
	{}

	uint64_t client_guid;
	uint64_t last_ping_time;
};

typedef unordered_map<uint64_t, ClientPingInfo *> ClientPingInfoMap_t;
typedef unordered_set<uint64_t> ClientGuidSet_t;

class ClientPingTimeoutNotify
{
public:
	virtual void pingTimeoutNotify(ClientGuidSet_t & client_guid_set) = 0;
};

class ManageClientPing
{
public:
	ManageClientPing();
	~ManageClientPing();
public:
	int init(ClientPingTimeoutNotify * timeout_notify, int max_ping_time = 10000);

	int svc();
public:
	void addClient(uint64_t client_guid);
	void removeClient(uint64_t client_guid);
	void updatePint(uint64_t client_guid);

protected:
	void getClientOpInfo(ClientGuidSet_t & add_client_set, ClientGuidSet_t & delete_client_set, ClientGuidSet_t & update_client_set);
	void processAddRemoveClient();
	void checkClientPing();
	void removeFromCheckMap(uint64_t client_guid);
private:
	boost::thread * m_thread;

	int m_max_ping_time;

	time_t m_check_last_ping_time;

	ClientPingTimeoutNotify * m_timeout_notify;

	ClientPingInfoMap_t m_client_ping_info_map;
	ClientPingInfoMap_t m_check_client_ping_info_map;

	ClientGuidSet_t m_add_client_set;
	ClientGuidSet_t m_delete_client_set;
	ClientGuidSet_t m_update_client_set;

	boost::mutex m_client_mtx;
};

#endif
