/* 
 * DoorFSMLogic.h
 * 状态机控制逻辑的头文件
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


