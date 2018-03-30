// -------------------------------------------
// �ļ����ƣ� TalkRule.cpp
// �ļ�˵���� �Ի�����
// �������ڣ� 2018/03/01
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "TalkRule.h"
#include "../Data/TaskLoader.h"
#include "utils/string_util.h"
#include "../TaskUtil.h"
#include "../Data/TaskData_Simple.h"


// ִ�и�������
bool TalkRule::ExecUpdate(IKernel *pKernel, const PERSISTID &self, const int task_id,
	const IVarList &args)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	// �Ի�Npc
	const char *target_npc = args.StringVal(0);
	if (StringUtil::CharIsNull(target_npc))
	{
		return false;
	}

	// ȡ�û�ɱnpc����
	const TaskData_Simple *pData = (TaskData_Simple*)TaskLoader::instance().GetTaskBase(task_id);
	if (NULL == pData)
	{
		return false;
	}

	// ��ƥ��
	if (!StringUtil::CharIsEqual(pData->add_info.c_str(), target_npc))
	{
		return false;
	}

	// �����Ƿ�һ��
	if (!TaskUtilS::Instance()->IsTargetScene(pKernel, pData->scene_list))
	{
		return false;
	}

	// ���ҷ�Χ��Npc
	CVarList around_npc_list;
	pKernel->GetAroundList(self, TALK_VERIFY_DISTANCE, TYPE_NPC, 0, around_npc_list);

	bool verify = false;
	LoopBeginCheck(a);
	for (int i = 0; i < (int)around_npc_list.GetCount(); ++i)
	{
		LoopDoCheck(a);
		PERSISTID npc = around_npc_list.ObjectVal(i);
		IGameObj *pNpc = pKernel->GetGameObj(npc);
		if (NULL == pNpc)
		{
			continue;
		}

		if (!StringUtil::CharIsEqual(pNpc->GetConfig(), target_npc))
		{
			continue;
		}

		// �����npc�ľ��볬����ȫ����
		if (pKernel->Distance2D(self, npc) > TALK_VERIFY_DISTANCE)
		{
			continue;
		}

		verify = true;
		break;
	}

	// ��֤δͨ��
	if (!verify)
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
