
#include "SessionServerImp.h"
#include "SessionServerRaknet.h"

SessionServer * SessionServer::createSessionServer(NetProtocolType net_type)
{
	SessionServer * ss = NULL;
	if (NPT_TCP == net_type)
	{
		//
		ss = new SessionServerImp();
	}
	else if (NPT_RAKNET == net_type)
	{
		ss = new SessionServerRaknet();
	}
	else
	{
		// error, default tcp
		ss = new SessionServerImp();
	}

	return ss;
}
