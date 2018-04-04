//--------------------------------------------------------------------
// �ļ���:      WorldBossNpc.cpp
// ��  ��:      ����Bossģ��
// ˵  ��:		
// ��������:    2015��6��23��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "WorldBossNpc.h"

#include "FsGame/Define/PubDefine.h"
#include "FsGame/Define/LogDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/CoolDownDefine.h"
#include "FsGame/Define/SceneListDefine.h"
#include "FsGame/Define/ModifyPackDefine.h"
#include "FsGame/Define/StaticDataDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/WorldBossNpcDefine.h"
#include "FsGame/NpcBaseModule/ai/AIDefine.h"
//#include "FsGame/Define/ToolBoxSysInfoDefine.h"

#include "FsGame/CommonModule/PubModule.h"
#include "FsGame/SocialSystemModule/ChatModule.h"
#include "FsGame/CommonModule/SwitchManagerModule.h"
#include "FsGame/SystemFunctionModule/ChannelModule.h"
#include "FsGame/CommonModule/LevelModule.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "FsGame/CommonModule/LandPosModule.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
#include "FsGame/CommonModule/PropRefreshModule.h"
#include "FsGame/SystemFunctionModule/RankListModule.h"
#include "FsGame/Interface/FightInterface.h"
#include "FsGame/SystemFunctionModule/CapitalModule.h"
#include "FsGame/CommonModule/ActionMutex.h"

#include <time.h>
#include <algorithm>
#include "utils/util.h"
#include "utils/XmlFile.h"
#include "utils/util_func.h"
#include "utils/custom_func.h"
#include "utils/extend_func.h"
#include "utils/string_util.h"
#include "../CommonModule/AsynCtrlModule.h"
#include "../CommonModule/EnvirValueModule.h"
#include "SystemFunctionModule/ResetTimerModule.h"
#include "Define/ResetTimerDefine.h"
#include "Define/Fields.h"
#include "CommonModule/ReLoadConfigModule.h"
#include "ItemModule/ToolItem/ToolItemModule.h"

// boss ��ʱ�洢��������
static const char* WORLD_BOSS_ACTIVED                 = "world_boss_actived";

// ����1���Ӧ��
static const int MIDDAY_SECOND                        = 13 * 60 * 60;
// 12����Ӧ��
static const int MIDDAY_SECOND_AM                      = 12 * 60 * 60 + 30 * 60;

// ָ���ʼ��
WorldBossNpc*       WorldBossNpc::m_pWorldBossNpc            = NULL;    
ChannelModule*      WorldBossNpc::m_pChannelModule           = NULL;
RewardModule*       WorldBossNpc::m_pRewardModule            = NULL;
DropModule*         WorldBossNpc::m_pDropModule              = NULL;
LandPosModule*      WorldBossNpc::m_pLandPosModule           = NULL;
CapitalModule*      WorldBossNpc::m_pCapitalModule           = NULL;

// PUB������
std::wstring WorldBossNpc::m_domainName; 

// GM����ʱ��
GMTimeInfo WorldBossNpc::GMTime;

TimeInterval_t WorldBossNpc::m_NoticeTimeCount;
TimeInterval_t WorldBossNpc::m_ActiveTimeCount;

std::vector<int> WorldBossNpc::m_SceneScopeVec;
WorldBossNpc::ConstConfig WorldBossNpc::m_kConstConfig;
WorldBossNpc::BossAwardVec WorldBossNpc::m_vecBossAward;
WorldBossNpc::WorldBossActiveVec WorldBossNpc::m_vecActiveInfo;


// �ؼ��ػ�����ļ�
int nx_reload_world_boss_active_config(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == pKernel)
	{
		return 0;
	}
	WorldBossNpc::m_pWorldBossNpc->LoadActiveInfoResource(pKernel);

	return 0;
}

// �ؼ��ؽ��������ļ�
int nx_reload_world_boss_award_config(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == pKernel)
	{
		return 0;
	}
	WorldBossNpc::m_pWorldBossNpc->LoadAwardResource(pKernel);

	return 0;
}

// �ؼ���BOSS�ɳ������ļ�
int nx_reload_world_boss_grow_up_config(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == pKernel)
	{
		return 0;
	}
	WorldBossNpc::m_pWorldBossNpc->LoadActiveRuleResource(pKernel);

	return 0;
}

// ��������BOSS�GM
int nx_worldboss_start(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == pKernel)
	{
		return 0;
	}

	// �궨��  ���LUA�ű���������
	CHECK_ARG_NUM(state, nx_worldboss_start, 2);

	CHECK_ARG_OBJECT(state, nx_worldboss_start, 1);

	CHECK_ARG_INT(state, nx_worldboss_start, 2);

	int iKillTimes = pKernel->LuaToInt(state, 2);

	CVarList CMDMsg;
	CMDMsg << COMMAND_WORLD_BOSS_ACTIVE 
		   << SS_WORLD_BOSS_ACTIVE_RESTART 
		   << iKillTimes;

	pKernel->CommandToAllScene(CMDMsg);

	return 0;
}

// ��������BOSS�GM
int nx_worldboss_end(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == pKernel)
	{
		return 0;
	}

	CVarList CMDMsg;
	CMDMsg << COMMAND_WORLD_BOSS_ACTIVE 
		   << SS_WORLD_BOSS_ACTIVE_END;

	pKernel->CommandToAllScene(CMDMsg);

	return 0;
}

/*!
* @brief	��ʼ��
* @param	����ָ��
* @return	bool
*/
bool WorldBossNpc::Init(IKernel* pKernel)
{
	m_pWorldBossNpc      = this;
	m_pChannelModule     = dynamic_cast<ChannelModule*>(pKernel->GetLogicModule("ChannelModule"));
	m_pRewardModule      = dynamic_cast<RewardModule*>(pKernel->GetLogicModule("RewardModule"));
	m_pDropModule        = dynamic_cast<DropModule*>(pKernel->GetLogicModule("DropModule"));
	m_pLandPosModule     = dynamic_cast<LandPosModule*>(pKernel->GetLogicModule("LandPosModule"));
	m_pCapitalModule     = dynamic_cast<CapitalModule*>(pKernel->GetLogicModule("CapitalModule"));

	Assert(m_pWorldBossNpc 
		&& m_pChannelModule 
		&& m_pRewardModule 
		&& m_pDropModule 
		&& m_pLandPosModule
		&& m_pCapitalModule);

	// ��������
	LoadAwardResource(pKernel);
	LoadActiveInfoResource(pKernel);
	LoadActiveRuleResource(pKernel);
	LoadWorldBossGrowUpConfig(pKernel);

	// ���������ص�
	pKernel->AddEventCallback("scene", "OnCreate", WorldBossNpc::OnCreateScene);

	// �������
	pKernel->AddEventCallback("player", "OnRecover", WorldBossNpc::OnRecover);

	// ������׼�������ص�
	pKernel->AddEventCallback("player", "OnReady", WorldBossNpc::OnPlayerReady);

	// ������׼�������ص�
	pKernel->AddEventCallback("player", "OnContinue", WorldBossNpc::OnPlayerContinue);

	// Boss��������
	pKernel->AddIntCommandHook("NormalNpc", COMMAND_BEKILL, WorldBossNpc::OnBossBeKilled);

	// �ͻ�����Ϣ�ص�
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_WORLD_BOSS_ACTIVE, OnCustomMessage);

	// �������
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_CONTINUE_ON_READY, WorldBossNpc::OnCustomReady);

	// ע����볡��֮���¼��ص�
	pKernel->AddEventCallback("player",  "OnAfterEntryScene",  WorldBossNpc::OnPlayerEntryScence);

	// GM����ؿ����
	pKernel->AddIntCommandHook("scene", COMMAND_WORLD_BOSS_ACTIVE, WorldBossNpc::OnCommandMessage, 100);

	// ע��BOSS�������ص�
	pKernel->AddIntCommandHook("WorldBossNpc", COMMAND_BEDAMAGE, WorldBossNpc::OnCommandBossBeDamaged);

	// ��������
	DECL_HEARTBEAT(WorldBossNpc::HB_CheckWorldBossActive);
	DECL_HEARTBEAT(WorldBossNpc::HB_CheckNotice);
	DECL_HEARTBEAT(WorldBossNpc::HB_SendCustomMsg);
	DECL_HEARTBEAT(WorldBossNpc::HB_ShowAttackRank);
	DECL_HEARTBEAT(WorldBossNpc::HB_StartPlayCG);
	DECL_HEARTBEAT(WorldBossNpc::HB_BossBorn);
	DECL_HEARTBEAT(WorldBossNpc::HB_LeaveScene);

	// LUA����
	DECL_LUA_EXT(nx_reload_world_boss_active_config);
	DECL_LUA_EXT(nx_reload_world_boss_award_config);
	DECL_LUA_EXT(nx_worldboss_start);
	DECL_LUA_EXT(nx_worldboss_end);

	RELOAD_CONFIG_REG("WorldBossConfig", WorldBossNpc::ReloadConfig);
	return true;
}

/*!
* @brief	����  
* @param	����ָ��
* @return	bool
*/
bool WorldBossNpc::Shut(IKernel* pKernel)
{
	m_SceneScopeVec.clear();
	m_vecBossAward.clear();
	m_vecActiveInfo.clear();

	return true;
}

/*!
* @brief	ʱ��ת������
* @param	ʱ���ַ���
* @param	��������
* @return	void
*/
int WorldBossNpc::ParseTime(const char* timesStr)
{
	// �п�
	if (StringUtil::CharIsNull(timesStr))
	{
		return 0;
	}

	return util_analyzing_time_str(timesStr);
}

/*!
* @brief	����BOSSת��
* @param	�����ַ���
* @param	��������
* @return	void
*/
void WorldBossNpc::ParseBossID(const char* strBossIDs, std::map<int, std::string>& mapBossIDs)
{
	// �п�
	if (StringUtil::CharIsNull(strBossIDs))
	{
		return;
	}

	CVarList BossIDList;
	util_split_string(BossIDList, strBossIDs, ",");

	int iCount = (int)BossIDList.GetCount();
	LoopBeginCheck(m);
	for (int i = 0; i < iCount; i++)
	{
		LoopDoCheck(m);
		const char* bossStr = BossIDList.StringVal(i);
		if (StringUtil::CharIsNull(bossStr))
		{
			continue;
		}
		CVarList _BossIDList;
		util_split_string(_BossIDList, bossStr, ":");

		if (_BossIDList.GetCount() == 2)
		{
			mapBossIDs.insert(std::make_pair(_BossIDList.IntVal(0), _BossIDList.StringVal(1)));
		}
	}
}

/*!
* @brief	����ת��
* @param	�����ַ���
* @param	��������
* @return	void
*/
void WorldBossNpc::ParseWeek(const char* weeksStr, std::vector<int>& vecWeeks)
{
	// �ж��Ƿ�Ϊ��
	if (StringUtil::CharIsNull(weeksStr))
	{
		return;
	}

	CVarList WeekList;
	util_split_string(WeekList, weeksStr,",");

	int iCount = (int)WeekList.GetCount();
	LoopBeginCheck(m);
	for (int i = 0; i < iCount; i++)
	{
		LoopDoCheck(m);
		vecWeeks.push_back(WeekList.IntVal(i));
	}
}

// �ڲ�����ص�
int WorldBossNpc::OnCommandMessage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	// ����
	if (NULL == pKernel || !pKernel->Exists(self))
	{
		return 0;
	}

	int iSubCMD = args.IntVal(1);
	switch(iSubCMD)
	{
	case SS_WORLD_BOSS_ACTIVE_RESTART:
		{
			OnCommandGMRestartActive(pKernel, self, args);
		}
		break;

	case SS_WORLD_BOSS_ACTIVE_UPDATE_DATA:
		{
			m_ActiveTimeCount.m_BeginTime = args.IntVal(2);
			m_ActiveTimeCount.m_EndTime = args.IntVal(3);
		}
		break;

	case SS_WORLD_BOSS_ACTIVE_END:
		{
			OnCommandGMEndActive(pKernel, self, args);
		}
		break;

	default:
		break;
	}
	return 0;
}

// ����¿���
int WorldBossNpc::OnCommandGMRestartActive(IKernel* pKernel, 
										   const PERSISTID& self, 
										   const IVarList& args)
{
	// �ҵ���Ҫ����������ĳ���
	int iSceneID = pKernel->GetSceneId();
	std::vector<int>& vecSceneScope = m_SceneScopeVec;

	std::vector<int>::iterator it = find(vecSceneScope.begin(), 
		vecSceneScope.end(), iSceneID);
	if (it != vecSceneScope.end())
	{
		PERSISTID CurrScene = pKernel->GetScene();
		IGameObj* pScene = pKernel->GetGameObj(CurrScene);
		if (NULL == pScene)
		{
			return NULL;
		}

		// �����������
		DELETE_HEART_BEAT(pKernel, CurrScene, "WorldBossNpc::HB_CheckNotice" );
		DELETE_HEART_BEAT(pKernel, CurrScene, "WorldBossNpc::HB_CheckWorldBossActive" );

		if (pScene->FindData(WORLD_BOSS_ACTIVED))
		{
			PERSISTID WorldBoss = pScene->QueryDataObject(WORLD_BOSS_ACTIVED);
			if (pKernel->Exists(WorldBoss))
			{
				// ����BOSS����
				pKernel->Destroy(CurrScene, WorldBoss);
			}
		}

		m_pWorldBossNpc->ClearActiveData(pKernel);

		// ����GM��ʶ
		pScene->SetDataInt("WorldBossGM", 1);

		// ������������
		if (args.IntVal(2) > 0)
		{
			CVarList PubMsg;
			PubMsg << SP_WORLD_BOSS_MSG_STATUS 
				   << SP_WORLD_BOSS_BE_KILL_TIME_SET_GM 
				   << pKernel->GetSceneId() 
				   << args.IntVal(2);
			SendMsgToPubServer(pKernel, PubMsg);
		}

		// ������������(ʱ��)
		int daySecond = util_convert_zerotime_to_sec();

		// ����ʱ��
		GMTime.m_NoticeBeginTime = daySecond;
		GMTime.m_NoticeEndTime = daySecond + 60 * 1 - 10;

		// �ʱ��
		GMTime.m_ActiveBeginTime = daySecond + 60 * 1;
		GMTime.m_ActiveEndTime = GMTime.m_ActiveBeginTime + 60 * 10;

		// ��ʱ�������BOSS�ʱ��
		ADD_HEART_BEAT(pKernel, self, "WorldBossNpc::HB_CheckWorldBossActive", 3 * 1000);
	}

	return 0;
}

// �����
int WorldBossNpc::OnCommandGMEndActive(IKernel* pKernel, 
										   const PERSISTID& self, 
										   const IVarList& args)
{
	// ������������(ʱ��)
	int daySecond = util_convert_zerotime_to_sec();

	// �ʱ��
	GMTime.m_ActiveBeginTime = daySecond + 1;
	GMTime.m_ActiveEndTime = GMTime.m_ActiveBeginTime + 10;

	return 0;
}

/*!
* @brief	������������ 
* @param	pKernel
* @param	self
* @param	sender
* @param	args
* @return	int
*/
int WorldBossNpc::OnCreateScene(IKernel* pKernel, 
								const PERSISTID& self, 
								const PERSISTID& sender, 
								const IVarList& args)
{
	// ����
	if (!pKernel->Exists(sender))
	{
		return 0;
	}

	// �����Ƿ����
	IGameObj* pScene = pKernel->GetGameObj(self);
	if (NULL == pScene)
	{
		return 0;
	}

	// ��Ҫ�������������
	int iCurrSceneID = pKernel->GetSceneId();
	std::vector<int>::iterator it = find(m_SceneScopeVec.begin(), 
		m_SceneScopeVec.end(), iCurrSceneID);
	if (it == m_SceneScopeVec.end())  																											  
	{
		return 0;
	}

	m_pWorldBossNpc->AddSceneData(pKernel, self);

	ADD_HEART_BEAT(pKernel, self, "WorldBossNpc::HB_CheckWorldBossActive", TIMER_WORLDBOSS_ACTIVE * 1000);
	return 0;
}

/*!
* @brief	�����״̬���� 
* @param	pKernel
* @param	self
* @param	slice
* @return	int
*/
int WorldBossNpc::HB_CheckWorldBossActive(IKernel* pKernel, const PERSISTID& self, int slice)
{
	// �����Ƿ����
	IGameObj* pScene = pKernel->GetGameObj(self);
	if (NULL == pScene)
	{
		return 0;
	}

	int nSceneId = pKernel->GetSceneId();

	// ͳ�Ƶ�������
	time_t tNow = ::time(NULL);
	tm* pNow = ::localtime(&tNow);
	if (NULL == pNow)
	{
		return 0;
	}

	int iCurrSec = pNow->tm_hour * 3600 + pNow->tm_min * 60 + pNow->tm_sec;
	//UpdateTodayActive(pKernel, self, iCurrSec);

	std::vector<WorldBossActive_t>& vecActiveInfo = m_vecActiveInfo;
	int iSize = (int)vecActiveInfo.size();
	LoopBeginCheck(q);
	for (int i = 0; i < iSize; ++i)
	{
		LoopDoCheck(q);
		// �����������Ϣ
		WorldBossActive_t* pActiveInfo = &vecActiveInfo[i];
		if (NULL == pActiveInfo)
		{
			continue;
		}

		// �жϳ���
		if (pKernel->GetSceneId() != pActiveInfo->m_SceneID)
		{
			continue;
		}

		// ��ѯ�Ƿ���GM����(Ĭ��Ϊ0--û�п���GM)
		int iGMVal = 0;
		if (pScene->FindData(FLAG_WORLD_BOSS_GM))
		{
			iGMVal = pScene->QueryDataInt(FLAG_WORLD_BOSS_GM);
		}

		// ����
		bool bSwitch = true;

		// ���ܿ���
		if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_WORLD_BOSS))
		{
			bSwitch = false;
		}	

		int iState = STATE_WBOSS_ACTIVE_CLOSE;
		if (iGMVal == 1)
		{
// 			if (GMTime.m_DoRule != pActiveInfo->m_DoRule)
// 			{
// 				continue;
// 			}

			// ���ؿ�
			if (bSwitch)
			{
				iState = m_pWorldBossNpc->GetGMWorldBossActiveState(pKernel, self, pActiveInfo, iCurrSec);
			}

			m_NoticeTimeCount.m_BeginTime = GMTime.m_NoticeBeginTime;
			m_NoticeTimeCount.m_EndTime = GMTime.m_NoticeEndTime;
			m_ActiveTimeCount.m_BeginTime = GMTime.m_ActiveBeginTime;
			m_ActiveTimeCount.m_EndTime = GMTime.m_ActiveEndTime;
		}
		else
		{
			// �������ڼ�
			std::vector<int>& vecWeeks = pActiveInfo->m_Weeks;
			std::vector<int>::iterator it = find(vecWeeks.begin(), vecWeeks.end(), pNow->tm_wday);
			if (it == vecWeeks.end())
			{
				continue;
			}

			// ���ؿ�
			if (bSwitch)
			{
				iState = m_pWorldBossNpc->GetWorldBossActiveState(pKernel, self, pActiveInfo, iCurrSec);
			}

			m_NoticeTimeCount.m_BeginTime = pActiveInfo->m_NoticeBeginTime;
			m_NoticeTimeCount.m_EndTime = pActiveInfo->m_NoticeEndTime;
			m_ActiveTimeCount.m_BeginTime = pActiveInfo->m_ActiveBeginTime;
			m_ActiveTimeCount.m_EndTime = pActiveInfo->m_ActiveEndTime;
		}

		// ����member����
		m_pWorldBossNpc->SyncMemberData(pKernel);

		// ���������������ID
		if (iState == STATE_WBOSS_ACTIVE_OPEN || iState == STATE_WBOSS_ACTIVE_NOTICE)
		{
			if (pScene->FindData(WORLD_BOSS_ACTIVE_CFG_ID))
			{
				pScene->SetDataInt(WORLD_BOSS_ACTIVE_CFG_ID, pActiveInfo->m_ID);
			}
		}

		// ��ȡ�״̬�Ƿ��Ѿ�ִ��
		int iVisit = pScene->QueryInt(FLAG_WORLD_BOSS_VISIT);;
		switch(iState)
		{
		case STATE_WBOSS_ACTIVE_NOTICE:
			{
				if (iVisit != FLAG_WORLD_BOSS_VISIT_NOTICED)
				{
					m_pWorldBossNpc->ActiveNotice(pKernel, self);
				}
				break;
			}
		case STATE_WBOSS_ACTIVE_OPEN:
			{
				if (/*iVisit < FLAG_WORLD_BOSS_VISIT_ACTIVED*/iVisit == FLAG_WORLD_BOSS_VISIT_NOTICED)
				{
					m_pWorldBossNpc->ActiveOpening(pKernel, self, pActiveInfo, pNow, iState, i);
				}
				break;
			}
		case STATE_WBOSS_ACTIVE_CLOSE: 
			{
				if (pScene->QueryDataInt(WORLD_BOSS_ACTIVE_CFG_ID) == pActiveInfo->m_ID 
					&& iVisit != FLAG_WORLD_BOSS_VISIT_CLOSED 
					&& iVisit == FLAG_WORLD_BOSS_VISIT_BORNED)
				{
					m_pWorldBossNpc->ActiveClosed(pKernel, self, iState);
				}
				break; 
			}
		default:
			break;
		}
	}

	return 0;
}

/*!
* @brief	����BOSS  
* @param	����ָ��
* @param	��ǰ����
* @param	���÷��ض���
* @return	��������int
*/
PERSISTID WorldBossNpc::CreateWorldBoss(IKernel* pKernel, 
										const WorldBossActive_t* pActiveInfo, 
										const int& iCurrSceneID, 
										std::string& strBossPos, 
										int iActiveTime, 
										int iWeek)
{
	// ��ȡBOSS��Ϣ,����BOSS
	IGameObj* pScene = pKernel->GetGameObj(pKernel->GetScene());
	if (NULL == pScene)
	{
		return PERSISTID();
	}

	if (NULL == pActiveInfo)
	{
		return PERSISTID();
	}

	// ��ȡBOSSID
	const std::string& strBossCfgID = pActiveInfo->m_strBossID;
	pScene->SetDataString("BOSS_CONFIG_ID", strBossCfgID.c_str());

	// BOSS����
	float fY = pKernel->GetMapHeight(pActiveInfo->m_BossPos.fPosX, pActiveInfo->m_BossPos.fPosZ);

	// ����BOSS	
	CVarList BossParam;
	BossParam << CREATE_TYPE_PROPERTY_VALUE 
			  << "GroupID"
			  << -1;
	PERSISTID WorldBoss = pKernel->CreateObjectArgs("", strBossCfgID.c_str(), 0, pActiveInfo->m_BossPos.fPosX, fY, pActiveInfo->m_BossPos.fPosZ, pActiveInfo->m_BossPos.fOrient, BossParam);
	IGameObj* pWorldBossObj = pKernel->GetGameObj(WorldBoss);
	if (NULL == pWorldBossObj)
	{
		return PERSISTID();
	}

	// ��ȡ���ӵ����԰�id
	int nCurBossPropertyPakId = m_pWorldBossNpc->QueryCurPackageId(pKernel);
	if (nCurBossPropertyPakId > 0)
	{
		// ��BOSS������
		MapPropValue mapPropValue;
		CVarList vPropNameList;
		CVarList vPropValueList;
		StaticDataQueryModule::m_pInstance->GetOneRowData(STATIC_DATA_NPC_BASE_PACK,
			StringUtil::IntAsString(nCurBossPropertyPakId), vPropNameList, vPropValueList, VTYPE_INT64);

		LoopBeginCheck(n);
		for (size_t i = 0; i < vPropNameList.GetCount(); ++i)
		{
			LoopDoCheck(n);
			// ��ȡ���㹫ʽ����
			float fVal = (float)vPropValueList.IntVal(i);
			if (FloatEqual(fVal, 0.0f))
			{
				continue;
			}
			PropRefreshModule::m_pInstance->CountRefreshData(pKernel, WorldBoss,
				vPropNameList.StringVal(i), fVal, EModify_ADD_VALUE, mapPropValue);
		}
		PropRefreshModule::m_pInstance->RefreshData(pKernel, WorldBoss, mapPropValue, EREFRESH_DATA_TYPE_ADD);
	}
	
	int64_t nMaxHP = pWorldBossObj->QueryInt64(FIELD_PROP_MAX_HP);
	pWorldBossObj->SetInt64(FIELD_PROP_HP, nMaxHP);
	
	// ��ӿ������ȼ�
	pKernel->AddVisualPriorityByScene(WorldBoss, true);

	// ��¼bossˢ��ʱ��
	pScene->SetInt64(FIELD_PROP_WORLD_BOSS_CREATE_TIME, ::util_get_utc_time());

	// ����BOSS LifeTime
	IGameObj* pWorldBoss = pKernel->GetGameObj(WorldBoss);
	if (NULL == pWorldBoss)
	{
		return PERSISTID();
	}
	pWorldBoss->SetInt("LifeTime", (iActiveTime + 3) * 1000);
	return WorldBoss;
}


/*!
* @brief	��ȡ�״̬
* @param	����ָ��
* @param	��ǰ����
* @param	���ػ״̬����
* @return	��������bool
*/
int WorldBossNpc::GetWorldBossActiveState(IKernel* pKernel, 
										  const PERSISTID& self, 
										  const WorldBossActive_t* pActiveInfo, 
										  int iCurrSec)
{
	// Ĭ��ֵ
	int iState = STATE_WBOSS_ACTIVE_CLOSE;

	// ��������
	if (pActiveInfo->m_NoticeBeginTime <= iCurrSec && iCurrSec < pActiveInfo->m_NoticeEndTime)
	{
		iState = STATE_WBOSS_ACTIVE_NOTICE;
	}
	// �����
	else if (pActiveInfo->m_ActiveBeginTime <= iCurrSec && iCurrSec <= pActiveInfo->m_ActiveEndTime)
	{
		iState = STATE_WBOSS_ACTIVE_OPEN;
	}
	// ��ر�
	else
	{
		iState = STATE_WBOSS_ACTIVE_CLOSE;
	}

	return iState;
}

/*!
* @brief	��ȡGM�״̬
* @param	����ָ��
* @param	��ǰ����
* @param	���ػ״̬����
* @return	��������bool
*/
int WorldBossNpc::GetGMWorldBossActiveState(IKernel* pKernel, 
											const PERSISTID& self, 
											const WorldBossActive_t* pActiveInfo, 
											int iCurrSec)
{
	// Ĭ��ֵ
	int iState = STATE_WBOSS_ACTIVE_CLOSE;

	// ��������
	if (GMTime.m_NoticeBeginTime <= iCurrSec && iCurrSec < GMTime.m_NoticeEndTime)  
	{
		iState = STATE_WBOSS_ACTIVE_NOTICE;
	}
	// �����
	else if (GMTime.m_ActiveBeginTime <= iCurrSec && iCurrSec <= GMTime.m_ActiveEndTime)  
	{
		iState = STATE_WBOSS_ACTIVE_OPEN;
	}
	// ��ر�
	else
	{
		iState = STATE_WBOSS_ACTIVE_CLOSE;
	}
	return iState;
}

/*!
* @brief	��ȡ�������Ϣָ��
* @param    ����ID
* @return	WorldBossActive_t����ָ��
*/
WorldBossActive_t* WorldBossNpc::GetActiveInfoCfg(IKernel* pKernel, const int iID)
{
	std::vector<WorldBossActive_t>& vecActiveInfo = m_vecActiveInfo;
	int iSize = (int)vecActiveInfo.size();

	LoopBeginCheck(q);
	for (int i = 0; i < iSize; ++i)
	{
		LoopDoCheck(q);
		if (iID == vecActiveInfo[i].m_ID)
		{
			return &vecActiveInfo[i];
		}
	}

	return NULL;
}

// ��ȡ��ǰ�������Ϣ
WorldBossActive_t* WorldBossNpc::GetActiveInfoCfgBySceneId(IKernel* pKernel, const int nSceneId)
{
	std::vector<WorldBossActive_t>& vecActiveInfo = m_vecActiveInfo;
	int iSize = (int)vecActiveInfo.size();

	LoopBeginCheck(q);
	for (int i = 0; i < iSize; ++i)
	{
		LoopDoCheck(q);
		if (nSceneId == vecActiveInfo[i].m_SceneID)
		{
			return &vecActiveInfo[i];
		}
	}

	return NULL;
}

/*!
* @brief	��ȡ�����������Ϣָ��
* @param    ��ɱ����
* @return	WorldBossAward_t����ָ��
*/
WorldBossAward_t* WorldBossNpc::GetBossAward(IKernel* pKernel, const char* strBossCfgID)
{
	// ����
	if (m_vecBossAward.empty() || StringUtil::CharIsNull(strBossCfgID))
	{
		return NULL;
	}

	std::vector<WorldBossAward_t>& vecAward = m_vecBossAward;

	// �������ҷ��������Ľ���
	struct BeKillTimesFinder
	{
		BeKillTimesFinder(const std::string& strBossID): _strBossID(strBossID){};
		bool operator() (const WorldBossAward_t& award)
		{
			return (strcmp(award.m_BossCfgID.c_str(), _strBossID.c_str()) == 0);
		}

		std::string _strBossID;
	};
	std::vector<WorldBossAward_t>::iterator it = find_if(vecAward.begin(), 
		vecAward.end(), BeKillTimesFinder(strBossCfgID));

	if (it != vecAward.end())
	{
		WorldBossAward_t& award = *it;
		return &award;
	}

	return  NULL;
}

/*!
* @brief	����BOSS����
* @param    ����ָ��
* @param    BOSS����
* @param    ���һ����Ҷ���
* @param    Я������
* @return	����ִ�з���int
*/
int WorldBossNpc::OnBossBeKilled(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	// ȡ��ǰ����
	PERSISTID CurrScene = pKernel->GetScene();
	IGameObj* pScene = pKernel->GetGameObj(CurrScene);
	if (NULL == pScene)
	{
		return 0;
	}

	//�ж��Ƿ��л������ʶ
	if (!pScene->FindAttr(FLAG_WORLD_BOSS_VISIT))
	{	
		return 0;
	}

	// �ж��Ƿ��ڼ��ڻ�ɱBOSS
	int iVisit = pScene->QueryInt(FLAG_WORLD_BOSS_VISIT);
	if (FLAG_WORLD_BOSS_VISIT_BORNED != iVisit)
	{
		return 0;
	}

	// ���³�����Ӣ��״̬
	IRecord* pEliteNpcRec = pScene->GetRecord(WORLD_BOSS_ELITE_NPC_REC);
	if (NULL != pEliteNpcRec)
	{
		int nRowIndex = pEliteNpcRec->FindObject(WBEN_COL_NPC_OBJECT, self);
		if (-1 != nRowIndex)
		{
			pEliteNpcRec->SetObject(nRowIndex, WBEN_COL_NPC_OBJECT, PERSISTID());
			int nNpcIndex = pEliteNpcRec->QueryInt(nRowIndex, WBEN_COL_NPC_INDEX);
			CVarList senddata;
			senddata << SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE << SC_WORLD_BOSS_ELITE_NPC_DEAD << nNpcIndex;
			pKernel->CustomByScene(pKernel->GetSceneId(), senddata);
		}
	}

	//�ж��Ƿ������ﴴ����BOSS
	PERSISTID QueryBoss =  pScene->QueryDataObject(WORLD_BOSS_ACTIVED);
	if (QueryBoss != self)
	{
		return 0;
	}

	// ����״̬
	m_pWorldBossNpc->OnSaveActiveStatusToPUB(pKernel, pKernel->GetSceneId(), 
		STATE_WBOSS_ACTIVE_CLOSE, pScene->QueryDataInt(WORLD_BOSS_ACTIVE_CFG_ID));

	// ���ý�����ʶ
	//pScene->SetInt(FLAG_WORLD_BOSS_VISIT, FLAG_WORLD_BOSS_VISIT_CLOSED);

	// �˺����ݿ���
	m_pWorldBossNpc->CopyAndSortHurtRecordFromAI(pKernel, self, sender);

	// ���һ������
	const wchar_t* wsName = pKernel->QueryWideStr(sender, "Name"); 
	if (!StringUtil::CharIsNull(wsName))
	{
		// �������һ������
		pScene->SetDataWideStr("Last_Harm", wsName);
		
		// ���һ������
		m_pWorldBossNpc->ActiveLastDamageAward(pKernel, self, sender);
	}

	// ��������
	m_pWorldBossNpc->ActiveRankAward(pKernel, CurrScene, SUC_KILL_WORLD_BOSS);

	// boss�ɳ�����仯
	m_pWorldBossNpc->UpdateGrowUpLevel(pKernel, true);

	// ����BOSS��ʱ����������
	pScene->SetDataObject(WORLD_BOSS_ACTIVED, PERSISTID());

	// ֪ͨ�����
	m_pWorldBossNpc->NoticeActiveFinish(pKernel, CurrScene, true);
	return 0;
}


/*!
* @brief	BOSS����ʱ����˺�ͳ�����ݿ������� 
* @param    ����ָ��
* @param    BOSS����
* @param    sender
* @return	����ִ�з���bool
*/
bool WorldBossNpc::CopyAndSortHurtRecordFromAI(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender)
{
	// �����ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ȡ��BOSS���ϵ��˺�ͳ�Ʊ������
	IRecord* pAIRecord = pKernel->GetRecord(self, WORLDBOSS_ATTACK_LIST_REC);
	if (NULL == pAIRecord)
	{
		return false;
	}

	int iAICount = pAIRecord->GetRows();

	CopyAIHurtVector tempDatas;
	tempDatas.reserve(iAICount);

	LoopBeginCheck(u); 
	for (int i = 0; i < iAICount; ++i)
	{
		LoopDoCheck(u);
		int iDamage = pAIRecord->QueryInt(i, WORLDBOSS_ATTACK_LIST_REC_DAMAGE);
		if (iDamage > 0)
		{
			CopyAIHurt_t tmp;
			tmp.m_player_name = pAIRecord->QueryWideStr(i, WORLDBOSS_ATTACK_LIST_REC_PLAYER_NAME);
			tmp.m_damage = iDamage;
			tempDatas.push_back(tmp);
		}		

	}
	// ����
	sort(tempDatas.begin(), tempDatas.end(), std::less<CopyAIHurt_t>());

	IGameObj* pSceneObj = pKernel->GetSceneObj();
	if (NULL == pSceneObj)
	{
		return false;
	}
	IRecord* pSceneDamRec = pSceneObj->GetRecord(WORLD_BOSS_DAMAGE_REC);
	if (NULL == pSceneDamRec)
	{
		return false;
	}

	pSceneDamRec->ClearRow();
	// �������
	int len = (int)tempDatas.size();
	LoopBeginCheck(v);
	for (int i = 0; i < len; ++i)
	{
		LoopDoCheck(v);
		const CopyAIHurt_t& tmp = tempDatas[i];
		pSceneDamRec->AddRowValue(-1, CVarList() << i + 1 << tmp.m_player_name << tmp.m_damage);
	}

	const wchar_t* wsKillerName = L"";
	if (pKernel->Exists(sender))
	{
		wsKillerName = pKernel->QueryWideStr(sender, FIELD_PROP_NAME);
	}
	
	CVarList PubMsg;
	PubMsg << SP_WORLD_BOSS_MSG_STATUS
		<< SP_UPDATE_BOSS_REC_INFO
		<< pKernel->GetSceneId()
		<< wsKillerName
		<< STATE_WBOSS_ACTIVE_CLOSE;

	SendMsgToPubServer(pKernel, PubMsg);
	return true;
}


/*!
* @brief	����ͻ�����Ϣ
* @param    ����ָ��
* @param    ��Ҷ���
* @param    ����
* @param    Я������
* @return	����ִ�з���int
*/
int WorldBossNpc::OnCustomMessage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	// �����ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return 0;
	}

	// ���ܿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_WORLD_BOSS, self))
	{
		return 0;
	}

	// ����ϢID
	int iSubCommand = args.IntVal(1);
	switch(iSubCommand)
	{
		// ������ҵ�ָ������
	case CS_WORLD_BOSS_ACTIVE_TRANSMIT:
		{
			// ������ҹ����ҳ������
			int iScene = args.IntVal(2);
			if (!m_pWorldBossNpc->IsWorldBossScene(iScene))
			{
				return 0;
			}
			WorldBossActive_t* pActiveInfo = m_pWorldBossNpc->GetActiveInfoCfgBySceneId(pKernel, iScene);
			if (NULL == pActiveInfo)
			{ 
				return 0;
			}

			// Ŀ�곡�������������
// 			if (SceneInfoModule::IsMaxPeopNum(pKernel, pActiveInfo->m_SceneID))
// 			{
// 				//Ŀ�곡�����Ѵ�����������Ժ�����
// 				::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, 
// 					GUIDE_BATTLE_MODULE_CUSTOM_string_3, CVarList());
// 
// 				return 0;
// 			}

			// ��ǰ�Ƿ��г������ڻ
			int nStatus = m_pWorldBossNpc->GetActiveStatusFromPUB(pKernel, iScene);
			if (nStatus != STATE_WBOSS_ACTIVE_OPEN)
			{
				// ϵͳ��ʾ��رղ��ܽ���
				CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_52, CVarList());
				return 0;
			}

			// �жϵȼ�����
			int iLevel = pSelf->QueryInt("Level");
			if (iLevel < pActiveInfo->m_PlayerLevel)
			{
				::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_51, CVarList() << pActiveInfo->m_PlayerLevel);
				return 0;
			}
			
			// ����
			float fPosY = pKernel->GetMapHeight(pActiveInfo->m_PlayerPos.fPosX, pActiveInfo->m_PlayerPos.fPosZ);
			if (NULL != m_pLandPosModule)
			{
				m_pLandPosModule->SetPlayerLandPosi(pKernel, self, pKernel->GetSceneId(), pSelf->GetPosiX(), pSelf->GetPosiY(), pSelf->GetPosiZ());
			}

			AsynCtrlModule::m_pAsynCtrlModule->SwitchLocate(pKernel, self, iScene, pActiveInfo->m_PlayerPos.fPosX, fPosY, pActiveInfo->m_PlayerPos.fPosZ, pActiveInfo->m_PlayerPos.fOrient);
		}
		break;

		// ������һ������
	case CS_WORLD_BOSS_ACTIVE_BACK_TO_LAST_SCENE:
		{
			if (NULL != m_pLandPosModule)
			{
				m_pLandPosModule->PlayerReturnLandPosi(pKernel, self);
			}
		}

		break;
	case CS_WORLD_BOSS_ACTIVE_QUERY_ACTIVE_STATUS:
		{
			m_pWorldBossNpc->OnQueryWorldBossState(pKernel, self);
		}
		break;
	case CS_WORLD_BOSS_ACTIVE_QUERY_BASE_INFO:
		{
			m_pWorldBossNpc->OnQueryBaseInfo(pKernel, self);
		}
		break;
	}

	return 0;
}

/*!
* @brief	������߼���Ƿ�������BOSS����� 
* @param    ����ָ��
* @param    ��Ҷ���
* @param    sender
* @param    Я������
* @return	����ִ�з���int
*/
int WorldBossNpc::OnPlayerReady(IKernel* pKernel, const PERSISTID& self,const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	bool first = args.BoolVal(0);

	if (first)
	{
		m_pWorldBossNpc->OnQueryWorldBossState(pKernel, self);
	}
	OnPlayerContinue(pKernel, self, sender, args);
	return 0;
}

// ��Ҷ�������
int WorldBossNpc::OnPlayerContinue(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	return 0;
}

/*!
* @brief	��Ҷ�����������Ƿ�������BOSS����� 
* @param    ����ָ��
* @param    ��Ҷ���
* @param    sender
* @param    Я������
* @return	����ִ�з���int
*/
int WorldBossNpc::OnCustomReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	m_pWorldBossNpc->OnQueryWorldBossState(pKernel, self);

	return 0;
}

/*!
* @brief	������ݻָ�
* @param    ����ָ��
* @param    ��Ҷ���
* @param    sender
* @param    args
* @return   ����int
*/
int WorldBossNpc::OnRecover(IKernel* pKernel, const PERSISTID& self,
							const PERSISTID& sender, const IVarList& args) 
{
	return 0;
}

// ��ѯ����boss�״̬
void WorldBossNpc::OnQueryWorldBossState(IKernel* pKernel, const PERSISTID& self)
{
	// ȡ����Ҷ�Ӧ�Ļ����
	int nActiveID = QueryCurActivity(pKernel);
	WorldBossActive_t* pActiveInfo = m_pWorldBossNpc->GetActiveInfoCfg(pKernel, nActiveID);
	if (NULL == pActiveInfo)
	{
		return;
	}
	// �ʣ��ʱ��
	int iActiveTime = GetRemainTime(pActiveInfo);

	// ������Ϣ���ͻ��ˣ�֪ͨ�����
	CVarList TransMsg;
	TransMsg << SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE
		<< SC_WORLD_BOSS_ACTIVE_OPEN
		<< iActiveTime;

	pKernel->Custom(self, TransMsg);
}

// �ظ�����boss�������ݲ�ѯ
int WorldBossNpc::OnQueryBaseInfo(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	IPubData* pWBPubData = GetPubData(pKernel);
	if (NULL == pWBPubData)
	{
		return 0;
	}
	IRecord* pKillerRec = pWBPubData->GetRecord(PUB_KILL_WORLD_BOSS_PLAYER_NAME_REC);
	if (NULL == pKillerRec)
	{
		return 0;
	}
	int nRows = pKillerRec->GetRows();

	CVarList msg;
	msg << SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE
		<< SC_WORLD_BOSS_ACTIVE_BASE_INFO
		<< nRows;

	LoopBeginCheck(a);
	for (int i = 0; i < nRows; ++i)
	{
		LoopDoCheck(a);
		const wchar_t* wsName = pKillerRec->QueryWideStr(i, KWBP_PUB_COL_KILLER_NAME);
		int nSceneId = pKillerRec->QueryInt(i, KWBP_PUB_COL_SCENE_ID);
		int nBossState = pKillerRec->QueryInt(i, KWBP_PUB_COL_BOSS_STATE);
		msg << nSceneId << wsName << nBossState;
	}

	pKernel->Custom(self, msg);
	return 0;
}

/*!
* @brief	������Ҳ����Ƿ�������BOSS�����
* @param    ����ָ��
* @param    ��Ҷ���
* @return	����ִ�з���bool
*/
int WorldBossNpc::QueryBossActiveType()
{
	return 1;// m_pWorldBossNpc->GetModel();
}

/*!
* @brief	������ҹ����ҳ������ 
* @param    ����ָ��
* @param    ��Ҷ���
* @return	����ִ�з���int
*/
int WorldBossNpc::GetActiveSceneByPlayer(IKernel* pKernel, const PERSISTID& self)
{
	// ����
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return 0;
	}

	// Ŀǰ�ʱֻ��һ������,˫����Ӫ���˶����Խ�
	// ��ҹ���
	int iActiveSceneID = 0;
	std::vector<int>& vecScope = m_SceneScopeVec;
	int iSize = (int)vecScope.size();
	LoopBeginCheck(ab);
	for (int i = 0; i < iSize; ++i)
	{
		LoopDoCheck(ab);
		iActiveSceneID = vecScope[i];
// 		int iEqualNation = GetNationTypeBySceneId(iActiveSceneID);
// 		if (iEqualNation == iNation)
// 		{
// 			return iActiveSceneID;
// 		}
	}

	return iActiveSceneID;
}


/*!
* @brief	�㲥��Ϣ��30��һ��
* @param	[in] pKernel ����ָ��
* @param	[in] self ����
* @param	[in] slice
* @return	int �߼�ִ�н��
*/
int WorldBossNpc::HB_CheckNotice(IKernel* pKernel, const PERSISTID& self, int slice)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	CustomSysInfoByScene(pKernel, 0, SYSTEM_INFO_ID_53, CVarList());
	return 0;
}


/*!
* @brief	������������
* @param	[in] pKernel ����ָ��
* @return	bool �߼�ִ�н�� 
*/
bool WorldBossNpc::ClearActiveData(IKernel* pKernel)
{
	// ����˺�ͳ�Ʊ�����
	PERSISTID CurrScene = pKernel->GetScene();
	IGameObj* pScene = pKernel->GetGameObj(CurrScene);
	if (NULL == pScene)
	{
		return 0;
	}

	// ����GM����(0:����  1:GM),Ĭ�ϲ�����GM
	pScene->SetDataInt(FLAG_WORLD_BOSS_GM, 0);

	// �������һ�����֣�Ĭ��""��
	pScene->SetDataWideStr("Last_Harm", L"");

	// ����Boss����ID(Ĭ��"")
	pScene->SetDataString("BOSS_CONFIG_ID", "");
	return true;
}

/*!
* @brief	���һ���� 
* @param	[in] pKernel ����ָ��
* @param	[in] killer ���һ�����
* @param	[in] wsName  �������
* @return	int �߼�ִ�н��
*/
int WorldBossNpc::ActiveLastDamageAward(IKernel* pKernel, 
										const PERSISTID& boss, 
										const PERSISTID& killer)
{
	// �жϳ���
	IGameObj* pScene = pKernel->GetGameObj(pKernel->GetScene());
	if (NULL == pScene)
	{
		return 0;
	}

	//������Ʒ
	const char* cBossID = pScene->QueryDataString("BOSS_CONFIG_ID");
	if (StringUtil::CharIsNull(cBossID))
	{
		return 0;
	}

	WorldBossAward_t* pBossAward =  GetBossAward(pKernel, cBossID);
	if (NULL == pBossAward)
	{
		extend_warning(LOG_ERROR, "[WorldBossNpc::ActiveLastDamageAward] not find last damage award!");
		return 0;
	}

	// �������
	const wchar_t* killerName = pKernel->QueryWideStr(killer, "Name");
	if (StringUtil::CharIsNull(killerName))
	{
		extend_warning(LOG_ERROR, "[WorldBossNpc::ActiveLastDamageAward] not find palyer name!");
		return 0;
	}

	// �ʼ����Ž���
	bool result = m_pRewardModule->RewardViaMail(pKernel, killerName, pBossAward->m_nLastHurtAward, CVarList());
	if (result)
	{
		// BOSS����
		const char* npcName = pKernel->QueryString(boss, "Config");
		if (StringUtil::CharIsNull(npcName))
		{
			extend_warning(LOG_ERROR, "[WorldBossNpc::ActiveLastDamageAward] not find boss configID!");
			npcName = "";
		}

		CVarList LastKillMsg;
		LastKillMsg << killerName 
					<< npcName 
					<< pBossAward->m_nLastHurtAward;

		CustomSysInfoByScene(pKernel, 0, SYSTEM_INFO_ID_54, LastKillMsg);
	}
	return 0;
}

/*!
* @brief	������
* @param	[in] pKernel ����ָ��
* @param	[in] CurrScene ����
* @param	[in] boss 
* @return	int �߼�ִ�н��
*/
int WorldBossNpc::ActiveRankAward(IKernel* pKernel, const PERSISTID& CurrScene, int nResult)
{
	// ����
	IGameObj* pScene = pKernel->GetGameObj(CurrScene);
	if (NULL == pScene)
	{
		return 0;
	}

	//������Ʒ
	const char* cBossID = pScene->QueryDataString("BOSS_CONFIG_ID");
	WorldBossAward_t* pBossAward = GetBossAward(pKernel, cBossID);
	if (NULL == pBossAward)
	{
		return 0;
	}

	// �˺��������
	IRecord* pWBossDamRecord = pScene->GetRecord(WORLD_BOSS_DAMAGE_REC);
	if (NULL == pWBossDamRecord) 
	{
		return 0;
	}

	int iRowCount = pWBossDamRecord->GetRows();
	std::vector<RankAward_t>& vecRankAward = pBossAward->m_vecRankAward;// nResult == SUC_KILL_WORLD_BOSS ? : pBossAward->m_FailRankAward;
	int iRankVecCount = (int)vecRankAward.size();
	if (iRowCount < 1 || iRankVecCount < 1)
	{
		return 0;
	}
	LoopBeginCheck(y);
	for (int k = 0; k < iRankVecCount; ++k)
	{
		LoopDoCheck(y);
		RankAward_t& tRankAward = vecRankAward[k];
		LoopBeginCheck(z);
		for (int i = 0; i < iRowCount; ++i)
		{
			LoopDoCheck(z);
			int iRank = pWBossDamRecord->QueryInt(i, WBD_COL_PLAYER_RANK);
			if (iRank >= tRankAward.m_MinRank && iRank <= tRankAward.m_MaxRank)
			{
				const wchar_t* play_name = pWBossDamRecord->QueryWideStr(i, WBD_COL_PLAYER_NAME);
				if (StringUtil::CharIsNull(play_name))
				{
					continue;
				}

				int nRewardId = nResult == SUC_KILL_WORLD_BOSS ? tRankAward.m_nWinRewardId : tRankAward.m_nFailRewardId;
				// ��Ҷ����Ƿ��ڵ�ǰ��������
				PERSISTID& Attacker = pKernel->FindPlayer(play_name);
				if (pKernel->Exists(Attacker))
				{

					// ����������н�Ʒ��Ϣ
					if (!pKernel->FindData(Attacker, "TempRankAward"))
					{
						ADD_DATA(pKernel, Attacker, "TempRankAward", VTYPE_INT);
						pKernel->SetDataInt(Attacker, "TempRankAward", nRewardId);
					}
// 
					// �淨��־(��¼�����������)
					GamePlayerActionLog log;
					log.actionType = LOG_GAME_ACTION_WORLDBOSS;
					log.actionState = iRank;
					LogModule::m_pLogModule->SaveGameActionLog(pKernel, Attacker, log);
				}

				m_pRewardModule->RewardViaMail(pKernel, play_name, nRewardId, CVarList() << iRank << cBossID);
			}
		}
	}

	return 0;
}

/*!
* @brief	��Ʒ����
* @param	[*] pKernel ����ָ��
* @param	[Object] Attacker �����ߣ���ң�
* @param	[Object] boss 
* @param	[string] strItemList ������Ϣ
* @return	int �߼�ִ�н��
*/
int WorldBossNpc::DropItems(IKernel* pKernel,const PERSISTID& Attacker, const PERSISTID& boss, const std::string& strItemList)
{
	// ����
	if (strItemList.empty())
	{
		return 0;
	}

	// ������װ��������
	CVarList DropInfoList;

	// ����λ�õĶ���
	DropInfoList << Attacker;

	// �������
	CVarList ItemList;
	util_split_string(ItemList, strItemList, ",");
	int iItemCount = (int)ItemList.GetCount();
	DropInfoList << iItemCount;

	// ���������Ʒ��Ϣ
	LoopBeginCheck(cc);
	for (int i = 0; i < iItemCount; ++i)
	{
		LoopDoCheck(cc);
		const char* strItem = ItemList.StringVal(i);
		if (StringUtil::CharIsNull(strItem))
		{
			continue;
		}

		CVarList ItemInfo;
		util_split_string(ItemInfo, strItem, ":");
		DropInfoList << ItemInfo.StringVal(0) 
			<< ItemInfo.IntVal(1);
	}

	m_pDropModule->PlayerDropItems(pKernel, Attacker, DropInfoList);

	return 0;
}

/*!
* @brief	�������
* @param	[*] pKernel ����ָ��
* @param	[Object] CurrScene ����
* @param	[int] slice
* @return	int �߼�ִ�н��
*/
int WorldBossNpc::HB_SendCustomMsg(IKernel* pKernel, const PERSISTID& self, int slice)
{
	// ����
	IGameObj* pScene = pKernel->GetGameObj(self);
	if (NULL == pScene)
	{
		return 0;
	}

	/*!
	* @brief	�������
	* @param	wstring	��ɱboss�������
	* @param	int			�ҵ�����
	* @param	int			�ҵ��˺�
	* @param	int		ǰn��
	����ѭ��n��
	* @param	wstring �������
	* @param	int		�������
	* @param	int		�����Ӫ
	* @param	int		����˺�ֵ
	*/
//	SC_WORLD_BOSS_ACTIVE_ACTOR_AWARD,

	// ���һ������
	const wchar_t* wstrLastHarm = pScene->QueryDataWideStr("Last_Harm");
	if (StringUtil::CharIsNull(wstrLastHarm))
	{
		wstrLastHarm = LAST_KILL_BOSS_NAME;
	}

	// �˺��������
	IRecord* pDamRec = pScene->GetRecord(WORLD_BOSS_DAMAGE_REC);
	if (NULL == pDamRec)
	{
		return 0;
	}

	CVarList ShowMsg;
	int iRowCount = pDamRec->GetRows();
	// ��������ͽ����������
	LoopBeginCheck(ee);
	for (int i = 0; i < iRowCount; ++i)
	{
		LoopDoCheck(ee);
		const wchar_t* player_name = pDamRec->QueryWideStr(i, WBD_COL_PLAYER_NAME);
		if (StringUtil::CharIsNull(player_name))
		{
			continue;
		}

		// ��Ҳ��ڵ�ǰ���������ͽ������
		PERSISTID& Attacker = pKernel->FindPlayer(player_name);
		IGameObj* pAttacker = pKernel->GetGameObj(Attacker);
		if (NULL == pAttacker)
		{
			continue;
		}

		int iHurt = pDamRec->QueryInt(i, WBD_COL_PLAYER_TOTAL_DAMAGE);

		// ֪ͨ�ɾ�ģ���������boss��سɾ�[2015.08.03 tongzt]
		int iRank = i + 1;	// ����

		ShowMsg.Clear();
		ShowMsg << SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE
			<< SC_WORLD_BOSS_ACTIVE_ACTOR_AWARD
//			<< wstrLastHarm
			<< iRank
			<< iHurt;
/*			<< ranklist;*/

		// ��ѯ��ʱ����
		if (!pKernel->FindData(Attacker, "TempRankAward"))
		{ 
			continue;
		}
		const char* strRankAward = pAttacker->QueryDataString("TempRankAward");
		if (StringUtil::CharIsNull(strRankAward))
		{
			continue;
		}

		ShowMsg << strRankAward;

		SafeCustom(pKernel, Attacker, ShowMsg);

		REMOVE_DATA(pKernel, Attacker, "TempRankAward");
	}

	// �������ݺ�ɾ��BOSS����ʶ
	m_pWorldBossNpc->ClearActiveData(pKernel);

	// �뿪��ǰ����
	ADD_COUNT_BEAT(pKernel, self, "WorldBossNpc::HB_LeaveScene", m_kConstConfig.nLeaveSceneDelay, 1);
	return 0;
}

/*!
* @brief	֪ͨ��һ����
* @param	[*] pKernel ����ָ��
* @param	[PERSISTID] ����
* @return	int ִ�з���
*/
int WorldBossNpc::NoticeActiveFinish(IKernel* pKernel, const PERSISTID& self, bool bossBeKilled)
{
	// BOSSδ������,�㲥�����
	if (!bossBeKilled)
	{
		CustomSysInfoByScene(pKernel, 0, SYSTEM_INFO_ID_52, CVarList());
	}

	CVarList FinishMsg;
	FinishMsg << SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE 
		      << SC_WORLD_BOSS_ACTIVE_CLOSE;

	pKernel->CustomByWorld(FinishMsg);

	// ��������ڵ�npc
	ClearSceneNpc(pKernel);

	// ɾ��ʵʱ�˺���ʾ����
	DELETE_HEART_BEAT(pKernel, pKernel->GetScene(), "WorldBossNpc::HB_ShowAttackRank");

	ADD_COUNT_BEAT(pKernel, pKernel->GetScene(), "WorldBossNpc::HB_SendCustomMsg", 1500, 1);
	return 0;
}

/*!
* @brief	����ʣ��ʱ��
* @param	����ID
* @return	int 
*/
int WorldBossNpc::GetRemainTime(const WorldBossActive_t* pActiveInfo)
{
	if (NULL == pActiveInfo)
	{
		return 0;
	}

	int daySecond = util_convert_zerotime_to_sec();

	int iTime = 0;

	// ���δ��ʼ
	if (m_ActiveTimeCount.m_BeginTime >= daySecond)
	{
		iTime = m_ActiveTimeCount.m_EndTime - m_ActiveTimeCount.m_BeginTime;
	}
	// ��Ѿ���ʼ
	else if (m_ActiveTimeCount.m_BeginTime < daySecond && m_ActiveTimeCount.m_EndTime > daySecond)
	{
		iTime = m_ActiveTimeCount.m_EndTime - daySecond;
	}
	// �����
	else
	{
		iTime = 0;
	}

	return iTime;
}

// ��ѯ��ǰ�Ļ
int WorldBossNpc::QueryCurActivity(IKernel* pKernel)
{
	IPubData* pWorldBossPubData = GetPubData(pKernel);
	if (NULL == pWorldBossPubData)
	{
		return 0;
	}

	int nCurActivityId = WBAT_NONE_WORLD_BOSS;
	// ͳһʱ���ֻ����һ�ֻ�ڽ�����
	IRecord* pRecord = pWorldBossPubData->GetRecord(WORLD_BOSS_ACTIVE_STATUS_REC);
	if (NULL == pRecord)
	{
		return false;
	}
	int nRows = pRecord->GetRows();
	LoopBeginCheck(t);
	for (int i = 0; i < nRows;++i)
	{
		LoopDoCheck(t);
		int nState = pRecord->QueryInt(i, WORLD_BOSS_STATUS_REC_COL_STATUS);
		if (nState == STATE_WBOSS_ACTIVE_OPEN)
		{
			nCurActivityId = pRecord->QueryInt(i, WORLD_BOSS_STATUS_REC_COL_ID);
			break;
		}
	}
	
	return nCurActivityId;
}

// ��������ڵ�����npc
void WorldBossNpc::ClearSceneNpc(IKernel* pKernel)
{
	CVarList Result;
	pKernel->GetChildList(pKernel->GetScene(), TYPE_NPC, Result);

	int iCount = (int)Result.GetCount();
	if (iCount == 0)
	{
		return;
	}

	LoopBeginCheck(u);
	for (int i = 0; i < iCount; ++i)
	{
		LoopDoCheck(u);
		PERSISTID npc = Result.ObjectVal(i);
		pKernel->DestroySelf(npc);
	}
}

void WorldBossNpc::ReloadConfig(IKernel* pKernel)
{
	// ��������
	LoadAwardResource(pKernel);
	LoadActiveInfoResource(pKernel);
	LoadActiveRuleResource(pKernel);
	m_pWorldBossNpc->LoadWorldBossGrowUpConfig(pKernel);
}

// �ж�����boss�����Ƿ�ɽ���
bool WorldBossNpc::IsSceneEnter(IKernel* pKernel, const PERSISTID& self, int nSceneId)
{
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}
	WorldBossActive_t* pActiveInfo = m_pWorldBossNpc->GetActiveInfoCfgBySceneId(pKernel, nSceneId);
	if (NULL == pActiveInfo) // ��������boss���� ����Ҫ��֤
	{
		return true;
	}
	bool bEnter = true;
	do 
	{
		// ��ǰ�Ƿ��г������ڻ
		int nStatus = m_pWorldBossNpc->GetActiveStatusFromPUB(pKernel, nSceneId);
		if (nStatus != STATE_WBOSS_ACTIVE_OPEN)
		{
			// ϵͳ��ʾ��رղ��ܽ���
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_52, CVarList());

			bEnter = false;
			break;
		}

		// �жϵȼ�����
		int iLevel = pSelf->QueryInt("Level");
		if (iLevel < pActiveInfo->m_PlayerLevel)
		{
			::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_51, CVarList() << pActiveInfo->m_PlayerLevel);
			bEnter = false;
		}
	} while (0);

	return bEnter;
}

/*!
* @brief	���볡��֮��
* @param	pKernel ����ָ��
* @param	���
* @param	
* @param    ����
* @return	int 
*/
int WorldBossNpc::OnPlayerEntryScence(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	// ��Ҷ���
	IGameObj* pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	// ������ڽ���ĳ���
	int iEntrySceneID = pKernel->GetSceneId();

	std::vector<int>::const_iterator it_beg = m_SceneScopeVec.begin();
	std::vector<int>::const_iterator it_end = m_SceneScopeVec.end();
	std::vector<int>::const_iterator it = find(it_beg, it_end, iEntrySceneID);
	if (it == it_end)
	{
		return 0;
	}

	int iState = m_pWorldBossNpc->GetActiveStatusFromPUB(pKernel, iEntrySceneID);
	if (iState == STATE_WBOSS_ACTIVE_CLOSE)
	{
		// ��ҽ�������,�����������һ������
		m_pLandPosModule->PlayerReturnLandPosi(pKernel, self);
	}

	return 0;
}

// ��ʼ����CG
int WorldBossNpc::HB_StartPlayCG(IKernel* pKernel, const PERSISTID& self, int slice)
{
	int nSceneId = pKernel->GetSceneId();

	pKernel->CustomByScene(nSceneId, CVarList() << SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE << SC_WORLD_BOSS_START_PLAY_CG);
	return 0;
}

// �뿪��������
int WorldBossNpc::HB_LeaveScene(IKernel* pKernel, const PERSISTID& self, int slice)
{
	// ����
	IGameObj* pScene = pKernel->GetGameObj(self);
	if (NULL == pScene)
	{
		return 0;
	}

	// ֪ͨ�����ڳ���������뿪
	CVarList Result;
	pKernel->GetChildList(self, TYPE_PLAYER, Result);

	int iCount = (int)Result.GetCount();
	if (iCount == 0)
	{
		return 0;
	}

	LoopBeginCheck(u);
	// ���ؽ���ʱ�ĳ���
	for (int i = 0; i < iCount; ++i)
	{
		LoopDoCheck(u);
		PERSISTID _player = Result.ObjectVal(i);
		if (pKernel->Exists(_player))
		{
			m_pLandPosModule->PlayerReturnLandPosi(pKernel, _player);
		}
	}
	return 0;
}

// ��ʼˢBoss
int WorldBossNpc::HB_BossBorn(IKernel* pKernel, const PERSISTID& self, int slice)
{
	IGameObj* pSceneObj = pKernel->GetGameObj(self);
	if (NULL == pSceneObj)
	{
		return 0;
	}
	// ��ȡ��ǰ��ʱ��
	time_t tNow = ::time(NULL);
	tm* pNow = ::localtime(&tNow);
	if(NULL == pNow)
	{
		return 0;
	}

	// ��ȡ��ǰ��������
	if (!pSceneObj->FindData(FLAG_WORLD_BOSS_CONFIG_DATA_INDEX))
	{
		return 0;
	}

	int nConfigIndex = pSceneObj->QueryDataInt(FLAG_WORLD_BOSS_CONFIG_DATA_INDEX);

	// �ҵ���������
	std::vector<WorldBossActive_t>& vecActiveInfo = m_vecActiveInfo;
	if (nConfigIndex < 0 || nConfigIndex >= (int)vecActiveInfo.size())
	{
		return 0;
	}

	WorldBossActive_t* pActiveInfo = &vecActiveInfo[nConfigIndex];
	if (NULL == pActiveInfo)
	{
		return 0;
	}

	int iActiveTime = GetRemainTime(pActiveInfo);

	// ����BOSS
	std::string strBossPos;
	PERSISTID WorldBoss = CreateWorldBoss(pKernel, pActiveInfo, 
		pKernel->GetSceneId(), strBossPos, iActiveTime, pNow->tm_wday);
	if(WorldBoss.IsNull()) 
	{
		return 0;
	}

	// ���û��ʶ
	pSceneObj->SetInt(FLAG_WORLD_BOSS_VISIT, FLAG_WORLD_BOSS_VISIT_BORNED);

	// ��������ʱ�洢BOSS����ID
	if (pSceneObj->FindData(WORLD_BOSS_ACTIVED))
	{
		pSceneObj->SetDataObject(WORLD_BOSS_ACTIVED, WorldBoss);
	}

	return 0;
}

/*!
* @brief	��������BOSS���˺�
* @param	����ָ��
* @param	self
* @param	sender
* @param	args
* @return	int 
*/
int WorldBossNpc::OnCommandBossBeDamaged(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	// BOSS
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return 0;
	}

	// ����Ļ�,�ҵ���������
	PERSISTID damager = get_pet_master(pKernel, sender);

	// ������
	IGameObj* pSender = pKernel->GetGameObj(damager);
	if (NULL == pSender)
	{
		return 0;
	}

	// ����������
	const wchar_t* wsPlayerName = pSender->QueryWideStr(FIELD_PROP_NAME);
	if (StringUtil::CharIsNull(wsPlayerName))
	{
		return 0;
	}

	//�����߶�BOSS�˺����
	int damage = args.IntVal(1);

	IRecord* pAttackerList = pSelf->GetRecord(WORLDBOSS_ATTACK_LIST_REC);
	if(NULL == pAttackerList)
	{
		return 0;
	}

	//������������뵽����
	int row = pAttackerList->FindWideStr(WORLDBOSS_ATTACK_LIST_REC_PLAYER_NAME, wsPlayerName);
	if (row == -1)
	{
		CVarList values;
		values	<< wsPlayerName 
				<< damage;
		pAttackerList->AddRowValue(-1, values);

	}
	else
	{
		int sum = pAttackerList->QueryInt(row, WORLDBOSS_ATTACK_LIST_REC_DAMAGE);
		sum = sum + damage;
		pAttackerList->SetInt(row, WORLDBOSS_ATTACK_LIST_REC_DAMAGE, sum);
	}

	return 0;
}


/////////////////////////////////�������������߼�/////////////////////////////////////////

/*!
* @brief	��ȡ�ռ�������ָ��
* @param	����ָ��
* @return	int 
*/
IPubData* WorldBossNpc::GetPubData(IKernel* pKernel)
{
	IPubSpace* pDomainSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (NULL == pDomainSpace)
	{
		return NULL;
	}

	return pDomainSpace->GetPubData(GetDomainName(pKernel).c_str());
}


/*!
* @brief	��ȡ�ռ���
* @param	����ָ��
* @return	int 
*/
const std::wstring& WorldBossNpc::GetDomainName(IKernel* pKernel)
{
	if (m_domainName.empty())
	{
		wchar_t wstr[256];
		const int server_id = pKernel->GetServerId();
		SWPRINTF_S(wstr, L"Domain_WorldBoss_%d", server_id);

		m_domainName = wstr;
	}

	return m_domainName;
}

/*!
* @brief	ȥ�������������һ״̬
* @param	����ָ��
* @param	����ID
* @return	int 
*/
int WorldBossNpc::GetActiveStatusFromPUB(IKernel* pKernel, int iSceneID)
{
	IPubSpace* pPubSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (pPubSpace == NULL)
	{
		return 0;
	}

	// ��������ָ��
	IPubData* pPAData = pPubSpace->GetPubData(WorldBossNpc::m_pWorldBossNpc->GetDomainName(pKernel).c_str());
	if (NULL == pPAData)
	{
		return 0;
	}

	// ���ݱ�
	IRecord* pRecord = pPAData->GetRecord(WORLD_BOSS_ACTIVE_STATUS_REC);
	if (NULL == pRecord)
	{
		return 0;
	}

	// ���Ҷ�Ӧ�ĳ����״̬
	const int iRow = pRecord->FindInt(WORLD_BOSS_STATUS_REC_COL_SCENEID, iSceneID);
	if (iRow != -1)
	{
		return pRecord->QueryInt(iRow, WORLD_BOSS_STATUS_REC_COL_STATUS);
	}

	return 0;
}

int WorldBossNpc::GetActiveCfgIDFromPUB(IKernel* pKernel, int iSceneID)
{
	IPubSpace* pPubSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (pPubSpace == NULL)
	{
		return 0;
	}

	// ��������ָ��
	IPubData* pPAData = pPubSpace->GetPubData(WorldBossNpc::m_pWorldBossNpc->GetDomainName(pKernel).c_str());
	if (NULL == pPAData)
	{
		return 0;
	}

	// ���ݱ�
	IRecord* pRecord = pPAData->GetRecord(WORLD_BOSS_ACTIVE_STATUS_REC);
	if (NULL == pRecord)
	{
		return 0;
	}

	// ���Ҷ�Ӧ�ĳ����״̬
	const int iRow = pRecord->FindInt(WORLD_BOSS_STATUS_REC_COL_SCENEID, iSceneID);
	if (iRow != -1)
	{
		return pRecord->QueryInt(iRow, WORLD_BOSS_STATUS_REC_COL_ID);
	}

	return 0;
}

// ��֤�Ƿ�Ϊ����boss����
bool WorldBossNpc::IsWorldBossScene(int iSceneID)
{
	if (NULL == m_pWorldBossNpc)
	{
		return false;
	}
	std::vector<int>::iterator iter = find(m_pWorldBossNpc->m_SceneScopeVec.begin(), m_pWorldBossNpc->m_SceneScopeVec.end(), iSceneID);
	return iter != m_pWorldBossNpc->m_SceneScopeVec.end();
}

/*!
* @brief	����Ϣ��Pub������
* @param	����ָ��
* @return	int 
*/
bool WorldBossNpc::SendMsgToPubServer(IKernel* pKernel, const IVarList& msg)
{
	CVarList PubMsg;
	PubMsg << PUBSPACE_DOMAIN 
		<< GetDomainName(pKernel).c_str()
		<< msg;

	return pKernel->SendPublicMessage(PubMsg);
}

/*!
* @brief	���û״̬
* @param	����ָ��
* @return	int 
*/
int WorldBossNpc::OnSaveActiveStatusToPUB(IKernel* pKernel, const int iSceneID, const int iStatus, const int iID)
{
	CVarList PubMsg;
	PubMsg << SP_WORLD_BOSS_MSG_STATUS
		<< SP_WORLD_BOSS_ACTIVE_STATUS_SET
		<< iSceneID 
		<< iStatus
		<< iID;

	return SendMsgToPubServer(pKernel, PubMsg);
}

/*!
* @brief	�����ʱ����
* @param	����ָ��
* @param	Scene
* @return	void 
*/
void WorldBossNpc::AddSceneData(IKernel* pKernel, const PERSISTID& Scene)
{
	IGameObj* pScene = pKernel->GetGameObj(Scene);
	if (NULL == pScene)
	{
		return;
	}
	// ����GM����(0:����  1:GM),Ĭ�ϲ�����GM
	ADD_DATA(pKernel, Scene, FLAG_WORLD_BOSS_GM, VTYPE_INT);
	pScene->SetDataInt(FLAG_WORLD_BOSS_GM, 0);

	// ״̬��ʶ,Ĭ�Ϲر�
	//ADD_DATA(pKernel, Scene, FLAG_WORLD_BOSS_VISIT, VTYPE_INT);
	pScene->SetInt(FLAG_WORLD_BOSS_VISIT, FLAG_WORLD_BOSS_VISIT_CLOSED);

	// ���û����ID(Ĭ��-1)
	ADD_DATA(pKernel, Scene, WORLD_BOSS_ACTIVE_CFG_ID, VTYPE_INT);
	pScene->SetDataInt(WORLD_BOSS_ACTIVE_CFG_ID, -1);

	// ���һ�����֣�Ĭ��""��
	ADD_DATA(pKernel, Scene, "Last_Harm", VTYPE_WIDESTR);
	pScene->SetDataWideStr("Last_Harm", L"");

	// Boss����ID(Ĭ��"")
	ADD_DATA(pKernel, Scene, "BOSS_CONFIG_ID", VTYPE_STRING);
	pScene->SetDataString("BOSS_CONFIG_ID", "");

	// ���BOSS��ʱ����������
	ADD_DATA(pKernel, Scene, WORLD_BOSS_ACTIVED, VTYPE_OBJECT);  
	pScene->SetDataObject(WORLD_BOSS_ACTIVED, PERSISTID());
}

/*!
* @brief	member����ͬ��
* @param	����ָ��
* @return	void 
*/
void WorldBossNpc::SyncMemberData(IKernel* pKernel)
{
	IPubSpace* pPubSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (pPubSpace != NULL)
	{
		wchar_t pubDataName[256];
		SWPRINTF_S(pubDataName, L"Domain_SceneList%d", pKernel->GetServerId());
		IPubData* pPubData = pPubSpace->GetPubData(pubDataName);
		if (pPubData != NULL)
		{
			IRecord* pRecord = pPubData->GetRecord(SERVER_MEMBER_FIRST_LOAD_SCENE_REC);
			if (pRecord != NULL)
			{
				// �ַ�����
				CVarList UpdateMsg;
				UpdateMsg << COMMAND_WORLD_BOSS_ACTIVE
						  << SS_WORLD_BOSS_ACTIVE_UPDATE_DATA
						  << m_ActiveTimeCount.m_BeginTime
						  << m_ActiveTimeCount.m_EndTime;

				int iRows = pRecord->GetRows();
				LoopBeginCheck(ax);
				for(int i = 0; i < iRows; ++i)
				{
					LoopDoCheck(ax);

					pKernel->CommandToScene(pRecord->QueryInt(i, SERVER_MEMBER_FIRST_LOAD_SCENE_COL_scene_id), UpdateMsg);
				}
			}
			else
			{
				extend_warning(LOG_ERROR, "[WorldBossNpc::HB_CheckWorldBossActive] pRecord is null");
			}	
		}
		else
		{
			extend_warning(LOG_ERROR, "[WorldBossNpc::HB_CheckWorldBossActive] pPubData is null");
		}
	}
	else
	{
		extend_warning(LOG_ERROR, "[WorldBossNpc::HB_CheckWorldBossActive] pPubSpace is null");
	}
}

/*!
* @brief	��㲥
* @param	����ָ��
* @param	Scene
* @return	void 
*/
void WorldBossNpc::ActiveNotice(IKernel* pKernel, const PERSISTID& Scene)
{
	IGameObj* pScene = pKernel->GetGameObj(Scene);
	if (NULL == pScene)
	{
		return;
	}
	// ���ù����ʶ
	pScene->SetInt(FLAG_WORLD_BOSS_VISIT, FLAG_WORLD_BOSS_VISIT_NOTICED);

	// ֻ��һ���������ӻ��ʼ��ʾ����
	if (pKernel->GetSceneId() != m_nCustomSceneId)
	{
		return;
	}

	// ������������
	int iHBCount = 0;
	int iDistTime = 0;

	int daySecond = util_convert_zerotime_to_sec();
	if (m_NoticeTimeCount.m_BeginTime >= daySecond)
	{
		iDistTime = m_NoticeTimeCount.m_EndTime - m_NoticeTimeCount.m_BeginTime;
	}
	else if (m_NoticeTimeCount.m_BeginTime < daySecond && m_NoticeTimeCount.m_EndTime > daySecond)
	{
		iDistTime = m_NoticeTimeCount.m_EndTime - daySecond;
	}
	iHBCount = iDistTime > 0 ? (iDistTime / 15) : 0;

	// �����
	if (iHBCount > 0)
	{
		ADD_COUNT_BEAT(pKernel, Scene, "WorldBossNpc::HB_CheckNotice", 15000, iHBCount);
	}
}

/*!
* @brief	���ʼ
* @param	����ָ��
* @param	Scene
* @param	pActiveInfo
* @param	pNow
* @param	iState
* @return	void 
*/
void WorldBossNpc::ActiveOpening(IKernel* pKernel, const PERSISTID& Scene, const WorldBossActive_t* pActiveInfo, const tm* pNow, int iState, int nConfigIndex)
{
	// ����
	IGameObj* pScene = pKernel->GetGameObj(Scene);
	if (NULL == pScene)
	{
		return;
	}

	// ���û��ʶ
	pScene->SetInt(FLAG_WORLD_BOSS_VISIT, FLAG_WORLD_BOSS_VISIT_ACTIVED);

	if(!pScene->FindData(FLAG_WORLD_BOSS_CONFIG_DATA_INDEX))
	{
		pScene->AddDataInt(FLAG_WORLD_BOSS_CONFIG_DATA_INDEX, nConfigIndex);
	}
	else
	{
		pScene->SetDataInt(FLAG_WORLD_BOSS_CONFIG_DATA_INDEX, nConfigIndex);
	}

	// ɾ����������
	DELETE_HEART_BEAT(pKernel, Scene, "WorldBossNpc::HB_CheckNotice");

	// �״̬���浽PUB����
	m_pWorldBossNpc->OnSaveActiveStatusToPUB(pKernel, pKernel->GetSceneId(), 
		iState, pScene->QueryDataInt(WORLD_BOSS_ACTIVE_CFG_ID));

	// �ʣ��ʱ��
	int iActiveTime = GetRemainTime(pActiveInfo);

	// ����ˢboss����
	ADD_COUNT_BEAT(pKernel, Scene, "WorldBossNpc::HB_BossBorn", m_kConstConfig.nReadyTime, 1);

	// ���Ӳ���CG������
	ADD_COUNT_BEAT(pKernel, Scene, "WorldBossNpc::HB_StartPlayCG", m_kConstConfig.nBornCGTime, 1);

	ADD_HEART_BEAT(pKernel, Scene, "WorldBossNpc::HB_ShowAttackRank", 5000);

	CVarList PubMsg;
	PubMsg << SP_WORLD_BOSS_MSG_STATUS
		<< SP_UPDATE_BOSS_REC_INFO
		<< pKernel->GetSceneId()
		<< L""
		<< STATE_WBOSS_ACTIVE_OPEN;
	 
	SendMsgToPubServer(pKernel, PubMsg);

	if (pKernel->GetSceneId() != m_nCustomSceneId)
	{
		return;
	} 
	// ������Ϣ���ͻ��ˣ�֪ͨ�����
	CVarList TransMsg;
	TransMsg << SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE 
			 << SC_WORLD_BOSS_ACTIVE_OPEN 
			 << iActiveTime;
	pKernel->CustomByWorld(TransMsg);

	// ������Ϣ���ͻ��ˣ���ʾ����봰��
	TransMsg.Clear();
	TransMsg << SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE
			 << SC_WORLD_BOSS_ACTIVE_SHOW_ENTRY_PANEL;
	pKernel->CustomByWorld(TransMsg);
}

/*!
* @brief	�����
* @param	����ָ��
* @param	Scene
* @param	iState
* @return	void 
*/
void WorldBossNpc::ActiveClosed(IKernel* pKernel, const PERSISTID& Scene, int iState)
{
	IGameObj* pScene = pKernel->GetGameObj(Scene);
	if (NULL == pScene)
	{
		return;
	}

	// ���ý�����ʶ
	pScene->SetInt(FLAG_WORLD_BOSS_VISIT, FLAG_WORLD_BOSS_VISIT_CLOSED);

	// �״̬���浽PUB����
	m_pWorldBossNpc->OnSaveActiveStatusToPUB(pKernel, pKernel->GetSceneId(), 
		iState, pScene->QueryDataInt(WORLD_BOSS_ACTIVE_CFG_ID));

	// BOSS����
	PERSISTID WorldBoss = pScene->QueryDataObject(WORLD_BOSS_ACTIVED);
	// WorldBossΪNull ˵��boss�Ѿ���ɱ���������
	bool bKilled = WorldBoss.IsNull();
	if (!bKilled)
	{
		// �˺����ݿ���
		m_pWorldBossNpc->CopyAndSortHurtRecordFromAI(pKernel, WorldBoss, PERSISTID());

		// ��������
		m_pWorldBossNpc->ActiveRankAward(pKernel, Scene, FAIL_KILL_WORLD_BOSS);

		// ����BOSS����
		pKernel->Destroy(Scene, WorldBoss);

		// boss̫����,���𽵵�
		UpdateGrowUpLevel(pKernel, false);

		// ֪ͨ�����
		m_pWorldBossNpc->NoticeActiveFinish(pKernel, Scene, bKilled);
	}
}

// ��ȡ����boss�����԰� 
int	WorldBossNpc::QueryCurPackageId(IKernel* pKernel)
{
	IPubData* pWBPubData = GetPubData(pKernel);
	if (NULL == pWBPubData)
	{
		return 0;
	}
	IRecord* pGrowUpRec = pWBPubData->GetRecord(PUB_WORLD_BOSS_GROW_UP_REC);
	if (NULL == pGrowUpRec)
	{
		return 0;
	}
	int nSceneId = pKernel->GetSceneId();

	int nCurLevel = 0;
	int nRowIndex = pGrowUpRec->FindInt(WORLD_BOSS_GROW_UP_REC_COL_SCENEID, nSceneId);
	if (nRowIndex != -1)
	{
		nCurLevel = pGrowUpRec->QueryInt(nRowIndex, WORLD_BOSS_GROW_UP_REC_COL_LEVEL);
	}

	const WorldBossGrowUp* pGrowData = GetGrowUpData(nSceneId);
	if (NULL == pGrowData)
	{
		return 0;
	}

	// ���ܳ������ȼ�
	nCurLevel = __min(nCurLevel, pGrowData->nMaxLevel);

	return pGrowData->nBasePackageId + nCurLevel;
}

// ��ȡ�ɳ�����
const WorldBossGrowUp* WorldBossNpc::GetGrowUpData(int nSceneId)
{
	WorldBossGrowUp* pFind = NULL;
	int nSize = m_vecWorldBossGrowUp.size();
	LoopBeginCheck(q);
	for (int i = 0; i < nSize;++i)
	{
		LoopDoCheck(q);
		WorldBossGrowUp* pData = &m_vecWorldBossGrowUp[i];
		if (pData != NULL && pData->nSceneId == nSceneId)
		{
			pFind = pData;
			break;
		}
	}

	return pFind;
}

// ����boss�ȼ�
void WorldBossNpc::UpdateGrowUpLevel(IKernel* pKernel, bool bBeKilled)
{
	IGameObj* pSceneObj = pKernel->GetSceneObj();
	if (NULL == pSceneObj)
	{
		return;
	}

	// ��ȡ�ɳ�������
	int nSceneId = pKernel->GetSceneId();
	const WorldBossGrowUp* pGrowData = GetGrowUpData(nSceneId);
	if (NULL == pGrowData)
	{
		return;
	}

	int nChangeLv = 0;
	if (bBeKilled)
	{
		int64_t nBornTime = pSceneObj->QueryInt64(FIELD_PROP_WORLD_BOSS_CREATE_TIME);
		int64_t nDurTime = ::util_get_utc_time() - nBornTime;
		if (nDurTime < pGrowData->nKillDurationTime)
		{
			nChangeLv = pGrowData->nIncrement;
		}
	}
	else
	{
		nChangeLv = pGrowData->nIncrement * -1;
	}
	
	// �ޱ仯
	if (0 == nChangeLv)
	{
		return;
	}

	CVarList PubMsg;
	PubMsg << SP_WORLD_BOSS_MSG_STATUS
		<< SP_WORLD_BOSS_BE_KILL_TIME_SET
		<< pKernel->GetSceneId()
		<< nChangeLv
		<< pGrowData->nMaxLevel;

	SendMsgToPubServer(pKernel, PubMsg);
}