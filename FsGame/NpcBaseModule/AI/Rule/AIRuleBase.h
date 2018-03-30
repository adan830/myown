//--------------------------------------------------------------------
// �ļ���:      AIRuleBase.h
// ��  ��:      ��������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "Fsgame/Define/header.h"
#include "../Template/AITemplateBase.h"

//Ѱ·���
enum
{
	AI_FINDPATH_POS_X,
	AI_FINDPATH_POS_Z
};

class AIRuleBase
{
public:

	AIRuleBase(void);

public:

    virtual bool Init(IKernel *pKernel);

	//��øù��������
	const AI_RULE_TYPE GetRuleType() const
    {
        return m_ruleType;
    };

	//��øù����������
	const int GetSubRuleType() const
    {
        return m_subRuleCode;
    };

	//ִ�иù���
	virtual int DoRule(IKernel * pKernel, const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase & templateWrap);

protected:

    //����������
	AI_RULE_TYPE m_ruleType;

    //����������
	int m_subRuleCode;
};

