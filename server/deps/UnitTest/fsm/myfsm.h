#pragma once

struct IFsm
{

	enum Event { SummerLogin, SummerLogout, Summercreate };

	enum States { Offline, Idle, Matching, Batting}; 

	void A(Event e);

protected:
	States m_cur_state = Offline;
	void processEvent(Event e);


	virtual void enterOffline() = 0;
	virtual void enterIdle() = 0;
	virtual void enterMatching() = 0;
	virtual void enterBatting() = 0;
};

struct IFSMState
{
	virtual void onEnter() = 0;
	virtual void onLeave() = 0;
};


struct IFSMCondition
{

};