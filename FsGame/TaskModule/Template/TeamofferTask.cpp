// -------------------------------------------
// �ļ����ƣ� TeamofferTask.cpp
// �ļ�˵���� ��������
// �������ڣ� 2018/04/02
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "TeamofferTask.h"
#include "../Data/TaskLoader.h"
#include "..\TaskUtil.h"
#include "FsGame\Define\GameDefine.h"
#include "../TaskManager.h"
#include "Define/CommandDefine.h"
#include "SystemFunctionModule/ResetTimerModule.h"
#include "SystemFunctionModule/ActivateFunctionModule.h"

TeamofferTask::TeamofferTask()
{
	m_task_type = TASK_TYPE_TEAMOFFER;
}

// ��ʼ��
bool TeamofferTask::Init(IKernel *pKernel)
{
	// ���ܿ���
	pKernel->AddIntCommandHook("player", COMMAND_MSG_ACTIVATE_FUNCTION, TeamofferTask::OnCommandActiveFunc);

	DECL_RESET_TIMER(RESET_DAILY_TASK, TeamofferTask::ResetTask);
	return true;
}

// ����
bool TeamofferTask::Start(IKernel *pKernel, const PERSISTID &self)
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

	// �ѿ���
	if (!ActivateFunctionModule::m_pInstance->CheckActivateFunction(pKernel, self, AFM_TEAMOFFER_TASK))
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

	return true;
}

// ע�ᶨʱ��
void TeamofferTask::RegistResetTimer(IKernel *pKernel, const PERSISTID &self)
{
	// ��ʱ��ע��
	REGIST_RESET_TIMER(pKernel, self, RESET_TEAMOFFER_TASK);
}

// �����ύ��
void TeamofferTask::OnSubmit(IKernel *pKernel, const PERSISTID &self, 
	const int task_id)
{

}

// ���ܿ��� 
int TeamofferTask::OnCommandActiveFunc(IKernel *pKernel, const PERSISTID &self,
	const PERSISTID &sender, const IVarList &args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	const int func_type = args.IntVal(1);
	if (func_type != AFM_TEAMOFFER_TASK)
	{
		return 0;
	}

	// ģ��
	TaskTemplate *pTemplate = TaskManager::m_pThis->GetTemplate(TASK_TYPE_TEAMOFFER);
	if (NULL == pTemplate)
	{
		return 0;
	}

	// ����
	pTemplate->Start(pKernel, self);

	return 0;
}

// ��������ʱ��
int TeamofferTask::ResetTask(IKernel *pKernel, const PERSISTID &self, int slice)
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
	const TaskConfig *task_config = TaskLoader::instance().GetTaskConfig(TASK_TYPE_TEAMOFFER);
	if (NULL == task_config)
	{
		return 0;
	}

	TaskTemplate *pTemplate = TaskManager::m_pThis->GetTemplate(TASK_TYPE_TEAMOFFER);
	if (NULL == pTemplate)
	{
		return 0;
	}

	const int exist_row = pConfRec->FindInt(COLUMN_TASK_CONFIG_REC_TYPE, TASK_TYPE_TEAMOFFER);

	// δ����������һ�ο������
	if (exist_row < 0)
	{
		pTemplate->Start(pKernel, self);
	}
	else // �ѿ��������ô���
	{
		pConfRec->SetInt(exist_row, COLUMN_TASK_CONFIG_REC_COUNT, 0);
	}

	return 0;
}

// ��ȡ��������
void TeamofferTask::AcceptPostTask(IKernel *pKernel, const PERSISTID &self, 
	const int task_id)
{

}
