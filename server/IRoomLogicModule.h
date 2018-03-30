//--------------------------------------------------------------------
// �ļ���:		IRoomLogicModule.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��5��
// ������:		���н�
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IROOMLOGICMODULE_H
#define _SERVER_IROOMLOGICMODULE_H

#include "../public/Macros.h"
#include "../server/LoopCheck.h"
// �߼�ģ��ӿ�
//#include "../server/IRoomKernel.h"
class IKernel;

class IRoomLogicModule
{
public:
	virtual ~IRoomLogicModule() = 0;

	// ��ʼ��
	virtual bool Init(IKernel* pKernel) = 0;
	// �ر�
	virtual bool Shut(IKernel* pKernel) = 0;
	// ��������֮ǰ�ĳ�ʼ��������OnCreateClass�Ѿ�ִ����ϣ�
	virtual bool BeforeLaunch(IKernel* pKernel) { return true; }
};

inline IRoomLogicModule::~IRoomLogicModule() {}

#endif // _SERVER_ILOGICMODULE_H

