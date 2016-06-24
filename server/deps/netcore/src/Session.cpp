
#include "Session.h"
#include "SessionEventBuffer.h"
#include "SessionServerRaknet.h"

Session::~Session()
{
}

Session * Session::createSession()
{
	return new SessionEventBuffer();
}

