//--------------------------------------------------------------------
// �ļ���:      AITNormalFight.cpp
// ��  ��:      ��ͨNPCģ��
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AITNormalFight.h"
#include "FsGame/NpcBaseModule/AI/Rule/AIRuleBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include <time.h>

AITNormalFight::AITNormalFight(void)
{
	m_templateType = AI_TEMP_NORMAL_FIGHT;

	//��������
	RegistEventRule(AI_RULE_SPRING, AIRS_SPRING_BASE);
	//Ѳ�߹���
	RegistEventRule(AI_RULE_PATROL, AIRS_PATROL_BASE);
	//�ܵ��˺���AI����
	RegistEventRule(AI_RULE_BE_DAMAGED, AIRS_BE_DAMAGED_BASE); 
	//��ʼս��AI����
	RegistEventRule(AI_RULE_BEGIN_FIGHT, AIRS_BEGIN_FIGHT_BASE); 
	//ִ��ս��AI����
	RegistEventRule(AI_RULE_FIGHT, AIRS_FIGHT_BASE);   
	//ѡ�񹥻�����AI����
	RegistEventRule(AI_RULE_SEL_TARTGET, AIRS_SEL_TARGET_BASE); 
	//�˳�ս��AI����
	RegistEventRule(AI_RULE_END_FIGHT, AIRS_END_FIGHT_BASE);  
	//׷��������
	RegistEventRule(AI_RULE_CHASE_TARGET, AIRS_CHASE_TARGET_BASE);
	//��ɱ��
	RegistEventRule(AI_RULE_ONDEAD, AIRS_ONDEAD_BASE);
	//����
	RegistEventRule(AI_RULE_ESCAPE, AIRS_ESCAPE_BASE);

 }

//��������
AITNormalFight::~AITNormalFight()
{

}


