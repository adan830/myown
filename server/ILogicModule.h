//--------------------------------------------------------------------
// �ļ���:		ILogicModule.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��5��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_ILOGICMODULE_H
#define _SERVER_ILOGICMODULE_H

#include "../public/Macros.h"
#include "../server/LoopCheck.h"
// �߼�ģ��ӿ�

class IKernel;

class ILogicModule
{
public:
	virtual ~ILogicModule() = 0;

	// ��ʼ��
	virtual bool Init(IKernel* pKernel) = 0;
	// �ر�
	virtual bool Shut(IKernel* pKernel) = 0;
	// ��������֮ǰ�ĳ�ʼ��������OnCreateClass�Ѿ�ִ����ϣ�
	virtual bool BeforeLaunch(IKernel* pKernel) { return true; }
};

inline ILogicModule::~ILogicModule() {}

#endif // _SERVER_ILOGICMODULE_H

