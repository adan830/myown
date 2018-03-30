// -------------------------------------------
// �ļ����ƣ� TaskBaseRule.h
// �ļ�˵���� �����������
// �������ڣ� 2018/02/26
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _TaskBaseRule_H_
#define _TaskBaseRule_H_

#include "Fsgame/Define/header.h"
#include "../TaskDefine.h"

class TaskBaseRule
{

public:
    // ��ʼ��
    virtual bool Init(IKernel *pKernel);

	// ִ�н�ȡ����
	virtual bool ExecAccept(IKernel *pKernel, const PERSISTID &self, 
		const int task_id);

	// ִ�и�������
	virtual bool ExecUpdate(IKernel *pKernel, const PERSISTID &self, 
		const int task_id, const IVarList &args);

	// �����Լ��
	virtual void ExecCheck(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// ������ɺ�
	virtual void OnComplete(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

public:
	// ������ȼ���
	virtual bool Calculate(IKernel *pKernel, const PERSISTID &self,
		const int task_id, std::string &new_progress, const IVarList &args, bool& is_complete);

	// �޸Ľ���
	virtual bool SetProgress(IKernel *pKernel, const PERSISTID &self,
		const int task_id, const char *new_progress, bool is_completed = true);
};

#endif
