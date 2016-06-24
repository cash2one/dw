#include "myfsm.h"

void IFsm::processEvent(Event e)
{
	States old_state = m_cur_state;
	bool pass = false;
	switch (m_cur_state) { //transitions 
	case Closed:
		if (e == Open) {
			//outcome actions
			m_cur_state = Opened;
			pass = true;
		}
		else if (e == Lock) {
			//outcome actions
			m_cur_state = Locked;
			pass = true;
		}
		break;
	case Unlocked:
		if (e == Lock) {
			//outcome actions
			m_cur_state = Locked;
			pass = true;
		}
		else if (e == Open) {
			//outcome actions
			m_cur_state = Opened;
			pass = true;
		}
		break;
	case Locked:
		if (e == Unlock) {
			//outcome actions
			m_cur_state = Unlocked;
			pass = true;
		}
		break;
	case Opened:
		if (e == Close) {
			//outcome actions
			m_cur_state = Closed;
			pass = true;
		}
		break;
	}

	if (old_state == m_cur_state && !pass) { return; }

	switch (m_cur_state) { // income actions
	case Offline:
		enterClosed();
		break;
	case Idle:
		enterUnlocked();
		break;
	case Matching:
		enterLocked();
		break;
	case Batting:
		enterOpened();
		break;
	}
}

void IFsm::A(Event e)
{
	processEvent(e);
}

#include <iostream>

void IFsm::enterOffline()
{
	std::cout << "Enter Opened state." << std::endl;
}

void IFsm::enterIdle()
{
	std::cout << "Enter Closed state." << std::endl;
}

void IFsm::enterMatching()
{
	std::cout << "Enter Locked state." << std::endl;
}

void IFsm::enterBatting()
{
	std::cout << "Enter Unlocked state." << std::endl;
}