//--------------------------------------------------------------------
// �ļ���:      AITBoss.h
// ��  ��:      BOSSģ��
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once

#include "AITemplateBase.h"

//AI��ͨս����ģ����
class AITBoss : public AITemplateBase
{
public:
	AITBoss(void);

	~AITBoss();

    //������Ϊ��������Ϣ����
    virtual int	OnBasicActionMsg(IKernel* pKernel, const PERSISTID& self, int msg, const IVarList& args = CVarList());


};
