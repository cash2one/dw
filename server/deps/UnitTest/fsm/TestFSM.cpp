/* 
 * TestFSM.cpp
 * �������ɵ�״̬�����
 */

#include "DoorFSM.h"

int main() 
{
  DoorFSM door;
  door.A(DoorFSM::Close);
  door.A(DoorFSM::Lock);
  door.A(DoorFSM::Unlock);
  door.A(DoorFSM::Open);
}
  

