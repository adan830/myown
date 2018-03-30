//--------------------------------------------------------------------
// �ļ���:      FriendModule.h
// ��  ��:      ����ģ��
// ˵  ��:		
// ��������:    2014��10��17��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#include "FriendModule.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/CommonModule/SnsDataModule.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"

#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/FriendDefine.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/Define/FriendMsgDefine.h"
#include "FsGame/Define/SnsDefine.h"
#include "FsGame/Define/SnsDataDefine.h"
#include "FsGame/Define/StaticDataDefine.h"
#include "FsGame/Define/PlayerBaseDefine.h"

#include "utils/util_func.h"
#include "utils/custom_func.h"
#include "utils/util_func.h"
#include "utils/string_util.h"
#include "utils/extend_func.h"
#include "SystemFunctionModule/CapitalModule.h"
#include "CommonModule/EnvirValueModule.h"
#include "NpcBaseModule/AI/AIDefine.h"
//#include "TeamModule.h"
//#include "PartnerModule.h"
#include "CommonModule/ReLoadConfigModule.h"
//#include "TradeModule/VipModule.h"
//#include "Define/VipDefine.h"
#include "Define/ResetTimerDefine.h"
#include "SystemFunctionModule/ResetTimerModule.h"
#include "Interface/ILuaExtModule.h"
#include "CommonModule/LuaExtModule.h"
#include "SkillModule/BufferModule.h"
#include "SystemFunctionModule/RewardModule.h"

#define CONFIG_FRIEND_XML_GIFT "ini\\SocialSystem\\Friend\\FriendGift.xml"
#define CONFIG_FRIEND_XML_INTIMACY_UPGRADE "ini\\SocialSystem\\Friend\\IntimacyLevelUp.xml"
#define CONFIG_FRIEND_XML_SWORN_BUFF "ini\\SocialSystem\\Friend\\SwornBuff.xml"
#define CONFIG_FRIEND_XML_TEAM_ADD_INTIMACY "ini\\SocialSystem\\Friend\\TeamAddIntimacy.xml"
#define TEAM_FRIEND_GAIN "ini\\SocialSystem\\Friend\\TeamFriendGain.xml"
#define TEAM_FRIEND_INTIMACY "ini\\SocialSystem\\Friend\\TeamFriendIntimacy.xml"
#define TEAM_FRIEND_INITMACY_AWARD "ini\\SocialSystem\\Friend\\FriendIntimacyLvAward.xml"

#define FRIEND_FLOWER_TIMER 10800		// 3��Сʱ

std::wstring		FriendModule::m_domainName = L"";
FriendModule *		FriendModule::m_pFriendModule = NULL;
RankListModule *	FriendModule::m_pRankListModule = NULL;
//PartnerModule *     FriendModule::m_pPartnerModule = NULL;
CapitalModule*		FriendModule::m_pCapitalModule = NULL;
RevengeArenaModule* FriendModule::m_pRevengeArenaModule = NULL;
TeamModule*			FriendModule::m_pTeamModule = NULL;


int nx_add_friend_enemy(void *state)
{

	IKernel* pKernel = LuaExtModule::GetKernel(state);

	//����������
	CHECK_ARG_NUM(state, nx_add_friend_enemy, 1);

	// ����������
	CHECK_ARG_OBJECT(state, nx_add_friend_enemy, 1);
		//��ȡ����
	PERSISTID self = pKernel->LuaToObject(state, 1);
	int nScore = pKernel->LuaToInt(state, 2);
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return 0;
	}
	const char* uid = pKernel->QueryString(self, FIELD_PROP_UID);
	int nHaterT = 100;
	CVarList row_list;
	row_list << uid
		<< pSelfObj->QueryWideStr("Name")
		<< pSelfObj->QueryInt("Level")
		<< pSelfObj->QueryInt("Job")
		<< pSelfObj->QueryInt("Sex")
		<< pSelfObj->QueryInt("BattleAbility")
		<< pSelfObj->QueryWideStr("GuildName")
		<< pSelfObj->QueryString(FIELD_PROP_ACCOUNT)
		<< ONLINE
		<< nHaterT		// ���ֵ
		<< ::time(NULL);


	CVarList args;
	args.Append(row_list, 0, 9);
	args << nHaterT << ::time(NULL);


	CVarList snsInfo;
	snsInfo << SPACE_SNS
		<< SNS_FRIEND_SPACE
		<< SNS_MSG_FRIEND_ADD_ENEMY
		<< args;

	const char* playerUid = uid;
	pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
		pKernel->GetSceneId(), playerUid,
		snsInfo);
	return 1;
}


// ��ʼ��
bool FriendModule::Init(IKernel* pKernel)
{
	//ģ����ؼ����Գ�ʼ��
	m_pFriendModule = this;
	m_pRankListModule = (RankListModule *)pKernel->GetLogicModule("RankListModule");
    //m_pPartnerModule = (PartnerModule *)pKernel->GetLogicModule("PartnerModule");
	m_pCapitalModule = dynamic_cast<CapitalModule*>(pKernel->GetLogicModule("CapitalModule"));
	//m_pTeamModule = dynamic_cast<TeamModule*>(pKernel->GetLogicModule("TeamModule"));

	Assert(m_pFriendModule != NULL &&
		m_pRankListModule != NULL &&
		//m_pPartnerModule != NULL && 
		m_pCapitalModule != NULL);
//		NULL != m_pTeamModule);

	// �¼��ص�
	pKernel->AddEventCallback("player", "OnRecover", FriendModule::OnRecover, 1001);
	pKernel->AddEventCallback("player", "OnStore", FriendModule::OnStore);
	pKernel->AddEventCallback("player", "OnReady",	FriendModule::OnReady);

	// ����SNS��������Ϣ
	pKernel->AddEventCallback("scene", "OnSnsMessage", FriendModule::OnSnsMessage);

	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_FRIEND, FriendModule::OnCustomMessage);

	// �ڲ���Ϣ
	pKernel->AddIntCommandHook("player", COMMAND_SNS_DATA, OnPlayerSnsCommand);
	pKernel->AddIntCommandHook("player", COMMAND_FRIEND_MSG, FriendModule::OnCommandMessage);
	pKernel->AddIntCommandHook("player", COMMAND_BEKILL, FriendModule::OnCommandBeKill);
	//pKernel->AddIntCommandHook("player", COMMAND_KILLTARGET, FriendModule::OnCommandKillTarget);
	pKernel->AddIntCommandHook("player", COMMAND_LEVELUP_CHANGE, FriendModule::OnCommandLevelChange);


// 	pKernel->AddIntCommandHook("player", COMMAND_TEAM_JOIN, OnCommandRefreshFriendGain, 100);
// 	// ��ӳ�Ա
// 	pKernel->AddIntCommandHook("player", COMMAND_TEAM_ADD_MEMBER, OnCommandRefreshFriendGain, 100);
// 	// ɾ����Ա
// 	pKernel->AddIntCommandHook("player", COMMAND_TEAM_REMOVE_MEMBER, OnCommandRefreshFriendGain, 100);
// 	// �˳�����
// 	pKernel->AddIntCommandHook("player", COMMAND_TEAM_CLEAR, OnCommandRefreshFriendGain, 100);




	DECL_LUA_EXT(nx_add_friend_enemy);
	// ���Իص�
 	DECL_CRITICAL(FriendModule::C_OnDCBattleAbilityChange);
// 	DECL_CRITICAL(FriendModule::C_OnDCGuildNameChange);
// 	DECL_CRITICAL(FriendModule::C_OnDCNationChange);
 	DECL_CRITICAL(FriendModule::C_OnDCTeamIDChange);
// 	DECL_CRITICAL(FriendModule::C_OnVIPLevelChanged);

	DECL_RESET_TIMER(RESET_FRIEND_WEEK_FLOWER_NUM, FriendModule::ResetWeekFlowerNum);
	// ���ö�ʱ��
	DECL_RESET_TIMER(RESET_DAILY_FRIEND, FriendModule::ResetDaily);
	RELOAD_CONFIG_REG("FriendConfig", ReloadConfig);
	return LoadConfig( pKernel );
}

// �ͷ�
bool FriendModule::Shut(IKernel* pKernel)
{
	return true;
}

bool FriendModule::LoadConfig(IKernel* pKernel)
{
	std::string xmlPath = pKernel->GetResourcePath();

	std::string xmlGift = xmlPath + CONFIG_FRIEND_XML_GIFT;
	Configure<ConfigFriendGiftItem>::Release();
	if (!Configure<ConfigFriendGiftItem>::LoadXml(xmlGift.c_str(), OnConfigError))
	{
		return false;
	}

	std::string xmlIntimacyUpgrade= xmlPath + CONFIG_FRIEND_XML_INTIMACY_UPGRADE;
	Configure<ConfigFriendIntimacyUpgradeItem>::Release();
	if (!Configure<ConfigFriendIntimacyUpgradeItem>::LoadXml(xmlIntimacyUpgrade.c_str(), OnConfigError))
	{
		return false;
	}

	std::string xmlSwornBuff = xmlPath + CONFIG_FRIEND_XML_SWORN_BUFF;
	Configure<ConfigSwornBuff>::Release();
	if (!Configure<ConfigSwornBuff>::LoadXml(xmlSwornBuff.c_str(), OnConfigError))
	{
		return false;
	}
	std::string teamAddintimacy = xmlPath + CONFIG_FRIEND_XML_TEAM_ADD_INTIMACY;
	Configure<ConfigTeamAddIntimacy>::Release();
	if (!Configure<ConfigTeamAddIntimacy>::LoadXml(teamAddintimacy.c_str(), OnConfigError))
	{
		return false;
	}


	return 	 LoadTeamFirendIntimacy(pKernel)
		&& LoadTeamFriendConfig(pKernel)
		&& LoadFirendIntimacyAward(pKernel);
}


void FriendModule::ReloadConfig(IKernel* pKernel)
{
	FriendModule::m_pFriendModule->LoadConfig(pKernel);
}

//�������
int FriendModule::OnRecover(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
	IGameObj * pPalyer = pKernel->GetGameObj(self);
	if (NULL == pPalyer)
	{
		return 0;
	}

	if (pPalyer->QueryInt("PrepareRole") == ROLE_FLAG_ROBOT)
	{
		//���˵�������
		return 0;
	}

	REGIST_RESET_TIMER(pKernel, self, RESET_FRIEND_WEEK_FLOWER_NUM);
	REGIST_RESET_TIMER(pKernel, self, RESET_DAILY_FRIEND);

	// ���Իص�
 	ADD_CRITICAL(pKernel, self, "BattleAbility", "FriendModule::C_OnDCBattleAbilityChange");
// 	ADD_CRITICAL(pKernel, self, "GuildName", "FriendModule::C_OnDCGuildNameChange");
	//ADD_CRITICAL(pKernel, self, FIELD_PROP_NATION, "FriendModule::C_OnDCNationChange");
 	ADD_CRITICAL(pKernel, self, "TeamID", "FriendModule::C_OnDCTeamIDChange");
// 	ADD_CRITICAL(pKernel, self, "VipLevel", "FriendModule::C_OnVIPLevelChanged");

	int nOnline = pPalyer->QueryInt(FIELD_PROP_ONLINE);

	const char * pMyUid = pPalyer->QueryString("Uid");
	int nMyLevel = pPalyer->QueryInt("Level");
	int nMyJob = pPalyer->QueryInt("Job");
	int nMySex = pPalyer->QueryInt("Sex");
	int nAbility = pPalyer->QueryInt("BattleAbility");
	const wchar_t *pGuildName = pPalyer->QueryWideStr("GuildName");
	//int teamID = pPalyer->QueryInt(FIELD_PROP_TEAM_ID);
	int vipLevel = pPalyer->QueryInt(FIELD_PROP_VIP_LEVEL);
	IRecord* pRec = pPalyer->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return 0;
	}

	int iMaxRows = pRec->GetRows();

	CVarList var;
    LoopBeginCheck(a);
	for (int i = 0; i < iMaxRows; ++i)
	{	
        LoopDoCheck(a);
		const wchar_t* pPlayerName = pRec->QueryWideStr(i, COLUMN_FRIEND_REC_NAME);

		// ���߲ŷ���֤
		if (pKernel->GetPlayerScene(pPlayerName) <= 0)
		{
			continue;
		}

		var.Clear();
		// ���������Ϣ����	(Ӧ�ü��ϵȼ�\����\UID)
		var << COMMAND_FRIEND_MSG << SS_FRIEND_ONLINE_STATUS_UPDATE << pMyUid << ONLINE;
		pKernel->CommandByName( pPlayerName, var);
	}

	// ���¹����ռ�
	IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
	if (NULL == pOnlinePlayerRec)
	{
		return 0;
	}

	if (pOnlinePlayerRec->FindString(PUB_COL_PLAYER_UID, pMyUid) >= 0)
	{
		return 0;
	}
	
	if (ONLINE == nOnline)
	{
		//�������������Ϣ
		CVarList pubMsgList;
		pubMsgList << PUBSPACE_DOMAIN
			<< m_pFriendModule->GetDomainName(pKernel).c_str()
			<< FRIEND_SEVER_PUB_ADD_ONLINE_PLAYER_INFO
			<< pMyUid
			<< pKernel->SeekRoleName(pMyUid)
			<< nMyLevel
			<< nMyJob
			<< nMySex
			<< nAbility
			<< pGuildName
			<< 0//teamID
			<< vipLevel;

		pKernel->SendPublicMessage(pubMsgList);

		// ����Ϣ��sns����������º�����Ϣ
		m_pFriendModule->OnCustomQueryFriendInfo(pKernel, self);
	}

	//m_pFriendModule->RefreshTeamFriendGain(pKernel, self);
	//////////////////////////////////////////////////////////////////////////
	// SNS��������ѯ����

	// ѯ��SNS����������������
	pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
		pKernel->GetSceneId(), pMyUid,
		CVarList() << SPACE_SNS << SNS_FRIEND_SPACE << SNS_MSG_FRIEND_QUERY_FRIEND_RESULT << pMyUid << self);

	// ѯ��SNS���������������б�
	pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
		pKernel->GetSceneId(), pMyUid,
		CVarList() << SPACE_SNS << SNS_FRIEND_SPACE << SNS_MSG_FRIEND_QUERY_FRIEND_APPLY << pMyUid << self);

	// ѯ��SNS�����������ͻ��б�
	pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
		pKernel->GetSceneId(), pMyUid,
		CVarList() << SPACE_SNS << SNS_FRIEND_SPACE << SNS_MSG_FRIEND_GIFT_GET << self);

	// ѯ��SNS������������ѹ�ϵ�б�
	pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
		pKernel->GetSceneId(), pMyUid,
		CVarList() << SPACE_SNS << SNS_FRIEND_SPACE << SNS_MSG_FRIEND_QUERY_FRIEND_DELETE << pMyUid << self);
	//��ȡ���Ӻ������ܶ�
	pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
		pKernel->GetSceneId(), pMyUid,
		CVarList() << SPACE_SNS << SNS_FRIEND_SPACE << SNS_MSG_FRIEND_ADD_INTIMACY_GET << self);

	//���ɾ���������
	pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
		pKernel->GetSceneId(), pMyUid,
		CVarList() << SPACE_SNS << SNS_FRIEND_SPACE << SNS_MSG_FRIEND_QUERY_DEL_SWORN << self);
	//��ȡ����
	pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
		pKernel->GetSceneId(), pMyUid,
		CVarList() << SPACE_SNS << SNS_FRIEND_SPACE << SNS_MSG_FRIEND_QUERY_ENEMY << self);



	return 0;
}

// �������
int FriendModule::OnStore(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return 0;
    }
	int reason = args.IntVal(0);
	if (reason == STORE_EXIT)
	{
		const char * pMyUid = pSelfObj->QueryString("Uid");

		IRecord* pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
		if (NULL == pRec)
		{
			return 0;
		}

		int iMaxRows = pRec->GetRows();

		CVarList var;
        LoopBeginCheck(b);
		for (int i = 0; i < iMaxRows; ++i)
		{	
            LoopDoCheck(b);
			const wchar_t* pPlayerName = pRec->QueryWideStr(i, COLUMN_FRIEND_REC_NAME);

			// ���߲ŷ���֤
			if (pKernel->GetPlayerScene(pPlayerName) <= 0)
			{
				continue;
			}

			var.Clear();
			var << COMMAND_FRIEND_MSG << SS_FRIEND_ONLINE_STATUS_UPDATE << pMyUid << OFFLINE;
			pKernel->CommandByName( pPlayerName, var);
		}

		//���¹����ռ�
		IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
		if (NULL == pOnlinePlayerRec)
		{
			return 0;
		}

		if (pOnlinePlayerRec->FindString(PUB_COL_PLAYER_UID, pMyUid) < 0)
		{
			return 0;
		}

		CVarList pubMsgList;
		pubMsgList << PUBSPACE_DOMAIN
				<< GetDomainName(pKernel).c_str()
				<< FRIEND_SEVER_PUB_DEL_ONLINE_PLAYER_INFO
				<< pMyUid;

		pKernel->SendPublicMessage(pubMsgList);
	}

	return 0;
}

// ���׼������
int FriendModule::OnReady(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
	//if (!pKernel->Exists(self))
	//{
	//	return 0;
	//}

	//IGameObj* pPlayer = pKernel->GetGameObj(self);
	//if (NULL == pPlayer)
	//{
	//	return 0;
	//}

	//const wchar_t *selfName = pPlayer->GetName();
	//const char *selfUid = pPlayer->QueryString("Uid");
	//if (StringUtil::CharIsNull(selfName) ||
	//	StringUtil::CharIsNull(selfUid))
	//{
	//	return 0;
	//}

	


	return 0;
}

// �Ƿ�Ϊ����
bool FriendModule::IsEnemy(IKernel* pKernel, const PERSISTID& self, const char* strUid)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj || StringUtil::CharIsNull(strUid))
	{
		return false;
	}

	// �Ƿ��Ѿ��ǳ���
	IRecord * pEnemyRecord = pSelfObj->GetRecord(FIELD_RECORD_ENEMY_REC);
	if (NULL == pEnemyRecord)
	{
		return false;
	}

	int nEnemyRow = pEnemyRecord->FindString(COLUMN_ENEMY_REC_UID, strUid);
	if (nEnemyRow > -1)
	{
		// �ǳ���
		return true;
	}

	return false;
}

// ����sns��Ϣ
int FriendModule::OnSnsMessage(IKernel * pKernel, const PERSISTID & self,const PERSISTID & sender, const IVarList & args)
{
	int msgid = args.IntVal(0);
	switch (msgid)
	{
	case SNS_MSG_FRIEND_PUSH_APPLY: // ��������
		OnSnsQuestFriendApplyRPL(pKernel, args);
		break;
	case SNS_MSG_FRIEND_PUSH_DELETE: // ���ͽ����ϵ
		OnSnsQuestFriendDeleteRPL(pKernel, args);
		break;
	case SNS_MSG_FRIEND_PUSH_RESULT: // ���ͽ��
		OnSnsQuestFriendApplyResultRPL(pKernel, args);
		break;
	case SNS_MSG_FRIEND_GIFT_PUSH: // �����ͻ�
		OnSnsQuestFriendGiftPush(pKernel, args);
		break;
	case SNS_MSG_FRIEND_ADD_INTIMACY_RESULT:
		OnSnsAddFriendIntimacyPush(pKernel,args);
		break;
	case SNS_MSG_FRIEND_PUSH_DEL_SWORN:
		OnSnsDelSworn(pKernel,args);
		break;
	case SNS_MSG_FRIEND_PUSH_ENEMY:
		OnSnsAddEnemy(pKernel, args);
		break;
	default:
		break;
	}

	return 0;
}

// ��ɱ�ص�
int FriendModule::OnCommandBeKill(IKernel* pKernel, const PERSISTID& self,	const PERSISTID& sender, const IVarList& args)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return 0;
    }
	// û��
	if (pSelfObj->QueryInt("Dead") == 0)
	{
		return 0;
	}

	m_pFriendModule->OnCustomAddOnceEnemy(pKernel, self, sender);

	return 0;
}

// int FriendModule::OnCommandKillTarget(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
// {
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (NULL == pSelfObj)
// 	{
// 		return 0;
// 	}
// 
// 	const PERSISTID& target = args.ObjectVal(1);
// 	IGameObj* pKillTarget = pKernel->GetGameObj(target);
// 	if (NULL == pKillTarget)
// 	{
// 		return 0;
// 	}
// 
// 	// û��
// 	if (pKillTarget->QueryInt("Dead") == 0)
// 	{
// 		return 0;
// 	}
// 
// 	// ���ɱ���������, ����ٳ��ֵ
// 	//if (pKillTarget->GetClassType() == TYPE_PLAYER)
// 	//{ �������޳�����ֻ�������
// 	if (m_pRevengeArenaModule->IsRevengeArena(pKernel))
// 	{
// 		int nHatar = EnvirValueModule::EnvirQueryInt(ENV_VALUE_PLAYER_REVENGE_WIN_HATREDT);
// 		// ������uid������ֲ��� [9/24/2016 lihl]
// 		const char* pszUid = pKillTarget->QueryString(FIELD_PROP_UID);
// 		if (StringUtil::CharIsNull(pszUid))
// 		{
// 			const wchar_t* pwszName = pKillTarget->QueryWideStr(FIELD_PROP_NAME);
// 			if (NULL != pwszName)
// 			{
// 				pszUid = pKernel->SeekRoleUid(pwszName);
// 			}
// 		}
// 		m_pFriendModule->OnDecEnmeyHaterT(pKernel, pSelfObj, pszUid, nHatar);
// 	}
// 	
// 	else
// 	{
// 		// �ж��Ƿ������
// 		if (m_pTeamModule->IsInTeam(pKernel, self))
// 		{
// 			
// 			int npcType = pKillTarget->QueryInt(FIELD_PROP_NPC_TYPE);
// 			if (pKillTarget->GetClassType() != TYPE_NPC){
// 				return 0;
// 			}
// 			
// 				// ��ȡ�����еĺ���
// 				CVarList teamList;
// 				m_pTeamModule->GetTeamMemberList(pKernel, self, teamList);
// 				
// 				int nMaxIntLevel = 1;
// 				IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
// 				if (NULL == pFriendRec)
// 				{
// 					return 0;
// 				}			
// 
// 				CVarList friendList;
// 				int nTeamCount = teamList.GetCount();
// 				LoopBeginCheck(a);
// 				for (int i = 0; i < nTeamCount; ++i)
// 				{
// 					LoopDoCheck(a);
// 					const wchar_t* pTeamName = teamList.WideStrVal(i);
// 					const PERSISTID& player = pKernel->FindPlayer(pTeamName);
// 					if (!pKernel->Exists(player)){
// 						continue;
// 					}
// 					int nRow = pFriendRec->FindWideStr(COLUMN_FRIEND_REC_NAME, pTeamName);
// 					if ( nRow != -1 )
// 					{
// 						friendList << pTeamName;
// 					}
// 				
// 
// 				// ����к����ڶ�����
// 				if (friendList.GetCount() > 0)
// 				{
// 					int nExp = m_pFriendModule->GetTeamAddIntimacyValue(TEAM_ADD_INTIMACY_KILL_NPC);
// 					m_pFriendModule->IncTeamFriendIntimacy(pKernel, pSelfObj, friendList, nExp);
// 				}
// 			}
// 		}
// 	}
// 
// 	return 0;
// }

//�ȼ��仯�ص�
int FriendModule::OnCommandLevelChange(IKernel* pKernel, const PERSISTID& self,	const PERSISTID& sender, const IVarList& args)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return 0;
    }
    // �б�
    IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
    if (NULL == pRec)
    {
        return 0;
    }

    // �ȼ�
    const int nLevel = pSelfObj->QueryInt("Level");

    // ���Uid
    const char * pUid = pSelfObj->QueryString("Uid");
    if (StringUtil::CharIsNull(pUid))
    {
        return 0;
    }

    // ˢ�º��ѱ�
    int nRow = pRec->GetRows();
    LoopBeginCheck(c);
    for (int i = 0; i < nRow; ++i)
    {
        LoopDoCheck(c);
        // ȡ����
        const wchar_t * friendname = pRec->QueryWideStr(i, COLUMN_FRIEND_REC_NAME);
        if (!friendname)
        {
            continue;
        }

        // ������������
        int scene = pKernel->GetPlayerScene(friendname);
        if (scene > 0)
        {
            // ���µȼ�
            pKernel->CommandByName(friendname, CVarList() << COMMAND_FRIEND_MSG << SS_FRIEND_LEVEL_UPDATE << pUid << nLevel);
        }
    }

    // ����������ұ�
    IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
    if (NULL == pOnlinePlayerRec)
    {
        return 0;
    }

    if (pOnlinePlayerRec->FindString(PUB_COL_PLAYER_UID, pUid) < 0)
    {
        return 0;
    }

    CVarList pubMsgList;
    pubMsgList 
        << PUBSPACE_DOMAIN
        << GetDomainName(pKernel).c_str()
        << FRIEND_SEVER_PUB_SET_LEVEL_INFO
        << pUid
        << nLevel;

    pKernel->SendPublicMessage(pubMsgList);
	m_pFriendModule->RecommendFriendByPlayerLv(pKernel,self);
    return 0;
}


// ս�����仯�ص�
int FriendModule::C_OnDCBattleAbilityChange(IKernel * pKernel, const PERSISTID & self, 
											const char * property, const IVar & old)
{
	//�������
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (pSelfObj == NULL)
    {
        return 0;
    }
	const char * pMyUid = pSelfObj->QueryString("Uid");
	if (StringUtil::CharIsNull(pMyUid))
	{
		return 0;
	}

	//�к��ѱ�ˢ��
	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return 0;
	}

	//�Լ�ս����
	int nBattleAbility = pSelfObj->QueryInt("BattleAbility");

	//ˢ�º��ѱ�
	int nRow = pRec->GetRows();
    LoopBeginCheck(d);
	for (int i = 0; i < nRow; ++i)
	{
        LoopDoCheck(d);
		//ȡ����
		const wchar_t * pFriendName = pRec->QueryWideStr(i, COLUMN_FRIEND_REC_NAME);
		if (StringUtil::CharIsNull(pFriendName))
		{
			continue;
		}

		//������������
		int scene = pKernel->GetPlayerScene(pFriendName);
		if (scene > 0)
		{
			//����ս����
			pKernel->CommandByName(pFriendName, CVarList() << COMMAND_FRIEND_MSG << SS_FRIEND_BATTLEABILITY_UPDATE << pMyUid << nBattleAbility);
		}
	}

	//�����Ƽ������
	IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
	if (NULL == pOnlinePlayerRec)
	{
		return 0;
	}

	if (pOnlinePlayerRec->FindString(PUB_COL_PLAYER_UID, pMyUid) < 0)
	{
		return 0;
	}

	CVarList pubMsgList;
	pubMsgList 
		<< PUBSPACE_DOMAIN
		<< GetDomainName(pKernel).c_str()
		<< FRIEND_SEVER_PUB_SET_BATTLEABILITY_INFO
		<< pMyUid
		<< nBattleAbility;

	pKernel->SendPublicMessage(pubMsgList);

	return 1;
}

int FriendModule::C_OnDCTeamIDChange(IKernel*pKernel, const PERSISTID & self, const char * property, const IVar & old)
{
	//�������
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}
	const char * pMyUid = pSelfObj->QueryString("Uid");
	if (StringUtil::CharIsNull(pMyUid))
	{
		return 0;
	}

	//�к��ѱ�ˢ��
	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return 0;
	}

	//�Լ�����id
	int teamID = pSelfObj->QueryInt(FIELD_PROP_TEAM_ID);

	//ˢ�º��ѱ�
	int nRow = pRec->GetRows();
	LoopBeginCheck(d);
	for (int i = 0; i < nRow; ++i)
	{
		LoopDoCheck(d);
		//ȡ����
		const wchar_t * pFriendName = pRec->QueryWideStr(i, COLUMN_FRIEND_REC_NAME);
		if (StringUtil::CharIsNull(pFriendName))
		{
			continue;
		}

		//������������
		int scene = pKernel->GetPlayerScene(pFriendName);
		if (scene > 0)
		{
			//����ս����
			pKernel->CommandByName(pFriendName, CVarList() << COMMAND_FRIEND_MSG << SS_FRIEND_UPDATE_TEAM_ID << pMyUid << teamID);
		}
	}

	//�����Ƽ������
	IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
	if (NULL == pOnlinePlayerRec)
	{
		return 0;
	}

	if (pOnlinePlayerRec->FindString(PUB_COL_PLAYER_UID, pMyUid) < 0)
	{
		return 0;
	}

	CVarList pubMsgList;
	pubMsgList
		<< PUBSPACE_DOMAIN
		<< GetDomainName(pKernel).c_str()
		<< FRIEND_SEVER_PUB_SET_TEAM_ID
		<< pMyUid
		<< teamID;

	pKernel->SendPublicMessage(pubMsgList);

	return 1;
}

// int FriendModule::C_OnVIPLevelChanged(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old)
// {
// 
// 	//�������
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (pSelfObj == NULL)
// 	{
// 		return 0;
// 	}
// 	const char * pMyUid = pSelfObj->QueryString("Uid");
// 	if (StringUtil::CharIsNull(pMyUid))
// 	{
// 		return 0;
// 	}
// 
// 	//�к��ѱ�ˢ��
// 	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
// 	if (NULL == pRec)
// 	{
// 		return 0;
// 	}
// 
// 	int vipLevel = pSelfObj->QueryInt(FIELD_PROP_VIP_LEVEL);
// 	
// 	//ˢ�º��ѱ�
// 	int nRow = pRec->GetRows();
// 	LoopBeginCheck(d);
// 	for (int i = 0; i < nRow; ++i)
// 	{
// 		LoopDoCheck(d);
// 		//ȡ����
// 		const wchar_t * pFriendName = pRec->QueryWideStr(i, COLUMN_FRIEND_REC_NAME);
// 		if (StringUtil::CharIsNull(pFriendName))
// 		{
// 			continue;
// 		}
// 
// 		//������������
// 		int scene = pKernel->GetPlayerScene(pFriendName);
// 		if (scene > 0)
// 		{
// 			pKernel->CommandByName(pFriendName, CVarList() << COMMAND_FRIEND_MSG << SS_FRIEND_UPDATE_VIP << pMyUid << vipLevel);
// 		}
// 	}
// 
// 	CVarList pubMsgList;
// 	pubMsgList
// 		<< PUBSPACE_DOMAIN
// 		<< GetDomainName(pKernel).c_str()
// 		<< FRIEND_SEVER_PUB_SET_VIP
// 		<< pMyUid
// 		<< vipLevel;
// 
// 	pKernel->SendPublicMessage(pubMsgList);
// 	m_pFriendModule-> RecommendFriendByVipLv(pKernel,self);
// 	return 0;
// }

void FriendModule::OnPlayerGiftChanged(IKernel* pKernel, IGameObj* pSelf, int nMySumFlower, int nMyWeekFlower)
{
	if (pSelf == NULL)
	{
		return;
	}
	const char * pMyUid = pSelf->QueryString( FIELD_PROP_UID );
	if (StringUtil::CharIsNull(pMyUid))
	{
		return;
	}

	//�к��ѱ�ˢ��
	IRecord * pRec = pSelf->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return;
	}

	//ˢ�º��ѱ�
	int nRow = pRec->GetRows();
	LoopBeginCheck(d);
	for (int i = 0; i < nRow; ++i)
	{
		LoopDoCheck(d);
		//ȡ����
		const wchar_t * pFriendName = pRec->QueryWideStr(i, COLUMN_FRIEND_REC_NAME);
		if (StringUtil::CharIsNull(pFriendName))
		{
			continue;
		}

		//������������
		int scene = pKernel->GetPlayerScene(pFriendName);
		if (scene > 0)
		{
			//����ս����
			pKernel->CommandByName(pFriendName,
				CVarList() << COMMAND_FRIEND_MSG 
				<< SS_FRIEND_FLOWER_UPDATE
							<< pMyUid << nMySumFlower << nMyWeekFlower);
		}
	}
}

void FriendModule::OnDecEnmeyHaterT(IKernel* pKernel, IGameObj* pSelf, const char* pEnemyUid, int nHatarT)
{
	if (pSelf == NULL)
	{
		return;
	}

	if (StringUtil::CharIsNull(pEnemyUid))
	{
		return;
	}

	IRecord * pRec = pSelf->GetRecord(FIELD_RECORD_ENEMY_REC);
	if (NULL == pRec)
	{
		return;
	}
	int nRow = pRec->FindString(COLUMN_ENEMY_REC_UID, pEnemyUid);
	if (nRow != -1)
	{
		int nNowHatarT = pRec->QueryInt(nRow, COLUMN_ENEMY_REC_HATRED);
		nNowHatarT -= nHatarT;
		if (nNowHatarT <= 0)
		{
			pRec->RemoveRow(nRow);
		}
		else
		{
			pRec->SetInt(nRow, COLUMN_ENEMY_REC_HATRED, nNowHatarT);
		}
	}
}

//��Ӻ�������
int FriendModule::AddFriendApply(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	const char * pUid = args.StringVal(0);
    int applyType = args.IntVal(8);
	if (StringUtil::CharIsNull(pUid))
	{
		return 0;
	}
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return 0;
    }

	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_APPLY_REC);
	if (NULL == pRec)
	{
		return 0;
	}

	//�ж��Ƿ��Ѿ��������
	if (m_pFriendModule->FindApplyRow(pKernel, self, pUid, (ApplyType)applyType) >= 0)
	{
		return 0;
	}
    IRecord* friendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
    if (friendRec == NULL)
    {
        return 0;
    }
    
    int friendRow = friendRec->FindString(COLUMN_FRIEND_REC_UID, pUid);
    //�ж��Ƿ��Ǻ���
    if (friendRow >= 0)
    {
        return 0;
    }

	if (pRec->GetRows() >= pRec->GetRowMax())
	{
		pRec->RemoveRow(0);
	}

	pRec->AddRowValue(-1, args);

	return 1;
}

// ����ɾ�������ԣ��������δ������
int FriendModule::DeleteFriendApply(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	const char * pUid = args.StringVal(0);
    int applyType = args.IntVal(1);
	if (StringUtil::CharIsNull(pUid))
	{
		return 0;
	}
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return 0;
    }

	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_APPLY_REC);
	if (NULL == pRec)
	{
		return 0;
	}

	int nRow = m_pFriendModule->FindApplyRow(pKernel, self, pUid, (ApplyType)applyType);
	if (nRow < 0)
	{
		return 0;
	}

	pRec->RemoveRow(nRow);

	return 1;
}

void FriendModule::DeleteFriend(IKernel * pKernel, const PERSISTID & self, const wchar_t * targetName)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}
	const wchar_t * pSelfName = pSelfObj->QueryWideStr("Name");
	const char * pSelfUid = pSelfObj->QueryString("Uid");
	if (StringUtil::CharIsNull(pSelfName) ||
		StringUtil::CharIsNull(pSelfUid) ||
		StringUtil::CharIsNull(targetName))
	{
		return;
	}

	IRecord* pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return;
	}



	int nRowIndex = pRec->FindWideStr(COLUMN_FRIEND_REC_NAME, targetName);
	if (nRowIndex == -1)
	{
		return;
	}

	std::string  strTargetUid = pRec->QueryString(nRowIndex, COLUMN_FRIEND_REC_UID);
	if (strTargetUid.empty())
	{
		return;
	}

	pRec->RemoveRow(nRowIndex);



	//��Ӻ�����־modify by   (2015-04-23)
	
	LogModule::m_pLogModule->OnFriendEvent(pKernel, self, targetName, LOG_FRIEND_DELETE);

	//�Ƿ�����
	if (CheckPlayerOnline(pKernel, targetName))
	{
		pKernel->CommandByName(targetName, CVarList() << COMMAND_FRIEND_MSG << SS_FRIEND_REMOVE_PLAYER << pSelfName);
	}
	else
	{
		// Ŀ�겻���ߣ�������Ϣ��SNS����������¼ɾ��
		pKernel->SendSnsMessage(pKernel->GetDistrictId(),
			pKernel->GetServerId(),
			pKernel->GetMemberId(),
			pKernel->GetSceneId(),
			pSelfUid,
			CVarList() << SPACE_SNS
			<< SNS_FRIEND_SPACE
			<< SNS_MSG_FRIEND_DELETE
			<< pSelfUid
			<< pSelfName
			<< strTargetUid.c_str()
			<< targetName);
	}

	{
		CVarList sysTip;
		sysTip << targetName;
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17102, sysTip);
	}
}

bool FriendModule::IsCanAddIntimacy(IKernel*pKernel, IGameObj* pSelfObj, const char *friendUid)
{
	if (pSelfObj == NULL){
		return false;
	}

	IRecord* pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec){
		return false;
	}

	int nRowIndex = pRec->FindString(COLUMN_FRIEND_REC_UID, friendUid);
	if (nRowIndex < 0){
		return false;
	}
	int intimacyLevel = pRec->QueryInt(nRowIndex, COLUMN_FRIEND_REC_INTIMACY_LEVEL);
	int intimacyExpNow = pRec->QueryInt(nRowIndex, COLUMN_FRIEND_REC_INTIMACY_EXP);
	int relation = pRec->QueryInt(nRowIndex, COLUMN_FRIEND_REC_RELATION);
	int intimacyMaxLevel = 0;
	if (relation == FRIEND_RELATION_NOMAL)
	{
		intimacyMaxLevel = EnvirValueModule::EnvirQueryInt(ENV_VALUE_NOMAL_FRIEND_INTIMACY_MAX);
	}
	else if (relation == FRIEND_RELATION_SWORN)
	{
		intimacyMaxLevel = EnvirValueModule::EnvirQueryInt(ENV_VALUE_SWORN_FRIEND_INTIMACY_MAX);
	
	}

	if (!IsCanAddIntimacy(intimacyLevel, intimacyMaxLevel, intimacyExpNow))
	{
		const wchar_t *pwstrTargetName = pRec->QueryWideStr(nRowIndex, COLUMN_FRIEND_REC_NAME);
		if (relation == FRIEND_RELATION_NOMAL)
		{
			::CustomSysInfo(pKernel, pSelfObj->GetObjectId(), SYSTEM_INFO_ID_17124, CVarList() << pwstrTargetName);
		}
		else if (relation == FRIEND_RELATION_SWORN)
		{
			::CustomSysInfo(pKernel, pSelfObj->GetObjectId(), SYSTEM_INFO_ID_17125, CVarList() << pwstrTargetName);
		}
		return false;
	}

	return true;

}

bool FriendModule::IsCanAddIntimacy(int intimacyLevelNow, int intimacyLevelMax, int intimacyExpNow)
{
	if (intimacyLevelNow < intimacyLevelMax)
	{
		return true;
	}
	else if (intimacyLevelNow == intimacyLevelMax)
	{
		int levelUpNeedExp = GetIntimacyUpNeedExp(intimacyLevelNow);
		if (intimacyExpNow < levelUpNeedExp)
		{
			return true;
		}
	}
	

	return false;
}

int FriendModule::GetIntimacyUpNeedExp(int intimacyLevel)
{
	ConfigFriendIntimacyUpgradeItem* pUpgradeItem = Configure<ConfigFriendIntimacyUpgradeItem>::Find(intimacyLevel);
	if (NULL != pUpgradeItem)
	{
		return pUpgradeItem->GetExp();
	}
	return 0;
}

int FriendModule::ResetDaily(IKernel* pKernel, const PERSISTID& self, int slice)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}
	IRecord* pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec){
		return 0;
	}

	for (int i = 0; i < pRec->GetRows(); i++)
	{
		pRec->SetInt(i, COLUMN_FRIEND_REC_DAILY_INTIMACY, 0);
	}

	pSelfObj->SetInt(FIELD_PROP_DAILY_INTIMACY_AWARD,0);

	return 0;
}

// ���ܺ�������
bool FriendModule::OnAcceptFriend(IKernel* pKernel, IGameObj* pSelf, int nApplyIndex, IRecord* pRecApply, IRecord* pRecFriends)
{
	Assert(NULL != pSelf);
	Assert(NULL != pRecApply);
	Assert(nApplyIndex >= 0);
	Assert(NULL != pRecFriends);

	bool bOK = false;
	do
	{
		const char* pFridUid = pRecApply->QueryString(nApplyIndex, COLUMN_FRIEND_APPLY_REC_UID);
		if (StringUtil::CharIsNull(pFridUid)) 
			break;

		const wchar_t* pFridName = pRecApply->QueryWideStr(nApplyIndex, COLUMN_FRIEND_APPLY_REC_NAME);
		if (StringUtil::CharIsNull(pFridName))
			break;

		// ʼ��Ϊ���ѣ�û�л�鹦�� [8/23/2016 lihl]
		ApplyType applyType = APPLY_TYPE_FRIEND;

		// �Ƿ������Ӻ���
		if (!m_pFriendModule->IsCanAddFriend(pKernel, pSelf->GetObjectId(), pFridName, true))
		{
			break;
		}

		const char* pszUid = pSelf->QueryString(FIELD_PROP_UID);
		const wchar_t* pwszName = pSelf->QueryWideStr(FIELD_PROP_NAME);

		if (StringUtil::CharIsNull(pszUid)) return false;
		if (StringUtil::CharIsNull(pwszName)) return false;

		int result = 1; // 1Ϊ����
		bool friend_online = false;
		int level = pSelf->QueryInt("Level");
		int job = pSelf->QueryInt("Job");
		int sex = pSelf->QueryInt("Sex");
		int fightcap = pSelf->QueryInt("BattleAbility");
		const wchar_t *guild_name = pSelf->QueryWideStr("GuildName");

		if (CheckPlayerOnline(pKernel, pFridName))
		{
			friend_online = true;

			CVarList rowVar;
			rowVar << result
				<< pszUid
				<< pwszName
				<< level
				<< job
				<< sex
				<< fightcap
				<< guild_name
				<< ONLINE
				<< applyType;


			//�öԷ��Ӻ���
			SSFrienMsg subMsg = SS_FRIEND_ADD_RESULT;
			pKernel->CommandByName(pFridName, CVarList() << COMMAND_FRIEND_MSG << subMsg << rowVar);
		}
		else
		{
			// ֪ͨSNS��������������
			pKernel->SendSnsMessage(pKernel->GetDistrictId(),
				pKernel->GetServerId(),
				pKernel->GetMemberId(),
				pKernel->GetSceneId(),
				pFridUid,
				CVarList() << SPACE_SNS
				<< SNS_FRIEND_SPACE
				<< SNS_MSG_FRIEND_RESULT
				<< pszUid
				<< pwszName
				<< pFridUid
				<< pFridName
				<< level
				<< job
				<< sex
				<< fightcap
				<< guild_name
				<< result   //0-�ܾ� 1-����
				<< applyType);
		}

		int	taglevel = pRecApply->QueryInt(nApplyIndex, COLUMN_FRIEND_APPLY_REC_LEVEL);
		int	tagjob = pRecApply->QueryInt(nApplyIndex, COLUMN_FRIEND_APPLY_REC_JOB);
		int tagsex = pRecApply->QueryInt(nApplyIndex, COLUMN_FRIEND_APPLY_REC_SEX);
		int	tagfightcap = pRecApply->QueryInt(nApplyIndex, COLUMN_FRIEND_APPLY_REC_POWER);
		int vipLv = pRecApply->QueryInt(nApplyIndex, COLUMN_FRIEND_APPLY_REC_VIP_LEVEL);
		const wchar_t *tag_guild_name = pRecApply->QueryWideStr(nApplyIndex, COLUMN_FRIEND_APPLY_REC_GUILD_NAME);
		int teamID = 0;
		//�����Ƽ������
		IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
		if (NULL != pOnlinePlayerRec)
		{
			int row = pOnlinePlayerRec->FindString(PUB_COL_PLAYER_UID, pFridUid);
			if (row >= 0)
			{
				teamID = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_TEAM_ID);
			}
		}
		// ������ѱ���
		CVarList row_value;
		row_value << pFridUid
			<< pFridName
			<< taglevel
			<< tagjob
			<< tagsex
			<< tagfightcap
			<< tag_guild_name
			<< (friend_online ? ONLINE : OFFLINE)
			<< util_get_utc_time()			// ����ʱ��
			<< 1							// ���ܶȵȼ�
			<< 0							// ���ܶ�
			<< 0							// �ܻ���
			<< 0							// �����ջ���
			<< 0							// ���ͻ���
			<< 0							// ���͸��ҵĻ���
			<< teamID							// ����teamID
			<< FRIEND_RELATION_NOMAL		//��ϵ
			<< L""							//��ν
			<< int64_t(0)					//Ѱ��Ԯ��ʱ�� 
			<< vipLv							// vip
			<< 0;							// ÿ���������ܶ�ֵ
		pRecFriends->AddRowValue(-1, row_value);
		
 		PERSISTID& pid = pSelf->GetObjectId();
// 		pKernel->Command(pid, pid, CVarList() << COMMAND_SEVEN_DAY_OBJ << COMMAND_SEVEN_DAY_OBJ_ADD_FRIEND);
		// ����Ƽ�
		m_pFriendModule->DeleteRecommend(pKernel, pid, pFridName);
		LogModule::m_pLogModule->OnFriendEvent(pKernel, pid, pFridName, LOG_FRIEND_ADD);
		// �ɹ���Ӻ���  ���º�����Ϣ
		if (SnsDataModule::m_pSnsDataModule)
		{
			CVarList query_args;
			query_args << SNS_DATA_REQUEST_FORM_FRIEND_UPDATE;
			query_args << pFridName;
			query_args << 1;
			query_args << "PlayerAttribute";
			SnsDataModule::OnQueryPlayerData(pKernel, pid, pid, query_args);
		}

		bOK = true;
	}while (false);

	// ��������
	if (bOK)
	{
		pRecApply->RemoveRow(nApplyIndex);
	}
	return bOK;
}

// �ܾ���������
void FriendModule::OnRefuseFriend(IKernel* pKernel, IGameObj* pSelf, int nApplyIndex, IRecord* pRecApply)
{
	Assert(NULL != pRecApply);
	Assert(nApplyIndex >= 0);
	Assert(NULL != pSelf);

	do
	{
		const wchar_t* pFridName = pRecApply->QueryWideStr(nApplyIndex, COLUMN_FRIEND_APPLY_REC_NAME);
		if (StringUtil::CharIsNull(pFridName))
			break;
		const char* pFridUid = pRecApply->QueryString(nApplyIndex, COLUMN_FRIEND_APPLY_REC_UID);
		if (StringUtil::CharIsNull(pFridUid))
			break;

		ApplyType applyType = APPLY_TYPE_FRIEND;
		int result = 0; //0-�ܾ� 1-����
		// ֪ͨ���
		if (CheckPlayerOnline(pKernel, pFridName))
		{
			CVarList rowVar;
// 			rowVar << result
// 				<< pSelf->QueryString(FIELD_PROP_UID)
// 				<< pSelf->QueryWideStr(FIELD_PROP_NAME)
// 				<< pSelf->QueryInt(FIELD_PROP_LEVEL)
// 				<< pSelf->QueryInt(FIELD_PROP_NATION)
// 				<< pSelf->QueryInt(FIELD_PROP_JOB)
// 				<< pSelf->QueryInt(FIELD_PROP_SEX)
// 				<< pSelf->QueryInt(FIELD_PROP_BATTLE_ABILITY)
// 				<< pSelf->QueryInt(FIELD_PROP_GUILD_NAME)
// 				<< ONLINE
// 				<< applyType;


			SSFrienMsg subMsg =  SS_FRIEND_ADD_RESULT;
			pKernel->CommandByName(pFridName, CVarList() << COMMAND_FRIEND_MSG << subMsg << rowVar);
		}
		else
		{
			// ֪ͨSNS��������������
			pKernel->SendSnsMessage(pKernel->GetDistrictId(),
				pKernel->GetServerId(),
				pKernel->GetMemberId(),
				pKernel->GetSceneId(),
				pFridUid,
				CVarList() << SPACE_SNS
				<< SNS_FRIEND_SPACE
				<< SNS_MSG_FRIEND_RESULT
				<< pSelf->QueryString(FIELD_PROP_UID)
				<< pSelf->QueryWideStr(FIELD_PROP_NAME)
				<< pFridUid
				<< pFridName
				<< pSelf->QueryInt(FIELD_PROP_LEVEL)
				<< pSelf->QueryInt(FIELD_PROP_JOB)
				<< pSelf->QueryInt(FIELD_PROP_SEX)
				//<< pSelf->QueryInt(FIELD_PROP_BATTLE_ABILITY)
				//<< pSelf->QueryString(FIELD_PROP_GUILD_NAME)
				<< result   //0-�ܾ� 1-����
				<< applyType);
		}		
	} while (false);
	pRecApply->RemoveRow(nApplyIndex);
}

// �Ƿ�����Ӻ���
bool FriendModule::IsCanAddFriend(IKernel * pKernel, const PERSISTID & self, const wchar_t * targetName, bool bShowFailedTip/* = false*/, bool bIgnoreFull/* = false*/)
{
	if (!bIgnoreFull)
	{
		if (IsFullFriend(pKernel, self))
		{
			// ��������
			if (bShowFailedTip)
			{
				::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17104, CVarList() << targetName);
			}
			return false;
		}
	}

	//�жϸ�����Ƿ����
	if (::strnlen(pKernel->SeekRoleUid(targetName), 50) == 0)
	{
		// ��Ҳ�����
		if (bShowFailedTip)
		{
			::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17006, CVarList() << targetName);
		}
		return false;
	}

	if (IsFriend(pKernel, self, targetName))
	{
		// ���Ǻ���
		if (bShowFailedTip)
		{
			::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17126, CVarList() << targetName);
		}
		return false;
	}

	if (IsInBlackList(pKernel, self, targetName))
	{
		// ��������Ҳ��ܼӺ���
		if (bShowFailedTip)
		{
			::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17127, CVarList() << targetName);
		}
		return false;
	}

	return true;
}

// �����б��Ƿ�����
bool FriendModule::IsFullFriend(IKernel * pKernel, const PERSISTID & palyer)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(palyer);
    if (NULL == pSelfObj)
    {
        return false;
    }
	IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec)
	{
		return false;
	}

	if (pFriendRec->GetRows() >= pFriendRec->GetRowMax())
	{
		return true;
	}

	return false;
}

//targetName�Ƿ����Լ��ĺ���
bool FriendModule::IsFriend(IKernel * pKernel, const PERSISTID & self, const wchar_t * targetName)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return false;
    }
	IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec)
	{
		return false;
	}

	int nRow = pFriendRec->FindWideStr(COLUMN_FRIEND_REC_NAME, targetName);
	if (nRow < 0)
	{
		return false;
	}

	return true;
}

// targetName�Ƿ������ѵĳ�����
bool FriendModule::IsInEnemyList(IKernel * pKernel, const PERSISTID & self, const wchar_t * targetName)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return false;
    }
	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_ENEMY_REC);
	if (NULL == pRec)
	{
		return false;
	}

	int nRow = pRec->FindWideStr(COLUMN_ENEMY_REC_NAME, targetName);
	if (nRow < 0)
	{
		return false;
	}

	return true;
}

bool FriendModule::IsInBlackList(IKernel * pKernel, const PERSISTID & self, const wchar_t * targetName)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_BLACKLIST_REC);
	if (NULL == pRec)
	{
		return false;
	}

	int nRow = pRec->FindWideStr(COLUMN_BLACKLIST_REC_NAME, targetName);
	if (nRow < 0)
	{
		return false;
	}

	return true;
}

// ������������ж�����Ƿ����
bool FriendModule::CheckPlayerExist(IKernel* pKernel, const wchar_t* name)
{
	//�жϸ�����Ƿ����
	if (::strnlen(pKernel->SeekRoleUid(name), 50) == 0)
	{
		return false;
	}

	return true;
}

// ������������ж�����Ƿ����ߣ����ж��Ƿ���ڣ�
bool FriendModule::CheckPlayerOnline(IKernel* pKernel, const wchar_t* name)
{
	// �����ڵ�Ȼ��������
	if(!CheckPlayerExist(pKernel, name))
	{
		return false;
	}

	//�ж�����Ƿ�����
	if (pKernel->GetPlayerScene(name) <= 0)
	{
		return false;
	}

	return true;
}

// ��ѯ���������롱�Ļظ�
int FriendModule::OnSnsQuestFriendApplyRPL(IKernel *pKernel, const IVarList &args)
{
	// �յ����������������ĺ���������Ϣ��������������б�
	const PERSISTID player = args.ObjectVal(1);
    IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if (!pKernel->Exists(player) || NULL == pPlayerObj)
	{
		return 0;
	}

	const char *sender_uid = args.StringVal(2);
	if (StringUtil::CharIsNull(sender_uid))
	{
		return 0;
	}

    const wchar_t* sender_name = args.WideStrVal(3);
    int applyType = args.IntVal(10);
    if (StringUtil::CharIsNull(sender_name))
    {
        return 0;
    }

	IRecord * pApplyRec = pPlayerObj->GetRecord(FIELD_RECORD_FRIEND_APPLY_REC);
	if (NULL == pApplyRec)
	{
		return 0;
	}

    int row = m_pFriendModule->FindApplyRow(pKernel, player, sender_uid, (ApplyType)applyType);

	// ȡ��������
	const wchar_t* pSendName = pKernel->SeekRoleName(sender_uid);
	if (StringUtil::CharIsNull( pSendName) )
	{
		return 0;
	}

	if ( m_pFriendModule->IsCanAddFriend(pKernel, player, pSendName, false, true) )
	{
		CVarList rowValue;
		rowValue.Append(args, 2, 10);
		rowValue.SetWideStr(1, pKernel->SeekRoleName(sender_uid));
		if (row < 0)
		{
			pApplyRec->AddRowValue(-1, rowValue);
		}
		else
		{
			pApplyRec->SetRowValue(row, rowValue);
		}
	}
	else
	{
		if (row >= 0)
		{
			pApplyRec->RemoveRow( row );
		}
	}

	return 0;
}

// ��ѯ��������ѹ�ϵ���Ļظ�
int FriendModule::OnSnsQuestFriendDeleteRPL(IKernel *pKernel, const IVarList &args)
{
	// �յ����������������ĺ���������Ϣ��������������б�
	const PERSISTID player = args.ObjectVal(1);
    IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if (!pKernel->Exists(player) || NULL == pPlayerObj)
	{
		return 0;
	}

	const char *sender_uid = args.StringVal(2);
	if (StringUtil::CharIsNull(sender_uid))
	{
		return 0;
	}

	IRecord * pRec = pPlayerObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return 0;
	}

	// ����������ҵĺ��ѱ��У�ɾ���˺���
	int row = pRec->FindString(COLUMN_FRIEND_REC_UID, sender_uid);
	if (row >= 0)
	{
		pRec->RemoveRow(row);

		::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_17102, CVarList() << args.WideStrVal(3));

		//��Ӻ�����־modify by SunJian(2015-04-23)
		//FriendLog log;
		//log.friendName        = pKernel->SeekRoleName(sender_uid);
		//log.type              = LOG_FRIEND_DELETE;
		//LogModule::m_pLogModule->SaveFriendLog(pKernel, player, log);
		LogModule::m_pLogModule->OnFriendEvent(pKernel, player, pKernel->SeekRoleName(sender_uid), LOG_FRIEND_DELETE);
	}

	return 0;
}

// ��ѯ���������������Ļظ�
int FriendModule::OnSnsQuestFriendApplyResultRPL(IKernel* pKernel, const IVarList& args)
{
	// �յ����������������ĺ����������������ҵĺ����б�
	const PERSISTID player = args.ObjectVal(1);
    IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if (!pKernel->Exists(player) || NULL == pPlayerObj)
	{
		return 0;
	}

	const char      *sender_uid     = args.StringVal(2);
	const wchar_t   *sender_name    = pKernel->SeekRoleName(sender_uid);
	int             level           = args.IntVal(4);
	int             job             = args.IntVal(5);
	int             sex             = args.IntVal(6);
	int             fightcap        = args.IntVal(7);
	const wchar_t	*guild_name = args.WideStrVal(8);			
	int             result          = args.IntVal(9);

	// ��ѯ�Ƿ���������ĺ��� ���ǲ������
	// if (!m_pFriendModule->TestRemovePlayerInvited(pKernel, player, sender_uid, true))
	// {
	// 	return 0;
	// }

	// ֱ�ӵ���OnCommandAddFriendResult����
	CVarList rowVar;
	rowVar << COMMAND_FRIEND_MSG << SS_FRIEND_ADD_RESULT
		<< result
		<< sender_uid
		<< sender_name
		<< level
		<< job
		<< sex
		<< fightcap
		<< guild_name
		<< OFFLINE
		<< APPLY_TYPE_FRIEND;

	m_pFriendModule->OnCommandAddFriendResult(pKernel, player, rowVar);
	return 0;
}

// ��ȡ�������ͻ����Ļظ�
void FriendModule::OnSnsQuestFriendGiftPush(IKernel* pKernel, const IVarList& args)
{
	// �յ����������������ĺ����������������ҵĺ����б�
	const PERSISTID player = args.ObjectVal(1);
	IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if (!pKernel->Exists(player) || NULL == pPlayerObj)
	{
		return;
	}

	const char* pSendUid = args.StringVal(2);
	int nCount = args.IntVal(3);
	if (StringUtil::CharIsNull(pSendUid))
	{
		::extend_warning(LOG_ERROR, "[FriendModule::OnSnsQuestFriendGiftPush] sender uid error, it's null");
		return;
	}

	if (nCount < 1)
	{
		::extend_warning(LOG_ERROR, "[FriendModule::OnSnsQuestFriendGiftPush] sender gift count(%d) error, it's lower 0", nCount);
		return;
	}
	
	m_pFriendModule->OnRecvFlower(pKernel, pPlayerObj, pSendUid, nCount);
}

void FriendModule::OnSnsAddFriendIntimacyPush(IKernel* pKernel, const IVarList& args)
{
	const PERSISTID player = args.ObjectVal(1);
	IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if (!pKernel->Exists(player) || NULL == pPlayerObj)
	{
		return;
	}

	int count = args.IntVal(2);
	int index = 2;
	LoopBeginCheck(a);
	for (int i = 0; i < count; i++)
	{
		LoopDoCheck(a);
		const char* friendUid = args.StringVal(++index);
		int value			= args.IntVal(++index);
		m_pFriendModule->OnCommandAddFriendIntiMacy(pKernel, player, friendUid, value);
	}

}

void FriendModule::OnSnsDelSworn(IKernel* pKernel, const IVarList& args)
{
	const PERSISTID player = args.ObjectVal(1);
	IGameObj* pSelfObj = pKernel->GetGameObj(player);
	if (!pKernel->Exists(player) || NULL == pSelfObj)
	{
		return;
	}
	
	IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec){
		return;
	}
	int intimacyLevel = EnvirValueModule::EnvirQueryInt(ENV_VALUE_NOMAL_FRIEND_INTIMACY_MAX);

	int count = args.IntVal(2);
	int index = 2;
	LoopBeginCheck(a);
	for (int i = 0; i < count; i++)
	{
		LoopDoCheck(a);
		const wchar_t* name = args.WideStrVal(++index);
		int nRow = pFriendRec->FindWideStr(COLUMN_FRIEND_REC_NAME, name);
		if (nRow < 0){
			continue;
		}
	
		pFriendRec->SetInt(nRow, COLUMN_FRIEND_REC_RELATION, FRIEND_RELATION_NOMAL);
		pFriendRec->SetInt(nRow, COLUMN_FRIEND_REC_INTIMACY_LEVEL, intimacyLevel);
		pFriendRec->SetInt(nRow, COLUMN_FRIEND_REC_INTIMACY_EXP, 0);
		int friendSex = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_SEX);
		bool isSameSex = friendSex == pSelfObj->QueryInt(FIELD_PROP_SEX) ? true : false;
		LogModule::m_pLogModule->OnSworn(pKernel, player, name, isSameSex, FRIEND_SOWRN_DEL);
	}
}

void FriendModule::OnSnsAddEnemy(IKernel*pKernel, const IVarList& args)
{
	const PERSISTID player = args.ObjectVal(1);
	IGameObj* pSelfObj = pKernel->GetGameObj(player);
	if (!pKernel->Exists(player) || NULL == pSelfObj)
	{
		return;
	}
	int count = args.IntVal(2);
	int index = 3;
	for (int i = 0; i < count; i++)
	{
		CVarList info;
		info.Append(args, index,12);
		m_pFriendModule->AddOnceEnemy(pKernel, player, info);
		index += 12;
	}

}

//���������Ϣ
bool FriendModule::PackPlayerInfo(IKernel* pKernel, IVarList& var, const wchar_t* pTargetName)
{
	if (StringUtil::CharIsNull(pTargetName))
	{
		return false;
	}

	const char * pTargetUID = pKernel->SeekRoleUid(pTargetName);
	if (StringUtil::CharIsNull(pTargetUID))
	{
		return false;
	}

	IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
	if (NULL == pOnlinePlayerRec)
	{
		return false;
	}

	int row = pOnlinePlayerRec->FindString(PUB_COL_PLAYER_UID, pTargetUID);
	if (row == -1)
	{
		return false;
	}

	int level = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_LEVEL);
	int job = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_JOB);
	int sex = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_SEX);
	int ability = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_BATTLE_ABLITITY);
	const wchar_t * guild_name = pOnlinePlayerRec->QueryWideStr(row, PUB_COL_PLAYER_GUILD_NAME);

	var	<< pTargetUID
		<< pTargetName
		<< level
		<< job
		<< sex
		<< ability
		<< guild_name;

	return true;
}

// ��ȡDomain������
const std::wstring& FriendModule::GetDomainName(IKernel * pKernel)
{
	if (m_domainName.empty())
	{
		wchar_t wstr[256];
		const int server_id = pKernel->GetServerId();
		SWPRINTF_S(wstr, DOMAIN_FRIEND_RECOMMEND_REC, server_id);

		m_domainName = wstr;
	}

	return m_domainName;
}

// bool FriendModule::IsCanSworn(IKernel*pKernel, const PERSISTID& self, const wchar_t* targetName)
// {
// 
// 	IGameObj *pSelfObj = pKernel->GetGameObj(self);
// 	if (pSelfObj == NULL){
// 		return false;
// 	}
// 	if (pKernel->GetPlayerScene(targetName) <= 0)
// 	{
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_FRIEND_IS_NOT_ONLINE, CVarList());
// 		return false;
// 	}
// 
// 
// 	IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
// 	if (NULL == pFriendRec)
// 	{
// 		return false;
// 	}
// 
// 	int nRow = pFriendRec->FindWideStr(COLUMN_FRIEND_REC_NAME, targetName);
// 	if (nRow < 0)
// 	{
// 		return false;
// 	}
// 	
// 	if (pKernel->GetPlayerScene(targetName) <= 0)
// 	{
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_FRIEND_IS_NOT_ONLINE, CVarList());
// 		return false;
// 	}
// 
// 	int relation = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_RELATION);
// 	if (relation != FRIEND_RELATION_NOMAL){
// 		return false;
// 	}
// 
// 	int friendSex = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_SEX);
// 	int selfSex = pSelfObj->QueryInt(FIELD_PROP_SEX);
// 	int swornSexNum = GetSwornNum(pKernel, self, friendSex);
// 	if (swornSexNum >= VipModule::m_pVipModule->GetNumber(pKernel, self, VIP_FRIEND_SWORN_NUM))
// 	{
// 		if (selfSex == friendSex)
// 		{
// 			::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_FRIEND_SWORN_FULL1, CVarList());
// 		}
// 		else
// 		{
// 			::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_FRIEND_SWORN_FULL2, CVarList());
// 		}
// 
// 		return false;
// 	}
// 
// 
// 	int intimacyLevel = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_INTIMACY_LEVEL);
// 	int needintimacyLevel = EnvirValueModule::EnvirQueryInt(ENV_VALUE_SWORN_INTIMACY_LEVEL);
// 	if (intimacyLevel < needintimacyLevel)
// 	{
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_FRIEND_INTIMACY_LEVEL_LIMIT, CVarList());
// 		return false;
// 	}
// 
// 
// 	return true;
// }

// ��ȡ������������ұ�
IRecord * FriendModule::GetPubOnlineRec(IKernel * pKernel)
{
	IPubSpace * pDomainSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (!pDomainSpace)
	{
		return NULL;
	}

	// ��ȡ�����ռ�
	IPubData * pPubData = pDomainSpace->GetPubData(GetDomainName(pKernel).c_str());
	if (NULL == pPubData)
	{
		return NULL;
	}

	IRecord * pOnlinePlayerRec = pPubData->GetRecord(PUB_ONLINE_PLAYER_REC);
	if (NULL == pOnlinePlayerRec)
	{
		return NULL;
	}

	return pOnlinePlayerRec;
}


void FriendModule::OnCustomIntimacyAwards(IKernel*pKernel, const PERSISTID& self)
{

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}

	if (pSelfObj->QueryInt(FIELD_PROP_DAILY_INTIMACY_AWARD) != 0){
		return;
	}
	int maxIntimacyLv = 0;

	
	IRecord* pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec){
		return ;
	}

	for (int i = 0; i < pRec->GetRows(); i++)
	{
		
		int intimacyLv = pRec->QueryInt(i, COLUMN_FRIEND_REC_INTIMACY_LEVEL);
		maxIntimacyLv = max(intimacyLv, maxIntimacyLv);

	}

	auto it = m_teamIntimacyAward.find(maxIntimacyLv);
	if (it == m_teamIntimacyAward.end()){
		return;
	}
// 	RewardModule::AwardEx award;
// 	award.name = INTIMACY_AWARD;
// 	award.srcFunctionId = FUNCTION_EVENT_INTIMACY_AWARD;
// 	RewardModule::ParseAwardData(it->second, award);
//	RewardModule::m_pRewardInstance->RewardPlayer(pKernel, self, &award);

	pSelfObj->SetInt(FIELD_PROP_DAILY_INTIMACY_AWARD, 1);


	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_FRIEND << S2C_FRIEND_INTIMACY_AWARD << it->second);

}


void FriendModule::AddOnceEnemy(IKernel*pKernel, const PERSISTID& self, const IVarList & args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}
	const char*	pObjUid = args.StringVal(0);
	const wchar_t* pObjName = args.WideStrVal(1);
	if (StringUtil::CharIsNull(pObjUid))
	{
		return;
	}

	// [8/17/2016 lihl] �ں������в��������(�ܶ�Ҫ��)
	if (IsInBlackList(pKernel, self, pObjName)){
		return;
	}

	// �Ƿ��Ѿ��ǳ���
	IRecord * pEnemyRecord = pSelfObj->GetRecord(FIELD_RECORD_ENEMY_REC);
	if (NULL == pEnemyRecord){
		return;
	}

	int nEnemyRow = pEnemyRecord->FindString(COLUMN_ENEMY_REC_UID, pObjUid);
	int nHaterT = args.IntVal(9);
	if (nEnemyRow > -1)
	{
		// �ǳ��� ���³��ֵ
		int nNowHaterT = pEnemyRecord->QueryInt(nEnemyRow, COLUMN_ENEMY_REC_HATRED);
		nNowHaterT += nHaterT;
		pEnemyRecord->SetInt(nEnemyRow, COLUMN_ENEMY_REC_HATRED, nNowHaterT);
		pEnemyRecord->SetInt64(nEnemyRow, COLUMN_ENEMY_REC_BE_KILL_TIME, ::time(NULL));
		return;
	}

	if (pEnemyRecord->GetRows() >= pEnemyRecord->GetRowMax())
	{
	// �ܶ�˵��ɾ
	//	pEnemyRecord->RemoveRow(0);

		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17118, CVarList());
		return;
	}
	int level = args.IntVal(2);
	int job = args.IntVal(3);
	int sex = args.IntVal(4);
	int battleAbility = args.IntVal(5);
	const wchar_t* guildName = args.WideStrVal(6);
	const char* account = args.StringVal(7);
	int64_t time = args.Int64Val(9);
	int vipLv = args.IntVal(10);
	int isOnline = OFFLINE;
	if (pKernel->GetPlayerScene(pObjName) > 0)
	{
		isOnline = ONLINE;
	}

	CVarList row_list;
	row_list << pObjUid
		<< pObjName
		<< level
		<< job
		<< sex
		<< battleAbility
		<< guildName
		<< account
		<< isOnline
		<< nHaterT		// ���ֵ
		<< time
		<< vipLv;

	pEnemyRecord->AddRowValue(-1, row_list);
}

void FriendModule::SwornFriendAddOnceEnemy(IKernel*pKernel, const PERSISTID&self, const IVarList & args)
{

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	const wchar_t* pSelfName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
	if (pSelfObj == NULL){
		return;
	}

	IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec){
		return;
	}
	CVarList info;
	info << COMMAND_FRIEND_MSG << SS_FRIEND_ADD_ONECE_ENEMY << args;

	CVarList snsInfo;
	snsInfo << SPACE_SNS
		<< SNS_FRIEND_SPACE
		<< SNS_MSG_FRIEND_ADD_ENEMY
		<< args;
	const wchar_t * enemyName = args.WideStrVal(1);
	for (int row = 0; row < pFriendRec->GetRows(); row++)
	{
		int relation = pFriendRec->QueryInt(row, COLUMN_FRIEND_REC_RELATION);
		if (relation == FRIEND_RELATION_SWORN)
		{
			int onLine = pFriendRec->QueryInt(row, COLUMN_FRIEND_REC_ONLINE);
			const wchar_t* pName = pFriendRec->QueryWideStr(row, COLUMN_FRIEND_REC_NAME);
			if (wcscmp(pName, enemyName) == 0){
				continue;
			}
			if (onLine == ONLINE)
			{
				pKernel->CommandByName(pName,info);
			}
			else
			{
				const char* playerUid = pFriendRec->QueryString(row, COLUMN_FRIEND_REC_UID);
				pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
					pKernel->GetSceneId(), playerUid,
					snsInfo);
			}
		}
	}




}
bool FriendModule::LoadTeamFriendConfig(IKernel*pKernel)
{
	std::string pathName = pKernel->GetResourcePath();
	//�����ļ�·��
	pathName += TEAM_FRIEND_GAIN;
	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string err_msg = pathName;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}
	m_teamFriendGain.clear();
	std::string strSectionName = "";
	const size_t iSectionCount = xmlfile.GetSectionCount();
	LoopBeginCheck(a)
		for (unsigned i = 0; i < iSectionCount; i++)
		{
			LoopDoCheck(a);
			strSectionName = xmlfile.GetSectionByIndex(i);
			if (StringUtil::CharIsNull(strSectionName.c_str())){
				continue;
			}
			int friendNum = xmlfile.ReadInteger(strSectionName.c_str(), "FriendNum", 0);
			const char* buffID = xmlfile.ReadString(strSectionName.c_str(), "BuffID", "");
			m_teamFriendGain[friendNum] = buffID;
		}
	return true;
}

bool FriendModule::LoadTeamFirendIntimacy(IKernel*pKernel)
{
	std::string pathName = pKernel->GetResourcePath();
	//�����ļ�·��
	pathName += TEAM_FRIEND_INTIMACY;
	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string err_msg = pathName;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}
	m_teamFriendIntimacy.clear();
	std::string strSectionName = "";
	const size_t iSectionCount = xmlfile.GetSectionCount();
	LoopBeginCheck(a)
		for (unsigned i = 0; i < iSectionCount; i++)
		{
			LoopDoCheck(a);
			strSectionName = xmlfile.GetSectionByIndex(i);
			if (StringUtil::CharIsNull(strSectionName.c_str())){
				continue;
			}
			int intimacyLv = xmlfile.ReadInteger(strSectionName.c_str(), "IntimacyLv", 0);
			const char* buffID = xmlfile.ReadString(strSectionName.c_str(), "BuffID", "");
			m_teamFriendIntimacy[intimacyLv] = buffID;
		}
	return true;
}



bool FriendModule::LoadFirendIntimacyAward(IKernel*pKernel)
{
	std::string pathName = pKernel->GetResourcePath();
	//�����ļ�·��
	pathName += TEAM_FRIEND_INITMACY_AWARD;
	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string err_msg = pathName;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}
	m_teamIntimacyAward.clear();
	std::string strSectionName = "";
	const size_t iSectionCount = xmlfile.GetSectionCount();
	LoopBeginCheck(a)
		for (unsigned i = 0; i < iSectionCount; i++)
		{
			LoopDoCheck(a);
			strSectionName = xmlfile.GetSectionByIndex(i);
			if (StringUtil::CharIsNull(strSectionName.c_str())){
				continue;
			}
			int intimacyLv = xmlfile.ReadInteger(strSectionName.c_str(), "IntimacyLv", 0);
			const char* award = xmlfile.ReadString(strSectionName.c_str(), "Award", "");
			m_teamIntimacyAward[intimacyLv] = award;
		}
	return true;
}

int FriendModule::OnCommandRefreshFriendGain(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	//m_pFriendModule-> RefreshTeamFriendGain(pKernel,self);
	return 0;
}

// �������Ϊ������
void FriendModule::SetPlayerInvited(IKernel * pKernel, const PERSISTID & self, const char *role_uid)
{
    if (StringUtil::CharIsNull(role_uid))
    {
        return;
    }
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }

    IRecord* pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_INVITATION_SEND_REC);
    if (NULL == pRec)
    {
        return;
    }

    int nRowIndex = pRec->FindString(COLUMN_FRIEND_INVITATION_SEND_REC_UID, role_uid);
    if (nRowIndex == -1)
    {
        CVarList row_value;
        row_value << role_uid;

        pRec->AddRowValue(-1, row_value);
    }
    
}

// �鿴������Ƿ��Ѿ�������
bool FriendModule::TestRemovePlayerInvited(IKernel * pKernel, const PERSISTID & self, const char *role_uid, bool bRemove)
{
    if (StringUtil::CharIsNull(role_uid))
    {
        return false;
    }
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return false;
    }

    IRecord* pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_INVITATION_SEND_REC);
    if (NULL == pRec)
    {
        return false;
    }

    int nRowIndex = pRec->FindString(COLUMN_FRIEND_INVITATION_SEND_REC_UID, role_uid);
    if (nRowIndex == -1)
    {
        return false;
    }

	if (bRemove)
	{
		pRec->RemoveRow(nRowIndex);
	}

    return true;
}

int FriendModule::GetSwornNum(IKernel*pKernel, const PERSISTID & self, int sex)
{
	int totalNum = 0;
	IRecord * pFriendRec = pKernel->GetRecord(self,FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec){
		return totalNum;
	}
	int rowMax = pFriendRec->GetRows();
	for (int row = 0; row < rowMax;row++)
	{ 
		int relation = pFriendRec->QueryInt(row, COLUMN_FRIEND_REC_RELATION);
		if (relation != FRIEND_RELATION_SWORN){
			continue;
		}

		int rowSex = pFriendRec->QueryInt(row, COLUMN_FRIEND_REC_SEX);
		if (rowSex != sex){
			continue;
		}
		++totalNum;
	}

	return totalNum;
}

void FriendModule::RecommendFriendByVipLv(IKernel*pKernel, const PERSISTID & self)
{

	if (m_pFriendModule->IsFullFriend(pKernel, self))
	{
		return;
	}
	size_t recommendNum = 5;
	IGameObj*pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}
	IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
	if (NULL == pOnlinePlayerRec)
	{
		return ;
	}
	int selfVipLv = pSelfObj->QueryInt(FIELD_PROP_VIP_LEVEL);

	const wchar_t*selfName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
	int count = pOnlinePlayerRec->GetRows();

	std::vector<int> playerGroup;

	for (int row = 0; row < count; row++)
	{

		const wchar_t* playerName = pOnlinePlayerRec->QueryWideStr(row, PUB_COL_PLAYER_NAME);
		int targetLevel = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_LEVEL);
		if (!CanBeFirend(pKernel, self, selfName, playerName, targetLevel)){
			continue;
		}

		int vipLv = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_VIP);
		if (vipLv == selfVipLv)
		{
			playerGroup.push_back(row);
		}

		

	}

	if (playerGroup.empty()){
		return;
	}

	if (playerGroup.size() > recommendNum)
	{
		std::random_shuffle(playerGroup.begin(), playerGroup.end());
	}



	IRecord * pNearbyRec = pSelfObj->GetRecord(FIELD_RECORD_SYSTEM_RECOMMEND_REC);
	if (NULL == pNearbyRec)
	{
		return;
	}

	pNearbyRec->ClearRow();
	
	size_t index = 0;

	for (auto it : playerGroup)
	{
		if (index >= recommendNum)
		{
			break;
		}

		int row = it;

		CVarList info;
		pOnlinePlayerRec->QueryRowValue(row, info);
		pNearbyRec->AddRowValue(-1,info);
		++index;
	}

	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_FRIEND << S2C_FRIEND_SYSTEM_RECOMMEND);
}

void FriendModule::RecommendFriendByPlayerLv(IKernel*pKernel, const PERSISTID & self)
{

	if (m_pFriendModule->IsFullFriend(pKernel, self))
	{
		return;
	}


	size_t recommendNum = 5;
	IGameObj*pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}
	const wchar_t*selfName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
	IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
	if (NULL == pOnlinePlayerRec)
	{
		return;
	}
	int selfLevel = pSelfObj->QueryInt(FIELD_PROP_LEVEL);

	if (selfLevel < 20){
		return ;
	}

	int nRecommandLevel = EnvirValueModule::EnvirQueryInt(ENV_VALUE_FRIEND_RECOMM_LEVEL_INTVAL);
	if (0 >= nRecommandLevel || selfLevel % nRecommandLevel != 0){
		return ;
	}

	int count = pOnlinePlayerRec->GetRows();
	std::vector<int> playerGroup;

	for (int row = 0; row < count; row++)
	{
		int vipLv = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_VIP);
		if (vipLv == 0){
			continue;
		}
		const wchar_t* playerName = pOnlinePlayerRec->QueryWideStr(row, PUB_COL_PLAYER_NAME);
		int targetLevel = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_LEVEL);
		if (!CanBeFirend(pKernel, self, selfName, playerName, targetLevel)){
			continue;
		}
		int rowPlayerLevel = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_LEVEL);
		if (rowPlayerLevel < selfLevel){
			continue;
		}

		playerGroup.push_back(row);
	}


	if (playerGroup.empty()){
		return;
	}
	if (playerGroup.size() > recommendNum)
	{
		std::random_shuffle(playerGroup.begin(), playerGroup.end());
	}

	IRecord * pNearbyRec = pSelfObj->GetRecord(FIELD_RECORD_SYSTEM_RECOMMEND_REC);
	if (NULL == pNearbyRec)
	{
		return;
	}

	pNearbyRec->ClearRow();

	size_t index = 0;

	for (auto it : playerGroup)
	{
		if (index >= recommendNum)
		{
			break;
		}

		int row = it;

		CVarList info;
		pOnlinePlayerRec->QueryRowValue(row, info);
		pNearbyRec->AddRowValue(-1, info);
		++index;
	}
	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_FRIEND << S2C_FRIEND_SYSTEM_RECOMMEND);
}

// void FriendModule::RecommendFriendByAddGuild(IKernel*pKernel, const PERSISTID & self)
// {
// 
// 	if (m_pFriendModule->IsFullFriend(pKernel, self))
// 	{
// 		return;
// 	}
// 
// 	size_t recommendNum = 5;
// 	IGameObj*pSelfObj = pKernel->GetGameObj(self);
// 	if (pSelfObj == NULL){
// 		return;
// 	}
// 	const wchar_t*selfName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
// 	IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
// 	if (NULL == pOnlinePlayerRec)
// 	{
// 		return;
// 	}
// 	const wchar_t*selfGuildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
// 
// 	int count = pOnlinePlayerRec->GetRows();
// 
// 	std::vector<int> playerGroup;
// 
// 	for (int row = 0; row < count; row++)
// 	{
// 		int vipLv = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_VIP);
// 		if (vipLv == 0){
// 			continue;
// 		}
// 		const wchar_t* playerName = pOnlinePlayerRec->QueryWideStr(row, PUB_COL_PLAYER_NAME);
// 		
// 		int targetLevel = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_LEVEL);
// 		
// 		if (!CanBeFirend(pKernel, self, selfName,playerName,targetLevel)){
// 			continue;
// 		}
// 		
// 		const wchar_t* guildName = pOnlinePlayerRec->QueryWideStr(row, PUB_COL_PLAYER_GUILD_NAME);
// 		if (wcscmp(guildName, selfGuildName) != 0){
// 			continue;
// 		}
// 
// 		playerGroup.push_back(row);
// 	}
// 
// 
// 	if (playerGroup.empty()){
// 		return;
// 	}
// 	if (playerGroup.size() > recommendNum)
// 	{
// 		std::random_shuffle(playerGroup.begin(), playerGroup.end());
// 	}
// 
// 	IRecord * pNearbyRec = pSelfObj->GetRecord(FIELD_RECORD_SYSTEM_RECOMMEND_REC);
// 	if (NULL == pNearbyRec)
// 	{
// 		return;
// 	}
// 
// 	pNearbyRec->ClearRow();
// 
// 	size_t index = 0;
// 
// 	for (auto it : playerGroup)
// 	{
// 		if (index >= recommendNum){
// 			break;
// 		}
// 
// 		int row = it;
// 		CVarList info;
// 		pOnlinePlayerRec->QueryRowValue(row, info);
// 		pNearbyRec->AddRowValue(-1, info);
// 		++index;
// 	}
// 
// 	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_FRIEND << S2C_FRIEND_SYSTEM_RECOMMEND);
// }

bool FriendModule::CanBeFirend(IKernel*pKernel, const PERSISTID & self, const wchar_t * selfName,const wchar_t*targetName, int targetLv)
{
	if (wcscmp(selfName, targetName) == 0){
		return false;
	}

	if (IsInBlackList(pKernel, self, targetName)){
		return false;
	}

	if (IsFriend(pKernel, self, targetName)){
		return false;
	}
	

// 	if (targetLv < EnvirValueModule::EnvirQueryInt(ENV_VALUE_FRINED_FUNCTION_OPEN_LV)){
// 		return false;
// 	}
	return true;
}

int FriendModule::GetTeamAddIntimacyValue(int type)
{
	auto config =  Configure<ConfigTeamAddIntimacy>::Find(1);
	if (config == NULL){
		return 0;
	}
	switch (type)
	{
		case	TEAM_ADD_INTIMACY_OFFER://����
			return config->GetTeamOffer();
			break;
		case	TEAM_ADD_INTIMACY_SECRET://��ӽ���
			return config->GetTeamSecret();
			break;
		case 	TEAM_ADD_INTIMACY_INBREAK://��֯����
			return config->GetTeamInbreak();
			break;
		case	TEAM_ADD_INTIMACY_ESCORT://���Ѻ��
			return config->GetTeamEscort();
			break;
		case	TEAM_ADD_INTIMACY_KILL_NPC://Ұ��һ�
			return config->GetTeamKillNpc();
			break;
	}
	return 0;




}

int FriendModule::GetDailyAddIntimacyLimit()
{
	auto config = Configure<ConfigTeamAddIntimacy>::Find(1);
	if (config == NULL){
		return 0;
	}
	return  config->GetDailyLimit();

	return 0;
}

//������������������
int FriendModule::FindApplyRow(IKernel * pKernel, const PERSISTID & self, const char* targetUid, ApplyType applyType)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return -1;
    }
    IRecord* applyRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_APPLY_REC);
    if (applyRec == NULL)
    {
        return -1;
    }
    LoopBeginCheck(f);
    for (int i = 0; i < applyRec->GetRows(); ++i)
    {
        LoopDoCheck(f);
        const char* playerUid = applyRec->QueryString(i, COLUMN_FRIEND_APPLY_REC_UID);
        if (strcmp(playerUid, targetUid) != 0)
        {
            continue;
        }
        int type = applyRec->QueryInt(i, COLUMN_FRIEND_APPLY_REC_APPLY_TYPE);
        if (type == applyType)
        {
            return i;
        }
    }

    return -1;
}

// bool FriendModule::RefreshTeamFriendGain(IKernel*pKernel, const PERSISTID& self)
// {
// 	IGameObj *pSelfObj = pKernel->GetGameObj(self);
// 	if (pSelfObj == NULL){
// 		return false;
// 	}
// 
// 	for (auto it : m_teamFriendGain)
// 	{
// 		BufferModule::m_pInstance->RemoveBuffer(pKernel, self, it.second.c_str());
// 	}
// 	for (auto it : m_teamFriendIntimacy)
// 	{
// 		BufferModule::m_pInstance->RemoveBuffer(pKernel, self, it.second.c_str());
// 	}
// 
// 
// 	IRecord *pTeamRec = pSelfObj->GetRecord(TEAM_REC_NAME);
// 	if (NULL == pTeamRec)
// 	{
// 		return false;
// 	}
// 	int friendCount = 0;
// 	int maxIntimacyLv = 0;
// 	int count = pTeamRec->GetRows();
// 	for (int i = 0; i < count; i++)
// 	{
// 		const wchar_t * memberName = pTeamRec->QueryWideStr(i, TEAM_REC_COL_NAME);
// 
// 
// 		IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
// 		if (NULL == pFriendRec)
// 		{
// 			return false;
// 		}
// 
// 		int nRow = pFriendRec->FindWideStr(COLUMN_FRIEND_REC_NAME, memberName);
// 		if (nRow >= 0)
// 		{
// 			++friendCount;
// 			int intimacyLv = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_INTIMACY_LEVEL);
// 			maxIntimacyLv = max(maxIntimacyLv, intimacyLv);
// 		}
// 
// 	}
// 
// 	if (friendCount == 0){
// 		return 0;
// 	}
// 	{ //���������ӳ�
// 		auto it = m_teamFriendGain.find(friendCount);
// 		if (it != m_teamFriendGain.end())
// 		{
// 			BufferModule::m_pInstance->AddBuffer(pKernel, self, self, it->second.c_str());
// 		}
// 	}
// 
// 	{  //�������ܶ����ȼ��ӳɼӳ�
// 		auto it = m_teamFriendIntimacy.find(maxIntimacyLv);
// 		if (it != m_teamFriendIntimacy.end()){
// 			BufferModule::m_pInstance->AddBuffer(pKernel, self, self, it->second.c_str());
// 		}
// 
// 	}
// 
// 	return true;
// }