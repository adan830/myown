//--------------------------------------------------------------------
// �ļ���:		TrapNpcModule.h
// ��  ��:		����NPC
// ˵  ��:		
// ��������:	2015��07��11��
// ������:		  
//    :	    
//--------------------------------------------------------------------

#ifndef __TrapNpc_H__
#define __TrapNpc_H__

#include "Fsgame/Define/header.h"

class TrapEventManagerModule;
class TrapNpcModule: public ILogicModule  
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);
	// �ͷ�
	virtual bool Shut(IKernel* pKernel);

private:
	// ���볡��
	static int OnEntry(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ����ص�
	static int OnSpring(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);
	// ж�ػص�
	static int OnEndSpring(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
	// ���ٻص�
	static int OnDestroy(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
public:
    // �ӳ�����
    static int HB_Delay_Spring(IKernel* pKernel, const PERSISTID& self, int silce);
public:
	static TrapEventManagerModule* m_pTrapEventManager;
};

#endif // __TrapNpc_H__
