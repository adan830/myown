// -------------------------------------------
// �ļ����ƣ� KillNpcRule.cpp
// �ļ�˵���� ��ɱ����
// �������ڣ� 2018/02/27
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "KillNpcRule.h"
#include "../Data/TaskLoader.h"
#include "FsGame\Define\GameDefine.h"
#include "utils/string_util.h"
#include "utils/extend_func.h"
#include "Define/CommandDefine.h"
#include "../Data/TaskData_KillNpc.h"
#include "../TaskUtil.h"


// ִ�н�ȡ����
bool KillNpcRule::ExecAccept(IKernel *pKernel, const PERSISTID &self,
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

	// ȡ�û�ɱnpc����
	const TaskData_KillNpc *pData = (TaskData_KillNpc*)TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	std::string cur_progress_str = "";
	
	// ��ɱ�б�����
	if (pData->kill_way == KILL_NPC_STAT_WAY_ALL)
	{
		TaskUtilS::Instance()->ConvertCEL2Str(pData->npc_list, cur_progress_str, true);
	}

	CVarList row_value;
	row_value << task_id
			  << TASK_STATUS_DOING
			  << cur_progress_str;

	if (pPrgRec->AddRowValue(-1, row_value) < 0)
	{
		extend_warning(LOG_WARNING, "[%s][%d] add row failed:[%d]", __FILE__, __LINE__, task_id);
		return false;
	}

	return true;
}

// ִ�и�������
bool KillNpcRule::ExecUpdate(IKernel *pKernel, const PERSISTID &self, const int task_id,
	const IVarList &args)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	// Ŀ��npc
	const char *target_npc = args.StringVal(0);
	if (StringUtil::CharIsNull(target_npc))
	{
		return false;
	}

	// ȡ�û�ɱnpc����
	const TaskData_KillNpc *pData = (TaskData_KillNpc*)TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// �����Ƿ�һ��
	if (!TaskUtilS::Instance()->IsTargetScene(pKernel, pData->scene_list))
	{
		return false;
	}

	// ��ɱ�б����У���Ҫ���Ŀ��npc�Ƿ����ڸ�����
	if (pData->kill_way == KILL_NPC_STAT_WAY_ALL)
	{
		CoupleElement temp_data(target_npc);
		CoupleElementList::const_iterator find_it = find(
			pData->npc_list.begin(),
			pData->npc_list.end(),
			temp_data);
		if (find_it == pData->npc_list.end())
		{
			// δ�ҵ�
			return false;
		}
	}

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

// ������ȼ���
bool KillNpcRule::Calculate(IKernel *pKernel, const PERSISTID &self,
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

	// Ŀ������
	const char *target_npc_id = args.StringVal(0);
	const int incc_value = args.IntVal(1);// ��ֵ
	if (StringUtil::CharIsNull(target_npc_id))
	{
		return false;
	}

	const int exist_row = pPrgRec->FindInt(COLUMN_TASK_PROGRESS_REC_ID, task_id);
	if (exist_row < 0)
	{
		return false;
	}

	// ȡ�û�ɱnpc����
	const TaskData_KillNpc *pData = (TaskData_KillNpc*)TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// ��ǰ����
	const char *cur_progress_str = pPrgRec->QueryString(exist_row, COLUMN_TASK_PROGRESS_REC_PROGRESS);
	CoupleElementList cur_progress_list;
	TaskUtilS::Instance()->ConvertStr2CEL(cur_progress_str, cur_progress_list);
	
	// ��ɱ�б�����
	if (pData->kill_way == KILL_NPC_STAT_WAY_ALL)
	{
		CoupleElement temp_data(target_npc_id);
		CoupleElementList::iterator it = find(
			cur_progress_list.begin(),
			cur_progress_list.end(),
			temp_data);

		// δ�ҵ�
		if (it == cur_progress_list.end())
		{
			return false;
		}

		it->num += incc_value;
	}

	CoupleElementList::const_iterator start_it = cur_progress_list.begin();
	CoupleElementList::const_iterator end_it = cur_progress_list.end();

	bool all_done = true;
	LoopBeginCheck(a);
	for (; start_it != end_it; ++start_it)
	{
		LoopDoCheck(a);
		// Ŀ�����
		CoupleElement ele(start_it->id.c_str());
		CoupleElementList::const_iterator find_it = find(
			pData->npc_list.begin(),
			pData->npc_list.end(),
			ele);
		if (find_it == pData->npc_list.end())
		{
			continue;
		}

		// ��ǰֵС��Ŀ��ֵ
		if (start_it->num < find_it->num)
		{
			all_done = false;
			break;
		}
	}
	
	is_complete = all_done;
	TaskUtilS::Instance()->ConvertCEL2Str(cur_progress_list, new_progress);
	return true;
}
