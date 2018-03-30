//------------------------------------------------------------------------------
// �ļ���:      SkillStage.cpp
// ��  ��:      ���ܸ��׶���Ϣ����
// ˵  ��:      ����ֻ�ڼ���ϵͳ�ڲ�ʹ�ã��Ҳ�Ҫֱ�Ӷ���SkillStage��ʵ������ʹ��
//              �䵥��ģʽ�ӿ�SkillStageSingleton::Instance()
// ��������:    2014��10��23��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------


#include "FsGame/skillmodule/impl/SkillStage.h"

#include <cmath>
#include <cassert>
#include <string>
#include "FsGame/define/ServerCustomDefine.h"
#include "FsGame/Define/SkillDefine.h"
#include "utils/extend_func.h"
#include "utils/custom_func.h"
#include "utils/util_macros.h"
#include "FsGame/SkillModule/impl/SkillDataQueryModule.h"
#include "FsGame/Define/FightPropertyDefine.h"
#include "../../Define/CommandDefine.h"
#include "SkillFlow.h"
#include "Skillaccess.h"
#include "../../Interface/PropertyInterface.h"
#include "../../NpcBaseModule/AI/AIDefine.h"
#include "utils/string_util.h"

//////////////////////////////////////////////////
// ��ʼ
bool SkillStage::CustomSkillBeginStage(IKernel* pKernel,
                                       const PERSISTID& self, 
                                       const char* szSkillConfigID,
                                       const int64_t uuid,
                                       const ESkillStageTargetType eSkillStageTargetType,
                                       const IVarList& params)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return false;
    }

    CVarList msg;
    msg << SERVER_CUSTOMMSG_SKILL << SKILL_STAGE_TYPE_BEGIN << eSkillStageTargetType << uuid << szSkillConfigID << self;

	int count = (int)params.GetCount();

	switch (eSkillStageTargetType)
	{
	case SKILL_STAGE_TARGET_TYPE_TARGET:          //Ŀ�꣺PERSISTID
		{
			if (count < 1)
			{
				return false;
			}
			msg << params.ObjectVal(0);
		}
		break;

	case SKILL_STAGE_TARGET_TYPE_POSITION:        //λ�ã�x,y,z
		{
			if (count < 2)
			{
				return false;
			}

			//ѹ����������10�����ͻ������ֵ0.1
			msg << util_compress_int2((int)(params.FloatVal(0)*10), (int)(params.FloatVal(1)*10));
		}
		break;

	default:
		::extend_warning(LOG_WARNING,
			"[SkillStage::CustomSkillResetDest] unknown SKILL_STAGE_TARGET_TYPE: %d !", eSkillStageTargetType);
		break;
	}

	SafeCustomByKen(pKernel, self, msg);
	return true;
}

// ����
bool SkillStage::CustomSkillPrepareStage(IKernel* pKernel,
                                         const PERSISTID& self,
                                         const int64_t uuid,
                                         const int iPrepareTime)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

    CVarList msg;
    msg << SERVER_CUSTOMMSG_SKILL << SKILL_STAGE_TYPE_PREPARE << uuid << iPrepareTime;

	SafeCustomByKen(pKernel, self, msg);
    return true;
}

// ����
bool SkillStage::CustomSkillLeadStage(IKernel* pKernel,
                                      const PERSISTID& self,
                                      const int64_t uuid, 
                                      const int iLeadTime)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

    CVarList msg;
    msg << SERVER_CUSTOMMSG_SKILL << SKILL_STAGE_TYPE_LEAD << uuid << iLeadTime;

    SafeCustomByKen(pKernel, self, msg);
    return true;
}

// ��������Ч�������ϲ���Ϣ����
bool SkillStage::CustomSkillEffectStageRealTime(IKernel* pKernel,
                                                const PERSISTID& self,                      // ������
                                                const int64_t uuid,                         // ����uuid
                                                const ESkillEventType eEventType,           // �����¼�����
                                                const PERSISTID& target,                    // Ч�����ö��� 
                                                const ESkillStageEffectType eEffectType,    // Ч��Ч������ID
                                                const IVarList& params,                     // Ч�����跢�͵�����
                                                const EMessageSendType eMsgSendType)        // ��Ϣ���͸���Щ����
{
    CVarList outMsgParams;
	outMsgParams << target << params;

	PERSISTID selfMaster = get_pet_master(pKernel, self);
    switch (eMsgSendType)
    {
    case EMSG_SEND_TYPE_ALL:
        return CustomSkillEffectStage(pKernel, selfMaster, uuid, util_compress_int4(eEventType, eEffectType), outMsgParams);
    case EMSG_SEND_TYPE_SELF:
        {
            return CustomSkillEffectStageBySelf(pKernel, selfMaster, uuid, util_compress_int4(eEventType, eEffectType), outMsgParams);
        }
    case EMSG_SEND_TYPE_SELF_TARGET:
        {
            
            PERSISTID targetMaster = get_pet_master(pKernel, target);
            CustomSkillEffectStageBySelf(pKernel, selfMaster, uuid, util_compress_int4(eEventType, eEffectType), outMsgParams);
			if (selfMaster != targetMaster)
			{
				CustomSkillEffectStageBySelf(pKernel, targetMaster, uuid, util_compress_int4(eEventType, eEffectType), outMsgParams);
			}
            return true;
        }
    default:
        break;
    }

    return false;
}

// ��Ϣֻ�����Լ�
bool SkillStage::CustomSkillEffectStageBySelf(IKernel* pKernel,
                                              const PERSISTID& self,              // ������
                                              const int64_t uuid,                 // ����uuid
													const int iCompressValue,			 // �����¼�����Ч���ͺϲ�ֵ
                                              const IVarList& params)             // eEventType���͵�����Ч���Ĳ�����ÿ��Ч���Ĳ�����Ҫ���������ĸ�ʽ�����Ϣ
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return false;
    }

    if (pSelfObj->GetClassType() != TYPE_PLAYER)
    {
        return false;
    }

    CVarList msg;
    msg << SERVER_CUSTOMMSG_SKILL << SKILL_STAGE_TYPE_EFFECT << iCompressValue << uuid << params;

	SafeCustom(pKernel, self, msg);
    return true;
}

bool SkillStage::CustomSkillEffectStage(IKernel* pKernel,
                                        const PERSISTID& self,              // ������
                                        const int64_t uuid,                 // ����uuid
                                        const int iCompressValue,           // �����¼�����Ч���ͺϲ�ֵ
                                        const IVarList& params)             // eEventType���͵�����Ч���Ĳ�����ÿ��Ч���Ĳ�����Ҫ���������ĸ�ʽ�����Ϣ
{
    if (!pKernel->Exists(self))
    {
        return false;
    }
    CVarList msg;
    msg << SERVER_CUSTOMMSG_SKILL << SKILL_STAGE_TYPE_EFFECT << iCompressValue << uuid << params;

	SafeCustomByKen(pKernel, self, msg);
    return true;
}

// ���
bool SkillStage::CustomSkillBreakStage(IKernel* pKernel,
                                       const PERSISTID& self,
                                       const int64_t uuid,
                                       const int reason)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

    CVarList msg;
    msg << SERVER_CUSTOMMSG_SKILL << SKILL_STAGE_TYPE_BREAK << uuid;

    SafeCustomByKen(pKernel, self, msg);
    return true;
}

// ����
bool SkillStage::CustomSkillFinish(IKernel* pKernel,
                                   const PERSISTID& self,
                                   const int64_t uuid,
                                   const int reason)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

    CVarList msg;
    msg << SERVER_CUSTOMMSG_SKILL << SKILL_STAGE_TYPE_FINISH << uuid;

    SafeCustomByKen(pKernel, self, msg);
    return true;
}
 
// �����ܿշ� ���������ͻ��˲�ͬ�� �ÿͻ��˵ĳ���
int SkillStage::CustomSkillWithoutTarget(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float fOrient)
{
	// ����ͷſ��Դ���������̵���ʹ�õļ���
	int result = SkillFlowSingleton::Instance()->CheckForceUseSkill(pKernel, self, skill);

	// �жϷ���ֵ 
	if (USESKILL_RESULT_SUCCEED != result)
	{
		return result; 
	}

	IGameObj* pSkillObj = pKernel->GetGameObj(skill);
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSkillObj || NULL == pSelfObj)
	{
		return 0;
	}

	const char* szSkillConfigID = pSkillObj->QueryString(SKILL_CONFIGID);
	int iPubCoolTime = 0;
	int iCoolDownTime = 0;
	int iCoolCategory = 0;//������ȴ
	SkillAccessSingleton::Instance()->GetCoolDownTime(pKernel, self, skill, iPubCoolTime, iCoolDownTime, iCoolCategory);

	CVarList msg;
	msg << SERVER_CUSTOMMSG_SKILL << SKILL_STAGE_TYPE_BEGIN << util_compress_int4(SKILL_STAGE_TARGET_TYPE_NONE) << 0 << szSkillConfigID << self << fOrient;

	pKernel->Command(self, self, CVarList() << COMMAND_SKILL_BEGIN << skill);
	SafeCustomByKen(pKernel, self, msg);
	return 0;
}

// ���Ͳ��Ż���Ч��
// int SkillStage::CustomSkillHitFly(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target,const PERSISTID& skill)
// {
// 	IGameObj* pSkillObj = pKernel->GetGameObj(skill);
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	IGameObj* pTargetObj = pKernel->GetGameObj(target);
// 	if (NULL == pSkillObj || NULL == pSelfObj || NULL == pTargetObj)
// 	{
// 		return 0;
// 	}
// 
// 	const SkillMainData* pMainData = SkillDataQueryModule::m_pInstance->GetSkillMain(pSkillObj);
// 	if (NULL == pMainData)
// 	{
// 		return 0;
// 	}
// 	// �����Ƿ���л���Ч��
// 	if (!pMainData->GetIsCanHitFly())
// 	{
// 		return 0;
// 	}
// 	
// 	// ֻ�����ɱ��npc�Ų��Ż���Ч��
// 	if (pSelfObj->GetClassType() != TYPE_PLAYER || pTargetObj->GetClassType() != TYPE_NPC)
// 	{
// 		return 0;
// 	}
// 
// 	// ֻ����ͨ�ֲ��ܱ�����
// 	int nNpcType = pTargetObj->QueryInt("NpcType");
// 	if (nNpcType != NPC_TYPE_ORDINARY)
// 	{
// 		return 0;
// 	}
// 
// 	// NPC�Ƿ�����
// 	int nDead = 0;
// 	if (FPropertyInstance->GetDead(pTargetObj, nDead) && 1 == nDead)
// 	{
// 		// ���ͻ�����Ϣ
// 		CVarList msg;
// 		msg << SERVER_CUSTOMMSG_SKILL_DEAD_EFFECT << target;
// 		SafeCustom(pKernel, self, msg);
// 	}
// 	return 0;
// }

// �ͻ�������ʧ�ܷ��� ��������
/*!
	 * @brief	�ͻ�������ʧ����Ϣ,��Ϊ����ʹ��
	 * @param	wstring ʩ��������
	 * @param	string ����id
	 * @param	int ʧ��ԭ��(0 ʱ����֤ʧ�� 1����������֤ʧ�� 2������֤ʧ��)
	 * @param	int ����Ŀ�����n
	 ����ѭ��n��
	 *  @param	PERSISTID Ŀ��
	 */
int SkillStage::CustomSkillClientHitFailed(IKernel* pKernel, const PERSISTID& self, const char* strSkillId, int nFailedType, const IVarList& targetList)
{
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf || StringUtil::CharIsNull(strSkillId))
	{
		return 0;
	}
	CVarList msgSend;
	const wchar_t* wsPlayerName = pSelf->QueryWideStr("Name");
	float x = pSelf->GetPosiX();
	float z = pSelf->GetPosiZ();
	msgSend << SERVER_CUSTOMMSG_SKILL_HIT_FAILED << wsPlayerName << strSkillId << nFailedType << x << z;
	int nCount = (int)targetList.GetCount();
	msgSend << nCount;
	LoopBeginCheck(c)
	for (int i = 0;i < nCount;++i)
	{
		LoopDoCheck(c)
		PERSISTID target = targetList.ObjectVal(i);
		IGameObj* pTargetObj = pKernel->GetGameObj(target);
		if (pTargetObj == NULL)
		{
			continue;
		}
		int nType = pTargetObj->QueryInt("NpcType");
		x = pTargetObj->GetPosiX();
		z = pTargetObj->GetPosiZ();
		msgSend << targetList.ObjectVal(i) << nType << x << z;
	}

	SafeCustom(pKernel, self, msgSend);
	return 0;
}