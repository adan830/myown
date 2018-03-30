//--------------------------------------------------------------------
// �ļ���:      AIRFightBase.h
// ��  ��:      ����ս������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "FsGame/NpcBaseModule/AI/Rule/AIRuleBase.h"

class AIRFightBase: public AIRuleBase
{
public:
	AIRFightBase(void);
	virtual int DoRule(IKernel * pKernel, const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase & templateWrap);
    //��ȡʹ�õļ���
    virtual const char* GetUseSkill(IKernel* pKernel, const PERSISTID& self);
	int DoFight(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender,const IVarList & args, AITemplateBase &templateWrap);
};
