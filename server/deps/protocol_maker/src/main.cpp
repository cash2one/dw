// protocol_maker.cpp : Defines the entry point for the console application.

#include <iostream>

#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/idl.h"
#include "flatbuffers/util.h"

#include "ProtocolMaker.h"

using namespace std;

int main(int argc, char * argv[])
{
	if (argc < 3)
	{
		std::cout << "please input : source path and output path" << std::endl;
		return 1;
	}

	ProtocolMaker maker;

	maker.init(argv[1], argv[2]);
	
	return 0;
}

