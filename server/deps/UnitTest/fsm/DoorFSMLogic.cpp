/* 
 * DoorFSMLogic.cpp
 * 状态机控制逻辑的实现文件
 */
#include "DoorFSMLogic.h"
#include <iostream>

void DoorFSMLogic::enterOpened()
{
    std::cout << "Enter Opened state." << std::endl;
}

void DoorFSMLogic::enterClosed()
{
    std::cout << "Enter Closed state." << std::endl;
}

void DoorFSMLogic::enterLocked()
{
    std::cout << "Enter Locked state." << std::endl;
}

void DoorFSMLogic::enterUnlocked()
{
    std::cout << "Enter Unlocked state." << std::endl;
}

