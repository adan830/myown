//--------------------------------------------------------------------
// �ļ���:      SkillEventFuncBuffer.h
// ��  ��:      ����Buff�¼��߼�
// ˵  ��:		
// ��������:    2014��11��25��
// ������:       
//    :       
//--------------------------------------------------------------------
#include "FsGame/SkillModule/inner/SkillEventManager.h"
#include "FsGame/SkillModule/inner/SkillEventFuncBuffer.h"

#include "FsGame/Define/CommandDefine.h"

#include "FsGame/Interface/FightInterface.h"
#include "FsGame/SkillModule/FightModule.h"
#include "FsGame/SkillModule/impl/TargetFilter.h"
#include "FsGame/SkillModule/impl/SkillConfigData.h"
#include "FsGame/SkillModule/inner/SkillEventExecuteUtil.h"
#include "FsGame/SkillModule/impl/FightDataAccess.h"
#include "FsGame/SkillModule/impl/SkillFlow.h"
#include "FsGame/Interface/PropertyInterface.h"
#include "FsGame/Define/BufferDefine.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"
#include "utils/exptree.h"
#include "../Impl/SkillDataQueryModule.h"
#include "../../SystemFunctionModule/StaticDataQueryModule.h"
#include "../../Define/GameDefine.h"
#include "../../Define/ModifyPackDefine.h"
#include "../BufferModule.h"
#include "CommonModule/LevelModule.h"

using namespace SkillEventExecuteUtil;
// Buffer�˺��¼�
// EVENT_EXEC_BUFF_CHANGE_TARGET_HP = 620,
// ���м����¼�
// 620: Buffer�˺�Ŀ��
bool SkillEventBuffChangeHP::Execute(IKernel* pKernel, const IVarList& vEventParams, const PERSISTID& self,
	const PERSISTID& buff, const int64_t uuid, const int iEventType,
	const IVarList& params, IVarList& outMsgParams)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	IGameObj* pObject = pKernel->GetGameObj(buff);
	if (NULL == pSelfObj || NULL == pObject)
	{
		return false;
	}

	const char* strHP = vEventParams.StringVal(1);		// �ı��HP
	int		nCalType  = vEventParams.IntVal(2);			// ��������

	PERSISTID skill = pObject->QueryObject("Skill");
	PERSISTID sender = pObject->QueryObject("Sender");

	// ���㼼���˺�ֵ
	ExpTree exp;
	float fVal = exp.CalculateEvent(pKernel, sender, skill, PERSISTID(), PERSISTID(), strHP);

	// fVal����0Ϊ��Ѫ,С��0Ϊ�˺�
	if (fVal > 0)
	{
		float fHandleVal = 0.0f;
		EventCureCal nAddType = EModify_ADD_VALUE == nCalType ? EVENT_VALUE_RESTORE : EVENT_PERCENT_RESTORE;
		fHandleVal = nAddType == EVENT_VALUE_RESTORE ? fVal : fVal / 100.0f;
	
		// ����˿�ʼ��Ѫ
		int nRealAdd = SkillEventExecuteUtil::RestoreHPForObject(pKernel, pSelfObj, fHandleVal, nAddType);
		if (nRealAdd > 0)
		{
			// ֪ͨ�ͻ��˼�Ѫ
			::CustomDisplayStage(pKernel, self, self, CUSTOM_ADD_HP, CVarList() << nRealAdd, false);
		}
	}
	else
	{
		int nRealVal = 0;
		if (EModify_ADD_RATE == nCalType)
		{
			float fRate = (float)fVal / 100.0f;
			double fMaxHP = (double)pSelfObj->QueryInt64("MaxHP");
			nRealVal = (int)(fMaxHP * fRate);
		}
		else
		{
			nRealVal = (int)fVal;
		}
		// ת��Ϊ��Ѫֵ,��������ĸ�ֵ
		nRealVal *= -1;
		// buff�ͷ��߲�����,��ɾ����buff
		if (!pKernel->Exists(sender))
		{
			const char* strBuff = pObject->GetConfig();
			FightInterfaceInstance->RemoveBuffer(pKernel, self, strBuff);
		}
		else
		{
			FightModule::m_pInstance->EventDamageTarget(pKernel, sender, self, buff, uuid, true, nRealVal);
		}
	}
	    
	return true;
}