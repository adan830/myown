//--------------------------------------------------------------------
// �ļ���:      AIREscapeBase.h
// ��  ��:      ������ʼս������
// ˵  ��:
// ��������:    2016��10��29��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "FsGame/NpcBaseModule/AI/Rule/AIRuleBase.h"

class AIREscapeBase : public AIRuleBase
{
public:
	AIREscapeBase(void);
public:
	virtual int DoRule(IKernel * pKernel,const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase &templateWrap);
};
