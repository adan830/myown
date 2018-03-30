// -------------------------------------------
// �ļ����ƣ� BranchTask.h
// �ļ�˵���� ֧������
// �������ڣ� 2018/02/28
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _BranchTask_H_
#define _BranchTask_H_

#include "TaskTemplate.h"

class BranchTask : public TaskTemplate
{

public:
	BranchTask();

    // ��ʼ��
    virtual bool Init(IKernel *pKernel);

	// ����
	virtual bool Start(IKernel *pKernel, const PERSISTID &self);

	// �����ύ��
	virtual void OnSubmit(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

private:
	// �ͻ���׼������
	static int OnReady(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

private:
	// ��ȡ��������
	virtual void AcceptPostTask(IKernel *pKernel, const PERSISTID &self,
		const int task_id);
};

#endif
