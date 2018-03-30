// -------------------------------------------
// �ļ����ƣ� TaskTemplate.cpp
// �ļ�˵���� ��������ģ��
// �������ڣ� 2018/02/26
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "TaskTemplate.h"
#include "../Data/TaskLoader.h"
#include "FsGame\Define\GameDefine.h"
#include "..\TaskUtil.h"
#include "..\TaskManager.h"
#include "Define\CommandDefine.h"
#include "Define\ServerCustomDefine.h"
#include "SystemFunctionModule/RewardModule.h"

TaskTemplate::TaskTemplate() : m_task_type(TASK_TYPE_NONE)
{

}

// ȡ����������
const TaskTypes TaskTemplate::GetTaskType()
{
	return m_task_type;
}

// �ɷ��ȡ����
bool TaskTemplate::CanAccept(IKernel *pKernel, const PERSISTID &self, 
	const int task_id)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ��������
	const TaskBaseData *pData = TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// ������ȱ��Ѵ�����
	IRecord *pProgRec = pSelf->GetRecord(FIELD_RECORD_TASK_PROGRESS_REC);// TODO
	if (NULL == pProgRec)
	{
		return false;
	}

	if (pProgRec->GetRows() == pProgRec->GetRowMax())
	{
		return false;
	}

	// ��������ܵȼ�
	const int level = pSelf->QueryInt(FIELD_PROP_LEVEL);
	if (level < pData->accept_min_level 
		&& level > pData->accept_max_level)
	{
		return false;
	}

	// ��ӵ�и�����
	const int task_status = TaskUtilS::Instance()->QueryTaskStatus(pKernel, self, task_id);
	if (TASK_STATUS_DOING == task_status  
		|| TASK_STATUS_DONE == task_status)
	{
		return false;
	}

	// ǰ������δ���
	if (!TaskUtilS::Instance()->IsPrevTaskSubmit(pKernel, self, task_id))
	{
		return false;
	}


	return true;
}

// ��ȡ����
bool TaskTemplate::DoAccept(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ��������
	const TaskBaseData *pData = TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// �������
	TaskBaseRule *pRule = TaskManager::m_pThis->GetRule(TaskRules(pData->rule));
	if (NULL == pRule)
	{
		return false;
	}

	// ��ȡ�ɹ�
	if (pRule->ExecAccept(pKernel, self, task_id))
	{
		// ֪ͨ�����ѽ�ȡ
		CVarList s2s_msg;
		s2s_msg << COMMAND_TASK_ACTION_MSG
				<< TASK_ACTION_ACCEPT
				<< task_id;
		pKernel->Command(self, self, s2s_msg);

		return true;
	}

	return false;
}

// �ɷ��������
bool TaskTemplate::CanUpdate(IKernel *pKernel, const PERSISTID &self, 
	const int task_id)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// �����ڽ�����
	const int task_status = TaskUtilS::Instance()->QueryTaskStatus(pKernel, self, task_id);
	if (TASK_STATUS_DOING != task_status)
	{
		return false;
	}

	return true;
}

// ��������
bool TaskTemplate::DoUpdate(IKernel *pKernel, const PERSISTID &self, 
	const int task_id, const IVarList &args)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ��������
	const TaskBaseData *pData = TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// ȡ���������
	TaskBaseRule *pRule = TaskManager::m_pThis->GetRule(TaskRules(pData->rule));
	if (NULL == pRule)
	{
		return false;
	}

	return pRule->ExecUpdate(pKernel, self, task_id, args);
}

// ������ɺ�
void TaskTemplate::OnComplete(IKernel *pKernel, const PERSISTID &self, 
	const int task_id)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return;
	}

	// ��������
	const TaskBaseData *pData = TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return;
	}

	// ȡ���������
	TaskBaseRule *pRule = TaskManager::m_pThis->GetRule(TaskRules(pData->rule));
	if (NULL == pRule)
	{
		return;
	}

	// �������
	TaskUtilS::Instance()->CleanGroupClone(pKernel, self, task_id);

	// ������ɻص�
	pRule->OnComplete(pKernel, self, task_id);

	// ��¼���������־
	// TODO

	// ��¼����ɴ���
	TaskUtilS::Instance()->RecordTaskNum(pKernel, self, pData->type);

	// �ɷ��Զ��ύ
	if (!pData->IsAutoSubmit())
	{
		return;
	}

	// �ύ����
	if (CanSubmit(pKernel, self, task_id))
	{
		DoSubmit(pKernel, self, task_id);
	}

	return;
}

// ע�ᶨʱ��
void TaskTemplate::RegistResetTimer(IKernel *pKernel, const PERSISTID &self)
{

}

// ��ȡ�������
void TaskTemplate::OnAccept(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return;
	}

	// ��������
	const TaskBaseData *pData = TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return;
	}

	// ������������
	if (pData->type == TASK_TYPE_MAIN)
	{
		pSelf->SetInt(FIELD_PROP_CUR_MAIN_TASK, task_id);
	}

	// �Զ�Ѱ·
	if (pData->IsAutoPathFinding())
	{
		CVarList s2c_msg;
		s2c_msg << SERVER_CUSTOMMSG_TASK_MSG
				<< S2C_TASK_SUMMSG_FINDPATH
				<< task_id;
		pKernel->Custom(self, s2c_msg);
	}

	// �����Լ��
	TaskUtilS::Instance()->SelfCheck(pKernel, self, task_id);

	// ����������鸱��
	TaskUtilS::Instance()->CreateGroupClone(pKernel, self, task_id);

	// �����г�������
	TaskUtilS::Instance()->AutoSwitchScene(pKernel, self, task_id);
}

// �ɷ��ύ����
bool TaskTemplate::CanSubmit(IKernel *pKernel, const PERSISTID &self, 
	const int task_id)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ���񲻿��ύ
	const int task_status = TaskUtilS::Instance()->QueryTaskStatus(pKernel, self, task_id);
	if (TASK_STATUS_DONE != task_status)
	{
		return false;
	}

	return true;
}

// �ύ����
bool TaskTemplate::DoSubmit(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ���ȱ�
	IRecord *pPrgRec = pSelf->GetRecord(FIELD_RECORD_TASK_PROGRESS_REC);
	if (NULL == pPrgRec)
	{
		return false;
	}

	// ��������
	const TaskBaseData *pData = TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// ��ȡ����
	if (pData->IsHaveReward())
	{
		if (RewardModule::m_pRewardInstance->RewardPlayerById(pKernel, self, pData->reward_id))
		{
			// �Ƿ���Ҫ�ͻ��˴򿪽�����ʾ
			if (pData->IsShowReward())
			{
				// ֪ͨ�ͻ�����ʾ����
				CVarList s2c_msg;
				s2c_msg << SERVER_CUSTOMMSG_TASK_MSG
						<< S2C_TASK_SUMMSG_SHOW_REWARD
						<< pData->reward_id
						<< 1.0f;
				pKernel->Custom(self, s2c_msg);
			}
		}
		else // ��ȡʧ��
		{
			// ��Ϣ֪ͨ
			// TODO
			// ʧ����־��¼
			// TODO

			return false;
		}		
	}

	// ��¼�����ύ�ɹ�����־
	// TODO

	// ɾ������
	const int exist_row = pPrgRec->FindInt(COLUMN_TASK_PROGRESS_REC_ID, task_id);
	if (exist_row >= 0)
	{
		pPrgRec->RemoveRow(exist_row);

		// ��¼����ɾ������־
		// TODO
	}	

	// ֪ͨ�������ύ
	CVarList s2s_msg;
	s2s_msg << COMMAND_TASK_ACTION_MSG
			<< TASK_STATUS_SUBMITTED
			<< task_id;
	pKernel->Command(self, self, s2s_msg);

	return true;
}

