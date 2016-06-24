
#ifndef ACCEPT_SERVER_HPP__
#define ACCEPT_SERVER_HPP__

#include <string>
#include <boost/thread.hpp>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "SessionHandle.h"
#include "Session.h"

using namespace std;

class AcceptServer
{
public:
	AcceptServer();
	~AcceptServer();
public:
	int init(const string & ip_str, int port, SessionHandle * session_handle);

	int svc();

	void handleNewConnection(evutil_socket_t fd, struct sockaddr *address);
protected:
	bool initListener();
private:
	struct event_base * m_event_base;
	struct evconnlistener * m_event_listener;
	SessionHandle * m_session_handle;
	int m_port;
	string m_ip_str;
	boost::thread * m_tread_handle;
};

#endif
