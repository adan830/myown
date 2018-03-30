//--------------------------------------------------------------------
// �ļ���:      AIRChaseTargetBase.h
// ��  ��:      ����׷������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AIRChaseTargetBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "utils/util_func.h"
#include <math.h>
#include "FsGame/Interface/FightInterface.h"
#include "FsGame/SystemFunctionModule/MotionModule.h"
#include "FsGame/Define/Fields.h"
#include "FsGame/CommonModule/EnvirValueModule.h"

AIRChaseTargetBase::AIRChaseTargetBase()
{
	m_ruleType = AI_RULE_CHASE_TARGET;
	m_subRuleCode = AIRS_CHASE_TARGET_BASE;
}

//�ǲ����ڼ��ܷ�Χ��
int AIRChaseTargetBase::InSkillRange(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	float dist = pKernel->Distance2D(self, sender);
	//��ײ�뾶
	if (dist < pSelfObj->QueryFloat("MinAttackDist"))
	{
		return AI_RANGE_NEAR;
	}
	else if (dist > pSelfObj->QueryFloat("MaxAttackDist"))
	{
		return AI_RANGE_OVER;
	}
	return AI_RANGE_OK;
}

int AIRChaseTargetBase::DoRule(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args, AITemplateBase &templateWrap)
{
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return AI_RT_IGNORE;
    }

	//�ǲ��ǻ���ս��
	if (pSelf->QueryInt("CurState") != AI_STATE_FIGHT)
	{
		return AI_RT_IGNORE;
	}

	//�����������
 	if ( pSelf->QueryInt("CantMove") > 0)
 	{
 		return AI_RT_IGNORE;
 	}

	//�õ���ǰ����Ŀ��
	PERSISTID curTarget = pSelf->QueryObject("AITargetObejct");
	if (!pKernel->Exists(curTarget))
	{
		return AI_RT_IGNORE;
	}

	if (!IsCanChasePlayer(pKernel, curTarget))
	{
		return AI_RT_IGNORE;
	}


    // �����ͷż���
// 	if (USESKILL_RESULT_SUCCEED != FightInterfaceInstance->IsSkillUseBusy(pKernel, self))
// 	{
// 		// ����ʩ����Ҫ����׷��
// 		if(USESKILL_RESULT_ERR_LEADING_BUSY == FightInterfaceInstance->IsSkillUseBusy(pKernel, self))
// 		{
// 			USOperationState type = USOperationState(FightInterfaceInstance->GetCurUSOperationState(pKernel, self));
// 			if(type == USO_CHANGE_ORIENT)
// 			{
// 				pKernel->Rotate(self, pKernel->Angle(self, curTarget), PI2);
// 			}
// 			else if(type == USO_CAN_MOTION)
// 			{
//                 //�ڼ��ܹ�����׷��
// 				AIFunction::StartMoveToObject(pKernel, self, curTarget, 0, 2);
// 			}
// 		}
// 		return AI_RT_IGNORE;
// 	}
	
	//�Ƿ��ڹ�����Χ��
	int iIn = InSkillRange(pKernel, self, curTarget);

    float runSpeed = pSelf->QueryFloat("RunSpeed");

	if(iIn == AI_RANGE_OK || iIn == AI_RANGE_NEAR)
	{
		//����
        if (!FloatEqual(runSpeed, 0.0f))
        {
		    MotionModule::MotionStop(pKernel, self);
        }
		return AI_RT_SUCCESS;
	}

	//�����ƶ�������
	if (FloatEqual(runSpeed, 0.0f))
	{
		return AI_RT_IGNORE;
	}
	
	float minDistance = pSelf->QueryFloat("MinAttackDist");
	float maxDistance = pSelf->QueryFloat("MaxAttackDist");

	////����̫Զ,����Ŀ��
	if (AIFunction::StartMoveToObject(pKernel, self, curTarget, minDistance, maxDistance) == AI_RT_FAIL)
	{
		return AI_RT_END_FIGHT;    
	}
	return AI_RT_IGNORE;
}

// �Ƿ���׷�����
bool AIRChaseTargetBase::IsCanChasePlayer(IKernel * pKernel, const PERSISTID& target)
{
	IGameObj* pTargetObj = pKernel->GetGameObj(target);
	if (NULL == pTargetObj)
	{
		return false;
	}
	// ������ҿ϶���׷��
	if (pTargetObj->GetClassType() != TYPE_PLAYER)
	{
		return true;
	}

#ifndef FSROOMLOGIC_EXPORTS
	// ���ʹ���Ṧ����׷��
// 	if (FightInterfaceInstance->IsInFlySkill(pKernel, target))
// 	{
// 		return false;
// 	}
#endif // !FSROOMLOGIC_EXPORTS

	return true;
}