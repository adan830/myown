// -------------------------------------------
// �ļ����ƣ� GatherRule.h
// �ļ�˵���� �ɼ�����
// �������ڣ� 2018/03/01
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _GatherRule_H_
#define _GatherRule_H_

#include "TaskBaseRule.h"

class GatherRule : public TaskBaseRule
{

public:
	// ִ�и�������
	virtual bool ExecUpdate(IKernel *pKernel, const PERSISTID &self, 
		const int task_id, const IVarList &args);
};

#endif
