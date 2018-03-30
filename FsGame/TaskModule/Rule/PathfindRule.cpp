// -------------------------------------------
// �ļ����ƣ� PathfindRule.cpp
// �ļ�˵���� Ѱ·����
// �������ڣ� 2018/03/01
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "../Data/TaskLoader.h"
#include "FsGame\Define\GameDefine.h"
#include "..\TaskUtil.h"
#include "utils/string_util.h"
#include "Define/CommandDefine.h"
#include "PathfindRule.h"

// ִ�и�������
bool PathfindRule::ExecUpdate(IKernel *pKernel, const PERSISTID &self, const int task_id,
	const IVarList &args)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ȡ������
	const TaskBaseData *pData = TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// �����Ƿ�һ��
	if (!TaskUtilS::Instance()->IsTargetScene(pKernel, pData->scene_list))
	{
		return false;
	}

	// �����Ƿ�ƥ��
	Position position(pSelf->GetPosiX(), pSelf->GetPosiY(), pSelf->GetPosiZ(), pSelf->GetOrient());
	if (!TaskUtilS::Instance()->IsTargetPosition(pKernel, pData->posi_list, position))
	{
		return false;
	}

	// ������ȼ���
	std::string new_progress = "";
	bool is_complete = false;
	if (!Calculate(pKernel, self, task_id, new_progress, CVarList() << 1, is_complete))
	{
		return false;
	}

	// �޸Ľ���
	return SetProgress(pKernel, self, task_id, new_progress.c_str(), is_complete);
}

// �����Լ��
void PathfindRule::ExecCheck(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	// ���Ѱ·������ʱ������飩
	/*CVarList s2s_msg;
	s2s_msg << COMMAND_TASK_COMMAND
	<< S2S_TASK_SUBMSG_UPDATE
	<< TASK_RULE_PATHFINDING
	<< 1;
	pKernel->Command(self, self, s2s_msg);*/
}