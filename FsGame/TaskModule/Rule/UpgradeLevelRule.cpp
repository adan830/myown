// -------------------------------------------
// �ļ����ƣ� UpgradeLevelRule.cpp
// �ļ�˵���� ��������
// �������ڣ� 2018/03/02
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "FsGame/Define/GameDefine.h"
#include "Define/CommandDefine.h"
#include "UpgradeLevelRule.h"

// �����Լ��
void UpgradeLevelRule::ExecCheck(IKernel *pKernel, const PERSISTID &self,
	const int task_id)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return;
	}
	
	// �����������
	CVarList s2s_msg;
	s2s_msg << COMMAND_TASK_COMMAND
			<< S2S_TASK_SUBMSG_UPDATE
			<< TASK_OPT_WAY_BY_RULE
			<< TASK_RULE_UPGRADE_LEVEL
			<< pSelf->QueryInt(FIELD_PROP_LEVEL);
	pKernel->Command(self, self, s2s_msg);
}