// vr.cpp : Defines the entry point for the console application.
//

#include <malloc.h>
#include "ManageLogger.h"
#include "VrConfig.h"
#include "ManageClientPacket.h"
#include "ClientMessageManager.h"
#include "Redirect.h"
#include "ManagerClientSession.h"
#include "ManageVrpublicMessage.h"
#include "ManageVrpublicSession.h"
#include "ManageVrpublicPacket.h"
#include "ManageTrSession.h"
#include "ManageDB.h"
#include "BaseFunc.h"
#include "ManageFriendSession.h"
#include "ManageSingleton.h"
#include "utility/inc/Single.hpp"
#include "pool/inc/pool.h"
#include "plugin/inc/pluginDepot.h"
#include "dynamicparse/inc/dynamicparse.h"
#include "pool/inc/loader.hpp"

#include "ManageClientMsgPump.h"
#include "ManageFriendMsgPump.h"
#include "ManageVrpublicMsgPump.h"
#include "ManageTrMsgPump.h"

// plugin
#include "PlayerLoginVR.h"
#include "TrSystemVR.h"
#include "FRSystemVR.h"
#include "account.pb.h"
#include <google/protobuf/descriptor.h>
#include "Logger.h"

int startServer(int argc, char * argv[])
{
	CHECK_INPUT_PARAM(argc, argv);

	START_DAEMONIZE_CMD();

	IGNORE_LINUX_SINGLE();





	ManageSingleton::instance();

	ManageLogger::instance()->init();

	if (-1 == VrConfig::instance()->init())
	{
		DEF_LOG_ERROR("failed to load vr cfg");
		return -1;
	}

	ClientCfg & client_cfg = VrConfig::instance()->getClientCfg();

	TrCfg & tr_cfg = VrConfig::instance()->getTrCfg();

	VrpublicCfg & vrpublic_cfg = VrConfig::instance()->getVrpublicCfg();

	DbInfo & data_account_db = VrConfig::instance()->getDBAccountInfo();

	GuidCfg & guid_cfg = VrConfig::instance()->getGuidCfg();

	MakeGuid::instance()->init(guid_cfg.ip_addr, guid_cfg.port);

	FriendCfg & friend_cfg = VrConfig::instance()->getFriendCfg();

	// 在 pump 初始化之前  create
	Single<DynamicParser>::Instance();
	std::string root_dir = VrConfig::instance()->getProtoCfg();
	if (-1 == Single<DynamicParser>::GetInstance()->init(root_dir))
	{
		DEF_LOG_ERROR("failed to init dynamic parse, path:%s", root_dir.c_str());
		return -1;
	}

	// 更新数据库字段
	TableColumnsMap cols;
	Single<DynamicParser>::GetInstance()->querydbDesc(data_account_db.ip_addr, data_account_db.port,
		data_account_db.user_name, data_account_db.passwd, data_account_db.db_name, cols);
	std::vector<std::string> changed;
	Single<DynamicParser>::GetInstance()->checkEntity(cols, data_account_db.db_name, changed);
	Single<DynamicParser>::GetInstance()->updatedbChanged(data_account_db.ip_addr, data_account_db.port,
		data_account_db.user_name, data_account_db.passwd, data_account_db.db_name, changed);


	//Entity::summoner * sum1 = new Entity::summoner();
	//sum1->set_guid(11);
	//sum1->set_sum_name("robot_3");
	//const google::protobuf::Descriptor * des1 = sum1->GetDescriptor();

	//Entity::summoner * sum2 = new Entity::summoner();
	//const google::protobuf::Descriptor * des2 = sum2->GetDescriptor();

	//google::protobuf::Message * sum3 = Single<DynamicParser>::GetInstance()->createMessage("Entity.summoner");
	//const google::protobuf::Descriptor * des3 = sum3->GetDescriptor();

	//const google::protobuf::Descriptor * des4 = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName("Entity.summoner");

	//Entity::summoner * sum1 = new Entity::summoner();
	//sum1->set_guid(11);
	//sum1->set_sum_name("robot_3");
	//const google::protobuf::Descriptor * des1 = sum1->GetDescriptor();

	//google::protobuf::Message * msg = google::protobuf::MessageFactory::generated_factory()->GetPrototype(des4)->New();
	//msg->MergeFrom(*sum1);

	//sum2->MergeFrom(*msg);

	Single<Loader>::Instance();
	if (-1 == Single<Loader>::GetInstance()->Init(4, data_account_db.ip_addr,
		data_account_db.user_name, data_account_db.passwd, data_account_db.port, data_account_db.db_name))
	{
		DEF_LOG_ERROR("failed to init loader");
		return -1;
	}

	Single<Pool>::Instance();
	Single<PluginDepot>::Instance();

	Single<Sender>::Instance();
	Single<Sender>::GetInstance()->client_cb = std::bind(&ManageClientMsgPump::handleOutputPacket,
		ManageClientMsgPump::instance(), std::placeholders::_1);

	Single<Sender>::GetInstance()->vrpublic_cb = std::bind(&ManageVrpublicMsgPump::handleOutputPacket,
		ManageVrpublicMsgPump::instance(), std::placeholders::_1);

	Single<Sender>::GetInstance()->tr_cb = std::bind(&ManageTrMsgPump::handleOutputPacket,
		ManageTrMsgPump::instance(), std::placeholders::_1);

	Single<Sender>::GetInstance()->friend_cb = std::bind(&ManageFriendMsgPump::handleOutputPacket,
		ManageFriendMsgPump::instance(), std::placeholders::_1);

	// 这里添加插件
	//Single<PluginDepot>::GetInstance()->addPlugin(Single<DemoSystemVR>::Instance());
	Single<PluginDepot>::GetInstance()->addPlugin(Single<PlayerLoginVR>::Instance());
	Single<PluginDepot>::GetInstance()->addPlugin(Single<FRSystemVR>::Instance());
	Single<PluginDepot>::GetInstance()->addPlugin(Single<TrSystemVR>::Instance());

	// 在pump之前初始化
	PluginDepotCfg plugindepot_cfg;
	plugindepot_cfg.logger = g_logger_instance;
	plugindepot_cfg.pool = Single<Pool>::GetInstance();
	plugindepot_cfg.sender = Single<Sender>::GetInstance();
	if (-1 == Single<PluginDepot>::GetInstance()->init(plugindepot_cfg))
	{
		DEF_LOG_ERROR("failed to init plugin depot");
		return -1;
	}


	// pump 初始化
	ManageClientMsgPump::instance()->init(client_cfg);
	ManageFriendMsgPump::instance()->init(friend_cfg);
	ManageVrpublicMsgPump::instance()->init(vrpublic_cfg);
	ManageTrMsgPump::instance()->init(tr_cfg);

	ManageDB::GetInstance()->init(data_account_db);


	BaseFunc::IllegalWordCheck::InitMark();

	time_t last_maloc_trim_time = getSystemMilliSeconds();

	int incoming_packet_num = 0;
	int incoming_bytes = 0;

	while (true)
	{
		ManageClientMsgPump::instance()->update();
		ManageFriendMsgPump::instance()->update();
		ManageVrpublicMsgPump::instance()->update();
		ManageTrMsgPump::instance()->update();

		ManageDB::GetInstance()->update();

		Single<Pool>::GetInstance()->update();

		if (0 == incoming_packet_num)
		{
			boost::system_time sleep_time = boost::get_system_time() + boost::posix_time::millisec(1);
			boost::thread::sleep(sleep_time);
		}

#ifndef WIN32
		time_t curr_time = getSystemMilliSeconds();
		if (curr_time - last_maloc_trim_time > 60000)
		{
			malloc_trim(0);
			last_maloc_trim_time = curr_time;
		}
#endif
	}
}

int main(int argc, char * argv[])
{	
	int ret = startServer(argc, argv);

	boost::system_time sleep_time = boost::get_system_time() + boost::posix_time::millisec(3000);
	boost::thread::sleep(sleep_time);

	return 0;
}


