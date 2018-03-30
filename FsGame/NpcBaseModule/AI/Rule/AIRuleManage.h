//--------------------------------------------------------------------
// �ļ���:      AIRuleManage.h
// ��  ��:      AIϵͳ������ִ�й�����������
// ˵  ��:
// ��������:    2015��3��30��
// ������:       
// �޸���:        
//--------------------------------------------------------------------

#ifndef __AIRULEMANAGE_H__
#define __AIRULEMANAGE_H__

#include "../AIDefine.h"
#include "AIRuleBase.h"

class AIRuleManage
{
public:

    //��ʼ��
    static bool Init(IKernel *pKernel);

    //ȡ��һ��ָ����AIִ�й������Ҳ��������������µĻ���
    static AIRuleBase &GetRuleItem(AI_RULE_TYPE ruleType, unsigned int subType);

    ~AIRuleManage();

private:

    //���ù��쿽��
    AIRuleManage(const AIRuleManage &a);
    //���ø�ֵ����
    AIRuleManage & operator =(const AIRuleManage &a);


    //�Ҳ���ʱ�������
    static AIRuleBase m_AIRuleBase;

    //��ʼ������
    static bool RegistAIRule(IKernel *pKernel, AIRuleBase * pRuleBase);

    //������󱣴��,��ά���飬��һ����ִ�����ͣ��ڶ�������һ���͵�����ִ�С�
    static std::vector<std::vector<AIRuleBase*>> m_ruleList;
};

#endif//__AIRULEMANAGE_H__