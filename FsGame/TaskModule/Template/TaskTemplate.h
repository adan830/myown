// -------------------------------------------
// �ļ����ƣ� TaskTemplate.h
// �ļ�˵���� ��������ģ��
// �������ڣ� 2018/02/26
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _TaskTemplate_H_
#define _TaskTemplate_H_

#include "Fsgame/Define/header.h"
#include "../TaskDefine.h"

class TaskTemplate
{
protected:
	TaskTypes m_task_type;

public:
	TaskTemplate();

	// ȡ����������
	const TaskTypes GetTaskType();

public:
    // ��ʼ��
    virtual bool Init(IKernel *pKernel) = 0;

	// ����
	virtual bool Start(IKernel *pKernel, const PERSISTID &self) = 0;

	// �ɷ��ȡ����
	virtual bool CanAccept(IKernel *pKernel, const PERSISTID &self, 
		const int task_id);

	// ��ȡ����
	virtual bool DoAccept(IKernel *pKernel, const PERSISTID &self, 
		const int task_id);

	// �ɷ��������
	virtual bool CanUpdate(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// ��������
	virtual bool DoUpdate(IKernel *pKernel, const PERSISTID &self,
		const int task_id, const IVarList &args);

	// ������ɺ�
	void OnComplete(IKernel *pKernel, const PERSISTID &self, 
		const int task_id);

	// �����ύ��
	virtual void OnSubmit(IKernel *pKernel, const PERSISTID &self, 
		const int task_id) = 0;

	// ע�ᶨʱ��
	virtual void RegistResetTimer(IKernel *pKernel, const PERSISTID &self);

public:
	// ��ȡ������������ݲ�����д��
	virtual void OnAccept(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// �ɷ��ύ���������ݲ�����д��
	virtual bool CanSubmit(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// �ύ���������ݲ�����д��
	virtual bool DoSubmit(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

private:
	// ��ȡ��������
	virtual void AcceptPostTask(IKernel *pKernel, const PERSISTID &self,
		const int task_id) = 0;
};

#endif
