//------------------------------------------------------------------------------
// �ļ���:      BufferModuleFuns.cpp
// ��  ��:      ���Ǳ���Ϣ����buffer�¼�����
// ˵  ��:
// ��������:    2014��11��4��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#include "BufferModule.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/FightPropertyDefine.h"
#include "FsGame/SkillModule/impl/TargetFilter.h"
#include "FsGame/SkillModule/impl/SkillDataQueryModule.h"
#include "FsGame/SkillModule/inner/SkillEventManager.h"
#include "FsGame/Interface/PropertyInterface.h"
#include "../SystemFunctionModule/StaticDataQueryModule.h"
#include "../Define/StaticDataDefine.h"
#include "utils/string_util.h"
//#include "BufferDecayLogic.h"
#include "../SystemFunctionModule/CoolDownModule.h"
#include "Define/Fields.h"

//���ĳ��BUFFER
bool BufferModule::InterAddBuffer(IKernel* pKernel, const PERSISTID& self,
								  const PERSISTID& sender, const PERSISTID& buffer)
{
	//У�鱣��
	if (!pKernel->Exists(sender))
	{
		return false;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	IGameObj* pBufferObj = pKernel->GetGameObj(buffer);
	if (NULL == pBufferObj)
	{
		return false;
	}

	//��ȡBUFFER����
	PERSISTID container = GetBufferContainer(pKernel, self);
	if (!pKernel->Exists(container))
	{
		::extend_warning(LOG_WARNING,
			"[BufferModule::InterAddBuffer] the role '%d-%d' BufferContainer is not exists.",
			self.nIdent, self.nSerial);
		return false;
	}

	const char* szBufferConfigID = pKernel->GetConfig(buffer);
	if (StringUtil::CharIsNull(szBufferConfigID))
	{
		return false;
	}

	const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(szBufferConfigID);
	if (NULL == pBuffBaseData)
	{
		return false;
	}

	bool bDeadClear = pBuffBaseData->GetDeadClear() > 0 ? true : false;
	//�������������Ҵ�BUFFER��������������ͣ��������
	int nDead = 0;
	FPropertyInstance->GetDead(pSelfObj,nDead);
	if (nDead > 0 && bDeadClear)
	{
		return false;
	}

	// ���߿�����buff����
	if(pSelfObj->QueryInt("ImmunoControlBuff") > 0 && BUFFAFFECT_CONTROL == pBuffBaseData->GetAffectType())
	{
		return false;
	}

	// �����ͷŵ�buff,�൱�������ͷŵ�
	PERSISTID attacker = get_pet_master(pKernel, sender);
	//===���²���˲ʱ��BUFFER���г���ʱ���
	// ��ʼ��buffer����
	if (!InitBufferData(pKernel, self, attacker, buffer))
	{
		return false;
	}

	// �����滻
	if (!DoBufferReplace(pKernel, self, container, buffer))
	{
		return false;
	}

	//��ŵ�BUFFER������
	if (!pKernel->Place(buffer, container))
	{
		::extend_warning(LOG_TRACE, 
			"[BufferModule::InterAddBuffer] BUFFER %s cant place to BufferContainer.", 
			pKernel->GetConfig(buffer));
		return false;
	}

	// ֻ�в�ͬ���֮���buff����Ҫ˥��
// 	if (TYPE_PLAYER == pSelfObj->GetClassType() && TYPE_PLAYER == pKernel->Type(attacker) && self != attacker)
// 	{
// 		// ����buffʱ��˥��
// 		int nRealTime = pBufferObj->QueryInt(FIELD_PROP_LIFE_TIME);
// 		if(BufferDecayLogicSingleton::Instance()->ComputeBuffRealTime(pKernel, self, pBufferObj, pBuffBaseData, nRealTime) && nRealTime > 0)
// 		{
// 			// ����Buffer������ʱ��
// 			BPropertyInstance->SetLifeTime(pBufferObj, nRealTime);
// 		}
// 
// 		// ����Buffer������ʱ��
// 		BPropertyInstance->SetLifeTime(pBufferObj, nRealTime);
// 	}
	
	// ���ö����buffЧ��
	ChangeBuffDisplay(pKernel, self, szBufferConfigID);

	//����
	BufferModule::ActiveBuffer(pKernel, self, buffer);

	return true;
}

// ��ʼ��buffer��������
bool BufferModule::InitBufferData(IKernel* pKernel, const PERSISTID& self, 
								  const PERSISTID& sender, const PERSISTID& buffer)
{
	IGameObj* pSenderObj = pKernel->GetGameObj(sender);
	if (NULL == pSenderObj)
	{
		return false;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	IGameObj* pBufferObj = pKernel->GetGameObj(buffer);
	if (NULL == pBufferObj)
	{
		return false;
	}

	if (pBufferObj->FindAttr("Skill"))
	{
		PERSISTID curSkill;
		FPropertyInstance->GetCurSkill(pSenderObj,curSkill);
		if (pKernel->Exists(curSkill))
		{
			BPropertyInstance->SetSkill(pBufferObj,curSkill);
		}
	}
	BPropertyInstance->SetSender(pBufferObj,sender);
	BPropertyInstance->SetCounter(pBufferObj,0);
	BPropertyInstance->SetTimer(pBufferObj,0);//��ʽ���buffer����
	BPropertyInstance->SetAddTime(pBufferObj,::util_get_time_64());//��ӵ�ʱ��

	return true;
}

//���ĳ��BUFFER
bool BufferModule::InterRemoveBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer, int iRemoveReason)
{
	IGameObj* pBufferObj = pKernel->GetGameObj(buffer);
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pBufferObj || NULL == pSelf)
	{
		return false;
	}

	//�ָ�����
	int nBufferActive=0;
	BPropertyInstance->GetActive(pBufferObj,nBufferActive);
	if (nBufferActive == 1)
	{
		BufferModule::DeactiveBuffer(pKernel, self, buffer);
	}
	// DeactiveBuffer()�ĵ��ÿ��ܴ����¼�����buffer�ᱻ��ǰɾ��
	if (pKernel->Exists(buffer))
	{
		pKernel->DestroySelf(buffer);
	}

	// ����buff��ʾЧ��
	ChangeBuffDisplay(pKernel, self, "");
	return true;
}

//˲��״̬��Ч��
int BufferModule::InstantActiveBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer)
{
	// ��������¼�
	m_pInstance->BufferEventEffect(pKernel, BUFFEVENT_ACTIVE, self, self, buffer);

	return 0;
}

//����״̬��Ч��
bool BufferModule::ActiveBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	IGameObj* pBufferObj = pKernel->GetGameObj(buffer);
	if (NULL == pBufferObj)
	{
		return false;
	}

	const char* szBufferConfigID = pKernel->GetConfig(buffer);
	if (StringUtil::CharIsNull(szBufferConfigID))
	{
		return false;
	}

	BPropertyInstance->SetActive(pBufferObj, 1);

	// �����buff�Ķ�����Ϣ
	//pKernel->Command(buffer, self, CVarList() << COMMAND_ADD_BUFFER_TO_OBJECT);

	// ��������¼�
	m_pInstance->BufferEventEffect(pKernel, BUFFEVENT_ACTIVE, self, self, buffer);

	// buffer���������ִ���¼���ɾ�����ڴ���Ҫ��֤buffer�����Ƿ�Ϸ�
	if (!pKernel->Exists(buffer))
	{
		return true;
	}

	//��Ӽ�ʱ��BUFFER
	if (!pKernel->FindHeartBeat(buffer, "BufferModule::H_BufferUpdate"))
	{
		pKernel->AddHeartBeat(buffer, "BufferModule::H_BufferUpdate", BUFFER_HEART_INTERVAL);
	}

	return true;
}

//�ر�״̬��Ч��
bool BufferModule::DeactiveBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer)
{
	if (!pKernel->Exists(self) || !pKernel->Exists(buffer))
	{
		return false;
	}

	const char* szBufferConfigID = pKernel->GetConfig(buffer);
	if (StringUtil::CharIsNull(szBufferConfigID))
	{
		return false;
	}

	//��Ӽ�ʱ��BUFFER
	if (pKernel->Exists(buffer))
	{
		if (pKernel->FindHeartBeat(buffer, "BufferModule::H_BufferUpdate"))
		{
			pKernel->RemoveHeartBeat(buffer, "BufferModule::H_BufferUpdate");
		}
	}

	IGameObj* pBufferObj = pKernel->GetGameObj(buffer);
	if (NULL == pBufferObj)
	{
		return false;
	}
	
	int nBufferActive = 0;
	BPropertyInstance->GetActive(pBufferObj,nBufferActive);
	if (nBufferActive == 1)
	{
		BPropertyInstance->SetActive(pBufferObj,0);
		m_pInstance->BufferEventEffect(pKernel, BUFFEVENT_DEACTIVE, self, self, buffer);
	}

	return true;
}

//��ʱ��ƴ�ʱ��Ч��
bool BufferModule::TimerBuffer(IKernel* pKernel, const PERSISTID& self, 
							   const PERSISTID& eventsender, const PERSISTID& buffer)
{
	if (!pKernel->Exists(self) || !pKernel->Exists(buffer))
	{
		return false;
	}

	// �����ʱ����
	m_pInstance->BufferEventEffect(pKernel, BUFFEVENT_TIMER, self, eventsender, buffer);
	//�˺���ɺ�BUFFER��������Ѿ���ɾ��
	if (!pKernel->Exists(buffer))
	{
		return false;
	}

	return true;
}

//�滻����߼�
bool BufferModule::DoBufferReplace(IKernel* pKernel, const PERSISTID& self, const PERSISTID& container, const PERSISTID& buffer)
{
	if (!pKernel->Exists(self) ||!pKernel->Exists(container) ||  !pKernel->Exists(buffer))
	{
		return false;
	}

	const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(pKernel->GetConfig(buffer));
	
	// ��ȡ�滻��Ϣ
	int iReplaceCategory = 0 ,iReplaceType = 0;
	if (NULL != pBuffBaseData)
	{
		iReplaceCategory = pBuffBaseData->GetReplaceCategory();
		iReplaceType = pBuffBaseData->GetReplaceType();
	}
	// ��������BUFFER
	unsigned int it;
	PERSISTID temp_buffer = pKernel->GetFirst(container, it);
	LoopBeginCheck(c)
	while (pKernel->Exists(temp_buffer))
	{
		LoopDoCheck(c)
		IGameObj* pTmpBufferObj = pKernel->GetGameObj(temp_buffer);
		// ��ȡ�Ѵ��ڵ�BUFF����
		int tempReplaceCategory = 0;
		const BuffBaseData* pBuffMainData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(pKernel->GetConfig(temp_buffer));
		if (NULL == pTmpBufferObj || NULL == pBuffMainData)
		{
			temp_buffer = pKernel->GetNext(container, it);
			// ֱ������ 
			continue;
		}

		tempReplaceCategory = pBuffMainData->GetReplaceCategory();
		// ֻ��Ҫ���������ͬ��buffer
		if (tempReplaceCategory == iReplaceCategory)
		{ 
			// ֱ���滻
			if (BUFFER_REPLACE_DIRECT == iReplaceType)
			{
				// ɾ�����е�ͬ��buff
				BufferModule::InterRemoveBuffer(pKernel, self, temp_buffer, BUFFER_REMOVE_REPLACE);
			}
			else if(BUFFER_REPLACE_REFRESH_TIME == iReplaceType) // ˢ��bufferʱ��
			{
				BPropertyInstance->SetCounter(pTmpBufferObj,0);
				BPropertyInstance->SetTimer(pTmpBufferObj,0);
				BPropertyInstance->SetAddTime(pTmpBufferObj,::util_get_time_64());
				return false;
			}
			else if (BUFFER_REPLACE_NOT_REPLACE == iReplaceType)
			{
				return false;
			}
		} 

		temp_buffer = pKernel->GetNext(container, it);

	} // end of while (...
	return true;
}

//���BUFFER��ʣ��ʱ�䣬��λΪms
int BufferModule::GetBufferLeaveTime(int timeType, int lifeTime, int exitTime, time_t addTime)
{
	int leave_time = 0;
	switch (timeType)
	{
	case BUFFER_TIMETYPE_ONLINE:
		{
			//ֻ��������ʱ��
			leave_time = lifeTime - exitTime;
			break;
		}
	case BUFFER_TIMETYPE_ALLTIME:
		{
			//����ȫ��ʱ��
			time_t now = ::util_get_time_64();
			leave_time = int(lifeTime - (now - addTime));
			break;
		}
	default:
		break;
	}
	return leave_time;
}

//�����¼�
void BufferModule::BufferEventEffect(IKernel* pKernel,
									 const EBufferEventType type,
									 const PERSISTID& self,
									 const PERSISTID& sender,
									 const PERSISTID& buffer,
									 const IVarList& params)
{
	if (!pKernel->Exists(self))
	{
		return ;
	}
	IGameObj* pBufferObj = pKernel->GetGameObj(buffer);
	if (NULL == pBufferObj)
	{
		return ;
	}

	const char* szBufferConfigID = pKernel->GetConfig(buffer);
	if (StringUtil::CharIsNull(szBufferConfigID))
	{
		return ;
	}

	const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(szBufferConfigID);
	if (NULL == pBuffBaseData)
	{
		return;
	}

	//Ч����
	const StringArray& effectid = pBuffBaseData->GetBuffEffect();
	EEventExecType func_id = EVENT_EXEC_INVALID;//�¼����(EEventExecType)
	int nEventType = 0;//�¼����ͣ�eBufferEventType��

	size_t nEffectCount = effectid.GetCount();
	LoopBeginCheck(r);
	for(size_t i = 0; i < nEffectCount; ++i)
	{
		LoopDoCheck(r);
		//�¼����ͣ�Ŀ�����ͣ��¼����
		const BuffEffectData* pBuffEffectData = SkillDataQueryModule::m_pInstance->GetBuffEffectProp(effectid.StringVal(i));
		if (NULL == pBuffEffectData)
		{
			continue;
		}
		nEventType = pBuffEffectData->GetEventType();
		if (nEventType != type)
		{
			continue;
		}
		func_id = (EEventExecType)pBuffEffectData->GetFuncNo();
		PERSISTID target = sender;
		CVarList msgParams;
		CVarList configparams;
		pBuffEffectData->GetParams().ConvertToVarList(configparams);
		SkillEventManager::m_pInstance->ExecuteOperate(pKernel,
			func_id,      // ���ܲ������
			configparams,      // ���ܲ���
			self, buffer, 0, type, CVarList() << target << params, msgParams);
	}

    return ;
}

// ������ʾbuff�Ŀ�������BuffDisplayID
bool BufferModule::ChangeBuffDisplay(IKernel* pKernel, const PERSISTID& self, const std::string& szBufferConfigID)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	//��ȡBUFFER����
	PERSISTID container = GetBufferContainer(pKernel, self);
	if (!pKernel->Exists(container))
	{
		::extend_warning(LOG_WARNING,
			"[BufferModule::InterAddBuffer] the role '%d-%d' BufferContainer is not exists.",
			self.nIdent, self.nSerial);
		return false;
	}

	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}
	// ɾ��buff��ʱ��
	if (szBufferConfigID.empty())
	{
		unsigned int it = 0;
		fast_string strDisplay = "";
		PERSISTID buffer = pKernel->GetFirst(container, it);
		LoopBeginCheck(d)
		while (pKernel->Exists(buffer))
		{
			LoopDoCheck(d)
			const char* szTmpConfig = pKernel->GetConfig(buffer);
			const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(szTmpConfig);
			if (NULL == pBuffBaseData)
			{
				return false;
			}
			// �ҵ���Ҫ��ʾbuff��Ч��
			const char* strBuffDisplay = pBuffBaseData->GetBuffDisplay();
			if (!StringUtil::CharIsNull(strBuffDisplay))
			{
				strDisplay = strBuffDisplay;
				break;
			}
			
			buffer = pKernel->GetNext(container, it);
		}
		// û�ҵ���ʾ��buffЧ��,strDisplayΪ""
		pSelf->SetString("BuffDisplayID", strDisplay.c_str());
	}
	else
	{
		// ���buff��ʱ��
		const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(szBufferConfigID.c_str());
		if (NULL == pBuffBaseData)
		{
			return false;
		}
		// �Ƿ����buffЧ��id
		const char* strBuffDisplay = pBuffBaseData->GetBuffDisplay();
		if (StringUtil::CharIsNull(strBuffDisplay))
		{
			return false;
		}
		pSelf->SetString("BuffDisplayID", strBuffDisplay);
	}
	
	return true;
}

// buff��ֱ���滻����
bool BufferModule::DoBuffDirectReplace(IKernel* pKernel, const PERSISTID& curbuff, const PERSISTID& oldbuff)
{
	return true;
}

// �����������buff
int BufferModule::RemoveBuffByClearType( IKernel* pKernel, const PERSISTID &self, int nClearType ) 
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}
	unsigned int uit;
	PERSISTID buffercontainer = m_pInstance->GetBufferContainer(pKernel, self);
	if (!pKernel->Exists(buffercontainer))
	{
		::extend_warning(LOG_TRACE,
			"[BufferModule::OnCommandBeKill] the role '%d-%d' not have BufferContainer.",
			self.nIdent, self.nSerial);
		return 0;
	}

	// �Ƴ�Ŀ��������������Ҫ�Ƴ���BUFFER
	PERSISTID buffer = pKernel->GetFirst(buffercontainer, uit);
	LoopBeginCheck(e)
	while (pKernel->Exists(buffer))
	{
		LoopDoCheck(e)
		const char* szBufferConfigID = pKernel->GetConfig(buffer);
		if (StringUtil::CharIsNull(szBufferConfigID))
		{
			buffer = pKernel->GetNext(buffercontainer, uit);
			continue;
		}

		int nReason = 0;
		if (IsNeedClearBuff(pKernel, szBufferConfigID, nClearType, nReason))
		{
			m_pInstance->InterRemoveBuffer(pKernel, self, buffer, nReason);
		}

		buffer = pKernel->GetNext(buffercontainer, uit);
	}

	return 1;
}

// �Ƿ���Ҫ���buff
bool BufferModule::IsNeedClearBuff(IKernel* pKernel, const char* strBufferConfigID, int nClearType, int& outReason)
{
	const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(strBufferConfigID);
	if (NULL == pBuffBaseData)
	{
		return false;
	}
	bool bClear = false;

	switch (nClearType)
	{
	case BUFFER_SWITCH_CLEAR:
		bClear = pBuffBaseData->GetChangeSceneClear() > 0 ? true : false;
		outReason = BUFFER_REMOVE_CLEAR;
		break;
	case BUFFER_OFFLINE_CLEAR:
		bClear = pBuffBaseData->GetOffLineClear() > 0 ? true : false;
		outReason = BUFFER_REMOVE_OFFLINE;
		break;
	case BUFFER_DEAD_CLEAR:
		bClear = pBuffBaseData->GetDeadClear() > 0 ? true : false;
		outReason = BUFFER_REMOVE_DEAD;
		break;
	}
	
	return bClear;
}