//--------------------------------------------------------------------
// �ļ���:      AITBoss.cpp
// ��  ��:      BOSSģ��
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AITBoss.h"
#include "../Rule/AIRuleBase.h"
#include "../AIDefine.h"
#include "FsGame/Define/GameDefine.h"


AITBoss::AITBoss(void)
{
	m_templateType = AI_TEMP_BOSS;

	//��������
	//Ѳ��
	RegistEventRule(AI_RULE_PATROL, AIRS_PATROL_BASE);
	//����
	RegistEventRule(AI_RULE_SPRING, AIRS_SPRING_BASE);
	//�ܵ��˺���AI����
	RegistEventRule(AI_RULE_BE_DAMAGED, AIRS_BE_DAMAGED_BASE); 
	//��ʼս��AI����
	RegistEventRule(AI_RULE_BEGIN_FIGHT, AIRS_BEGIN_FIGHT_BASE); 
	//ִ��ս��AI����
	RegistEventRule(AI_RULE_FIGHT, AIRS_FIGHT_BASE);   
	//��鹥����
	RegistEventRule(AI_RULE_CHECK_ATTACKERS, AIRS_CHECK_ATTACKERS_BASE);
	//ѡ�񹥻�����AI����
	RegistEventRule(AI_RULE_SEL_TARTGET, AIRS_SEL_TARGET_BASE); 
	//�˳�ս��AI����
	RegistEventRule(AI_RULE_END_FIGHT, AIRS_END_FIGHT_BASE);  
	//��ɱ��
	RegistEventRule(AI_RULE_ONDEAD, AIRS_ONDEAD_BASE);
	//׷��������
	RegistEventRule(AI_RULE_CHASE_TARGET, AIRS_CHASE_TARGET_BASE);
    //����
    //RegistEventRule(AI_RULE_RECUPERATE, AIRS_RECUPERATE_BOSS);
	//����
	RegistEventRule(AI_RULE_ESCAPE, AIRS_ESCAPE_BASE);
 }

//��������
AITBoss::~AITBoss()
{

}

//ÿ���ƶ���ɺ�ص�
int AITBoss::OnBasicActionMsg(IKernel * pKernel , const PERSISTID & self , int msg , const IVarList & args)
{
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return AI_RT_IGNORE;
    }

    //�Ѿ�����
    if (pSelf->QueryInt("Dead") > 0)
    {
        return AI_RT_IGNORE;
    }

    //��ǰ״̬
    int cur_state = pSelf->QueryInt("CurState");
    if (cur_state == AI_STATE_PATROL)
    {
        pSelf->SetDataInt("PatrolRestTime", 0);
        pSelf->SetInt("SubState",AI_PATROL_WAIT);
    }
    else if (cur_state == AI_STATE_BACK)
    {
        //OnProcess(pKernel, AI_RULE_RECUPERATE, self);
        
        //�ص�Ҳ����ת�����
        ADD_COUNT_BEAT(pKernel, self, "AIHeartBeat::HB_AIRatate", 1000, 1);
    }
    
    return 0;
}