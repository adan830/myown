// -------------------------------------------
// �ļ����ƣ� BranchTask.cpp
// �ļ�˵���� ֧������
// �������ڣ� 2018/02/28
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "BranchTask.h"
#include "../Data/TaskLoader.h"
#include "..\TaskUtil.h"
#include "FsGame\Define\GameDefine.h"
#include "../TaskManager.h"

BranchTask::BranchTask()
{
	m_task_type = TASK_TYPE_BRANCH;
}

// ��ʼ��
bool BranchTask::Init(IKernel *pKernel)
{
	// �ͻ���׼������
	pKernel->AddEventCallback("player", "OnReady", BranchTask::OnReady, -10);
	return true;
}

// ����
bool BranchTask::Start(IKernel *pKernel, const PERSISTID &self)
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

	// ��������
	const TaskConfig *task_config = TaskLoader::instance().GetTaskConfig(m_task_type);
	if (NULL == task_config)
	{
		return false;
	}

	if (pConfRec->FindInt(COLUMN_TASK_CONFIG_REC_TYPE, m_task_type) < 0)
	{
		// �������ñ�
		CVarList row_value;
		row_value << TASK_TYPE_BRANCH
				  << 0;
		pConfRec->AddRowValue(-1, row_value);
	}
	
	return true;
}

// �����ύ��
void BranchTask::OnSubmit(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return;
	}

	// ����������
	AcceptPostTask(pKernel, self, task_id);
}

// �ͻ���׼������
int BranchTask::OnReady(IKernel* pKernel, const PERSISTID& self,
	const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ֧��ģ��
	TaskTemplate *pTemplate = TaskManager::m_pThis->GetTemplate(TASK_TYPE_BRANCH);
	if (NULL == pTemplate)
	{
		return 0;
	}

	// ����֧��
	pTemplate->Start(pKernel, self);

	return 0;
}

// ��ȡ��������
void BranchTask::AcceptPostTask(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	// ����������
	TaskUtilS::Instance()->AcceptPostTask(pKernel, self, task_id);
}
