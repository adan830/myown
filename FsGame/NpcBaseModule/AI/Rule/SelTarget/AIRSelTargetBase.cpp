//--------------------------------------------------------------------
// �ļ���:      AIRSelTargetBase.cpp
// ��  ��:      ��������Ŀ�����
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AIRSelTargetBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "FsGame/NpcBaseModule/AI/AIDefine.h"
#include "FsGame/Define/Fields.h"
#include "utils/string_util.h"

AIRSelTargetBase::AIRSelTargetBase()
{
	m_ruleType = AI_RULE_SEL_TARTGET;
	m_subRuleCode = AIRS_SEL_TARGET_BASE;
}


int AIRSelTargetBase::DoRule(IKernel * pKernel,const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase &templateWrap)
{
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return AI_RT_NULL;
    }

    IRecord *pAttackerList = pSelf->GetRecord("AttackerList");
    if(pAttackerList == NULL)
    {
        return AI_RT_NULL;
    }
    
	//���׷����Χ
	float distance2 = pSelf->QueryFloat("ChaseRange") * pSelf->QueryFloat("ChaseRange");
	
	//������
	float fBornX = pSelf->QueryFloat("BornX");
	float fBornZ = pSelf->QueryFloat("BornZ");

	PERSISTID CurAITargetObject = pSelf->QueryObject("AITargetObejct");
    IGameObj *pCurAITargetObject = NULL;
    if(pKernel->Exists(CurAITargetObject))
    {
        pCurAITargetObject = pKernel->GetGameObj(CurAITargetObject);
    }

    //��ǰ����Ŀ�����˺�
    int iCurDamageValue = 0;
    if(pCurAITargetObject != NULL && pCurAITargetObject->QueryInt("Dead") != 1)
    {
		// ����ͬһ����
		if (pCurAITargetObject->QueryInt(FIELD_PROP_GROUP_ID) != pSelf->QueryInt(FIELD_PROP_GROUP_ID))
		{
			CurAITargetObject = PERSISTID();
		}

		// ����޵�
		if (pCurAITargetObject->GetClassType() == TYPE_PLAYER 
			&& pCurAITargetObject->QueryInt("CantBeAttack") == 1 
			/*&& !StringUtil::CharIsNull(pCurAITargetObject->QueryString(FIELD_PROP_TASK_RESOURCE))*/)
		{
			CurAITargetObject = PERSISTID();
		}
		else
		{
			int iAITargetRow = pAttackerList->FindObject(AI_ATTACKER_REC_OBJECT, CurAITargetObject);
			if (iAITargetRow >= 0)
			{
				iCurDamageValue = pAttackerList->QueryInt(iAITargetRow, AI_ATTACKER_REC_DAMAGE);
			}

			//����ڰ�ȫ���Ͳ�����
			//��ǰ����Ŀ�����뿪���׷����Χ
			len_t x = pCurAITargetObject->GetPosiX();
			len_t z = pCurAITargetObject->GetPosiZ();
			if (util_dot_distance2(fBornX, fBornZ, x, z) > distance2 || pCurAITargetObject->FindData("SafeAreaFlag") || pCurAITargetObject->QueryInt("Invisible") > 0)
			{
				CurAITargetObject = PERSISTID();
			}
		}
              
    }
    else
    {
        CurAITargetObject = PERSISTID();
    }


	int index = -1;
	int max_enm = -1;
	PERSISTID newTarget = PERSISTID();
	int rows = pAttackerList->GetRows();
    LoopBeginCheck(a);
	for (int i = 0; i < rows; ++i)
	{
        LoopDoCheck(a);
		newTarget = pAttackerList->QueryObject(i, AI_ATTACKER_REC_OBJECT);

		//���ǵ�ǰ������
		if(CurAITargetObject == newTarget)
		{
			continue;
		}

		//���󲻴���
		if (!pKernel->Exists(newTarget))
		{
			continue;
		}
		IGameObj *pNewTarget = pKernel->GetGameObj(newTarget);
		if (pNewTarget == NULL)
		{
			continue;
		}

		// ����ͬһ����
		if (pNewTarget->QueryInt(FIELD_PROP_GROUP_ID) != pSelf->QueryInt(FIELD_PROP_GROUP_ID))
		{
			continue;
		}
		
        //������
		if (pNewTarget->QueryInt("Dead") == 1 || pNewTarget->QueryInt("Invisible") > 0)
		{
			continue;
		}
		//����������Χ
		if (util_dot_distance2(fBornX, fBornZ, pNewTarget->GetPosiX(), pNewTarget->GetPosiZ()) > distance2)
		{
			continue;
		}

        //����ڰ�ȫ���Ͳ�����
        if(pNewTarget->FindData("SafeAreaFlag"))
        {
            continue;
        }

		// ����޵�
		if (pNewTarget->QueryInt("CantBeAttack") == 1)
		{
			continue;
		}

		//��ȡ�����˺�(�����ڹ����������˺�ֵ���ܵ���0)
		int n = pAttackerList->QueryInt(i, AI_ATTACKER_REC_DAMAGE);
		if(max_enm < n)
		{
			max_enm = n;
			index = i;
		}
	}

	//�ҳ��ֵ����
	if (index != -1)
	{
		newTarget = pAttackerList->QueryObject(index, AI_ATTACKER_REC_OBJECT);
	}

    if(index == -1 && CurAITargetObject.IsNull())
    {
       return AI_RT_END_FIGHT;
    }
    else if(CurAITargetObject.IsNull())
    {
        pSelf->SetObject("AITargetObejct", newTarget);
    }
    else if(max_enm > (1.2 * iCurDamageValue))
    {
        pSelf->SetObject("AITargetObejct", newTarget);
    }
    else
    {
        pSelf->SetObject("AITargetObejct", CurAITargetObject);
    }

	return AI_RT_SUCCESS;
}
