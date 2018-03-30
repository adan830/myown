//--------------------------------------------------------------------
// �ļ���:      AIRChaseTargetBase.cpp
// ��  ��:      ����׷������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "FsGame/NpcBaseModule/AI/Rule/AIRuleBase.h"

//׷��Ŀ��AI������࣬����׷��Ŀ��AI�����ɴ�����
class AIRChaseTargetBase : public AIRuleBase
{
public:
	AIRChaseTargetBase(void);
public:
	//ִ�й���
	virtual int DoRule(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args, AITemplateBase &templateWrap);
protected:
	//�ǲ����ڼ��ܷ�Χ��
	virtual int InSkillRange(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender);

	// �Ƿ���׷�����
	bool IsCanChasePlayer(IKernel * pKernel, const PERSISTID& target);
};
