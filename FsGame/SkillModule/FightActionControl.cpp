//--------------------------------------------------------------------
// �ļ���:		FightActionControl.h
// ��  ��:		ս����Ϊ״̬����
// ˵  ��:		
//				
// ��������:	2016��04��06��
// ��������:	2016��04��06��
// ������:		  ( )
//    :	    
//--------------------------------------------------------------------
#include "FightActionControl.h"
#include "..\Interface\PropertyInterface.h"
#include "..\Define\FightDefine.h"
#include "..\Define\GameDefine.h"
#include "..\Interface\FightInterface.h"
#include "..\NpcBaseModule\AI\AIDefine.h"

// ��ʼ��
bool FightActionControl::Init(IKernel* pKernel)
{
	// �Ƴ�Ӳֱ״̬״̬����
	DECL_HEARTBEAT(FightActionControl::H_RemoveActionState);

	return true;
}

// �ı�ս����Ϊ״̬
bool FightActionControl::ChangeState(IKernel* pKernel, const PERSISTID& self, int nState, int nStiffTime)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ����NPC����
	if (pSelfObj->GetClassType() == TYPE_NPC && pSelfObj->QueryInt("NpcType") > NPC_TYPE_BUILDING)
	{
		return false;
	}

	// ����������
	int nDead = 0;
	if (FPropertyInstance->GetDead(pSelfObj, nDead) && 1 == nDead)
	{
		return false;
	}

	// ��ȡ��ǰ״̬
	int nCurState = 0;
	if (!FPropertyInstance->GetFightActionState(pSelfObj, nCurState))
	{
		return false;
	}

	// ǿ�ƻָ�Ϊ����״̬(ʹ�����ܼ��ܵ����⴦��)
	if (nCurState > FAS_NORMAL_STATE && FAS_NORMAL_STATE == nState)
	{
		// ȥ��ԭ����״̬
		DELETE_HEART_BEAT(pKernel, self, "FightActionControl::H_RemoveActionState");
		FightInterfaceInstance->RemoveControlBuffer(pKernel, self, CVarList() << CB_CANT_MOVE << CB_STIFF_STATE);

		// ���õ�ǰ״ֵ̬
		FPropertyInstance->SetFightActionState(pSelfObj, nState);
		return true;
	}

	// ���ȼ��ϵ�,����״̬�ı�
	if (nState < nCurState || FAS_NORMAL_STATE == nState)
	{
		return false;
	}

	if (nStiffTime <= 0)
	{
		char strInfo[128] = {0};
		SPRINTF_S(strInfo, "FightActionControl::ChangeState oldstate = %d, newstate = %d, result = %d", nCurState, nState, nStiffTime);
		::extend_warning(LOG_INFO, strInfo);
		return false;
	}

	// ȥ��ԭ����״̬
	DELETE_HEART_BEAT(pKernel, self, "FightActionControl::H_RemoveActionState");
	FightInterfaceInstance->RemoveControlBuffer(pKernel, self, CVarList() << CB_CANT_MOVE << CB_STIFF_STATE);

	
	// �����µ�
	FightInterfaceInstance->AddControlBuffer(pKernel, self, self, CVarList() << CB_CANT_MOVE << CB_STIFF_STATE, CONTROL_BUFF_FOREVER);
	pKernel->AddCountBeat(self, "FightActionControl::H_RemoveActionState", nStiffTime, 1);

	// ���õ�ǰ״ֵ̬
	FPropertyInstance->SetFightActionState(pSelfObj, nState);
	return true;
}

// ��ȡ��ǰ����Ϊ״̬
int FightActionControl::GetFightActionState(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	int nState = FAS_NORMAL_STATE;
	if(!FPropertyInstance->GetFightActionState(pSelfObj, nState))
	{
		return nState;
	}

	return nState;
}

// ����Ϊ����״̬
void FightActionControl::ResetNormalState(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if(NULL == pSelfObj)
	{
		return;
	}

	FightInterfaceInstance->RemoveControlBuffer(pKernel, self, CVarList() << CB_CANT_MOVE << CB_STIFF_STATE);
	FPropertyInstance->SetFightActionState(pSelfObj, FAS_NORMAL_STATE);
}

// �Ƴ�Ӳֱ״̬
int FightActionControl::H_RemoveActionState(IKernel* pKernel,const PERSISTID& self,int slice)
{
	FightActionControlSingleton::Instance()->ResetNormalState(pKernel, self);
	return 0;
}