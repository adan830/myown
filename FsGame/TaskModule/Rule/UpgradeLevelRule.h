// -------------------------------------------
// �ļ����ƣ� UpgradeLevelRule.h
// �ļ�˵���� ��������
// �������ڣ� 2018/03/01
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _UpgradeLevelRule_H_
#define _UpgradeLevelRule_H_

#include "TaskBaseRule.h"

class UpgradeLevelRule : public TaskBaseRule
{

public:
	// �����Լ��
	virtual void ExecCheck(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

};

#endif
