// -------------------------------------------
// �ļ����ƣ� TaskBaseRule.cpp
// �ļ�˵���� �����������
// �������ڣ� 2018/02/26
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "TaskBaseRule.h"
#include "../Data/TaskLoader.h"
#include "FsGame\Define\GameDefine.h"
#include "..\TaskUtil.h"
#include "utils/string_util.h"
#include "utils/extend_func.h"
#include "Define/CommandDefine.h"

// ��ʼ��
bool TaskBaseRule::Init(IKernel *pKernel)
{
	return true;
}

// ִ�н�ȡ����
bool TaskBaseRule::ExecAccept(IKernel *pKernel, const PERSISTID &self, 
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

	CVarList row_value;
	row_value << task_id
			  << TASK_STATUS_DOING
			  << StringUtil::IntAsString(0);

	if (pPrgRec->AddRowValue(-1, row_value) < 0)
	{
		extend_warning(LOG_WARNING, "[%s][%d] add row failed:[%d]", __FILE__, __LINE__, task_id);
		return false;
	}

	return true;
}

// ִ�и�������
bool TaskBaseRule::ExecUpdate(IKernel *pKernel, const PERSISTID &self, const int task_id, 
	const IVarList &args)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	// ��֤�Ƿ�����ӽ���
	// TODO

	// ������ȼ���
	std::string new_progress = "";
	bool is_complete = false;
	if (!Calculate(pKernel, self, task_id, new_progress, args, is_complete))
	{
		return false;
	}

	// �޸Ľ���
	return SetProgress(pKernel, self, task_id, new_progress.c_str(), is_complete);
}

// �����Լ��
void TaskBaseRule::ExecCheck(IKernel *pKernel, const PERSISTID &self, 
	const int task_id)
{
	return;
}

// ������ɺ�
void TaskBaseRule::OnComplete(IKernel *pKernel, const PERSISTID &self, 
	const int task_id)
{

	// �����ض������޸ĵ�����
	// TODO
	return;
}

// ������ȼ���
bool TaskBaseRule::Calculate(IKernel *pKernel, const PERSISTID &self, 
	const int task_id, std::string &new_progress, const IVarList &args, bool& is_complete)
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

	const int exist_row = pPrgRec->FindInt(COLUMN_TASK_PROGRESS_REC_ID, task_id);
	if (exist_row < 0)
	{
		return false;
	}

	const TaskBaseData *pData = TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// ��ǰ����
	int cur_progress = StringUtil::StringAsInt(pPrgRec->QueryString(exist_row, COLUMN_TASK_PROGRESS_REC_PROGRESS));
	int incc_value = args.IntVal(0);// ��ֵ
	incc_value = incc_value > 0 ? incc_value : 1;
	
	// ����
	if (pData->calculate_mode == TASK_CALCULATE_MODE_ADD)
	{
		cur_progress += incc_value;
	}
	else if (pData->calculate_mode == TASK_CALCULATE_MODE_RESET)
	{
		cur_progress = incc_value;
	}
	
	if (cur_progress >= pData->count)
	{
		cur_progress = pData->count;
		is_complete = true;
	}

	new_progress = StringUtil::IntAsString(cur_progress);
	return true;
}

// �޸Ľ���
bool TaskBaseRule::SetProgress(IKernel *pKernel, const PERSISTID &self, 
	const int task_id, const char *new_progress, bool is_completed /*= true*/)
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

	const int exist_row = pPrgRec->FindInt(COLUMN_TASK_PROGRESS_REC_ID, task_id);
	if (exist_row < 0)
	{
		return false;
	}
	
	// ����ǰ����
	pPrgRec->SetString(exist_row, COLUMN_TASK_PROGRESS_REC_PROGRESS, new_progress);

	// �����
	if (is_completed)
	{
		// �����������Ϊ���ύ
		pPrgRec->SetInt(exist_row, COLUMN_TASK_PROGRESS_REC_STATUS, TASK_STATUS_DONE);

		// ���������
		CVarList s2s_msg;
		s2s_msg << COMMAND_TASK_ACTION_MSG
				<< TASK_ACTION_COMPLETE
				<< task_id;
		pKernel->Command(self, self, s2s_msg);
	}

	return true;
}
