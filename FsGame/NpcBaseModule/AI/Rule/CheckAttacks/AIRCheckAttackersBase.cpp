//--------------------------------------------------------------------
// �ļ���:      AIRCheckAttackersBase.cpp
// ��  ��:      ������鹥���߹���
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AIRCheckAttackersBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include <time.h>

AIRCheckAttackersBase::AIRCheckAttackersBase()
{
	m_ruleType = AI_RULE_CHECK_ATTACKERS;
	m_subRuleCode = AIRS_CHECK_ATTACKERS_BASE;
}

int AIRCheckAttackersBase::DoRule(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args, AITemplateBase &templateWrap)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

    IRecord *pAttackerList = pSelfObj->GetRecord("AttackerList");
    if(pAttackerList == NULL)
    {
        return AI_RT_NULL;
    }
	int rows = pAttackerList->GetRows();
	//ɾ�����Ϸ��Ķ���
	LoopBeginCheck(a);
	for (int i = rows - 1; i >= 0; i--)
	{
		LoopDoCheck(a);
		PERSISTID target = pAttackerList->QueryObject(i, AI_ATTACKER_REC_OBJECT);
		IGameObj* pTargetObj = pKernel->GetGameObj(target);
		// ɾ�������ڶ�����ѷ�����
		if (pTargetObj == NULL)
		{
			pAttackerList->RemoveRow(i);
			//continue;
		}

		// �������,�������˺�����0
//         if(pTargetObj->QueryInt("Dead") > 0)
//         {
//             pAttackerList->SetInt(i, AI_ATTACKER_REC_DAMAGE, 0);
//         }
	}
	return AI_RT_SUCCESS;
}
