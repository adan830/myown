//------------------------------------------------------------------------------
// �ļ���:      PVPRecAttacker.h
// ��  ��:		pvp������¼
// ˵  ��:
// ��������:    2017��10��17��
// ������:          
//------------------------------------------------------------------------------
#include "PVPRecAttacker.h"
#include "Define\CommandDefine.h"
#include "Define\Fields.h"
#include "utils\extend_func.h"
#include "FsGame\CommonModule\EnvirValueModule.h"
#include "utils\util_func.h"

bool PVPRecAttacker::Init(IKernel* pKernel)
{
	pKernel->AddEventCallback("player", "OnRecover", OnPlayerRecover);

	pKernel->AddIntCommandHook("player", COMMAND_BEDAMAGE, OnCommandBeDamage);
	DECL_CRITICAL(PVPRecAttacker::C_OnFightStateChanged);

	return true;
}

//��ȡ���������б� killerΪ��,����ȫ��������,��Ϊ�ջ��޳���
bool PVPRecAttacker::GetAttackerList(IKernel* pKernel, IVarList& outAttackerList, const PERSISTID& self, const PERSISTID& killer)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
		  
	outAttackerList.Clear();
	IRecord* pAttackerList = pSelfObj->GetRecord(FIELD_RECORD_PVP_ATTACKER_LIST);
	if (NULL == pAttackerList)
	{
		return false;
	}

	float fAssistDis = EnvirValueModule::EnvirQueryFloat(ENV_VALUE_ASSIST_ATTACK_DISTANCE);
	float fAssistDis2 = fAssistDis * fAssistDis;
	float fSelfX = pSelfObj->GetPosiX();
	float fSelfZ = pSelfObj->GetPosiZ();

	int nRows = pAttackerList->GetRows();
	LoopBeginCheck(a);
	for (int i = 0; i < nRows;++i)
	{
		LoopDoCheck(a);
		PERSISTID attacker = pAttackerList->QueryObject(i, COLUMN_PVP_ATTACKER_LIST_OBJECT);
		IGameObj* pAttackerObj = pKernel->GetGameObj(attacker);
		if (NULL == pAttackerObj || attacker == killer)
		{
			continue;
		}

		float fAttackerX = pAttackerObj->GetPosiX();
		float fAttackerZ = pAttackerObj->GetPosiZ();

		float fDisDiff2 = util_dot_distance2(fSelfX, fSelfZ, fAttackerX, fAttackerZ);
		if (fDisDiff2 < fAssistDis2)
		{
			outAttackerList << attacker;
		}
	}

	return outAttackerList.GetCount() > 0;
}

/// \brief ����
int PVPRecAttacker::OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	// ���ս��״̬�ص�
	pKernel->AddCritical(self, FIELD_PROP_FIGHT_STATE, "PVPRecAttacker::C_OnFightStateChanged");
	return 0;
}

// ��Ӧ���˺�
int PVPRecAttacker::OnCommandBeDamage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	PERSISTID realattacker = get_pet_master(pKernel, sender);
	IGameObj* pAttacker = pKernel->GetGameObj(realattacker);
	IGameObj* pTargetObj = pKernel->GetGameObj(self);
	if (NULL == pAttacker || NULL == pTargetObj || pAttacker->GetClassType() != TYPE_PLAYER || pTargetObj->GetClassType() != TYPE_PLAYER)
	{
		return 0;
	}

	IRecord* pAttackerList = pTargetObj->GetRecord(FIELD_RECORD_PVP_ATTACKER_LIST);
	if (NULL == pAttackerList)
	{
		return 0;
	}

	int nRowIndex = pAttackerList->FindObject(COLUMN_PVP_ATTACKER_LIST_OBJECT, pAttacker->GetObjectId());
	if (-1 == nRowIndex)
	{
		pAttackerList->AddRowValue(-1, CVarList() << pAttacker->GetObjectId());
	}
	return 0;
}

//ս��״̬�����ı�
int PVPRecAttacker::C_OnFightStateChanged(IKernel *pKernel, const PERSISTID &self, const char *property, const IVar &old)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	IRecord* pAttackerList = pSelfObj->GetRecord(FIELD_RECORD_PVP_ATTACKER_LIST);
	if (NULL == pAttackerList)
	{
		return 0;
	}
	pAttackerList->ClearRow();

	return 0;
}