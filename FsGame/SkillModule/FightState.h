//--------------------------------------------------------------------
// �ļ���:		FightState.h
// ��  ��:		ս��״̬
// ˵  ��:		
//				
// ��������:	2014��06��27��
// ��������:	2014��06��27��
// ������:		  ( )
//    :	    
//--------------------------------------------------------------------

#ifndef __FightState_H__
#define __FightState_H__

#include "Fsgame/Define/header.h"

class FightState : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ر�
	virtual bool Shut(IKernel* pKernel);

public:
	/// \brief ����
	static int OnReady(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

private:
	// �������ս��״̬
	static int OnCommandUpdateFightState(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

public:
	//����ս��״̬
	static int HB_ResetFightState(IKernel * pKernel, const PERSISTID & self,
		int slice);

private:
	static FightState* m_pFightState;
};

#endif //__FightState_H__
