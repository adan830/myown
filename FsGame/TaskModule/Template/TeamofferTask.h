// -------------------------------------------
// �ļ����ƣ� TeamofferTask.h
// �ļ�˵���� ��������
// �������ڣ� 2018/04/02
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _Teamoffer_Task_H_
#define _Teamoffer_Task_H_

#include "TaskTemplate.h"

class TeamofferTask : public TaskTemplate
{

public:
	TeamofferTask();

    // ��ʼ��
    virtual bool Init(IKernel *pKernel);

	// ����
	virtual bool Start(IKernel *pKernel, const PERSISTID &self);

	// ע�ᶨʱ��
	virtual void RegistResetTimer(IKernel *pKernel, const PERSISTID &self);

	// �����ύ��
	virtual void OnSubmit(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

private:
	// ���ܿ��� 
	static int OnCommandActiveFunc(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ��������ʱ��
	static int ResetTask(IKernel *pKernel, const PERSISTID &self, int slice);

private:
	// ��ȡ��������
	virtual void AcceptPostTask(IKernel *pKernel, const PERSISTID &self,
		const int task_id);
};

#endif
