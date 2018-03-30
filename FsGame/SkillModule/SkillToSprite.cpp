//--------------------------------------------------------------------
// �ļ���:      SkillToSprite.cpp
// ��  ��:      NPC����ҵļ��ܵ���ӣ�ɾ���Ͳ�ѯ
// ˵  ��:
// ������:       
// ��������:    2014��10��24��
//    :       
//--------------------------------------------------------------------
#include "SkillToSprite.h"
#include "utils/util_func.h"
#include "utils/extend_func.h"
#include "FSGame/define/ServerCustomDefine.h"
#include "FSGame/define/CommandDefine.h"
#include "FsGame/Define/FightPropertyDefine.h"
#include "FsGame/Define/SkillDefine.h"
#include "FsGame/CommonModule/LogModule.h"
#include "Impl/SkillDataQueryModule.h"
#include "Impl/SkillAccess.h"
#include "utils/string_util.h"
#include "Define/Classes.h"

//�Ƿ��Ѿ�ӵ�д˼���
bool SkillToSprite::Contains(IKernel* pKernel,
                             const PERSISTID& self,
                             const char* szSkillConfigID)
{
    if (!pKernel->Exists(self) || NULL == szSkillConfigID || StringUtil::CharIsNull(szSkillConfigID))
    {
        return false;
    }

    return pKernel->Exists(FindSkill(pKernel, self, szSkillConfigID));
}

// ����ĳ��ɫ�����Ƿ���ָ���ļ���
PERSISTID SkillToSprite::FindSkill(IKernel* pKernel,
                                   const PERSISTID& self,
                                   const char* szSkillConfigID)
{
    if (!pKernel->Exists(self) || NULL == szSkillConfigID || StringUtil::CharIsNull(szSkillConfigID))
    {
        return PERSISTID();
    }

    PERSISTID container = GetSkillContainer(pKernel, self);
    if (!pKernel->Exists(container))
    {
        return PERSISTID();
    }

    // ��õ�һ���Ӷ���
    unsigned int it;
    PERSISTID skill = pKernel->GetFirst(container, it);
	LoopBeginCheck(a)
    while (pKernel->Exists(skill))
    {
		LoopDoCheck(a)
        const char* szSkillID = pKernel->GetConfig(skill);
        if (::strcmp(szSkillID, szSkillConfigID) == 0)
        {
            return skill;//�ҵ����һ�µļ�����
        }

        skill = pKernel->GetNext(container, it);
    }

    return PERSISTID();
}


// ��ָ��������������Ӽ���
bool SkillToSprite::AddSkillToContainer(IKernel* pKernel,
                                             const PERSISTID& self,
                                             const PERSISTID& skill,
                                             int pos)
{
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
	 IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj || NULL == pSkillObj)
    {
        return false;
    }

    //�����Ѿ�����
    const char* szSkillConfigID = pSkillObj->QueryString(SKILL_CONFIGID);
    if (Contains(pKernel, self, szSkillConfigID))
    {
        return false;
    }

    PERSISTID skillcontainer = GetSkillContainer(pKernel, self);
    if (!pKernel->Exists(skillcontainer))
    {
        return false;
    }

    //��ָ��λ�÷���
    if (pos > 0)
    {
        if (pKernel->Parent(skill) != skillcontainer || pKernel->GetIndex(skill) != pos)
        {
            if (!pKernel->PlacePos(skill, skillcontainer, pos))
            {
                ::extend_warning(LOG_WARNING,
                    "[SkillToSprite::AddSkill] %s can't PlacePos into skillcontainer.",
                    szSkillConfigID);
                return false;
            }
        }
    }
    else
    {
        if (pKernel->Parent(skill) != skillcontainer)
        {
            if (!pKernel->Place(skill, skillcontainer))
            {
                ::extend_warning(LOG_WARNING,
                    "[SkillToSprite::AddSkill] %s can't Place into skillcontainer.",
                    szSkillConfigID);
                return false;
            }
        }
    }

	pSkillObj->SetInt("Level", 1);
//     if (pKernel->Type(self) == TYPE_PLAYER)
//     {
// #ifndef FSROOMLOGIC_EXPORTS
//         // ��¼��־
//         std::wstring log_info = L"AddSkill: ";
//         std::wstring name = pSelfObj->QueryWideStr("Name");
//         log_info = log_info + name + L" add skill : " + ::util_string_as_widestr(szSkillConfigID);
//         LogModule::m_pLogModule->SavePlayerLog(pKernel, self, LOG_PLAYER_SKILL, log_info.c_str());
// #endif // FSROOMLOGIC_EXPORTS
//     }

    return true;
}

// ɾ��ָ�����������е�ָ������
bool SkillToSprite::RemoveSkillFromContainer(IKernel* pKernel,
                                             const PERSISTID& self,
                                             const PERSISTID& skill)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSelfObj || NULL == pSkillObj)
    {
        return false;
    }

    //�����Լ�ӵ�еļ���
    PERSISTID skillcontainer = GetSkillContainer(pKernel, self);
    PERSISTID skillParentContainer = pKernel->Parent(skill);
    if (!pKernel->Exists(skillcontainer) || !pKernel->Exists(skillParentContainer))
    {
        return false;
    }
    if (skillcontainer != skillParentContainer)
    {
        return false;
    }

//     if (pKernel->Type(self) == TYPE_PLAYER)
//     {
// #ifndef FSROOMLOGIC_EXPORTS
//         // ��¼��־
//         std::wstring log_info = L"RemoveSkill: ";
//         std::wstring name = pSelfObj->QueryWideStr("Name");
//         const char* szSkillConfigID = pSkillObj->QueryString(SKILL_CONFIGID);
//         log_info = log_info + name + L" remove skill : " + ::util_string_as_widestr(szSkillConfigID);
//         LogModule::m_pLogModule->SavePlayerLog(pKernel, self, LOG_PLAYER_SKILL, log_info.c_str());
// #endif // FSROOMLOGIC_EXPORTS
//     }

    //ɾ���ü���
    pKernel->DestroySelf(skill);
    return true;
}

// ��ռ��������еļ���
bool SkillToSprite::ClearAllSkill(IKernel* pKernel, const PERSISTID& self)
{
    PERSISTID container = GetSkillContainer(pKernel, self);
    if (!pKernel->Exists(container))
    {
        return false;
    }

    pKernel->ClearChild(container);

    return true;
}


// ��Ӽ���
bool SkillToSprite::AddSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill)
{
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (!pKernel->Exists(self) || NULL == pSkillObj)
    {
        return false;
    }
 
    const char* szScript = pKernel->GetScript(skill);
	if (strcmp(szScript, AMBER_SKILL_NORMAL) != 0 && strcmp(szScript, AMBER_PASSIVE_SKILL) != 0)
	{
		const char* szSkillConfigID = pSkillObj->QueryString(SKILL_CONFIGID);
		::extend_warning(LOG_ERROR,
			"[SkillToSprite::AddSkill] %s script: %s", szSkillConfigID, szScript);
		return false;
	}

    // ����ִ�м������
    if (!AddSkillToContainer(pKernel, self, skill))
    {
        return false;
    }

    return true;
}

//
bool SkillToSprite::AddSkill(IKernel* pKernel,
                             const PERSISTID& self,
                             const char* szSkillConfigID)
{
    if (!pKernel->Exists(self) || NULL == szSkillConfigID || StringUtil::CharIsNull(szSkillConfigID))
    {
        return false;
    }

    PERSISTID skill = pKernel->CreateFromConfig(pKernel->GetScene(), "", szSkillConfigID);
    if (pKernel->Exists(skill))
    {
        if (AddSkill(pKernel, self, skill))
        {
            return true;
        }
        pKernel->DestroySelf(skill);
    }

    return false;
}

//ɾ������
bool SkillToSprite::RemoveSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill)
{
    if (!pKernel->Exists(self) || !pKernel->Exists(skill))
    {
        return false;
    }

    return RemoveSkillFromContainer(pKernel, self, skill);
}

// ����ҵ�һ��δ��ȴ�ļ���
const PERSISTID SkillToSprite::FindRandomSkillNotInCD(IKernel* pKernel, const PERSISTID& self, float& fSkillDis)
{
	PERSISTID skill = RandomFindSkillId(pKernel, self);
	if (IsSkillValid(pKernel, self, skill, fSkillDis))
	{
		return skill;
	}
		
	PERSISTID container = GetSkillContainer(pKernel, self);
	if (!pKernel->Exists(container))
	{
		return PERSISTID();
	}
	// ��õ�һ���Ӷ���
	unsigned int it;
	skill = pKernel->GetFirst(container, it);
	LoopBeginCheck(b)
	while (pKernel->Exists(skill))
	{
		LoopDoCheck(b)
		// ��֤�����Ƿ���Ч
		if(!IsSkillValid(pKernel, self, skill, fSkillDis))
		{
			skill = pKernel->GetNext(container, it);
			continue;
		}

		if (!SkillAccessSingleton::Instance()->IsCoolDown(pKernel, self, skill))
		{
			break;
		}
		skill = pKernel->GetNext(container, it);
	}

	return skill;
}

// �ж������Ƿ�Ϸ�
bool SkillToSprite::IsSkillValid(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float& fSkillDis)
{
	fSkillDis = 0.0f;
	const SkillMainData* pMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pKernel, skill);
	if (NULL == pMain)
	{
		return false;
	}
	const char* strReadySkillId = pKernel->GetConfig(skill);
	// ��Ѫ���ܲ�������ͷ�
	if (strcmp(strReadySkillId, RESTORE_HP_SKILL_ID) == 0)
	{
		return false;
	}
	fSkillDis = SkillAccessSingleton::Instance()->GetSkillHitDistance(pKernel, strReadySkillId);

	// ����ȴ�����������,���������ȴ�Ļ���˳����һ��δ��ȴ�ļ���
	bool bInCoolDown = SkillAccessSingleton::Instance()->IsCoolDown(pKernel, self, skill);
	
	if(util_float_equal_zero(fSkillDis) || bInCoolDown)
	{
		return false;
	}
	return true;
}

// ������ļ��ܵȼ�������Ϊ1��
// void SkillToSprite::SetSkillLevelMinLevel(IKernel* pKernel, const PERSISTID& self)
// {
// 	PERSISTID container = GetSkillContainer(pKernel, self);
// 	if (!pKernel->Exists(container))
// 	{
// 		return;
// 	}
// 	// ��õ�һ���Ӷ���
// 	unsigned int it;
// 	PERSISTID skill = pKernel->GetFirst(container, it);
// 	LoopBeginCheck(d)
// 	while (pKernel->Exists(skill))
// 	{
// 		LoopDoCheck(d)
// 		IGameObj* pSkillObj = pKernel->GetGameObj(skill);
// 		if (NULL != pSkillObj)
// 		{
// 			pSkillObj->SetInt("Level", 1);
// 		}
// 
// 		skill = pKernel->GetNext(container, it);
// 	}
// }

// ��ȡ����ļ�����������
PERSISTID SkillToSprite::GetSkillContainer(IKernel* pKernel, const PERSISTID& self)
{
    if (!pKernel->Exists(self))
    {
        return PERSISTID();
    }
	
    PERSISTID container = pKernel->GetChild(self, SKILL_CONTAINER_NAME);
    if (!pKernel->Exists(container))
    {
        return PERSISTID();
    }

    return container;
}


// ����ҵ�һ������id
const PERSISTID SkillToSprite::RandomFindSkillId(IKernel* pKernel, const PERSISTID& self)
{
	if (!pKernel->Exists(self))
	{
		return PERSISTID();
	}

	PERSISTID container = GetSkillContainer(pKernel, self);
	if (!pKernel->Exists(container))
	{
		return PERSISTID();
	}
	// ��õ�һ���Ӷ���
	unsigned int it;
	PERSISTID skill = pKernel->GetFirst(container, it);
	int nCount = pKernel->GetChildCount(container);
	int nFindIndex = util_random_int(nCount);
	LoopBeginCheck(c)
	while (pKernel->Exists(skill))
	{
		LoopDoCheck(c)
		if (nFindIndex == it)
		{
			break;
		}

		skill = pKernel->GetNext(container, it);
	}

	return skill;
}
// end of SkillSprite.cpp
