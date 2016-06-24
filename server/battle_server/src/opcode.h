
#ifndef OPCODE_HPP__
#define OPCODE_HPP__

enum Opcode
{
	// 
	MSG_BASE = 0,
	// guid
	E_SMsgRequestGuidReq,
	E_SMsgRequestGuidAck,
	E_MSG_PLAYER_CLOSE_CONNECTION,	// gate notify server
	MSG_NEW_CLIENT_SESSION,
	MSG_CLOSE_CLIENT_SESSION,
	E_ClientPingTimeOut,
	E_PlayerOffline,				// vr notify tr

	// gate
	E_SMsgGateRegister = 100,
	E_SMsgClientClosedGateNotifyServer,

	// vr
	// gate register (vr, vrbattle)
	E_SMsgVrRegisterVrpublic	 = 150,	// vr register to vrpublic
	E_SMsgVrpublicNotifyVrLoginInfo,	// 
	E_SMsgVrpublicNotifyVrLoginInfoAck,
	E_SMsgVrRegisterTr,
	E_SMsgBattleRegisterTr,
	E_SMsgVrRegisterVRFriend, // Vr register to friend server
	E_InterMsgError, 		//������֮�������Ϣ

	E_VrModuleTrConnected,			// VRģ�鴦��tr�Ѿ�������Ϣ
	E_VrModuleTrConnectionClosed,
	E_VrModuleVrpConnected,			// VRģ�鴦��vrpublic�Ѿ�������Ϣ
	E_VrModuleVrpConnectionClosed,

	// match
	E_SMsgMatchReq = 200,
	E_SMsgNotifyBattleMatchResult,
	E_proto_Player,
	E_SMsgNotifyBattleMatchResultAck,
	E_MsgMatchAck_Copy,
	E_MsgCancelMatchAck_Copy,
	E_MsgMatchEnterBattle_Copy,

	// dev ob
	E_SMsgRegisterToDevOb = 300,
	E_SMsgObReportGateInfo,
	E_SMsgRegisterOneBattle,
	E_SMsgUpdateBattleFrame,
	E_SMsgUpdateBattleStatus,
	E_SMsgRequestObListReq,
	E_SMsgRequestObListAck,

	E_SMsgBattleEndNotifyVr					= 400,
	E_SMsgVrReciveBattleReportNotifyBattle,

	//friend���
	E_SMsgVRNotifyCreateSummonerToVRF		= 500,  // �����ٻ�ʦ֪ͨ���ѷ�
	E_SMsgVRNotifySummonerLoginToVRF,	// �ٻ�ʦ����֪ͨ
	E_SMsgVRNotifySummonerLogoffToVRF,  // ����
	E_SMsgNotifySumStatusChangeToVRF,	// �ٻ�ʦ״̬�ı�
	E_SMsgNotifySearchFriend,			// ���Һ���
	E_SMsgNotifySearchFriendAck,		// ���Һ���ȷ��
	E_SMsgVRNotifyFriendOperateReq,		//
	E_SMsgVRNotifyAddFriendReq,		//
	E_SMsgVRNotifyDelFriendReq,		//
	E_SMsgVRNotifyConfirmAddFriendReq,		//
	E_SMsgVRNotifyAddCancelFriendReq,		//
	E_SMsgVRFriendNotifyFriendOperateReq,	// ���ѷ������������
	E_SMsgVRFriendNotifyFriendOperateAck,	// 
	E_SMsgFriendListReq,
	E_SMsgFriendListAck,
	E_SMsgApplyForFriendListReq,
	E_SMsgApplyForFriendListAck,
	E_SMsgFriendStatusReq,

	E_SMsgVRFriendDetailInfoReq,
	E_SMsgVRFriendDetailInfoAck,
	E_SMsgFSFriendDetailInfoReq,			// FS: friend server
	E_SMsgFSFriendDetailInfoAck,

	E_SMsgFriendStatusAck,
	E_SMsgNotifySummonerStatusChange,

	E_SMsgInterError, 					//������֮�������Ϣ

	// request message id
	R_GetSummonerGuidByGateGuid			= 800,
	R_GetGateGuidBySummonerId,				

	// notify message id
	N_CreateSummoner					= 900,		// �����ٻ�ʦ֪ͨ
	N_PlayerOnline,									// �������֪ͨ
	N_PlayerOffline,								// �������֪ͨ
	N_VrModuleNotifyAreaId,


	MSG_PLAYER_CLIENT_START_MSG = 1000,
};

#endif