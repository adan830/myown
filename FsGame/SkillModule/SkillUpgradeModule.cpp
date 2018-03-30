//---------------------------------------------------------
//�ļ���:       SkillUpgradeModule.h
//��  ��:       ���ܺͼ�������ģ��
//˵  ��:  
//          
//��������:      2016��4��5��
//������:         ������
//�޸���:
//   :         
//---------------------------------------------------------

#include "SkillUpgradeModule.h"
#include "SkillToSprite.h"
#include "FsGame/Define/Skilldefine.h"
#include "FsGame/CommonModule/ContainerModule.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "utils/XmlFile.h"
#include "utils/custom_func.h"
#include "utils/util_func.h"
#include "utils/extend_func.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/CommonModule/SwitchManagerModule.h"
//#include "FsGame/Define/SkillUpgradeSysInfoDefine.h"
#include "FsGame/SkillModule/SkillToSprite.h"
#include "FsGame/SystemFunctionModule/CapitalModule.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/Define/CommandDefine.h"
#include "Skillmodule.h"
#include "PassiveSkillSystem.h"
#include "utils/string_util.h"
#include "../Define/FightPropertyDefine.h"
#include "../Define/StaticDataDefine.h"
#include "../CommonModule/EnvirValueModule.h"
#include "../Interface/FightInterface.h"
#ifndef FSROOMLOGIC_EXPORTS
#include "FsGame/CommonModule/ReLoadConfigModule.h"
#endif
#include "Define/GameDefine.h"
//#include "Define/BattleAbilityDefine.h"
#define SKILL_UPGRADE_CONFIG_FILE    "ini\\Skill\\SkillUpgradeConfig.xml"
#define SKILL_LEVEL_COST_CONFIG_FILE "ini\\Skill\\SkillLevelCostConfig.xml"
#define NORMAL_SKILL_CONFIG_FILE     "ini\\Skill\\NormalSkillConfig.xml"

SkillUpgradeModule * SkillUpgradeModule::m_pSkillUpgradeModule = NULL;
CapitalModule * SkillUpgradeModule::m_pCapitalModule = NULL;

inline int nx_reload_skill_upgrade_config(void* state)
{
    IKernel* pKernel = LuaExtModule::GetKernel(state);

    if (NULL != SkillUpgradeModule::m_pSkillUpgradeModule)
    {
        SkillUpgradeModule::m_pSkillUpgradeModule->LoadResource(pKernel);
    }

    return 0;
}

// ����ĳ������
int nx_upgrade_skill(void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ����������
	CHECK_ARG_NUM(state, nx_upgrade_skill, 3);
	// ����������
	CHECK_ARG_OBJECT(state, nx_upgrade_skill, 1);
	CHECK_ARG_STRING(state, nx_upgrade_skill, 2);
	CHECK_ARG_INT(state, nx_upgrade_skill, 3);

	// ��ȡ����
	PERSISTID self = pKernel->LuaToObject(state, 1);
	const char* strSkill = pKernel->LuaToString(state, 2);
	int		nType = pKernel->LuaToInt(state, 3);

	SkillUpgradeModule::m_pSkillUpgradeModule->OnUpgradeSkill(pKernel, self, nType, CVarList() << CLIENT_CUSTOMMSG_SKILL_UPGRADE << C2S_ESKILL_UPGRADE_SKILL << strSkill);
	return 1;
}

bool SkillUpgradeModule::Init(IKernel* pKernel)
{
    m_pSkillUpgradeModule = this;
    m_pCapitalModule = (CapitalModule *)pKernel->GetLogicModule("CapitalModule");

    Assert(m_pCapitalModule != NULL);

    pKernel->AddEventCallback("player", "OnRecover", SkillUpgradeModule::OnPlayerRecover);

    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_SKILL_UPGRADE, SkillUpgradeModule::OnSkillSetupMsg);

    LoadResource(pKernel);

    // �������Ա仯�ص�
    DECL_CRITICAL(SkillUpgradeModule::OnPlayerLevelUp);

    DECL_LUA_EXT(nx_reload_skill_upgrade_config);
	DECL_LUA_EXT(nx_upgrade_skill);

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("SkillUpgradeConfig", SkillUpgradeModule::ReloadConfig);
#endif
    return true;
}

bool SkillUpgradeModule::Shut(IKernel *pKernel)
{
    return true;
}

// ������ݻָ�
int SkillUpgradeModule::OnPlayerRecover(IKernel* pKernel, const PERSISTID& player,
                           const PERSISTID& sender, const IVarList& args)
{
    // ���Ա仯�Ļص�
    pKernel->AddCritical(player, "Level", "SkillUpgradeModule::OnPlayerLevelUp");
    
    // Ϊ��ҷ���Ĭ�ϼ���(������û�л�ü��ܵĻ�)

    if (!pKernel->Exists(player))
    {
        return 0;
    }
    
    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return 0;
    }

    // ������ͨ����
    m_pSkillUpgradeModule->SetPlayerNormalSkill(pKernel, player);

	// ����ѧϰ
	m_pSkillUpgradeModule->LearnPlayerSkill(pKernel, player);
    return 0;
}

// �������Կͻ��˵���Ϣ
int SkillUpgradeModule::OnSkillSetupMsg(IKernel *pKernel, const PERSISTID &player,
                           const PERSISTID & sender, const IVarList & args)
{
    int sub_msg_id = args.IntVal(1);

    switch (sub_msg_id)
    {
    case C2S_ESKILL_UPGRADE_SKILL:
        m_pSkillUpgradeModule->OnUpgradeSkill(pKernel, player, UPGRADE_ONE_LEVEL, args);
		break;
	case C2S_ESKILL_UPGRADE_MULTI:
		m_pSkillUpgradeModule->OnUpgradeSkill(pKernel, player, UPGRADE_MULTI_LEVEL, args);
		break;

    default:
       break;
    }

    return 0;
}

// ����������ܵ�����
int SkillUpgradeModule::OnUpgradeSkill(IKernel *pKernel, const PERSISTID &player, int nUpgradeType, const IVarList & args)
{
    /*
        �յ�����Ϣ
            [msg_id][sub_msg_id][skill_id]

        ��Ӧ����Ϣ

            �ɹ�:
                [msg_id][sub_msg_id][1][citta_row_id_in_record][citta_id][citta_level]
            ʧ��:
                [msg_id][sub_msg_id][0][citta_id]
            
     */
    IGameObj *player_obj = pKernel->GetGameObj(player);
    if (player_obj == NULL)
    {
        return 0;
    }

#ifndef FSROOMLOGIC_EXPORTS
	//�����������ܿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_SKILL_UP, player))
	{
		return 0;
	}
#endif

    const char *skill_id = args.StringVal(2);
	if (StringUtil::CharIsNull(skill_id))
	{
		return 0;
	}

	// ����������
	int nOldSkillLevel = 0;
	int nNewSkillLevel = 0;
	if(!m_pSkillUpgradeModule->UpgradeSkillLevel(pKernel, player, skill_id, nUpgradeType, nOldSkillLevel, nNewSkillLevel))
	{
		return 0;
	}

	// ��¼����������־modify by   (2015-04-23)
	RoleUpgradeLog log;
	log.type           = LOG_ROLE_UPGRADE_SKILL;
	log.levelBefore    = nOldSkillLevel;
	log.levelAfter     = nNewSkillLevel;
	log.skillID        = skill_id;
#ifndef FSROOMLOGIC_EXPORTS
	LogModule::m_pLogModule->SaveRoleUpgradeLog(pKernel, player, log);
#endif

	// ս��������
	//pKernel->Command(player, player, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_SKILL_TYPE);
    return 0;
}

// ��ҽ�ɫ����
int SkillUpgradeModule::OnPlayerLevelUp(IKernel* pKernel, const PERSISTID& player,
                                        const char* property, const IVar& old)
{
    m_pSkillUpgradeModule->LearnPlayerSkill(pKernel, player);
	m_pSkillUpgradeModule->SetPlayerNormalSkill(pKernel, player);

	// ս��������
	//pKernel->Command(player, player, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_SKILL_TYPE);
    return 0;
}


// �������������߼�
bool SkillUpgradeModule::UpgradeSkillLevel(IKernel * pKernel, const PERSISTID & player, const char * skill_id, int nUpgradeType, int &nOldSkillLevel, int& nNewSkillLevel) 
{
	IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if(StringUtil::CharIsNull(skill_id) || NULL == pPlayerObj)
	{
		return false;
	} 

	// ���ܵĶ���
	const SkillBaseDef *pSkillDef = GetSkillDef(skill_id);
	if (pSkillDef == NULL)
	{
		return false;
	}

	PERSISTID skill = SKillToSpriteSingleton::Instance()->FindSkill(pKernel, player, skill_id);
	IGameObj* pSkillObj = pKernel->GetGameObj(skill);
	if (NULL == pSkillObj)
	{
		return false;
	}


	// Ĭ��Ϊֻ��һ��
	nOldSkillLevel = pSkillObj->QueryInt("Level");
	nNewSkillLevel = nOldSkillLevel + 1;

	const SkillLevelCostDef * level_cost = GetSkillLevelCost(pSkillDef->seq_no, nNewSkillLevel);
	if (level_cost == NULL)
	{
		// �Ҳ����˵ȼ��ļ۸�
		return false;
	}

	int nCostMoney = 0;
	// ������Ƿ��㹻
	if (!m_pCapitalModule->CanDecCapital(pKernel, player, level_cost->capital_type, level_cost->capital_amount))
	{
		//CVarList unlock_tip_args;
		//::CustomSysInfo(pKernel, player, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_CITTA_UPGRADE_FAILED_GOLD, unlock_tip_args);

		return false;
	}

	// ��ǰ��ͭǮ
	__int64 currentCapital = m_pCapitalModule->GetCapital(pKernel, player, CAPITAL_COPPER);

	// ��ҵȼ�
	int player_level = pPlayerObj->QueryInt("Level");

	// һ����������ߵȼ�
	int nMaxUpgradeLevel = 1;// EnvirValueModule::EnvirQueryInt(FU_SKILL_LEVEL);

	// ��������������ĵȼ���
	int nUpgradeLevel = 0;
	if(player_level >= nOldSkillLevel + nMaxUpgradeLevel)
	{
		nUpgradeLevel = nMaxUpgradeLevel;
	}	
	else
	{
		nUpgradeLevel = player_level - nOldSkillLevel;
	}

	// һ�������ķ�ʽ
	if (UPGRADE_MULTI_LEVEL  == nUpgradeType)
	{
		LoopBeginCheck(p);
		for (int i = nOldSkillLevel + 1;i <= nOldSkillLevel + nUpgradeLevel;++i)
		{
			LoopDoCheck(p);
			level_cost = GetSkillLevelCost(pSkillDef->seq_no, i);
			if (level_cost == NULL)
			{
				// �Ҳ����˵ȼ��ļ۸�
				continue;
			}
			nNewSkillLevel = i;
			nCostMoney += level_cost->capital_amount;
			// �������ͭǮ���ֵ
			if (nCostMoney > currentCapital)
			{
				// �ָ�����һ���������
				--nNewSkillLevel;
				nCostMoney -= level_cost->capital_amount;
				break;
			}
		}
	}
	else
	{
		nCostMoney = level_cost->capital_amount;
	}

	// ���ܵȼ����ܸ�������
	if (nNewSkillLevel > pSkillDef->max_level || nNewSkillLevel > player_level)
	{
		return false;
	}

	// ��ͭǮ
	m_pCapitalModule->DecCapital(pKernel, player, level_cost->capital_type, nCostMoney, FUNCTION_EVENT_ID_UPDATE_SKILL);

	// �������ܵȼ�
	pSkillObj->SetInt("Level", nNewSkillLevel);

	// ���Ӷ�Ӧ�������������
// 	CVarList update_msg;
// 	update_msg << COMMAND_QUEST_COMMAND
// 						  << COMMAND_QUEST_SUB_UPDATE
// 						  << CRITERIA_TYPE_SKILL_UPGRADE
// 						  << (nNewSkillLevel - nOldSkillLevel);
// 	pKernel->Command(player, player, update_msg);
// 
// 	// �����ճ���Ծ
// 	CVarList s2s_msg;
// 	s2s_msg << COMMAND_DAILY_ACTIVITY
// 			<< DAILY_ACTIVITY_TYPE_UPGRADE_SKILL
// 			<< (nNewSkillLevel - nOldSkillLevel);
// 	pKernel->Command(player, player, s2s_msg);

	return true;
}

// ��ȡ��������
const SkillUpgradeModule::SkillBaseDef* SkillUpgradeModule::GetSkillDef(const char *skill_id)
{
	SkillMap::iterator it = m_mapSkillData.find(skill_id);
	if (it != m_mapSkillData.end())
	{
		return &(it->second);
	}
	else
	{
		return NULL;
	}
}

// ������ҵĵȼ���ְҵ����ȡ��ҿ��õ���ͨ����																			  
int SkillUpgradeModule::GetUsableNormalSkill(IKernel *pKernel, const PERSISTID &player, std::vector<std::string> *out_skill)
{
	IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if (pPlayerObj == NULL)
	{
		return 0;
	}

	int player_career = pPlayerObj->QueryInt("Job");
	int player_level = pPlayerObj->QueryInt("Level");

	NormalSkillMap::const_iterator it = m_mapNormalSkill.begin();
	NormalSkillMap::const_iterator it_end = m_mapNormalSkill.end();

	LoopBeginCheck(c);
	for (; it != it_end; ++it)
	{
		LoopDoCheck(c);
		const NormalSkillDef &skill_def = it->second;
		if (skill_def.level_to_unlock > player_level)
		{
			continue;
		}

		if (player_career == skill_def.career_id || PUBLIC_CAREER_SKILL == skill_def.career_id)
		{
			out_skill->push_back(skill_def.id);
		}
	}

	return 0;
}

// ��ȡ������������
const SkillUpgradeModule::SkillLevelCostDef* SkillUpgradeModule::GetSkillLevelCost(int skill_seqno, int level)
{
	char the_key[256] = {0};
	SPRINTF_S(the_key, "%d_%d", skill_seqno, level);

	SkillLevelCostMap::iterator it = m_mapSkillLevelCost.find(the_key);
	if (it == m_mapSkillLevelCost.end())
	{
		return NULL;
	}
	else
	{
		return &(it->second);
	}
}

// ������ҵ���ͨ����
int SkillUpgradeModule::SetPlayerNormalSkill(IKernel *pKernel, const PERSISTID &player)
{
	PERSISTID skill_contaienr = pKernel->GetChild(player, SKILL_CONTAINER_NAME);
	if (!pKernel->Exists(skill_contaienr))
	{
		return 0;
	}

	// ��ҵ���ͨ����
	std::vector<std::string> normal_skill;
	m_pSkillUpgradeModule->GetUsableNormalSkill(pKernel, player, &normal_skill);

	std::vector<std::string>::iterator it = normal_skill.begin();
	std::vector<std::string>::iterator it_end = normal_skill.end();

	LoopBeginCheck(d);
	for (; it!=it_end; ++it)
	{
		LoopDoCheck(d);
		std::string &skill_config_id = *it;
		FightInterfaceInstance->UpdateSkillData(pKernel, player, skill_config_id.c_str(), 1);
	}

	return 0;
}

// ��Ҽ���ѧϰ
int SkillUpgradeModule::LearnPlayerSkill(IKernel *pKernel, const PERSISTID &player)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(player);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ��ȡ��ǰ��ҵȼ�
	int nCurLevel = pSelfObj->QueryInt("Level");

	// ���ְҵ
	int nJob = pSelfObj->QueryInt("Job");
	
	SkillMap::iterator iter = m_mapSkillData.begin();
	LoopBeginCheck(rt);
	for (;iter != m_mapSkillData.end();++iter)
	{
		LoopDoCheck(rt);
		const SkillBaseDef& data = iter->second;
		// ֻѧ��ְҵ�ļ���
		if(data.career_id != nJob)
		{
			continue;;
		}
		PERSISTID skill = SKillToSpriteSingleton::Instance()->FindSkill(pKernel, player, data.id.c_str());
		if (!skill.IsNull())
		{
			// �˼�������Ѿ�ӵ��
			continue;
		}

		// Ĭ��Ϊϵͳ������������Ӽ���
		if (nCurLevel >= data.level_to_unlock)
		{
			// �����Ҽ���
			FightInterfaceInstance->UpdateSkillData(pKernel, player, data.id.c_str(), 1);

			// ֪ͨ�ͻ���
			CVarList msg_to_client;
			msg_to_client << SERVER_CUSTOMMSG_SKILL_UPGRADE;
			msg_to_client << S2C_ESKILL_SETUP_SKILL_UNLOCKED;
			msg_to_client << data.id.c_str();

			pKernel->Custom(player, msg_to_client);
		}
	}

	return 0;
}

// ����������Դ
bool SkillUpgradeModule::LoadResource(IKernel* pKernel)
{
	m_mapSkillLevelCost.clear();
	m_mapSkillData.clear();
	m_mapNormalSkill.clear();

	if (!LoadSkill(pKernel))
	{
		return false;
	}

	if (!LoadSkillLevelCost(pKernel))
	{
		return false;
	}

	if (!LoadNormalSkill(pKernel))
	{
		return false;
	}

	return true;
}

// ���뼼������
bool SkillUpgradeModule::LoadSkill(IKernel* pKernel)
{
    std::string res_path = pKernel->GetResourcePath();
    std::string skill_config_path = res_path;
    skill_config_path.append(SKILL_UPGRADE_CONFIG_FILE);

    CXmlFile xml(skill_config_path.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = skill_config_path;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    int sec_count = (int)sec_list.GetCount();
	LoopBeginCheck(l);
    for (int i=0; i<sec_count; i++)
    {
		LoopDoCheck(l);
        const char *sec = sec_list.StringVal(i);

        SkillBaseDef skill_def;
        skill_def.id = sec;
        skill_def.career_id = xml.ReadInteger(sec, "CareerID", 0);
        skill_def.seq_no = xml.ReadInteger(sec, "SeqNo", -1);
        skill_def.level_to_unlock = xml.ReadInteger(sec, "LevelToUnlock", 0);
        skill_def.max_level = xml.ReadInteger(sec, "MaxLevel", 15);

		if (m_mapSkillData.find(skill_def.id) == m_mapSkillData.end())
		{
			m_mapSkillData.insert(make_pair(skill_def.id, skill_def));
		}
    }
    return true;
}

// ���뼼�ܵȼ��۸�
bool SkillUpgradeModule::LoadSkillLevelCost(IKernel* pKernel)
{
	std::string res_path = pKernel->GetResourcePath();
	std::string skill_config_path = res_path;
	skill_config_path.append(SKILL_LEVEL_COST_CONFIG_FILE);

	CXmlFile xml(skill_config_path.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = skill_config_path;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}

	CVarList sec_list;
	xml.GetSectionList(sec_list);

	int sec_count = (int)sec_list.GetCount();
	LoopBeginCheck(m);
	for (int i=0; i<sec_count; i++)
	{
		LoopDoCheck(m);
		const char *sec = sec_list.StringVal(i);

		SkillLevelCostDef level_cost_def;
		level_cost_def.skill_seq_no = xml.ReadInteger(sec, "SkillSeqNo", -1);
		level_cost_def.skill_level = xml.ReadInteger(sec, "Level", -1);
		level_cost_def.capital_type = xml.ReadInteger(sec, "CapitalType", -1);
		level_cost_def.capital_amount = xml.ReadInteger(sec, "CapitalAmount", -1);

		if (level_cost_def.skill_seq_no == -1 || level_cost_def.skill_level == -1
			|| level_cost_def.capital_type == -1 || level_cost_def.capital_amount == -1)
		{
			std::string err_msg = skill_config_path;
			err_msg.append(" invalid config.");
			::extend_warning(LOG_ERROR, err_msg.c_str());

			continue;
		}

		char the_key[256] = {0};
		SPRINTF_S(the_key, "%d_%d", level_cost_def.skill_seq_no, level_cost_def.skill_level);

		SkillLevelCostMap::iterator it = m_mapSkillLevelCost.find(the_key);
		if (it == m_mapSkillLevelCost.end())
		{
			m_mapSkillLevelCost.insert(SkillLevelCostMap::value_type(the_key, level_cost_def));
		}
	}
	return true;
}

//��������ͨ����
bool SkillUpgradeModule::LoadNormalSkill(IKernel* pKernel)
{
	 /*
        <Property ID="10110a" CareerID="1" CittaLevelToUnlock="0" /> 
    */

    std::string res_path = pKernel->GetResourcePath();
    std::string skill_config_path = res_path;
    skill_config_path.append(NORMAL_SKILL_CONFIG_FILE);

    CXmlFile xml(skill_config_path.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = skill_config_path;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    int sec_count = (int)sec_list.GetCount();
	LoopBeginCheck(o)
    for (int i=0; i<sec_count; i++)
    {
		LoopDoCheck(o)
        const char *sec = sec_list.StringVal(i);

        NormalSkillDef skill_def;
        skill_def.id = sec;
        skill_def.career_id = xml.ReadInteger(sec, "CareerID", 0);
        skill_def.level_to_unlock = xml.ReadInteger(sec, "LevelToUnlock", 0);

		NormalSkillMap::iterator it = m_mapNormalSkill.find(skill_def.id);
		NormalSkillMap::iterator it_end = m_mapNormalSkill.end();

		if (it == it_end)
		{
			m_mapNormalSkill.insert(NormalSkillMap::value_type(skill_def.id, skill_def));
		}
    }

    return true;
}

void SkillUpgradeModule::ReloadConfig(IKernel* pKernel)
{
	m_pSkillUpgradeModule->LoadResource(pKernel);
}