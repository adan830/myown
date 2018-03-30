//------------------------------------------------------------------------------
// �ļ���:      SkillAccess.cpp
// ��  ��:      ���ܶ������Է��ʡ�����״̬��ز�ѯ
// ˵  ��:      ����ֻ�ڼ���ϵͳ�ڲ�ʹ�ã��Ҳ�Ҫֱ�Ӷ���SkillAccess��ʵ������ʹ��
//              �䵥��ģʽ�ӿ�SkillAccessSingleton::Instance()
// ��������:    2014��10��27��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#include "FsGame/skillmodule/impl/skillaccess.h"

#include "FsGame/define/CommandDefine.h"
#include "FsGame/define/CoolDownDefine.h"
#include "FsGame/Define/FightPropertyDefine.h"
#include "FsGame/Define/SkillEventFuncIdDefine.h"
#include "utils/extend_func.h"
#include "utils/custom_func.h"
#include "utils/exptree.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
#include "FsGame/SkillModule/impl/SkillConfigData.h"
#include "FsGame/SkillModule/impl/SkillDataQueryModule.h"
#include "FsGame/skillmodule/impl/SkillFlow.h"
#include "FsGame/SkillModule/inner/SkillEventManager.h"
#include "FsGame/SystemFunctionModule/CoolDownModule.h"
#include "FsGame/SkillModule/SkillToSprite.h"
#include <cmath>
#include "FsGame/Define/staticdatadefine.h"
#include "../../Define/SkillDefine.h" 
#include "utils/string_util.h"
#include "../FightActionControl.h"
#include "../../NpcBaseModule/AI/AIDefine.h"

bool SkillAccess::Init(IKernel* pKernel)
{
    return true;
}

// ָ�������Ƿ�������ȴ
bool SkillAccess::IsCoolDown(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSelfObj || NULL == pSkillObj)
    {
        return false;
    }

 	const SkillMainData* pSkillMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
	if (pSkillMain == NULL)
	{
		return false;
	}

	// ����֤֪��ȡ����CD����
	int nCancelCD = 0;
	if(pSkillObj->FindData("TempCancelCD"))
	{
		nCancelCD = pSkillObj->QueryDataInt("TempCancelCD");
	}
	if(nCancelCD > 0)
	{
		return false;
	}

    int iCategory = pSkillMain->GetCoolDownCategory();
    return CoolDownModule::m_pInstance->IsCoolDown(pKernel, self, iCategory);
}

// ��ȡ��������Ŀ��,λ��,����
bool SkillAccess::GetSkillTargetAndPos(IKernel* pKernel,
                                       const PERSISTID& self,
                                       const PERSISTID& skill,
                                       PERSISTID& useskilltarget,
                                       float& dx,
                                       float& dy,
                                       float& dz,
                                       float& doirent)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSelfObj || NULL == pSkillObj)
    {
        return false;
    }

	const SkillMainData* pSkillMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
	if (NULL == pSkillMain)
	{
		return false;
	}

    bool  ret = true;

    float fSelfX, fSelfY, fSelfZ, fSelfOrient;
    pKernel->GetLocation(self, fSelfX, fSelfY, fSelfZ, fSelfOrient);

    // ������Ĭ�ϳ���
    doirent = ::util_dot_angle(fSelfX, fSelfZ, dx, dz);

    // �����ͷ�ѡ������
    ESkillSelectType eSelectType = (ESkillSelectType)pSkillMain->GetSelectType();
    switch (eSelectType)
    {
        case SKILL_SELECT_TYPE_DEFAULT:          // ˲������ѡ��
        {
			// û��λ�Ƶ����Լ���λ��
			if (DISPLACE_NONE_TYPE == pSkillMain->GetDisplaceType())
			{
				if (pSelfObj->GetClassType() == TYPE_PLAYER)
				{
					doirent = fSelfOrient;
				}

				dx = fSelfX;
				dy = fSelfY;
				dz = fSelfZ;
			}
			else
			{
				// ��֤λ�õ���Ч��
				if (!SkillAccessSingleton::Instance()->IsValidDisplace(pKernel, self, pSkillMain->GetID(), DISPLACE_SELF_TYPE, dx, dz))
				{
					return false;
				}
			}
        }
        break;

        case SKILL_SELECT_TYPE_SELECT_OBJECT:    // ����ѡȡ������Ŀ�꣩
        {
			// ������һ��� FIGHTER_CUR_SKILL_TARGET
			if (pKernel->Type(self) == TYPE_PLAYER && 1 == pSelfObj->QueryInt("Online")) 
			{
				//������뿪Ŀ��ᴥ��Ŀ����գ����Բ���lastobject
				useskilltarget = pSelfObj->QueryObject(FIGHTER_CUR_SKILL_TARGET);
			}
			// û��Ŀ��,�ѿͻ���ѡ���Ŀ����Ϊ����Ŀ��
            if (!pKernel->Exists(useskilltarget))
            {
                ret = false;
                break;
            }
            // �ж��Ƕ�������������
            if (pSkillMain->GetIsHitSelf())
            {
                useskilltarget = self;
                doirent = fSelfOrient;
            }
        }
        break;
		case SKILL_SELECT_TYPE_POS:          // ѡȡĳ��λ��
			{
				if (pSkillMain->GetIsHitSelf())
				{
					useskilltarget = self;
				}
			}
			break;
        default:
			{
				const char* szSkillConfigID = pSkillObj->QueryString(SKILL_CONFIGID);
				ret = false;
				::extend_warning(LOG_WARNING,
								 "[SkillAccess::GetSkillTargetAndPos] SKILL %s unknown !", szSkillConfigID);
			}
            break;
    }

    return ret;
}


//��ü��ܵĹ�����ȴʱ��
bool SkillAccess::GetCoolDownTime(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill,
                                  int& iPubCooldowntime, int& iCateCooldowntime, int& iCoolCategory)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSelfObj || NULL == pSkillObj)
    {
        return false;
    }

    const SkillMainData* pSkillMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
    if (NULL == pSkillMain)
    {
        return false;
    }
	// CDΪ�ɳ�����
	int nCoolDownTime = SkillDataQueryModule::m_pInstance->QuerySkillGrowProperty(pKernel, skill, SKILL_GP_COOL_DOWN_TIME);

	// ��ʱȥ��ƫ���,����Ҫ�ӻ���
// 	int nCDTimeDiff = SkillDataQueryModule::m_pInstance->GetSkillTimeDiff(pKernel, self, COOL_DOWN_TIME_DIFF);
// 	int nPublicCDTimeDiff = SkillDataQueryModule::m_pInstance->GetSkillTimeDiff(pKernel, self, PUBLIC_CD_TIME_DIFF);
//     //����ʱ��
//     iPubCooldowntime = __max(pSkillMain->GetPublicCDTime() - nPublicCDTimeDiff, 0);
// 
// 	// ��������Ӱ���CD
// 	int nPSCooldowntime = 0;
// 	CVarList args;
// 	if (SkillDataQueryModule::m_pInstance->QueryPassiveSkillEffect(pKernel, args, self, skill, PS_CHANGE_SKILL_CONFIG, SKILL_CD_TIME_NAME))
// 	{
// 		nPSCooldowntime = args.IntVal(0);
// 	}
	
	//iCateCooldowntime = __max(nCoolDownTime - nCDTimeDiff + nPSCooldowntime, 0);
	
	iCateCooldowntime = nCoolDownTime;
	iPubCooldowntime = pSkillMain->GetPublicCDTime();
    iCoolCategory = pSkillMain->GetCoolDownCategory();

    return true;
}

// ��ȡ�����˺�����ϵ��
void SkillAccess::GetExtraHitDamageRatio(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float& fExtraHitDamageRatio)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSelfObj || NULL == pSkillObj)
    {
        return ;
    }

	fExtraHitDamageRatio = 1.0f;
	const SkillMainData* pSkillMainData = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
	if (NULL != pSkillMainData)
	{
        const StringArray& vExtraHitDamageRatioList = pSkillMainData->GetExtraHitDamage(); 
        const int count = (int)vExtraHitDamageRatioList.GetCount();
        if (count == 0)
        {
            return;
        }
        int nHitIndex = GetHitIndex(pKernel, pSelfObj, pSkillObj);

		// �������е�һ������
		if (nHitIndex < 0)
		{
			return;
		}
		
		// ���δ����������ø���,ȡ���һ������
		nHitIndex = __min(nHitIndex, count - 1);
		fExtraHitDamageRatio = StringUtil::StringAsFloat(vExtraHitDamageRatioList.StringVal(nHitIndex));
	}
}

//��ȡ������ʩ��Ŀ�����ͣ���Ŀ�����Ŀ�����֣�����USESKILL_TYPE_TARGET����USESKILL_TYPE_POSITION��
int SkillAccess::GetUseSkillType(IKernel* pKernel, const PERSISTID& skill)
{
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSkillObj)
    {
        return -1;
    }

	const SkillMainData* pSkillMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
	if (pSkillMain == NULL)
	{
		return -1;
	}
    ESkillSelectType eSelectType = (ESkillSelectType)pSkillMain->GetSelectType();
	int nSkillTargetType = 0;
    switch (eSelectType)
    {
        case SKILL_SELECT_TYPE_SELECT_OBJECT:   // ����ѡȡ������Ŀ�꣩
            nSkillTargetType = USESKILL_TYPE_TARGET;
			break;

        case SKILL_SELECT_TYPE_DEFAULT:         // ˲������ѡ��
            nSkillTargetType = pSkillMain->GetIsHitSelf() ? USESKILL_TYPE_TARGET : USESKILL_TYPE_POSITION;
			break;
		case SKILL_SELECT_TYPE_POS:				// ѡȡĳ��λ��
			nSkillTargetType = pSkillMain->GetIsHitSelf() ? USESKILL_TYPE_TARGET : USESKILL_TYPE_POSITION;
			break;
        default:
			{
				const char* szSkillConfigID = pSkillObj->QueryString(SKILL_CONFIGID);
				::extend_warning(LOG_WARNING,
					 "[SkillAccess::GetUseSkillType] SKILL %s unknown SelectType: %d !", szSkillConfigID, eSelectType);
			}               
            break;
    }

    return nSkillTargetType;
}

// ��֤λ�Ƶ���Ч��
bool SkillAccess::IsValidDisplace(IKernel* pKernel, const PERSISTID& self, const char* strSkillId, int nObjType, float dx, float dz)
{
	bool bValid = false;
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
	// npc����Ҫ��֤
	if (pSelfObj->GetClassType() != TYPE_PLAYER)
	{
		return true;
	}

	// �ҵ���Ӧ�ļ�������
	const SkillMainData* pSkillData = SkillDataQueryModule::m_pInstance->GetSkillMainConfig(strSkillId);
	if (NULL == pSkillData)
	{
		return false;
	}
	// ���ͷ��ߵ�������
	int nMaxDis = DISPLACE_SELF_TYPE == nObjType ? pSkillData->GetDisplaceRange() : pSkillData->GetTargetDisplaceRange();

	float fMaxDis = (float)(nMaxDis * nMaxDis);
	// ����ʩ������Ŀ��ľ���
	float fRealDis2 = util_dot_distance2(dx, dz, pSelfObj->GetPosiX(), pSelfObj->GetPosiZ());
	// �Կͻ���λ�����ݵ���֤
	if (fRealDis2 <= fMaxDis)
	{
		bValid = true;
	}

	return bValid;
}

// ��ѯ���ܵ�λ�����ͺ��ٶ� nObjType��ӦSkillDisplaceObject
bool SkillAccess::QuerySkillDisplaceTypeAndSpeed(IKernel* pKernel, const char* strSkillId, const int nObjType, int& nOutDisplaceType, float& fDisplaceSpeed)
{
	nOutDisplaceType = DISPLACE_NONE_TYPE;
	fDisplaceSpeed = 0.0f;
	// �ҵ���Ӧ�ļ�������
	const SkillMainData* pSkillData = SkillDataQueryModule::m_pInstance->GetSkillMainConfig(strSkillId);
	if (NULL == pSkillData)
	{
		return false;
	}

	if (DISPLACE_SELF_TYPE == nObjType)
	{ 
		nOutDisplaceType = static_cast<SkillDisplaceType>(pSkillData->GetDisplaceType());
		fDisplaceSpeed = pSkillData->GetDisplaceSpeed();
	}
	else
	{
		nOutDisplaceType = static_cast<SkillDisplaceType>(pSkillData->GetTargetDisplaceType());
		fDisplaceSpeed = SKILL_HITBACK_MOTION_SPEED;
	}
	return true;
}

// ��ȡ�����ͷ�ѡ������
ESkillSelectType SkillAccess::GetSkillSelectType(IKernel* pKernel, const PERSISTID& skill)
{
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSkillObj)
    {
        return SKILL_SELECT_TYPE_DEFAULT;
    }

	const SkillMainData* pSkillMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
	if (pSkillMain == NULL)
	{
		return SKILL_SELECT_TYPE_DEFAULT;
	}
    return (ESkillSelectType)pSkillMain->GetSelectType();
}

// ��ȡ������̲���
bool SkillAccess::GetSkillCastRange(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, SkillCastRangeParams& cast_range_params,const SkillMainData* pSkillMain)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSelfObj || NULL == pSkillObj)
    {
        return false;
    }

	if (NULL == pSkillMain)
	{
		pSkillMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
		if (NULL == pSkillMain)
		{
			return false;
		}
	}
	
    cast_range_params.fRangeDistMax = (float)pSkillMain->GetRangeDistMax();        // �����Ч����1
    cast_range_params.fRangeDistMin = (float)pSkillMain->GetRangeDistMin();        // ��С��Ч����0

    if (!pSelfObj->FindData("RangeDistMax"))
    {
        pSelfObj->AddDataFloat("RangeDistMax", 0.0f);
    }
    pSelfObj->SetDataFloat("RangeDistMax", cast_range_params.fRangeDistMax);
    if (!pSelfObj->FindData("RangeDistMin"))
    {
        pSelfObj->AddDataFloat("RangeDistMin", 0.0f);
    }
    pSelfObj->SetDataFloat("RangeDistMin", cast_range_params.fRangeDistMin);
    return true;
}

// ��ȡ�������÷�Χ����
bool SkillAccess::GetSkillHitRange(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, int nHitIndex, SkillHitRangeParams& hit_range_params)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSelfObj || NULL == pSkillObj)
    {
        return false;
    }

	const SkillMainData *pSkillMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
	if (pSkillMain == NULL)
	{
		return false;
	}

	CVarList vHitRangeIDs;
    pSkillMain->GetHitRangeID().ConvertToVarList(vHitRangeIDs);

	// �õ����ܵ����д���
	int nSkillHitCount = pSkillObj->QueryInt(SKILL_HURT_COUNT);

	int nIndex = 0;
	// ������м���,ÿ�����з�Χ��ͬ
	if(nSkillHitCount > 1)
	{
		nIndex = nHitIndex - 1;// ���д�����1����
	}

    if (!GetSkillHitRangeByHitRangeID(pKernel, vHitRangeIDs.StringVal(nIndex), hit_range_params))
    {
        return false;
    }

    return true;
}

// ͨ������id�ҵ���������
const float SkillAccess::GetSkillHitDistance(IKernel* pKernel, const char* szSkillId)
{
	float fDis = 0.0f;
	do 
	{
		if (NULL == szSkillId || StringUtil::CharIsNull(szSkillId))
		{
			break;
		}
		const SkillMainData* pMainData = SkillDataQueryModule::m_pInstance->GetSkillMainConfig(szSkillId);
		if (NULL == pMainData)
		{
			break;
		}

		fDis = pMainData->GetRangeDistMax();
	} while (false);
	
	return fDis;
}

// ��ȡ���ܿ����ж����ϵ
bool SkillAccess::GetSkillHitTargetRelation(IKernel* pKernel, const PERSISTID& skill, int& iTargetRelationType, int& iTargetClassType)
{
    iTargetRelationType = HIT_TARGET_RELATION_TYPE_NONE;
    iTargetClassType = HIT_TARGET_TYPE_NONE;

    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSkillObj)
    {
        return false;
    }

    const SkillMainData* pSkillMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
    if (NULL == pSkillMain)
    {
        return false;
    }

    CVarList vTargetRelations, vTargetClasss;
    pSkillMain->GetHitTargetRelationType().ConvertToVarList(vTargetRelations);
    pSkillMain->GetHitTargetType().ConvertToVarList(vTargetClasss);
    iTargetRelationType = SkillUtilFuncs::GetTargetRelationType(vTargetRelations);
    iTargetClassType = SkillUtilFuncs::GetTargetClassType(vTargetClasss);
    return true;
}

// �Ƿ�ͻ���ѡ��Ŀ������
bool SkillAccess::IsClientHitTarget(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pSkillObj)
{
	if (NULL == pSelfObj || NULL == pSkillObj)
	{
		return false;
	}
	const SkillMainData* pSkillMain = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
	if (NULL == pSkillMain)
	{
		return false;
	}

	// �ɿͻ������еļ���,������Ҳ������д���,�����ɿͻ���֪ͨ����������
	if (pSkillMain->GetIsClientHit() && pSelfObj->GetClassType() == TYPE_PLAYER && pSelfObj->QueryInt("Online") == 1 && !pSelfObj->FindData("RobotPlayer"))
	{
		return true;
	}

	return false;
}

// ��ȡ��ǰ���д���
int SkillAccess::GetHitIndex(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pSkillObj)
{
	if(NULL == pSelfObj || NULL == pSkillObj)
	{
		return 0;
	}
	// �����д�����Ϊ��0����
	int nHitIndex = 0;
	if (IsClientHitTarget(pKernel, pSelfObj, pSkillObj))
	{
		// �ͻ������д�1����
		if (pSkillObj->FindData("ClientHitsIndex"))
		{
			nHitIndex = pSkillObj->QueryDataInt("ClientHitsIndex") - 1;
		}
	}
	else
	{
		nHitIndex = pSkillObj->QueryInt(SKILL_CUR_HITS) - 1;
	}

	return nHitIndex;
}

// ������ֵ
int SkillAccess::ComputeHateValue(IKernel* pKernel, const PERSISTID& skill, int nDamValue)
{
	const SkillMainData* pSkillData = SkillDataQueryModule::m_pInstance->GetSkillMain(pKernel, skill);
	if (NULL == pSkillData)
	{
		return 0;
	}

	int		nHateValue = pSkillData->GetHateValue();
	float	fHateValue = pSkillData->GetHateRate();

	int nAddHateValue = nHateValue + (int)(fHateValue * nDamValue);
	return nAddHateValue;
}

// ͨ����Χid�ҵ���Ӧ�Ĳ���
bool SkillAccess::GetSkillHitRangeByHitRangeID(IKernel* pKernel, const char* szHitRangeID, SkillHitRangeParams& hitRangeParams)
{
	const SkillHitRangeData* pSkillHitRange = SkillDataQueryModule::m_pInstance->GetHitRangePropByID(szHitRangeID);

	if (NULL == pSkillHitRange)
	{
		return false;
	}

	hitRangeParams = pSkillHitRange->GetHitRangeParams();
	return true;
}