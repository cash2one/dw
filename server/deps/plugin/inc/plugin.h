#pragma once
#include <unordered_map>
#include <iostream>
#include "typedef.h"
#include <flatbuffers/flatbuffers.h>
#include "MessagePump.h"
#include "sender.h"

namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

class Pool;
class Logger;
struct PluginDepot;
struct PluginParam;
struct  PluginCfg
{
	Pool* pool = NULL;
	Logger* logger = NULL;
	PluginDepot* depot = NULL;
	Sender * sender = NULL;
	PluginParam* params = NULL;
};


struct PluginParam
{
	void append(const std::string param_name, const std::string param_value);

	bool getStr(const std::string  param_name, std::string & param_value);
	bool getInt32(const std::string  param_name, int32 & param_value);
	bool getUint64(const std::string  param_name, uint64 & param_value);

	std::map<std::string, std::string > pairs;
};

struct PackInfo
{
	PackInfo(uint32 a_opcode, uint64 a_guid, google::protobuf::Message* a_msg)
	{
		opcode = a_opcode;
		guid = a_guid;
		msg = a_msg;
	}

	uint32 opcode = 0;
	uint64 guid = 0;
	google::protobuf::Message* msg = NULL;
};


typedef std::function<int(PackInfo & pack_info) > MsgHandler;
typedef unordered_map<int, PacketHandleBase<InterPacket>* > InputHandlerMap;
typedef unordered_map<int, MsgHandler> RequestHandlerMap;
typedef unordered_map<int, vector<MsgHandler> >	NotifyHandlerMap;

typedef std::function<int(const vector<string> & gm_param, uint64 target_guid)> GMMsgHandler;
typedef unordered_map<string, GMMsgHandler>	GMcmdHandlerMap;



#define BEGIN_INPUT_MSG_MAP() \
	const InputHandlerMap & input()	\
	{	\
		int input_size = 0; \
		if (m_input_msg_type_map.size() == 0)	\
																				{

#define  INPUT_NEW_MSG(pluginClassName, op_code, msgClassName) \
			(new PacketHandle<pluginClassName, InterPacket, msgClassName>(this, &pluginClassName::handle##op_code))

#define INPUT_HANDLE_MSG(pluginClassName, op_code, msgClassName)	\
			{ \
				m_input_msg_type_map[op_code] = INPUT_NEW_MSG(pluginClassName, op_code, msgClassName); \
				input_size++; \
			}


#define END_INPUT_MSG_MAP() \
																		}	\
		return m_input_msg_type_map;	\
	}

#define INPUT_FUN_DECLARE(pluginClassName, op_code, msgClassName) \
	int handle##op_code(InterPacket * input_packet, msgClassName const* input_msg)

#define INPUT_FUN_DEFINE(pluginClassName, op_code, msgClassName) \
	int  pluginClassName::handle##op_code(InterPacket * input_packet, msgClassName const* input_msg)


#define BEGIN_REQUEST_MSG_MAP() \
	const RequestHandlerMap & request()	\
	{	\
		 int request_size = 0; \
		if (m_request_msg_type_map.size() == 0)	\
								{ \

#define REQUEST_HANDLE_MSG(pluginClassName, op_code, msgClassName)	\
			{ \
				MsgHandler it = std::bind(&pluginClassName::handle##op_code, this, std::placeholders::_1); \
				m_request_msg_type_map[op_code] = it; \
				request_size++; \
			}


#define END_REQUEST_MSG_MAP() \
						}	\
		return m_request_msg_type_map;	\
	}

#define REQUEST_FUN_DECLARE(pluginClassName, op_code, msgClassName) \
	int handle##op_code(PackInfo& req)

#define REQUEST_FUN_DEFINE(pluginClassName, op_code, msgClassName) \
	int  pluginClassName::handle##op_code(PackInfo& req)


#define BEGING_NOTIFY_MSG_MAP() \
	const NotifyHandlerMap & notify()	\
	{	\
		int notify_size = 0; \
		if (m_notify_msg_type_map.size() == 0)	\
												{

#define NOTIFY_HANDLE_MSG(pluginClassName, op_code, msgClassName)	\
			{ \
				MsgHandler it = std::bind(&pluginClassName::handle##op_code, this, std::placeholders::_1); \
				m_notify_msg_type_map[op_code].push_back(it); \
				notify_size++; \
			}

#define END_NOTIFY_MSG_MAP()	\
						}	\
		return m_notify_msg_type_map;	\
	}


#define NOTIFY_FUN_DECLARE(pluginClassName, op_code, msgClassName) \
	int handle##op_code(PackInfo& ntf)

#define NOTIFY_FUN_DEFINE(pluginClassName, op_code, msgClassName) \
	int  pluginClassName::handle##op_code(PackInfo& ntf)


#define BEGING_GMCMD_MSG_MAP() \
	const GMcmdHandlerMap & gm()	\
	{	\
	if (m_gmcmd_msg_type_map.size() == 0)	\
				{

#define GMCMD_HANDLE_MSG(op_code, fun)	\
		GMMsgHandler it = std::bind(fun, this, std::placeholders::_1, std::placeholders::_2); \

#define END_GMCMD_MSG_MAP()	\
				}	\
		return m_gmcmd_msg_type_map;	\
	}

struct Plugin
{
	Plugin();
	virtual ~Plugin(){}

	virtual int init(const PluginCfg plugin_cfg);
	virtual bool isStartupSuccess(){ return m_startup_success; }
	virtual bool isShutdownSuccess(){ return m_shutdown_success;  }

	virtual const InputHandlerMap& input() { return m_input_msg_type_map; }
	virtual const RequestHandlerMap& request() { return m_request_msg_type_map; }
	virtual const NotifyHandlerMap& notify() { return m_notify_msg_type_map; }
	virtual const GMcmdHandlerMap& gm() { return m_gmcmd_msg_type_map; }
	
	const std::string name() { return m_plugin_name; }
protected:
	PluginCfg m_plugin_cfg;

	friend struct PluginDepot;
	InputHandlerMap		m_input_msg_type_map;
	RequestHandlerMap	m_request_msg_type_map;
	NotifyHandlerMap	m_notify_msg_type_map;
	GMcmdHandlerMap		m_gmcmd_msg_type_map;

	bool m_startup_success = false;
	bool m_shutdown_success = false;

	std::string m_plugin_name = "default"; // 用字符串当做plugin标识, 不会像枚举一样冲突, and 容易辨识
};