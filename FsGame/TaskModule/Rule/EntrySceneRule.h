// -------------------------------------------
// �ļ����ƣ� EntrySceneRule.h
// �ļ�˵���� ���볡������
// �������ڣ� 2018/03/01
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _EntrySceneRule_H_
#define _EntrySceneRule_H_

#include "TaskBaseRule.h"

class EntrySceneRule : public TaskBaseRule
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
