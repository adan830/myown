//--------------------------------------------------------------------
// �ļ���:      DoorNpcModule.h
// ��  ��:      ����NPCģ��
// ˵  ��:		
// ��������:    2014��12��6��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef __DoorNpcModule_H__
#define __DoorNpcModule_H__

#include "Fsgame/Define/header.h"

class AsynCtrlModule;
class DoorNpcModule: public ILogicModule  
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ر�
	virtual bool Shut(IKernel* pKernel);
public:

	static int OnSpring(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	static int OnEndSpring(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	static AsynCtrlModule * m_pAsynCtrlModule;
};

#endif // __DoorNpcModule_H__
