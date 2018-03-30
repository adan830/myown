//--------------------------------------------------------------------
// �ļ���:      LevelModule.cpp
// ��  ��:      ������ȼ����ģ��
// ˵  ��:
// ��������:    2014��10��17��
// ������:       
//    :       
//--------------------------------------------------------------------
#include "FsGame/CommonModule/LevelModule.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "utils/extend_func.h"
#include "utils/util_func.h"
#include "utils/XmlFile.h"
#include "utils/string_util.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/Define/PlayerBaseDefine.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/Define/RankingDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#ifndef FSROOMLOGIC_EXPORTS
#include "FsGame/SystemFunctionModule/RankListModule.h"
#include "ReLoadConfigModule.h"
#endif
#include "Define/Fields.h"
#include "EnvirValueModule.h"
//#include "Define/BattleAbilityDefine.h"
#include "utils/custom_func.h"
#include "Define/ToolBoxSysInfoDefine.h"
//#include "SkillModule/CriminalSystem.h"
//#include "Define/PKModelDefine.h"

LevelModule* LevelModule::m_pLevelModule = NULL;

int nx_reload_levelup_config(void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);
	// ���ָ����ڣ�����ģ���Ѵ���
	if (NULL != LevelModule::m_pLevelModule)
	{
		LevelModule::m_pLevelModule->LoadResource(pKernel);

//		WorldAverageLevelAddExpSingleton::Instance()->LoadResource(pKernel);
	}

	return 0;
}

//gm ����: /add_exp 100
int nx_add_exp(void* state)
{
    // ��ú���ָ��
    IKernel* pKernel = LuaExtModule::GetKernel(state);
    // ���ָ����ڣ�����ģ���Ѵ���
    if (NULL == LevelModule::m_pLevelModule)
    {
        return 0;
    }

    // ����������
    CHECK_ARG_NUM(state, nx_add_exp, 3);
    // ����������
    CHECK_ARG_OBJECT(state, nx_add_exp, 1);
    CHECK_ARG_INT(state, nx_add_exp, 2);
    CHECK_ARG_INT(state, nx_add_exp, 3);

    // ��ȡ����
    PERSISTID self = pKernel->LuaToObject(state, 1);
    int exp_type = pKernel->LuaToInt(state, 2);
    int exp_amount = pKernel->LuaToInt(state, 3);

    bool add_ret = LevelModule::m_pLevelModule->AddExp(pKernel, self, exp_type, exp_amount);
    if (!add_ret)
    {
        return 0;
    }
    
    return 1;
}

bool LevelModule::Init(IKernel* pKernel)
{
	m_pLevelModule = this;	
	m_nMaxLevel = 0;

	pKernel->AddEventCallback("player", "OnRecover", LevelModule::OnRecover, -2);

	DECL_CRITICAL(LevelModule::C_OnLevelChanged);

	//���������ļ�
	LoadResource(pKernel);
	DECL_LUA_EXT(nx_reload_levelup_config);
    DECL_LUA_EXT(nx_add_exp);

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("LevelConfig", LevelModule::ReloadConfig);
#endif

//	WorldAverageLevelAddExpSingleton::Instance()->Init(pKernel);
	return true;
}

bool LevelModule::Shut(IKernel* pKernel)
{
	return true;
}

// ��ӽ�ɫ�ľ���
bool LevelModule::AddExp(IKernel* pKernel,
						 const PERSISTID& self,
						 int nSrcFrom,
						 int64_t exp)
{
	if ( exp <= 0 )
	{
		return false;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	//// ��������ʱ�����������о�������
	////if ( pSelfObj->QueryInt("Dead") > 0 )
	////{
	////	/*if (nSrcFrom != FUNCTION_EVENT_ID_TASK)
	////	{
	////		return false;
	////	}*/

	////	
	////}

	// Ԥ�����˺Ų�������
	if (pSelfObj->QueryInt("PrepareRole") == ROLE_FLAG_ROBOT)
	{
		return true;
	}

	// �������Ѿ��ﵽ������
	if (pSelfObj->QueryInt("Level") >= m_pLevelModule->GetPlayerMaxLevel(pKernel))
	{
		return false;
	}

	// ���㾭��ֵ �����������
	//��ҵ�ǰ����
	int64_t curExp = pSelfObj->QueryInt64("PlayerExp");

// 	int nAddExp = WorldAverageLevelAddExpSingleton::Instance()->GetCurrentAddExp(pKernel, pSelfObj, (int)exp, nSrcFrom);
// 
// 	// �����Ƿ���Ҫ�����ͷ�˥��
// 	float fRate = 1.0f;
// 	if (CriminalSystemSingleton::Instance()->QueryCiminalRate(pKernel, fRate, self, nSrcFrom, CP_EXP_RATE))
// 	{
// 		nAddExp = (int)((float)nAddExp * fRate);
// 	}

	int64_t nNewExp = curExp + nSrcFrom;
	pSelfObj->SetInt64("PlayerExp", nNewExp);

#ifndef FSROOMLOGIC_EXPORTS
	// ��־��¼
	LogModule::m_pLogModule->OnIncExp(pKernel, self, (int)exp, (int)nNewExp, (int)curExp, nSrcFrom);
#endif

	// ��ֵľ�����ʾ����Ҫ��ʾ��ϵͳ��ʾ��
	CVarList tip_args;
	tip_args << nSrcFrom;
	::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_7016, tip_args);

	//����������
	int curLevel = pSelfObj->QueryInt("Level");
	int64_t upgradeExp = GetLevelUpgradeExp(pKernel, self, curLevel);

	//������˾���ֵ����µļ���
	int newLevel = curLevel;
	LoopBeginCheck(b);
	while (nNewExp >= upgradeExp && upgradeExp > 0)
	{
		LoopDoCheck(b);
		newLevel++;
		nNewExp -= upgradeExp;

		//����ѭ�����洦�����һ�����༶ֻˢһ�����Ե������Ĭ����ֻ����һ����һ��
		if (newLevel > curLevel)
		{
			LevelUp(pKernel, self, newLevel, nNewExp);
		}

		// �����������辭��ֵ
		upgradeExp = GetLevelUpgradeExp(pKernel, self, newLevel);

	}
	return true;
}

// ��ȡ��ɫ�������辭��
int64_t LevelModule::GetLevelUpgradeExp(IKernel* pKernel,
									const PERSISTID& self,
									int level)
{
	// ����
	if (!pKernel->Exists(self))
	{
		return 0;
	}
	// ���ȼ���ֵ�Ƿ�Ϸ�
	if (level <= 0 ||
		level > m_pLevelModule->GetPlayerMaxLevel(pKernel))
	{
		extend_warning(LOG_WARNING, "[%s][%d] invalid level", __FILE__, __LINE__);
		return 0;
	}

	return m_pLevelModule->m_vecExpToUpgrade[level];
}

// ��ȡ������Ϣ
bool LevelModule::LoadResource(IKernel* pKernel)
{
	m_vecExpToUpgrade.clear();

	//��ʼ���������������辭��
	std::string pathName = pKernel->GetResourcePath();
	//�����ļ�·��
	pathName += "ini/SystemFunction/LevelExp/levelexp.xml";

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string err_msg = pathName;
		err_msg.append(" does not exist.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}

	//���ȼ�
	m_nMaxLevel = xmlfile.ReadInteger("MaxLevel", "Exp", 80);

	//�������辭��
	m_vecExpToUpgrade.resize(m_nMaxLevel + 1);

	const int iSectionCount = (int)xmlfile.GetSectionCount();
	LoopBeginCheck(a);
	for (int i = 0; i < iSectionCount; ++i)
	{
		LoopDoCheck(a);
		const char* sectionName = xmlfile.GetSectionByIndex(i);
		if (StringUtil::CharIsNull(sectionName))
		{
			continue;
		}

		int iLevel = ::atoi(sectionName);
		if (iLevel <= 0)
		{
			continue;
		}

		int64_t iLevelExpValue = xmlfile.ReadInt64(sectionName, "Exp", 0);
		if (iLevel <= 0 || iLevel > m_nMaxLevel)
		{
			continue;
		}
		m_vecExpToUpgrade[iLevel] = iLevelExpValue;
	}	
	return true;
}

// ����
bool LevelModule::LevelUp(IKernel* pKernel, const PERSISTID& self, int newlevel, int64_t curExp)
{
	// ����
	if (!pKernel->Exists(self))
	{
		return false;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	int oldlevel = pSelfObj->QueryInt("Level");
	if (oldlevel == newlevel)
	{
		return false;
	}

	//���õ�ǰ����
	pSelfObj->SetInt64("PlayerExp", curExp);

	int64_t upgradeExp = GetLevelUpgradeExp(pKernel, self, newlevel);
	//�������¼���Ҫ�ľ���
	pSelfObj->SetInt64("PlayerUpgradeExp", upgradeExp);

	// ������ߵȼ�
	if ( newlevel > m_pLevelModule->GetPlayerMaxLevel(pKernel) )
	{
		newlevel = m_pLevelModule->GetPlayerMaxLevel(pKernel);
	} 

	pSelfObj->SetInt("Level", newlevel);
#ifndef FSROOMLOGIC_EXPORTS
	m_pLevelModule->SaveLevelUpInfo(pKernel, self, oldlevel, newlevel);
#endif
	return true;
}

//ˢ���������辭��
int LevelModule::OnRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	// ����
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// �ȼ��ص�
	ADD_CRITICAL(pKernel, self, "Level", "LevelModule::C_OnLevelChanged");

	// �½���ɫ
	int nLevel = pSelfObj->QueryInt(FIELD_PROP_LEVEL);
	if (nLevel <= 0)
	{
		nLevel = 1;
		pSelfObj->SetInt(FIELD_PROP_LEVEL, nLevel);
	}
	// ˢ���������辭��
	int64_t iUpgradeExp = m_pLevelModule->GetLevelUpgradeExp(pKernel, self, nLevel);
	pSelfObj->SetInt64("PlayerUpgradeExp", iUpgradeExp);

//	OnCommandComputeKillNpcExpRate(pKernel, self, self, args);
	return 0;
}

//�ȼ��ı�ص�
int LevelModule::C_OnLevelChanged(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old)
{
	// ����
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	int old_level = old.IntVal();
	int new_level = pSelfObj->QueryInt("Level");

	if (old_level <= 0 || new_level <= 0)
	{
		return 0;
	}

	// ��ǰ��ߵȼ�
	int maxLevel = m_pLevelModule->GetPlayerMaxLevel(pKernel);

	// ��¼������Ϣ
	//int saveLevel = new_level > maxLevel ? maxLevel : new_level;
	//if( old_level != saveLevel )
	//{
	//	m_pLevelModule->SaveLevelUpInfo(pKernel, self, old_level, saveLevel);
	//}
	//
	// ������ߵȼ�
	if (new_level > maxLevel)
	{
		pSelfObj->SetInt("Level", maxLevel);
		return 0;
	}

	LoopBeginCheck(c);
	for (int curLevel = old_level; curLevel < new_level; ++curLevel)
	{
		LoopDoCheck(c);
		m_pLevelModule->RefreshLevelUp(pKernel, self, curLevel, curLevel + 1);
	}

	//֪ͨ�ͻ��˲�����Ч
//	pKernel->CustomByKen(self, CVarList() << SERVER_CUSTOMMSG_EFFECT << self << LEVEL_UP);

	CVarList msg;
	msg << COMMAND_LEVELUP_CHANGE << old_level;
	pKernel->Command(self, self, msg);

    //������Ϣ ��ֹ��ҵ�½���ݶ�ʧ
    pKernel->Command(self, self, CVarList() << COMMAND_SAVE_ROLE_INFO);

	// ս��������
//	pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PROPERTY_TYPE);
	return 0;
}

// ˢ�µȼ��仯�������
bool LevelModule::RefreshLevelUp(IKernel* pKernel,
								 const PERSISTID& obj,
								 const int old_level,
								 int curLevel)
{
	// ����
	if (!pKernel->Exists(obj))
	{
		return false;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(obj);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ˢ�½�ɫ����ֵ
	int64_t levelUpgradeExp = pSelfObj->QueryInt64("PlayerUpgradeExp");
	int64_t curLevelUpgradeExp = GetLevelUpgradeExp(pKernel, obj, curLevel);
	if (levelUpgradeExp < curLevelUpgradeExp)
	{
		pSelfObj->SetInt64("PlayerUpgradeExp", curLevelUpgradeExp);
	}
 
	// ��������Ѫ
	pSelfObj->SetInt64("HP", pSelfObj->QueryInt64("MaxHP"));
	return true;
}

// �������Ӿ���ֵ
bool LevelModule::TryAddExp( IKernel* pKernel, const PERSISTID& self, int64_t exp )
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// �ȼ�����
	int iSelfLevel = pSelfObj->QueryInt("Level");	       // ��ҵ�ǰ�ȼ�
	int iMaxLevel = m_pLevelModule->GetPlayerMaxLevel(pKernel);	// ���ȼ�

	// �ѵ����ȼ������ܼӾ���
	if ( iSelfLevel >= iMaxLevel )
	{
		return false;
	}

	// Ҫ��ӵľ���ֵ�Ƿ���Ч
	int64_t iPlayerExp = pSelfObj->QueryInt64("PlayerExp");
	int64_t iNewExp = exp + iPlayerExp;

	//����������
	int64_t iUpgradeExp = m_pLevelModule->GetLevelUpgradeExp(pKernel, self, iSelfLevel);
	int iAddLevel = 0;	// ������
	int iNewLevel = iSelfLevel;	// �µȼ�

	//������˾���ֵ����µļ���
	LoopBeginCheck(d);
	while (iNewExp >= iUpgradeExp && iUpgradeExp > 0)
	{
		LoopDoCheck(d);
		++iAddLevel;
		iNewExp -= iUpgradeExp;
		
		// �����������辭��ֵ
		iNewLevel += iAddLevel;	// �µȼ�
		iUpgradeExp = m_pLevelModule->GetLevelUpgradeExp(pKernel, self, iNewLevel);
	}

	// �µȼ��������ȼ���ﵽ���ȼ�����������ľ���ֵ����0����ʾ�þ���ֵ�������
	if ( iNewLevel > iMaxLevel ) // ����ʱ��ĳ������־���ᱻ��¼
	{
		return false;
	}

	return true;
}

// ȡ�ý�ɫ�ȼ�����ֵ
int LevelModule::GetPlayerMaxLevel(IKernel* pKernel)
{
	if( pKernel == NULL )
	{
		return 0;
	}

	return m_nMaxLevel;
}

// ���������Ϣ��¼
void LevelModule::SaveLevelUpInfo(IKernel* pKernel, const PERSISTID& self, const int oldLevel, const int newLevel)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if( pSelfObj == NULL )
	{
		return;
	}

#ifndef FSROOMLOGIC_EXPORTS
	//��������������־
	RoleUpgradeLog log;
	log.type           = LOG_ROLE_UPGRADE_PLAYER;
	log.levelBefore    = oldLevel;
	log.levelAfter     = newLevel;
	LogModule::m_pLogModule->SaveRoleUpgradeLog(pKernel, self, log);
#endif // FSROOMLOGIC_EXPORTS
}

void LevelModule::ReloadConfig(IKernel* pKernel)
{
	m_pLevelModule->LoadResource(pKernel);
}