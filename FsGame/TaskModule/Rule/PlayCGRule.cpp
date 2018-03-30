// -------------------------------------------
// �ļ����ƣ� PlayCGRule.cpp
// �ļ�˵���� ����cg����
// �������ڣ� 2018/03/01
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "../Data/TaskLoader.h"
#include "PlayCGRule.h"
#include "Define/ServerCustomDefine.h"
#include "../Data/TaskData_Simple.h"
#include "CommonModule/CommRuleModule.h"
#include "utils/string_util.h"
#include "../TaskUtil.h"

// ִ�и�������
bool PlayCGRule::ExecUpdate(IKernel *pKernel, const PERSISTID &self, 
	const int task_id, const IVarList &args)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// CG
	const char *cg_id = args.StringVal(0);
	if (StringUtil::CharIsNull(cg_id))
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
	if (!StringUtil::CharIsEqual(pData->add_info.c_str(), cg_id))
	{
		return false;
	}

	// �����Ƿ�һ��
	if (!TaskUtilS::Instance()->IsTargetScene(pKernel, pData->scene_list))
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
void PlayCGRule::ExecCheck(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	const TaskData_Simple *pData = (TaskData_Simple *)TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return;
	}

	// ����CG
	CommRuleModule::m_pThis->PlayCG(pKernel, self, pData->add_info.c_str());
}