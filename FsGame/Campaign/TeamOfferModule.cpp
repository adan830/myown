//--------------------------------------------------------------------
// �ļ���:		TeamOfferModule.cpp
// ��  ��:		�������ģ��
// ˵  ��:		
// ��������:		2017��01��13��
// ������:		tongzt
// �޸���:		 
//--------------------------------------------------------------------

#include "TeamOfferModule.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/CommonModule/ActionMutex.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/NpcBaseModule/NpcCreatorModule.h"

#ifndef FSROOMLOGIC_EXPORTS
#include "FsGame/CommonModule/ReLoadConfigModule.h"
#endif

#include "utils/string_util.h"
#include "utils/XmlFile.h"
#include "utils/extend_func.h"
#include "utils/util_func.h"
#include "utils/custom_func.h"
#include <algorithm>
#include "TaskModule/TaskUtil.h"
#include "CommonModule/EnvirValueModule.h"
#include "SocialSystemModule/TeamModule.h"
#include "CommonModule/CommRuleModule.h"
#include "TaskModule/Data/TaskLoader.h"
#include "SystemFunctionModule/RewardModule.h"


TeamOfferModule *TeamOfferModule::m_pTeamOfferModule = NULL;
std::wstring TeamOfferModule::m_domainName = L"";

// ������Դ
inline int nx_reload_teamoffer_res(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);

	TeamOfferModule::m_pTeamOfferModule->LoadRes(pKernel);

	return 1;
}

// �����������
inline int nx_launch_teamoffer(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	// ����������
	CHECK_ARG_NUM(state, nx_launch_teamoffer, 1);

	// ���ÿ����������
	CHECK_ARG_OBJECT(state, nx_launch_teamoffer, 1);
	PERSISTID player = pKernel->LuaToObject(state, 1);

	TeamOfferModule::m_pTeamOfferModule->Launch(pKernel, player);

	return 1;
}

// ����������͸���
inline int nx_entry_group(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	// ����������
	CHECK_ARG_NUM(state, nx_entry_group, 1);

	// ���ÿ����������
	CHECK_ARG_OBJECT(state, nx_entry_group, 1);
	PERSISTID player = pKernel->LuaToObject(state, 1);
	IGameObj *pPlayer = pKernel->GetGameObj(player);
	if (NULL == pPlayer)
	{
		return 0;
	}

	TeamOfferModule::m_pTeamOfferModule->EntryGroup(pKernel, player, pPlayer->QueryInt(FIELD_PROP_TEAM_ID));

	return 1;
}


// ��ʼ��
bool TeamOfferModule::Init(IKernel* pKernel)
{
	m_pTeamOfferModule = this;
	Assert(m_pTeamOfferModule != NULL);

	// �ͻ�����Ϣ
	pKernel->AddIntCustomHook(CLASS_NAME_PLAYER, CLIENT_CUSTOMMSG_TEAMOFFER, TeamOfferModule::OnCustomMsg);

	// ����ڲ���Ϣ
	pKernel->AddIntCommandHook(CLASS_NAME_PLAYER, COMMAND_TEAMOFFER_MSG, TeamOfferModule::OnCommandMsg);

	// ˢ�����ڲ���Ϣ
	pKernel->AddIntCommandHook(CLASS_NAME_RANDOM_NPC_CREATOR, COMMAND_TEAMOFFER_MSG, TeamOfferModule::OnCreatorCommandMsg);

	// ����
	pKernel->AddEventCallback(CLASS_NAME_PLAYER, "OnRecover", TeamOfferModule::OnPlayerRecover, -1);

	// �뿪����
	pKernel->AddEventCallback(CLASS_NAME_PLAYER, "OnLeaveScene", TeamOfferModule::OnPlayerLeaveScene);

	// ���볡��
	pKernel->AddEventCallback(CLASS_NAME_PLAYER, "OnAfterEntryScene", TeamOfferModule::OnPlayerEntry);

	// �ͻ���׼������
	pKernel->AddEventCallback(CLASS_NAME_PLAYER, "OnReady", TeamOfferModule::OnPlayerReady);

	// �ͻ��˶�������OnReady��Ϣ
	pKernel->AddIntCustomHook(CLASS_NAME_PLAYER, CLIENT_CUSTOMMSG_CONTINUE_ON_READY, TeamOfferModule::OnPlayerReady);

	// ������߻ص�
	pKernel->AddEventCallback(CLASS_NAME_PLAYER, "OnStore", TeamOfferModule::OnStore);

	// ���չ�������Ϣ
	pKernel->AddEventCallback(CLASS_NAME_SCENE, "OnPublicMessage", TeamOfferModule::OnPublicMessage);

	// ���͹�������
	pKernel->AddIntCommandHook(CLASS_NAME_TEAM_OFFER_NPC, COMMAND_BEKILL, TeamOfferModule::OnOfferNpcBeKilled);

	// ������Ȼص�
	pKernel->AddIntCommandHook(CLASS_NAME_PLAYER, COMMAND_TASK_ACTION_MSG, TeamOfferModule::OnCommandTaskProc);

	if (!LoadRes(pKernel))
	{
		Assert(false);
		return false;
	}

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("TeamOfferModule", TeamOfferModule::ReloadTeamofferConfig);
#endif

	DECL_HEARTBEAT(TeamOfferModule::HB_TeamOfferEnd);
	DECL_HEARTBEAT(TeamOfferModule::HB_Contrl);

	DECL_CRITICAL(TeamOfferModule::C_OnTeamChange);

	//TeamModule::AddCheckTeamRule(COMMAND_TEAM_CHECK_TEAMOFFER, TeamOfferModule::CheckAddTeamoffer);

	DECL_LUA_EXT(nx_reload_teamoffer_res);
	DECL_LUA_EXT(nx_launch_teamoffer);
	DECL_LUA_EXT(nx_entry_group);

	return true;
}

// �ر�
bool TeamOfferModule::Shut(IKernel* pKernel)
{
	m_CaptainBoxVec.clear();
	m_OfferAwardRuleVec.clear();

	return true;
}

// ������Դ
bool TeamOfferModule::LoadRes(IKernel* pKernel)
{
	if (!LoadCaptainBox(pKernel))
	{
		return false;
	}

	if (!LoadAwardRule(pKernel))
	{
		return false;
	}

	return true;
}

// ���ضӳ���������
bool TeamOfferModule::LoadCaptainBox(IKernel* pKernel)
{
	m_CaptainBoxVec.clear();

	std::string file_path = pKernel->GetResourcePath();
	file_path += TEAM_OFFER_CAPTAIN_BOX_FILE;

	// xml��ȡ
	CXmlFile xml(file_path.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = file_path;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}

	// ����
	LoopBeginCheck(b);
	for (int i = 0; i < (int)xml.GetSectionCount(); ++i)
	{
		LoopDoCheck(b);
		const char *sec = xml.GetSectionByIndex(i);
		if (StringUtil::CharIsNull(sec))
		{
			continue;
		}

		// �������
		const int offer_num = xml.ReadInteger(sec, "TeamofferNum", 0);

		CaptainBox box(offer_num);
		box.reward_id = xml.ReadInteger(sec, "RewardID", 0);

		m_CaptainBoxVec.push_back(box);
	}

	return true;
}

// ���ؽ�������
bool TeamOfferModule::LoadAwardRule(IKernel* pKernel)
{
	m_OfferAwardRuleVec.clear();

	std::string file_path = pKernel->GetResourcePath();
	file_path += TEAM_OFFER_AWARD_RULE_FILE;

	// xml��ȡ
	CXmlFile xml(file_path.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = file_path;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}

	// ����
	LoopBeginCheck(b);
	for (int i = 0; i < (int)xml.GetSectionCount(); ++i)
	{
		LoopDoCheck(b);
		const char *sec = xml.GetSectionByIndex(i);
		if (StringUtil::CharIsNull(sec))
		{
			continue;
		}

		OfferAwardRule award_rule;

		award_rule.lower_num = xml.ReadInteger(sec, "LowerNum", 0);
		award_rule.lower_level = xml.ReadInteger(sec, "LowerLevel", 0);

		int upper_num = xml.ReadInteger(sec, "UpperNum", 0);
		upper_num = upper_num <= 0 ? INT_MAX : upper_num;
		award_rule.upper_num = max(award_rule.lower_num, upper_num);

		int upper_level = xml.ReadInteger(sec, "UpperLevel", 0);
		upper_level = upper_level <= 0 ? INT_MAX : upper_level;
		award_rule.upper_level = max(award_rule.lower_level, upper_level);

		award_rule.reward_id = xml.ReadInteger(sec, "RewardID", 0);

		m_OfferAwardRuleVec.push_back(award_rule);
	}

	return true;
}

// ȡ�öӳ�����
const int TeamOfferModule::GetCaptainBox(const int offer_num)
{

	//  ����
	CaptainBox temp(offer_num);
	CaptainBoxVec::const_iterator find_it = find(
		m_CaptainBoxVec.begin(),
		m_CaptainBoxVec.end(),
		temp);

	if (find_it != m_CaptainBoxVec.end())
	{
		return find_it->reward_id;
	}

	return 0;
}

// ȡ�ý�������
const OfferAwardRule * TeamOfferModule::GetAwardRule(const int offer_num, const int level)
{
	struct AwardRuleFinder 
	{
		AwardRuleFinder(int _num, int _level) : 
			offer_num(_num), 
			level(_level)
		{
		}
		int offer_num;
		int level;

		bool operator () (const OfferAwardRule& rule) const
		{
			return (offer_num >= rule.lower_num && offer_num <= rule.upper_num)
				&& (level >= rule.lower_level && level <= rule.upper_level);
		}
	};

	OfferAwardRuleVec::const_iterator find_it = find_if(
		m_OfferAwardRuleVec.begin(),
		m_OfferAwardRuleVec.end(),
		AwardRuleFinder(offer_num, level));
	if (find_it != m_OfferAwardRuleVec.end())
	{
		return &(*find_it);
	}

	return NULL;
}

// �ɷ��ȡ��������
bool TeamOfferModule::CanAcceptOfferTask(IKernel* pKernel, const PERSISTID &self)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// δ����
	if (!TaskUtilS::Instance()->IsStart(pKernel, self, TASK_TYPE_TEAMOFFER))
	{		
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_151, CVarList());
		return false;
	}

	// ������������
	if (TaskUtilS::Instance()->IsHaveOfType(pKernel, self, TASK_TYPE_TEAMOFFER))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_153, CVarList());
		return false;
	}

	// ��������
	if (!TaskUtilS::Instance()->IsHaveNumOfType(pKernel, self, TASK_TYPE_TEAMOFFER))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_152, CVarList());
		return false;
	}

	return true;
}

// ��Ա�ɷ�������ͻ
bool TeamOfferModule::CanTeammemberDo(IKernel* pKernel, const PERSISTID &self)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// �����Ա
	CVarList team_mem_list;
	TeamModule::m_pTeamModule->GetTeamMemberList(pKernel, self, team_mem_list);
	const int team_mem_num = (int)team_mem_list.GetCount();	

	// ������������
	if (team_mem_num < EnvirValueModule::EnvirQueryInt(ENV_VALUE_TEAMOFFER_LIMIT_PEOPLE))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_154, CVarList());
		return false;
	}

	// ��Ա�����Ƿ�����
	bool is_suff = true;
	LoopBeginCheck(b);
	for (int j = 0; j < team_mem_num; ++j)
	{
		LoopDoCheck(b);
		const wchar_t *mem_name = team_mem_list.WideStrVal(j);
		if (wcscmp(pSelfObj->GetName(), mem_name) == 0)
		{
			continue;
		}

		// ����
		if (pKernel->GetPlayerScene(mem_name) > 0)
		{
			PERSISTID member = pKernel->FindPlayer(mem_name);
			IGameObj *pMember = pKernel->GetGameObj(member);
			if (NULL == pMember)
			{
				// ��Ա{@0:name}�������ͳ���
				is_suff = false;
				CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_155, CVarList() << mem_name);
				continue;
			}
			else
			{
				// ���ܻ���
				if (ActionMutexModule::IsHaveAction(pKernel, member, false))
				{
					CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_156, CVarList() << mem_name);
					return false;
				}

				// �ȼ�����
				if (!TaskUtilS::Instance()->IsStart(pKernel, member, TASK_TYPE_TEAMOFFER))
				{
					is_suff = false;
					CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_157, CVarList() << mem_name);
					continue;
				}

				// ��������
				if (!TaskUtilS::Instance()->IsHaveNumOfType(pKernel, self, TASK_TYPE_TEAMOFFER))
				{
					is_suff = false;
					CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_158, CVarList() << mem_name);
					continue;
				}
			}
		}
		else
		{	// ����
			is_suff = false;
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_159, CVarList() << mem_name);
		}
	}

	return is_suff;
}

// ��Ա�ɷ�������ͷ���
bool TeamOfferModule::CanTeammemberEntry(IKernel* pKernel, const PERSISTID &self, 
	const int task_id)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// �����Ա
	CVarList team_mem_list;
	TeamModule::m_pTeamModule->GetTeamMemberList(pKernel, self, team_mem_list);
	const int team_mem_num = (int)team_mem_list.GetCount();

	// ������������
	if (team_mem_num < EnvirValueModule::EnvirQueryInt(ENV_VALUE_TEAMOFFER_LIMIT_PEOPLE))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_154, CVarList());
		return false;
	}

	// ��Ա�����Ƿ�����
	bool can_entry = true;
	LoopBeginCheck(b);
	for (int j = 0; j < team_mem_num; ++j)
	{
		LoopDoCheck(b);
		const wchar_t *mem_name = team_mem_list.WideStrVal(j);
		if (wcscmp(pSelfObj->GetName(), mem_name) == 0)
		{
			continue;
		}

		// ����
		if (pKernel->GetPlayerScene(mem_name) > 0)
		{
			PERSISTID member = pKernel->FindPlayer(mem_name);
			IGameObj *pMember = pKernel->GetGameObj(member);
			if (NULL == pMember)
			{
				// ��Ա{@0:name}�������ͳ���
				can_entry = false;
				CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_155, CVarList() << mem_name);
				continue;
			}
			else
			{
				// ���ܻ���
				if (ActionMutexModule::IsHaveAction(pKernel, member, false))
				{
					CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_156, CVarList() << mem_name);
					return false;
				}

				// �ȼ�����
				if (!TaskUtilS::Instance()->IsStart(pKernel, member, TASK_TYPE_TEAMOFFER))
				{
					can_entry = false;
					CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_157, CVarList() << mem_name);
					continue;
				}

				// ��������
				if (!TaskUtilS::Instance()->IsHaveNumOfType(pKernel, self, TASK_TYPE_TEAMOFFER))
				{
					can_entry = false;
					CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_158, CVarList() << mem_name);
					continue;
				}

				// ��ƥ�����������
				if (TaskUtilS::Instance()->QueryTaskStatus(pKernel, member, task_id) != TASK_STATUS_DOING)
				{
					can_entry = false;
					CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_160, CVarList() << mem_name);
					continue;
				}

				// �ڸ�����
				if (pMember->QueryInt(FIELD_PROP_GROUP_ID) > 0)
				{
					can_entry = false;
					CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_161, CVarList() << mem_name);
					continue;
				}
			}
		}
		else
		{	// ����
			can_entry = false;
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_159, CVarList() << mem_name);
		}
	}

	return can_entry;
}

// �Ƿ�ɷ����������
bool TeamOfferModule::CanLaunch(IKernel* pKernel, const PERSISTID &self, 
	bool check_entry_npc/* = true*/)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
	IPubData *pPubData = GetPubData(pKernel);
	if (NULL == pPubData)
	{
		return false;
	}

	// ������ͼ�¼��
	IRecord *pRec = pPubData->GetRecord(TEAMOFFER_REC);
	if (NULL == pRec)
	{
		return false;
	}

	// ���ܻ���
	if (ActionMutexModule::IsHaveAction(pKernel, self))
	{
		return false;
	}

	// �ɷ��ȡ��������
	if (!CanAcceptOfferTask(pKernel, self))
	{
		return false;
	}

	// �Ƕӳ��޷�����
	if (!TeamModule::m_pTeamModule->IsTeamCaptain(pKernel, self))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_162, CVarList());
		return false;
	}	

	// �Ƿ������Npc����
	bool is_around = false;
	const char *entry_npc = EnvirValueModule::EnvirQueryString(ENV_VALUE_TEAMOFFER_ENTRY_NPC);
	if (StringUtil::CharIsNull(entry_npc))
	{
		is_around = true;
	}
	else
	{
		CVarList npc_list;
		pKernel->GetAroundList(self, 15.0f, TYPE_NPC, 0, npc_list);

		LoopBeginCheck(a);
		for (int i = 0; i < (int)npc_list.GetCount(); ++i)
		{
			LoopDoCheck(a);
			IGameObj *pNpc = pKernel->GetGameObj(npc_list.ObjectVal(i));
			if (NULL == pNpc)
			{
				continue;
			}

			if (StringUtil::CharIsEqual(entry_npc, pNpc->GetConfig()))
			{
				is_around = true;
				break;
			}
		}
	}

	// ������������Npc̫Զ
	if (!is_around && check_entry_npc)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_163, CVarList());
		return false;
	}

	// �Ƿ��н����е�����
	const int exist_row = pRec->FindInt(TEAMOFFER_REC_COL_TEAM_ID, pSelfObj->QueryInt(FIELD_PROP_TEAM_ID));
	if (exist_row >= 0)
	{
		if (pRec->QueryInt(exist_row, TEAMOFFER_REC_COL_STATE) == TEAMOFFER_STATE_DOING)
		{
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_164, CVarList());
			return false;
		}
	}
	
	// ��Ա�����Ƿ�����
	if (!CanTeammemberDo(pKernel, self))
	{
		return false;
	}

	return true;
}

// ���ͳ�����֤
bool TeamOfferModule::IsVaildOfferScene(IKernel* pKernel, const PERSISTID &self, 
	const int task_id, const int scene_id)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	// ȡ������
	const TaskBaseData *pData = TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// �����Ƿ�һ��
	return TaskUtilS::Instance()->IsTargetScene(pKernel, pData->scene_list, scene_id);
}

// �������ƽ���ȼ�
const int TeamOfferModule::GetAverageLvl(IKernel* pKernel, const PERSISTID &self)
{

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ����
	const int team_id = pSelfObj->QueryInt(FIELD_PROP_TEAM_ID);
	if (team_id <= 0)
	{
		return 0;
	}

	IRecord *pTeamRec = pSelfObj->GetRecord(TEAM_REC_NAME);
	if (NULL == pTeamRec)
	{
		return 0;
	}

	// ����
	const int rows = pTeamRec->GetRows();
	int sum_level = 0;

	LoopBeginCheck(a);
	for (int i = 0; i < rows; ++i)
	{
		LoopDoCheck(a);
		sum_level += pTeamRec->QueryInt(i, TEAM_REC_COL_BGLEVEL);
	}

	return (int)(sum_level / rows);
}

// ���׼������
void TeamOfferModule::PlayerReady(IKernel* pKernel, const PERSISTID &self, 
	const IVarList& args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	IPubData *pPubData = GetPubData(pKernel);
	if (NULL == pPubData)
	{
		return;
	}

	// ������ͼ�¼��
	IRecord *pRec = pPubData->GetRecord(TEAMOFFER_REC);
	if (NULL == pRec)
	{
		return;
	}

	// �޶���
	const int team_id = pSelfObj->QueryInt(FIELD_PROP_TEAM_ID);
	if (team_id <= 0)
	{
		return;
	}

	// ������
	const int exist_row = pRec->FindInt(TEAMOFFER_REC_COL_TEAM_ID, team_id);
	if (exist_row < 0)
	{
		return;
	}

	// �������ͳ���
	const int task_id = pRec->QueryInt(exist_row, TEAMOFFER_REC_COL_TASK_ID);
	if (!IsVaildOfferScene(pKernel, self, task_id, pKernel->GetSceneId()))
	{
		return;
	}

	// �������͸���
	const int group_id = pRec->QueryInt(exist_row, TEAMOFFER_REC_COL_GROUP_ID);
	if (pSelfObj->QueryInt(FIELD_PROP_GROUP_ID) != group_id)
	{
		return;
	}
	
	// ��ǰ״̬
	const int state = pRec->QueryInt(exist_row, TEAMOFFER_REC_COL_STATE);	
	if (state == TEAMOFFER_STATE_DOING)	// ���н׶�
	{
		// ֪ͨ�ͻ��˵���ʱ
		const int offer_time = EnvirValueModule::EnvirQueryInt(ENV_VALUE_TEAMOFFER_TIME) * 60 * 1000;
		const int64_t state_time = pRec->QueryInt64(exist_row, TEAMOFFER_REC_COL_STATE_TIME);
		const int64_t now_time = util_get_time_64();
		const int64_t diff_time = offer_time - (now_time - state_time);

		// �е���ʱ
		if (diff_time > 0)
		{
			CVarList s2c_msg;
			s2c_msg << SERVER_CUSTOMMSG_TEAMOFFER
					<< S2C_TEAMOFFER_SUB_MSG_COUNTDOWN
					<< (int)diff_time;
			pKernel->Custom(self, s2c_msg);
		}
	}	
}

// �������
void TeamOfferModule::PlayerOnline(IKernel* pKernel, const PERSISTID &self)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// ���Իص�
	ADD_CRITICAL(pKernel, self, FIELD_PROP_TEAM_ID, "TeamOfferModule::C_OnTeamChange");

	// ���
	IPubData *pPubData = TeamOfferModule::m_pTeamOfferModule->GetPubData(pKernel);
	if (NULL == pPubData)
	{
		return;
	}

	// ������ͼ�¼��
	IRecord *pRec = pPubData->GetRecord(TEAMOFFER_REC);
	if (NULL == pRec)
	{
		return;
	}

	// �޶���
	const int team_id = pSelfObj->QueryInt(FIELD_PROP_TEAM_ID);
	if (team_id <= 0)
	{
		// �����������
		TaskUtilS::Instance()->CleanTaskByType(pKernel, self, TASK_TYPE_TEAMOFFER);
		return;
	}

	// ������
	const int exist_row = pRec->FindInt(TEAMOFFER_REC_COL_TEAM_ID, team_id);
	if (exist_row < 0)
	{
		// �����������
		TaskUtilS::Instance()->CleanTaskByType(pKernel, self, TASK_TYPE_TEAMOFFER);
	}	
}

// ��������
void TeamOfferModule::Launch(IKernel* pKernel, const PERSISTID &self,
	bool check_entry_npc/* = true*/)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// ������֤
	if (!CanLaunch(pKernel, self, check_entry_npc))
	{
		return;
	}

	// ����id
	const int team_id = pSelfObj->QueryInt(FIELD_PROP_TEAM_ID);

	// ��������id
	const int task_id = TaskUtilS::Instance()->RandomTask(pKernel, self, TASK_TYPE_TEAMOFFER);

	// ���ͷ�����Ϣ����������
	CVarList s2p_msg;
	s2p_msg << S2P_TEAMOFFER_SUB_MSG_LAUNCH
			<< pSelfObj->GetName()
			<< team_id
			<< task_id
			<< -1
			<< TEAMOFFER_STATE_NONE
			<< PERSISTID();
	SendS2PMsg(pKernel, s2p_msg);
}

// �ӳ���������ͬ��
bool TeamOfferModule::SyncTask(IKernel* pKernel, const PERSISTID &self, 
	const IVarList &args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ����id������id
	const int team_id = args.IntVal(2);
	const int task_id = args.IntVal(3);

	if (pSelfObj->QueryInt(FIELD_PROP_TEAM_ID) != team_id)
	{
		return false;
	}

	if (!TeamModule::m_pTeamModule->IsTeamCaptain(pKernel, self))
	{
		return false;
	}

	// ��������
	if (!TaskUtilS::Instance()->AcceptTask(pKernel, self, task_id))
	{
		return false;
	}

	// �����Ա
	CVarList mem_list;
	TeamModule::m_pTeamModule->GetTeamMemberList(pKernel, self, mem_list);

	// ֪ͨ��Աͬ������
	CVarList s2s_msg;
	s2s_msg << COMMAND_TEAMOFFER_MSG
			<< S2S_TEAMOFFER_SUB_MSG_MEMBER_SYNC_TASK
			<< team_id
			<< task_id;
	
	LoopBeginCheck(a);
	for (int i = 0; i < (int)mem_list.GetCount(); ++i)
	{
		LoopDoCheck(a);
		const wchar_t *name = mem_list.WideStrVal(i);
		if (StringUtil::CharIsNull(name))
		{
			continue;
		}

		if (wcscmp(name, pSelfObj->GetName()) == 0)
		{
			continue;
		}

		pKernel->CommandByName(name, s2s_msg);
	}

	return true;
}

// ��Աͬ������
bool TeamOfferModule::MemberSyncTask(IKernel* pKernel, const PERSISTID &self, const IVarList &args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ����id������id
	const int team_id = args.IntVal(2);
	const int task_id = args.IntVal(3);

	if (pSelfObj->QueryInt(FIELD_PROP_TEAM_ID) != team_id)
	{
		return false;
	}

	if (TeamModule::m_pTeamModule->IsTeamCaptain(pKernel, self))
	{
		return false;
	}

	// ��������
	return TaskUtilS::Instance()->AcceptTask(pKernel, self, task_id);
}

// ɾ����������
void TeamOfferModule::DeleteTask(IKernel* pKernel, const PERSISTID &self)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	IPubData *pPubData = GetPubData(pKernel);
	if (NULL == pPubData)
	{
		return;
	}

	// ������ͼ�¼��
	IRecord *pRec = pPubData->GetRecord(TEAMOFFER_REC);
	if (NULL == pRec)
	{
		return;
	}

	const int team_id = pSelfObj->QueryInt(FIELD_PROP_TEAM_ID);
	const int exist_row = pRec->FindInt(TEAMOFFER_REC_COL_TEAM_ID, team_id);
	if (exist_row < 0)
	{
		TaskUtilS::Instance()->CleanTaskByType(pKernel, self, TASK_TYPE_TEAMOFFER);
		return;
	}

	// �ѽ���
	if (pRec->QueryInt(exist_row, TEAMOFFER_REC_COL_STATE) == TEAMOFFER_STATE_END)
	{
		TaskUtilS::Instance()->CleanTaskByType(pKernel, self, TASK_TYPE_TEAMOFFER);
	}
}

// �ӳ�������븱��
void TeamOfferModule::EntryGroup(IKernel* pKernel, const PERSISTID &self, 
	const int team_id)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	if (!TeamModule::m_pTeamModule->IsInTeam(pKernel, self))
	{
		return;
	}

	IPubData *pPubData = GetPubData(pKernel);
	if (NULL == pPubData)
	{
		return;
	}

	// ������ͼ�¼��
	IRecord *pRec = pPubData->GetRecord(TEAMOFFER_REC);
	if (NULL == pRec)
	{
		return;
	}

	if (pSelfObj->QueryInt(FIELD_PROP_TEAM_ID) != team_id)
	{
		return;
	}

	const int cur_group_id = pSelfObj->QueryInt(FIELD_PROP_GROUP_ID);

	// �쳣
	const int exist_row = pRec->FindInt(TEAMOFFER_REC_COL_TEAM_ID, team_id);
	if (exist_row < 0)
	{
		TaskUtilS::Instance()->CleanTaskByType(pKernel, self, TASK_TYPE_TEAMOFFER);

		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_165, CVarList());
		return;
	}

	CVarList row_value;
	pRec->QueryRowValue(exist_row, row_value);

	const int task_id = row_value.IntVal(TEAMOFFER_REC_COL_TASK_ID);
	const int group_id = row_value.IntVal(TEAMOFFER_REC_COL_GROUP_ID);
	const int state = row_value.IntVal(TEAMOFFER_REC_COL_STATE);
	const int64_t state_time = row_value.Int64Val(TEAMOFFER_REC_COL_STATE_TIME);
	PERSISTID creator = row_value.ObjectVal(TEAMOFFER_REC_COL_CREATOR);

	// �������ͷ�����
	if (group_id > 0 && cur_group_id == group_id)
	{
		return;
	}

	// ���ܻ���
	if (ActionMutexModule::IsHaveAction(pKernel, self))
	{
		return;
	}

	// ���ڸ�����
	if (cur_group_id > 0)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_166, CVarList());
		return;
	}

	// �Ƿ�����������
	if (!TaskUtilS::Instance()->IsHaveOfType(pKernel, self, TASK_TYPE_TEAMOFFER))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_167, CVarList());
		return;
	}

	// ����һ��
	if (TaskUtilS::Instance()->QueryTaskStatus(pKernel, self, task_id) == TASK_STATUS_MAX)
	{
		TaskUtilS::Instance()->AcceptTask(pKernel, self, task_id);
		return;
	}

	// ��������
	if (!IsVaildOfferScene(pKernel, self, task_id, pKernel->GetSceneId()))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_168, CVarList());
		return;
	}

	// �ӳ���һ�β���
	if (TeamModule::m_pTeamModule->IsTeamCaptain(pKernel, self) && group_id < 0)
	{		
		// δ��ʼ�����е�һ�η������
		if (TEAMOFFER_STATE_NONE == state)
		{			
			// ��Ա�����Ƿ�����
			if (!CanTeammemberEntry(pKernel, self, task_id))
			{
				return;
			}

			// ����ƽ���ȼ�
			int average_lvl = GetAverageLvl(pKernel, self);
			average_lvl = average_lvl == 0 ? pSelfObj->QueryInt(FIELD_PROP_LEVEL) : average_lvl;

			const int new_group_id = pKernel->NewGroupId();

			// ����ˢ����
			const PERSISTID creator = NpcCreatorModule::m_pNpcCreatorModule->CreateTaskrCreator(
				pKernel, pKernel->GetScene(), task_id, new_group_id, average_lvl);

			IGameObj *pCreator = pKernel->GetGameObj(creator);
			if (NULL == pCreator)
			{
				return;
			}

			// ����id
			ADD_DATA_INT(pCreator, "OfferTeam");
			pCreator->SetDataInt("OfferTeam", team_id);

			// ���͸�����ʼ֪ͨ
			CVarList s2p_msg;
			s2p_msg << S2P_TEAMOFFER_SUB_MSG_START
					<< team_id
					<< task_id
					<< new_group_id
					<< creator;
			SendS2PMsg(pKernel, s2p_msg);
						
			// �������
			CommRuleModule::ChangeGroup(pKernel, self, GROUP_CHANGE_TYPE_ENTRY, new_group_id);

			// ֪ͨ�ͻ��˵���ʱ
			const int offer_time = EnvirValueModule::EnvirQueryInt(ENV_VALUE_TEAMOFFER_TIME) * 60 * 1000;
			CVarList s2c_msg;
			s2c_msg << SERVER_CUSTOMMSG_TEAMOFFER
					<< S2C_TEAMOFFER_SUB_MSG_COUNTDOWN
					<< offer_time;
			pKernel->Custom(self, s2c_msg);

			// ���뻥��
			ActionMutexModule::NoteAction(pKernel, self, ACTION_TEAMOFFER);

			// ��������
			ADD_HEART_BEAT(pKernel, creator, "TeamOfferModule::HB_Contrl", 1000);
			ADD_COUNT_BEAT(pKernel, creator, "TeamOfferModule::HB_TeamOfferEnd", offer_time, 1);
		}
	}
	else
	{
		// ����δ��ʼ
		if (TEAMOFFER_STATE_NONE == state)
		{
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_169, CVarList());
			return;
		}
		// �����У��ɽ������
		else if (TEAMOFFER_STATE_DOING == state)
		{
			if (group_id > 0)
			{
				CommRuleModule::ChangeGroup(pKernel, self, GROUP_CHANGE_TYPE_ENTRY, group_id);

				// ֪ͨ�ͻ��˵���ʱ
				const int offer_time = EnvirValueModule::EnvirQueryInt(ENV_VALUE_TEAMOFFER_TIME) * 60 * 1000;
				const int64_t now_time = util_get_time_64();
				const int64_t diff_time = offer_time - (now_time - state_time);

				// �е���ʱ
				if (diff_time > 0)
				{
					CVarList s2c_msg;
					s2c_msg << SERVER_CUSTOMMSG_TEAMOFFER
							<< S2C_TEAMOFFER_SUB_MSG_COUNTDOWN
							<< (int)diff_time;
					pKernel->Custom(self, s2c_msg);
				}
				
				// ���뻥��
				ActionMutexModule::NoteAction(pKernel, self, ACTION_TEAMOFFER);
			}
		}
		// ����������
		else if (TEAMOFFER_STATE_END == state)
		{
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_170, CVarList());
			return;
		}
	}	
}

// ��Ա�������
void TeamOfferModule::FollowEntry(IKernel* pKernel, const PERSISTID &self)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	if (!TeamModule::m_pTeamModule->IsInTeam(pKernel, self))
	{
		return;
	}

	// �ӳ��Ƿ���ͬ����
	const wchar_t *captain_name = pSelfObj->QueryWideStr(FIELD_PROP_TEAM_CAPTAIN);
	if (StringUtil::CharIsNull(captain_name))
	{
		return;
	}

	PERSISTID captain = pKernel->FindPlayer(captain_name);
	IGameObj *pCaptain = pKernel->GetGameObj(captain);
	if (NULL == pCaptain)
	{
		return;
	}

	IPubData *pPubData = GetPubData(pKernel);
	if (NULL == pPubData)
	{
		return;
	}

	// ������ͼ�¼��
	IRecord *pRec = pPubData->GetRecord(TEAMOFFER_REC);
	if (NULL == pRec)
	{
		return;
	}

	// �쳣
	const int exist_row = pRec->FindInt(TEAMOFFER_REC_COL_TEAM_ID, pSelfObj->QueryInt(FIELD_PROP_TEAM_ID));
	if (exist_row < 0)
	{
		TaskUtilS::Instance()->CleanTaskByType(pKernel, self, TASK_TYPE_TEAMOFFER);
		return;
	}

	CVarList row_value;
	pRec->QueryRowValue(exist_row, row_value);

	const int task_id = row_value.IntVal(TEAMOFFER_REC_COL_TASK_ID);
	const int group_id = row_value.IntVal(TEAMOFFER_REC_COL_GROUP_ID);
	const int state = row_value.IntVal(TEAMOFFER_REC_COL_STATE);
	const int64_t state_time = row_value.Int64Val(TEAMOFFER_REC_COL_STATE_TIME);
	PERSISTID creator = row_value.ObjectVal(TEAMOFFER_REC_COL_CREATOR);

	// ��������
	if (!IsVaildOfferScene(pKernel, self, task_id, pKernel->GetSceneId()))
	{
		return;
	}

	// �ӳ�δ�ڷ���
	if (pCaptain->QueryInt(FIELD_PROP_GROUP_ID) != group_id)
	{
		return;
	}

	// �������ͷ�����
	const int cur_group_id = pSelfObj->QueryInt(FIELD_PROP_GROUP_ID);	
	if (group_id > 0 && cur_group_id == group_id)
	{
		return;
	}

	// ���ܻ���
	if (ActionMutexModule::IsHaveAction(pKernel, self))
	{
		return;
	}

	// ���ڸ�����
	if (cur_group_id > 0)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_166, CVarList());
		return;
	}

	// ����һ��
	if (TaskUtilS::Instance()->QueryTaskStatus(pKernel, self, task_id) == TASK_STATUS_MAX)
	{
		TaskUtilS::Instance()->AcceptTask(pKernel, self, task_id);
		return;
	}

	// �Ƿ�����������
	if (!TaskUtilS::Instance()->IsHaveOfType(pKernel, self, TASK_TYPE_TEAMOFFER))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_167, CVarList());
		return;
	}

	if (TEAMOFFER_STATE_DOING == state)
	{
		if (group_id > 0)
		{
			CommRuleModule::ChangeGroup(pKernel, self, GROUP_CHANGE_TYPE_ENTRY, group_id);

			// ֪ͨ�ͻ��˵���ʱ
			const int offer_time = EnvirValueModule::EnvirQueryInt(ENV_VALUE_TEAMOFFER_TIME) * 60 * 1000;
			const int64_t now_time = util_get_time_64();
			const int64_t diff_time = offer_time - (now_time - state_time);

			// �е���ʱ
			if (diff_time > 0)
			{
				CVarList s2c_msg;
				s2c_msg << SERVER_CUSTOMMSG_TEAMOFFER
						<< S2C_TEAMOFFER_SUB_MSG_COUNTDOWN
						<< (int)diff_time;
				pKernel->Custom(self, s2c_msg);
			}

			// ���뻥��
			ActionMutexModule::NoteAction(pKernel, self, ACTION_TEAMOFFER);
		}
	}
	// ����������
	else if (TEAMOFFER_STATE_END == state)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_170, CVarList());
		return;
	}	
}

// ������Ϣ����������
void TeamOfferModule::SendS2PMsg(IKernel* pKernel, const IVarList& s2p_sub_msg)
{
	// ���ͷ�����Ϣ����������
	CVarList s2p_msg;
	s2p_msg << PUBSPACE_DOMAIN
			<< TeamOfferModule::GetDomainName(pKernel)
			<< SP_DOMAIN_MSG_TEAMOFFER;
	s2p_msg.Concat(s2p_sub_msg);
	pKernel->SendPublicMessage(s2p_msg);
}

// ȡ�ù���������
IPubData* TeamOfferModule::GetPubData(IKernel* pKernel)
{
	IPubSpace* pDomainSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (pDomainSpace == NULL)
	{
		return NULL;
	}

	IPubData *pPubData = pDomainSpace->GetPubData(GetDomainName(pKernel).c_str());
	
	return pPubData;
}

// �˳�����
void TeamOfferModule::LeaveGroup(IKernel* pKernel, const PERSISTID &self)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// �뿪����
	CVarList s2s_msg;
	s2s_msg << COMMAND_TEAM_MSG
			<< COMMAND_TEAM_LEAVE_MSG;
	pKernel->Command(self, self, s2s_msg);
}

// ������ͽ������
void TeamOfferModule::OnResult(IKernel* pKernel, const IVarList& args)
{
	PERSISTID creator = args.ObjectVal(6);
	if (!pKernel->Exists(creator))
	{
		return;
	}

	CVarList sub_msg;
	sub_msg.Append(args, 4, args.GetCount() - 4);

	// ֪ͨˢ�����������ͽ��
	CVarList s2s_msg;
	s2s_msg << COMMAND_TEAMOFFER_MSG
			<< S2S_TEAMOFFER_SUB_MSG_RESULT;
	s2s_msg.Concat(sub_msg);

	pKernel->Command(pKernel->GetScene(), creator, s2s_msg);
}

// ����������ͽ��
void TeamOfferModule::ProcResult(IKernel* pKernel, const PERSISTID &creator,
	const IVarList& args)
{
	IGameObj *pCreator = pKernel->GetGameObj(creator);
	if (NULL == pCreator)
	{
		return;
	}

	const int result = args.IntVal(2);
	const int team_id = args.IntVal(3);
	const int group_id = pCreator->QueryInt(FIELD_PROP_GROUP_ID);

	CVarList mem_list;
	mem_list.Append(args, 6, args.GetCount() - 6);

	// ����������
	if (mem_list.IsEmpty())
	{
		// �������
		pKernel->ClearGroup(group_id);

		// ֹͣ����
		CVarList s2p_msg;
		s2p_msg << S2P_TEAMOFFER_SUB_MSG_STOP
				<< team_id
				<< 0
				<< group_id;
		SendS2PMsg(pKernel, s2p_msg);

		return;
	}

	// ʧ��
	if (RESULTS_FAILED == result)
	{		
		CVarList s2c_msg;
		s2c_msg << SERVER_CUSTOMMSG_TEAMOFFER
				<< S2C_TEAMOFFER_SUB_MSG_RESULT
				<< result;

		LoopBeginCheck(a);
		for (int i = 0; i < (int)mem_list.GetCount(); ++i)
		{
			LoopDoCheck(a);
			const wchar_t *name = mem_list.WideStrVal(i);
			if (StringUtil::CharIsNull(name))
			{
				continue;
			}

			PERSISTID member = pKernel->FindPlayer(name);
			IGameObj *pMember = pKernel->GetGameObj(member);
			if (NULL == pMember)
			{
				continue;
			}

			// δ����
			if (!TaskUtilS::Instance()->IsStart(pKernel, member, TASK_TYPE_TEAMOFFER))
			{
				continue;
			}

			// �Ƿ�ƥ��
			if (pMember->QueryInt(FIELD_PROP_TEAM_ID) != team_id)
			{
				continue;
			}

			if (pMember->QueryInt(FIELD_PROP_GROUP_ID) != group_id)
			{
				continue;
			}
			
			// ʧ�ܽ���
			pKernel->Custom(member, s2c_msg);

			// �������
			ActionMutexModule::CleanAction(pKernel, member);

			// �г�����
			CommRuleModule::ChangeGroup(pKernel, member, GROUP_CHANGE_TYPE_QUIT, -1);

			// �淨��־-�������
			//GamePlayerActionLog log;
			//log.actionType = LOG_GAME_ACTION_TEAMOFFER;
			//log.actionState = LOG_GAME_ACTION_FAIL;
			//LogModule::m_pLogModule->SaveGameActionLog(pKernel, member, log);

			//// ��¼j������־
			//TeamOfferLog end_log;
			//end_log.offer_level = pCreator->QueryDataInt("CreatorLevel");
			//end_log.offer_num = task_conf.IntVal(QUEST_CONFIG_REC_COMPLETE_COUNT);
			//end_log.opt_type = LOG_GAME_ACTION_TEAMOFFER;
			//end_log.offer_result = result;
			//LogModule::m_pLogModule->OnTeamOfferEnd(pKernel, member, end_log);
		}

		// �������
		pKernel->ClearGroup(group_id);	

		// ��������
		CVarList s2p_msg;
		s2p_msg << S2P_TEAMOFFER_SUB_MSG_RESET
				<< team_id;
		SendS2PMsg(pKernel, s2p_msg);
	}
	else // �������ͳɹ�
	{		
		// ͨ��ʱ��
		const int pass_time = args.IntVal(5);

		// ��ǰ��������
		int team_num = (int)mem_list.GetCount();
		std::map<std::wstring, IGameObj*> playerGroup;
		LoopBeginCheck(b);
		for (int i = 0; i < team_num; ++i)
		{
			LoopDoCheck(b);
			const wchar_t *name = mem_list.WideStrVal(i);
			if (StringUtil::CharIsNull(name))
			{
				continue;
			}

			PERSISTID member = pKernel->FindPlayer(name);
			IGameObj *pMember = pKernel->GetGameObj(member);
			if (NULL == pMember)
			{		
				// ɾ����������	
				pKernel->CommandByName(name, CVarList() << COMMAND_TEAMOFFER_MSG << S2S_TEAMOFFER_SUB_MSG_DELETE_TASK);
				continue;
			}

			// δ����
			if (!TaskUtilS::Instance()->IsStart(pKernel, member, TASK_TYPE_TEAMOFFER))
			{
				continue;
			}

			// ������
			if (pMember->QueryInt(FIELD_PROP_TEAM_ID) != team_id)
			{
				continue;
			}

			// ɾ��
			TaskUtilS::Instance()->CleanTaskByType(pKernel, member, TASK_TYPE_TEAMOFFER);

			// ���ڸ�����
			if (pMember->QueryInt(FIELD_PROP_GROUP_ID) != group_id)
			{
				continue;
			}
			playerGroup[name] = pMember;

			// �����������
			TaskUtilS::Instance()->RecordTaskNum(pKernel, member, TASK_TYPE_TEAMOFFER);

			// ������ȡ
			RewardTeamOffer(pKernel, member, pass_time);

			// �������
			ActionMutexModule::CleanAction(pKernel, member);

			// ���ս��״̬
			pMember->SetInt(FIELD_PROP_FIGHT_STATE, 0);

			// �г�����
			CommRuleModule::ChangeGroup(pKernel, member, GROUP_CHANGE_TYPE_QUIT, -1);

			// �淨��־-�������
			//GamePlayerActionLog log;
			//log.actionType = LOG_GAME_ACTION_TEAMOFFER;
			//log.actionState = LOG_GAME_ACTION_SUCCESS;
			//LogModule::m_pLogModule->SaveGameActionLog(pKernel, member, log);

			//// ��¼������־
			//TeamOfferLog end_log;
			//end_log.offer_level = pCreator->QueryDataInt("CreatorLevel");
			//end_log.offer_num = task_conf.IntVal(QUEST_CONFIG_REC_COMPLETE_COUNT);
			//end_log.pass_time = pass_time;
			//end_log.opt_type = LOG_GAME_ACTION_TEAMOFFER;
			//end_log.offer_result = result;
			//LogModule::m_pLogModule->OnTeamOfferEnd(pKernel, member, end_log);
		}

		// �������
		pKernel->ClearGroup(group_id);

		// �������������
		CVarList s2p_msg;
		s2p_msg << S2P_TEAMOFFER_SUB_MSG_STOP
				<< team_id
				<< 0
				<< group_id;
		SendS2PMsg(pKernel, s2p_msg);

		// ֪ͨ�ӳ��Զ�������һ��
		CVarList s2s_msg;
		s2s_msg << COMMAND_TEAMOFFER_MSG
				<< S2S_TEAMOFFER_SUB_MSG_CONTINUE;

		LoopBeginCheck(c);
		for (int i = 0; i < (int)mem_list.GetCount(); ++i)
		{
			LoopDoCheck(c);
			const wchar_t *name = mem_list.WideStrVal(i);
			if (StringUtil::CharIsNull(name))
			{
				continue;
			}

			pKernel->CommandByName(name, s2s_msg);
		}
	}			
}

// �������
void TeamOfferModule::AddTeam(IKernel* pKernel, const PERSISTID &self)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	IPubData *pPubData = GetPubData(pKernel);
	if (NULL == pPubData)
	{
		return;
	}

	// ������ͼ�¼��
	IRecord *pRec = pPubData->GetRecord(TEAMOFFER_REC);
	if (NULL == pRec)
	{
		return;
	}

	const int team_id = pSelfObj->QueryInt(FIELD_PROP_TEAM_ID);
	const int exist_row = pRec->FindInt(TEAMOFFER_REC_COL_TEAM_ID, team_id);
	if (exist_row < 0)
	{
		return;
	}

	CVarList row_value;
	pRec->QueryRowValue(exist_row, row_value);

	const int task_id = row_value.IntVal(TEAMOFFER_REC_COL_TASK_ID);
	const int group_id = row_value.IntVal(TEAMOFFER_REC_COL_GROUP_ID);

	// ������֤
	if (!CanAcceptOfferTask(pKernel, self))
	{
		return;
	}

	// ��ȡ��������
	if (!TaskUtilS::Instance()->AcceptTask(pKernel, self, task_id))
	{
		return;
	}
}

// �˳�����
void TeamOfferModule::QuitTeam(IKernel* pKernel, const PERSISTID &self, 
	const int old_team_id)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// �����������
	TaskUtilS::Instance()->CleanTaskByType(pKernel, self, TASK_TYPE_TEAMOFFER);

	IPubData *pPubData = GetPubData(pKernel);
	if (NULL == pPubData)
	{
		return;
	}

	// ������ͼ�¼��
	IRecord *pRec = pPubData->GetRecord(TEAMOFFER_REC);
	if (NULL == pRec)
	{
		return;
	}

	const int exist_row = pRec->FindInt(TEAMOFFER_REC_COL_TEAM_ID, old_team_id);
	if (exist_row < 0)
	{
		return;
	}

	CVarList row_value;
	pRec->QueryRowValue(exist_row, row_value);

	const int group_id = row_value.IntVal(TEAMOFFER_REC_COL_GROUP_ID);
	const int state = row_value.IntVal(TEAMOFFER_REC_COL_STATE);	

	// �뿪����
	if (pSelfObj->QueryInt(FIELD_PROP_GROUP_ID) == group_id)
	{
		CommRuleModule::ChangeGroup(pKernel, self, GROUP_CHANGE_TYPE_QUIT, -1);
	}

	// �����������
	if (ActionMutexModule::IsThisAction(pKernel, self, ACTION_TEAMOFFER))
	{
		ActionMutexModule::CleanAction(pKernel, self);
	}

	// δ��ʼʱ����֤�����Ƿ��ɢ
	if (state == TEAMOFFER_STATE_NONE)
	{
		// ���������Ч��֤
		CVarList s2p_msg;
		s2p_msg << S2P_TEAMOFFER_SUB_MSG_CHECK
				<< old_team_id;
		m_pTeamOfferModule->SendS2PMsg(pKernel, s2p_msg);
	}
	
	// �����˳��������
	CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_171, CVarList());
}

// ����������һ������
void TeamOfferModule::ContinueNext(IKernel* pKernel, const PERSISTID &self)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// �Ƕӳ�
	if (!TeamModule::m_pTeamModule->IsTeamCaptain(pKernel, self))
	{
		return;
	}

	// δ����
	if (!TaskUtilS::Instance()->IsStart(pKernel, self, TASK_TYPE_TEAMOFFER))
	{
		return;
	}

	// ������ɴ���
	if (!TaskUtilS::Instance()->IsHaveNumOfType(pKernel, self, TASK_TYPE_TEAMOFFER))
	{
		return;
	}

	// �Ƿ����һ��
	const int offer_num = TaskUtilS::Instance()->QueryTaskNum(pKernel, self, TASK_TYPE_TEAMOFFER);
	if (offer_num % 10 == 0)
	{
		// ���������ѽ����������½�ȡ��һ������
		CVarList s2c_msg;
		s2c_msg << SERVER_CUSTOMMSG_TEAMOFFER
				<< S2C_TEAMOFFER_SUB_MSG_CONTINUE_NEXT
				<< offer_num;
		pKernel->Custom(self, s2c_msg);

		return;
	}

	// ���ͷ���
	m_pTeamOfferModule->Launch(pKernel, self, false);
}

// ��������
void TeamOfferModule::RewardTeamOffer(IKernel* pKernel, const PERSISTID &self, 
	const int pass_time)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// δ����
	if (!TaskUtilS::Instance()->IsStart(pKernel, self, TASK_TYPE_TEAMOFFER))
	{
		return;
	}
	
	// ������ɴ���
	const int offer_num = TaskUtilS::Instance()->QueryTaskNum(pKernel, self, TASK_TYPE_TEAMOFFER);

	// ��ҵȼ�
	const int self_level = pSelfObj->QueryInt(FIELD_PROP_LEVEL);

	CVarList s2c_msg;
	s2c_msg << SERVER_CUSTOMMSG_TEAMOFFER
			<< S2C_TEAMOFFER_SUB_MSG_RESULT
			<< RESULTS_SUCCESS
			<< offer_num
			<< TaskUtilS::Instance()->QueryTaskTotalNum(TASK_TYPE_TEAMOFFER)
			<< pass_time;

	// ���ݴ����͵ȼ�ȡ����
	const OfferAwardRule *award_rule = GetAwardRule(offer_num, self_level);
	if (NULL != award_rule)
	{					
		// ��ȡ���˽���
		RewardModule::m_pRewardInstance->RewardPlayerById(pKernel, self, award_rule->reward_id);

		s2c_msg << award_rule->reward_id;
	}

	// �ӳ�����
	if (TeamModule::m_pTeamModule->IsTeamCaptain(pKernel, self))
	{
		const int reward_id = GetCaptainBox(offer_num);
		if (reward_id != 0)
		{			
			// ��ȡ����
			RewardModule::m_pRewardInstance->RewardPlayerById(pKernel, self, reward_id);
		}
		//pKernel->Command(self,self, CVarList() << COMMAND_FRIEND_MSG << SS_FRIEND_TEAM_ADD_INTIMACY << TEAM_ADD_INTIMACY_OFFER);
	}

	pKernel->Custom(self, s2c_msg);
}

// ���¼���������ͻ����
void TeamOfferModule::ReloadTeamofferConfig(IKernel* pKernel)
{
	TeamOfferModule::m_pTeamOfferModule->LoadRes(pKernel);
}

// �ͻ�����Ϣ
int TeamOfferModule::OnCustomMsg(IKernel* pKernel, const PERSISTID &self, 
							  const PERSISTID & sender, const IVarList & args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ����Ϣ
	const int sub_msg = args.IntVal(1);

	switch (sub_msg)
	{
	case C2S_TEAMOFFER_SUB_MSG_LAUNCH:	// ����һ���������
		{
			m_pTeamOfferModule->Launch(pKernel, self);
		}
		break;
	case C2S_TEAMOFFER_SUB_MSG_ENTRY_GROUP:	// �������ͷ���
		{
			// �ӳ�������
			if (TeamModule::m_pTeamModule->IsTeamCaptain(pKernel, self))
			{
				m_pTeamOfferModule->EntryGroup(pKernel, self, pSelfObj->QueryInt(FIELD_PROP_TEAM_ID));
			}
			else
			{
				m_pTeamOfferModule->FollowEntry(pKernel, self);
			}			
		}
		break;
	case C2S_TEAMOFFER_SUB_MSG_LEAVE:	// �˳��������
		{
			m_pTeamOfferModule->LeaveGroup(pKernel, self);
		}
		break;
	default:
		break;
	}
	return 0;
}

// ����ڲ���Ϣ
int TeamOfferModule::OnCommandMsg(IKernel *pKernel, const PERSISTID &self,
									const PERSISTID &sender, const IVarList &args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ��ȡ����
	int sub_msg = args.IntVal(1);
	
	switch (sub_msg)
	{
	case S2S_TEAMOFFER_SUB_MSG_SYNC_TASK:	// ֪ͨ�ӳ�����ͬ����������
		{
			// ͬ������ʧ��
			if (!m_pTeamOfferModule->SyncTask(pKernel, self, args))
			{
				CVarList s2p_msg;
				s2p_msg << S2P_TEAMOFFER_SUB_MSG_STOP
						<< args.IntVal(2)
						<< args.IntVal(3)
						<< -1;
				m_pTeamOfferModule->SendS2PMsg(pKernel, s2p_msg);
			}
		}		
		break;
	case S2S_TEAMOFFER_SUB_MSG_ENTRY:	// ����������ͷ��鸱��
		{
			m_pTeamOfferModule->EntryGroup(pKernel, self, args.IntVal(2));
		}		
		break;
	case S2S_TEAMOFFER_SUB_MSG_CONTINUE:	// ֪ͨ�Ӷӳ�������һ��
		{
			m_pTeamOfferModule->ContinueNext(pKernel, self);			
		}		
		break;
	case S2S_TEAMOFFER_SUB_MSG_MEMBER_SYNC_TASK:	// ֪ͨ��Աͬ����������
		{
			m_pTeamOfferModule->MemberSyncTask(pKernel, self, args);
		}
		break;
	case S2S_TEAMOFFER_SUB_MSG_DELETE_TASK:	// ֪ͨ��Աɾ���ɵ���������
		{
			m_pTeamOfferModule->DeleteTask(pKernel, self);
		}
		break;
	default:
		break;
	}

	return 0;
}

// ˢ�����ڲ���Ϣ
int TeamOfferModule::OnCreatorCommandMsg(IKernel *pKernel, const PERSISTID &creator,
	const PERSISTID &sender, const IVarList &args)
{
	if (!pKernel->Exists(creator))
	{
		return 0;
	}

	// ��ȡ����
	int sub_msg = args.IntVal(1);
	
	switch (sub_msg)
	{
	case S2S_TEAMOFFER_SUB_MSG_RESULT:	// ������ͽ��
		{
			m_pTeamOfferModule->ProcResult(pKernel, creator, args);
		}		
		break;
	default:
		break;
	}

	return 0;
}

// �������
int TeamOfferModule::OnCommandTaskProc(IKernel *pKernel, const PERSISTID &self, 
	const PERSISTID &sender, const IVarList &args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// �ύ����
	const int sub_msg = args.IntVal(1);
	const int task_id = args.IntVal(2);
	if (TASK_ACTION_SUBMIT != sub_msg)
	{
		return 0;
	}

	IPubData *pPubData = m_pTeamOfferModule->GetPubData(pKernel);
	if (NULL == pPubData)
	{
		return 0;
	}

	// ������ͼ�¼��
	IRecord *pRec = pPubData->GetRecord(TEAMOFFER_REC);
	if (NULL == pRec)
	{
		return 0;
	}

	const int team_id = pSelfObj->QueryInt(FIELD_PROP_TEAM_ID);
	const int exist_row = pRec->FindInt(TEAMOFFER_REC_COL_TEAM_ID, team_id);
	if (exist_row < 0)
	{
		return 0;
	}

	// ���ڽ�����
	if (pRec->QueryInt(exist_row, TEAMOFFER_REC_COL_STATE) != TEAMOFFER_STATE_DOING)
	{
		return 0;
	}	

	// ȡ������
	const TaskBaseData *pData = TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return 0;
	}

	if (pData->type != TASK_TYPE_TEAMOFFER)
	{
		return 0;
	}

	if (task_id != pRec->QueryInt(exist_row, TEAMOFFER_REC_COL_TASK_ID))
	{
		return 0;
	}

	// ������ͳɹ�
	CVarList s2p_msg;
	s2p_msg << S2P_TEAMOFFER_SUB_MSG_RESULT
			<< RESULTS_SUCCESS
			<< team_id;
	m_pTeamOfferModule->SendS2PMsg(pKernel, s2p_msg);

	return 0;
}

// �������Npc��ɱ�ص�
int TeamOfferModule::OnOfferNpcBeKilled(IKernel* pKernel, const PERSISTID& offer_npc,
									  const PERSISTID& killer, const IVarList& args)
{
	IGameObj *pOfferNpc = pKernel->GetGameObj(offer_npc);
	if (NULL == pOfferNpc)
	{
		return 0;
	}

	// ��¼ɱ��NPC��Ϣ
	NpcCreatorModule::m_pNpcCreatorModule->RecordKillNpc(pKernel, offer_npc);

	return 0;
}

// �������
int TeamOfferModule::OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, 
								  const PERSISTID& sender, const IVarList& args)
{
	m_pTeamOfferModule->PlayerOnline(pKernel, self);

	return 0;
}

// ����뿪����
int TeamOfferModule::OnPlayerLeaveScene(IKernel *pKernel, const PERSISTID &self, 
									 const PERSISTID &sender, const IVarList &args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	if (!TeamModule::m_pTeamModule->IsInTeam(pKernel, self))
	{
		return 0;
	}

	IPubData *pPubData = m_pTeamOfferModule->GetPubData(pKernel);
	if (NULL == pPubData)
	{
		return 0;
	}

	// ������ͼ�¼��
	IRecord *pRec = pPubData->GetRecord(TEAMOFFER_REC);
	if (NULL == pRec)
	{
		return 0;
	}

	const int cur_group_id = pSelfObj->QueryInt(FIELD_PROP_GROUP_ID);
	const int team_id = pSelfObj->QueryInt(FIELD_PROP_TEAM_ID);

	// �쳣
	const int exist_row = pRec->FindInt(TEAMOFFER_REC_COL_TEAM_ID, team_id);
	if (exist_row < 0)
	{
		return 0;
	}

	const int group_id = pRec->QueryInt(exist_row, TEAMOFFER_REC_COL_GROUP_ID);

	// �������ͷ�����
	if (group_id > 0 && cur_group_id == group_id)
	{
		// �����������
		if (ActionMutexModule::IsThisAction(pKernel, self, ACTION_TEAMOFFER))
		{
			ActionMutexModule::CleanAction(pKernel, self);
		}

		CommRuleModule::ChangeGroup(pKernel, self, GROUP_CHANGE_TYPE_QUIT, -1);
	}

	return 0;
}

// ���볡��
int TeamOfferModule::OnPlayerEntry(IKernel * pKernel, const PERSISTID & self, 
								const PERSISTID & sender, const IVarList & args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	return 0;
}

// �ͻ���׼������
int TeamOfferModule::OnPlayerReady(IKernel* pKernel, const PERSISTID& self, 
								const PERSISTID& sender, const IVarList& args)
{
	m_pTeamOfferModule->PlayerReady(pKernel, self, args);

	return 0;
}

// ����
int TeamOfferModule::OnStore(IKernel* pKernel, const PERSISTID& self, 
	const PERSISTID& sender, const IVarList& args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	int reason = args.IntVal(0);
	if (reason != STORE_EXIT)
	{
		return 0;
	}

	return 0;
}

// ����仯�ص�
int TeamOfferModule::C_OnTeamChange(IKernel* pKernel, const PERSISTID& self, 
	const char *property, const IVar& old)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ���
	if (pSelfObj->QueryInt(property) <= 0)
	{
		m_pTeamOfferModule->QuitTeam(pKernel, self, old.IntVal());
	}
	else // �������
	{
		m_pTeamOfferModule->AddTeam(pKernel, self);
	}

	return 0;
}

// ������ͽ�������
int TeamOfferModule::HB_TeamOfferEnd(IKernel* pKernel, const PERSISTID& creator, int slice)
{
	IGameObj *pCreator = pKernel->GetGameObj(creator);
	if (NULL == pCreator)
	{
		return 0;
	}

	// ������ͽ��������ʱ�������ʧ�ܣ�
	CVarList s2p_msg;
	s2p_msg << S2P_TEAMOFFER_SUB_MSG_RESULT
			<< RESULTS_FAILED
			<< pCreator->QueryDataInt("OfferTeam");
	m_pTeamOfferModule->SendS2PMsg(pKernel, s2p_msg);

	return 0;
}

// ������͸�����������
int TeamOfferModule::HB_Contrl(IKernel* pKernel, const PERSISTID& creator, int slice)
{
	IGameObj *pCreator = pKernel->GetGameObj(creator);
	if (NULL == pCreator)
	{
		return 0;
	}

	// ���������Ч��֤
	CVarList player_list;
	pKernel->GroupChildList(pCreator->QueryInt(FIELD_PROP_GROUP_ID), TYPE_PLAYER, player_list);
	if (player_list.IsEmpty())
	{
		// ������ͽ����ʧ�ܣ�
		CVarList s2p_msg;
		s2p_msg << S2P_TEAMOFFER_SUB_MSG_RESULT
				<< RESULTS_FAILED
				<< pCreator->QueryDataInt("OfferTeam");
		m_pTeamOfferModule->SendS2PMsg(pKernel, s2p_msg);
	}

	return 0;
}

// ���������ݷ������´�����Ϣ
int TeamOfferModule::OnPublicMessage(IKernel* pKernel, const PERSISTID& self, 
	const PERSISTID& sender, const IVarList& args)
{
	int sub_msg_id = args.IntVal(2);
	if (sub_msg_id != PS_DOMAIN_MSG_TEAMOFFER)
	{
		return 0;
	}

	// ����
	const int opt_type = args.IntVal(3);

	switch (opt_type)
	{
	case P2S_TEAMOFFER_SUB_MSG_SYNC_TASK:	// ֪ͨ�����Աͬ����������
		{
			const wchar_t *capt_name = args.WideStrVal(4);
			if (!StringUtil::CharIsNull(capt_name))
			{
				if (pKernel->GetPlayerScene(capt_name) < 0)
				{
					CVarList s2p_msg;
					s2p_msg << S2P_TEAMOFFER_SUB_MSG_STOP
							<< args.IntVal(5)
							<< args.IntVal(6)
							<< -1;
					m_pTeamOfferModule->SendS2PMsg(pKernel, s2p_msg);
				}
				else
				{
					CVarList s2s_msg;
					s2s_msg << COMMAND_TEAMOFFER_MSG
							<< S2S_TEAMOFFER_SUB_MSG_SYNC_TASK
							<< args.IntVal(5)
							<< args.IntVal(6);
					pKernel->CommandByName(capt_name, s2s_msg);
				}				
			}
		}
		break;
	case P2S_TEAMOFFER_SUB_MSG_ENTRY_GROUP:	// ֪ͨ���������鸱��
		{
			CVarList mem_list;
			mem_list.Append(args, 5, args.GetCount() - 5);
			CVarList s2s_msg;
			s2s_msg << COMMAND_TEAMOFFER_MSG
					<< S2S_TEAMOFFER_SUB_MSG_ENTRY
					<< args.IntVal(4);

			LoopBeginCheck(a);
			for (int i = 0; i < (int)mem_list.GetCount(); ++i)
			{
				LoopDoCheck(a);
				pKernel->CommandByName(mem_list.WideStrVal(i), s2s_msg);
			}			
		}
		break;
	case P2S_TEAMOFFER_SUB_MSG_RESULT:	// ������ͽ��
		m_pTeamOfferModule->OnResult(pKernel, args);
		break;
	default:
		break;
	}

	return 0;
}

const std::wstring & TeamOfferModule::GetDomainName(IKernel * pKernel)
{
	if (m_domainName.empty())
	{
		wchar_t wstr[256];
		const int server_id = pKernel->GetServerId();
		SWPRINTF_S(wstr, L"Domain_Teamoffer_%d", server_id);
		m_domainName = wstr;
	}
	return m_domainName;
}
