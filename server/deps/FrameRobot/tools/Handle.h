#ifndef HANDLE_H_
#define HANDLE_H_


//#include "EventBase.h"

//#include "MsgBase.h"

class TimeEvent;

/*
* @berif:消息处理句柄
*/
template <class Oper>
class MessageBaseHandle{
public:
 virtual ~MessageBaseHandle(){};
 virtual void Handle( Oper* pOper,char* pMsg,int len )=0;
// virtual void handleRobotMsg(Oper * pOper, MsgBase * msg) = 0;
};
template <class T,class Oper,class Msg >
class MessageHandle :public MessageBaseHandle<Oper> {
public:
	typedef void (T::*Func)( Oper*,Msg* );
	T* m_pObj;
	Func m_pFunc;
	MessageHandle(T* obj,Func func):m_pObj(obj),m_pFunc(func){
	}
	void Handle( Oper* pOper, char* pMsgBuff,int len ){
		Msg msg( pMsgBuff, len );
#ifdef _WIN32
			//设断点
		if ( *(WORD*)(pMsgBuff+2) == 257/*Msg_ReqSysTime*/ )
		{
			(m_pObj->*m_pFunc)( pOper,&msg );
		}
		else if ( *(WORD*)(pMsgBuff+2) == 12870 || *(WORD*)(pMsgBuff+2) == 12853 )
		{
			(m_pObj->*m_pFunc)( pOper,&msg );
		}
		else
		{
			(m_pObj->*m_pFunc)( pOper,&msg );
		}
#else
		(m_pObj->*m_pFunc)( pOper,&msg );
#endif
	}

	//void handleRobotMsg(Oper * pOper, MsgBase * msg)
	//{
	//	Msg * m = dynamic_cast<Msg *>(msg);
	//	if (NULL != m)
	//	{
	//		(m_pObj->*m_pFunc)( pOper, m);		
	//	}
	//	else 
	//	{
	//		// error
	//	}
	//}
};

  /*
   * @berif:事件处理句柄
   */
  class EventBaseHandle{
  public:
 	 virtual ~EventBaseHandle(){};
 	 virtual void Handle(TimeEvent* evt)=0;
  };
  /*
   * @brief:事件处理对象
   */
  template <class T,class Evt >
 class EventHandle :public EventBaseHandle{
 public:
 	typedef void (T::*Func)( Evt* );
 	T* m_pObj;
 	Func m_pFunc;
 	EventHandle(T* obj,Func func):m_pObj(obj),m_pFunc(func){
 	}
 	void Handle(TimeEvent* timeEvt){
 		(m_pObj->*m_pFunc)((Evt*)timeEvt);
 	}
 };

#endif /* HANDLE_H_ */
