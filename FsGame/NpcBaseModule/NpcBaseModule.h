//--------------------------------------------------------------------
// �ļ���:      NpcBaseModule.h
// ��  ��:      NPC����ģ��
// ˵  ��:		
// ��������:    2014��10��17��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef __NpcBaseModule_H__
#define __NpcBaseModule_H__

#include "Fsgame/Define/header.h"

class NpcBaseModule: public ILogicModule  
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ر�
	virtual bool Shut(IKernel* pKernel);

	// ������Ʒ����
	bool LoadNpcPathConfig(IKernel* pKernel);

public:

	static int OnLoad(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	//��ӢNPCλ��
	/*static int QueryElitePos(IKernel* pKernel, const PERSISTID& player, 
		const PERSISTID& sender, const IVarList& args);*/
public:
	static NpcBaseModule* m_pNpcBaseModule;
};

#endif // __NpcBaseModule_H__
