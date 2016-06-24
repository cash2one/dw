
#ifndef EMPTY_MSG_HPP__
#define EMPTY_MSG_HPP__

#include <string>
#include "flatbuffers/flatbuffers.h"

using namespace std;

class EmptyMsg
{
public:
	bool Verify(flatbuffers::Verifier & verifier) const
	{
		return true;
	}

	string SerializeAsString()
	{
		return "";
	}
};


class ProtoAgentMsg
{
public:
	bool Verify(flatbuffers::Verifier & verifier) const
	{
		return true;
	}
};

#endif
