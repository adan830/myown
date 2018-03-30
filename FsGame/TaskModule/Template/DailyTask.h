// -------------------------------------------
// �ļ����ƣ� DailyTask.h
// �ļ�˵���� �ճ�����
// �������ڣ� 2018/02/28
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _DailyTask_H_
#define _DailyTask_H_

#include "TaskTemplate.h"

class DailyTask : public TaskTemplate
{

public:
	DailyTask();

    // ��ʼ��
    virtual bool Init(IKernel *pKernel);

	// ����
	virtual bool Start(IKernel *pKernel, const PERSISTID &self);

	// �ɷ��ȡ����
	virtual bool CanAccept(IKernel *pKernel, const PERSISTID &self, 
		const int task_id);

	// �����ύ��
	virtual void OnSubmit(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// ע�ᶨʱ��
	virtual void RegistResetTimer(IKernel *pKernel, const PERSISTID &self);

private:
	// ���ܿ��� 
	static int OnCommandActiveFunc(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// �ճ�����ʱ��
	static int ResetTask(IKernel *pKernel, const PERSISTID &self, int slice);

private:
	// ��ȡ��������
	virtual void AcceptPostTask(IKernel *pKernel, const PERSISTID &self,
		const int task_id);
};

#endif
