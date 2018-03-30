//--------------------------------------------------------------------
// �ļ���:      AIRBeDamagedBase.cpp
// ��  ��:      ����������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AIRBeDamagedBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "FsGame/NpcBaseModule/AI/AIDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "Interface/FightInterface.h"


AIRBeDamagedBase::AIRBeDamagedBase()
{
	m_ruleType = AI_RULE_BE_DAMAGED;
	m_subRuleCode = AIRS_BE_DAMAGED_BASE;
}

int AIRBeDamagedBase::DoRule(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args, AITemplateBase &templateWrap)
{
	if(self == sender)
	{
		return AI_RT_FAIL;
	}

    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return AI_RT_FAIL;
    }
    IGameObj *pSender = pKernel->GetGameObj(sender);
    if(pSender == NULL)
    {
        return AI_RT_FAIL;
    }

	// ����Ļ�,�ҵ���������
	PERSISTID damager = get_pet_master(pKernel, sender);

	//��¼ս��ʱÿ����sender���ԡ�self�����˺�������ӹ����б�
	RecDamage(pKernel, self, damager, args);

	if (pSelf->QueryInt("CurState") != AI_STATE_FIGHT)
	{
		//����ս���У�����ս��
		templateWrap.OnProcess(pKernel, AI_RULE_BEGIN_FIGHT, self, damager, args);
	}
	return AI_RT_SUCCESS;
}

//��¼ս��ʱÿ����sender���ԡ�self�����˺�
int AIRBeDamagedBase::RecDamage(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return AI_RT_FAIL;
    }

	//��ζԹ����˺����
	int nDamage = args.IntVal(1);
	int nDamageCate = args.IntVal(2);
	// ֻ��¼���ܲ����ĳ��ֵ
	if (nDamageCate != DAMAGE_CATE_SKILL)
	{
		return AI_RT_FAIL;
	}
	
	PERSISTID skill = args.ObjectVal(3);
	int nAddHateVal = FightInterfaceInstance->ComputeHateValue(pKernel, skill, nDamage);

    IRecord *pAttackerList = pSelf->GetRecord("AttackerList");
    if(pAttackerList == NULL)
    {
        return AI_RT_NULL;
    }

	//������������뵽����  ��¼�˺�ֵ
	int row = pAttackerList->FindObject(AI_ATTACKER_REC_OBJECT, sender);
	int rows = pAttackerList->GetRows();
	int iMaxRows = pAttackerList->GetRowMax();
	if (row < 0)
	{
		if(rows < iMaxRows)
		{
			//�����Ϊ�� ֱ��д����λ
			CVarList values;
			values << sender << nAddHateVal << time(NULL) << time(NULL);
			pAttackerList->AddRowValue(-1, values);
		}
		else
		{
			//ɾ�������С���Ҳ��ǵ�ǰĿ��
			PERSISTID AITargetObejct = pSelf->QueryObject("AITargetObejct");
			if(!pKernel->Exists(AITargetObejct))
			{
				AITargetObejct = PERSISTID();
			}

			// �滻���һ��
			int minEnm = INT_MAX;
			int minRow = 0;
            LoopBeginCheck(a);
			for(int i = 0; i < rows; ++i)
			{
                LoopDoCheck(a);
				PERSISTID target = pAttackerList->QueryObject(i, AI_ATTACKER_REC_OBJECT);
				if(AITargetObejct != target)
				{
					int enm = pAttackerList->QueryInt(i, AI_ATTACKER_REC_DAMAGE);
					if(enm < minEnm)
					{
						minEnm = enm;
						minRow = i;
					}
				}
			}

			if (minRow >= 0)
			{
				pAttackerList->SetObject(minRow, AI_ATTACKER_REC_OBJECT ,sender);
				pAttackerList->SetInt(minRow, AI_ATTACKER_REC_DAMAGE, nAddHateVal);
				pAttackerList->SetInt64(minRow, AI_ATTACKER_REC_LASTTIME, ::time(NULL));
				pAttackerList->SetInt64(minRow, AI_ATTACKER_REC_FIRSTTIME, time(NULL));
			}
		}
	}
	else
	{
		//�����˺���ֵ ��������ֵ
		int sum = pAttackerList->QueryInt(row, AI_ATTACKER_REC_DAMAGE);
		sum = sum + nAddHateVal;
		pAttackerList->SetInt(row, AI_ATTACKER_REC_DAMAGE, sum);
		int64_t iTime = pAttackerList->QueryInt64(row, AI_ATTACKER_REC_FIRSTTIME);
		//û�е�һ�ι���ʱ����м�¼
		if (iTime == 0)
		{
			pAttackerList->SetInt64(row, AI_ATTACKER_REC_FIRSTTIME, time(NULL));
		}
		//�������һ�ν���ʱ��
		pAttackerList->SetInt64(row, AI_ATTACKER_REC_LASTTIME, ::time(NULL));
	}

	return AI_RT_SUCCESS;
}
