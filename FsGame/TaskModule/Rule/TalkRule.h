// -------------------------------------------
// �ļ����ƣ� TalkRule.h
// �ļ�˵���� �Ի�����
// �������ڣ� 2018/03/01
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _TalkRule_H_
#define _TalkRule_H_

#include "TaskBaseRule.h"

class TalkRule : public TaskBaseRule
{

public:
	// ִ�и�������
	virtual bool ExecUpdate(IKernel *pKernel, const PERSISTID &self, 
		const int task_id, const IVarList &args);
};

#endif
