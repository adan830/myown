//--------------------------------------------------------------------
// �ļ���:      AIRSelTargetBase.h
// ��  ��:      ��������Ŀ�����
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "FsGame/NpcBaseModule/AI/Rule/AIRuleBase.h"

class AIRSelTargetBase : public AIRuleBase
{
public:
	AIRSelTargetBase(void);
public:
	virtual int DoRule(IKernel * pKernel, const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase & templateWrap);
};
