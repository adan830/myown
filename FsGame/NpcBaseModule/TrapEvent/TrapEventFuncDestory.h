#ifndef _TRAP_EVENT_FUNC_DESTORY_H__
#define _TRAP_EVENT_FUNC_DESTORY_H__
#include "TrapEventBase.h"
//����Ŀ��
class TrapEventFuncDestory :public TrapEventBase
{
public:

	virtual bool Execute(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& target, const IVarList& vEventParams);
};















#endif