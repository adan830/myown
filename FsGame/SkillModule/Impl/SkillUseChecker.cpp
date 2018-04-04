//--------------------------------------------------------------------
// �ļ���:      Server\FsGame\SkillModule\impl\skillUseChecker.h
// ��  ��:      ��鼼��ʹ�õ�����������ʵ�ֺ���ȫ�������ڴ�
// ˵  ��:
// ��������:	2014��10��31��
// ������:		 
//    :       
//--------------------------------------------------------------------
#include "FsGame/SkillModule/impl/SkillUseChecker.h"

#include "FsGame/Define/FightPropertyDefine.h"
#include "FsGame/Define/CoolDownDefine.h"
#include "FsGame/SkillModule/impl/TargetFilter.h"
#include "FsGame/SkillModule/impl/SkillAccess.h"
#include "FsGame/SkillModule/impl/SkillDataQueryModule.h"
#include "FsGame/SystemFunctionModule/CoolDownModule.h"
#include "../../Interface/PropertyInterface.h"
#include "SystemFunctionModule/EnergyModule.h"

//�Ƿ�ɶ�ĳ��������ʹ�øü��ܣ�����1����ȷ��������ʹ�øü���, ����ֵ�Ǵ����룩
int SkillUseChecker::CanUseSkill(IKernel* pKernel, const PERSISTID& self, 
                                 float x, float y, float z, float orient, const PERSISTID& skill,
                                 const PERSISTID& target, float dx, float dy, float dz)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return USESKILL_RESULT_NO_OBJECT;
    }

    // ��鼼���Ƿ��Ѿ�����ȴʱ������
	if(SkillAccessSingleton::Instance()->IsCoolDown(pKernel, self, skill))
	{
		return USESKILL_RESULT_ERR_COOLDOWN;
	}

	// ����Ƿ��ڹ�����ȴ��
	if (CoolDownModule::m_pInstance->IsCoolDown(pKernel, self, COOLDOWN_CATEGOTYID_SKILL))
	{
		return USESKILL_RESULT_ERR_PUBLICCD;
	}

    // ������
    int result = CheckCastRange(pKernel, self, target, skill);
    if (USESKILL_RESULT_SUCCEED != result)
    {
        return result;
    }

	// ��������Ƽ����ͷŵ������ж�
	result = HasPropertyLimit(pKernel, self, target, skill);
    return result;
}

// ��������Ƽ����ͷŵ������ж�
int SkillUseChecker::HasPropertyLimit(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const PERSISTID& skill)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return USESKILL_RESULT_NO_OBJECT;
    }

    IGameObj* pTargetObj = pKernel->GetGameObj(target);
    if (NULL != pTargetObj && self != target)
    {
        if (pTargetObj->FindAttr("CantBeAttack") && pTargetObj->QueryInt("CantBeAttack") > 0)
        {
            return USESKILL_RESULT_ERR_CANTBEATTACK;
        }
    }

	IGameObj* pSkillObj = pKernel->GetGameObj(skill);
	if (pSkillObj == NULL)
	{
		return USESKILL_RESULT_ERR_NOSKILL;
	}

	const SkillMainData* pSkillMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pKernel, skill);
	if (NULL == pSkillMain)
	{
		return USESKILL_RESULT_ERR_CONFIG_DATA;
	}

    //�Ƿ��Ѿ�����
    if (!pSelfObj->FindAttr(FIGHTER_DEAD) || (pSelfObj->QueryInt(FIGHTER_DEAD) != 0))
    {
        //��ʱ����ʹ�ü���
        return USESKILL_RESULT_ERR_DEAD;
    }

    //�������Ե��жϣ��Ƿ����ʹ�ü���
    if (pSelfObj->FindAttr("CantUseSkill") && pSelfObj->QueryInt("CantUseSkill") > 0)
    {
        //�ܷ�ʹ�ü���
        return USESKILL_RESULT_ERR_CANTUSESKILL;
    }

	// �����������
	if (!EnergyModule::IsMPEnough(pKernel, self, pSkillMain->GetConsumeMP()) )
	{
		return USESKILL_RESULT_ERR_MP;
	}

	// ���ڽ�е״̬,������ͨ����
	if (pSelfObj->QueryInt(FIELD_PROP_CANT_NORMAL_ATTACK) > 0 && pSkillMain->GetIsNormalAttack())
	{
		return USESKILL_RESULT_ERR_CANT_NORMAL_ATTACK;
	}

	// ����Ĭ״̬,��Ĭ�в����ͷż���,�Գ�Ĭ��Ч�ļ��ܳ���
	int nSilent = 0;
	if (FPropertyInstance->GetSilent(pSelfObj, nSilent) && nSilent > 0 && !pSkillMain->GetIsUnSilent())
	{
		return USESKILL_RESULT_ERR_SILENT;
	}

	// ����������ܵĴ���
// 	if (pSkillMain->GetIsStorage() && pSkillObj->QueryInt("StorageNum") <= 0)
// 	{
// 		return USESKILL_RESULT_ERR_STORAGE_NUM;
// 	}
// 
// 	// Ӳֱ״̬���
// 	if (!pSkillMain->GetIsUseInStiff() && pSelfObj->QueryInt("StiffState")> 0)
// 	{
// 		return USESKILL_RESULT_ERR_STIFF;
// 	}

    return USESKILL_RESULT_SUCCEED;
}

// ����ж�
int SkillUseChecker::CheckCastRange(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const PERSISTID& skill)
{
    if (!pKernel->Exists(skill))
    {
        return USESKILL_RESULT_ERR_NOSKILL;
    }

    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return USESKILL_RESULT_NO_OBJECT;
    }

    IGameObj* pTargetObj = pKernel->GetGameObj(target);
    if (NULL == pTargetObj)
    {
        return USESKILL_RESULT_SUCCEED;
    }

    // ˲������Ҫ�ж����
    if (SkillAccessSingleton::Instance()->GetSkillSelectType(pKernel, skill) == SKILL_SELECT_TYPE_DEFAULT)
    {
        return USESKILL_RESULT_SUCCEED;
    }
    // ��ȡ��̲����ж����
    SkillCastRangeParams skillCastRangeParams;
    bool ret = SkillAccessSingleton::Instance()->GetSkillCastRange(pKernel, self, skill, skillCastRangeParams, NULL);
    if (!ret)
    {
        return USESKILL_RESULT_ERR_OTHER;
    }

    float x, y, z, orient;
    pKernel->GetLocation(self, x, y, z, orient);
    float dx, dy, dz, dorient;
    pKernel->GetLocation(target, dx, dy, dz, dorient);
    // �ж�Ŀ����Ƿ�����Ч������
    float dist2d = ::util_dot_distance(x, z, dx, dz);
    if (dist2d < skillCastRangeParams.fRangeDistMin)
    {
        return TARGETFILTER_RESULT_ERR_TOONEAR;
    }
    else if (dist2d > skillCastRangeParams.fRangeDistMax)
    {
        return TARGETFILTER_RESULT_ERR_TOOFAR;
    }

    return USESKILL_RESULT_SUCCEED;
}

bool SkillUseChecker::CheckSkillRelation(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, const PERSISTID& target)
{
    if (!pKernel->Exists(self) || !pKernel->Exists(skill))
    {
        return false;
    }

    if (!pKernel->Exists(target))
    {
        return false;
    }

    // ��ȡ��������
    int iTargetRelationType = HIT_TARGET_RELATION_TYPE_NONE;
    int iTargetClassType = HIT_TARGET_TYPE_NONE;
    if (!SkillAccessSingleton::Instance()->GetSkillHitTargetRelation(pKernel, skill, iTargetRelationType, iTargetClassType))
    {
        return false;
    }

    return USESKILL_RESULT_SUCCEED == TargetFilter::SatisfyRelation(pKernel, self, target, iTargetRelationType, iTargetClassType);
}

// �жϼ��ܸ߼�Ч�������ϵ
bool SkillUseChecker::CheckEventEffectRelation(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const char* szSkillEffectPackID)
{
    int iTargetRelationType = HIT_TARGET_RELATION_TYPE_NONE;
    int iTargetClassType = HIT_TARGET_TYPE_NONE;

    if (HIT_TARGET_RELATION_TYPE_NONE != iTargetRelationType && HIT_TARGET_TYPE_NONE != iTargetClassType)
    {
        int iRelationResult = TargetFilter::SatisfyRelation(pKernel, self, target, iTargetRelationType, iTargetClassType);
        if (TARGETFILTER_RESULT_SUCCEED != iRelationResult)
        {
            return false;
        }
    }

    return true;
}
