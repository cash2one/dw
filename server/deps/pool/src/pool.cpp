#include "pool.h"
#include "dynamicparse.h"
#ifdef WIN64
#include <WinSock2.h>
#endif 
#include "loader.hpp"
#include "Single.hpp"


std::string GetEntityeName(uint64_t guid);

MSG_TYPE* Pool::get(uint64_t guid)
{	
	auto it = m_entities.find(guid);
	if (it != m_entities.end())
		return it->second;
	return NULL;
}

bool Pool::add(uint64_t guid, MSG_TYPE* msg, bool add_to_db)
{	
	if (NULL != get(guid))
	{	
		DEF_LOG_ERROR("entity existed, %llu", guid);
		return false;
	}

	m_entities.insert(std::make_pair(guid, msg));

	if (!add_to_db)
		return true;

	RequestInfost *st = new RequestInfost;
	st->as = msg->New();
	st->as->MergeFrom(*msg);
	st->op = DB_INSERT;
	st->cb = std::bind(&Pool::cbLoaded, this, std::placeholders::_1);
	st->guid = guid;
	Single<Loader>::GetInstance()->AddTask(guid, st);
	return true;
}

bool Pool::update(uint64_t guid)
{	
	auto msg = get(guid);
	if (NULL == msg)
	{
		DEF_LOG_ERROR("entity NOT existed, %llu", guid);
		return false;
	}

	RequestInfost *st = new RequestInfost;
	st->as = msg->New();
	st->as->MergeFrom(*msg);
	st->op = DB_UPDATE;
	st->guid = guid;
	st->cb = std::bind(&Pool::cbLoaded, this, std::placeholders::_1);
	Single<Loader>::GetInstance()->AddTask(guid, st);
	return true;
}

bool Pool::remove(uint64_t guid)
{	
	auto it = m_entities.find(guid);
	if (it == m_entities.end())
	{
		DEF_LOG_ERROR("entity NOT existed, %llu", guid);
		return false;
	}
	
	delete it->second;
	m_entities.erase(it);
	return true;
}

uint32_t Pool::load(uint64_t guid, LoadCallBack cb)
{
	auto msg = get(guid);
	if (NULL != msg)
	{
		DEF_LOG_ERROR("entity  existed, %llu", guid);
		return false;
	}

	RequestInfost *st = new RequestInfost;
	do 
	{
		st->op = DB_LOAD;
		st->guid = guid;
		st->as = Single<DynamicParser>::GetInstance()->create(GetEntityeName(guid));
		if (NULL == st->as)
		{
			DEF_LOG_ERROR("entity class NOT existed, %s", GetEntityeName(guid).c_str());
			return false;
		}

		uint32_t id = m_index++;
		if (0 == m_index)
			m_index++;
		st->id = id;
		m_cb.insert(std::make_pair(id, cb));
		st->cb = std::bind(&Pool::cbLoaded, this, std::placeholders::_1);
		Single<Loader>::GetInstance()->AddTask(guid, st);
		return id;

	} while (false);
	
	delete st;
	return 0;
}

void Pool::cbLoaded(RequestInfost *st)
{	
	if (st->op == DB_LOAD)
	{	
		std::unique_lock<std::mutex> lock(m_loaded_mutex);
		google::protobuf::Message* msg = NULL;
		if (st->res)
			msg = st->as;
		else
			delete st->as;

		Loadst l(st->id, st->guid, msg);
		m_loaded.push_back(l);

		return;
	}


	delete st->as;
	delete st;
}

void Pool::update()
{	
	std::vector<Loadst> local;
	{
		std::unique_lock<std::mutex> lock(m_loaded_mutex);
		local.swap(m_loaded);
	}

	for (auto& it : local)
	{
		auto findIt = m_cb.find(it.id);
		if (findIt == m_cb.end())
		{	
			// log err
			delete it.msg;
			continue;
		}

		(findIt->second)(it.id, it.guid, it.msg);
	}
}

std::string GetEntityeName(uint64_t guid)
{
	return "Entity.summoner";
}

