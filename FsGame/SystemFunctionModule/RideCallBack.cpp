//--------------------------------------------------------------------
// �ļ���:		RideCallBack.cpp
// ��  ��:		����ص�����
// ˵  ��:		
// ��������:	2014��12��32��
// ������:		
// �޸���:		 tongzt
//--------------------------------------------------------------------

#include "RideModule.h"
#include "FsGame/Define/LogDefine.h"
#include "FsGame/Define/ViewDefine.h"
#include "FsGame/Define/ModifyPackDefine.h"
#include "FsGame/SkillModule/Impl/SkillFlow.h"
#include "../SkillModule/Impl/SkillDataQueryModule.h"
#include "../Define/BufferDefine.h"

#include "utils/extend_func.h"
#include "utils/string_util.h"
#include "../Define/Fields.h"
#include "../CommonModule/SwitchManagerModule.h"


/*!
* @brief	�������
* @param	pKernel ����ָ��
* @param	self    �Լ�
* @param	sender
* @param	args    ����
* @return	bool
*/
int RideModule::OnPlayerRecover(IKernel* pKernel,
	const PERSISTID& self,
	const PERSISTID& sender,
	const IVarList& args)
{
	return m_pRideModule->PlayerOnline(pKernel, self);
}

/*!
* @brief	��ҽ�����Ϸ
* @param	pKernel ����ָ��
* @param	self    �Լ�
* @param	sender
* @param	args    ����
* @return	bool
*/
int RideModule::OnPlayerReady(IKernel* pKernel,
	const PERSISTID& self,
	const PERSISTID& sender,
	const IVarList& args)
{
	// �ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return 0;
	}

	// �������(online_flag == 1)
	int online_flag = pSelf->QueryInt("Online");

	if (!pKernel->FindViewport(self, VIEWPORT_RIDE) && online_flag == 1)
	{
		PERSISTID box = pKernel->GetChild(self, L"RideBox");
		if (pKernel->Exists(box))
		{
			pKernel->AddViewport(self, VIEWPORT_RIDE, box);
		}
		else
		{
			::extend_warning(pKernel, /*LOG_WARNING, */"[RideModule::OnPlayerReady]: RideBox not Exists");
		}
	}

	return 0;
}

/*!
* @brief	���ﱳ������
* @param	pKernel ����ָ��
* @param	self    �Լ�
* @param	sender
* @param	args    ����
* @return	bool
*/
int RideModule::OnRideBoxCreate(IKernel* pKernel,
	const PERSISTID& ridebox,
	const PERSISTID& sender,
	const IVarList& args)
{
	// �ж�
	IGameObj* pRidebox = pKernel->GetGameObj(ridebox);
	if (NULL == pRidebox)
	{
		return 0;
	}

	// ��������
	pRidebox->SetWideStr("Name", L"RideBox");

	return 0;
}

/*!
* @brief	���ﱳ��Class�����ص�
* @param	pKernel ����ָ��
* @param	index   ����
* @return	int
*/
int RideModule::OnRideBoxClassCreate(IKernel* pKernel,
	int index)
{
	// �������Ա���
	pKernel->SetSaving(index, "Name", true);

	return 0;
}

/*!
* @brief	�ͷż��ܿ�ʼ�ص�
* @param	pKernel ����ָ��
* @param	self   �Լ�
* @param	sender ����target
* @param	args   ����
* @return	int
*/
int RideModule::OnCommandPlayerBeginSkill(IKernel * pKernel,
	const PERSISTID & self,
	const PERSISTID & sender,
	const IVarList & args)
{
	// �ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return 0;
	}

	// ��ȡ���ܶ���
	PERSISTID skill = args.ObjectVal(1);
	if (!pKernel->Exists(skill))
	{
		return 0;
	}

	// �ظ�HP���ܲ��ܴ������
	const char* strSkill = pKernel->GetConfig(skill);
	if (StringUtil::CharIsNull(strSkill) || strcmp(strSkill, RESTORE_HP_SKILL_ID) == 0)
	{
		return 0;
	}

	// ��������
	RideModule::m_pRideModule->DownRide(pKernel, self);

	return 0;
}

/*!
* @brief	�ͷż�����ɻص�
* @param	pKernel ����ָ��
* @param	self   �Լ�
* @param	sender ����target
* @param	args   ����
* @return	int
*/
int RideModule::OnCommandPlayerFinishSkill(IKernel * pKernel,
	const PERSISTID & self,
	const PERSISTID & sender,
	const IVarList & args)
{
	// �ж�
	IGameObj* pSender = pKernel->GetGameObj(sender);
	if (NULL == pSender)
	{
		return 0;
	}
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return 0;
	}

	// ��ȡ����
	PERSISTID skill = args.ObjectVal(1);
	if (!pKernel->Exists(skill))
	{
		return 0;
	}

	// �ظ�HP���ܲ��ܴ������
	const char* strSkill = pKernel->GetConfig(skill);
	if (StringUtil::CharIsNull(strSkill) || strcmp(strSkill, RESTORE_HP_SKILL_ID) == 0)
	{
		return 0;
	}

	// �жϹ�����NPC����
	const char* npc_script = pSender->GetScript();

	int iType = 0;
	if (strcmp(npc_script, "player") != 0)
	{
		iType = pSender->QueryInt("NpcType");
	}

	if (strcmp(npc_script, "PetNpc") == 0 || strcmp(npc_script, "player") == 0 || iType == 3)
	{
		RideModule::m_pRideModule->DownRide(pKernel, self);
	}

	return 0;
}

/*!
* @brief	������ұ����﹥����������Ҫ�����߼�
* @param	pKernel ����ָ��
* @param	self   ��������
* @param	sender ��������
* @param	args
* @return	int
*/
int RideModule::OnCommandBeKill(IKernel* pKernel,
	const PERSISTID& self,
	const PERSISTID& sender,
	const IVarList& args)
{
	IGameObj* playerObj = pKernel->GetGameObj(self);
	if (NULL == playerObj)
	{
		return 0;
	}

	// ��������������
	RideModule::m_pRideModule->DownRide(pKernel, self);
	return 0;
}

/*!
* @brief	�ܵ��˺���ɻص�
* @param	pKernel ����ָ��
* @param	self   �ܵ��˺������
* @param	sender �ͷ���target
* @param	args   ����
* @return	int
*/
int RideModule::OnCommandBeDamage(IKernel* pKernel, const PERSISTID& self,
	const PERSISTID& sender, const IVarList& args)
{
	IGameObj* playerObj = pKernel->GetGameObj(self);
	if (NULL == playerObj)
	{
		return 0;
	}

	IGameObj* pSender = pKernel->GetGameObj(sender);
	if (NULL == pSender)
	{
		return 0;
	}

	// �жϹ�����NPC����
	const char* npc_script = pSender->GetScript();
	int iType = 0;
	if (strcmp(npc_script, "player") != 0)
	{
		iType = pSender->QueryInt("NpcType");
	}
	if (strcmp(npc_script, "PetNpc") == 0 || strcmp(npc_script, "player") == 0 || iType == 3)
	{
		RideModule::m_pRideModule->DownRide(pKernel, self);
	}

	return 0;
}

// �����buffʱ����
int RideModule::OnCommandAddBuffer(IKernel * pKernel, const PERSISTID & self,
	const PERSISTID & sender, const IVarList & args)
{
	if (!pKernel->Exists(self) || !pKernel->Exists(sender) || pKernel->Type(self) != TYPE_PLAYER)
	{
		return 0;
	}

	// ��ȡbuff����������
	const char* szFromID = pKernel->GetConfig(sender);
	const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(szFromID);
	if (pBuffBaseData == NULL)
	{
		return false;
	}
	// �����ӵ�buff��������buff  ������
	if (pBuffBaseData->GetAffectType() != BUFFAFFECT_BENEFICIAL)
	{
		RideModule::m_pRideModule->DownRide(pKernel, self);
	}

	return 0;
}

/*!
* @brief	����ͻ�����Ϣ
* @param	pKernel
* @param	self
* @param	sender
* @param	args
* @return	int
*/
int RideModule::OnCustomMessage(IKernel* pKernel,
	const PERSISTID& self,
	const PERSISTID& sender,
	const IVarList& args)
{
	// �����ж�
	if (!pKernel->Exists(self) || m_pRideModule == NULL)
	{
		return 0;
	}

	// ������ϢID
	int sub_msg = args.IntVal(1);
	switch (sub_msg)
	{
	case CS_RIDE_SKIN_MSG_REQ:
		{
			m_pRideModule->PushRideSkinMsg(pKernel, self, args.IntVal(2));
		}
		break;
	case CS_RIDE_CHANGE_SKIN:
		{
			m_pRideModule->ChangeRideSkin(pKernel, self, args.IntVal(2), args.IntVal(3));
		}
		break;
	case CS_RIDE_UP_DOWN:
		{
			m_pRideModule->RideUpDownOperate(pKernel, self, args.IntVal(2));
		}
		break;
	case CS_RIDE_CLEAR_SKIN_FLAG:
		{
			m_pRideModule->ClearSkinFlag(pKernel, self, args.IntVal(2));
		}
		break;
	case CS_RIDE_SKIN_UNLOCK:
		{
			m_pRideModule->UnlockSkin(pKernel, self, args.IntVal(2));
		}
		break;
	case CS_RIDE_UPGRADE_LEVEL:
		m_pRideModule->UpgradeLevel(pKernel, self);
		break;
	case CS_RIDE_UPGRADE_STEP:
		m_pRideModule->UpgradeStep(pKernel, self);
		break;
	default:
		break;
	}

	return 0;
}