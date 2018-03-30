//--------------------------------------------------------------------
// �ļ���:      AIRSpringBase.cpp
// ��  ��:      ������������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AIRSpringBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "FsGame/Interface/FightInterface.h"
#include <time.h>
#include "FsGame/Define/Classes.h"

AIRSpringBase::AIRSpringBase()
{
	m_ruleType = AI_RULE_SPRING;
	m_subRuleCode = AIRS_SPRING_BASE;
}

int AIRSpringBase::DoRule(IKernel * pKernel,const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase &templateWrap)
{
	IGameObj * selfObj = pKernel->GetGameObj(self);
	IGameObj *pSender = pKernel->GetGameObj(sender);
	if (selfObj == NULL || pSender == NULL)
	{
		return AI_RT_IGNORE;
	}

	// ��֤�Ƿ�Ϊ���˺�Ŀ��
	if (!FightModule::m_pInstance->CanDamageTarget(pKernel, self, sender))
	{
		return AI_RT_IGNORE;
	}
    
    //����ڰ�ȫ���Ͳ�����
    if(pSender->FindData("SafeAreaFlag"))
    {
        return AI_RT_IGNORE;
    }

	//����������״̬��������
	int curState = selfObj->QueryInt("CurState");
	if (curState == AI_STATE_DIED || curState == AI_ESCAPE)
	{
		return AI_RT_IGNORE;
	}

	//�����򱻶�������������
	int iInitiative = selfObj->QueryInt("Initiative");
	if (iInitiative == 0)
	{
		return AI_RT_IGNORE;
	}

	//�Ѵ��������¼��AttackerList
    IRecord *pAttackerList = selfObj->GetRecord("AttackerList");
    if(pAttackerList == NULL)
    {
        return AI_RT_NULL;
    }
	int row = pAttackerList->FindObject(AI_ATTACKER_REC_OBJECT, sender);
	if (row < 0)
	{
		if( pAttackerList->GetRows() >= pAttackerList->GetRowMax())
		{
			return AI_RT_IGNORE;
		}

		row = pAttackerList->AddRow(-1);
		if (row < 0)
		{ 
			return AI_RT_IGNORE;
		}

		//��¼�ù�����
		pAttackerList->SetObject(row, AI_ATTACKER_REC_OBJECT, sender);
		pAttackerList->SetInt(row, AI_ATTACKER_REC_DAMAGE, 0);
		pAttackerList->SetInt64(row, AI_ATTACKER_REC_LASTTIME, ::time(NULL));
		pAttackerList->SetInt64(row, AI_ATTACKER_REC_FIRSTTIME, 0);
	}

    if (selfObj->QueryInt("CurState") != AI_STATE_FIGHT)
    {
        //����ս���У�����ս��
        return templateWrap.OnProcess(pKernel, AI_RULE_BEGIN_FIGHT, self, sender, args);
    }
	return AI_RT_IGNORE;
}