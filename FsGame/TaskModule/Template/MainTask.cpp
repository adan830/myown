// -------------------------------------------
// �ļ����ƣ� MainTask.cpp
// �ļ�˵���� ��������
// �������ڣ� 2018/02/26
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "MainTask.h"
#include "../Data/TaskLoader.h"
#include "..\TaskUtil.h"
#include "FsGame\Define\GameDefine.h"
#include "../TaskManager.h"

MainTask::MainTask()
{
	m_task_type = TASK_TYPE_MAIN;
}

// ��ʼ��
bool MainTask::Init(IKernel *pKernel)
{
	// �ͻ���׼������
	pKernel->AddEventCallback("player", "OnReady", MainTask::OnReady, -10);
	return true;
}

// ����
bool MainTask::Start(IKernel *pKernel, const PERSISTID &self)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ���ñ�
	IRecord *pConfRec = pSelf->GetRecord(FIELD_RECORD_TASK_CONFIG_REC);
	if (NULL == pConfRec)
	{
		return false;
	}

	// ��ȡ��һ����������
	int cur_main_task = pSelf->QueryInt(FIELD_PROP_CUR_MAIN_TASK);
	if (cur_main_task != 0)
	{
		return false;
	}

	// ��������
	const TaskConfig *task_config = TaskLoader::instance().GetTaskConfig(TASK_TYPE_MAIN);
	if (NULL == task_config)
	{
		return false;
	}

	if (pConfRec->FindInt(COLUMN_TASK_CONFIG_REC_TYPE, m_task_type) < 0)
	{
		// �������ñ�
		CVarList row_value;
		row_value << TASK_TYPE_MAIN
				  << 0;
		pConfRec->AddRowValue(-1, row_value);
	}	

	// ��ȡ����
	if (CanAccept(pKernel, self, task_config->first_task))
	{
		return DoAccept(pKernel, self, task_config->first_task);
	}

	return false;
}

// �ɷ��ȡ����
bool MainTask::CanAccept(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	// ���������ȡ����
	// ����ɹ�
	if (TaskUtilS::Instance()->QueryTaskStatus(pKernel, self, task_id) == TASK_STATUS_SUBMITTED)
	{
		return false;
	}
	
	return TaskTemplate::CanAccept(pKernel, self, task_id);
}

// �����ύ��
void MainTask::OnSubmit(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return;
	}

	// �ύ��
	IRecord *pSubmitRec = pSelf->GetRecord(FIELD_RECORD_TASK_SUBMIT_REC);
	if (NULL == pSubmitRec)
	{
		return;
	}

	// ��¼���ύ����
	pSubmitRec->AddRowValue(-1, CVarList() << task_id);

	// ����������
	AcceptPostTask(pKernel, self, task_id);
}

// �ͻ���׼������
int MainTask::OnReady(IKernel* pKernel, const PERSISTID& self, 
	const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ����ģ��
	TaskTemplate *pTemplate = TaskManager::m_pThis->GetTemplate(TASK_TYPE_MAIN);
	if (NULL == pTemplate)
	{
		return 0;
	}

	// ��������
	pTemplate->Start(pKernel, self);

	return 0;
}

// ��ȡ��������
void MainTask::AcceptPostTask(IKernel *pKernel, const PERSISTID &self, 
	const int task_id)
{
	// ����������
	TaskUtilS::Instance()->AcceptPostTask(pKernel, self, task_id);
}
