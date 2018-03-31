//------------------------------------------------------------------------------
// �ļ���:      skill.cpp
// ��  ��:      ����ģ�鹫���ӿ�ʵ��
// ˵  ��:
// ��������:    2013��2��22��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------
#include "FsGame/Interface/FightInterface.h"
#include "FsGame/SkillModule/SkillModule.h"
#include "FsGame/SkillModule/SkillToSprite.h"
#include "FsGame/SystemFunctionModule/CoolDownModule.h"
#include "FsGame/SkillModule/Impl/SkillFlow.h"
#include "FsGame/SkillModule/Impl/SkillAccess.h"
#include "FsGame/SkillModule/impl/SkillDataQueryModule.h"
#include "FsGame/SkillModule/impl/SkillConfigData.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
//#include "FsGame/Interface/aiinterface.h"
//#include "define/npcpropertydefine.h"
#include "FsGame/Define/FightPropertyDefine.h"
#include "../Define/staticdatadefine.h"
#include "../SkillModule/Impl/SkillDefineDataStruct.h"
//#include "../SkillModule/PassiveSkillSystem.h"
#include "utils/string_util.h"
//#include "FsGame/SkillModule/FlySkillModule.h"
#include "Define/CommandDefine.h"
//#include "Define/BattleAbilityDefine.h"
//#include "SkillModule/Impl/PVPRecAttacker.h"
#include "Define/Classes.h"
#include "Define/BattleAbilityDefine.h"
#include "SkillModule/PassiveSkillSystem.h"
#include "SkillModule/PVPRecAttacker.h"

const char* const NPC_PROPERTY_NORMAL_ATTACK_ID = "NormalAttackID";	//��ʱ����


bool FightInterface::Contains(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

    if (NULL == szSkillConfigID || StringUtil::CharIsNull(szSkillConfigID))
    {
        return false;
    }

    return SKillToSpriteSingleton::Instance()->Contains(pKernel, self, szSkillConfigID);
}

// ���ĳ������
PERSISTID FightInterface::FindSkill(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID)
{
    return SKillToSpriteSingleton::Instance()->FindSkill(pKernel, self, szSkillConfigID);
}

PERSISTID FightInterface::AddSkill(IKernel* pKernel,
	const PERSISTID& self,
	const char* szSkillConfigID)
{
	if (!pKernel->Exists(self))
	{
		return PERSISTID();
	}

	if (NULL == szSkillConfigID || StringUtil::CharIsNull(szSkillConfigID))
	{
		return PERSISTID();
	}

	if (!SKillToSpriteSingleton::Instance()->Contains(pKernel, self, szSkillConfigID))
	{
		if (!SKillToSpriteSingleton::Instance()->AddSkill(pKernel, self, szSkillConfigID))
		{
			return PERSISTID();
		}
	}

	return SKillToSpriteSingleton::Instance()->FindSkill(pKernel, self, szSkillConfigID);
}

int FightInterface::UseSkill(IKernel* pKernel,
                             const PERSISTID& self,
                             const char* szSkillConfigID,
                             const IVarList& args)
{
    IGameObj* pGameObj = pKernel->GetGameObj(self);
    if (NULL == pGameObj)
    {
        return USESKILL_RESULT_NO_OBJECT;
    }

	if (NULL == szSkillConfigID || StringUtil::CharIsNull(szSkillConfigID))
    {
        return USESKILL_RESULT_ERR_CONFIG_DATA;
    }

    PERSISTID skill = FindSkill(pKernel, self, szSkillConfigID);
    if (!pKernel->Exists(skill))
    {
        return USESKILL_RESULT_ERR_NOSKILL;
    }

    return SkillModule::m_pInstance->UseSkill(pKernel, self, skill, args);
}


int FightInterface::UseSkill(IKernel* pKernel,
                             const PERSISTID& self,
                             const PERSISTID& skill,
                             const IVarList& args)
{
    if (!pKernel->Exists(self))
    {
        return USESKILL_RESULT_ERR_OTHER;
    }

    return SkillModule::m_pInstance->UseSkill(pKernel, self, skill, args);
}

/**
*@brief  ʹ�ü���(��ʱֻ�ṩNPCʹ��)
*@param  [IN] pKernel �����ں�ָ��
*@param  [IN] self ����ʩ����
*@return bool Ѱ���Ƿ�ɹ�
*/
bool FightInterface::ReadyUseRandomSkill(IKernel* pKernel, const PERSISTID& self, float& fSkillMinDis, float& fSkillMaxDis, const char* strSkillId)
{
	fSkillMinDis = 0.0f;
	fSkillMaxDis = 0.0f;
	if (!pKernel->Exists(self))
	{
		return false;
	}

	return SkillModule::m_pInstance->ReadyUseRandomSkill(pKernel, self, fSkillMinDis, fSkillMaxDis, strSkillId);
}

// ��ϼ���
bool FightInterface::BreakSkill(IKernel* pKernel,
                                const PERSISTID& self,
                                const PERSISTID& breaker,
                                bool bForce,
                                int iBreakReason)
{
    if (!pKernel->Exists(self) || !pKernel->Exists(breaker))
    {
        return false;
    }

    extend_warning(LOG_TRACE,
        "[FightInterface::BreakSkill] self '%d-%d' breaker%d-%d",
        self.nIdent, self.nSerial, breaker.nIdent, breaker.nSerial);

    return SkillFlowSingleton::Instance()->BreakSkill(pKernel, self, breaker, bForce, iBreakReason);
}

//	ǿ��һ��������ȴʱ��Ŀ�ʼ
bool FightInterface::ForceBeginCoolDown(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj || StringUtil::CharIsNull(szSkillConfigID))
	{
		return false;
	}
    PERSISTID skill = SKillToSpriteSingleton::Instance()->FindSkill(pKernel, self, szSkillConfigID);
    IGameObj* pSkillObj = pKernel->GetGameObj(skill);
    if (NULL == pSkillObj)
    {
        return false; 
    }

    //��ʼ������ȴ����
    int iPubCoolTime = 0;
    int iCoolDownTime = 0;
    int iCoolCategory = 0;
    if (!SkillAccessSingleton::Instance()->GetCoolDownTime(pKernel, self, skill, iPubCoolTime, iCoolDownTime, iCoolCategory))
    {
        return false;
    }
    
	CoolDownModule::m_pInstance->BeginCoolDown(pKernel, self, iCoolCategory, iCoolDownTime);
	
	return true;
}

// �ж��Ƿ�������ʹ�ü���
int FightInterface::IsSkillUseBusy(IKernel* pKernel, const PERSISTID& self)
{
    if (!pKernel->Exists(self))
    {
        return USESKILL_RESULT_NO_OBJECT;
    }

    return SkillFlowSingleton::Instance()->IsSkillUseBusy(pKernel, self);
}

// ָ�������Ƿ�������ȴ
bool FightInterface::IsCoolDown(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill)
{
    if (!pKernel->Exists(self) || !pKernel->Exists(skill))
    {
        return false;
    }

    return SkillAccessSingleton::Instance()->IsCoolDown(pKernel, self, skill);
}

bool FightInterface::IsCoolDown(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID)
{
    PERSISTID skill = SKillToSpriteSingleton::Instance()->FindSkill(pKernel, self, szSkillConfigID);
    if (!pKernel->Exists(self) || !pKernel->Exists(skill))
    {
        return false;
    }

    return SkillAccessSingleton::Instance()->IsCoolDown(pKernel, self, skill);
}

/*!
* @brief	���㱾�μ��ܲ����ĳ��ֵ
* @param	[in] pKernel �����ں�ָ��
* @param	[in] skill ����
* @param	[in] nDamValue �˺�ֵ
* @return	int �������ӵĳ��ֵ
*/
int FightInterface::ComputeHateValue(IKernel* pKernel, const PERSISTID& skill, int nDamValue)
{
	if (!pKernel->Exists(skill))
	{
		return 0;
	}

	return SkillAccessSingleton::Instance()->ComputeHateValue(pKernel, skill, nDamValue);
}

/*!
 * @brief	��ȡ��ǰʹ�ü��ܵĲٿ����� 
 * @param	[in] pKernel �����ں�ָ��
 * @param	[in] self ʩ����
 * @return	int ����ֵ��USOperationState
 */
// int FightInterface::GetCurUSOperationState(IKernel* pKernel, const PERSISTID& self)
// {
// 	return SkillAccessSingleton::Instance()->GetCurUSOperationState(pKernel, self);
// }
/*!
 * @brief	���³��������ļ������ݼ�Ч��
 * @param	self �����(�����ǳ���Ҳ���������)
 * @param	strSkillId ����id
 * @param	nSkillLevel ���ܵȼ�
 * @param	nSkillSrc  ������Դ(���������õ�)
 * @return	bool
 */
bool FightInterface::UpdateSkillData(IKernel* pKernel, const PERSISTID &self, const char* strSkillId, int nSkillLevel, int nSkillSrc)
{
	if (!pKernel->Exists(self) || NULL == strSkillId || StringUtil::CharIsNull(strSkillId) || nSkillLevel <= 0)
	{
		return false;
	}

	int nSkillType = GetSkillTypeByConfig(pKernel, strSkillId);
	if (USE_SKILL_TYPE == nSkillType)
	{
		PERSISTID skill = FindSkill(pKernel, self, strSkillId);
		if (skill.IsNull())
		{
			skill = AddSkill(pKernel, self, strSkillId);
		}

		IGameObj* pSkillObj = pKernel->GetGameObj(skill);
		if (pSkillObj != NULL)
		{
			pSkillObj->SetInt("Level", nSkillLevel);
		}

		if (pKernel->Type(self) == TYPE_PLAYER)
		{
			// ս��������
			pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_SKILL_TYPE);
		}
	}
	else if(PASSIVE_SKILL_TYPE == nSkillType)
	{
		int nSkillId = StringUtil::StringAsInt(strSkillId);
		PassiveSkillSystemSingleton::Instance()->UpdatePassiveSkill(pKernel, self, nSkillId, nSkillLevel, nSkillSrc);

		if (pKernel->Type(self) == TYPE_PLAYER)
		{
			// ս��������
			pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PROPERTY_TYPE);
			pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PASSIVE_SKILL_TYPE);
		}
	}

	return true;
}

/*!
 * @brief	ͨ������id,�ж��Ƿ�Ϊ��ҵ���ͨ���� 
 * @param	[in] pKernel �����ں�ָ��
 * @param	[in] self ʩ����
 * @param	[in] strSkillId ����id
 * @return	int ����ֵ��USOperationState
 */
// bool FightInterface::IsPlayerNormalAttackSkill(IKernel* pKernel, const PERSISTID& self, const char* strSkillId)
// {
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (NULL == pSelfObj || NULL == strSkillId || StringUtil::CharIsNull(strSkillId))
// 	{
// 		return false;
// 	}
// 
// 	return SkillAccessSingleton::Instance()->IsPlayerNormalAttackSkill(pKernel, pSelfObj, strSkillId);
// }

/*!
 * @brief	�����м��ܵȼ�����Ϊ��ͼ� 
 * @param	[in] pKernel �����ں�ָ��
 * @param	[in] self ʩ����
 * @return	void 
 */
// void FightInterface::SetSkillLevelMinLevel(IKernel* pKernel, const PERSISTID& self)
// {
// 	if(!pKernel->Exists(self))
// 	{
// 		return;
// 	}
// 
// 	SKillToSpriteSingleton::Instance()->SetSkillLevelMinLevel(pKernel, self);
// }

/*!
 * @brief	�����м��ܵȼ�����Ϊ��ͼ� 
 * @param	[in] pKernel �����ں�ָ��
 * @param	[in] self ʩ����
 * @param	[in] strSkill ��������
 * @return	int ʣ���CD���� -1 ��ȡʧ�� 
 */
// int FightInterface::GetSkillLeftCDSeconds(IKernel* pKernel, const PERSISTID& self, const char* strSkill)
// {
// 	if(!pKernel->Exists(self))
// 	{
// 		return -1;
// 	}
// 
// 	return SkillAccessSingleton::Instance()->GetSkillLeftCD(pKernel, self, strSkill);
// }

/*!
 * @brief	��ȡ���ܵ���CD 
 * @param	[in] pKernel �����ں�ָ��
 * @param	[in] self ʩ����
 * @param	[in] strSkill ��������
 * @return	int ��CD���� -1 ��ȡʧ�� 
 */
int FightInterface::QuerySkillCD(IKernel* pKernel, const PERSISTID& self, const char* strSkill)
{
	if(!pKernel->Exists(self))
	{
		return -1;
	}

	// �ҵ����ܶ���
	PERSISTID skill = SKillToSpriteSingleton::Instance()->FindSkill(pKernel, self, strSkill);
	if (skill.IsNull())
	{
		return -1;
	}

	//��ʼ������ȴ����
	int iPubCoolTime = 0;
	int iCoolDownTime = 0;
	int iCoolCategory = 0;//������ȴ
	SkillAccessSingleton::Instance()->GetCoolDownTime(pKernel, self, skill, iPubCoolTime, iCoolDownTime, iCoolCategory);

	return iCoolDownTime;
}

#ifndef FSROOMLOGIC_EXPORTS
	/*!
	* @brief	�Ƿ����Ṧ��
	* @param	[in] pKernel �����ں�ָ��
	* @param	[in] self ʩ����
	* @return	bool
	*/
// 	bool FightInterface::IsInFlySkill(IKernel* pKernel, const PERSISTID& self)
// 	{
// 		return FlySkillModule::m_pInstance->IsInFlySkill(pKernel, self);
// 	}
#endif

/*!
 * @brief	�Ƴ����������ļ���
 * @param	self �����(�����ǳ���Ҳ���������)
 * @param	strSkillId ����id
 * @return	bool
 */
bool FightInterface::RemoveSkillData(IKernel* pKernel, const PERSISTID &self, const char* strSkillId)
{
	if (!pKernel->Exists(self) || NULL == strSkillId || StringUtil::CharIsNull(strSkillId))
	{
		return false;
	}
	bool bRet = false;
	int nSkillType = GetSkillTypeByConfig(pKernel, strSkillId);
	if (USE_SKILL_TYPE == nSkillType)
	{
		bRet = RemoveSkill(pKernel, self, strSkillId);

		if (pKernel->Type(self) == TYPE_PLAYER)
		{
			// ս��������
//			pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_SKILL_TYPE);
		}
	}
	else if(PASSIVE_SKILL_TYPE == nSkillType)
	{
		int nSkillId = StringUtil::StringAsInt(strSkillId);
// 		bRet = PassiveSkillSystemSingleton::Instance()->RemovePassiveSkill(pKernel, self, nSkillId);
// 
// 		if (pKernel->Type(self) == TYPE_PLAYER)
// 		{
// 			// ս��������
// 			pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PROPERTY_TYPE);
// 			pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PASSIVE_SKILL_TYPE);
// 		}
	}	

	return bRet;
}

/*!
 * @brief	ͨ���������ж���������
 * @param	
 * @param
 * @return	int �������Ͷ�ӦSkillType
 */
int FightInterface::GetSkillTypeByConfig(IKernel* pKernel, const char* strSkillId)
{
	int nSkillType = NONE_SKILL_TYPE;
	do 
	{
		if (NULL == strSkillId || StringUtil::CharIsNull(strSkillId))
		{
			break;
		}
		const SkillBaseData* pBaseData = SkillDataQueryModule::m_pInstance->GetSkillBase(strSkillId);
		if (NULL == pBaseData)
		{
			break;
		}
		const char* strScript = pBaseData->Getscript();
		// �������ܵĴ���
		if (strcmp(strScript, AMBER_SKILL_NORMAL) == 0)
		{
			nSkillType = USE_SKILL_TYPE;
		}
// 		else if (strcmp(strScript, CLASS_NAME_SKILL_INTERACT) == 0)
// 		{
// 			nSkillType = INTERACT_SKILL_TYPE;
// 		}
		else
		{
			nSkillType = PASSIVE_SKILL_TYPE;
		}
	} while (false);
	
	return nSkillType;
}

bool FightInterface::RemoveSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill)
{
	IGameObj* pSkillObj = pKernel->GetGameObj(skill);
	if (NULL == pSkillObj)
	{
		return false;
	}
	bool ret = SKillToSpriteSingleton::Instance()->RemoveSkill(pKernel, self, skill);

	return ret;
}


bool FightInterface::RemoveSkill(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	if (NULL == szSkillConfigID || StringUtil::CharIsNull(szSkillConfigID))
	{
		return false;
	}

	PERSISTID skill = SKillToSpriteSingleton::Instance()->FindSkill(pKernel, self, szSkillConfigID);
	if (skill.IsNull())
	{
		return false;
	}
	bool ret = SKillToSpriteSingleton::Instance()->RemoveSkill(pKernel, self, skill);

	return ret;
}

/*!
* @brief	��ȡ��ɱ��������б�
* @param	outAttackerList ��������б�
* @param	self ����ɱĿ��
* @param	killer ��ɱ��(Ϊ��ʱ,���ص��б��д��л�ɱ��)
* @return	bool �Ƿ��ѯ�ɹ�
*/
bool FightInterface::GetAttackerList(IKernel* pKernel, IVarList& outAttackerList, const PERSISTID& self, const PERSISTID& killer)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}
	return PVPRecAttackerSingleton::Instance()->GetAttackerList(pKernel, outAttackerList, self, killer);
}