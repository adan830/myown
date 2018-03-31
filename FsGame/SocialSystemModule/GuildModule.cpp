//--------------------------------------------------------------------
// �ļ���:		GuildModule.cpp
// ��  ��:		����ϵͳ
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸�����:	���֪ͨ�ͻ��������������������
//    :	    
//--------------------------------------------------------------------

#include "GuildModule.h"
#include "GuildLoadResource.h"
#include "../Define/ClientCustomDefine.h"
#include "../Define/CommandDefine.h"
#include "../Define/ServerCustomDefine.h"
#include "../Define/GuildMsgDefine.h"
#include "../Define/LogDefine.h"
#include "../Define/PubDefine.h"
#include "../Define/GameDefine.h"
#include "utils/util_func.h"
#include "utils/custom_func.h"
#include "utils/extend_func.h"
#include "FsGame/SystemFunctionModule/CapitalModule.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/CommonModule/VirtualRecModule.h"
#include "FsGame/ItemModule/ItemBaseModule.h"
#include "utils/string_util.h"
#include "FsGame/SocialSystemModule/ChatModule.h"
#include "server/LoopCheck.h"
#include "CommonModule/LogModule.h"
#include "CommonModule/ReLoadConfigModule.h"
#include "Define/Skilldefine.h"
#include "SkillModule/PassiveSkillSystem.h"
#include "Define/ResetTimerDefine.h"
#include "SystemFunctionModule/ResetTimerModule.h"
#include "SystemFunctionModule/RewardModule.h"
#include "Define/BattleAbilityDefine.h"
#include "Define/SnsDefine.h"
#include "SystemMailModule.h"
#include "Define/Fields.h"
#include "CommonModule/EnvirValueModule.h"
#include "Define/PlayerBaseDefine.h"
#include "CommonModule/ActionMutex.h"
#include "SystemFunctionModule/ActivateFunctionModule.h"

GuildModule* GuildModule::m_pGuildModule = NULL;
CapitalModule* GuildModule::m_pCapitalModule = NULL;
VirtualRecModule* GuildModule::m_pVirtualRecModule = NULL;
ItemBaseModule* GuildModule::m_pItemBaseModule = NULL;
ChatModule* GuildModule::m_pChatModule = NULL;

// �ؼ��������ļ�
int nx_reload_guild_config(void* state)
{
	// ��ȡ����ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);
	// �ж�
	bool isLoad = false;
	if (NULL != GuildModule::m_pGuildModule)
	{
		isLoad = GuildModule::m_pGuildModule->LoadResource(pKernel);
	}
	return true == isLoad ? 1 : 0;
}

//�������
int nx_guild_create(void* state)
{
    IKernel* pKernel = LuaExtModule::GetKernel(state);
    if (pKernel == NULL)
    {
        return 1;
    }

    CHECK_ARG_NUM(state, nx_guild_create, 2);
    CHECK_ARG_OBJECT(state, nx_guild_create, 1)
    CHECK_ARG_WIDESTR(state, nx_guild_create, 2);

    // read params
    PERSISTID self = pKernel->LuaToObject(state, 1);
    const wchar_t* guildName = pKernel->LuaToWideStr(state, 2);

    // ģ��ͻ�����Ϣ
    CVarList msg;
	msg << CLIENT_CUSTOMMSG_GUILD << CS_GUILD_CREATE << guildName << 1 << L"T";
    GuildModule::m_pGuildModule->OnCustomMessage(pKernel, self, self, msg);
    return 1;
}

//��ɢ���
int nx_guild_dismiss(void* state)
{
    IKernel *pKernel = LuaExtModule::GetKernel(state);
    if (pKernel == NULL)
    {
        return 1;
    }

    CHECK_ARG_NUM(state, nx_guild_dismiss, 1);
    CHECK_ARG_OBJECT(state, nx_guild_dismiss, 1);

    PERSISTID self = pKernel->LuaToObject(state, 1);

    CVarList msg;
    msg << CLIENT_CUSTOMMSG_GUILD << CS_GUILD_DISMISS;
    GuildModule::m_pGuildModule->OnCustomMessage(pKernel, self, self, msg);
    return 1;
}

int nx_guild_add_value(void*state)
{
    IKernel *pKernel = LuaExtModule::GetKernel(state);
    if (pKernel == NULL)
    {
        return 1;
    }

	CHECK_ARG_NUM(state, nx_guild_add_value, 3);
	CHECK_ARG_OBJECT(state, nx_guild_add_value, 1);
	CHECK_ARG_INT(state, nx_guild_add_value, 2);
	CHECK_ARG_INT(state, nx_guild_add_value, 3);

    PERSISTID self = pKernel->LuaToObject(state, 1);
    int type = pKernel->LuaToInt(state, 2);
	int value = pKernel->LuaToInt(state, 3);
	
	const wchar_t*guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
	const wchar_t *playerName = pKernel->QueryWideStr(self, FIELD_PROP_NAME);
	GuildModule::m_pGuildModule->AddGuildNumValue(pKernel,guildName,static_cast<GUILD_NUM_TYPE>(type),value,GUILD_NUM_CHANGE::GUILD_NUM_CHANGE_GM,playerName);
 

	return 1;
}

int nx_guild_dec_value(void*state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	if (pKernel == NULL)
	{
		return 1;
	}

	CHECK_ARG_NUM(state, nx_guild_dec_value, 3);
	CHECK_ARG_OBJECT(state, nx_guild_dec_value, 1);
	CHECK_ARG_INT(state, nx_guild_dec_value, 2);
	CHECK_ARG_INT(state, nx_guild_dec_value, 3);

	PERSISTID self = pKernel->LuaToObject(state, 1);
	int type = pKernel->LuaToInt(state, 2);
	int value = pKernel->LuaToInt(state, 3);

	const wchar_t*guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
	const wchar_t *playerName = pKernel->QueryWideStr(self, FIELD_PROP_NAME);
	GuildModule::m_pGuildModule->DecGuildValue(pKernel, static_cast<GUILD_NUM_TYPE>(type), guildName, value, GUILD_NUM_CHANGE::GUILD_NUM_CHANGE_GM);
	return 1;
}

//���ӽ�������
int nx_guild_add_builing_exp(void *state)
{
    IKernel *pKernel = LuaExtModule::GetKernel(state);
    if (pKernel == NULL)
    {
        return 1;
    }

	CHECK_ARG_NUM(state, nx_guild_add_value, 2);
	CHECK_ARG_OBJECT(state, nx_guild_add_value, 1);
	CHECK_ARG_INT(state, nx_guild_add_value, 2);

    PERSISTID self = pKernel->LuaToObject(state, 1);
	int value = pKernel->LuaToInt(state, 2);
	
	const wchar_t*guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
	GuildModule::m_pGuildModule->AddGuildBuildingExp(pKernel, guildName,value);
	return 1;

}
//���Ӽ��ܾ���
int nx_guild_add_skill_exp(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	if (pKernel == NULL)
	{
		return 1;
	}

	CHECK_ARG_NUM(state, nx_guild_add_value, 2);
	CHECK_ARG_OBJECT(state, nx_guild_add_value, 1);
	CHECK_ARG_INT(state, nx_guild_add_value, 2);

	PERSISTID self = pKernel->LuaToObject(state, 1);
	int value = pKernel->LuaToInt(state, 2);

	const wchar_t*guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
	GuildModule::m_pGuildModule->AddGuildSkillExp (pKernel, guildName, value);
	return 1;

}



int nx_tigger_guild_timer(void*state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	if (pKernel == NULL)
	{
		return 1;
	}

	CHECK_ARG_NUM(state, nx_tigger_guild_timer, 1);
	CHECK_ARG_OBJECT(state, nx_tigger_guild_timer, 1);
	PERSISTID self = pKernel->LuaToObject(state, 1);
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj != NULL)
	{
		//2.1���ӳ��������ϻ�ȡ����ǳ�
		const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
		if (StringUtil::CharIsNull(guildName))
		{
			return 0;
		}

		//2.2�����µ��Ĺ���������
		CVarList pubMsg;
		pubMsg << PUBSPACE_GUILD << guildName << SP_GUILD_TIGGER_TIMER;
		pKernel->SendPublicMessage(pubMsg);
	}

	return 1;

}

// ��ʼ��
bool GuildModule::Init(IKernel* pKernel)
{
	if (NULL == pKernel)
	{
		Assert(false);

		return false;
	}

    m_pGuildModule = this;

    m_pCapitalModule = static_cast<CapitalModule*>(pKernel->GetLogicModule("CapitalModule"));
    m_pVirtualRecModule = static_cast<VirtualRecModule*>(pKernel->GetLogicModule("VirtualRecModule"));
    m_pItemBaseModule = static_cast<ItemBaseModule*>(pKernel->GetLogicModule("ItemBaseModule"));
	m_pChatModule = static_cast<ChatModule*>(pKernel->GetLogicModule("ChatModule"));
	Assert(m_pCapitalModule != NULL && m_pVirtualRecModule != NULL && m_pItemBaseModule != NULL && m_pChatModule != NULL);

    // ע��ͻ�����Ϣ�ص�
    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_GUILD, GuildModule::OnCustomMessage);

    // �������ݷ������·�����Ϣ����ص�
    pKernel->AddEventCallback("scene", "OnPublicMessage", OnPublicMessage);

    // �ڲ�����ص�
    pKernel->AddIntCommandHook("player", COMMAND_GUILD_BY_NAME, OnCommandGuild);
	// ���������ɻص�
//	pKernel->AddIntCommandHook("player", COMMAND_DAILY_ACTIVITY, OnCommandDailyActivity);

    // ����,����
    pKernel->AddEventCallback("player", "OnRecover", OnPlayerRecover);
    pKernel->AddEventCallback("player", "OnStore", OnPlayerDisconnect);
	pKernel->AddEventCallback("player", "OnContinue", OnPlayerRecover);
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_CONTINUE_ON_READY, OnReady);
	pKernel->AddEventCallback("player", "OnReady", OnReady);
	// ���������������Ϣ
	pKernel->AddIntCommandHook("player", COMMAND_GUILD_UP_LEVEL, OnGuildLevelChange);
	pKernel->AddIntCommandHook("player", COMMAND_GUILD_DOWN_LEVEL, OnGuildLevelChange);
	pKernel->AddIntCommandHook("player", COMMAND_BEKILL, OnCommandBeKilled);

    // �ȼ��仯
    pKernel->AddIntCommandHook("player", COMMAND_LEVELUP_CHANGE, OnLevelChange);

	DECL_CRITICAL(GuildModule::OnBattleAbilityChange);
	DECL_CRITICAL(GuildModule::C_OnGuildPositionChange);
	DECL_CRITICAL(GuildModule::C_OnPlayerdVipLv);

	//DECL_HEARTBEAT(GuildModule::H_TeachAutoRun);

    DECL_LUA_EXT(nx_guild_create);
    DECL_LUA_EXT(nx_guild_dismiss);
	DECL_LUA_EXT(nx_reload_guild_config);
	DECL_LUA_EXT(nx_guild_add_value);
	DECL_LUA_EXT(nx_guild_dec_value);
	DECL_LUA_EXT(nx_tigger_guild_timer);
	DECL_LUA_EXT(nx_guild_add_builing_exp);
	DECL_LUA_EXT(nx_guild_add_skill_exp);

	DECL_RESET_TIMER(RESET_GUILD_DAILY_RESET, GuildModule::ResetDaily);
    if (!LoadResource(pKernel))
    {
		Assert(false);

		return false;
    }

	RELOAD_CONFIG_REG("GuildConfig", GuildModule::ReloadConfig);
    return true;
}

// �ر�
bool GuildModule::Shut(IKernel* pKernel)
{
    m_GuildUpLevelConfig.clear();
    m_GuildDonateConfig.clear();
    m_GuildPositionConfig.clear();
    m_GuildShopConfig.clear();
    return true;
}

// �������
int GuildModule::OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0; 
	}
	
	int nPrepareRole = pSelfObj->QueryInt("PrepareRole");
	// Ԥ�����˺Ų�����Ԥ����֯
	if (nPrepareRole == ROLE_FLAG_ROBOT)
	{
		return 0;
	}

    const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
    const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
	if (!m_pGuildModule->IsWidStrValid(guildName))
    {
        m_pGuildModule->SetOutLineJoinGuild(pKernel, self, playerName);
    }
   
	REGIST_RESET_TIMER(pKernel, self, RESET_GUILD_DAILY_RESET);
	//������ҲŻ���°������״̬
	int online = pSelfObj->QueryInt("Online");
	if (online == ONLINE)
	{
		m_pGuildModule->SendOnlineStateToPub(pKernel, self, ONLINE);
	}

	// ս�����仯
	ADD_CRITICAL(pKernel, self, "HighestBattleAbility", "GuildModule::OnBattleAbilityChange");
	ADD_CRITICAL(pKernel, self, "GuildPosition", "GuildModule::C_OnGuildPositionChange");
	ADD_CRITICAL(pKernel, self, "Nation", "GuildModule::C_OnPlayerNationChange");
	ADD_CRITICAL(pKernel, self, FIELD_PROP_GUILD_NAME, "GuildModule::C_OnPlayerdGuilNameChange");
	ADD_CRITICAL(pKernel, self, FIELD_PROP_VIP_LEVEL, "GuildModule::C_OnPlayerdVipLv");
	// ˢ�°��ְλ
	m_pGuildModule->UpdateGuildInfo(pKernel, self);
   
	// ˢ�°��ȼ�����������
	m_pGuildModule->UpdateGuildLevelToPlayer(pKernel, self);
	//ˢ�¹��Ἴ��
	m_pGuildModule->RefreshGuildSkill(pKernel,self);
	m_pGuildModule->RefreshGuildSysmbol(pKernel,self);
	m_pGuildModule->SendPropsChangeToPub(pKernel, self);
	return 0;
}

// �������
int GuildModule::OnPlayerDisconnect(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{    
	int reason = args.IntVal(0);
	if (reason == STORE_EXIT)
	{
		if (!pKernel->Exists(self))
		{
			return 0;
		}
		m_pGuildModule->SendOnlineStateToPub(pKernel, self, OFFLINE);
		m_pGuildModule->SendPropsChangeToPub(pKernel, self);
	}
    return 0;
}

// ��ҵȼ��仯
int GuildModule::OnLevelChange(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args)
{
    if (!pKernel->Exists(self))
    {
        return 0;
    }

    m_pGuildModule->SendPropsChangeToPub(pKernel, self);

    return 0;
}

int GuildModule::OnCommandBeKilled(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args)
{
	if (pKernel->Type(sender) != TYPE_PLAYER)
	{
		// ���Է����ɱ����Ϊ
		return 0;
	}

	// ɱ����
	IGameObj *pKiller = pKernel->GetGameObj(sender);
	if (NULL == pKiller)
	{
		return 0;
	}

	// ������
	IGameObj *pDeader = pKernel->GetGameObj(self);
	if (NULL == pDeader)
	{
		return 0;
	}

	// ��ȡ������
	const wchar_t* guildName = pDeader->QueryWideStr( FIELD_PROP_GUILD_NAME );
	if (StringUtil::CharIsNull(guildName))
	{
		return 0;
	}

	const wchar_t* pwszName_ = pDeader->QueryWideStr(FIELD_PROP_NAME);
	int nVip_ = pDeader->QueryInt(FIELD_PROP_VIP_LEVEL);

	const wchar_t* pwszTargetName_ = pKiller->QueryWideStr(FIELD_PROP_NAME);
	int nTargetVip_ = pKiller->QueryInt(FIELD_PROP_VIP_LEVEL);

	int nSceneId_ = pKernel->GetSceneId();
	len_t fx_ = pDeader->GetPosiX();
	len_t fy_ = pDeader->GetPosiY();
	len_t fz_ = pDeader->GetPosiZ();

	CVarList msg;
	msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_MEMBER_BEKILL_BROADCAST << pwszName_ << nVip_
		<< pwszTargetName_ << nTargetVip_ << nSceneId_ << fx_ << fy_ << fz_;
	GuildModule::BroadCastGuild(pKernel, guildName, msg);

	return 0;
}

// ս�����仯�ص�
int GuildModule::OnBattleAbilityChange(IKernel* pKernel, const PERSISTID& self,
									   const char* property, const IVar& old)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	if (pSelfObj->QueryInt("Online") == OFFLINE)
	{
		return 0;
	}

	m_pGuildModule->SendPropsChangeToPub(pKernel, self);

	return 0;
}

int GuildModule::C_OnGuildPositionChange(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old)
{
	IGameObj * pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return 0;
	}

	int positionNow = pSelfObj->QueryInt("GuildPosition");
	if (positionNow == GUILD_POSITION_CAPTAIN && positionNow != old.IntVal()){
		const ConfigMailItem& mail = m_pMailModule->GetConfigMail(pKernel, GUILD_CAPTAIN_MAIL);
		m_pMailModule->SendMailToPlayer(pKernel, mail.sender.c_str(), pSelfObj->QueryWideStr(FIELD_PROP_NAME),
			mail.mail_title.c_str(), mail.mail_content.c_str(), mail.items);
	}
	return 0;
}

int GuildModule::C_OnPlayerdVipLv(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	m_pGuildModule->SendPropsChangeToPub(pKernel, self);
	return 0;
}

// ���ò�������Ҽ��빫������
int GuildModule::SetOutLineJoinGuild(IKernel* pKernel, const PERSISTID& self, const wchar_t* playerName)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

    if (StringUtil::CharIsNull(playerName))
    {
        return 0;
    }

    IPubData* pGuildPubData = GetPubData(pKernel, GUILD_PUB_DATA);
    if (pGuildPubData == NULL)
    {
        return 0;
    }

    IRecord* pOutlineJoin = pGuildPubData->GetRecord(GUILD_OUTLINE_JOIN_REC);
    if (pOutlineJoin == NULL)
    {
        return 0;
    }

    int row = pOutlineJoin->FindWideStr(GUILD_OUTLINE_JOIN_REC_COL_PLAYER_NAME, playerName);
    if (row >= 0)
    {
        const wchar_t* guildName = pOutlineJoin->QueryWideStr(row, GUILD_OUTLINE_JOIN_REC_COL_GUILD_NAME);
        if (m_pGuildModule->IsWidStrValid(guildName))
        {
            pSelfObj->SetWideStr("GuildName", guildName);
			pSelfObj->SetInt("GuildPosition", GUILD_POSITION_MEMBER);
		//	LogModule::m_pLogModule->OnGuildJoin(pKernel, self, guildName);
            CVarList msg;
            msg << PUBSPACE_GUILD << guildName << SP_GUILD_MSG_DELETE_JOIN_RECORD << playerName;
            pKernel->SendPublicMessage(msg);
			// ˢ�°��
			UpdateGuildInfo(pKernel, self);
        }
    }

    return 0;
}



// �򹫹������������������״̬�仯��Ϣ
void GuildModule::SendOnlineStateToPub(IKernel* pKernel, const PERSISTID& self, int onlineState)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return;
	}

    const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
    const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
    if (IsWidStrValid(guildName))
    {
        CVarList msg;
        msg << PUBSPACE_GUILD << guildName << SP_GUILD_MSG_MEMBER_ONLINE_STATE 
            << onlineState 
            << playerName;
        pKernel->SendPublicMessage(msg);
    }
	else
	{
		// �����������˼��빫�ᣬ��Ҫ����������еļ�¼
		IPubData* pGuildPubData = GetPubData(pKernel, GUILD_PUB_DATA);
		if (pGuildPubData == NULL)
		{
			return;
		}
		IRecord* pRecord = pGuildPubData->GetRecord(GUILD_APPLY_REC);
		if (pRecord == NULL)
		{
			return;
		}

		int row = pRecord->FindWideStr(GUILD_APPLY_REC_COL_NAME, playerName);
		if (row < 0)
		{
			return;
		}

		std::wstring guildNames = pRecord->QueryWideStr(row, GUILD_APPLY_REC_COL_GUILD);
		CVarList guildList;
		::util_split_wstring(guildList, guildNames, L";");
		if (guildList.GetCount() <= 0)
		{
			return;
		}

		CVarList msg;
		msg << PUBSPACE_GUILD << guildList.WideStrVal(0)
			<< SP_GUILD_MSG_UPDATE_APPLY_ONLINE_STATE
			<< playerName
			<< guildNames
			<< onlineState;
		pKernel->SendPublicMessage(msg);
	}
}

// �򹫹�����������������Ա仯����Ϣ���ȼ���ս������
void GuildModule::SendPropsChangeToPub(IKernel* pKernel, const PERSISTID& self)
{
    IGameObj* pPlayer = pKernel->GetGameObj(self);
    if (pPlayer == NULL)
    {
        return;
    }

    const wchar_t* guildName = pPlayer->QueryWideStr("GuildName");
    const wchar_t* playerName = pPlayer->QueryWideStr("Name");
    int level = pPlayer->QueryInt("Level");
    int ability = pPlayer->QueryInt("HighestBattleAbility");
	int vipLv = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
    // ���µȼ���ս�����ı仯
	if (m_pGuildModule->IsWidStrValid(guildName))
    {
        // ����ǹ����Ա����Ҫ���¹����Ա���ļ�¼
        CVarList msg;
        msg << PUBSPACE_GUILD << guildName 
            << SP_GUILD_MSG_UPDATE_PROPS 
            << playerName
			<< level << ability << vipLv;
        pKernel->SendPublicMessage(msg);
    }
    else
    {
        // �����������˼��빫�ᣬ��Ҫ����������еļ�¼
        IPubData* pGuildPubData = GetPubData(pKernel, GUILD_PUB_DATA);
        if (pGuildPubData == NULL)
        {
            return;
        }
        IRecord* pRecord = pGuildPubData->GetRecord(GUILD_APPLY_REC);
        if (pRecord == NULL)
        {
            return;
        }

        int row = pRecord->FindWideStr(GUILD_APPLY_REC_COL_NAME, playerName);
        if (row < 0)
        {
            return;
        }

        std::wstring guildNames = pRecord->QueryWideStr(row, GUILD_APPLY_REC_COL_GUILD);
        CVarList guildList;
        ::util_split_wstring(guildList, guildNames, L";");
        if (guildList.GetCount() <= 0)
        {
            return;
        }

        CVarList msg;
        msg << PUBSPACE_GUILD << guildList.WideStrVal(0) 
            << SP_GUILD_MSG_UPDATE_APPLY_PROPS 
            << playerName 
            << guildNames
			<< level << ability << vipLv;
        pKernel->SendPublicMessage(msg);
    }
}

// ������Դ
bool GuildModule::LoadResource(IKernel* pKernel)
{
	const char* path = pKernel->GetResourcePath();
	if (!LoadGuildUpLevelConfig(path, m_GuildUpLevelConfig))
    {
        return false;
    }
	if (!LoadGuildDonateConfig(path, m_GuildDonateConfig))
    {
        return false;
    }
	if (!LoadGuildPositionConfig(path, m_GuildPositionConfig))
    {
        return false;
    }
	if (!LoadGuildShopConfig(path, m_GuildShopConfig))
    {
        return false;
    }
	if (!LoadGuildSkill(path, m_guildSkill))
	{
		return false;
	}
	if (!LoadGuildSkillLevelUpConst(path, m_guildSkillLevelUpCost))
	{
		return false;
	}
    return true;
}

// ��ȡ���ȼ�
int GuildModule::GetGuildLevel(IKernel* pKernel, const wchar_t* guildName)
{
	return GuildBuildingLevel(pKernel,guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_JU_YI_TING);
}

// ���wchar_t�Ƿ���Ч
bool GuildModule::IsWidStrValid(const wchar_t* widStr)
{
    return !StringUtil::CharIsNull(widStr);
}

// ����ܷ񴴽�����
bool GuildModule::CheckCanCreateGuild(IKernel* pKernel, const PERSISTID& self, const wchar_t* guildName)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

    // ��⹫�������Ƿ���Ч
    if (!IsWidStrValid(guildName))
    {
        return false;
    }

	if (ChatModule::m_pChatModule->CheckBadWords(std::wstring(guildName)))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17603, CVarList());
		return false;
	}


    // �����ҵȼ��Ƿ��������
    if (ActivateFunctionModule::CheckActivateFunction(pKernel, self, AFM_GUILD_FUNCTION))
    {
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17604, CVarList());
        return false;
    }

    // �������Ƿ��Ѿ������˹���
    const wchar_t* name = pSelfObj->QueryWideStr("GuildName");
	if (IsWidStrValid(name))
    {
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17605, CVarList());
        return false;
    }

    // ��⹫�������Ƿ�����Ƿ��ַ�
    if (!pKernel->CheckName(guildName))
    {
        // ��ʾ�����Ƿ��ַ�
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17603, CVarList());
        return false;
    }

	// �������Ʋ����С�;"�ţ��÷�����Ϊ�ָ�ʹ��
	std::wstring strGuildName(guildName);
	if (strGuildName.find(L";") != std::wstring::npos)
	{
        // �������Ʋ����С�;"�ţ��÷�����Ϊ�ָ�ʹ��
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17603, CVarList());
		return false;
	}

    // ��⹫�����Ƴ���
    if (wcslen(guildName) > GUILD_NAME_MAX_LENGTH)
    {
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17606, CVarList());
        return false;
    }

    // ����Ƿ����㹻�Ľ�Ǯ
	int nCostGold = EnvirValueModule::EnvirQueryInt(ENV_VALUE_CREATE_GUILD_GOLD);
	if (!CapitalModule::m_pCapitalModule->CanDecCapital(pKernel, self, CAPITAL_GOLD, (__int64)nCostGold))
    {
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17607, CVarList());
        return false;
    }

	// ��鹫�����Ƿ�ﵽ����
	IRecord* pGuildList = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		return false;
	}
	if (pGuildList->GetRows() >= GUILD_LIST_REC_ROW_MAX)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17608, CVarList());
		return false;
	}

    // ��⹫���Ƿ�����
    if (IsGuildExist(pKernel, guildName))
    {
        // �����������޷�����
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17609, CVarList());
        return false;
    }

    return true;
}

// ����Ƿ���Խ��������Ƿ���빫��Ĳ���
bool GuildModule::CheckCanOptJoinGuild(IKernel* pKernel, const PERSISTID& self, const wchar_t* applyName)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

    //// ��������Ƿ���Ȩ��
	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_RATIFY))
	{
		// ��Ȩ����
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17610, CVarList());
		return false;
	}

    const wchar_t *guildName = pSelfObj->QueryWideStr("GuildName");
    if ( StringUtil::CharIsNull(guildName))
    {
        return false;
    }

    // ����������Ƿ�������ù���ļ�¼
    IRecord* applyRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_APPLY_REC);
    if (applyRecord == NULL)
    {
        return false;
    }
    int row = applyRecord->FindWideStr(GUILD_APPLY_REC_COL_NAME, applyName);
    if (row < 0)
    {
        return false;
    }
    const wchar_t *applyGuildString = applyRecord->QueryWideStr(row, GUILD_APPLY_REC_COL_GUILD);
    if (applyGuildString == NULL)
    {
        return false;
    }
    // û������������
    if (wcsstr(applyGuildString, guildName) == NULL)
    {
        return false;
    }
	

    // ����������Ƿ��ڸù�����������б���
    IPubData* pGuildData = GetPubData(pKernel, guildName);
    if (pGuildData == NULL)
    {
        return false;
    }
    IRecord* joinRecord = pGuildData->GetRecord(GUILD_JOIN_REC);
    if (joinRecord == NULL)
    {
        return false;
    }
    row = joinRecord->FindWideStr(GUILD_JOIN_REC_COL_PLAYER_NAME, applyName);
    if (row < 0)
    {
        return false;
    }

	//����Ƿ�ͬһ����
	IRecord* pGuildListRec = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
    if (pGuildListRec == NULL)
    {
        return false;
    }
	int guildListRow = pGuildListRec->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
    if (guildListRow < 0)
    {
        return false;
    }

    return true;
}

// �������Ƿ��ǹ���᳤
bool GuildModule::CheckIsCaptain(IKernel* pKernel, const PERSISTID& self)
{
    int row = 0;
    IRecord* pGuildRecord = GetGuildRecord(pKernel, self, row);
    if (pGuildRecord == NULL)
    {
        return false;
    }

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

    const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
    const wchar_t* captainName = pGuildRecord->QueryWideStr(row, GUILD_LIST_REC_COL_CAPTAIN);
    if (!IsWidStrValid(captainName) || wcscmp(playerName, captainName) != 0)
    {
        return false;
    }

    return true;
}

int GuildModule::OnReady(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args)
{
	
	m_pGuildModule->InformGuildApply(pKernel, self);
	// ͬ������
//	m_pGuildModule->OnTeahHandleClientReady(pKernel, self);

	return 0;
}

void GuildModule::InformGuildApply(IKernel*pKernel, const PERSISTID & self)
{
	//֪ͨ���µĹ�������
	// ����Ƿ���Ȩ��
	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_RATIFY))
	{
		return;
	}
	const wchar_t* guildName = GetPlayerGuildName(pKernel, self);
	if (!IsWidStrValid(guildName))
	{
		return ;
	}
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return;
	}

	IRecord *joinRecord = pGuildData->GetRecord(GUILD_JOIN_REC);
	if (joinRecord == NULL)
	{
		return;

	}
	
	if (joinRecord->GetRows() > 0)
	{
		pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD
			<< SC_GUILD_NEW_APPLY << NEW_NOTIFY_SHOW);
	}

}


void GuildModule::CustomMessageToGuildMember(IKernel*pKernel, const wchar_t* guildName, const IVarList&  msg)
{
	IPubData* pGuildData = m_pGuildModule->GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return ;
	}
	// ��Ա���
	IRecord* pRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pRecord == NULL)
	{
		return;
	}
	// ��Ա����
	int memberCount = pRecord->GetRows();
	// ֪ͨ��Ա���ȼ��ı�
	LoopBeginCheck(a);
	for (int i = 0; i < memberCount; ++i)
	{
		LoopDoCheck(a);

		const wchar_t* playerName = pRecord->QueryWideStr(i, GUILD_MEMBER_REC_COL_NAME);
		if (StringUtil::CharIsNull(playerName))
		{
			continue;
		}
		if (pKernel->GetPlayerScene(playerName) > 0)
		{
			// ֪ͨ���
			pKernel->CustomByName(playerName, msg);
		}
	}

}

void GuildModule::CommandToGuildMember(IKernel*pKernel, const wchar_t* guildName, const IVarList& msg)
{
	IPubData* pGuildData = m_pGuildModule->GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return;
	}
	// ��Ա���
	IRecord* pRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pRecord == NULL)
	{
		return;
	}
	// ��Ա����
	int memberCount = pRecord->GetRows();
	// ֪ͨ��Ա���ȼ��ı�
	LoopBeginCheck(a);
	for (int i = 0; i < memberCount; ++i)
	{
		LoopDoCheck(a);

		const wchar_t* playerName = pRecord->QueryWideStr(i, GUILD_MEMBER_REC_COL_NAME);
		if (StringUtil::CharIsNull(playerName))
		{
			continue;
		}
		if (pKernel->GetPlayerScene(playerName) > 0)
		{
			// ֪ͨ���
			pKernel->CommandByName(playerName, msg);
		}
	}

}

// ����Ƿ�����˳�����
bool GuildModule::CheckCanQuit(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

    // û�й��ᣬ��Ȼ����Ҫ�˳�
    const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
    if (!IsWidStrValid(guildName))
    {
        return false;
    }

    // �᳤�����˳�����
    if (CheckIsCaptain(pKernel, self))
    {
        return false;
    }

    return true;
}

// ����Ƿ�����߳���Ա
bool GuildModule::CheckCanFire(IKernel* pKernel, const PERSISTID& self, const wchar_t* member)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

    // �����߳��Լ�
    const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
    if (wcscmp(playerName, member) == 0)
    {
        return false;
    }

    // ֻ�л᳤�ſ����߳���Ա
    if (!CheckPositionPriv(pKernel, self,GUILD_PRIV_FIRE))
    {
		// ��Ȩ����
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17611, CVarList());
        return false;
    }

	// ����Ƿ�ﵽ������������
	if (!CheckFireLimit(pKernel, self))
	{
		// ���˴�������
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17612, CVarList());
		return false;
	}

    // �Ƿ��Ǳ�����Ա
    int captainRow = 0;
    IRecord* pMemberRecord = GetMemberRecord(pKernel, self, captainRow);
    if (pMemberRecord == NULL)
    {
        return false;
    }

    int memberRow = pMemberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, member);
    if (memberRow < 0)
    {
        return false;
    }

	//���ǰ�����ֻ���߳����Լ�ְλ�͵����
	if (!CheckIsCaptain(pKernel, self))
	{
		int selfRow = pMemberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, playerName);
		if (selfRow < 0)
		{
			return false;
		}

		//��ȡ���ְλ
		int iSelfPos = pMemberRecord->QueryInt(selfRow, GUILD_MEMBER_REC_COL_POSITION);
		//��ȡ������ְλ
		int iMemberPosi = pMemberRecord->QueryInt(memberRow, GUILD_MEMBER_REC_COL_POSITION);

		if (iMemberPosi <= iSelfPos)
		{
			return false;
		}
	}

    return true;
}

// ����Ƿ���Խ�ɢ����
bool GuildModule::CheckCanDismiss(IKernel* pKernel, const PERSISTID& self)
{
    // ֻ�л᳤�ſ��Խ�ɢ����
    if (!CheckIsCaptain(pKernel, self))
    {
        return false;
    }

    return true;
}

// ��������뿪����
bool GuildModule::SetLeaveGuild(IKernel* pKernel, const PERSISTID& self, int64_t leaveDate, bool isSetQuitGuildTime)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

    pSelfObj->SetWideStr("GuildName", L"");
	pSelfObj->SetInt("GuildPosition", GUILD_POSITION_NONE);
	//if (isSetQuitGuildTime)
	{
		pSelfObj->SetInt64("QuitGuildDate", leaveDate);
	}

	//LogModule::m_pLogModule->OnGuildQuit(pKernel, self);

	// ˢ����Ҫ����������־
	UpdateNewApplyByPosition(pKernel, self);
    return true;
}

//���л��������
bool GuildModule::SerialGuildInfo(IKernel *pKernel, const PERSISTID& self, CVarList &varGuild, int randVal, const wchar_t *guildName)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

    // ��ô˰���ڰ���б��е��к�
    IRecord *pGuildListRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
    if (pGuildListRecord == NULL)
    {
        return false;
    }
    int guildRow = pGuildListRecord->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
    if (guildRow < 0)
    {
        return false;
    }
	int guildLevel= GetGuildLevel(pKernel,guildName);

    IRecord* applyRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_APPLY_REC);
    if (applyRecord == NULL)
    {
        return false;
    }
    // �������Ƿ��Ѿ������˸ð��
    int ifRequested = 0;
    const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
    int applyRow = applyRecord->FindWideStr(GUILD_APPLY_REC_COL_NAME, playerName);
    if (applyRow >= 0)
    {
        const wchar_t *requestedGuildStr = applyRecord->QueryWideStr(applyRow, GUILD_APPLY_REC_COL_GUILD);
        if (requestedGuildStr != NULL)
        {
            ifRequested = (wcsstr(requestedGuildStr, guildName) != NULL ? 1 : 0);
        }
    }
    
    // ��ð���Ա����
    IPubData* pGuildData = GetPubData(pKernel, guildName);
    if (pGuildData == NULL)
    {
        return false;
    }
    IRecord* memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
    if (memberRecord == NULL)
    {
        return false;
    }
	int xiangFangLevel = GuildBuildingLevel(pKernel, guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);
    // ��ð������Ա����
	GuildUpLevelConfig_t *upLevelConfig = GetGuildUpLevelConfig(xiangFangLevel);
    if (upLevelConfig == NULL)
    {
        return false;
    }

	// ��������
    const wchar_t *captainName = pGuildListRecord->QueryWideStr(guildRow, GUILD_LIST_REC_COL_CAPTAIN);
	// �����ȼ�
    int captainLevel = pGuildListRecord->QueryInt(guildRow, GUILD_LIST_REC_COL_CAPTAIN_LEVEL);
	// �������
	const wchar_t* notice = pGuildListRecord->QueryWideStr(guildRow, GUILD_LIST_REC_COL_DECLARATION);
	//����Ա����
    int curMember = memberRecord->GetRows();
	int ability =  pGuildListRecord->QueryInt(guildRow, GUILD_LIST_REC_COL_FIGHT_ABILITY);

	varGuild << randVal << guildName << guildLevel << captainName << captainLevel
		<< upLevelConfig->m_MaxMember << curMember << ifRequested << notice << ability;
    return true;
}

//��ȡ�������������Ϣ
GuildUpLevelConfig_t *GuildModule::GetGuildUpLevelConfig(int lvl)
{
    if (0 > lvl || lvl >= (int)m_GuildUpLevelConfig.size())
    {
        return NULL;
    }
    return &(m_GuildUpLevelConfig[lvl]);
}

//���ְλ�̵깺��ӳɱ���
int GuildModule::GetGuildShopMemberLimit(int guildPosition, int rawLimit)
{
	/*if (guildPosition <= GUILD_POSITION_NONE || guildPosition >= GUILD_POSITION_COUNT)
	{
		return 0;
	}

	GuildPositionConfig_t& data = m_GuildPositionConfig[guildPosition];
	float scale = (float)data.m_BuyItemScale / 100.0f;
	*/
	return rawLimit;
}

//��ȡ���ְλ��Ϣ
GuildPositionConfig_t *GuildModule::GetGuildPositionConfig(int position)
{
    GuildPositionConfigVectorIter iter(m_GuildPositionConfig.begin());
	// ѭ������
	LoopBeginCheck(dd);
    for ( ; iter != m_GuildPositionConfig.end(); ++iter)
    {
		LoopDoCheck(dd);
        if (iter->m_Position == position)
        {
            return &(*iter);
        }
    }
    return NULL;
}

// �鿴���ְλȨ���Ƿ�����
bool GuildModule::CheckPositionPriv(IKernel* pKernel, const PERSISTID & self, GuildPriv iPrivType)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	//ȷ����Ұ��
	const wchar_t * guildName = pSelfObj->QueryWideStr("GuildName");
	if (StringUtil::CharIsNull(guildName))
	{
		return false;
	}

	int position = pSelfObj->QueryInt("GuildPosition");

	return CheckPositionPriv(pKernel, position, iPrivType);
}

// �鿴���ְλȨ���Ƿ�����
bool GuildModule::CheckPositionPriv(IKernel* pKernel, int position, GuildPriv iPrivType)
{
	//��ȡȨ�޶�Ӧ������Ϣ
	GuildPositionConfig_t *positionConfig = GetGuildPositionConfig(position);
	if (positionConfig == NULL)
	{
		return false;
	}


	return (positionConfig->m_privList & (1 << iPrivType)) > 0;

	////ȷ�϶�Ӧ����Ϣ
	//bool bRetValue = false;
	//switch(iPrivType)
	//{
	//case GUILD_PRIV_SET_POSITION:
	//	bRetValue = positionConfig->m_Promotion > 0 ? true : false;
	//	break;
	//case GUILD_PRIV_DEMOTION:
	//	bRetValue = positionConfig->m_Demotion > 0 ? true : false;
	//	break;
	//case GUILD_PRIV_RATIFY:
	//	bRetValue = positionConfig->m_Ratify > 0 ? true : false;
	//	break;
	//case GUILD_PRIV_FIRE:
	//	bRetValue = positionConfig->m_Fire > 0 ? true : false;
	//	break;
	//case GUILD_PRIV_ANNOUNCEMENT:
	//	bRetValue = positionConfig->m_Announcement > 0 ? true : false;
	//	break;
	//case GUILD_PRIV_APPLY_DEFEND_ACTIVITY:
	//	bRetValue = positionConfig->m_ApplyActivity > 0 ? true : false;
	//	break;
	//case GUILD_PRIV_AUTO_AGREE:
	//	bRetValue = positionConfig->m_autoAgree>0?true:false;
	//	break;
	//case GUILD_PRIV_STATION_DANCING_GIRL:
	//	bRetValue = positionConfig->m_DancingGirl > 0 ? true: false;
	//	break;
	//case GUILD_PRIV_UNLOCK_SKILL:
	//	bRetValue = positionConfig->m_unLockSkill > 0 ? true : false;
	//	break;
	//case GUILD_PRIV_CHANGE_SHOP:
	//	bRetValue = positionConfig->m_changeShopList > 0 ? true : false;
	//	break;
	//default:
	//	break;
	//}

	//return bRetValue;
}

// ֪ͨ����߹������µļ���������ʾ,����ȡ������������ʾ
bool GuildModule::UpdateNewApplyToAuditor(IKernel* pKernel, const PERSISTID& player)
{	
	IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if (pPlayerObj == NULL)
	{
		return 0;
	}

	if (GuildModule::m_pGuildModule == NULL)
	{
		return 0;
	}

	// ��ȡ����������¼��
	const wchar_t * guildName = pPlayerObj->QueryWideStr("GuildName");
	if (StringUtil::CharIsNull(guildName))
	{
		return 0;
	}

	// ����Ƿ�������Ȩ��
	if (!GuildModule::m_pGuildModule->CheckPositionPriv(pKernel, player, GUILD_PRIV_RATIFY))
	{
		return 0;
	}
	
	int applyCount = GuildModule::m_pGuildModule->GetGuildApplyCount(pKernel, guildName);

	return applyCount > 0;
}

// ְλ�����䶯��ˢ��������ʾ
int GuildModule::UpdateNewApplyByPosition(IKernel* pKernel, const PERSISTID& player)
{
	//IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	//if (pPlayerObj == NULL)
	//{
	//	return 0;
	//}

	//// �Ƿ�������Ȩ��
	//bool bPriv = CheckPositionPriv(pKernel, player, GUILD_PRIV_RATIFY);

	//// ��ȡ����������¼��
	//const wchar_t * guildName = pPlayerObj->QueryWideStr("GuildName");
	//int applyCount = GetGuildApplyCount(pKernel, guildName);
	//bool bNewApply = (applyCount > 0 ? NEW_NOTIFY_SHOW : NEW_NOTIFY_HIDE);

	//// ˢ�º��״̬
	//pKernel->Command(player, player, CVarList() 
	//	<< COMMAND_MSG_NEW_NOTIFY 
	//	<< NEW_NOTIFY_GUILD 
	//	<< NOTIFY_GUILD_APPLY 
	//	<< (bPriv ? bNewApply : NEW_NOTIFY_HIDE));
	m_pGuildModule->InformGuildApply(pKernel, player);
	return 0;
}


// ֪ͨ��������߹������µļ�������, ����ȡ������������ʾ
// guildName ��������
int GuildModule::UpdateNewApplyToAllAuditor(IKernel* pKernel, const wchar_t* guildName)
{
	if (StringUtil::CharIsNull(guildName))
	{
		return 1;
	}

	// ��ȡ��ṫ������
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 1;
	}

	// ��ȡ����Ա��Ϣ��
	IRecord *memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (memberRecord == NULL)
	{
		return 1;
	}

	// ��ȡ����������¼��
	int applyCount = GetGuildApplyCount(pKernel, guildName);
	int notifyType = applyCount > 0 ? NEW_NOTIFY_SHOW : NEW_NOTIFY_HIDE;

	int count = memberRecord->GetRows();
	// ѭ������
	LoopBeginCheck(de);
	for (int i = 0; i < count; ++i)
	{
		LoopDoCheck(de);
		// �Ƿ�����
		int online = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_ONLINE);
		if (online != ONLINE)
		{
			continue;
		}
		
		// ����Ƿ�������Ȩ��
		int position = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_POSITION);
		if (position == GUILD_POSITION_MEMBER || !CheckPositionPriv(pKernel, position, GUILD_PRIV_RATIFY))
		{
			continue;
		}
		
		// ��ȡ�������
		const wchar_t* name = memberRecord->QueryWideStr(i, GUILD_MEMBER_REC_COL_NAME);
		if (StringUtil::CharIsNull(name))
		{
			continue;
		}
		pKernel->CustomByName(name, CVarList() << SERVER_CUSTOMMSG_GUILD
			<< SC_GUILD_NEW_APPLY << notifyType);
	
	}

 	return 0;
}

//��ȡ����������¼��
int GuildModule::GetGuildApplyCount(IKernel* pKernel, const wchar_t* guildName)
{
	if (StringUtil::CharIsNull(guildName))
	{
		return 0;
	}

	// ��ȡ��ṫ������
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}

	// ��ȡ���������
	IRecord* pRecord = pGuildData->GetRecord(GUILD_JOIN_REC);
	if (pRecord == NULL)
	{
		return 0;
	}

	return pRecord->GetRows();
}

// ˢ�°��
void GuildModule::UpdateGuildInfo(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return;
	}

	// �Ƿ������
	const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
	if (StringUtil::CharIsNull(guildName))
	{
		// û�а�� ְλ����ΪNONE
		pSelfObj->SetInt("GuildPosition", GUILD_POSITION_NONE);
		return;
	}

	// �������
	const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(playerName))
	{
		return;
	}

	// �Ƿ��ܹ��鵽�������
	bool ret = false;
	LoopBeginCheck(a);
	do
	{
		LoopDoCheck(a);
		//ȷ����Ұ��
		IPubData* pGuildData = GetPubData(pKernel, guildName);
		if (pGuildData == NULL)
		{
			break;
		}

		// ����Ա��
		IRecord *memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
		if (memberRecord == NULL)
		{
			break;
		}

		//��UIDȷ����ѯ��������չ
		const char * selfUID = pKernel->SeekRoleUid(playerName);
		int selfRow = memberRecord->FindString(GUILD_MEMBER_REC_COL_NAME_UID, selfUID);
		if (selfRow < 0)
		{
			break;
		}
		
		// ���ְλ
		int selfPosition = memberRecord->QueryInt(selfRow, GUILD_MEMBER_REC_COL_POSITION);
		pSelfObj->SetInt("GuildPosition", selfPosition);

		ret = true;
		
	}while(false);

	if (!ret)
	{
		// ��������Ѿ������� ���� �Լ��Ѿ������ڸð���Ա
		pSelfObj->SetWideStr("GuildName", L"");
		pSelfObj->SetInt("GuildPosition", GUILD_POSITION_NONE);
		//LogModule::m_pLogModule->OnGuildQuit(pKernel, self);
	}
}

// ������˴���
bool GuildModule::CheckFireLimit(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return false;
	}
	//������ڹ�������
	const wchar_t *guildName = pPlayer->QueryWideStr("GuildName");
	if (StringUtil::CharIsNull(guildName))
	{
		return false;
	}
	IPubSpace *pGuildSpace = pKernel->GetPubSpace(PUBSPACE_GUILD);
	if (NULL == pGuildSpace)
	{
		return false;
	}

	//����Ƶ��
	std::wstring guildDataName = guildName + util_int_as_widestr(pKernel->GetServerId());
	IPubData *pPubDate = pGuildSpace->GetPubData(guildDataName.c_str());
	if (NULL == pPubDate)
	{
		return false;
	}

	// ����Ƿ��ѵ����˵��������
	int nFireNum = pPubDate->QueryAttrInt("FireNum");
	int nMaxFireNum = EnvirValueModule::EnvirQueryInt(ENV_VALUE_MAX_FIRE_NUM);
	if (nFireNum >= nMaxFireNum)
	{
		return false;
	}

	return true;
}


int GuildModule::GetGuildSkillLevelUpSpend(int skillID, int skillLevel)
{

	//GUildSkillLevelUpSpendIter its = m_guildSkillLevelUpCost.find(skillID);
	//if (its == m_guildSkillLevelUpCost.end())
	//{
	//	return 0;
	//}
	//std::vector<GuildSkillLevelUpSpend>::iterator itt = its->second.begin();
	//for (; itt != its->second.end(); itt++)
	//{
	//	if (itt->m_level == skillLevel)
	//	{
	//		return  itt->m_spend;
	//	}
	//}
	return 0;
}

void GuildModule::RefreshGuildSysmbol(IKernel*pKernel, const PERSISTID& self)
{

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}
	const wchar_t *guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(guildName)){
		return;
	}



	IPubData* pGuildPubData = GetPubData(pKernel, GUILD_PUB_DATA);
	if (pGuildPubData == NULL)
	{
		return;
	}

	IRecord* pGuildSysmbol = pGuildPubData->GetRecord(GUILD_SYSMBOL_REC);
	if (pGuildSysmbol == NULL)
	{
		return;
	}

	int row = pGuildSysmbol->FindWideStr(GUILD_SYSMBOL_REC_COL_GUILD_NAME, guildName);
	if (row < 0){
		return;
	}


	int guildIDentifying = pGuildSysmbol->QueryInt(row,GUILD_SYSMBOL_REC_COL_IDENTIFYING);
	const wchar_t*guildShortName = pGuildSysmbol->QueryWideStr(row, GUILD_SYSMBOL_REC_COL_SHORT_NAME);

	pSelfObj->SetWideStr(FIELD_PROP_GUILD_SHORT_NAME, guildShortName);
	pSelfObj->SetInt(FIELD_PROP_GUILD_IDENTIFYING, guildIDentifying);



}

int GuildModule::ResetDaily(IKernel* pKernel, const PERSISTID& self, int slice)
{

	IGameObj*pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){ return 0; }

	pSelfObj->SetInt(FIELD_PROP_GUILD_DONATE_TIMES, 0);
	pSelfObj->SetInt(FIELD_PROP_GUILD_DAILY_GIF, 0);



	return 0;
}

void GuildModule::BroadCastGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& broadcast)
{
	const wchar_t* pwszGuildName_ = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
	BroadCastGuild(pKernel, pwszGuildName_, broadcast);
}

void GuildModule::BroadCastGuild(IKernel* pKernel, const wchar_t* pwszGuildName, const IVarList& broadcast)
{
	// ��ȡ������
	if (StringUtil::CharIsNull(pwszGuildName))
	{
		extend_warning(LOG_WARNING, "[GuildModule::BroadCastGuild] guild name is empty");
		return;
	}

	// ��ȡ��Ա���
	int row = 0;
	IRecord* pRecord = m_pGuildModule->GetMemberRecord(pKernel, pwszGuildName);
	if (pRecord == NULL)
	{
		extend_warning(LOG_WARNING, "[GuildModule::BroadCastGuild] guild '%s' not exists", pwszGuildName);
		return;
	}

	// ֪ͨÿ������Ա
	LoopBeginCheck(d);
	for (int r = 0; r < pRecord->GetRows(); r++)
	{
		LoopDoCheck(d);
		const wchar_t* name = pRecord->QueryWideStr(r, GUILD_MEMBER_REC_COL_NAME);
		if (StringUtil::CharIsNull(name))
		{
			continue;
		}

		//�ж�����Ƿ�����
		if (pKernel->GetPlayerScene(name) > 0)
		{
			pKernel->CustomByName(name, broadcast);
		}
	}
}

void GuildModule::AddGuildLog(IKernel*pKernel, const PERSISTID& self, int type, const IVarList& args)
{
	IGameObj*pSelfObj = pKernel->GetGameObj(self);

	if (pSelfObj == NULL){
		return;
	}

	const  wchar_t* guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(guildName))
	{
		return;
	}


	std::wstring log;
	for (size_t index = 0; index < args.GetCount(); ++index)
	{
		std::wstring data;
		switch (args.GetType(index))
		{

			case VTYPE_BOOL:
				data = StringUtil::IntAsWideStr(args.BoolVal(index));
				break;
			case VTYPE_INT:
				data = StringUtil::IntAsWideStr(args.IntVal(index));
				break;
			case VTYPE_INT64:
				data = StringUtil::StringAsWideStr(StringUtil::Int64AsString(args.Int64Val(index)).c_str());
				break;
			case VTYPE_FLOAT:
				data = StringUtil::FloatAsWideStr(args.FloatVal(index));	
				break;
			case VTYPE_DOUBLE:
				
				data = StringUtil::StringAsWideStr(StringUtil::DoubleAsString(args.DoubleVal(index)).c_str());
				break;
			case VTYPE_STRING:
				data = StringUtil::StringAsWideStr(args.StringVal(index));
				break;
			case VTYPE_WIDESTR:
				data = args.WideStrVal(index);
				break;
			
			break;
			default:
				continue;
		}  

		log += data + L",";

	}






// 	CVarList pubMsg;
// 	pubMsg << PUBSPACE_GUILD << guildName << SP_GUILD_ADD_RARE_TREASURE << type << log;
// 	pKernel->SendPublicMessage(pubMsg);


}

// ���������ڹ���᳤������
const wchar_t* GuildModule::GetPlayerGuildMasterName(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return L"";
	}

	const wchar_t* wsGuildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(wsGuildName))
	{
		return L"";
	}

	// ��鹫�����Ƿ�ﵽ����
	IRecord* pGuildList = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		return L"";
	}

	int nRowIndex = pGuildList->FindWideStr(GUILD_LIST_REC_COL_NAME, wsGuildName);
	if (-1 == nRowIndex)
	{
		return L"";
	}
	const wchar_t* wsGuildMaster = pGuildList->QueryWideStr(nRowIndex, GUILD_LIST_REC_COL_CAPTAIN);
	return wsGuildMaster;
}

// �۳���֯��ֵ
bool GuildModule::DecGuildValue(IKernel* pKernel, const GUILD_NUM_TYPE type, 
	const wchar_t *guild_name, const int value, const GUILD_NUM_CHANGE src)
{
	if (StringUtil::CharIsNull(guild_name))
	{
		return false;
	}

	if (value <= 0)
	{
		return false;
	}

	// ��ǰ��֯��ֵ
	const int cur_value = m_pGuildModule->GetGuildNumValue(pKernel, guild_name, type);
	if (cur_value <= 0)
	{
		return false;
	}

	// ��۳�
	const int dec_value = value > cur_value ? cur_value : value;

	// �۳���֯��ֵ
	CVarList msg;
	msg << PUBSPACE_GUILD
		<< guild_name
		<< SP_GUILD_DEC_GUILD_NUM
		<< type
		<< dec_value
		<< src;
	pKernel->SendPublicMessage(msg);

	return true;
}

void GuildModule::SendGuildAward(IKernel* pKernel, const std::string &award, const wchar_t * guildName)
{
	CVarList args;
	StringUtil::SplitString(args, award, ",");
	int counts = args.GetCount();
	LoopBeginCheck(a);
	for (int i = 0; i < counts; i++)
	{
		LoopBeginCheck(a);
		CVarList awardArgs;
		StringUtil::SplitString(awardArgs, args.StringVal(i), ":");
		if (awardArgs.GetCount() != 2)
		{
			continue;
		}
		const char* name = awardArgs.StringVal(0);
		int value = awardArgs.IntVal(1);

		int type = 0;

		if (strcmp(name, "GuildCapital") == 0)
		{
			type = SP_GUILD_ADD_GUILD_NUM;
		}
		 
		if (type == 0){
			continue;
		}
		
		CVarList pubMsg;
		pubMsg << PUBSPACE_GUILD << guildName << type <<GUILD_NUM_CAPITAL<< value;
		pKernel->SendPublicMessage(pubMsg);
	}
}

// ˢ�°��ȼ�
void GuildModule::UpdateGuildLevelToPlayer(IKernel* pKernel, const PERSISTID& self)
{
    //1������������֤
	if( !pKernel->Exists(self) )
	{
		return;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if( pSelfObj == NULL )
	{
		return;
	}

	//2����ȡ�Լ��Ĺ�����Ϣ
	const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
	if ( StringUtil::CharIsNull(guildName) )
	{
		return;
	}

	//3����ȡ�����ռ�
	IPubData* pPubData = GetPubData(pKernel, GUILD_PUB_DATA);
	if (pPubData == NULL)
	{
		return;
	}

	//4����ȡ�����б�
	IRecord* guildListRecord = pPubData->GetRecord(GUILD_LIST_REC);
	if (guildListRecord == NULL)
	{
		return;
	}

	//5������������ڹ���
	int row = guildListRecord->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (row < 0)
	{
		return;
	}

	//6��ˢ�¹���ȼ����������
	int guildLevel = GetGuildLevel(pKernel, guildName);
	pSelfObj->SetInt("GuildLevel", guildLevel);

	return;
}

void GuildModule::RefreshGuildSkill(IKernel*pKernel, const PERSISTID& self)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return;
	}

	// �ҵ��������ܱ�
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return ;
	}

	const wchar_t* guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return;
	}

	IRecord* pSkillRecord = pGuildData->GetRecord(GUILD_PUB_SKILL_REC);
	if (pSkillRecord == NULL)
	{
		return;
	}
	std::list<int> addSkill;
	int rowMax = pSkillRecord->GetRows();
	for (int row = 0; row < rowMax; row++)
	{
		int skillID = pSkillRecord->QueryInt(row, GUILD_PUB_SKILL_REC_COL_SKILLID);
		 // �Ƿ��������������
		 int nRows = pPassiveSkillRec->FindInt(PS_COL_SKILL_ID, skillID);
		 if (nRows < 0)
		 {
			 addSkill .push_back( skillID);

		 }

	}
		 
	if (!addSkill.empty())
	{
		std::list<int>::iterator it = addSkill.begin();
		for (; it != addSkill.end(); it++)
		{
			PassiveSkillSystemSingleton::Instance()->UpdatePassiveSkill(pKernel, self, *it, 1, PSS_BY_GUILD);
		}
	}

	pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PROPERTY_TYPE);
	pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PASSIVE_SKILL_TYPE);
}






const GuildShopConfigItem_t* GuildModule::GetShopItemInfo(int itemIndex)
{
	for (auto& it : m_GuildShopConfig)
	{
		if (it.m_Index == itemIndex)
		{
			if (!StringUtil::CharIsNull(it.m_ItemId.c_str()))
			{
				return &it;
			}
		}

	}
	return NULL;
}

void GuildModule::ReloadConfig(IKernel* pKernel)
{
	m_pGuildModule->LoadResource(pKernel);
}

// ���ȼ��ı�
int GuildModule::OnGuildLevelChange( IKernel *pKernel, const PERSISTID &self, 
									const PERSISTID &sender, const IVarList &args )
{
	//����������֤
	if (!pKernel->Exists(self))
	{
		return 0;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if( pSelfObj == NULL )
	{
		return 0;
	}
	if( args.GetCount() < 2 )
	{
		return 0;
	}

	// ��ʼ���������
	pSelfObj->SetInt("GuildLevel", args.IntVal(1));

	return 0;
}


void GuildModule::CustomSysInfoByGuild(IKernel * pKernel, const wchar_t* guildName, int tips_type, const char * fast_string_id, const IVarList & para_list)
{

		if (StringUtil::CharIsNull(guildName)){
			return ;
		}

		// ��ȡ��Ա���
		int row = 0;
		IRecord* pRecord = GuildModule::m_pGuildModule->GetMemberRecord(pKernel, guildName);
		if (pRecord == NULL){
			return ;
		}

		// ֪ͨÿ������Ա
		LoopBeginCheck(d);
		for (int r = 0; r < pRecord->GetRows(); r++)
		{
			LoopDoCheck(d);
			int onlineState = pRecord->QueryInt(r, GUILD_MEMBER_REC_COL_ONLINE);
			if (onlineState == OFFLINE){
				continue;
			}

			const wchar_t* name = pRecord->QueryWideStr(r, GUILD_MEMBER_REC_COL_NAME);
			if (StringUtil::CharIsNull(name))
			{
				continue;
			}

			pKernel->CustomByName(name, CVarList() << SERVER_CUSTOMMSG_SYSINFO << tips_type << fast_string_id << para_list);
		}

		
		

}

int GuildModule::GetGuildNumValue(IKernel*pKernel, const wchar_t* guildName, GUILD_NUM_TYPE guildNumType)
{

	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}
	IRecord* pGuildNumRecord = pGuildData->GetRecord(GUILD_NUM_REC);
	if (pGuildNumRecord == NULL)
	{
		return 0;
	}

	int row = pGuildNumRecord->FindInt(GUILD_NUM_REC_COL_TYPE, guildNumType);
	if (row < 0){
		return 0;
	}

	return pGuildNumRecord->QueryInt(row,GUILD_NUM_REC_COL_ENABLE_VALUE);

}

int GuildModule::GuildBuildingLevel(IKernel *pKernel,const  wchar_t* guildName, GUILD_BUILDING_TYPE buildType)
{

	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return NULL;
	}
	IRecord* pGuildBuildingRecord = pGuildData->GetRecord(GUILD_BUILDING_REC);
	if (pGuildBuildingRecord == NULL)
	{
		return 0;
	}
	int row = pGuildBuildingRecord->FindInt(GUILD_BUILDING_REC_COL_TYPE, buildType);
	if (row >= 0)
	{
		return pGuildBuildingRecord->QueryInt(row, GUILD_BUILDING_REC_COL_LEVEL);
	}

	return 0;
}

void GuildModule::RewardGuildValue(IKernel *pKernel, const wchar_t*guildName, GUILD_NUM_AWARD type, int addValue, GUILD_NUM_CHANGE res, const wchar_t*playerName)
{
	switch (type)
	{
		case GUILD_AWARD_TYPE_CAPITAL:	// ��֯�ʽ�
		case GUILD_AWARD_TYPE_ANDINGDU: // ������
		case GUILD_AWARD_TYPE_FANRONGDU:	// ���ٶ�
		case GUILD_AWARD_TYPE_XINGDONGLI:	// �ж���
		case GUILD_AWARD_TYPE_JIANKANDU:		// ������
		{
			AddGuildNumValue(pKernel, guildName, static_cast<GUILD_NUM_TYPE>(type), addValue, res, playerName);
		}break;
		case 	GUILD_AWARD_TYPE_BUILDING_EXP:	// ��֯�����
		{
			AddGuildBuildingExp(pKernel,guildName,addValue);
		}break;
	}
}

void GuildModule::AddGuildNumValue(IKernel *pKernel, const wchar_t*guildName, GUILD_NUM_TYPE type, int value, GUILD_NUM_CHANGE res, const wchar_t*playerName /*== NULL*/)
{
	
	CVarList pubMsg;
	pubMsg << PUBSPACE_GUILD
		<< guildName
		<< SP_GUILD_ADD_GUILD_NUM
		<< type
		<< value
		<< res;
	if (!StringUtil::CharIsNull(playerName))
	{
		pubMsg << playerName;
	}
	

	pKernel->SendPublicMessage(pubMsg);
}

void GuildModule::AddGuildSkillExp(IKernel *pKernel, const wchar_t*guildName, int addValue)
{
// 	CVarList pubMsg;
// 	pubMsg << PUBSPACE_GUILD
// 		<< guildName
// 		<< SP_GUILD_SKILL_ADD_STUDY_EXP
// 		<< addValue;
// 	pKernel->SendPublicMessage(pubMsg);
}