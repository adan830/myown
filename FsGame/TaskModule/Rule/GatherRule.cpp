// -------------------------------------------
// �ļ����ƣ� GatherRule.cpp
// �ļ�˵���� �ɼ�����
// �������ڣ� 2018/03/01
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "GatherRule.h"
#include "../Data/TaskLoader.h"
#include "utils/string_util.h"
#include "../TaskUtil.h"
#include "../Data/TaskData_Simple.h"


// ִ�и�������
bool GatherRule::ExecUpdate(IKernel *pKernel, const PERSISTID &self, const int task_id,
	const IVarList &args)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// Ŀ����Ʒ
	const char *item_id = args.StringVal(0);
	if (StringUtil::CharIsNull(item_id))
	{
		return false;
	}

	// ȡ������
	const TaskData_Simple *pData = (TaskData_Simple*)TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// ��ƥ��
	if (!StringUtil::CharIsEqual(pData->add_info.c_str(), item_id))
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
