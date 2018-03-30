//--------------------------------------------------------------------
// �ļ���:		FightActionControl.h
// ��  ��:		ս����Ϊ״̬����
// ˵  ��:		
//				
// ��������:	2016��04��06��
// ��������:	2016��04��06��
// ������:		  ( )
//    :	    
//--------------------------------------------------------------------

#ifndef __FightActionControl_H__
#define __FightActionControl_H__

#include "Fsgame/Define/header.h"
#include "utils/Singleton.h"
#include <vector>

// Ŀǰ3����Ϊ ���� > ���� > ���� ��Ϊ���ȼ��ߵ� �������ȼ��͵� ��֮���ܸ���

class FightActionControl
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ı�ս����Ϊ״̬
	bool ChangeState(IKernel* pKernel, const PERSISTID& self, int nState, int nStiffTime = 0);

	// ��ȡ��ǰ����Ϊ״̬
	int GetFightActionState(IKernel* pKernel, const PERSISTID& self);

	// ����Ϊ����״̬
	void ResetNormalState(IKernel* pKernel, const PERSISTID& self);
private:
	// �Ƴ�Ӳֱ״̬
	static int H_RemoveActionState(IKernel* pKernel,const PERSISTID& self,int slice);
private:
};	

typedef HPSingleton<FightActionControl> FightActionControlSingleton;
#endif //__FightState_H__
