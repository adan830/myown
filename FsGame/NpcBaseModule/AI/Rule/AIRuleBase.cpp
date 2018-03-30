//--------------------------------------------------------------------
// �ļ���:      AIRuleBase.cpp
// ��  ��:      ��������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AIRuleBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"


AIRuleBase::AIRuleBase(void)
{
	m_ruleType = AI_RULE_MAX;

	m_subRuleCode = 0;
}

bool AIRuleBase::Init(IKernel *pKernel)
{
    return true;
}

int AIRuleBase::DoRule(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args, AITemplateBase &templateWrap)
{
	return AI_RT_FAIL;
}
