// -------------------------------------------
// �ļ����ƣ� DailyTask.cpp
// �ļ�˵���� �ճ�����
// �������ڣ� 2018/02/28
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "DailyTask.h"
#include "../Data/TaskLoader.h"
#include "..\TaskUtil.h"
#include "FsGame\Define\GameDefine.h"
#include "../TaskManager.h"
#include "Define/CommandDefine.h"
#include "SystemFunctionModule/ResetTimerModule.h"
#include "SystemFunctionModule/ActivateFunctionModule.h"

DailyTask::DailyTask()
{
	m_task_type = TASK_TYPE_DAILY;
}

// ��ʼ��
bool DailyTask::Init(IKernel *pKernel)
{
	// ���ܿ���
	pKernel->AddIntCommandHook("player", COMMAND_MSG_ACTIVATE_FUNCTION, DailyTask::OnCommandActiveFunc);

	DECL_RESET_TIMER(RESET_DAILY_TASK, DailyTask::ResetTask);
	return true;
}

// ����
bool DailyTask::Start(IKernel *pKernel, const PERSISTID &self)
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

	// ����δ����
	if (!ActivateFunctionModule::m_pInstance->CheckActivateFunction(pKernel, self, AFM_DAILY_TASK))
	{
		return false;
	}

	// �ѿ���
	if (pConfRec->FindInt(COLUMN_TASK_CONFIG_REC_TYPE, m_task_type) >= 0)
	{
		return false;
	}

	// �������ñ�
	CVarList row_value;
	row_value << m_task_type
			  << 0;
	pConfRec->AddRowValue(-1, row_value);

	// �������
	const int task_id = TaskUtilS::Instance()->RandomTask(pKernel, self, m_task_type);

	// ��ȡ����
	if (CanAccept(pKernel, self, task_id))
	{
		return DoAccept(pKernel, self, task_id);
	}

	return false;
}

// �ɷ��ȡ����
bool DailyTask::CanAccept(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	// �ճ������ȡ����
	// �Ƿ���
	if (!TaskUtilS::Instance()->IsStart(pKernel, self, m_task_type))
	{
		return false;
	}

	// �����Ƿ�����
	if (!TaskUtilS::Instance()->IsHaveNumOfType(pKernel, self, m_task_type))
	{
		return false;
	}

	// �Ƿ���ͬ��������
	if (TaskUtilS::Instance()->IsHaveOfType(pKernel, self, m_task_type))
	{
		return false;
	}
	
	return TaskTemplate::CanAccept(pKernel, self, task_id);
}

// �����ύ��
void DailyTask::OnSubmit(IKernel *pKernel, const PERSISTID &self,
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

// ע�ᶨʱ��
void DailyTask::RegistResetTimer(IKernel *pKernel, const PERSISTID &self)
{
	// ��ʱ��ע��
	REGIST_RESET_TIMER(pKernel, self, RESET_DAILY_TASK);
}

// ���ܿ��� 
int DailyTask::OnCommandActiveFunc(IKernel *pKernel, const PERSISTID &self, 
	const PERSISTID &sender, const IVarList &args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	const int func_type = args.IntVal(1);
	if (func_type != AFM_DAILY_TASK)
	{
		return 0;
	}

	// �ճ�ģ��
	TaskTemplate *pTemplate = TaskManager::m_pThis->GetTemplate(TASK_TYPE_DAILY);
	if (NULL == pTemplate)
	{
		return 0;
	}

	// �����ճ�
	pTemplate->Start(pKernel, self);

	return 0;
}

// �ճ�����ʱ��
int DailyTask::ResetTask(IKernel *pKernel, const PERSISTID &self, int slice)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return 0;
	}

	// ���ñ�
	IRecord *pConfRec = pSelf->GetRecord(FIELD_RECORD_TASK_CONFIG_REC);
	if (NULL == pConfRec)
	{
		return 0;
	}

	// ��������
	const TaskConfig *task_config = TaskLoader::instance().GetTaskConfig(TASK_TYPE_DAILY);
	if (NULL == task_config)
	{
		return 0;
	}

	TaskTemplate *pTemplate = TaskManager::m_pThis->GetTemplate(TASK_TYPE_DAILY);
	if (NULL == pTemplate)
	{
		return 0;
	}

	const int exist_row = pConfRec->FindInt(COLUMN_TASK_CONFIG_REC_TYPE, TASK_TYPE_DAILY);

	// δ����������һ�ο������
	if (exist_row < 0)
	{
		pTemplate->Start(pKernel, self);
	}
	else // �ѿ��������ô���
	{
		pConfRec->SetInt(exist_row, COLUMN_TASK_CONFIG_REC_COUNT, 0);

		// �������
		const int task_id = TaskUtilS::Instance()->RandomTask(pKernel, self, TASK_TYPE_DAILY);

		// ��ȡ����
		if (pTemplate->CanAccept(pKernel, self, task_id))
		{
			pTemplate->DoAccept(pKernel, self, task_id);
		}
	}

	return 0;
}

// ��ȡ��������
void DailyTask::AcceptPostTask(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	// ����������
	const int next_task_id = TaskUtilS::Instance()->RandomTask(pKernel, self, m_task_type);
	if (CanAccept(pKernel, self, next_task_id))
	{
		DoAccept(pKernel, self, next_task_id);
	}
}
