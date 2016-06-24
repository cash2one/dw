
#ifndef IO_SERVER_HPP__
#define IO_SERVER_HPP__

#include <string>
#include <vector>
#include <unordered_set>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "SessionHandle.h"
#include "Session.h"
#include "Logger.h"

using namespace std;

struct OutputBufferInfo 
{
	OutputBufferInfo()
		: buffer(NULL)
		, buf_len(0)
		, session(NULL)
	{}

	OutputBufferInfo(char * _buffer, size_t _buf_len, Session * _session)
		: buffer(_buffer)
		, buf_len(_buf_len)
		, session(_session)
	{}

	char * buffer;
	size_t buf_len;
	Session * session;
};

struct ConnectInfo 
{
	ConnectInfo()
		: port(0)
		, auto_reconnect(false)
	{}

	ConnectInfo(const string _ip_str, int _port, bool _auto_reconnect)
		: port(_port)
		, ip_str(_ip_str)
		, auto_reconnect(_auto_reconnect)
	{}

	int port;
	string ip_str;
	bool auto_reconnect;
};

struct ReconnectInfo 
{
	ReconnectInfo()
		: port(0)
		, last_connect_time(0)
		, session(NULL)
	{}

	ReconnectInfo(const string _ip_str, int _port, Session * _session)
		: port(_port)
		, ip_str(_ip_str)
		, last_connect_time(0)
		, session(_session)
	{
		last_connect_time = getSystemMilliSeconds();
	}

	int port;
	string ip_str;
	time_t last_connect_time;
	Session * session;
};

typedef list<OutputBufferInfo> OutputBufferInfoList_t;
typedef boost::unordered_set<Session *> SessionSet_t;
typedef vector<evutil_socket_t> SocketVec_t;
typedef vector<ConnectInfo> ConnectInfoVec_t;
typedef list<ReconnectInfo> ReconnectInfoLst_t;

class IoServer : public SessionHandle
{
public:
	IoServer();
	~IoServer();
public:
	int init(InputStreamHandle * input_stream_handle);

	void connect(const string & ip_str, int port, bool auto_reconnect = false);

	void handleOutputStream(Session * session, char * buffer, size_t buf_len);
public:
	virtual Session * createSessionWithSock(evutil_socket_t sock);
	virtual Session * createSession();
	virtual void handleAcceptedSession(evutil_socket_t sock);
	virtual void handleReadEvent(SessionId session_id);
	virtual void handleWriteEvent(SessionId session_id);
	virtual void handleConnectEvent(SessionId session_id);
	virtual void handleCloseEvent(SessionId session_id);

	virtual void closeSession(SessionId session_id);

public:
	int svc();
protected:
	void getOutputBuffer(OutputBufferInfoList_t & buffer_vec, std::unordered_set<SessionId> & is_closing_session_set);

	bool handleBufferInfo(OutputBufferInfo & buffer_info);

	void getAcceptedSocket(SocketVec_t & socket_vec);

	void handleNewSocket(evutil_socket_t sock);

	void addSession(Session * session);
	bool isAvilableSession(Session * session);
	void removeSession(Session * session);
	void destroySession(Session * session);
	void destroyOutputBufferInfo(OutputBufferInfo & output_buffer_info);
	void getConnectInfo(ConnectInfoVec_t & connect_vec);
	Session * doConnect(const string & ip_str, int port);

	void checkReconnectInfo();

	void checkReconnectWhileSocketClose(SessionId session_id);

	void processClosingSession(std::unordered_set<SessionId> & is_closing_session_set);
private:
	struct event_base * m_event_base;

	InputStreamHandle * m_input_stream_handle;

	boost::thread * m_tread_handle;

	OutputBufferInfoList_t m_output_buffer_info_vec;

	boost::mutex m_output_buffer_info_vec_mtx;

	SocketVec_t m_accepted_socket_vec;

	boost::mutex m_accepted_socket_vec_mtx;

	SessionSet_t m_avilable_session_set;

	ConnectInfoVec_t m_connect_info_vec;

	ReconnectInfoLst_t m_reconnect_info_lst;

	boost::mutex m_connect_info_vec_mtx;

	std::unordered_set<SessionId> m_is_closing_session_set;

};

#endif

