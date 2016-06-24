// netcore.cpp : Defines the entry point for the console application.
//

#include <boost/thread.hpp>
#include "SessionServer.h"

class Test : public InputStreamHandle
{
public:
	virtual void handleNewConnection(SessionId session_id, SessionId raknet_session)
	{

	}

	virtual void handleNewConnectionFailed()
	{

	}

	virtual void handleCloseConnection(SessionId session_id, int error_code)
	{
	}

	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
	{
		char * buf = new char[buf_len];
		memcpy(buf, buffer, buf_len);
		m_session_server->handleOutputStream(session_id, buf, buf_len);
		return buf_len;
	}

	void run()
	{
		m_session_server = SessionServer::createSessionServer();
		m_session_server->init(this);

		boost::posix_time::milliseconds empty_sleep_time(1);

		m_session_server->listen("0.0.0.0", 8899);

		while (true)
		{
			boost::this_thread::sleep(empty_sleep_time);
		}
	}

protected:
private:
	SessionServer * m_session_server;
};

int main(int argc, char * argv[])
{
	//Test test;
	//test.run();
	return 0;
}

