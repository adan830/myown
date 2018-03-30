//--------------------------------------------------------------------
// �ļ���:      AIREndFightBase.cpp
// ��  ��:      ������������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AIREndFightBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "FsGame/NpcBaseModule/AI/AIDefine.h"
#include "FsGame/NpcBaseModule/AI/AIFunction.h"
#include "public/Inlines.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame\Interface\FightInterface.h"

AIREndFightBase::AIREndFightBase()
{
	m_ruleType = AI_RULE_END_FIGHT;
	m_subRuleCode = AIRS_END_FIGHT_BASE;
}

int AIREndFightBase::DoRule(IKernel * pKernel,const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase &templateWrap)
{
	//�˳�ս��
    AISystem::ClearFight(pKernel, self);
    //����״̬
	templateWrap.ChangeState(pKernel, self, AI_STATE_BACK);

    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return AI_RT_NULL;
    }

    pSelf->SetInt("SubState", AI_PATROL_WAIT);

	//�Ѿ�����
	if (pSelf->QueryInt("Dead") > 0)
	{
		return AI_RT_IGNORE;
	}

    //ǿ�ƴ�ϼ���
    if(FightInterfaceInstance != NULL)
    {
        FightInterfaceInstance->BreakSkill(pKernel, self, self, true);
    }

    float pBornX = pSelf->QueryFloat("BornX");
    float pBornZ = pSelf->QueryFloat("BornZ");

    //�ж��ǲ����ڳ�����
    float px = pSelf->GetPosiX();
    float pz = pSelf->GetPosiZ();
    if (FloatEqual(px ,pBornX) && FloatEqual(pz, pBornZ))
    {
        AIFunction::MotionCallBack(pKernel, self, PERSISTID(), CVarList());
        return AI_RT_SUCCESS;
    }

	//����״̬��ֱ�ӷ��س�����,����״̬Ϊ1��Ѳ��
	int iBornStand = pSelf->QueryInt( "BornStand");
	if (iBornStand == 1)
	{
		//�ص�������
        AIFunction::StartMoveToPoint(pKernel, self, pBornX, pBornZ);
        //��ͨ�ֻس����㳬ʱ����
        if (!pKernel->FindHeartBeat(self, "AIHeartBeat::HB_BACK_BORN_TIMEOUT"))
        {
            pKernel->AddCountBeat(self, "AIHeartBeat::HB_BACK_BORN_TIMEOUT", AIFunction::CountFindPathPointListTime(pKernel, self) + 2000, 1);
        }
		return AI_RT_IGNORE;
	}

    //�ص�������
	if (AIFunction::StartMoveToPoint(pKernel, self, pBornX, pBornZ) == AI_RT_FAIL)
	{
        //�س�����Ѱ·ʧ��
		float pHight  = pKernel->GetWalkHeight(pBornX,pBornZ);
		float pOrient = pSelf->GetOrient();
		pKernel->MoveTo(self,pBornX,pHight,pBornZ,pOrient);

// #ifndef FSROOMLOGIC_EXPORTS
// 		BattleTeamModule::m_pBattleTeamModule->AddLittleStepGridRec(pKernel, self, pBornX, pBornZ);
// #endif // FSROOMLOGIC_EXPORTS

        AIFunction::MotionCallBack(pKernel, self, PERSISTID(), CVarList());
	}

    //��ͨ�ֻس����㳬ʱ����
    if (!pKernel->FindHeartBeat(self, "AIHeartBeat::HB_BACK_BORN_TIMEOUT"))
    {
        pKernel->AddCountBeat(self, "AIHeartBeat::HB_BACK_BORN_TIMEOUT", AIFunction::CountFindPathPointListTime(pKernel, self) + 2000, 1);
    }
    
	return AI_RT_SUCCESS;
}
