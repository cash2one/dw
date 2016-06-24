// vrpublic.cpp : Defines the entry point for the console application.
//

#ifdef WIN32
#include <WinSock2.h>
#endif // WIN32

#include <malloc.h>
#include "ManageLogger.h"
#include "ManagerClientSession.h"
#include "ClientMessageManager.h"
#include "ManageVrSession.h"
#include "VrMessageManager.h"
#include "Redirect.h"
#include "VrPublicConfig.h"
#include "MakeGuid.h"
#include "ManageServerList.h"
#include "ManageClientPacket.h"
#include "ManageAccount.h"
#include "ManageVrPacket.h"
#include "Redirect.h"

#define CHECK_INIT_VALUE(ret)	\
	if (ret == -1)	\
	{	\
		DEF_LOG_ERROR("failed to init <%s>", #ret);	\
		return 1;	\
	}

int main(int argc, char * argv[])
{
	CHECK_INPUT_PARAM(argc, argv);

	START_DAEMONIZE_CMD();

	IGNORE_LINUX_SINGLE();

	ManageLogger::instance()->init();

	CHECK_INIT_VALUE(VrPublicConfig::instance()->init());

	GateCfg & gate_cfg = VrPublicConfig::instance()->getGateCfg();
	VrCfg & vr_cfg = VrPublicConfig::instance()->getVrCfg();
	GuidCfg & guid_cfg = VrPublicConfig::instance()->getGuidCfg();
	DbInfo & douwu_db_info = VrPublicConfig::instance()->getDataDouwuCfg();
	DbInfo & data_center_db = VrPublicConfig::instance()->getDataCenterCfg();

	CHECK_INIT_VALUE(MakeGuid::instance()->init(guid_cfg.ip_addr, guid_cfg.port));

	CHECK_INIT_VALUE(ManageServerList::instance()->init(douwu_db_info));

	CHECK_INIT_VALUE(ManageVrPacket::instance()->init());

	CHECK_INIT_VALUE(ManagerClientSession::instance()->init(gate_cfg.ip_addr, gate_cfg.port, gate_cfg.net_type));

	CHECK_INIT_VALUE(ManageVrSession::instance()->init(vr_cfg.ip_addr, vr_cfg.port));

	CHECK_INIT_VALUE(ManageClientPacket::instance()->init());

	CHECK_INIT_VALUE(ManageAccount::GetInstance()->init(data_center_db));

	time_t last_maloc_trim_time = getSystemMilliSeconds();

	int client_incoming_packet_num = 0;
	int client_incoming_bytes = 0;

	int vr_incoming_packet_num = 0;
	int vr_incoming_bytes = 0;

	while (true)
	{
		ClientMessageManager::instance()->update(client_incoming_packet_num, client_incoming_bytes);

		VrMessageManager::instance()->update(vr_incoming_packet_num, vr_incoming_bytes);

		ManageAccount::GetInstance()->update();

		if (0 == client_incoming_packet_num && 0 == vr_incoming_packet_num)
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

	return 0;
}

