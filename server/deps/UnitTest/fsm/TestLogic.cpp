/* 
 * TestFSM.cpp
 * ����״̬���߼�
 */
#include "DoorFSMLogic.h"

int main() 
{
  DoorFSMLogic door;
  door.A(DoorFSM::Close);
  door.A(DoorFSM::Lock);
  door.A(DoorFSM::Unlock);
  door.A(DoorFSM::Open);
}
  

