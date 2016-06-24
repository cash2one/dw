/* 
 * TestFSM.cpp
 * 测试生成的状态机框架
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
  

