//--------------------------------------------------------------------
// �ļ���:      AIRuleManage.h
// ��  ��:      AIϵͳ������ִ�й�����������
// ˵  ��:
// ��������:    2015��3��30��
// ������:       
// �޸���:        
//    :    ������ţ
//--------------------------------------------------------------------

#include "AIRuleManage.h"
#include "FsGame/Define/LogDefine.h"
#include "utils/extend_func.h"

//spring
#include "Spring/AIRSpringBase.h"


//patrol
#include "FsGame/NpcBaseModule/AI/Rule/Patrol/AIRPatrolBase.h"

//seltarget
#include "FsGame/NpcBaseModule/AI/Rule/SelTarget/AIRSelTargetBase.h"

//beginfight
#include "FsGame/NpcBaseModule/AI/Rule/BeginFight/AIRBeginFightBase.h"

//fight
#include "FsGame/NpcBaseModule/AI/Rule/Fight/AIRFightBase.h"

//bedamage
#include "FsGame/NpcBaseModule/AI/Rule/BeDamaged/AIRBeDamagedBase.h"

//endfight
#include "FsGame/NpcBaseModule/AI/Rule/EndFight/AIREndFightBase.h"

//ondead
#include "FsGame/NpcBaseModule/AI/Rule/OnDead/AIROnDeadBase.h"

//ChaseTarget
#include "FsGame/NpcBaseModule/AI/Rule/ChaseTarget/AIRChaseTargetBase.h"

//CheckAttacker
#include "FsGame/NpcBaseModule/AI/Rule/CheckAttacks/AIRCheckAttackersBase.h"

#include "Escape/AIREscapeBase.h"


AIRuleBase AIRuleManage::m_AIRuleBase;

std::vector<std::vector<AIRuleBase*>> AIRuleManage::m_ruleList;


AIRuleManage::~AIRuleManage()
{
    std::size_t typeLength = m_ruleList.size();
    std::size_t subTypeLength = 0;
    LoopBeginCheck(a);
    for(std::size_t i = 0; i < typeLength; ++i)
    {
        LoopDoCheck(a);
        subTypeLength = m_ruleList[i].size(); 
        LoopBeginCheck(b);
        for(std::size_t j = 0; j < subTypeLength; ++j)
        {
            LoopDoCheck(b);
            if(m_ruleList[i][j] != NULL)
            {
                delete m_ruleList[i][j];
            }
        }
    }
    m_ruleList.clear();
}

//��ʼ��
bool AIRuleManage::Init(IKernel *pKernel)
{
    if(!m_ruleList.empty())
    {
        return false;
    }

    m_ruleList.resize(AI_RULE_MAX);
    //��������
    m_ruleList[AI_RULE_SPRING].assign(AIRS_SPRING_MAX, NULL);
    RegistAIRule(pKernel, new AIRSpringBase);
//     RegistAIRule(pKernel, new AIRSpringIntercept);
//     RegistAIRule(pKernel, new AIRSpringBattleNpc);
//     RegistAIRule(pKernel, new AIRSpringNationGuard);
//     RegistAIRule(pKernel, new AISectorSpringRange);
    //�������
//     m_ruleList[AI_RULE_FOLLOW].assign(AIRS_FOLLOW_MAX, NULL);
//     RegistAIRule(pKernel, new AIRFollowPet);

    //Ѳ��
    m_ruleList[AI_RULE_PATROL].assign(AIRS_PATROL_MAX, NULL);
    RegistAIRule(pKernel, new AIRPatrolBase);
//     RegistAIRule(pKernel, new AIRPatrolConvoy);
//     RegistAIRule(pKernel, new AIRPatrolBattleNpc);
//     RegistAIRule(pKernel, new AIRPatrolPet);
// 	RegistAIRule(pKernel, new AISectorSpringPatorl);

    //ѡ��Ŀ��
    m_ruleList[AI_RULE_SEL_TARTGET].assign(AIRS_SEL_TARGET_MAX, NULL);
    RegistAIRule(pKernel, new AIRSelTargetBase);
  //  RegistAIRule(pKernel, new AIRSelTargetBattleNpc);

    //��ʼս��
    m_ruleList[AI_RULE_BEGIN_FIGHT].assign(AIRS_BEGIN_FIGHT_MAX, NULL);
    RegistAIRule(pKernel, new AIRBeginFightBase);
   // RegistAIRule(pKernel, new AIRBeginFightBattleNpc);

    //ս��
    m_ruleList[AI_RULE_FIGHT].assign(AIRS_FIGHT_MAX, NULL);
    RegistAIRule(pKernel, new AIRFightBase);
//     RegistAIRule(pKernel, new AIRFightBattleNpc);
//     RegistAIRule(pKernel, new AIRFightPet);
// #ifndef FSROOMLOGIC_EXPORTS	
// 	RegistAIRule(pKernel, new AIRFightStartSceneBoss);
// 	RegistAIRule(pKernel, new AIRFightControlHpBoss);
// #endif
    //���˺�
    m_ruleList[AI_RULE_BE_DAMAGED].assign(AIRS_BE_DAMAGED_MAX, NULL);
    RegistAIRule(pKernel, new AIRBeDamagedBase);
//     RegistAIRule(pKernel, new AIRConvoyBeDamaged);
//     RegistAIRule(pKernel, new AIRBeDamagedIntercept);

    //����ս��
    m_ruleList[AI_RULE_END_FIGHT].assign(AIRS_END_FIGHT_MAX, NULL);
    RegistAIRule(pKernel, new AIREndFightBase);
//     RegistAIRule(pKernel, new AIREndFightBattleNpc);
//     RegistAIRule(pKernel, new AIREndFightIntercept);

    //��ɱ��
    m_ruleList[AI_RULE_ONDEAD].assign(AIRS_ONDEAD_MAX, NULL);
    RegistAIRule(pKernel, new AIROnDeadBase);
//     RegistAIRule(pKernel, new AIRConvoyNpcOnDead);
//     RegistAIRule(pKernel, new AIROnDeadBattleNpc);
// 	RegistAIRule(pKernel, new AIRBoxOnDead);
// 	RegistAIRule(pKernel, new AIROnDeadGenTrap);
// 	RegistAIRule(pKernel, new AIREscortNpcOnDead);

    //׷��Ŀ��
    m_ruleList[AI_RULE_CHASE_TARGET].assign(AIRS_CHASE_TARGET_MAX, NULL);
    RegistAIRule(pKernel, new AIRChaseTargetBase);

    //��鹥������
    m_ruleList[AI_RULE_CHECK_ATTACKERS].assign(AIRS_CHECK_ATTACKERS_MAX, NULL);
    RegistAIRule(pKernel, new AIRCheckAttackersBase);
	//RegistAIRule(pKernel, new AIRCheckAttackersBattleNpc);

    //����
    //m_ruleList[AI_RULE_RELIVE].assign(AIRS_RELIVE_MAX, NULL);
    //RegistAIRule(pKernel, new AIRReliveBattleNpc);

    //����
//     m_ruleList[AI_RULE_RECUPERATE].assign(AIRS_RECUPERATE_MAX, NULL);
//     RegistAIRule(pKernel, new AIRRecuperateBoss);

	//����
// 	m_ruleList[AI_RULE_ESCAPE].assign(AIRS_ESCAPE_MAX, NULL);
// 	RegistAIRule(pKernel, new AIREscapeBase);

#ifndef FSROOMLOGIC_EXPORTS
	//����
// 	m_ruleList[AI_RULE_DESTORY].assign(AIRS_DESTROY_MAX, NULL);
// 	RegistAIRule(pKernel, new AIRDestroyBattleLeadNpc);
#endif
    return true;
}


//ȡ��һ��ָ����AIִ�й������Ҳ��������������µĻ���
AIRuleBase &AIRuleManage::GetRuleItem(AI_RULE_TYPE ruleType, unsigned int subType)
{
    if(ruleType < 0 || ruleType >= AI_RULE_MAX || subType < 0)
    {
        return m_AIRuleBase;
    }
    
    if( subType >= m_ruleList[ruleType].size())
    {
         return m_AIRuleBase;
    }

    if(m_ruleList[ruleType][subType] == NULL)
    {
        return m_AIRuleBase;
    }

    return *m_ruleList[ruleType][subType];
}

//��ʼ������
bool AIRuleManage::RegistAIRule(IKernel *pKernel, AIRuleBase * pRuleBase)
{
    int type = pRuleBase->GetRuleType();
    int subtype = pRuleBase->GetSubRuleType();
    pRuleBase->Init(pKernel);
    //Խ�籣��
    if (type < 0 || type >= AI_RULE_MAX || subtype < 0)
    {
        extend_warning(LOG_ERROR, "����AI����ʧ��");
        return false;
    }

    //�Ƿ��ظ�
    if(m_ruleList[type][subtype] == NULL)
    {
        m_ruleList[type][subtype] = pRuleBase;
    }
    else
    {
        extend_warning(LOG_ERROR, "����AI����ʧ��");
        return false;
    }
    return  true;
}