// -------------------------------------------
// �ļ����ƣ� PathfindRule.h
// �ļ�˵���� Ѱ·����
// �������ڣ� 2018/03/01
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _PathfindRule_H_
#define _PathfindRule_H_

#include "TaskBaseRule.h"

class PathfindRule : public TaskBaseRule
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
