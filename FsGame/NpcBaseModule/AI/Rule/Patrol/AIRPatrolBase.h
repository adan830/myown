//--------------------------------------------------------------------
// �ļ���:      AIRPatrolBase.h
// ��  ��:      Ѳ�߹���
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "../AIRuleBase.h"

//ִ��Ѳ�߹������
class AIRPatrolBase : public AIRuleBase
{
public:
	AIRPatrolBase(void);
public:
	virtual int DoRule(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender,const IVarList & args, AITemplateBase &templateWrap);

protected:
	//Ѳ����һ��
	virtual int NextPatrol(IKernel * pKernel, const PERSISTID & self,const AITemplateBase &templateWrap);
};
