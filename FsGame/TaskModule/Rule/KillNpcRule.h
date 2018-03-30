// -------------------------------------------
// �ļ����ƣ� KillNpcRule.h
// �ļ�˵���� ��ɱNpc����
// �������ڣ� 2018/02/27
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _KillNpcRule_H_
#define _KillNpcRule_H_

#include "TaskBaseRule.h"

class KillNpcRule : public TaskBaseRule
{

public:
	// ִ�н�ȡ����
	virtual bool ExecAccept(IKernel *pKernel, const PERSISTID &self, 
		const int task_id);

	// ִ�и�������
	virtual bool ExecUpdate(IKernel *pKernel, const PERSISTID &self, 
		const int task_id, const IVarList &args);

private:
	// ������ȼ���
	virtual bool Calculate(IKernel *pKernel, const PERSISTID &self,
		const int task_id, std::string &new_progress, const IVarList &args, bool& is_complete);
};

#endif
