
#ifndef MANAGE_STATE_HPP__
#define MANAGE_STATE_HPP__

#include <map>
#include <vector>
#include <unordered_map>
#include <boost/bind.hpp>
#include "Logger.h"

using namespace std;


template<class _StateType, class _EventType, class _StateEntity>
class ManageState
{
public:
	ManageState();
	~ManageState();
public:
	typedef boost::function<bool(_StateEntity * )> StateActionFun_t;	// go to next state if fun return true

	struct StateHistory
	{
		StateHistory()
		{}

		StateHistory(_StateType _pre_state, _EventType _event_type, _StateType _next_state)
			: pre_state(_pre_state)
			, event_type(_event_type)
			, next_state(_next_state)
		{}
		_StateType pre_state;
		_EventType event_type;
		_StateType next_state;
	};

	struct StateCell 
	{
		StateCell()
		{}

		StateCell(_StateType _pre_state, _EventType _event_type, _StateType _next_state, StateActionFun_t _action_fun)
			: pre_state(_pre_state)
			, event_type(_event_type)
			, next_state(_next_state)
			, action_fun(_action_fun)
		{}
		_StateType pre_state;
		_EventType event_type;
		_StateType next_state;
		StateActionFun_t action_fun;
	};

	typedef map <_EventType, StateCell *> EventActionMap_t;
	typedef map <_StateType, EventActionMap_t *> StateMachineMap_t;

	typedef vector<StateHistory *> StateHistoryVec_t;

	bool registerState(_StateType pre_state, _EventType event_type, _StateType next_state, StateActionFun_t action_fun);
	bool processEvent(_StateEntity * entity, _EventType event_type);
protected:
	void recordState(_StateType pre_state, _EventType event_type, _StateType next_state);
private:

	StateMachineMap_t m_state_machine_map;

	StateHistoryVec_t m_state_history;
};

template<class _StateType, class _EventType, class _StateEntity>
ManageState<_StateType, _EventType, _StateEntity>::ManageState()
{

}

template<class _StateType, class _EventType, class _StateEntity>
ManageState<_StateType, _EventType, _StateEntity>::~ManageState()
{
	for (auto history : m_state_history)
	{
		delete history;
	}
}

template<class _StateType, class _EventType, class _StateEntity>
bool ManageState<_StateType, _EventType, _StateEntity>::registerState(_StateType pre_state, _EventType event_type, _StateType next_state, StateActionFun_t action_fun)
{
	EventActionMap_t * event_action_map = NULL;
	auto it = m_state_machine_map.find(pre_state);
	if (m_state_machine_map.end() != it)
	{
		event_action_map = it->second;
	}
	else
	{
		event_action_map = new EventActionMap_t();
		m_state_machine_map[pre_state] = event_action_map;
	}

	auto ea_it = event_action_map->find(event_type);
	if (ea_it != event_action_map->end())
	{
		// 
		DEF_LOG_ERROR("register reduplication state, pre state is <%d>, event type is <%d>, next_state is <%d>", int(pre_state), int(event_type), int(next_state));
		return false;
	}

	StateCell * state_cell = new StateCell(pre_state, event_type, next_state, action_fun);
	event_action_map->insert(std::make_pair(event_type, state_cell));

	return true;
}

template<class _StateType, class _EventType, class _StateEntity>
bool ManageState<_StateType, _EventType, _StateEntity>::processEvent(_StateEntity * entity, _EventType event_type)
{
	_StateType curr_state = entity->getState();

	auto it = m_state_machine_map.find(curr_state);
	if (m_state_machine_map.end() == it)
	{
		// error
		DEF_LOG_ERROR("do not find the entry of state : <%d>", int(curr_state));
		return false;
	}

	EventActionMap_t * ea_map = it->second;

	auto ea_it = ea_map->find(event_type);
	if (ea_map->end() == ea_it)
	{
		DEF_LOG_ERROR("do not find the entry of event, curr state is <%d>, event is <%d>", int(curr_state), int(event_type));
		return false;
	}

	StateCell * state_cell = ea_it->second;
	if (state_cell->action_fun(entity))
	{
		this->recordState(curr_state, event_type, state_cell->next_state);
		entity->setState(state_cell->next_state);
		return true;
	}
	else
	{
		DEF_LOG_ERROR("failed to execute state action fun, curr state is <%d>, event type is <%d>, next state is <%d>", int(curr_state), int(state_cell->event_type), int(state_cell->next_state));
	}

	return false;
}

template<class _StateType, class _EventType, class _StateEntity>
void ManageState<_StateType, _EventType, _StateEntity>::recordState(_StateType pre_state, _EventType event_type, _StateType next_state)
{
	//StateHistory * state_history = new StateHistory(pre_state, event_type, next_state);
	//m_state_history.push_back(state_history);
}


#endif
