//--------------------------------------------------------------------
// �ļ���:      AIRCheckAttackersBase.h
// ��  ��:      ������鹥���߹���
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "FsGame/NpcBaseModule/AI/Rule/AIRuleBase.h"

//��鹥����,��һ������AI�����������ɴ�����
class AIRCheckAttackersBase : public AIRuleBase
{
public:
	AIRCheckAttackersBase(void);
public:
	//ִ�й���
	virtual int DoRule(IKernel * pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args, AITemplateBase &templateWrap);

private:
};
