/* 
 * DoorFSMLogic.h
 * ״̬�������߼���ͷ�ļ�
 */
#include "DoorFSM.h"

class DoorFSMLogic : public DoorFSM 
{
 
 protected:
  virtual void enterOpened(); 
  virtual void enterLocked(); 
  virtual void enterUnlocked(); 
  virtual void enterClosed(); 
};


