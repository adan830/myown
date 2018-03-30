//--------------------------------------------------------------------
// �ļ���:		PlayerBaseModule.cpp
// ��  ��:		��һ�������
// ˵  ��:		
// ��������:	2014��10��17��
// ������:		 
//    :	   
//--------------------------------------------------------------------
#include "PlayerBaseModule.h"

#include "FsGame/Define/ViewDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/SnsDefine.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/StaticDataDefine.h"
//#include "FsGame/Define/EquipDefine.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/CommonModule/SnsDataModule.h"
#include "FsGame/CommonModule/AsynCtrlModule.h"
#include "FsGame/CommonModule/LevelModule.h"
#include "FsGame/SystemFunctionModule/ToolBoxModule.h"
//#include "FsGame/SceneBaseModule/SceneInfoModule.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
#include "utils/util_func.h"
#include <time.h>
#include "FsGame/Define/PlayerBaseDefine.h"
#include "utils/string_util.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "utils/custom_func.h"
#include "FsGame/CommonModule/EnvirValueModule.h"
#include "extension/FsGmCC/protocols/ProtocolsID.h"
#ifndef FSROOMLOGIC_EXPORTS
#include "FsGame/SystemFunctionModule/ResetTimerModule.h"
//#include "FsGame/Define/ExtraServerDefine.h"
#endif
#include <algorithm>
#include "CreateRoleModule.h"
//#include "ItemModule/FashionModule.h"
#include "ItemModule/ToolItem/ToolItemModule.h"
#include "Define/ExtraServerDefine.h"

// �����߼��ʱ��
const int FORCE_OFFLINE_TIME = 500;

// �������λ��
inline int nx_reset_player_position(void *state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == PlayerBaseModule::m_pPlayerBaseModule)
	{
		return 0;
	}

	// ����������
	CHECK_ARG_NUM(state, nx_reset_player_position, 2);

	// ����������
	CHECK_ARG_INT(state, nx_reset_player_position, 1);
	// ����������
	CHECK_ARG_WIDESTR(state, nx_reset_player_position, 2);
	

	// ��ȡ����
	int sceneID = pKernel->LuaToInt(state, 1);
	const wchar_t* strName = pKernel->LuaToWideStr(state, 2);
	
//	bool ret = PlayerBaseModule::m_pPlayerBaseModule->GMResetPlayerPosition(pKernel, strName, sceneID);
//	pKernel->LuaPushBool(state, ret);
	return 0;
}

#ifndef FSROOMLOGIC_EXPORTS
// �������ǩ��
inline int nx_exchange_gift(void *state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == PlayerBaseModule::m_pPlayerBaseModule)
	{
		return 0;
	}

	// ����������
	CHECK_ARG_NUM(state, nx_exchange_gift, 2);

	// ����������
	CHECK_ARG_OBJECT(state, nx_exchange_gift, 1);
	CHECK_ARG_STRING(state, nx_exchange_gift, 2);

	// ��ȡ����
	PERSISTID player = pKernel->LuaToObject(state, 1);
	const char* strKey = pKernel->LuaToString(state, 2);
	if (StringUtil::CharIsNull(strKey))
	{
		return 0;
	}

	// ��������һ��������
	CVarList msg;
	msg << PROTOCOL_ID_GIFT_EXCH << strKey << pKernel->QueryString(player, FIELD_PROP_ACCOUNT) << pKernel->QueryString(player, FIELD_PROP_UID);

	pKernel->SendExtraMessage(ESI_GMCC_SERVER, msg);

	//pKernel->LuaPushBool(state, true);

	return 1;
}
#endif

//�жϺ�����ַ��
std::vector<PLAYER_VERSION_FUNC> PlayerBaseModule::m_funList;

PlayerBaseModule * PlayerBaseModule::m_pPlayerBaseModule =NULL;

int OnExtraMessage( IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& msg )
{
#ifdef _DEBUG
	size_t nCount = msg.GetCount();
	int nIndex = msg.IntVal(0);
	printf("OnExtraMessage:%d\n", nIndex );
	for( size_t i = 1; i < nCount ; ++i)
	{
		if ( msg.GetType(i) == VTYPE_INT )
		{
			printf("��%d������:%d(int)\n", (int)i, msg.IntVal(i) );
		}
		else if ( msg.GetType(i) == VTYPE_STRING )
		{
			printf("��%d������:%s(string)\n", (int)i, msg.StringVal(i) );
		}
		else
		{
			printf("��%d������:(other)\n", (int)i );
		}
	}
#endif
	return 0;
}

// ��ʼ��
bool PlayerBaseModule::Init(IKernel* pKernel)
{
	//��ʼ���������
	srand((int)util_get_ticks());
	m_pPlayerBaseModule = this;

	Assert( m_pPlayerBaseModule != NULL );

	// �汾������ִ�к���
	m_funList.resize(PLAYER_VERSION_MAX, NULL);

// 	RegisterVersionFun(PLAYER_PACK_UPDATE, UpdatePack);     // ������ұ���
//     RegisterVersionFun(PLAYER_PACK_ITEM_ASSIGN, AssignPackItem);



	pKernel->AddClassCallback("player", "OnCreateClass", OnCreateClass);
	pKernel->AddEventCallback("player", "OnEntry", PlayerBaseModule::OnEntry, -1);
	pKernel->AddEventCallback("player", "OnEntryGame", PlayerBaseModule::OnEntryGame, -1);
	pKernel->AddEventCallback("player", "OnContinue", PlayerBaseModule::OnContinue, -1);
	pKernel->AddEventCallback("player", "OnRecover", PlayerBaseModule::OnRecover, 1000);
	pKernel->AddEventCallback("player", "OnStore", PlayerBaseModule::OnStore);
	pKernel->AddEventCallback("player", "OnDisconnect", PlayerBaseModule::OnDisconnect, 1000);
	pKernel->AddEventCallback("player", "OnSelect", PlayerBaseModule::OnPlayerSelect); 
	pKernel->AddEventCallback("npc", "OnSelect", PlayerBaseModule::OnPlayerSelect);

	pKernel->AddEventCallback("player", "OnStore", PlayerBaseModule::OnStore2, 50000);
	pKernel->AddEventCallback("player", "OnLeave", PlayerBaseModule::OnLeave, 50000);

	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_GET_SERVERTIME, PlayerBaseModule::OnCustomGetServerTime);
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_CHECKED_SELECTROLE, PlayerBaseModule::OnCheckedSelectRole);
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_TOKEN, PlayerBaseModule::GetToken);
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_SET_VISUAL_PLAYERS, PlayerBaseModule::OnCustomSetVisualPlayers);
    //�ͻ���ת����̨
    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_BACKGROUND, PlayerBaseModule::OnClientBackgroundTask);

	//pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_RECORD_PLAY_CG_SCENE, PlayerBaseModule::OnCustomRecordPlayCGScene);
	// ��ȡ�����������
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_GET_PLAYER_PROP_DATA, PlayerBaseModule::OnCustomGetPlayerPropData);

    // ��ұ��˺�
	pKernel->AddIntCommandHook("player", COMMAND_BEDAMAGE, PlayerBaseModule::OnCommandBeDamage);

	// �������
//	pKernel->AddIntCommandHook("player", COMMAND_BLOCK_PLAYER, PlayerBaseModule::OnCommandBlockPlayer);
	// ��ȡ�����������
	pKernel->AddIntCommandHook("player", COMMAND_CLIENT_QUERY_PROP_DATA, PlayerBaseModule::OnCommandClientQueryPropData);

	bool bOK = pKernel->AddEventCallback("scene", "OnExtraMessage", OnExtraMessage );

	DECL_HEARTBEAT(PlayerBaseModule::CB_ForceOffline);

#ifndef FSROOMLOGIC_EXPORTS
	DECL_RESET_TIMER(RESET_ROLE_LOGIN_TIME, PlayerBaseModule::ResetLoginTimeTimer);

	DECL_LUA_EXT(nx_exchange_gift);
#endif

    // GM�����������λ�õ��ʳ�
	DECL_LUA_EXT(nx_reset_player_position);

	// TODO: �ɲ�ѯ�����ֶ���ʱֱ��д��
// 	m_propPubset.insert( FIELD_PROP_BATTLE_ABILITY );
// 	m_propPubset.insert(FIELD_PROP_TEAM_ID);
// 	m_propPubset.insert(FIELD_PROP_TEAM_CAPTAIN);
// 	m_propPubset.insert(FIELD_PROP_GUILD_NAME);
// 	m_propPubset.insert(FIELD_PROP_GUILD_POSITION);
//	m_propPubset.insert(FIELD_PROP_NATION);
	m_propPubset.insert(FIELD_PROP_NAME);
	return true;
}

// �ͷ�
bool PlayerBaseModule::Shut(IKernel* pKernel)
{
	return true;
}

//����
int PlayerBaseModule::OnCreateClass(IKernel* pKernel, int index)
{
#ifndef FSROOMLOGIC_EXPORTS
	createclass_from_xml(pKernel, index, "struct\\player\\player.xml");
#else
	createclass_from_xml(pKernel, index, "room_struct\\player\\player.xml");
#endif // FSROOMLOGIC_EXPORTS
	return 0;
}

int PlayerBaseModule::OnEntry(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	PERSISTID scene = pKernel->GetScene();
	if (!pKernel->Exists(scene))
	{
		return 0;
	}

	// ��������
	IGameObj *pScene = pKernel->GetGameObj(scene);
	if (NULL == pScene)
	{
		return 0;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	//���ÿ��ӷ�Χ
	float fVisualRange = 50.0f;

	if (pScene->FindAttr("VisualRange"))
	{
		float visual_range = pScene->QueryFloat("VisualRange");

		fVisualRange = (visual_range < 50.0f) ? visual_range : 50.0f;
	}

	if (pPlayer->FindAttr("VisualRange"))
	{
		pPlayer->SetFloat("VisualRange", fVisualRange);	
	}

// 	CVarList extraMsg;
// 	extraMsg << "extra" << "player entry" << 10 << 10.5;
// 	pKernel->SendExtraMessage( 8, extraMsg );

	return 0;
}

//�������
int PlayerBaseModule::OnRecover(IKernel* pKernel, const PERSISTID& self, 
								const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return 0;
	}

	//����HPʱ��
	int64_t resetTime = static_cast<int64_t>(EnvirValueModule::EnvirQueryInt(ENV_VALUE_RESET_MAX_HP_TIME));
	int64_t logoutTime = pSelf->QueryInt64(FIELD_PROP_ROLE_LOGOUT_TIME);
    
	//Ѫ������
	int64_t maxHP = pSelf->QueryInt64("MaxHP");
	int64_t iLastHP = pSelf->QueryInt64("TempHP");

	//ʱ���ж��Ƿ�����HP
	if( ::time(NULL) - logoutTime >= resetTime )
	{	
		pSelf->SetInt64("HP", maxHP);
	}
	else
	{
		pSelf->SetInt64("HP", iLastHP > maxHP ? maxHP : iLastHP);
	}

	float fWalkStep = EnvirValueModule::EnvirQueryFloat(ENV_VALUE_PLAYER_WALK_STEP_DEFAULT);
	pSelf->SetFloat("WalkStep", fWalkStep);
	pSelf->SetFloat("CollideRadius", 1.0f);// �������������ײ�뾶
	pSelf->SetInt("MapFree", 1);// ����ʱ���ܳ�������

	// �ж��ǲ���ͬһ��
	int64_t nLastLoginTime_ = pSelf->QueryInt64(FIELD_PROP_ROLE_LOGIN_TIME);
	// �жϵ�ǰ�Ƿ��Ѿ���¼����
	int64_t tNow_ = util_get_utc_time();
	int nLoginDays_ = pSelf->QueryInt(FIELD_PROP_LOGIN_DAYS);
	// ���ߵǼǹ���
	if (util_is_sameday(nLastLoginTime_, tNow_))
	{
		if (nLoginDays_ <= 0)
		{
			pSelf->SetInt(FIELD_PROP_LOGIN_DAYS, 1);
		}
	}
	else
	{
		pSelf->SetInt(FIELD_PROP_LOGIN_DAYS, nLoginDays_ + 1);
	}

	pSelf->SetInt64("RoleLoginTime", tNow_);
	//Ԥ�������ͬ����ʾ�������ͻ����Զ�����Ϣ�ٴ�����
	pSelf->SetInt("VisualPlayers", 5);
	pSelf->SetInt("MaxVisuals", 5);

	// ���÷�����ID
#ifdef FSROOMLOGIC_EXPORTS

	int serverId = pKernel->GetRoomId();

#else

	int serverId = pKernel->GetServerId();

#endif
	

	pSelf->SetInt("ServerID", serverId);
	
#ifndef FSROOMLOGIC_EXPORTS
	// ע�����0����µ�¼ʱ��
	REGIST_RESET_TIMER(pKernel, self, RESET_ROLE_LOGIN_TIME);

	// �������
	LogModule::m_pLogModule->OnRoleEntry(pKernel, self);
#endif // FSROOMLOGIC_EXPORTS
	return 0;
}

//��Ҷ�������
int PlayerBaseModule::OnContinue(IKernel* pKernel, const PERSISTID& self, 
								const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ��������uid
	PlayerBaseModule::m_pPlayerBaseModule->UpdateAccountUID(pKernel, self);
	return 0;
}

//��Ҷ������������ص��ǻ���ʱ��10��(����ص�ֻ����һ��)
int PlayerBaseModule::OnDisconnect(IKernel* pKernel, const PERSISTID& self, 
								   const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pPlayerObj = pKernel->GetGameObj(self);
	if (!pPlayerObj)
	{
		return 0;
	}

#ifndef FSROOMLOGIC_EXPORTS
   if (pPlayerObj->FindData("CB_DisnectProtect"))
   {
	   int nProTime = pPlayerObj->QueryDataInt("CB_DisnectProtect");
	   return nProTime;
   }
#endif

	if (pPlayerObj->FindData("OnNormalExitGame") &&
		pPlayerObj->QueryDataInt("OnNormalExitGame") == 1)
	{
		return 0;
	}

	//��ͨ�ؾ����߱���300s
	int nScene = pKernel->GetSceneId();
	const char* name = pKernel->GetSceneScript(nScene);
	if (strcmp(name, "SecretScene") == 0)
	{
		return 300;
	}

	//��������60s����
	return 60;
}

//�������
int PlayerBaseModule::OnStore(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	// ��¼��ҵǳ�ʱ��
	pPlayer->SetInt64(FIELD_PROP_ROLE_LOGOUT_TIME, ::time(NULL));
	
	const int type = args.IntVal(0);
	if (STORE_EXIT == type)
	{
		//use_role_rec	
		if (pKernel->Type(self) == TYPE_PLAYER)
		{
			time_t start = pPlayer->QueryInt64("RoleLoginTime");
			if (start <= 0)
			{
				return 0;
			}
			time_t now = ::time(NULL);
			if (now <= 0)
			{
				return 0;
			}
			const char* address = pPlayer->QueryString("Token");
			const char* ClientType = pPlayer->QueryString("ClientType");

#ifndef FSROOMLOGIC_EXPORTS
			//�����ɫ��¼��־modify by SunJian(2015-04-23)
			RoleLog log;
			log.loginTime        = start;
			log.logoutTime       = now;
			log.ip               = pPlayer->QueryString("Address");
			log.mac              = address;
			log.osType           = ClientType;
			LogModule::m_pLogModule->SaveRoleLog(pKernel, self, log);

			// �������
			LogModule::m_pLogModule->OnRoleExit(pKernel, self);
#endif // FSROOMLOGIC_EXPORTS
		}

	}

	return 0;
}

int PlayerBaseModule::OnStore2(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	const int type = args.IntVal(0);
	if (STORE_EXIT != type)
	{
		return 0;
	}

	IGameObj* pPlayerObj = pKernel->GetGameObj(self);
	if (!pPlayerObj)
	{
		return 0;
	}

	if (pPlayerObj->FindData("TempStoreTime"))
	{
		CVarList msg;
		msg << (int)SERVER_CUSTOMMSG_SWITCH_ROLE << (int)EMSG_SWITCH_STATUS_1;
		pKernel->Custom(self, msg);

		int64_t t = util_get_time_64() - pPlayerObj->QueryDataInt64("TempStoreTime");
		if (t < 1000)
		{
			return 0;
		}
		char strtime[256] = { 0 };
		time_t ttime = time(NULL);
		util_convert_time_to_string(&ttime, strtime, sizeof(strtime));

		::extend_warning(LOG_ERROR, "[ERROR][%s]: LeaveGame to OnStore End:[%d]", strtime, t);
	}

	return 0;
}

int PlayerBaseModule::OnLeave(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pPlayerObj = pKernel->GetGameObj(self);
	if (!pPlayerObj)
	{
		return 0;
	}

	if (pPlayerObj->FindData("TempStoreTime"))
	{
		CVarList msg;
		msg << (int)SERVER_CUSTOMMSG_SWITCH_ROLE << (int)EMSG_SWITCH_STATUS_2;
		pKernel->Custom(self, msg);

		int64_t t = util_get_time_64() - pPlayerObj->QueryDataInt64("TempStoreTime");
		if (t < 3000)
		{
			return 0;
		}
		char strtime[256] = { 0 };
		time_t ttime = time(NULL);
		util_convert_time_to_string(&ttime, strtime, sizeof(strtime));

		::extend_warning(LOG_ERROR, "[ERROR][%s]: LeaveGame to OnLeave End:[%d]", strtime, t);
	}

	return 0;
}

// ��ҽ�����Ϸ
int PlayerBaseModule::OnEntryGame(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}
	
	// ��������uid
	PlayerBaseModule::m_pPlayerBaseModule->UpdateAccountUID(pKernel, self);

	return 0;
}

// ��������uid
void PlayerBaseModule::UpdateAccountUID(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return;
	}

	if (pKernel->Type(self) != TYPE_PLAYER 
		|| pSelfObj->QueryInt("Online") != ONLINE)
	{
		return;
	}

	// ����UID
	// $#648_47579443
	if (StringUtil::CharIsNull(pSelfObj->QueryString("AccountUID")))
	{
		std::string account = pSelfObj->QueryString("Account"); 
		if (!account.empty())
		{
			size_t pos = account.rfind("_");
			if (pos != std::string::npos)
			{
				// ȡuid��ʼλ�úͳ���
				size_t startPos = pos + strlen("_");
				int accountSize = static_cast<int>(account.size() - startPos);

				// ��ȡuid
				std::string uid = account.substr(startPos, accountSize);
				pSelfObj->SetString("AccountUID", uid.c_str());
			}
		}
	}
}
int PlayerBaseModule::OnPlayerSelect(IKernel* pKernel, const PERSISTID& self,
									 const PERSISTID& sender, const IVarList& args)
{
	// ��Ҷ���
	IGameObj *pObj = pKernel->GetGameObj(sender);
	if (NULL == pObj)
	{
		return 0;
	}

	int function_id = args.IntVal(0);

	if (function_id == 0)
	{
		return 0;
	}

	if(function_id > 10)
	{
		return 0;
	}
	else if(function_id == 1)
	{
		PERSISTID cur_co = pObj->QueryObject("LastObject");

		if(cur_co != self)
		{
			pObj->SetObject("LastObject",self);
		}
	}

	return 0;
}

//������ʱ���
int PlayerBaseModule::OnCustomGetServerTime(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	CVarList arg_list;
	arg_list << SERVER_CUSTOMMSG_SERVER_TIME << time(NULL) << ::util_get_time_64();
	pKernel->Custom( self, arg_list );
	return 0;
}

// ��С�ˣ����ؽ�ɫѡ��
int PlayerBaseModule::OnCheckedSelectRole(IKernel* pKernel, const PERSISTID& self,
										  const PERSISTID& player, const IVarList& args)
{
#ifndef FSROOMLOGIC_EXPORTS
	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

    //����ӳ����ߣ����ս��
    if (pPlayer->FindData("CB_DisnectProtect"))
    {
        if(pPlayer->QueryDataInt("CB_DisnectProtect") > 0)
        {
            //::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, CROSSBATTLE_MATCHBATTLE_NO_EXIT_ROLE, CVarList());
            return 0;
        }
    }

	if (!pPlayer->FindData("OnNormalExitGame"))
	{
		pPlayer->AddDataInt("OnNormalExitGame", 1);
	}
	else
	{
		pPlayer->SetDataInt("OnNormalExitGame", 1);
	}
	ADD_DATA_INT64(pPlayer, "TempStoreTime");
	pPlayer->SetDataInt64("TempStoreTime", util_get_time_64());

	// ��¼�л���ɫ��Ϣʱ�� [3/25/2017 lihailuo]
	::extend_warning( LOG_INFO, "player '%s(%s)' check back select role[timestamp:%lld]",
						pPlayer->QueryString(FIELD_PROP_UID), 
						StringUtil::WideStrAsString(pPlayer->QueryWideStr(FIELD_PROP_NAME)),
						util_get_time_64() );

	CVarList msg;
	msg << (int)SERVER_CUSTOMMSG_SWITCH_ROLE << (int)EMSG_SWITCH_ROLE_REPLY;
	if (pKernel->PlayerLeaveGame(self))
	{
		msg  << (int)EM_SWITCH_ROLE_SUCCEED;
	}
	else
	{
		msg << (int)EM_SWITCH_ROLE_FAILED;
	}
	pKernel->Custom(self, msg);
#endif
	return 0;
}

//��ȡ�豸��
int PlayerBaseModule::GetToken(IKernel* pKernel,const PERSISTID& self,const PERSISTID& sender,const IVarList& args)
{
	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	int nIndex = 1;
	const char* clientType = args.StringVal(nIndex++);
	const char* token = args.StringVal(nIndex++);
	const wchar_t* pServerName = args.WideStrVal(nIndex++);
	const char* AppPackage = args.StringVal(nIndex++);

	const char* platform = args.StringVal(nIndex++);
	bool bwifi = args.IntVal(nIndex++) != 0;
	const char* manufacturer = args.StringVal(nIndex++);
	const char* devid = args.StringVal(nIndex++);
	const char* model = args.StringVal(nIndex++);
	const char* os = args.StringVal(nIndex++);
	const char* os_ver = args.StringVal(nIndex++);
	const char* client_ver = args.StringVal(nIndex++);

    const char* account = pPlayer->QueryString("Account");
	if (StringUtil::CharIsNull(clientType)
		|| StringUtil::CharIsNull(token)
		|| StringUtil::CharIsNull(account)
		|| StringUtil::CharIsNull(pServerName)
		|| StringUtil::CharIsNull(AppPackage))
	{
		return 0;
	}

    pPlayer->SetString("ClientType", clientType);
    pPlayer->SetString("Token", token);
	pPlayer->SetWideStr("ServerName", pServerName);
	pPlayer->SetString("AppPackage", AppPackage);

#ifndef FSROOMLOGIC_EXPORTS
	const char* uid = pPlayer->QueryString(FIELD_PROP_UID);
	const char* ip = pPlayer->QueryString(FIELD_PROP_ADDRESS);

	if (!pPlayer->FindData("TempLoginPlatform"))
	{
		pPlayer->AddDataString("TempLoginPlatform", platform);
	}
	else
	{
		pPlayer->SetDataString("TempLoginPlatform", platform);
	}

	if (!pPlayer->FindData("TempLoginModel"))
	{
		pPlayer->AddDataString("TempLoginModel", model);
	}
	else
	{
		pPlayer->SetDataString("TempLoginModel", model);
	}

	if (!pPlayer->FindData("TempLoginManufacturer"))
	{
		pPlayer->AddDataString("TempLoginManufacturer", manufacturer);
	}
	else
	{
		pPlayer->SetDataString("TempLoginManufacturer", manufacturer);
	}

	if (!pPlayer->FindData("TempLoginDeviceId"))
	{
		pPlayer->AddDataString("TempLoginDeviceId", devid);
	}
	else
	{
		pPlayer->SetDataString("TempLoginDeviceId", devid);
	}
	LogModule::m_pLogModule->OnClientInfo(pKernel, self, ip, platform, client_ver, manufacturer, devid, model, os, os_ver, bwifi);

	CVarList msg_list;
	msg_list << "Token" << token 
		<< "ClientType" << clientType 
		<< "Account" << account 
		<< "ServerName" << pServerName
		<< "AppPackage" << AppPackage;
    SnsDataModule::SavePlayerAttr(pKernel, self, msg_list);

	//֪ͨPub����
// 	CVarList pubMsg;
// 	pubMsg << PUBSPACE_DOMAIN
// 		<< SnsPushModule::m_pSnsPushModule->GetDomainName(pKernel).c_str()
// 		<< SP_DOMAIN_MSG_MSG_PUSH_INFO
// 		<< account
// 		<< pKernel->SeekRoleUid(pPlayer->QueryWideStr("Name"))
// 		<< pPlayer->QueryInt("Nation")
// 		<< clientType
// 		<< token
// 		<< pServerName
// 		<< AppPackage;
// 	pKernel->SendPublicMessage(pubMsg);
#endif // FSROOMLOGIC_EXPORTS

	return 0;
}

//����ͬ����ʾ����
//msg��ʽ��
//       [msgid][visual_player_num]
int PlayerBaseModule::OnCustomSetVisualPlayers( IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args )
{
#ifndef FSROOMLOGIC_EXPORTS
	if (args.GetCount() < 2 ||
		args.GetType(1) != VTYPE_INT)
	{
		//�ͻ����Ϸ�����Ϣ����������
		return 0;
	}

	// �����Ҳ�����
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	int visual_player_num = args.IntVal(1);

	//ͬ����ʾ����50������10
	if(visual_player_num < 10)
	{
		visual_player_num = 10;
	}

	if(visual_player_num > 50)
	{
		visual_player_num = 50;
	}

	pPlayer->SetInt("VisualPlayers", visual_player_num);

	pPlayer->SetInt("MaxVisuals", visual_player_num + MAX_VISUALS_ADD);

	// ��Ӫս������Ӫ������Ұ
	//BattleHelperS::Instance()->ResetCampVisual(pKernel, self);

#endif // FSROOMLOGIC_EXPORTS

	return 0;
}

// ��Ӧ���˺��¼�
int PlayerBaseModule::OnCommandBeDamage(IKernel* pKernel, const PERSISTID& self,
							 const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	IGameObj* pSenderObj = pKernel->GetGameObj(sender);
	if (NULL == pSelfObj || NULL == pSenderObj)
	{
		return 0;
	}

	// ����Ϊ�������
	if (TYPE_PLAYER == pSelfObj->GetClassType() && 1 == pSelfObj->QueryInt("Online"))
	{
		// Ĭ��Ϊ������
		PERSISTID prior = sender;
		
		// ����npc,����Ϊʩ����,����,����Ϊ����
		const char* strScript = pSenderObj->GetScript();
		if (0 == strcmp(strScript, "TrapNpc") || 0 == strcmp(strScript, "PetNpc"))
		{
			prior = pSenderObj->QueryObject("Master");
		}

		// ������ʾ������
		if (pKernel->Exists(prior))
		{
			pKernel->AddVisualPriority(self, sender, true);
		}
	}
	return 0;
}

// 0����µ�¼ʱ��
int PlayerBaseModule::ResetLoginTimeTimer(IKernel* pKernel, const PERSISTID& self, int slice)
{
	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

#ifndef FSROOMLOGIC_EXPORTS
	// �жϵ�½ʱ���Ƿ�ͬһ��
	int64_t nLastLoginTime_ = pPlayer->QueryInt64(FIELD_PROP_ROLE_LOGIN_TIME);
	// �жϵ�ǰ�Ƿ��Ѿ���¼����
	int64_t tNow_ = util_get_utc_time();
	// ���ߵǼǹ���
	if (!util_is_sameday(nLastLoginTime_, tNow_))
	{
		// add login days
		int nLoginDays_ = pPlayer->QueryInt(FIELD_PROP_LOGIN_DAYS);
		pPlayer->SetInt(FIELD_PROP_LOGIN_DAYS, nLoginDays_ + 1);


		// ��ʱ���½�ɫ��¼��־(0�����)
		RoleLog log;
		log.loginTime = pPlayer->QueryInt64(FIELD_PROP_ROLE_LOGIN_TIME);
		log.logoutTime = ::time(NULL);
		log.ip = pPlayer->QueryString("Address");
		log.mac = pPlayer->QueryString("Token");
		log.osType = pPlayer->QueryString("ClientType");
		LogModule::m_pLogModule->SaveRoleLog(pKernel, self, log);


		// �������õ�¼ʱ��
		pPlayer->SetInt64(FIELD_PROP_ROLE_LOGIN_TIME, ::time(NULL));
		// ��ɫ�˳�
		LogModule::m_pLogModule->OnRoleExit(pKernel, self, true);
		// �û��˳�
		const char* pszAccId = pPlayer->QueryString(FIELD_PROP_ACCOUNT);
		LogModule::m_pLogModule->OnUserLogout(pszAccId, 0, true);

		const char* pszPlatform = "";
		const char* pszModel = "";
		const char* pszManu = "";
		const char* pszDeviceId = "";

		if (pPlayer->FindData("TempLoginPlatform"))
		{
			pszPlatform = pPlayer->QueryDataString("TempLoginPlatform");
		}

		if (pPlayer->FindData("TempLoginModel"))
		{
			pszModel = pPlayer->QueryDataString("TempLoginModel");
		}

		if (pPlayer->FindData("TempLoginManufacturer"))
		{
			pszManu = pPlayer->QueryDataString("TempLoginManufacturer");
		}

		if (pPlayer->FindData("TempLoginDeviceId"))
		{
			pszDeviceId = pPlayer->QueryDataString("TempLoginDeviceId");
		}

		// �û�����
		LogModule::m_pLogModule->OnUserLogin(pszAccId, pszAccId, log.ip.c_str(), pszPlatform, pszManu, pszDeviceId, pszModel, log.osType.c_str(), "", true);
		// ��ɫ����
		LogModule::m_pLogModule->OnRoleEntry(pKernel, self, true);
	}

//	ShopModule::m_pShopModule->OnResetPayedProducts(pKernel, self);
#endif // FSROOMLOGIC_EXPORTS
	return 0;
}

// 
// int PlayerBaseModule::OnCustomCheckStrength(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
// {
// 
// 	if (!pKernel->Exists(self))
// 	{
// 		return 0;
// 	}
// 
// 	if (args.GetCount() < 1)
// 	{
// 		return 0;
// 	}
// 
// 	IGameObj* player = pKernel->GetGameObj(self);
// 	if (NULL == player)
// 	{
// 		return 0;
// 	}
// 
// 	//���װ��ս��
// 	const int equipability = player->QueryInt("EquipBattleAbility");
// 	//��ó���ӵ��������ս��
// 	//const int petability = player->QueryInt("PetBattleAbility");
// 	//�������ս��
// 	const int rideability = player->QueryInt("RideBattleAbility");
// 	//��ü���ս��
// 	const int skillability = player->QueryInt("SkillBattleAbility");
// 	//��þ���ս��
// 	const int officelevability = player->QueryInt("OfficeLevBattleAbility");
// 
// 	//��ñ�ʯ,����ս����,ǿ��ս����
// 	int jewelability = 0;
// 	int soulability = 0;
// 	int strengthability = 0;
// 
// 	IRecord* battle_ability_record = player->GetRecord(EQUIP_BATTLE_ABILITY_REC);
// 	if (battle_ability_record == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	int row_count = battle_ability_record->GetRows();
// 
// 	LoopBeginCheck(a);
// 	for (int row = 0; row < row_count; ++row)
// 	{
// 		LoopDoCheck(a);
// 		int equip_ability = battle_ability_record->QueryInt(row, EQUIP_BA_EQUIPMENT);
// 		if (equip_ability == -1)
// 		{
// 			// -1 ��ʾ����λû��װ��
// 			continue;
// 		}
// 
// 		// �鿴��һ���Ƿ��ŵ���������ս����, �����װ��ս����Ҳ��¼����һ��
// 		int equip_index = battle_ability_record->QueryInt(row, EQUIP_BA_EQUIP_POS);
// 		if (equip_index == EQUIP_POS_WEAPON)
// 		{
// 			soulability += battle_ability_record->QueryInt(row, EQUIP_BA_SOUL_CLOTH);
// 		}
// 
// 		// �ۼ�����ս����
// 		soulability += battle_ability_record->QueryInt(row, EQUIP_BA_SOUL);
// 
// 		// �ۼ�ǿ��ս����
// 		strengthability += battle_ability_record->QueryInt(row, EQUIP_BA_STRHENTHEN);
// 
// 		// �ۼӱ�ʯս����
// 		LoopBeginCheck(b);
// 		for (int col = EQUIP_BA_JEWEL1; col <= EQUIP_BA_JEWEL4; ++col)
// 		{
// 			LoopDoCheck(b);
// 			jewelability += battle_ability_record->QueryInt(row, col);
// 		}
// 	}
// 
// 	CVarList msg;
// 	msg << SERVER_CUSTOMMSG_CHECK_STRENGTH
// 		<< equipability
// 		<< rideability
// 		<< skillability
// 		<< officelevability
// 		<< jewelability
// 		<< soulability
// 		<< strengthability;
// 
// 	pKernel->Custom(self, msg);
// 
// 	return 0;
// }

int PlayerBaseModule::OnClientBackgroundTask(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
    CVarList msg;
    msg << SERVER_CUSTOMMSG_BACKGROUND;
    pKernel->Custom(self, msg);

    return 0;
}

// �����ɫ
int PlayerBaseModule::OnCommandBlockPlayer(IKernel* pKernel, const PERSISTID& self,
										const PERSISTID& sender, const IVarList& args)
{
	// ��ȫ��֤
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ȡʱ��
	int64_t time = args.Int64Val(1);

	// �����ɫ
	BlockPlayer(pKernel, self, time);

	return 0;
}

int PlayerBaseModule::OnCommandClientQueryPropData(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pPlayer_ = pKernel->GetGameObj(self);
	if (NULL == pPlayer_)
	{
		return 0;
	}

	// ͨ������ת�� ��ʽ: ������ �ͻ��˶����ʶ �����б�....
	Assert(args.GetCount() >= 4);
	const wchar_t* pwszSender_ = args.WideStrVal(1);
	int nud_ = args.IntVal(2);

	m_pPlayerBaseModule->HandlerPlayerPropQuery(pKernel, pwszSender_, *pPlayer_, args, 3, nud_);
	return 0;
}

// ����������ҽ�ɫ
bool PlayerBaseModule::BlockPlayer(IKernel* pKernel, const PERSISTID& self, int64_t time)
{
	// ��ȡ��ɫ����
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ��¼���ʱ���
	if (pSelfObj->FindData("BlockPlayerTime"))
	{
		pSelfObj->SetDataInt64("BlockPlayerTime", time);
	}
	else
	{
		pSelfObj->AddDataInt64("BlockPlayerTime", time);
	}

	// ֪ͨ�ͻ��˲�Ҫ��������
	//pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_CONNECT_CLOSE << STR_SYSINFO_HEART_BEAT_ERROR);

	// �����������
	ADD_COUNT_BEAT(pKernel, self, "PlayerBaseModule::CB_ForceOffline", FORCE_OFFLINE_TIME, 1);

	return true;
}

// ������
int PlayerBaseModule::CB_ForceOffline(IKernel* pKernel, const PERSISTID& self, int time)
{
	// ��ȡ��ɫ����
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ��ȡ���ʱ���
	int64_t blockTime = 0;
	if (pSelfObj->FindData("BlockPlayerTime"))
	{
		blockTime = pSelfObj->QueryDataInt64("BlockPlayerTime");
	}

	// ʱ����
	time_t tNow = ::time(NULL);
	if (blockTime <= 0)
	{
		blockTime = tNow;
	}

	tm structTime;
	util_get_local_time(&blockTime, &structTime);

#ifndef FSROOMLOGIC_EXPORTS
	// ǿ������
	bool bOk = pKernel->BlockPlayer(pSelfObj->QueryWideStr("Name"), 
		structTime.tm_year + 1900, 
		structTime.tm_mon + 1, 
		structTime.tm_mday, 
		structTime.tm_hour, 
		structTime.tm_min, 
		structTime.tm_sec);

	if (!bOk)
	{
		return 0;
	}

	if (time > tNow)
	{
		LogModule::m_pLogModule->OnBlockPlayer(pKernel, self, time - tNow, time);
	}
	else
	{
		LogModule::m_pLogModule->OnResumePlayer(pKernel, self);
	}

	// ���ÿ��������ǰ��ɫ����
	if (!pSelfObj->FindData("OnNormalExitGame"))
	{
		pSelfObj->AddDataInt("OnNormalExitGame", 1);
	}
	else
	{
		pSelfObj->SetDataInt("OnNormalExitGame", 1);
	}
	pKernel->BreakPlayer(self);


#endif

	return 0;
}

// �������ɫ time=0 ���
bool PlayerBaseModule::BlockPlayer(IKernel* pKernel, const wchar_t* playerName, int64_t time)
{
	// ��ȫ���
	if (StringUtil::CharIsNull(playerName))
	{
		return false;
	}

	// ����Ƿ�����
	if (pKernel->GetPlayerScene(playerName) <= 0)
	{
		// ʱ����
		time_t tNow = ::time(NULL);
		if (time <= 0)
		{
			time = tNow;
		}

		// ��Ҳ�����
		tm structTime;
		util_get_local_time(&time, &structTime);

#ifndef FSROOMLOGIC_EXPORTS
		// ���ö���ʱ��
		bool bOK = pKernel->BlockPlayer(playerName, 
			structTime.tm_year + 1900, 
			structTime.tm_mon + 1, 
			structTime.tm_mday, 
			structTime.tm_hour, 
			structTime.tm_min, 
			structTime.tm_sec);
		if (bOK)
		{
			const char* pszRoleId = pKernel->SeekRoleUid(playerName);
			if ( time > tNow )
			{
				LogModule::m_pLogModule->OnBlockPlayer(pKernel, pszRoleId, time - tNow, time);
			}
			else
			{
				LogModule::m_pLogModule->OnResumePlayer(pKernel, pszRoleId );
			}
		}
#endif
	}
// 	else
// 	{
// 		// �������
// 		pKernel->CommandByName(playerName, CVarList() << COMMAND_BLOCK_PLAYER << time);
// 	}

	return true;
}

bool PlayerBaseModule::IsRobot(IKernel *pKernel, const PERSISTID &player)
{
#ifndef FSROOMLOGIC_EXPORTS
	// �ų�������
	if (pKernel->Find(player, "PrepareRole"))
	{
		int role_flag = pKernel->QueryInt(player, "PrepareRole");
		if (role_flag == ROLE_FLAG_ROBOT)
		{
			return true;
		}
	}
#endif
	return false;
}

int PlayerBaseModule::OnCustomGetPlayerPropData(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (args.GetCount() < 2)
	{
		return 0;
	}

	IGameObj* pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	// ��Ҷ���
	const wchar_t* pwszSender_ = pPlayer->QueryWideStr( FIELD_PROP_NAME);
	int nud_ = args.IntVal(1);
	const wchar_t* pwszTarget_ = args.WideStrVal(2);

	if (StringUtil::CharIsNull(pwszSender_) || StringUtil::CharIsNull(pwszTarget_))
	{
		// ��������
		return 0;
	}

	if ( wcscmp(pwszSender_, pwszTarget_) == 0 )
	{
		m_pPlayerBaseModule->HandlerPlayerPropQuery(pKernel, pwszSender_, *pPlayer, args, 3, nud_);
	}
	else
	{
		// ����Ƿ��ڵ�ǰ����
		PERSISTID& p_ = pKernel->FindPlayer(pwszTarget_);
		IGameObj* pTarget_ = pKernel->GetGameObj(p_);
		if (NULL != pTarget_)
		{
			m_pPlayerBaseModule->HandlerPlayerPropQuery(pKernel, pwszSender_, *pTarget_, args, 3, nud_);
		}
		else
		{
			// ����Ƿ�����
			int nScene_ = pKernel->GetPlayerScene(pwszTarget_);
			if (nScene_ > 0)
			{
				// ͨ������ת�� ��ʽ: ������ �ͻ��˶����ʶ �����б�....
				CVarList cmd_;
				cmd_ << COMMAND_CLIENT_QUERY_PROP_DATA << pwszSender_ << nud_;
				cmd_.Append(args, 3, args.GetCount() - 3);
				pKernel->CommandByName(pwszTarget_, cmd_);
			}
			else
			{
				// ֱ�ӷ���ʧ��
				CVarList retMsg_;
				retMsg_ << SERVER_CUSTOMMSG_GET_PLAYER_PROP_RSP << EM_PROP_QUERY_RESULT_OFFLINE << nud_ << pwszTarget_;
			}
		}
	}
	return 0;
}

//ע����Ұ汾�ź���
bool PlayerBaseModule::RegisterVersionFun(PlayerVersion version, PLAYER_VERSION_FUNC fun)
{
	if(version < 0 || version >= PLAYER_VERSION_MAX)
	{
		return false;
	}
	if(fun == NULL)
	{
		return false;
	}
	m_funList[version] = fun;

	return true;
}

void PlayerBaseModule::HandlerPlayerPropQuery(IKernel* pKernel, const wchar_t* pszSender, IGameObj& player, const IVarList& query, int nOffset, int nUID)
{
	CVarList msg;
	msg << SERVER_CUSTOMMSG_GET_PLAYER_PROP_RSP << EM_PROP_QUERY_RESULT_SUCCEED << nUID << player.QueryWideStr(FIELD_PROP_NAME);

	int nCount = query.GetCount();
	for (int i = nOffset; i < nCount; ++i)
	{
		const char* pszProp = query.StringVal(i);
		// �Ȳ��Ƿ��ڹ����б���(TODO: �Ժ�ͨ�����ö���)
		if (m_propPubset.find(pszProp) == m_propPubset.end())
		{
			continue;
		}

		if ( player.FindAttr(pszProp) )
		{
			int nType = player.GetAttrType(pszProp);
			msg << pszProp << nType;
			switch (nType)
			{
			case VTYPE_INT:		// 32λ����
			case VTYPE_BOOL:	// ����
				msg << player.QueryInt(pszProp);
				break;
			case VTYPE_INT64:	// 64λ����
				msg << player.QueryInt64(pszProp);
				break;
			case VTYPE_FLOAT:	// �����ȸ�����
				msg << player.QueryFloat(pszProp);
				break;
			case VTYPE_DOUBLE:	// ˫���ȸ�����
				msg << player.QueryDouble(pszProp);
				break;
			case VTYPE_STRING:	// �ַ���
				msg << player.QueryString(pszProp);
				break;
			case VTYPE_WIDESTR:	// ���ַ���
				msg << player.QueryWideStr(pszProp);
				break;
			case VTYPE_OBJECT:	// �����
				msg << player.QueryObject(pszProp);
				break;
			default:
				break;
			}
		}
	}

	pKernel->CustomByName(pszSender, msg);
}

// ������Ұ汾�������߼�
bool PlayerBaseModule::HandlePlayerVersion(IKernel *pKernel, const PERSISTID &self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if( pSelfObj == NULL )
	{
		return false;
	}

	// ��ҵ�ǰ�汾��
	PlayerVersion version = (PlayerVersion)pSelfObj->QueryInt("PlayerVersion");

	if( version < 0 || version >= PLAYER_VERSION_MAX )
	{
		return true;
	}
	if( m_funList.size() == 0 )
	{
		return true;
	}

	// �汾����ʱ����
	int tempVersion = version;

	bool isUpVersion = false;

	LoopBeginCheck(a);
    for ( int i = 0; i < PLAYER_VERSION_MAX; ++i )
	{
		LoopDoCheck(a);
		// С�ڵ�ǰ�汾�Ĳ�����
		if( i <= version )
		{
			continue;
		}

		bool result = m_funList[i](pKernel, self, args);
		if( result )
		{
			tempVersion = i;

			isUpVersion = true;
		}
		else
		{
			break;
		}
	}
    
	// �����µİ汾��
	if( isUpVersion )
	{
		pSelfObj->SetInt("PlayerVersion", tempVersion);
	}

	return true;
}