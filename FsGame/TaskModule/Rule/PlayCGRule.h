// -------------------------------------------
// �ļ����ƣ� PlayCGRule.h
// �ļ�˵���� ����CG����
// �������ڣ� 2018/03/01
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _PlayCGRule_H_
#define _PlayCGRule_H_

#include "TaskBaseRule.h"

class PlayCGRule : public TaskBaseRule
{

public:
	// ִ�и�������
	virtual bool ExecUpdate(IKernel *pKernel, const PERSISTID &self,
		const int task_id, const IVarList &args);

	// �����Լ��
	virtual void ExecCheck(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

};

#endif
