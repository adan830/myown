//--------------------------------------------------------------------
// �ļ���:      AIRBeDamagedBase.h
// ��  ��:      ����������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "FsGame/NpcBaseModule/AI/Rule/AIRuleBase.h"

class AIRBeDamagedBase : public AIRuleBase
{
public:
	AIRBeDamagedBase(void);
public:
	virtual int DoRule(IKernel * pKernel, const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase & templateWrap);
protected:
    //��¼ս��ʱÿ����sender���ԡ�self�����˺�
    int RecDamage(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);
};
