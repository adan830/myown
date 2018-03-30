// -------------------------------------------
// �ļ����ƣ� MainTask.h
// �ļ�˵���� ��������
// �������ڣ� 2018/02/26
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _MainTask_H_
#define _MainTask_H_

#include "TaskTemplate.h"

class MainTask : public TaskTemplate
{

public:
	MainTask();

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
