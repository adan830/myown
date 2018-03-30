//--------------------------------------------------------------------
// �ļ���:      SkillEventFuncCommon.h
// ��  ��:      ����ͨ���¼��߼�
// ˵  ��:		
// ��������:    2014��11��25��
// ������:       
//    :       
//--------------------------------------------------------------------
#include "FsGame/SkillModule/inner/SkillEventManager.h"
#include "FsGame/SkillModule/inner/SkillEventFuncCommon.h"

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
#include "FsGame/SkillModule/Impl/SkillDataQueryModule.h"
#include "../../NpcBaseModule/AI/AIDefine.h"
#include "../Impl/SkillStage.h"
#include "../../Define/Classes.h"

//���BUFF
//EVENT_EXEC_ADDBUFFER = 1,

// ���м����¼�
// 1    :���BUFFER
// ����1: ��ӵ�BUFF��ConfigID
bool SkillEventAddBuffer::Execute(IKernel* pKernel, const IVarList& vEventParams, const PERSISTID& self,
	const PERSISTID& skill, const int64_t uuid, const int iEventType,
	const IVarList& params, IVarList& outMsgParams)
{
	IGameObj* pGameObj = pKernel->GetGameObj(skill);
	if (!pKernel->Exists(self) || NULL == pGameObj)
	{
		return false;
	}
	// ����������
	size_t count = vEventParams.GetCount();
	CHECK_PARAM_COUNT(count, 3);

	const char* strBufferID		= vEventParams.StringVal(1);
	const char* strTimeRatio	= vEventParams.StringVal(2);
	const char* strRate			= vEventParams.StringVal(3);

	// Ĭ�ϸ���Ϊ100%
	int nAddBuffRate = BASE_RATE_NUM;
	if (!StringUtil::CharIsNull(strRate))
	{
		nAddBuffRate = convert_int(strRate, 100);
	}

	if (StringUtil::CharIsNull(strBufferID))
	{
		return false;
	}

	PERSISTID target = self;
	
	if (params.GetCount() >= 1 && VTYPE_OBJECT == params.GetType(0))
	{
		target = params.ObjectVal(0);
	}
	IGameObj* pTargetObj = pKernel->GetGameObj(target);
	if (NULL == pTargetObj)
	{
		return false;
	}
#ifdef FSROOMLOGIC_EXPORTS
	// ���ս�������ᱻ���ܸ���buff
	if (pTargetObj->QueryInt("GVG_IsTower") > 0)
	{
		return false;
	}
#endif 
	// �̶��˺���npc���ܱ����ܼ�buff
	if (pTargetObj->QueryInt("FixBeDamaged") > 0)
	{
		return false;
	}
	// �����buff�ĸ���
	int nRandom = ::util_random_int(BASE_RATE_NUM);
	if (nRandom >= nAddBuffRate)
	{
		return false;
	}

	float fTimeRatio = GetBufferTimeRatio(pKernel, target, strTimeRatio);

	const char* strLifeTime = pKernel->GetConfigProperty(strBufferID, "LifeTime");
	int nLifeTime = StringUtil::StringAsInt(strLifeTime);

	float fBufferTime = (float)nLifeTime * fTimeRatio;

	// ʱ��Ϊ0,�Ͳ��ü���
	if (util_float_equal_zero(fBufferTime))
	{
		return false;
	}

	// �������bufferʱ��ı�Ľӿ�
	if (!FightInterfaceInstance->AddBuffer(pKernel, target, self, strBufferID, (int)fBufferTime))
	{
		return false;
	}

	return true;
}

// �ҵ���ǰbuff��ʱ�����
const float SkillEventAddBuffer::GetBufferTimeRatio(IKernel* pKernel, const PERSISTID& self, const char* strBuffTimeRatio)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 1.0f;
	}

	// û��buffʱ������
	if (StringUtil::CharIsNull(strBuffTimeRatio))
	{
		return 1.0f;
	}

	CVarList args;
	StringUtil::SplitString(args, strBuffTimeRatio, ",");
	int nCount = (int)args.GetCount();
	// �����ֵ���ҵ�ʱ��Ĭ��Ϊ1,����ͬ���ͻ�ȡ����
	float fRatio = 1.0f;
	if (MAX_BUFF_TYPE_NUM != nCount)
	{
		return fRatio; 
	}

	int nClassType = pSelfObj->GetClassType();
	if (TYPE_NPC == nClassType && pSelfObj->FindAttr("AITemplate"))
	{
		int nTemplate = pSelfObj->QueryInt("AITemplate");
		int nBuffIndex = BOSS_ADD_BUFF;
		switch (nTemplate)
		{
		case AI_TEMP_BOSS:
			nBuffIndex = BOSS_ADD_BUFF;
			break;
		case AI_TEMP_NORMAL_FIGHT:
			nBuffIndex = NORMAL_NPC_ADD_BUFF;
			break;
		}
		fRatio = args.FloatVal(nBuffIndex);
	}

	return fRatio;
}