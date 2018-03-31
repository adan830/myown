//---------------------------------------------------------
//�ļ���:       ArenaModule.cpp
//��  ��:       ������
//˵  ��:       
//          
//��������:      2015��06��25��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------
#include "ArenaModule.h"
#include "utils/extend_func.h"
#include "utils/util_func.h"
#include "utils/custom_func.h"
#include "public/VarList.h"
#include "utils/XmlFile.h"
#include "utils/string_util.h"

#include "FsGame/Define/ArenaModuleDefine.h"
#include "FsGame/SystemFunctionModule/RankListModule.h"
#include "FsGame/SystemFunctionModule/CapitalModule.h"
#include "FsGame/CommonModule/LevelModule.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/CommonModule/SwitchManagerModule.h"
//#include "FsGame/SystemFunctionModule/CopyOffLineModule.h"
#include "FsGame/CommonModule/LandPosModule.h"
#include "FsGame/SystemFunctionModule/RewardModule.h"
#include "FsGame/SystemFunctionModule/ResetTimerModule.h"
#include "FsGame/CommonModule/SnsDataModule.h"
#include "FsGame/CommonModule/AsynCtrlModule.h"
#include "FsGame/CommonModule/ActionMutex.h"

#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/CapitalDefine.h"
#include "FsGame/Define/LogDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/FightNpcDefine.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/Define/PlayerBaseDefine.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"

#include <sstream>
#include <algorithm>
#include "../Define/CoolDownDefine.h"
#include "../SystemFunctionModule/CoolDownModule.h"
#include "CommonModule/ReLoadConfigModule.h"
#include "Define/SnsDefine.h"
#include "Define/RankingDefine.h"
#include "utils/exptree.h"
#include "system/WinFileSys.h"

// ��ѡ�������С����
#define ARENA_CANDIDATE_MAX_SIZE   500

// ��ѡ��ս�������С����
#define ARENA_CHALLENGEE_MAX_SIZE  100

#define ARENA_CAMP_DEFAULT      0 // û����Ӫ
#define ARENA_CAMP_CHALLENGER   1 // ��ս����Ӫ
#define ARENA_CAMP_CHALLENGEE   2 // ����ս����Ӫ

#define ARENA_NO_RANK           -1 // ���û���ϰ�

#define TEXT_ARENA_RANK_COL_UID     "player_uid"
#define TEXT_ARENA_RANK_COL_SN   "player_arena_sn"
#define TEXT_ARENA_RANK_COL_NATION  "player_nation"

// �Ƿ��һ����ս������
#define TEXT_ARENA_TIMES                  "ArenaTimes"
// �������Ļ����ˢ��ʱ��
#define TEXT_ARENA_CHANCE_RESET_DATE      "ArenaChanceResetDate"
// �������Ļ�����
#define TEXT_ARENA_CHALLENGE_CHANCE       "ArenaChance"
// �Ѿ�ʹ�õĻ�����
#define TEXT_ARENA_CHALLENGE_CHANCE_USED  "ArenaChanceUsed"
// �������Ķ�����Ϣ
#define TEXT_ARENA_CHALLENGEE_INFO  "ArenaChallengeeInfo"
// �������Ķ���
#define TEXT_ARENA_CHALLENGEE_OBJ   "ArenaChallengeeObj"
// �������ı�ʶ
#define TEXT_ARENA_FLAG             "ArenaFlag"
// �½����ľ��������а�
//#define TEXT_ARENA_NEW_RANK_LIST    "ArenaNewRankList"
// ��ҵ�ǰ�ľ��������а�
#define TEXT_ARENA_CUR_RANK_LIST    "ArenaCurvRankList"
// ���ֵ�����
#define TEXT_ARENA_CHALLENGEE_NAME  "ArenaChallengeeName"
// ��ѡ���б�
#define TEXT_ARENA_CANDIDATE_LIST   "ArenaCandidateList"
// ��ս��ʼʱ��
#define TEXT_ARENA_CHALLENGE_START_TIME "ChallengeStartTime"
#define TEXT_ARENA_CHALLENGE_END_TIME "ChallengeEndTime"
//��ʷ�������
#define TEXT_HIGHEST_ARENA_RANK         "HighestArenaRank"
// ������������
#define TEXT_RANK_UP_REWARD             "RankUpReward"
// ���������α��
#define TEXT_ARENA_SN					"ArenaSN"

// �����ļ�
#define ARENA_CONFIG_FILE           "ini/Campaign/Arena/ArenaConfig.xml"
#define ARENA_CANDIDATE_RULE_FILE   "ini/Campaign/Arena/CandidateRule.xml"
#define ARENA_CHALLENGE_REWARD_FILE "ini/Campaign/Arena/ChallengeReward.xml"
#define ARENA_RANK_RWARD_FILE       "ini/Campaign/Arena/RankReward.xml.xml"
#define ARENA_RANK_LIST_FILE        "ini/Campaign/Arena/RankList.xml"
#define ARENA_PROMOTION_REWARD_FILE "ini/Campaign/Arena/PromotionReward.xml"
#define ARENA_RANK_UP_REWARD_FILE   "ini/Campaign/Arena/RankUpReward.xml"

ArenaModule* ArenaModule::m_pArenaModule = NULL;
RankListModule * ArenaModule::m_pRankListModule = NULL;
CapitalModule * ArenaModule::m_pCapitalModule = NULL;
LevelModule * ArenaModule::m_pLevelModuel = NULL;
CopyOffLineModule * ArenaModule::m_pCopyOfflineModule = NULL;
LandPosModule * ArenaModule::m_pLandPosModule = NULL;
RewardModule * ArenaModule::m_pRewardModule = NULL;
VipModule * ArenaModule::m_pVipModule = NULL;
DynamicActModule * ArenaModule::m_pDynamicActModule = NULL;
LogModule * ArenaModule::m_pLogModule = NULL;

ArenaModule::ArenaConfig ArenaModule::m_ArenaConfig;
std::vector<ArenaModule::CandidateRule> ArenaModule::m_CandidateRule;      // ��������ѡ�˹���
std::vector<ArenaModule::ChallengeReward> ArenaModule::m_ChanllengeReward; // ��������ս����
ArenaModule::RankRewardMap ArenaModule::m_RankReward;                      // ��������������
std::vector<ArenaModule::ArenaRankListConfig> ArenaModule::m_ArenaRankList;      // �����������а�
ArenaModule::PromotionRewardMap ArenaModule::m_PromotionReward;             // ��������
ArenaModule::RankUpRewardMap ArenaModule::m_RankUpReward;               // ������������

std::wstring ArenaModule::m_domainName = L"";

int nx_reload_arena(void* state)
{
    IKernel* pKernel = LuaExtModule::GetKernel(state);

    if (NULL == ArenaModule::m_pArenaModule)
    {
        return 0;
    }

    ArenaModule::m_pArenaModule->LoadResource(pKernel);

    return 1;
}

int nx_challenge(void* state)
{
    IKernel* pKernel = LuaExtModule::GetKernel(state);

    if (NULL == ArenaModule::m_pArenaModule)
    {
        return 0;
    }

    // ����������
    CHECK_ARG_NUM(state, nx_challenge, 1);

    // ����������
    CHECK_ARG_OBJECT(state, nx_challenge, 1);

    // ��ȡ����
    PERSISTID player = pKernel->LuaToObject(state, 1);

	if (!pKernel->FindData(player, TEXT_ARENA_CANDIDATE_LIST))
	{
		return 0;
	}

	std::wstring wstr_name_list = pKernel->QueryDataWideStr(player, TEXT_ARENA_CANDIDATE_LIST);

	CVarList temp_name_list;
	util_split_wstring(temp_name_list, wstr_name_list, L"$");
    
    // ȡ��һ���˽�����ս
	const wchar_t *challengee_name = temp_name_list.WideStrVal(0);
    
	ArenaModule::m_pArenaModule->OnChallengeSomeone(pKernel, player, PERSISTID(), CVarList() << CLIENT_CUSTOMMSG_ARENA_MSG << ARENA_C2S_CHALLENGE_SOMEONE << challengee_name);

    return 1;
}

// ��ȡ��ս��ѡ��
int nx_get_candidate(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == ArenaModule::m_pArenaModule)
	{
		return 0;
	}

	// ����������
	CHECK_ARG_NUM(state, nx_get_candidate, 1);

	// ����������
	CHECK_ARG_OBJECT(state, nx_get_candidate, 1);

	// ��ȡ����
	PERSISTID player = pKernel->LuaToObject(state, 1);

	// ��ȡ����ս�ĺ�ѡ��
	ArenaModule::m_pArenaModule->OnGetChallengeCandidate(pKernel, player, PERSISTID(), CVarList());

	return 1;
}

int nx_arena_reward(void* state)
{
    IKernel* pKernel = LuaExtModule::GetKernel(state);

    if (NULL == ArenaModule::m_pArenaModule)
    {
        return 0;
    }
    
    ArenaModule::m_pArenaModule->RewardByArenaRank(pKernel);

    return 1;
}

// ������Ҿ�����״̬
int nx_reset_arena_state(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == ArenaModule::m_pArenaModule)
	{
		return 0;
	}

	// ����������
	CHECK_ARG_NUM(state, nx_reset_arena_state, 1);

	// ����������
	CHECK_ARG_WIDESTR(state, nx_reset_arena_state, 1);

	// ��ȡ����
	const wchar_t* strName = pKernel->LuaToWideStr(state, 1);

	ArenaModule::m_pArenaModule->ResetArenaStateByGm(pKernel, strName, UNLOCK_ARENA_STATE);

	return 1;
}

// ������Ҿ�����״̬
int nx_lock_arena_state(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == ArenaModule::m_pArenaModule)
	{
		return 0;
	}

	// ����������
	CHECK_ARG_NUM(state, nx_lock_arena_state, 1);

	// ����������
	CHECK_ARG_WIDESTR(state, nx_lock_arena_state, 1);

	// ��ȡ����
	const wchar_t* strName = pKernel->LuaToWideStr(state, 1);

	ArenaModule::m_pArenaModule->ResetArenaStateByGm(pKernel, strName, LOCK_ARENA_STATE);

	return 1;
}

//���������ظ�����
int nx_clear_arena_repeatinfo(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == ArenaModule::m_pArenaModule)
	{
		return 0;
	}

	ArenaModule::m_pArenaModule->PlayerRanklistClean(pKernel);

	return 1;
}

int nx_import_arena_list(void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ����������
	CHECK_ARG_NUM(state, nx_import_arena_list, 2);

	// ����������
	CHECK_ARG_INT(state, nx_import_arena_list, 1);
	CHECK_ARG_INT(state, nx_import_arena_list, 2);

	int nStartRow = pKernel->LuaToInt(state, 1);
	int nEndRow = pKernel->LuaToInt(state, 2);

	return 1;
}

// ��ʼ������
bool ArenaModule::Init(IKernel* pKernel)
{
	m_pArenaModule = this;	
    m_pRankListModule = (RankListModule *)pKernel->GetLogicModule("RankListModule");
    m_pCapitalModule = (CapitalModule *)pKernel->GetLogicModule("CapitalModule");
    m_pLevelModuel = (LevelModule *)pKernel->GetLogicModule("LevelModule");
    m_pCopyOfflineModule = (CopyOffLineModule *)pKernel->GetLogicModule("CopyOffLineModule");
    m_pLandPosModule = (LandPosModule *)pKernel->GetLogicModule("LandPosModule");
    m_pRewardModule = (RewardModule *)pKernel->GetLogicModule("RewardModule");
    m_pVipModule = (VipModule *)pKernel->GetLogicModule("VipModule");
	m_pDynamicActModule = (DynamicActModule *)pKernel->GetLogicModule("DynamicActModule");

    Assert(m_pRankListModule && m_pCapitalModule && m_pLevelModuel && m_pCopyOfflineModule
           && m_pLandPosModule && m_pRewardModule && m_pVipModule && m_pDynamicActModule);
	
	pKernel->AddEventCallback("player", "OnRecover", ArenaModule::OnRecover);
    pKernel->AddEventCallback("player", "OnAfterEntryScene", ArenaModule::OnAfterEntryScene);
    pKernel->AddEventCallback("player", "OnReady", ArenaModule::OnReady);
    pKernel->AddEventCallback("player", "OnLeaveScene", ArenaModule::OnLeaveScene);
	pKernel->AddEventCallback("player", "OnContinue", ArenaModule::OnPlayerContinue);
    // ���Թ���������Ϣ
    pKernel->AddEventCallback("scene", "OnPublicMessage", ArenaModule::OnPublicMessage);

	// ����SNS��������Ϣ
	pKernel->AddEventCallback("scene", "OnSnsMessage", ArenaModule::OnSnsMessage);

	// ���Թ���������Ϣ
	pKernel->AddEventCallback("scene", "OnCreate", ArenaModule::OnCreateScene);
    
	// �ͻ�����Ϣ�ص�
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_ARENA_MSG, ArenaModule::OnCustomMessage);
	
    // �������
    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_BUY_INFO, ArenaModule::OnBuyArenaChance);
	
    pKernel->AddIntCommandHook("player", COMMAND_ARENA_PLAYER, ArenaModule::OnPlayerCommand);

    // ���OnContinue ��� Ready
    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_CONTINUE_ON_READY, ArenaModule::OnContinueReady);
    
    pKernel->AddIntCommandHook("player", COMMAND_KILLTARGET, ArenaModule::OnKillObject);
    pKernel->AddIntCommandHook("player", COMMAND_BEKILL, ArenaModule::OnBeKilled);

    // ��ҵȼ��仯 
    pKernel->AddIntCommandHook("player", COMMAND_LEVELUP_CHANGE, OnPlayerLevel);

	LoadResource(pKernel);
	
	DECL_LUA_EXT(nx_reload_arena);
    DECL_LUA_EXT(nx_challenge);
    DECL_LUA_EXT(nx_arena_reward);
	DECL_LUA_EXT(nx_reset_arena_state);
	DECL_LUA_EXT(nx_lock_arena_state);
	DECL_LUA_EXT(nx_clear_arena_repeatinfo);
	DECL_LUA_EXT(nx_get_candidate);
	DECL_LUA_EXT(nx_import_arena_list);

	//���VIP�ȼ��仯�ص�
	DECL_CRITICAL(ArenaModule::C_OnVIPLevelChanged);
	DECL_RESET_TIMER(RESET_ARENA_CHANCE_TIMES, ArenaModule::ResetArenaChanceTimer);
    DECL_HEARTBEAT(ArenaModule::HB_BeforeFight);
    DECL_HEARTBEAT(ArenaModule::HB_RoundCheck);
    DECL_HEARTBEAT(ArenaModule::HB_CloseWait);
    DECL_HEARTBEAT(ArenaModule::HB_LeaveScene);
    
	RELOAD_CONFIG_REG("ArenaConfig", ArenaModule::ReloadConfig);
	return true;
}

// ��Դ�ͷź���
bool ArenaModule::Shut(IKernel* pKernel)
{
	return true;
}

// ������Դ
bool ArenaModule::LoadResource(IKernel* pKernel)
{
    LoadArenaConfig(pKernel, ARENA_CONFIG_FILE);
    
    LoadCandidateConfig(pKernel, ARENA_CANDIDATE_RULE_FILE);
    
    LoadChallengeRewardConfig(pKernel, ARENA_CHALLENGE_REWARD_FILE);
    
    LoadRankRewardConfig(pKernel, ARENA_RANK_RWARD_FILE);
    
    LoadRankListConfig(pKernel, ARENA_RANK_LIST_FILE);
    
    LoadPromotionReward(pKernel, ARENA_PROMOTION_REWARD_FILE);
    
    LoadRankUpReward(pKernel, ARENA_RANK_UP_REWARD_FILE);
    
	return true;
}

// ����������
bool ArenaModule::LoadArenaConfig(IKernel* pKernel, const char *file_name)
{
    // �����ļ��ĸ�ʽ��
    //
    //  <Property   ID="" ... />
    //
    std::string the_config_file = pKernel->GetResourcePath();
    the_config_file.append(file_name);

    CXmlFile xml(the_config_file.c_str());
    if (!xml.LoadFromFile()) 
    {
        std::string err_msg = the_config_file;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    if (sec_count == 0)
    {
        return false;
    }
    
    // ���ζ�ȡ������Ϣ
    const char *sec = sec_list.StringVal(0);

    m_ArenaConfig.open_level        = xml.ReadInteger(sec, "OpenLevel", 0);
    m_ArenaConfig.buy_chance_limit      = xml.ReadInteger(sec, "BuyChanceLimit", 0);
    m_ArenaConfig.chosen_count      = xml.ReadInteger(sec, "ChosenCount", 0);
    m_ArenaConfig.free_chance       = xml.ReadInteger(sec, "FreeChance", 0);
	m_ArenaConfig.challenge_cd		= xml.ReadInteger(sec, "ChallengeCD", 0) * 1000;
	m_ArenaConfig.challenge_clear_cd_fee = xml.ReadInteger(sec, "ClearCDFee", 10);
    std::string chance_fee          = xml.ReadString(sec, "ChanceFee", "");
    m_ArenaConfig.scene_id          = xml.ReadInteger(sec, "SceneID", 701);
    m_ArenaConfig.npc_config        = xml.ReadString(sec, "NpcConfig", "Monster_50002");
    std::string challenger_born     = xml.ReadString(sec, "ChallengerBorn", "");
    std::string challengee_born     = xml.ReadString(sec, "ChallengeeBorn", "");
    m_ArenaConfig.count_down        = xml.ReadInteger(sec, "Countdown", 0);
    m_ArenaConfig.round_time        = xml.ReadInteger(sec, "RoundTime", 0);
    m_ArenaConfig.wait_before_close = xml.ReadInteger(sec, "WaitBeforeClose", 0);
    m_ArenaConfig.rank_reward_name  = xml.ReadString(sec, "RankReward", "");
    m_ArenaConfig.challenger_win    = xml.ReadString(sec, "ChallengerWin", "");
    m_ArenaConfig.challenger_failed = xml.ReadString(sec, "ChallengerFailed", "");
    m_ArenaConfig.challengee_win    = xml.ReadString(sec, "ChallengeeWin", "");
    m_ArenaConfig.challengee_failed = xml.ReadString(sec, "ChallengeeFailed", "");
	m_ArenaConfig.nBroadRank		= xml.ReadInteger(sec, "BroadRank", 5);

    // @chance_fee �ĸ�ʽ: fee,fee,...fee
    bool parse_success = ParseFee(chance_fee, m_ArenaConfig.chance_fee, sizeof(m_ArenaConfig.chance_fee)/sizeof(int));
    if (!parse_success)
    {
        extend_warning(LOG_ERROR, "%s [ChanceFee] error", file_name);
    }
    
    // ��������, ����ĸ�ʽ�� x,y,z,o
    parse_success = ParseCoordinate(challenger_born, m_ArenaConfig.challenger_born);
    if (!parse_success)
    {
        extend_warning(LOG_ERROR, "%s [ChallengerBorn] error", file_name);
    }

    parse_success = ParseCoordinate(challengee_born, m_ArenaConfig.challengee_born);
    if (!parse_success)
    {
        extend_warning(LOG_ERROR, "%s [ChallengeeBorn] error", file_name);
    }   

    return true;
}

// ��ѡ�˹���
bool ArenaModule::LoadCandidateConfig(IKernel* pKernel, const char *file_name)
{
    // �����ļ��ĸ�ʽ��
    //
    //  <Property   ID="" ... />
    //
    
    m_CandidateRule.clear();
    
    std::string the_config_file = pKernel->GetResourcePath();
    the_config_file.append(file_name);

    CXmlFile xml(the_config_file.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = the_config_file;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    int sec_count = (int)sec_list.GetCount();

	LoopBeginCheck(a)
    for (int i=0; i<sec_count; ++i)
    {
		LoopDoCheck(a)
        // ���ζ�ȡ������Ϣ
        const char *sec = sec_list.StringVal(i);

        CandidateRule the_rule;
        the_rule.lower_rank      = xml.ReadInteger(sec, "LowerRank", 0);
        the_rule.upper_rank      = xml.ReadInteger(sec, "UpperRank", 0);
        the_rule.candidate_count = xml.ReadInteger(sec, "CandidateCount", 0);
        
        m_CandidateRule.push_back(the_rule);
    }

    return true;
}

// ��ս��������
bool ArenaModule::LoadChallengeRewardConfig(IKernel* pKernel, const char *file_name)
{
    // �����ļ��ĸ�ʽ��
    //
    //  <Property   ID="" ... />
    //
    
    m_ChanllengeReward.clear();
    
    std::string the_config_file = pKernel->GetResourcePath();
    the_config_file.append(file_name);

    CXmlFile xml(the_config_file.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = the_config_file;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    int sec_count = (int)sec_list.GetCount();

    LoopBeginCheck(b)
    for (int i=0; i<sec_count; ++i)
    {
        LoopDoCheck(b)
        // ���ζ�ȡ������Ϣ
        const char *sec = sec_list.StringVal(i);

        ChallengeReward the_reward;
        the_reward.lower_level      = xml.ReadInteger(sec, "LowerLevel", 0);
        the_reward.upper_level      = xml.ReadInteger(sec, "UpperLevel", 0);
		the_reward.winner_award.str_exp = xml.ReadString(sec, "WinnerExp", "");
        the_reward.winner_award.str_capital = xml.ReadString(sec, "WinnerCapital", "");
        the_reward.winner_award.str_item    = xml.ReadString(sec, "WinnerItem", "");
		the_reward.loser_award.str_exp = xml.ReadString(sec, "LoserExp", "");
        the_reward.loser_award.str_capital = xml.ReadString(sec, "LoserCapital", "");
        the_reward.loser_award.str_item    = xml.ReadString(sec, "LoserItem", "");

        m_ChanllengeReward.push_back(the_reward);
    }
    
    return true;
}

// ������������
bool ArenaModule::LoadRankRewardConfig(IKernel* pKernel, const char *file_name)
{
    // �����ļ��ĸ�ʽ��
    //
    //  <Property   ID="" ... />
    //
    
    m_RankReward.clear();
    
    std::string the_config_file = pKernel->GetResourcePath();
    the_config_file.append(file_name);

    CXmlFile xml(the_config_file.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = the_config_file;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    int sec_count = (int)sec_list.GetCount();

    LoopBeginCheck(c)
    for (int i=0; i<sec_count; ++i)
    {
        LoopDoCheck(c)
        // ���ζ�ȡ������Ϣ
        const char *sec = sec_list.StringVal(i);

        RankReward the_reward;
        the_reward.rank_list_name    = xml.ReadString(sec, "RankList", "");
        the_reward.lower_rank        = xml.ReadInteger(sec, "LowerRank", 0);
        the_reward.upper_rank        = xml.ReadInteger(sec, "UpperRank", 0);
        //the_reward.award.exp         = xml.ReadInteger(sec, "Exp", 0);
        the_reward.award.str_capital = xml.ReadString(sec, "Capital", "");
        the_reward.award.str_item    = xml.ReadString(sec, "Item", "");

        RankRewardMap::iterator it_find = m_RankReward.find(the_reward.rank_list_name);
        if (it_find == m_RankReward.end())
        {
            std::vector<RankReward> the_reward_list;
            the_reward_list.push_back(the_reward);
            
            m_RankReward.insert(std::make_pair(the_reward.rank_list_name, the_reward_list));
        }
        else
        {
            std::vector<RankReward> &the_reward_list = it_find->second;
            the_reward_list.push_back(the_reward);
        }
    }

    return true;
}


// �������а�����
bool ArenaModule::LoadRankListConfig(IKernel* pKernel, const char *file_name)
{
    // �����ļ��ĸ�ʽ��
    //
    //  <Property   ID="" ... />
    //
    
    m_ArenaRankList.clear();
    
    std::string the_config_file = pKernel->GetResourcePath();
    the_config_file.append(file_name);

    CXmlFile xml(the_config_file.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = the_config_file;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    int sec_count = (int)sec_list.GetCount();

    LoopBeginCheck(d)
    for (int i=0; i<sec_count; ++i)
    {
        LoopDoCheck(d)
        // ���ζ�ȡ������Ϣ
        const char *sec = sec_list.StringVal(i);

        ArenaRankListConfig the_def;
        the_def.lower_level       = xml.ReadInteger(sec, "LowerLevel", 0);
        the_def.upper_level       = xml.ReadInteger(sec, "UpperLevel", 0);
        the_def.name              = xml.ReadString(sec, "Name", "");
        the_def.level             = xml.ReadInteger(sec, "Level", 0);
        the_def.need_notify       = xml.ReadInteger(sec, "NeedNotify", 0);

        m_ArenaRankList.push_back(the_def);
    }

    return true;
}

// ���������������
bool ArenaModule::LoadPromotionReward(IKernel* pKernel, const char *file_name)
{
    // �����ļ��ĸ�ʽ��
    //
    //  <Property   ID="" ... />
    //

    m_PromotionReward.clear();

    std::string the_config_file = pKernel->GetResourcePath();
    the_config_file.append(file_name);

    CXmlFile xml(the_config_file.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = the_config_file;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    int sec_count = (int)sec_list.GetCount();

    LoopBeginCheck(e)
    for (int i=0; i<sec_count; ++i)
    {
        LoopDoCheck(e)
        
        // ���ζ�ȡ������Ϣ
        const char *sec = sec_list.StringVal(i);

        PromotionReward the_reward;
        the_reward.rank_list_name    = xml.ReadString(sec, "RankListName", "");
        the_reward.reward_name       = xml.ReadString(sec, "RewardName", "");
        the_reward.capital           = xml.ReadString(sec, "Capital", "");
        the_reward.item              = xml.ReadString(sec, "Item", "");

        m_PromotionReward.insert(std::make_pair(the_reward.rank_list_name, the_reward));
    }

    return true;
}


// ������������
bool ArenaModule::LoadRankUpReward(IKernel* pKernel, const char *file_name)
{
    // �����ļ��ĸ�ʽ��
    //
    //  <Property   ID="" ... />
    //

    m_RankUpReward.clear();

    std::string the_config_file = pKernel->GetResourcePath();
    the_config_file.append(file_name);

    CXmlFile xml(the_config_file.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = the_config_file;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    int sec_count = (int)sec_list.GetCount();

    LoopBeginCheck(e)
    for (int i=0; i<sec_count; ++i)
    {
        LoopDoCheck(e)

        // ���ζ�ȡ������Ϣ
        const char *sec = sec_list.StringVal(i);

        RankUpReward the_reward;
        the_reward.rank_list_name   = xml.ReadString(sec, "RankListName", "");
        the_reward.lower_rank       = xml.ReadInteger(sec, "LowerRank", 0);
        the_reward.upper_rank       = xml.ReadInteger(sec, "UpperRank", 0);
		the_reward.award.str_capital = xml.ReadString(sec, "CapitalReward", "");
		the_reward.award.str_item = xml.ReadString(sec, "ItemReward", "");

        RankUpRewardMap::iterator it = m_RankUpReward.find(the_reward.rank_list_name);
        
        if (it == m_RankUpReward.end())
        {
            std::vector<RankUpReward> reward_list;
            reward_list.push_back(the_reward);
            
            m_RankUpReward.insert(std::make_pair(the_reward.rank_list_name, reward_list));
        }
        else
        {
            std::vector<RankUpReward> &reward_list = it->second;
            reward_list.push_back(the_reward);
        }
    }

    return true;
}

// ��ȡdomain������
const std::wstring& ArenaModule::GetDomainName(IKernel * pKernel)
{
    if (m_domainName.empty())
    {
        wchar_t wstr[256];
        const int server_id = pKernel->GetServerId();
        SWPRINTF_S(wstr, L"Domain_Arena_%d", server_id);

        m_domainName = wstr;
    }

    return m_domainName;
}

// ��ȡ��ҵ�����, ����-1��ʾû�������а�
bool ArenaModule::GetArenaRank(IKernel* pKernel, const PERSISTID& player, CVarList &result)
{
    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return false;
    }
    
    int role_level = player_obj->QueryInt("Level");
    
    const ArenaRankListConfig *the_config = NULL;
    std::vector<ArenaRankListConfig>::const_iterator it = m_ArenaRankList.begin();
    std::vector<ArenaRankListConfig>::const_iterator it_end = m_ArenaRankList.end();

    LoopBeginCheck(f)
    for (; it!=it_end; ++it)
	{
	    LoopDoCheck(f)
        const ArenaRankListConfig &temp_config = *it;

        if (role_level >= temp_config.lower_level && role_level <= temp_config.upper_level)
        {
            the_config = &temp_config;
            break;
        }
    }
    
    if (the_config == NULL)
    {
        return false;
    }

    int rank = -1;  
    IRecord *rank_rec = GetArenaRankListRecord(pKernel, the_config->name.c_str());
    if (rank_rec != NULL)
    {
        const char *role_uid = player_obj->QueryString("Uid");
        int row = rank_rec->FindString(ARENA_RANK_COL_UID, role_uid);

        if (row == -1)
        {
            rank = -1;
        }
        else
        {
            rank = row + 1; // @row ��0��ʼ��@rank_no ��1��ʼ 
        }
    }
    
    result << rank << the_config->lower_level << the_config->upper_level;
    
    return true;
}


// GM ����������, ��������GM
void ArenaModule::GMReset(IKernel* pKernel, const PERSISTID& player)
{
    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return;
    }
    
    player_obj->SetInt(TEXT_ARENA_CHALLENGE_CHANCE, m_ArenaConfig.free_chance);
    player_obj->SetInt(TEXT_ARENA_CHALLENGE_CHANCE_USED, 0);
}

int ArenaModule::OnRecover(IKernel* pKernel, const PERSISTID& self,
                           const PERSISTID& sender, const IVarList& args)
{
    //pKernel->AddCritical(self, "Level", "ArenaModule::OnPlayerLevel");
    
   REGIST_RESET_TIMER(pKernel, self, RESET_ARENA_CHANCE_TIMES);
    
    int player_level = pKernel->QueryInt(self, "Level");
    
    if (player_level < m_ArenaConfig.open_level)
    {
        return 0;    
    }
    
    ResetArenaChance(pKernel, self);

// 	IGameObj* pPlayer = pKernel->GetGameObj(self);
// 	if (NULL == pPlayer)
// 	{
// 		return 0;
// 	}
// 
// 	// ���ڰ�������������ԣ�����Ϸ������ݱ�����  ���кϷ�����ʱ,����OnReady bFirst�Ĵ�����
// 	CVarList arena_rank;
// 	GetArenaRank(pKernel, self, arena_rank);
// 	int find_rank = arena_rank.IntVal(0); 
// 	if (find_rank == -1)
// 	{		
// 		int nLevel = pPlayer->QueryInt("Level");
// 		// �Ϸ���,���������а����ݱ����,����п��ܲ������а��У����԰���Ҵ������а�
// 		const char * rank_name = GetRankListName(pKernel, nLevel);
// 		if (StringUtil::CharIsNull(rank_name))
// 		{
// 			return 0;
// 		}
// 		const wchar_t* player_name = pPlayer->QueryWideStr("Name");
// 		const char* player_uid = pPlayer->QueryString("Uid");
// 		CVarList rank_list_row;
// 		if (!PrepareDataForArenaRankList(pKernel, self, rank_name, rank_list_row))
// 		{
// 			return 0;
// 		}
// 		CVarList the_msg;
// 		the_msg << ARENA_PUB_SUB_MSG_ADD_TO_RANKLIST;
// 		the_msg << player_name << player_uid << rank_name << rank_list_row;
// 
// 		SendArenaMsgToPublic(pKernel, the_msg);
// 	}

	// VIPLevel�ص�
	if (!pKernel->FindCritical(self, "VipLevel", "ArenaModule::C_OnVIPLevelChanged"))
	{
		pKernel->AddCritical(self, "VipLevel", "ArenaModule::C_OnVIPLevelChanged");
	}

	return 0;
}

//���볡��
int ArenaModule::OnAfterEntryScene(IKernel * pKernel, const PERSISTID & self,
                             const PERSISTID & sender, const IVarList & args)
{
#if defined(CREATE_ROBOT) 
	OnCreateRobot(pKernel, self, sender, args);
#endif
    // ֻ���ľ���������
    if (!IsArenaScene(pKernel))
    {
        return 0;
    }

    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }

	ActionMutexModule::NoteAction(pKernel, self, ACTION_ARENA);
    
    // ȷ������ս�߼�
    int arena_flag = player_obj->QueryInt(TEXT_ARENA_FLAG);
    if (arena_flag != ARENA_FLAG_ENTERING)
    {
        return 0;
    }
    
    // ��¼�����ʹ����һ�ξ���������
    int chance_used = player_obj->QueryInt(TEXT_ARENA_CHALLENGE_CHANCE_USED);
    ++chance_used;
    player_obj->SetInt(TEXT_ARENA_CHALLENGE_CHANCE_USED, chance_used);

    // ������Ϣ
    std::string challengee_info_str = player_obj->QueryString(TEXT_ARENA_CHALLENGEE_INFO);
    
    CVarList challengee_info;
    util_split_string(challengee_info, challengee_info_str, ",");
    
    const char *challengee_uid = challengee_info.StringVal(0);
    int challengee_nation = atoi(challengee_info.StringVal(1));
    
    // ���鳡������
    int scene_group_id = pKernel->NewGroupId();
    player_obj->SetInt("GroupID" , scene_group_id);
    
    // ����ս����Ӫ
    player_obj->SetInt( "Camp", ARENA_CAMP_CHALLENGER);
    
    // �������
    FreezePlayer(pKernel, self);    
    
    // ��������NPC, ����������ID��������Դ(���ĸ���ҿ�������)
    CVarList npc_args;
    npc_args << CREATE_TYPE_PROPERTY_VALUE << "GroupID" << scene_group_id
             << "CopyFrom" << challengee_uid << "Nation" << challengee_nation
             << "Camp" << ARENA_CAMP_CHALLENGEE;
    
    len_t y = pKernel->GetMapHeight(m_ArenaConfig.challenger_born.x, 
        m_ArenaConfig.challenger_born.z);
        
    PERSISTID challengee_npc = pKernel->CreateObjectArgs("", m_ArenaConfig.npc_config.c_str(), 0,
        m_ArenaConfig.challengee_born.x, y,
        m_ArenaConfig.challengee_born.z,
        m_ArenaConfig.challengee_born.o, npc_args);
        
    if (pKernel->Exists(challengee_npc))
    {
        IGameObj *challengee_obj = pKernel->GetGameObj(challengee_npc);
        if (challengee_obj != NULL)
        {
            // ���ñ���ս�ߵ����Բ����ᱻ��ս���
            challengee_obj->SetInt("PurposeType", ARENA_BATTLE_NPC);
            challengee_obj->SetInt("Level", player_obj->QueryInt("Level")); // ����NPC�ĳ�ʼ�ȼ�Ϊ��ҵȼ�
            challengee_obj->SetInt("Nation", challengee_nation);
        }
        
        FreezePlayer(pKernel, challengee_npc);
    }
    
    // ������ִ���ʧ��, ������һ���յĶ���Ȼ�����̽���������ս��
    
    // �趨׼������
    player_obj->SetInt(TEXT_ARENA_FLAG, ARENA_FLAG_READY);
    
    // ��¼��ս������
    player_obj->SetObject(TEXT_ARENA_CHALLENGEE_OBJ, challengee_npc);
    
    // �����HP�ָ���100%
    player_obj->SetInt64("HP", pKernel->QueryInt64(self, "MaxHP"));
        
    return 0;
}

 // ����
int ArenaModule::OnReady(IKernel * pKernel, const PERSISTID & self,
                         const PERSISTID & sender, const IVarList & args)
{
	bool firstentry = args.BoolVal(0);
	if (firstentry) // �״ν���,��ѯ���ߵľ�����ս��
	{
		OnPlayerContinue(pKernel, self, sender, args);
	}
    // ֻ���ľ���������
    if (!IsArenaScene(pKernel))
    {
        return 0;
    }

    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }

    // ȷ������ս�߼�
    int arena_flag = player_obj->QueryInt(TEXT_ARENA_FLAG);
    if (arena_flag != ARENA_FLAG_READY)
    {
        // ������ͳ�������
        GoToCity(pKernel, self);
        
        return 0;
    }
    
    // �趨Ϊս����
    player_obj->SetInt(TEXT_ARENA_FLAG, ARENA_FLAG_IN_BATTLE);
    player_obj->SetInt("Camp", ARENA_CAMP_CHALLENGER);

    // ��ʼ����ʱ
    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_BeforeFight");
    pKernel->AddCountBeat(self, "ArenaModule::HB_BeforeFight", m_ArenaConfig.count_down * 1000, 1);

    // ֪ͨ�ͻ��˵���ʱ��ʼ
    CVarList msg_for_client;
    msg_for_client << SERVER_CUSTOMMSG_ARENA_MSG;
	msg_for_client << ARENA_S2C_COUNTDOWN_BEFORE_FIGHT;
    msg_for_client << m_ArenaConfig.count_down; // ����ʱ

    pKernel->Custom(self, msg_for_client);
    
    return 0;
}

int ArenaModule::OnContinueReady(IKernel * pKernel, const PERSISTID & self,
                           const PERSISTID & sender, const IVarList & args)
{
    // ֻ���ľ���������
    if (!IsArenaScene(pKernel))
    {
        return 0;
    }

    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }

    int arena_flag = player_obj->QueryInt(TEXT_ARENA_FLAG);
//     if (arena_flag == ARENA_FLAG_NOT_ARENA)
//     {
//         // ����ս״̬, ������ͳ�������
//         GoToCity(pKernel, self);
//         return 0;
//     }

    if (arena_flag != ARENA_FLAG_END)
    {
        // ��ʱ��û�зֳ�ʤ��, ǿ�ƽ�����ս
        player_obj->SetInt(TEXT_ARENA_FLAG, ARENA_FLAG_END);
        
        // �Ƴ�ȫ������
        DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_BeforeFight");
        DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_RoundCheck");
        DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_CloseWait");
        
        // ����ǰ�뿪������, �ж�Ϊ��������
        OnChallengeEnd(pKernel, self, ARENA_RET_CHALLENGE_LOSE, ARENA_RET_ESCAPE);

        // ������ ����Ҵ���������
        WaitThenLeaveArena(pKernel, self);
    }

    return 0;
}

// ����뿪����
int ArenaModule::OnLeaveScene(IKernel *pKernel, const PERSISTID &self,
                        const PERSISTID &sender, const IVarList &args)
{ 
    // ֻ���ľ���������
    if (!IsArenaScene(pKernel))
    {
        return 0;
    }
  
    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }

	ActionMutexModule::CleanAction(pKernel, self);

    // ������״̬
    int arena_flag = player_obj->QueryInt(TEXT_ARENA_FLAG);
    
    // �Ƴ�ȫ����ʱ����
    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_BeforeFight");
    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_RoundCheck");
    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_CloseWait");
    
    if (arena_flag != ARENA_FLAG_NOT_ARENA && arena_flag != ARENA_FLAG_END)
    {
        // ����ǰ�뿪������, �ж�Ϊ��������
        player_obj->SetInt(TEXT_ARENA_FLAG, ARENA_FLAG_END);
        OnChallengeEnd(pKernel, self, ARENA_RET_CHALLENGE_LOSE, ARENA_RET_ESCAPE);
    }
    
    int scene_group_id = player_obj->QueryInt("GroupID");
    
    // ������ս˫��
    UnlockPlayer(pKernel, self);
    
    // ���������ս״̬
    player_obj->SetInt(TEXT_ARENA_FLAG, ARENA_FLAG_NOT_ARENA);
    player_obj->SetString(TEXT_ARENA_CHALLENGEE_INFO, "");
    player_obj->SetInt("Camp", ARENA_CAMP_DEFAULT);
    player_obj->SetDataWideStr(TEXT_ARENA_CANDIDATE_LIST, L"");
    player_obj->SetInt("GroupID", -1);
    
    if (scene_group_id > 0)
    {
		pKernel->ClearGroup(scene_group_id);
    }
    
    // �ָ����HP
    player_obj->SetInt64("HP", player_obj->QueryInt64("MaxHP"));
    player_obj->SetInt("Dead", 0);

	UnfreezePlayer(pKernel, self);
    
    return 0;
}

int ArenaModule::OnPlayerContinue(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{ 
		return 0;
	}

	const char* strUid = pSelfObj->QueryString(FIELD_PROP_UID);
	pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
		pKernel->GetSceneId(), strUid,
		CVarList() << SPACE_SNS
		<< SNS_ARENA_SPACE
		<< SNS_MSG_ARENA_QUERY_FIGHT_RESULT
		<< self);

	return 0;
}

//����sns��Ϣ
int ArenaModule::OnSnsMessage(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args)
{
	int msgid = args.IntVal(0);
	switch (msgid)
	{
	case SNS_MSG_ARENA_PUSH_FIGHT_RESULT:
		OnSnsQueryFightResult(pKernel, args);
		break;
	}
	return 0;
}

// ��ѯս���ظ�
int ArenaModule::OnSnsQueryFightResult(IKernel* pKernel, const IVarList& args)
{
	int nIndex = 1;
	PERSISTID player = args.ObjectVal(nIndex++);
	IGameObj* pSelfObj = pKernel->GetGameObj(player);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	IRecord* pResultRec = pSelfObj->GetRecord(FIELD_RECORD_ARENA_FIGHT_RESULT);
	if (NULL == pResultRec)
	{
		return 0;
	}
	int nRows = args.IntVal(nIndex++);
	if (nRows > 0)
	{
		// ֪ͨ�����ʾ���
		pKernel->Custom(player, CVarList() << SERVER_CUSTOMMSG_ARENA_MSG << ARENA_S2C_FIGHT_RESULT_CHANGE_TIP);
	}
	LoopBeginCheck(g);
	for (int i = 0; i < nRows;++i)
	{
		LoopDoCheck(g);
		// ������ �Ƴ����һ��
		if (pResultRec->GetRows() == pResultRec->GetRowMax())
		{
			pResultRec->RemoveRow(pResultRec->GetRows() - 1);
		}
		int nResult = args.IntVal(nIndex++);
		const wchar_t* enemy_name = args.WideStrVal(nIndex++);
		int challenger_prev_rank = args.IntVal(nIndex++);
		int challenger_new_rank = args.IntVal(nIndex++);
		int64_t nFightTime = args.Int64Val(nIndex++);

		CVarList newdata;
		newdata << nResult << enemy_name << challenger_prev_rank << challenger_new_rank << nFightTime;
		pResultRec->AddRowValue(0, newdata);
	}
	
	return 0;
}

int ArenaModule::OnCreateRobot(IKernel * pKernel, const PERSISTID & player,
                         const PERSISTID & sender, const IVarList &args)
{
    if (!pKernel->Exists(player))
    {
        return 0;
    }

    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return 0;
    }
    
//     STORE_TYPES_ENUM save_type = (STORE_TYPES_ENUM)args.IntVal(0);
//     
//     if (save_type == STORE_EXIT)
//     {
        int player_level = player_obj->QueryInt("Level");
        const char* rank_list_name = GetRankListName(pKernel, player_level);
        m_pRankListModule->InsertPlayerToRankList(pKernel, player, rank_list_name);
 //   }
   
    return 0;
}

// ��ҵȼ��仯
int ArenaModule::OnPlayerLevel(IKernel* pKernel, const PERSISTID &self,
    const PERSISTID & sender, const IVarList & args)
{   
#if defined(CREATE_ROBOT)

    return 0;

#endif
    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }

    int old_level = args.IntVal(1);;
    int new_level = player_obj->QueryInt("Level");
    const wchar_t *player_name = player_obj->QueryWideStr("Name");
    const char *player_uid = player_obj->QueryString("Uid");
    
    if (new_level < m_ArenaConfig.open_level)
    {
        // δ�ﵽ�����ȼ�
        return 0;
    }
    
    const char * old_rank_name = GetRankListName(pKernel, old_level);
    const char * new_rank_name = GetRankListName(pKernel, new_level);
    
    if (strcmp(old_rank_name, new_rank_name) !=0)
    {
        // �¾ɰ�ͬ, ��ҽ������µ����а�,��Ҫ�����ʷ��¼
        player_obj->SetInt(TEXT_HIGHEST_ARENA_RANK, 0);
    }
    
    // �������Ƿ����
    
    if (!StringUtil::CharIsNull(old_rank_name)
        && !StringUtil::CharIsNull(new_rank_name))
    {
        // �°�;ɰ񶼲�Ϊ��
        
        CVarList rank_list_row;
        if (!PrepareDataForArenaRankList(pKernel, self, new_rank_name, rank_list_row))
        {
            return 0;
        }
        
        if (strcmp(old_rank_name, new_rank_name) != 0)
        {
            // �¾ɰ�ͬ, �������ǽ���
            //PromotePlayer(pKernel, self, old_rank_name, new_rank_name, rank_list_row);
        }
        else
        {
            // �¾ɰ���ͬ, ��������п��ܲ������а��У����԰���Ҵ������а�
            CVarList the_msg;
            the_msg << ARENA_PUB_SUB_MSG_ADD_TO_RANKLIST;
            the_msg << player_name << player_uid << new_rank_name << rank_list_row;

            SendArenaMsgToPublic(pKernel, the_msg);
        }
    }
    else if (StringUtil::CharIsNull(old_rank_name)
             && !StringUtil::CharIsNull(new_rank_name))
    {
        // �ɰ�Ϊ��, �°�Ϊ��, �������ǵ�һ�ο�������������
        
        ResetArenaChance(pKernel, self);
        
        CVarList rank_list_row;
        if (!PrepareDataForArenaRankList(pKernel, self, new_rank_name, rank_list_row))
        {
            return 0;
        }
        
        //PromotePlayer(pKernel, self, "", new_rank_name, rank_list_row);
    }
    
    return 0;
}

// ���Կͻ��˵���Ϣ
int ArenaModule::OnCustomMessage(IKernel * pKernel, const PERSISTID & player,
                    const PERSISTID & sender, const IVarList & args)
{
    // ��Ϣ��ʽ
    //  
    //      [CLIENT_CUSTOMMSG_ARENA_MSG][sub_msg_id]...
    //

    if (!pKernel->Exists(player))
    {
        return 0;
    }
    
    int role_level = pKernel->QueryInt(player, "Level");
    if (role_level < m_ArenaConfig.open_level)
    {
        // δ�ﵽ������Ҫ��ȼ�
        return 0;
    }
    
    if (args.GetCount() < 2)
    {
        return 0;
    }
    
    int sub_msg_id = args.IntVal(1);
    
    switch (sub_msg_id)
    {
	case ARENA_C2S_QUERY_SELF_INFO:
        {
            // ��ѯ��ҵľ�������Ϣ
            OnSelfArenaInfo(pKernel, player, sender, args);
        }
        break;
	case ARENA_C2S_GET_CANDIDATE:
        {
            // ��ȡ����ս�ĺ�ѡ��
            OnGetChallengeCandidate(pKernel, player, sender, args);
        }
        break;
	case ARENA_C2S_CHALLENGE_SOMEONE:
        {
            // ��սĳ��
            OnChallengeSomeone(pKernel, player, sender, args);
        }
        break;
	case ARENA_C2S_GIVE_UP:
        {
            // ������ս
            OnGiveUpChallenge(pKernel, player, sender, args);
        }
        break;
	case ARENA_C2S_CHALLENGE_CLEAR_CD:
        {
			OnClearChallengeCD(pKernel, player, sender, args);
        }
        break;
	case ARENA_C2S_REQUEST_FIGHT_RESULT:
        {
			OnRequestFightResult(pKernel, player, sender, args);
        }
        break;
    default:
        break;
    }
    
    return 0;
}

// ���Թ�������Ϣ
int ArenaModule::OnPublicMessage(IKernel * pKernel, const PERSISTID & self,
                                 const PERSISTID & sender, const IVarList & args)
{
    //
    // ��Ϣ��ʽ
    // [msg_id][sub_msg_id][...]
    
    // public��������Ѿ���������Ϣ������һ������������

    int main_msg_id = args.IntVal(0);
    if (main_msg_id != PS_DOMAIN_MSG_ARENA)
    {
        return 0;
    }

    int sub_msg_id = args.IntVal(1);

    switch (sub_msg_id)
    {
    case ARENA_PUB_SUB_MSG_GIVE_RANK_REWARD:
        {
            // �������а���
            RewardByArenaRank(pKernel);
        }
        break;
    default:
        break;
    }

    return 0;
}

// ������ҵ�command
int ArenaModule::OnPlayerCommand(IKernel * pKernel, const PERSISTID & self,
                                 const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ
    //     [msg_id][sub_msg_id]...
    //
    
    int sub_msg_id = args.IntVal(1);
    
    switch(sub_msg_id)
    {
    case ARENA_PUB_SUB_MSG_CHALLENGE_REQUEST:
        {
            // ����ս����Ļ�Ӧ
            
            OnPubChallengeRequest(pKernel, self, sender, args);
        }
        break;
    case ARENA_PUB_SUB_MSG_PROMOTE:
        {
            // �Խ����Ļ�Ӧ

            OnPubPromote(pKernel, self, sender, args);
        }
        break;
    case ARENA_PUB_SUB_MSG_GET_CANDIDATE:
        {
            // �Ի�ȡ��ѡ�˵Ļ�Ӧ

            OnPubCandidate(pKernel, self, sender, args);
        }
        break;
    case ARENA_PUB_SUB_MSG_CHALLENGE_END:
        {
            // ����ս�����Ļ�Ӧ
            OnPubChallengeEnd(pKernel, self, sender, args);
        }
        break;
	case ARENA_MSG_ADD_FIGHT_RESULT_DATA:
		{
			// ����һ��ս������
			OnAddFightResultData(pKernel, self, sender, args);
		}
        break;
    default:
        break;
    }
    
    return 0;
}

// ��ʼս��ǰ�ĵ���ʱ
int ArenaModule::HB_BeforeFight( IKernel* pKernel, const PERSISTID& self, int slice)
{
    // ����ʱ����, ���Կ�����
    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }

    // ��¼��ս��ʼʱ��
    ADD_DATA(pKernel, self, TEXT_ARENA_CHALLENGE_START_TIME, VTYPE_INT64);
    player_obj->SetDataInt64(TEXT_ARENA_CHALLENGE_START_TIME, ::time(NULL));
    
    // ������¼��ս����ʱ��
    ADD_DATA(pKernel, self, TEXT_ARENA_CHALLENGE_END_TIME, VTYPE_INT64);

    // �ⶳ���
    UnfreezePlayer(pKernel, self);
    
    // ������ս����
    player_obj->SetInt(TEXT_ARENA_FLAG, ARENA_FLAG_IN_BATTLE);
    
    PERSISTID challengee = player_obj->QueryObject(TEXT_ARENA_CHALLENGEE_OBJ);
    if (pKernel->Exists(challengee))
    {
        UnfreezePlayer(pKernel, challengee);

        // ������ս�ߵ�NPC�����趨����
        AISystem::Instance()->GetAITemlate(pKernel, challengee).OnProcess(pKernel, AI_RULE_SPRING, challengee, self, CVarList());
    }
    
    // ս��ʱ���ļ�����
    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_RoundCheck");
    pKernel->AddCountBeat(self, "ArenaModule::HB_RoundCheck", m_ArenaConfig.round_time * 1000, 1);
    
    // ֪ͨ�ͻ��˿���
    CVarList msg_for_client;
    msg_for_client << SERVER_CUSTOMMSG_ARENA_MSG;
	msg_for_client << ARENA_S2C_FIGHT;
    msg_for_client << m_ArenaConfig.round_time;
    pKernel->Custom(self, msg_for_client);
    
    return 0;
}

// �غϼ��ļ�ʱ��
int ArenaModule::HB_RoundCheck( IKernel* pKernel, const PERSISTID& self, int slice)
{
    // ��սʱ���Ѿ�����, ����û����ɱ, ��սʧ��
    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }
    
    // ���ý�����־ 
    player_obj->SetInt(TEXT_ARENA_FLAG, ARENA_FLAG_END);
    
    int result = ARENA_RET_CHALLENGE_LOSE;
    int reason = ARENA_RET_TIME_OUT;

    PERSISTID challengee_obj = player_obj->QueryObject(TEXT_ARENA_CHALLENGEE_OBJ);
    if (!pKernel->Exists(challengee_obj))
    {
        result = ARENA_RET_CHALLENGE_WIN;
        reason = ARENA_RET_ONE_DIE;
    }
    else
    {
        // ������սʱ��, �ж���ս��ʧ��
        result = ARENA_RET_CHALLENGE_LOSE;
        reason = ARENA_RET_TIME_OUT;
    }
    
    OnChallengeEnd(pKernel, self, result, reason);

    WaitThenLeaveArena(pKernel, self);
    
    return 0;
}

// ��ʱ�˳�������
int ArenaModule::HB_CloseWait(IKernel* pKernel, const PERSISTID& self, int slice)
{
    // �ȴ�ʱ�����, ������뿪������
    
    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_CloseWait");

    LeaveArenaNow(pKernel, self);
    
    return 0;
}


// �뿪����������
int ArenaModule::HB_LeaveScene(IKernel* pKernel, const PERSISTID& self, int slice)
{
    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_LeaveScene");
    
    LeaveArenaNow(pKernel, self);
    
    return 0;
}

// ���һ�λ�ɱ
int ArenaModule::OnKillObject(IKernel* pKernel, const PERSISTID& self,
                              const PERSISTID& sender, const IVarList& args)
{
    // ֻ���ľ���������
    if (!IsArenaScene(pKernel))
    {
        return 0;
    }
    
    //������һ�λ�ɱ, ��ս����
    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }
    
    // �����ҵľ�����״̬
    int arena_flag = player_obj->QueryInt(TEXT_ARENA_FLAG);
    if (arena_flag == ARENA_FLAG_END)
    {
        return 0;
    }
    
    player_obj->SetInt(TEXT_ARENA_FLAG, ARENA_FLAG_END);
    
    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_RoundCheck");
    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_CloseWait");
    
    OnChallengeEnd(pKernel, self, ARENA_RET_CHALLENGE_WIN, ARENA_RET_ONE_DIE);
    
    WaitThenLeaveArena(pKernel, self);
    
    return 0;
}

// ����ɱ
int ArenaModule::OnBeKilled(IKernel* pKernel, const PERSISTID& self,
                            const PERSISTID& sender, const IVarList& args)
{
    // ֻ���ľ���������
    if (!IsArenaScene(pKernel))
    {
        return 0;
    }

    //��ұ���ɱ, ��ս����
    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }

    // �����ҵľ�����״̬
    int arena_flag = player_obj->QueryInt(TEXT_ARENA_FLAG);
    if (arena_flag == ARENA_FLAG_END)
    {
        return 0;
    }

    player_obj->SetInt(TEXT_ARENA_FLAG, ARENA_FLAG_END);

    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_RoundCheck");
    DELETE_HEART_BEAT(pKernel, self, "ArenaModule::HB_CloseWait");
    
    OnChallengeEnd(pKernel, self, ARENA_RET_CHALLENGE_LOSE, ARENA_RET_ONE_DIE);

    WaitThenLeaveArena(pKernel, self);
    
    return 0;
}

// �������
// int ArenaModule::PromotePlayer(IKernel* pKernel, const PERSISTID& self,
//                                   const char *current_ranklist_name, const char *new_ranklist_name)
// {
//     IGameObj *player_obj = pKernel->GetGameObj(self);
//     if (player_obj == NULL)
//     {
//         return 0;
//     }
//     
//     const char *player_uid = pKernel->QueryString(self, "Uid");
//     const wchar_t *player_name = pKernel->QueryWideStr(self, "Name");
// 
//     CVarList the_msg;
//     the_msg << ARENA_PUB_SUB_MSG_PROMOTE;
//     the_msg << player_name << player_uid << current_ranklist_name << new_ranklist_name;
// 
//     SendArenaMsgToPublic(pKernel, the_msg);
//     
//     return 0;
// }
// 
// // �������
// int ArenaModule::PromotePlayer(IKernel* pKernel, const PERSISTID& self,
//                                const char *current_ranklist_name, const char *new_ranklist_name,
//                                const IVarList &row_data)
// {
//     // ������ҵ�ʱ��, �����µ����ݲ������а�
//     IGameObj *player_obj = pKernel->GetGameObj(self);
//     if (player_obj == NULL)
//     {
//         return 0;
//     }
// 
//     const char *player_uid = pKernel->QueryString(self, "Uid");
//     const wchar_t *player_name = pKernel->QueryWideStr(self, "Name");
// 
//     CVarList the_msg;
//     the_msg << ARENA_PUB_SUB_MSG_PROMOTE
//             << player_name 
//             << player_uid 
//             << current_ranklist_name 
//             << new_ranklist_name
//             << row_data;
// 
//     SendArenaMsgToPublic(pKernel, the_msg);
//     
//     return 0;
// }

// ��ѯ�Լ��ľ�������Ϣ
int ArenaModule::OnSelfArenaInfo(IKernel * pKernel, const PERSISTID & player,
                                 const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ��
    // 
    //      [901][1]
    //
    // ��Ӧ��Ϣ��
    //
    //      [1801][1][��������ս����][����������][ս����][ս��][���ھ��������а�]
    //      [�½����ľ���������]
    
    CVarList summary; // [��������ս����][����������][ս����][ս��][���ھ��������а�]
    GetSelfArenaInfo(pKernel, player, summary);
    
    CVarList out_msg;
    out_msg << SERVER_CUSTOMMSG_ARENA_MSG;
	out_msg << ARENA_S2C_REV_SELF_INFO;
    out_msg << summary;
    //out_msg << pKernel->QueryString(player, TEXT_ARENA_NEW_RANK_LIST);
    
    // ֪ͨ�������ս�����ʶ
   // pKernel->SetString(player, TEXT_ARENA_NEW_RANK_LIST, "");
    
    pKernel->Custom(player, out_msg);
    
    return 0;
}

// ��ȡ��ѡ����ս��
int ArenaModule::OnGetChallengeCandidate(IKernel * pKernel, const PERSISTID & player,
                                         const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ��
    // 
    //      [901][3]
    //
    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return 0;
    }

    const wchar_t *challenger_name = player_obj->QueryWideStr("Name");
    const char *challenger_uid = player_obj->QueryString("Uid");
    int challenger_level = player_obj->QueryInt("Level");
    
    const char *rank_list_name = GetRankListName(pKernel, challenger_level);
    if (StringUtil::CharIsNull(rank_list_name))
    {
        return 0;
    }

    CVarList challenge_msg;
    challenge_msg << ARENA_PUB_SUB_MSG_GET_CANDIDATE; // ȥȡ��ѡ��
    challenge_msg << challenger_name 
        << challenger_uid 
        << rank_list_name
        << m_ArenaConfig.chosen_count;

    SendArenaMsgToPublic(pKernel, challenge_msg);
    
    return 0;
}

// ������սĳ��
int ArenaModule::OnChallengeSomeone(IKernel * pKernel, const PERSISTID & player,
                                    const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ��
    // 
    //      [901][4][challengee name]
    //
    // ��Ӧ��Ϣ��
    //
    //      ����ɹ���������Ϣ
    //
    //      ����ʧ�ܵ���Ϣ:
    //      [1801][4][error_code]
    //
    
		//����������
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_ARENA, player))
	{
		return 0;
	}

    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return 0;
    }

    const wchar_t *challengee_name = args.WideStrVal(2);
    const char *challengee_uid = pKernel->SeekRoleUid(challengee_name);

    // ���Ҫ��ս������ǺϺ�Ҫ��
    bool valid_challengee = false;
    std::wstring wstr_name_list = player_obj->QueryDataWideStr(TEXT_ARENA_CANDIDATE_LIST);

    CVarList temp_name_list;
    util_split_wstring(temp_name_list, wstr_name_list, L"$");
    int name_count = (int) temp_name_list.GetCount();

    LoopBeginCheck(h)
    for (int i=0; i<name_count; ++i)
    {
        LoopDoCheck(h)
        
        const wchar_t *temp_name = temp_name_list.WideStrVal(i);
        if (wcscmp(temp_name, challengee_name) == 0)
        {
            valid_challengee = true;
            break;
        }
    }
    
    if (!valid_challengee)
    {
        CVarList tip_args;
		::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_1, tip_args);
        return 0;
    }

	// ��ս�Ƿ���CD��
	if (CoolDownModule::m_pInstance->IsCoolDown(pKernel, player, COOLDOWN_ARENA_CHALLENGE))
	{
		::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_2, CVarList());
		return 0;
	}

	if (ActionMutexModule::IsHaveAction(pKernel, player))
	{
		return 0;
	}

    // ��¼����ս����
    ADD_DATA(pKernel, player, TEXT_ARENA_CHALLENGEE_NAME, VTYPE_WIDESTR);
    player_obj->SetDataWideStr(TEXT_ARENA_CHALLENGEE_NAME, challengee_name);

    bool success = false;
    
    // ����Ƿ�����ս����
    int arena_chance = player_obj->QueryInt(TEXT_ARENA_CHALLENGE_CHANCE);
    int arena_chance_used = player_obj->QueryInt(TEXT_ARENA_CHALLENGE_CHANCE_USED);
    if (arena_chance_used < arena_chance)
    {
        success = ChallengeSomeone(pKernel, player, challengee_name);
        
        if (!success)
        {
            CVarList tip_args;
			::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_1, tip_args);
        }
    }
    else
    {
        // ���᲻��, ֪ͨ�ͻ���ȥ����
        
        // �Ѿ�����Ļ�����
        int bought_chance = arena_chance - m_ArenaConfig.free_chance;
        if (bought_chance < 0)
        {
            
            CVarList tip_args;
			::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_1, tip_args);
            return 0;
        }

        // vip ���ö���Ĺ������
        int buy_limit = m_ArenaConfig.buy_chance_limit;
        //buy_limit += m_pVipModule->GetNumber(pKernel, player, BUY_CHALLENGE_TIMES);

        // ��Ҫ���ʽ�
        int fee = 0;
        if (buy_limit > bought_chance)
        {
			if (bought_chance >= 0 && bought_chance < ARENA_CHALLENGE_CHANCE_MAX_SIZE)
			{
				// û�г�����������
				fee = m_ArenaConfig.chance_fee[bought_chance];
			}
			else
			{
				fee = 0x7fffffff;
				::extend_warning(LOG_ERROR, "[ArenaModule::OnChallengeSomeone] config error ");
			}
        }
        
        CVarList buy_chance;
        buy_chance << SERVER_CUSTOMMSG_BUY_INFO;
        //buy_chance << VIP_BUY_CHALLENGE;
        buy_chance << fee;
        buy_chance << bought_chance; // �ѹ������
        buy_chance << buy_limit;     // �����������
        
        pKernel->Custom(player, buy_chance);        
    }
    
    return 0;
}


// ���򾺼�������
int ArenaModule::OnBuyArenaChance(IKernel * pKernel, const PERSISTID & player,
                            const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ
    //
    //      [msg][sub_msg]
    //

    int sub_msg_id = args.IntVal(1);
    
//     if (sub_msg_id != VIP_BUY_CHALLENGE)
//     {
//         return 0;
//     }
   
    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return 0;
    }
    
    int current_chance = player_obj->QueryInt(TEXT_ARENA_CHALLENGE_CHANCE);
    
    // �ѹ���Ļ��� = ��ǰӵ�еĻ��� - ��ѻ���
    int bought_chance = current_chance - m_ArenaConfig.free_chance;
    if (bought_chance < 0)
    {
        return 0;
    }
    
    // vip ���ö���Ĺ������
    int buy_limit = m_ArenaConfig.buy_chance_limit;
    //buy_limit += m_pVipModule->GetNumber(pKernel, player, BUY_CHALLENGE_TIMES);
    
    if (bought_chance >= buy_limit)
    {
        // �����Գ�����������
        CVarList tip_args;
        ::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_3, tip_args);
        return 0;
    }
    
    if (bought_chance >= ARENA_CHALLENGE_CHANCE_MAX_SIZE)
    {
        // ���ó���
        return 0;
    }
    
    // ��Ҫ��Ԫ����
    int fee = m_ArenaConfig.chance_fee[bought_chance];
	__int64 current_funds = m_pCapitalModule->GetCapital(pKernel, player, CAPITAL_TSILVER);
    if (fee > current_funds)
    {
        // �ʽ���
        CVarList tip_args;
        tip_args << fee;
		::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_4, tip_args);
            
        return 0;
    }
    
    // �۳��ʽ�
	int ret = m_pCapitalModule->DecCapital(pKernel, player, CAPITAL_TSILVER, fee, FUNCTION_EVENT_ID_BUY_ARENA);
    if (ret != DC_SUCCESS)
    {
        // ��Ǯʧ��
        CVarList tip_args;
		::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_4, tip_args);
        return 0;
    }
    
    // ����һ�λ���
    ++current_chance;
    player_obj->SetInt(TEXT_ARENA_CHALLENGE_CHANCE, current_chance);

	// ֪ͨ�ͻ��˹���ɹ�
	CVarList out_msg;
	out_msg << SERVER_CUSTOMMSG_ARENA_MSG
		<< ARENA_S2C_BUY_CHANCE_SUCCESS
		<< current_chance;

	pKernel->Custom(player, out_msg);

	//LogModule::m_pLogModule->OnArenaGameCost(pKernel, player, LOG_ARENA_BUY_CHALLENGE_TIMES, fee);
    
    // �����Ṻ���ֱ��ȥ��������ս
    
    // �ҵ�����Ķ���
//     if (player_obj->FindData(TEXT_ARENA_CHALLENGEE_NAME))
//     {
//         const wchar_t *challengee_name = player_obj->QueryDataWideStr(TEXT_ARENA_CHALLENGEE_NAME);
//         
//         bool success = ChallengeSomeone(pKernel, player, challengee_name);
// 
//         if (!success)
//         {  
//             CVarList tip_args;
//             ::CustomSysInfo(pKernel, player, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE,
//                 SYS_INFO_ARENA_ERROR, tip_args);
//         }
//     }
    
    return 0;
}

// ������ս
int ArenaModule::OnGiveUpChallenge(IKernel * pKernel, const PERSISTID & player,
                              const PERSISTID & sender, const IVarList & args)
{
    // ֻ��������
    if (!IsArenaScene(pKernel))
    {
        return 0;
    }

    if (!pKernel->Exists(player))
    {
        return 0;
    }

    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return 0;
    }

    // ȷ���Ǿ�����״̬
    int arena_flag = player_obj->QueryInt(TEXT_ARENA_FLAG);
    if (arena_flag == ARENA_FLAG_NOT_ARENA)
    {
        return 0;
    }

    // ����Ƿ��Ѿ�ս������
    if (arena_flag != ARENA_FLAG_END)
    {
        // ��ս������ǰ����
        player_obj->SetInt(TEXT_ARENA_FLAG, ARENA_FLAG_END);
        OnChallengeEnd(pKernel, player, ARENA_RET_CHALLENGE_LOSE, ARENA_RET_ESCAPE);
    }
    
    // �Ƴ��ȴ�����
    DELETE_HEART_BEAT(pKernel, player, "ArenaModule::HB_BeforeFight");
    DELETE_HEART_BEAT(pKernel, player, "ArenaModule::HB_RoundCheck");
    DELETE_HEART_BEAT(pKernel, player, "ArenaModule::HB_CloseWait");
    
    // ������ͻ�֮ǰ�ĵص�
    WaitThenLeaveArena(pKernel, player);

    return 0;
}

// ������սCD
int ArenaModule::OnClearChallengeCD(IKernel * pKernel, const PERSISTID & player, const PERSISTID & sender, const IVarList & args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(player);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// �Ѿ�����cd����
	if (!CoolDownModule::m_pInstance->IsCoolDown(pKernel, player, COOLDOWN_ARENA_CHALLENGE))
	{
		return 0; 
	}

	if (!m_pCapitalModule->CanDecCapital(pKernel, player, CAPITAL_TSILVER, m_ArenaConfig.challenge_clear_cd_fee))
	{
		::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_5, CVarList());
		return 0;
	}

	if (m_pCapitalModule->DecCapital(pKernel, player, CAPITAL_TSILVER, m_ArenaConfig.challenge_clear_cd_fee, FUNCTION_EVENT_ID_CLEAR_ARENA_CD) != DC_SUCCESS)
	{
		return 0;
	}

	CoolDownModule::m_pInstance->EndCoolDown(pKernel, player, COOLDOWN_ARENA_CHALLENGE);

	//LogModule::m_pLogModule->OnArenaGameCost(pKernel, player, LOG_ARENA_CLEAR_CHALLENGE_CD, m_ArenaConfig.challenge_clear_cd_fee);
	return 0;
}

// ��Ӧս����������
int ArenaModule::OnRequestFightResult(IKernel * pKernel, const PERSISTID & player, const PERSISTID & sender, const IVarList & args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(player);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	IRecord* pResultRec = pSelfObj->GetRecord(FIELD_RECORD_ARENA_FIGHT_RESULT);
	if (NULL == pResultRec)
	{
		return 0;
	}
	int nRows = pResultRec->GetRows();
	if (nRows == 0) // û��ս������
	{
		return 0;
	}
	
	CVarList msg;
	msg << SERVER_CUSTOMMSG_ARENA_MSG << ARENA_S2C_REV_FIGHT_RESULT_DATA << nRows;
	LoopBeginCheck(g);
	for (int i = 0; i < nRows; ++i)
	{
		LoopDoCheck(g);
		int nResult = pResultRec->QueryInt(i, COLUMN_ARENA_FIGHT_RESULT_RESULT);
		const wchar_t* enemy_name = pResultRec->QueryWideStr(i, COLUMN_ARENA_FIGHT_RESULT_NAME);
		int challenger_prev_rank = pResultRec->QueryInt(i, COLUMN_ARENA_FIGHT_RESULT_OLDRANK);
		int challenger_new_rank = pResultRec->QueryInt(i, COLUMN_ARENA_FIGHT_RESULT_CURRANK);
		int64_t nFightTime = pResultRec->QueryInt64(i, COLUMN_ARENA_FIGHT_RESULT_TIME);

		msg << nResult << enemy_name << challenger_prev_rank << challenger_new_rank << nFightTime;
	}

	pKernel->Custom(player, msg);
	return 0;
}

// ��ȡ��ҵľ�������Ϣ
bool ArenaModule::GetSelfArenaInfo(IKernel * pKernel, const PERSISTID & player, IVarList& summary)
{
    // @summary �ĸ�ʽ
    //    [������������ս����][���ջ�����][������������(0 ��ʾû������)][ս����][ս��][�������а�]
    //
    
    if (!pKernel->Exists(player))
    {
        return false;
    }

    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return false;
    }
    
    int arena_chance_used = player_obj->QueryInt(TEXT_ARENA_CHALLENGE_CHANCE_USED);
    int arena_chance = player_obj->QueryInt(TEXT_ARENA_CHALLENGE_CHANCE);
    int rank_no = 0;
    
    int role_level = player_obj->QueryInt("Level");
    const char* rank_list_name = GetRankListName(pKernel, role_level);
    
    IRecord *rank_rec = GetArenaRankListRecord(pKernel, rank_list_name);
    if (rank_rec != NULL)
    {
        const char *role_uid = player_obj->QueryString("Uid");
        int row = rank_rec->FindString(ARENA_RANK_COL_UID, role_uid);
        
        if (row != -1)
        {
            rank_no = row + 1; // @row ��0��ʼ��@rank_no ��1��ʼ 
        }
    }
    
    int battle_ability = player_obj->QueryInt("BattleAbility");
	__int64 battle_soul = 1;// m_pCapitalModule->GetCapital(pKernel, player, CAPITAL_BATTLE_SOUL);
    
    summary << arena_chance_used << arena_chance << rank_no 
            << battle_ability << battle_soul << rank_list_name;
    
    return true;
}

// ��ս����
int ArenaModule::OnChallengeEnd(IKernel * pKernel, const PERSISTID &challenger, int result, int reason)
{
    // ��������ս����
    IGameObj *challenger_obj = pKernel->GetGameObj(challenger);
    if (challenger_obj == NULL)
    {
        return 0;
    }

	//ActionMutexModule::CleanAction(pKernel, challenger);
    
    int challenger_level = challenger_obj->QueryInt("Level");
    const char *rank_list_name = GetRankListName(pKernel, challenger_level);
	const char *arena_sn = challenger_obj->QueryString(TEXT_ARENA_SN);
    
    // ��¼�½���ʱ��
    challenger_obj->SetDataInt64(TEXT_ARENA_CHALLENGE_END_TIME, ::time(NULL));
    
    // ���ܽ����Σ���Ҫ������ս�ߵ����а�����
    CVarList challenger_data;
    PrepareDataForArenaRankList(pKernel, challenger, rank_list_name, challenger_data);
    
    // ͬʱҲҪ������ս�ߵ�SNS����
    SnsDataModule::SaveSnsData(pKernel, challenger);
    
    const wchar_t *challenger_name = challenger_obj->QueryWideStr("Name");
    const char *challenger_uid = challenger_obj->QueryString("Uid");
    
    PERSISTID challengee = challenger_obj->QueryObject(TEXT_ARENA_CHALLENGEE_OBJ);
    const wchar_t *challengee_name = challenger_obj->QueryDataWideStr(TEXT_ARENA_CHALLENGEE_NAME);
    const char *challengee_uid = pKernel->SeekRoleUid(challengee_name);
    
    if (pKernel->Exists(challengee))
    {
        const wchar_t *npc_name = pKernel->QueryWideStr(challengee, "Name");
        if (wcscmp(npc_name, challengee_name) != 0)
        {
            challengee_uid = "";
        }
    }
    else
    {
        challengee_uid = "";
    }
    
    CVarList pub_msg;
    pub_msg << ARENA_PUB_SUB_MSG_CHALLENGE_END
			<< arena_sn
            << challenger_name
            << challenger_uid
            << pKernel->SeekRoleUid(challengee_name)
            << rank_list_name
            << result
            << challenger_data;
    
    SendArenaMsgToPublic(pKernel, pub_msg);

	m_pArenaModule->FreezeAllObject(pKernel, challenger_obj);
    return 0;
}

// ���ݾ�����������ͽ���, @reward_info ����˽������
bool ArenaModule::RewardByChallenge(IKernel * pKernel, const PERSISTID &challenger, int challenger_prev_rank,
                                    int challenger_new_rank, const wchar_t *challengee_name, int result)
{
	//����������
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_ARENA, challenger))
	{
		return false;
	}

    int challenger_level = pKernel->QueryInt(challenger, "Level");
    
    const char *ranklist_name = GetRankListName(pKernel, challenger_level);
    
    // ��������
    const ChallengeReward *reward_config = GetChallengeReward(challenger_level);
    
    if (reward_config == NULL)
    {											   

        return false;
    }
    
    RewardModule::AwardEx challenger_award;
	challenger_award.srcFunctionId = FUNCTION_EVENT_ID_ARENA;
    
    if (result == ARENA_RET_CHALLENGE_WIN)
    {
        // ��ս��ʤ��
        // ����ս�ߵĽ���
		//RewardModule::RewardPlayerById(pKernel, challenger, reward_config)
// 		challenger_award.name = m_ArenaConfig.challenger_win;		 
// 		RewardModule::ParseAddCapitals(reward_config->winner_award.str_capital.c_str(), challenger_award.capitals);
// 		RewardModule::ParseAddItems(reward_config->winner_award.str_item.c_str(), challenger_award.items);
// 
// 		ExpTree exp;
// 		challenger_award.exp = (int64_t)exp.CalculateEvent(pKernel, challenger, PERSISTID(), PERSISTID(), PERSISTID(), reward_config->winner_award.str_exp.c_str());
    }
    else
    {
        // ��ս��ʧ��
        // ����ս�ߵĽ���
//         challenger_award.name = m_ArenaConfig.challenger_failed;
// 		RewardModule::ParseAddCapitals(reward_config->loser_award.str_capital.c_str(), challenger_award.capitals);
// 		RewardModule::ParseAddItems(reward_config->loser_award.str_item.c_str(), challenger_award.items);
// 
// 		ExpTree exp;
// 		challenger_award.exp = (int64_t)exp.CalculateEvent(pKernel, challenger, PERSISTID(), PERSISTID(), PERSISTID(), reward_config->loser_award.str_exp.c_str());
    }
    
    const wchar_t *challenger_name = pKernel->QueryWideStr(challenger, "Name");

    // ���������
    m_pRewardModule->RewardPlayer(pKernel, challenger, &challenger_award, CVarList());
    return true;
}

// ��¼˫����ս������
void ArenaModule::RecordFightResultData(IKernel * pKernel, const PERSISTID &challenger, const wchar_t *challengee_name, int challenger_prev_rank, int challenger_new_rank, int challengee_rank, int nResult)
{
	IGameObj* pChallengerObj = pKernel->GetGameObj(challenger);
	if (NULL == pChallengerObj)
	{
		return;
	}

	// �ȼ�¼��ս�ߵ�����
	SaveOneFightResultData(pKernel, challenger, challengee_name, challenger_prev_rank, challenger_new_rank, nResult);

	// �ڼ�¼����ս�ߵ�����
	int nSceneId = pKernel->GetPlayerScene(challengee_name);

	int nFightResult = nResult == ARENA_RET_CHALLENGE_WIN ? ARENA_RET_CHALLENGE_LOSE : ARENA_RET_CHALLENGE_WIN;
	const wchar_t* wsChallengerName = pChallengerObj->QueryWideStr(FIELD_PROP_NAME);
	// �����˲��ü�¼
	if (IsRobot(pKernel, ARENA_RANK_LIST, challengee_name))
	{
		return;
	}
	
	// ����ս�ߵ���սǰ����ս������
	int nPreRank = 0;
	int nNewRank = 0;
	if (nFightResult == ARENA_RET_CHALLENGE_WIN)
	{
		nPreRank = challengee_rank;
		nNewRank = challengee_rank;
	}
	else
	{
		nPreRank = challenger_new_rank;
		nNewRank = challenger_prev_rank;
	}
	// �����߼�¼��sns
	if (nSceneId <= 0)
	{
		const char* targetUid = pKernel->SeekRoleUid(challengee_name);
		pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
			pKernel->GetSceneId(), targetUid,
			CVarList() << SPACE_SNS
			<< SNS_ARENA_SPACE
			<< SNS_MSG_ARENA_SAVE_FIGHT_RESULT
			<< nFightResult
			<< wsChallengerName
			<< nPreRank
			<< nNewRank);
	}
	else
	{
		CVarList msg;
		msg << COMMAND_ARENA_PLAYER << ARENA_MSG_ADD_FIGHT_RESULT_DATA << nFightResult << wsChallengerName << nPreRank << nNewRank;
		pKernel->CommandByName(challengee_name, msg);
	}
}

// ����һ��ս������
void ArenaModule::SaveOneFightResultData(IKernel * pKernel, const PERSISTID &player, const wchar_t *enemy_name, int challenger_prev_rank, int challenger_new_rank, int nResult)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(player);
	if (NULL == pSelfObj)
	{
		return;
	}

	IRecord* pResultRec = pSelfObj->GetRecord(FIELD_RECORD_ARENA_FIGHT_RESULT);
	if (NULL == pResultRec)
	{
		return;
	}
	// ������ �Ƴ����һ��
	if (pResultRec->GetRows() == pResultRec->GetRowMax())
	{
		pResultRec->RemoveRow(pResultRec->GetRows() - 1);
	}

	CVarList newdata;
	newdata << nResult << enemy_name << challenger_prev_rank << challenger_new_rank << time(NULL);
	pResultRec->AddRowValue(0, newdata);
}

// Public ����������ս����Ļ�Ӧ
int ArenaModule::OnPubChallengeRequest(IKernel * pKernel, const PERSISTID & self,
                                       const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ
    //     [msg_id][sub_msg_id][result][remark][arena_sn][challengee_uid][challengee_nation]
    //
    
    int result = args.IntVal(2);
    int remark = args.IntVal(3);
    
    if (result == ARENA_CODE_FAILED)
    {
        // public�������жϽ���������Խ�����ս
        
        switch (remark)
        {
        case ARENA_CODE_ONE_IN_CHALLENE:
            {
                // һ������ս��
				
				CVarList tip_args;
				::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_6, tip_args);
            }
            break;
        case ARENA_CODE_ONE_NOT_FOUND:
            {
                // ��������ڵ����а����Ҳ�����Ӧ����
            
                CVarList tip_args;
				::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_7, tip_args);
            }
            break;
        case ARENA_CODE_CHALLENGEE_LOW_RANK:
            {
                // �������ε����Լ�

                CVarList tip_args;
				::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_8, tip_args);
            }
            break;
        default:
            {
                // ϵͳ����
                CVarList tip_args;
				::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_9, tip_args);
            }
            break;
        }

		//��������սʧ�ܣ���ջ��������
		ActionMutexModule::CleanAction(pKernel, self);
            
        return 0;
    }

	const char *arena_sn = args.StringVal(4);
    const char *challengee_uid = args.StringVal(5);
    int challengee_nation = args.IntVal(6);
    
    // ���Խ�����ս
    
    char sz_challengee_info[512] = {0};
    SPRINTF_S(sz_challengee_info, "%s,%d", challengee_uid, challengee_nation); 
   
    pKernel->SetString(self, TEXT_ARENA_CHALLENGEE_INFO, sz_challengee_info);
    pKernel->SetInt(self, TEXT_ARENA_FLAG, ARENA_FLAG_ENTERING);
	pKernel->SetString(self, TEXT_ARENA_SN, arena_sn);


	// ��ʼ��ȴ
	CoolDownModule::m_pInstance->BeginCoolDown(pKernel, self, COOLDOWN_ARENA_CHALLENGE, m_ArenaConfig.challenge_cd);

    // ��¼����Ҵ�ʱ��λ��
    len_t pos_x = pKernel->GetPosiX(self);
    len_t pos_z = pKernel->GetPosiZ(self);
    len_t pos_y = pKernel->GetMapHeight(pos_x, pos_z);
    int sceneId = pKernel->GetSceneId();

    m_pLandPosModule->SetPlayerLandPosi(pKernel, self, sceneId, pos_x, pos_y, pos_z);	

	float fPosY = pKernel->GetMapHeight(m_ArenaConfig.challenger_born.x, m_ArenaConfig.challenger_born.z);

    // �������ȥ������
    bool switch_success = AsynCtrlModule::m_pAsynCtrlModule->SwitchLocate(pKernel, self,
        m_ArenaConfig.scene_id,
        m_ArenaConfig.challenger_born.x,
		fPosY,
        m_ArenaConfig.challenger_born.z,
        m_ArenaConfig.challenger_born.o);
        
    if (!switch_success)
    {
        //
        // ����ʧ��
        //

        pKernel->SetInt(self, TEXT_ARENA_FLAG, ARENA_FLAG_NOT_ARENA);
        
        // �ָ�˫��״̬״̬
        UnlockPlayer(pKernel, self);
        
        CVarList tip_args;
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_1, tip_args);
    }
    
    return 0;
}

// Public ����������ҽ����Ļ�Ӧ
int ArenaModule::OnPubPromote(IKernel * pKernel, const PERSISTID & self,
                        const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ
    //     [msg_id][sub_msg_id][result][rank_list_name][����]
    //
    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }

    int result = args.IntVal(2);

    if (result == ARENA_CODE_FAILED)
    {
        // ʧ��
        return 0;
    }
    
    std::string new_rank_list_name = args.StringVal(3);
    int new_rank = args.IntVal(4);

    std::string prev_rank_list_name = player_obj->QueryString(TEXT_ARENA_CUR_RANK_LIST);
    
    bool is_promotion = false;
    
    // �ж��Ƿ����
    if (prev_rank_list_name != new_rank_list_name)
    {
        is_promotion = true;
    }

    // ��¼��ʷ
    player_obj->SetString(TEXT_ARENA_CUR_RANK_LIST, new_rank_list_name.c_str());
    return 0;
}

// Public �������Ժ�ѡ�˵Ļ�Ӧ
int ArenaModule::OnPubCandidate(IKernel * pKernel, const PERSISTID & self,
                                const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ
    //    [msg_id][sub_msg_id][candidate_count][candidate_attr_count]
    //    [candidate1_rank][.. candidate1 row ..]... [candidateN_rank][.. candidateN row ..]
    //
    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }

    // ��ȡ����ѡ��
    CVarList candidate;
    candidate.Append(args, 2, args.GetCount() - 2);
    
    CVarList out_msg;
    out_msg << SERVER_CUSTOMMSG_ARENA_MSG;
	out_msg << ARENA_S2C_REV_CANDIDATE_INFO;
    out_msg << candidate; 
    
    pKernel->Custom(self, out_msg);

	int nSize = out_msg.GetCount();
    //
    // ��ȡ��ѡ����Ϣ����������
    //

    int player_level = player_obj->QueryInt("Level");
    const char *rank_list_name = GetRankListName(pKernel, player_level);

    int attr_count = candidate.IntVal(1);
    int name_col = RankListModule::m_pRankListModule->GetColumnIndexByName("player_name", rank_list_name);

    int temp_index = 2;
    int candidate_count = candidate.IntVal(0);
    std::wstring candidate_name_list;

    LoopBeginCheck(g)
    for (int i=0; i<candidate_count; ++i)
    {
        LoopDoCheck(g)

        const wchar_t *temp_name = candidate.WideStrVal(temp_index + 1 + name_col);

        candidate_name_list.append(temp_name);
        candidate_name_list.append(L"$");

        temp_index += attr_count;
    }

    // �����ѡ��
    ADD_DATA(pKernel, self, TEXT_ARENA_CANDIDATE_LIST, VTYPE_WIDESTR);
    player_obj->SetDataWideStr(TEXT_ARENA_CANDIDATE_LIST, candidate_name_list.c_str());

    return 0;
}

// Public ����������ս�����Ļ�Ӧ
int ArenaModule::OnPubChallengeEnd(IKernel * pKernel, const PERSISTID & self,
                                   const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ
    //    [msg_id][sub_msg_id][result_code][op_code][֮ǰ������][���ڵ�����]
    //
    
    // ��������ս����
    IGameObj *challenger_obj = pKernel->GetGameObj(self);
    if (challenger_obj == NULL)
    {
        return 0;
    }

	const wchar_t* challenger_name = challenger_obj->QueryWideStr(FIELD_PROP_NAME);
    int result_code = args.IntVal(2);
    int op_code = args.IntVal(3);
    int challenger_prev_rank = args.IntVal(4);
    int challenger_new_rank = args.IntVal(5);

	// ��ս��ʧ��ʱ,ȡ�±���ս�ߵ�����,���ڼ�¼ս��
	int challengee_rank = 0;
	if (result_code == ARENA_RET_CHALLENGE_LOSE)
	{
		challengee_rank = args.IntVal(6);
	}
    
    int challenger_level = challenger_obj->QueryInt("Level");
    const wchar_t *challengee_name = challenger_obj->QueryDataWideStr(TEXT_ARENA_CHALLENGEE_NAME);

    // �������α仯֪ͨ
    if (result_code == ARENA_RET_CHALLENGE_WIN)
    {
		// �Ƿ�����ȫ��ͨ��
		if (challenger_new_rank < m_pArenaModule->m_ArenaConfig.nBroadRank)
		{
			::CustomSysInfoBroadcast(pKernel, SYSTEM_INFO_ID_10, CVarList() << challenger_name << challengee_name << challenger_new_rank);
		}
    }

    // ���辺��������
	RewardByChallenge(pKernel, self, challenger_prev_rank, challenger_new_rank, challengee_name, result_code);

	// ��¼ս������
	m_pArenaModule->RecordFightResultData(pKernel, self, challengee_name, challenger_prev_rank, challenger_new_rank, challengee_rank, result_code);

    // ��ս��־
//     PVPLog the_log;
//     the_log.beChallenge = challengee_name;
//     the_log.result = result_code;
//     the_log.startTime = challenger_obj->QueryDataInt64(TEXT_ARENA_CHALLENGE_START_TIME);
//     the_log.endTime = challenger_obj->QueryDataInt64(TEXT_ARENA_CHALLENGE_END_TIME);
//     the_log.beforeRank = challenger_prev_rank;
//     the_log.afterRank = challenger_new_rank;
// 
//     m_pLogModule->SavePVPLog(pKernel, self, the_log);

    // ��ս�߻�õĽ���
    CVarList challenger_reward_info;
    const ChallengeReward *reward_config = GetChallengeReward(challenger_level);
    //int actionResult = LOG_GAME_ACTION_FAIL;
    if (reward_config != NULL)
    {
        if (result_code == ARENA_RET_CHALLENGE_WIN)
        {
           // actionResult = LOG_GAME_ACTION_SUCCESS;
			ExpTree exp;
			int nAddExp = (int)exp.CalculateEvent(pKernel, self, PERSISTID(), PERSISTID(), PERSISTID(), reward_config->winner_award.str_exp.c_str());

			challenger_reward_info << nAddExp
				<< reward_config->winner_award.str_capital;
        }
        else
        {
			ExpTree exp;
			int nAddExp = (int)exp.CalculateEvent(pKernel, self, PERSISTID(), PERSISTID(), PERSISTID(), reward_config->loser_award.str_exp.c_str());

			challenger_reward_info << nAddExp
				<< reward_config->loser_award.str_capital;
        }
    }
    else
    {
        challenger_reward_info << 0 << "" << "";
    }

    // �淨��־-������
//     GamePlayerActionLog log;
//     log.actionType     =    LOG_GAME_ACTION_ARENA;
//     log.actionState    =    actionResult;	
//     LogModule::m_pLogModule->SaveGameActionLog(pKernel, self, log);  

    // ֮ǰ���������
    int prev_best_rank = challenger_obj->QueryInt(TEXT_HIGHEST_ARENA_RANK);
    
    // �Ƿ�ͻ���������
    bool is_new_best = IsBestRank(pKernel, self, challenger_prev_rank, challenger_new_rank);
    
    // ͻ�����������õĽ���
    std::string capital_reward;
	std::string item_reward;
    
	bool bSendReward = false;
    if (is_new_best)
    {
        std::string the_rank_list_name = GetRankListName(pKernel, challenger_obj->QueryInt("Level"));
		bSendReward = GetRankUpReward(capital_reward, item_reward, the_rank_list_name, challenger_new_rank, prev_best_rank);

		// ����ͻ����ʷ�������
		challenger_obj->SetInt(TEXT_HIGHEST_ARENA_RANK, challenger_new_rank);
    }

    // ������ս�ߵ���Ϣ
    CVarList msg_for_client;
    msg_for_client << SERVER_CUSTOMMSG_ARENA_MSG;
	msg_for_client << ARENA_S2C_CHALLENGE_END;
    msg_for_client << result_code;
    msg_for_client << challenger_reward_info; // ��������
    msg_for_client << challenger_prev_rank; // ԭ��������
    msg_for_client << challenger_new_rank; // ������				   
    msg_for_client << prev_best_rank; // ��ʷ�������
    
	if (bSendReward)
    {
		msg_for_client << capital_reward << item_reward;	 // ������������
		RewardByRankUp(pKernel, self, capital_reward, item_reward, challenger_new_rank);
    }
    
	pKernel->Custom(self, msg_for_client);
	//LogModule::m_pLogModule->OnArenaGameRank(pKernel, self, challenger_prev_rank, challenger_new_rank);
    return 0;
}

// ����һ��ս������
int ArenaModule::OnAddFightResultData(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}
	//msg << COMMAND_ARENA_PLAYER << ARENA_MSG_ADD_FIGHT_RESULT_DATA << nFightResult << wsChallengerName << challenger_new_rank << challenger_prev_rank;
	int nResult = args.IntVal(2);
	const wchar_t* wsName = args.WideStrVal(3);
	int nOldRank = args.IntVal(4);
	int nCurRank = args.IntVal(5);

	int online = pSelfObj->QueryInt(FIELD_PROP_ONLINE);
	// �������,�����ݱ���sns
	if (online == OFFLINE)
	{
		const char* strUid = pSelfObj->QueryString(FIELD_PROP_UID);
		pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
			pKernel->GetSceneId(), strUid,
			CVarList() << SPACE_SNS
			<< SNS_ARENA_SPACE
			<< SNS_MSG_ARENA_SAVE_FIGHT_RESULT
			<< nResult
			<< wsName
			<< nOldRank
			<< nCurRank);
	}
	else
	{
		m_pArenaModule->SaveOneFightResultData(pKernel, self, wsName, nOldRank, nCurRank, nResult);
		// ֪ͨ�����ʾ���
		pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_ARENA_MSG << ARENA_S2C_FIGHT_RESULT_CHANGE_TIP);
	}
	return 0;
}

// VIP�����ص�
int ArenaModule::C_OnVIPLevelChanged(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old)
{
	return 0;
}

// ��սĳ��
bool ArenaModule::ChallengeSomeone(IKernel * pKernel, const PERSISTID & player,
                                   const wchar_t* challengee_name)
{
    if (!pKernel->Exists(player))
    {
        return false;
    }

    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return false;
    }
    
    // �ҵ�����������а�
    int player_level = player_obj->QueryInt("Level");
    const char *rank_list_name = GetRankListName(pKernel, player_level);
    
    if (StringUtil::CharIsNull(rank_list_name))
    {
        CVarList tip_args;
		::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_1, tip_args);
            
        return false;
    }

    const wchar_t *challenger_name = player_obj->QueryWideStr("Name");
    const char *challenger_uid = player_obj->QueryString("Uid");
    const char *challengee_uid = pKernel->SeekRoleUid(challengee_name);

    if (strcmp(challenger_uid, challengee_uid) == 0)
    {
        // ��������ս�Լ�
        return false;
    }																				  

    CVarList challenge_msg;
    challenge_msg << ARENA_PUB_SUB_MSG_CHALLENGE_REQUEST; // ��ս����
    challenge_msg << challenger_name << rank_list_name << challenger_uid << challengee_uid;

    SendArenaMsgToPublic(pKernel, challenge_msg);

    return true;
}

// ���ž�������������
bool ArenaModule::RewardByArenaRank(IKernel *pKernel)
{
    // �����������, ����п����ڶ�����д������ݣ�ֻ������ߵİ�

    // �������а�
    std::vector<ArenaRankListConfig>::const_iterator it = m_ArenaRankList.begin();
    std::vector<ArenaRankListConfig>::const_iterator it_end = m_ArenaRankList.end();

    // �������а�
    LoopBeginCheck(k)
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(k)
        
        const ArenaRankListConfig &the_rank_list_config = *it;

        // �������а������, �ҵ����ʵĽ�����
        const std::vector<RankReward> *reward_list =GetRankReward(the_rank_list_config.name);
        if (reward_list == NULL)
        {
            continue;
        }
        
        IRecord *rec = GetArenaRankListRecord(pKernel, the_rank_list_config.name.c_str());
        if (rec == NULL)
        {
            continue;
        }
        
        int rec_row_count = rec->GetRows();
        if (rec_row_count == 0)
        {
            continue;
        }
        
        // �������е�ÿ����Ŀ����ָ�������η�Χ
        std::vector<RankReward>::const_iterator reward_it = reward_list->begin();
        std::vector<RankReward>::const_iterator reward_it_end = reward_list->end();
        
        LoopBeginCheck(l)
        for (; reward_it != reward_it_end; ++reward_it)
        {
            LoopDoCheck(l)
            
            const RankReward &the_reward = *reward_it;
            
			int start_row = the_reward.upper_rank - 1; // ��Ϊ���δ�1��ʼ���������ݴ�0��ʼ, �ʶ���Ҫ -1
			int end_row = the_reward.lower_rank - 1;
            
            if (start_row > end_row)
            {
                continue;
            }
            
            // ȷ��������������Χ
            if (start_row >= rec_row_count)
            {
                continue;
            }
            
            if (end_row >= rec_row_count)
            {
                end_row = rec_row_count - 1;
            }

// 			RewardModule::AwardEx the_award;
// 			the_award.name = m_ArenaConfig.rank_reward_name;
// 			RewardModule::ParseAddCapitals(the_reward.award.str_capital.c_str(), the_award.capitals);
// 			RewardModule::ParseAddItems(the_reward.award.str_item.c_str(), the_award.items);
// 			the_award.srcFunctionId = FUNCTION_EVENT_ID_ARENA_REWARD;
// 			//the_award.exp = the_reward.award.exp * nMulriple;
// 			// ��ǰ��������������
// 			if (nMulriple > 1)
// 			{
// 				for (RewardModule::CapitalsList::iterator itr = the_award.capitals.begin(); itr != the_award.capitals.end(); ++itr)
// 				{
// 					itr->second *= nMulriple;
// 				}
// 
// 				for (RewardModule::ItemsList::iterator itr = the_award.items.begin(); itr != the_award.items.end(); ++itr)
// 				{
// 					itr->nAmount *= nMulriple;
// 				}
// 			}            
            CVarList mail_param;
            
            int account_col = m_pRankListModule->GetColumnIndexByName("player_account", the_rank_list_config.name.c_str());
            int role_name_col = m_pRankListModule->GetColumnIndexByName("player_name", the_rank_list_config.name.c_str());
            int role_type_col = m_pRankListModule->GetColumnIndexByName("player_role_flag", the_rank_list_config.name.c_str());

            // ���ݴ���Ŀ��Ӧ������, �ҵ���Ӧ�����
            LoopBeginCheck(o)
            for (int row=start_row; row<=end_row; ++row)
            {
                LoopDoCheck(o)
                
                mail_param.Clear();
                
                CVarList row_data;
                rec->QueryRowValue(row, row_data);
                
                 
                std::string player_account = row_data.StringVal(account_col);
                std::wstring player_name = row_data.WideStrVal(role_name_col);
                int role_type = row_data.IntVal(role_type_col);
                
                mail_param << row + 1; // ���к�תΪ����
                
                // �ų�������
                if (role_type == ROLE_FLAG_ROBOT)
                {
                    continue;
                }
                
                // ͨ���ʼ������������
               // m_pRewardModule->RewardViaMail(pKernel, player_name.c_str(), &the_award, mail_param);
            }
        }
    }

    return true;
}

int ArenaModule::ResetArenaChanceTimer(IKernel* pKernel, const PERSISTID& self, int slice)
{
    ResetArenaChance(pKernel, self);
    return 0;
}


// ���Ž�������
bool ArenaModule::RewardByPromotion(IKernel* pKernel, const PERSISTID& self, const char *rank_list_name)
{
    const PromotionReward *reward = GetPromotionReward(rank_list_name);
    
    if (reward == NULL)
    {
        return false;
    }
    
    const wchar_t *player_name = pKernel->QueryWideStr(self, "Name");
    
//     RewardModule::AwardEx the_award;
//     the_award.name = reward->reward_name;
// 	the_award.srcFunctionId = FUNCTION_EVENT_ID_ARENA_REWARD;
// 	RewardModule::ParseAddCapitals(reward->capital.c_str(), the_award.capitals);
// 	RewardModule::ParseAddItems(reward->item.c_str(), the_award.items);
//     
//     CVarList mail_param;
//     m_pRewardModule->RewardViaMail(pKernel, player_name, &the_award, mail_param);

    return true;
}

// ������սѡ��
bool ArenaModule::UnlockPlayer(IKernel * pKernel, const PERSISTID & player)
{
    if (!pKernel->Exists(player))
    {																												  
        return false;
    }

    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return false;
    }

    int level = player_obj->QueryInt("Level");																							    
    const char *rank_list_name = GetRankListName(pKernel, level);

	std::string arena_sn = player_obj->QueryString(TEXT_ARENA_SN);
    
	if (arena_sn.length() == 0)
	{
		return true;
	}
    
    CVarList challenge_msg;
    challenge_msg << ARENA_PUB_SUB_MSG_UNLOCK_PLAYER; // �������
	challenge_msg << arena_sn.c_str() << rank_list_name ;

    SendArenaMsgToPublic(pKernel, challenge_msg);

    return true;
}

// �������
void ArenaModule::FreezePlayer(IKernel * pKernel, const PERSISTID & player)
{
	IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if (NULL == pPlayerObj)
	{
		return;
	}
	pPlayerObj->SetInt("CantBeAttack", 1);
	pPlayerObj->SetInt("CantAttack", 1);
	pPlayerObj->SetInt("CantMove", 1);
	pPlayerObj->SetInt("CantUseSkill", 1);
}

// �ⶳ���
void ArenaModule::UnfreezePlayer(IKernel * pKernel, const PERSISTID & player)
{
	IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if (NULL == pPlayerObj)
	{
		return;
	}
	pPlayerObj->SetInt("CantBeAttack", 0);
	pPlayerObj->SetInt("CantAttack", 0);
	pPlayerObj->SetInt("CantMove", 0);
	pPlayerObj->SetInt("CantUseSkill", 0);
}

// �ȴ�һ��ʱ��Ȼ���뿪������
void ArenaModule::WaitThenLeaveArena(IKernel * pKernel, const PERSISTID & player)
{
    DELETE_HEART_BEAT(pKernel, player, "ArenaModule::HB_BeforeFight");
    DELETE_HEART_BEAT(pKernel, player, "ArenaModule::HB_RoundCheck");
    DELETE_HEART_BEAT(pKernel, player, "ArenaModule::HB_CloseWait");
    
    // ��ʱ�˳�
    pKernel->AddCountBeat(player, "ArenaModule::HB_CloseWait", m_ArenaConfig.wait_before_close * 1000, 1);

}

// �����뿪������
void ArenaModule::LeaveArenaNow(IKernel * pKernel, const PERSISTID & player)
{
    DELETE_HEART_BEAT(pKernel, player, "ArenaModule::HB_BeforeFight");
    DELETE_HEART_BEAT(pKernel, player, "ArenaModule::HB_RoundCheck");
    DELETE_HEART_BEAT(pKernel, player, "ArenaModule::HB_CloseWait");
    
    // �����뿪������
    if (!m_pLandPosModule->PlayerReturnLandPosi(pKernel, player))
    {
        // �˳�ʧ��, 2�������
        pKernel->AddCountBeat(player, "ArenaModule::HB_LeaveScene", 2 * 1000, 1);
    }
}

// ������Ƿ���Ա���ս
bool ArenaModule::IsValidChallengee(const char *challenger_uid,
                              CVarList &challengee_info, const char *rank_list_name)
{
    int uid_col = m_pRankListModule->GetColumnIndexByName(TEXT_ARENA_RANK_COL_UID, rank_list_name);
    int sn_col = m_pRankListModule->GetColumnIndexByName(TEXT_ARENA_RANK_COL_SN, rank_list_name);
    
	if (uid_col == -1 || sn_col == -1)
	{
		return false;
	}

    const char *temp_challengee_uid = challengee_info.StringVal(uid_col);
	std::string challengee_arena_sn = challengee_info.StringVal(sn_col);

    if (strcmp(temp_challengee_uid, challenger_uid) == 0)
    {
        // ������ս�Լ�
        return false;
    }

    if (challengee_arena_sn.length() > 0)
    {
        // �����Ѿ��ھ�������
        return false;
    }
    
    return true;
}

// ���þ���������
bool ArenaModule::ResetArenaChance(IKernel * pKernel, const PERSISTID & player)
{
    // �����ҵľ����������Ƿ���Ҫ����
    
    if (!pKernel->Exists(player))
    {
        return false;
    }

    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return false;
    }
    
    int player_level = player_obj->QueryInt("Level");

    if (player_level < m_ArenaConfig.open_level)
    {
        return 0;    
    }

    char date_of_today[128] = {0};
    util_convert_date_to_string(date_of_today, sizeof(date_of_today));
    
    const char *last_reset_date = player_obj->QueryString(TEXT_ARENA_CHANCE_RESET_DATE);
    
    // �������ڲ��ǽ���, ��Ҫ��������
    if (strcmp(date_of_today, last_reset_date) != 0)
    {
        player_obj->SetInt(TEXT_ARENA_CHALLENGE_CHANCE, m_ArenaConfig.free_chance);
        player_obj->SetInt(TEXT_ARENA_CHALLENGE_CHANCE_USED, 0);
        player_obj->SetString(TEXT_ARENA_CHANCE_RESET_DATE, date_of_today);
    }
    
    return true;
}

// �Ƿ񾺼�������
bool ArenaModule::IsArenaScene(IKernel * pKernel)
{
    PERSISTID scene = pKernel->GetScene();
    const char *scene_script = pKernel->GetScript(scene);

    if (strcmp(scene_script, "ArenaScene") == 0)
    {
        return true;
    }
    
    return false;
}


// gm��������Լ��ľ�����״̬
bool ArenaModule::ResetArenaStateByGm(IKernel * pKernel, const wchar_t* wsName, int nLockState)
{
	if (StringUtil::CharIsNull(wsName))
	{
		return false;
	}

	PERSISTID player = pKernel->FindPlayer(wsName);
	IGameObj* player_obj = pKernel->GetGameObj(player);
	if (NULL == player_obj)
	{
		return false;
	}

	int level = player_obj->QueryInt("Level");
	const char *rank_list_name = GetRankListName(pKernel, level);
	const char* strUid = pKernel->SeekRoleUid(wsName);

	CVarList challenge_msg;
	challenge_msg << ARENA_PUB_SUB_MSG_GM_RESET_ARENA_STATE; // �������
	challenge_msg << rank_list_name;
	challenge_msg << strUid;
	challenge_msg << nLockState;

	SendArenaMsgToPublic(pKernel, challenge_msg);
	return true;
}

// �����ȥ�ʳ�
bool ArenaModule::GoToCity(IKernel * pKernel, const PERSISTID & player)
{
    if (!pKernel->Exists(player))
    {
        return false;
    }

    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return false;
    }

	int main_scene_id = 1001;//GetNationMainSceneId(player_nation);	   // �Ժ�������

    pKernel->SwitchBorn(player, main_scene_id);
    
    return true;
}

bool ArenaModule::IsRobot(IKernel * pKernel, const char *ranklist_name, const wchar_t *role_name)
{
    IRecord *rec = GetArenaRankListRecord(pKernel, ranklist_name);
    if (rec == NULL)
    {
        return true;
    }
    
    const char *role_uid = pKernel->SeekRoleUid(role_name);
    
    int uid_col = m_pRankListModule->GetColumnIndexByName("player_uid", ranklist_name);
    int role_type_col = m_pRankListModule->GetColumnIndexByName("player_role_flag", ranklist_name);
    
    int row_id = rec->FindString(uid_col, role_uid);
    if (row_id == -1)
    {
        return true;
    }
    
    CVarList row_info;
    rec->QueryRowValue(row_id, row_info);
    
    int role_flag = row_info.IntVal(role_type_col);
    if (role_flag != ROLE_FLAG_ROBOT)
    {
        return false;
    }
    
    return true;
}

// �����������ͻ�ƽ���
int ArenaModule::RewardByRankUp(IKernel *pKernel, const PERSISTID & player, const std::string &capital_reward, const std::string& item_reward, int new_rank)
{ 
    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return 0;
    }
    
    // ��װ�ʼ��������û�
//     RewardModule::AwardEx award;
// 	award.srcFunctionId = FUNCTION_EVENT_ID_ARENA_REWARD;
//     award.name = TEXT_RANK_UP_REWARD;
// 	RewardModule::ParseAddItems(item_reward.c_str(), award.items);
// 	RewardModule::ParseAddCapitals(capital_reward.c_str(), award.capitals);
//     
//     CVarList mail_param;
//     mail_param << new_rank;
//     
//     const wchar_t *player_name = player_obj->QueryWideStr("Name");
//     
//     m_pRewardModule->RewardViaMail(pKernel, player_name, &award, mail_param);

    return 0;
}

// ȡ�ú�ѡ�˹���
const ArenaModule::CandidateRule* ArenaModule::GetCandidateRule(int player_rank)
{
    std::vector<CandidateRule>::const_iterator it = m_CandidateRule.begin();
    std::vector<CandidateRule>::const_iterator it_end = m_CandidateRule.end();
    
    LoopBeginCheck(q)
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(q)
        
        const CandidateRule& the_rule = *it;
        
        if (player_rank == ARENA_NO_RANK)
        {
            // �����û��
        }
        else
        {
			if (player_rank >= the_rule.upper_rank
				&& player_rank <= the_rule.lower_rank)
            {
                return &the_rule;
            }
        }
        

    }
    
    return NULL;
}

// ȡ����ս��������
const ArenaModule::ChallengeReward* ArenaModule::GetChallengeReward(int player_level)
{
    std::vector<ChallengeReward>::const_iterator it = m_ChanllengeReward.begin();
    std::vector<ChallengeReward>::const_iterator it_end = m_ChanllengeReward.end();

    LoopBeginCheck(r)
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(r)
        
        const ChallengeReward& the_reward = *it;

        if (player_level >= the_reward.lower_level
            && player_level <= the_reward.upper_level)
        {
            return &the_reward;
        }
    }

    return NULL;
}

// ȡ��������������
const std::vector<ArenaModule::RankReward>* ArenaModule::GetRankReward(const std::string &rank_list_name)
{
    RankRewardMap::const_iterator it_find = m_RankReward.find(rank_list_name);
    if (it_find != m_RankReward.end())
    {
        return &(it_find->second);
    }
    
    return NULL;
}

// �������а�����ֻ�ý�������
const ArenaModule::PromotionReward* ArenaModule::GetPromotionReward(const std::string &rank_list_name)
{
    PromotionRewardMap::const_iterator it_find = m_PromotionReward.find(rank_list_name);
    if (it_find != m_PromotionReward.end())
    {
        return &(it_find->second);
    }

    return NULL;
}

// ȡ�þ��������а�ı�
IRecord* ArenaModule::GetArenaRankListRecord(IKernel * pKernel, const char* rank_list_name)
{
    // ��ȡ�������������
    if (!pKernel->FindPubSpace(PUBSPACE_DOMAIN))
    {
        return NULL;
    }

    IPubSpace * pub_space = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
    if (pub_space == NULL)
    {
        return NULL;
    }

    wchar_t domain_name[256] = {0};
    SWPRINTF_S(domain_name, L"Domain_RankingList_%d", pKernel->GetServerId());

    IPubData * pub_data = pub_space->GetPubData(domain_name);
    if (pub_data == NULL)
    {
        return NULL;
    }

    IRecord * the_rec = pub_data->GetRecord(rank_list_name);
    return the_rec;
}

// ���ݵȼ�ȡ�ö�Ӧ�ľ��������а�
const char* ArenaModule::GetRankListName(IKernel * pKernel, int level)
{
    std::vector<ArenaRankListConfig>::const_iterator it = m_ArenaRankList.begin();											 
    std::vector<ArenaRankListConfig>::const_iterator it_end = m_ArenaRankList.end();
    
    LoopBeginCheck(s)
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(s)
        
        const ArenaRankListConfig &the_list = *it;
        
        if (level >= the_list.lower_level && level <= the_list.upper_level)
        {
#if defined(CREATE_ROBOT)

            if (level < 15)
            {
                return "";
            }

#endif

            return the_list.name.c_str();
        }
    }
    
    return "";
}


// ��������
bool ArenaModule::ParseFee(std::string &str_fee, int *fee_array, int fee_array_size)
{
    // @str_fee �ĸ�ʽ: 10,20,....N
    
    // ��ʼ��@fee_array
    
    memset(fee_array, 0, sizeof(int) * fee_array_size);
    
    CVarList split_result;
    util_split_string(split_result, str_fee, ",");
    
    int fee_count = (int) split_result.GetCount();
    if (fee_count > fee_array_size)
    {
        fee_count = fee_array_size;
    }
    
    if (fee_count == 0)
    {
        return true;
    }
    
    LoopBeginCheck(t)
    for (int i=0; i<fee_count; ++i)
    {
        LoopDoCheck(t)
        
        fee_array[i] = atoi(split_result.StringVal(i));
    }
    
    LoopBeginCheck(u)
    for (int i=0; i<(fee_array_size - fee_count); ++i)
    {
        LoopDoCheck(u)
    
        fee_array[fee_count+i] = fee_array[fee_count-1];
    }
    
    return true;
}

// ��������
bool ArenaModule::ParseCoordinate(const std::string &str_coordinate, Coordinate& coordinate)
{
    // @str_coordinate �ĸ�ʽ: x,y,z,o
    
    CVarList split_result;
    util_split_string(split_result, str_coordinate, ",");
    
    int list_size = (int) split_result.GetCount();
    if (list_size != 3)
    {
        return false;
    }

    coordinate.x = (len_t) atof(split_result.StringVal(0));
    coordinate.z = (len_t) atof(split_result.StringVal(1));
    coordinate.o = (len_t) atof(split_result.StringVal(2));
    
    return true;
}

// ������������Ϣ��public������
void ArenaModule::SendArenaMsgToPublic(IKernel * pKernel, const IVarList& msg)
{
    CVarList pub_msg;
    pub_msg << PUBSPACE_DOMAIN;
    pub_msg << GetDomainName(pKernel).c_str();
    pub_msg << SP_DOMAIN_MSG_ARENA;
    pub_msg << msg;

    pKernel->SendPublicMessage(pub_msg);
}

// ���а��һ��
bool ArenaModule::PrepareDataForArenaRankList(IKernel * pKernel, const PERSISTID & player, 
    const char *rank_list_name, IVarList& row_data)
{
    return m_pRankListModule->PrepareDataForRankList(pKernel, player, rank_list_name, row_data);
}

// ���а��֪ͨ����
ArenaModule::NotifyStrategy  ArenaModule::GetRankListNotifyStrategy(std::string &rank_list_name)
{
    std::vector<ArenaRankListConfig>::const_iterator it = m_ArenaRankList.begin();
    std::vector<ArenaRankListConfig>::const_iterator it_end = m_ArenaRankList.end();

    LoopBeginCheck(s)
	for (; it != it_end; ++it)
	{
		LoopDoCheck(s)
		const ArenaRankListConfig &the_list = *it;
		if (the_list.name == rank_list_name)
		{
			return (NotifyStrategy)the_list.need_notify;
		}
	}

	return DO_NOT_NOTIFY;
}

// �����������ͻ�ƽ���, @old_rank = ֮ǰ���������, @new_rank=��ǰ����
bool ArenaModule::GetRankUpReward(std::string &capital_reward, std::string& item_reward, const std::string &rank_list_name, int new_rank, int old_rank)
{
    RankUpRewardMap::iterator map_it = m_RankUpReward.find(rank_list_name);
    if (map_it == m_RankUpReward.end())
    {
        return false;
    }
    
    std::vector<RankUpReward> &reward_list = map_it->second;
    
    std::vector<RankUpReward>::iterator list_it = reward_list.begin();
    std::vector<RankUpReward>::iterator list_it_end = reward_list.end();
    
	std::vector<RankUpReward>::iterator old_it = list_it_end;
	std::vector<RankUpReward>::iterator new_it = list_it_end;

	// ֻ����������ﵽ��һ��������,�Żᷢ����
	bool bFind = false;
	LoopBeginCheck(a);
    for (; list_it != list_it_end; ++list_it)
    {
		LoopDoCheck(a);
        RankUpReward &the_reward = *list_it;
		if (old_rank >= the_reward.upper_rank && old_rank <= the_reward.lower_rank)
		{
			old_it = list_it;
		}

		if (new_rank >= the_reward.upper_rank && new_rank <= the_reward.lower_rank)
		{
			new_it = list_it;
			capital_reward = the_reward.award.str_capital;
			item_reward = the_reward.award.str_item;
		}	   

		if (old_it != list_it_end && new_it != list_it_end)
		{
			break;
		}
    }

	// ����������������û�н���
	if (old_it == list_it_end && new_it == list_it_end)
	{
		bFind = false;
	}
	else
	{
		bFind = old_it != new_it;
	}
	return bFind;
}

// ����������������ȡ����
bool ArenaModule::IsBestRank(IKernel * pKernel, const PERSISTID & player,
                                  int old_rank, int new_rank)
{
	if (new_rank <= 0)
	{
		return false;
	}
    // @old_rank ==0 ��ʾ��Ҵ�δ�ϰ�
    if (old_rank != 0)
    {
        if (old_rank <= new_rank)
        {
            return false;
        }
    }

    IGameObj *player_obj = pKernel->GetGameObj(player);

    int highest_arena_rank = player_obj->QueryInt(TEXT_HIGHEST_ARENA_RANK);
    if (highest_arena_rank != 0)
    {
        if (new_rank >= highest_arena_rank)
        {
            // ������û�дﵽ��ʷ�������
            return false;
        }
    }

    return true;
}

// �� float ת��INT, ����ȡ��
// int ArenaModule::Float2IntRoundUp(float f)
// {
//     int ret = (int) f;
//     int temp = (int) (f * 10);
//     
//     if (temp % 10 == 0)
//     {
//         return ret;
//     }
//     else
//     {
//         return ret + 1;
//     }
// }

// �����������, ����п����ڶ�����д������ݣ�ֻ������ߵİ�
int ArenaModule::PlayerRanklistClean(IKernel * pKernel)
{

    // ��ȡ���б��IRecord ָ��
    std::vector<RanklistRecInfo> all_rec;

    std::vector<ArenaRankListConfig>::const_iterator it = m_ArenaRankList.begin();
    std::vector<ArenaRankListConfig>::const_iterator it_end = m_ArenaRankList.end();

	LoopBeginCheck(a);
    for (; it!=it_end; ++it)
    {
		LoopDoCheck(a);

        const ArenaRankListConfig &the_rank_list = *it;

        IRecord *rec = GetArenaRankListRecord(pKernel, the_rank_list.name.c_str());
        if (rec == NULL)
        {
            continue;
        }

        int rec_row_count = rec->GetRows();
        if (rec_row_count == 0)
        {
            continue;
        }
        
        RanklistRecInfo rec_info;
        rec_info.name = the_rank_list.name;
        rec_info.level = the_rank_list.level;
        rec_info.rec = rec;
        
        all_rec.push_back(rec_info);
    }
        
    // ��@all_rec��������, ����Level�Ӹߵ���
    std::sort(all_rec.begin(), all_rec.end(), CompareRanklistLevel);
        
    // ��������
    std::vector<RanklistRecInfo>::iterator it_all_rec = all_rec.begin();
    std::vector<RanklistRecInfo>::iterator it_all_rec_end = all_rec.end();
    
    LoopBeginCheck(b)
    for (; it_all_rec != it_all_rec_end; ++it_all_rec)
    {
        LoopBeginCheck(b)
        
        const RanklistRecInfo& main_rec = *it_all_rec;
        
        int player_uid_col = m_pRankListModule->GetColumnIndexByName("player_uid", main_rec.name.c_str());

        int rec_row_count = main_rec.rec->GetRows();
        
        CVarList temp_row;
        
        LoopBeginCheck(c)
        for (int i=0; i<rec_row_count; ++i)
        {
            LoopBeginCheck(c)
            
            temp_row.Clear();
            main_rec.rec->QueryRowValue(i, temp_row);
            
            std::string main_uid = temp_row.StringVal(player_uid_col);
            
            // ȥ�͵ȼ��ı��������UID������
            
            std::vector<RanklistRecInfo>::iterator it_check_rec = all_rec.begin();
            std::vector<RanklistRecInfo>::iterator it_check_rec_end = all_rec.end();
            
            LoopBeginCheck(d)
            for (; it_check_rec != it_check_rec_end; ++it_check_rec)
            {
                LoopBeginCheck(d)
                
                const RanklistRecInfo& check_rec = *it_check_rec;
                
                if (check_rec.level >= main_rec.level)
                {
                    continue;
                }
                
                int check_row = check_rec.rec->FindString(player_uid_col, main_uid.c_str());
                if (check_row != -1)
                {
					  RankListModule::DeleteFromRankList(pKernel, main_uid.c_str(), check_rec.name.c_str());
                }
                
            }
            
        }// End of row loop
        
    }//end of all_rec loop
    
    return 0;       
}

// �Ƚ����а�ĵȼ�
bool ArenaModule::CompareRanklistLevel(const RanklistRecInfo &a, const RanklistRecInfo &b)
{
    return a.level > b.level;
}

// ���¼�������
void ArenaModule::ReloadConfig(IKernel * pKernel)
{
	m_pArenaModule->LoadResource(pKernel);
}

// ������������ж���
void ArenaModule::FreezeAllObject(IKernel* pKernel, IGameObj* pSelfObj)
{
	if (NULL == pSelfObj)
	{
		return;
	}

	int nGroupId = pSelfObj->GetGroupId();
	CVarList objectlist;
	pKernel->GroupChildList(nGroupId, 0, objectlist);
	int nCount = (int)objectlist.GetCount();
	LoopBeginCheck(a);
	for (int i = 0; i < nCount; ++i)
	{
		LoopDoCheck(a);
		PERSISTID object = objectlist.ObjectVal(i);
		FreezePlayer(pKernel, object);
	}
}

// ���Թ�������Ϣ
int ArenaModule::OnCreateScene(IKernel * pKernel, const PERSISTID & self,
	const PERSISTID & sender, const IVarList & args)
{
	m_pArenaModule->ExportPubRecData(pKernel, 6000, 7000);

// 	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_30_35", 0, 500, 3035);
// 
// 	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_35_40", 0, 259, 3540);
// 	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_40_45", 0, 100, 4045);
// 	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_45_50", 0, 100, 4550);
//	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_50_55", 0, 99, 5055);
//	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_55_60", 0, 99, 5560);
//	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_60_65", 0, 100, 6065);
//	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_65_70", 0, 100, 6570);
//	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_70_75", 0, 99, 7075);
//	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_75_80", 0, 100, 7580);
//	m_pArenaModule->ExportLevelRankData(pKernel, "player_level_rank_list_80_85", 0, 20, 8085);
	return 0;
}

// ����������������
void ArenaModule::ExportPubRecData(IKernel* pKernel, int nStart, int nEnd)
{
	char strFileName[32] = { 0 };
	SPRINTF_S(strFileName, "gm_x_arena_list_%d.lua", nStart);

	std::string path = pKernel->GetResourcePath();
	path += strFileName;

	FILE* fp = Port_FileOpen(path.c_str(), "wb");

	if (NULL == fp)
	{
		return;
	}

//  	char strData[100*1024] = { 0 };
// 	SPRINTF_S(strData, "%s", "local datas_ = {\"pubname\":\"domain\",\"dataname\":\"Domain_RankingList_7440005\",\"recname\":\"abc\",\"data\":{");
	fprintf(fp, "local datas_ = {pubname=\"domain\",dataname=\"Domain_RankingList_100\",recname=\"arena_rank_list\",data={");
	// ��������
	IPubData* pPubData = RankListModule::m_pRankListModule->GetPubData(pKernel);
	if (pPubData == NULL)
	{
		return;
	}

	// �̻��
	IRecord* pRecord = pPubData->GetRecord(ARENA_RANK_LIST);
	if (NULL == pRecord)
	{
		return;
	}

	int nRows = pRecord->GetRows();
	if (nStart < 0 || nEnd > nRows)
	{
		return;
	}

	LoopBeginCheck(a);
	for (int i = nStart; i < nEnd; ++i)
	{
		LoopDoCheck(a);
		fprintf(fp, "{");
		//strcat(strData, "{");

		const char* strUid = pRecord->QueryString(i, 0);
		const char* strAccount = pRecord->QueryString(i, 1);
		const wchar_t* wsName = pRecord->QueryWideStr(i, 2);
		std::string name = StringUtil::WideStrAsString(wsName);
		int nJob = pRecord->QueryInt(i, 3);
		int nSex = pRecord->QueryInt(i, 4);
		int nNation = pRecord->QueryInt(i, 5);
		int nPower = pRecord->QueryInt(i, 6);
		int nLevel = pRecord->QueryInt(i, 7);
		int state = pRecord->QueryInt(i, 8);
		const char* cloth = pRecord->QueryString(i, 9);
		const char* weapon = pRecord->QueryString(i, 10);
		const char* wing = pRecord->QueryString(i, 11);
		const char* resource = pRecord->QueryString(i, 12);
		int flag = pRecord->QueryInt(i, 13);
		int sn = pRecord->QueryInt(i, 14);
		int vip = pRecord->QueryInt(i, 15);

		fprintf(fp, "\"%s\",\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,\"%s\",\"%s\",\"%s\",\"%s\",%d,%d,%d", strUid, strAccount, name.c_str(), nJob, 
					nSex, nNation, nPower, nLevel, state, cloth, weapon, wing, resource, flag, sn, vip);

		fprintf(fp, "},\r\n");
// 		char strTmp[128] = { 0 };
// 		SPRINTF_S(strTmp, "%s,%s,%s,%d,%d,%d,%d,%d,%d,%s,%s,%s,%s,%d,%d,%d", strUid, strAccount, name.c_str()
// 			, nJob, nSex, nNation, nPower, nLevel, state, cloth, weapon, wing, resource, flag, sn, vip);
// 
// 		strcat(strData, strTmp);
// 		strcat(strData, "},/r/n");
	}

	

	fprintf(fp, "}\r\n}\r\nreturn datas_");
//	fprintf(fp, "%s", strData);

	fclose(fp);
}

void ArenaModule::ExportLevelRankData(IKernel* pKernel, const char* strRecName, int nStart, int nEnd,int nNo)
{
	char strFileName[32] = { 0 };
	SPRINTF_S(strFileName, "gm_x_level_list_%d.lua", nNo);

	std::string path = pKernel->GetResourcePath();
	path += strFileName;

	FILE* fp = Port_FileOpen(path.c_str(), "wb");

	if (NULL == fp)
	{
		return;
	}

	//  	char strData[100*1024] = { 0 };
	// 	SPRINTF_S(strData, "%s", "local datas_ = {\"pubname\":\"domain\",\"dataname\":\"Domain_RankingList_7440005\",\"recname\":\"abc\",\"data\":{");
	fprintf(fp, "local datas_ = {pubname=\"domain\",dataname=\"Domain_RankingList_100\",recname=\"%s\",data={", strRecName);
	// ��������
	IPubData* pPubData = RankListModule::m_pRankListModule->GetPubData(pKernel);
	if (pPubData == NULL)
	{
		return;
	}

	// �̻��
	IRecord* pRecord = pPubData->GetRecord(strRecName);
	if (NULL == pRecord)
	{
		return;
	}

	int nRows = pRecord->GetRows();
// 	if (nStart < 0 || nEnd > nRows)
// 	{
// 		return;
// 	}

	LoopBeginCheck(a);
	for (int i = 0; i < nRows; ++i)
	{
		LoopDoCheck(a);
		fprintf(fp, "{");
		//strcat(strData, "{");
		//player_uid, player_account, player_name, player_career, player_sex, player_nation, player_level, player_online_type, player_online, player_vip_level

		const char* strUid = pRecord->QueryString(i, 0);
		const char* strAccount = pRecord->QueryString(i, 1);
		const wchar_t* wsName = pRecord->QueryWideStr(i, 2);
		std::string name = StringUtil::WideStrAsString(wsName);
		int nJob = pRecord->QueryInt(i, 3);
		int nSex = pRecord->QueryInt(i, 4);
		int nNation = pRecord->QueryInt(i, 5);
		int nLevel = pRecord->QueryInt(i, 6);
		int online_type = pRecord->QueryInt(i, 7);
		int online = pRecord->QueryInt(i, 8);
		int vip = pRecord->QueryInt(i, 9);

		fprintf(fp, "\"%s\",\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,%d", strUid, strAccount, name.c_str(), nJob, nSex, nNation, nLevel, online_type, online, vip);

		fprintf(fp, "},\r\n");
		// 		char strTmp[128] = { 0 };
		// 		SPRINTF_S(strTmp, "%s,%s,%s,%d,%d,%d,%d,%d,%d,%s,%s,%s,%s,%d,%d,%d", strUid, strAccount, name.c_str()
		// 			, nJob, nSex, nNation, nPower, nLevel, state, cloth, weapon, wing, resource, flag, sn, vip);
		// 
		// 		strcat(strData, strTmp);
		// 		strcat(strData, "},/r/n");
	}



	fprintf(fp, "}\r\n}\r\nreturn datas_");
	//	fprintf(fp, "%s", strData);

	fclose(fp);
}