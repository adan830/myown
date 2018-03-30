//--------------------------------------------------------------------
// �ļ���:      SkillEventFuncCommon.h
// ��  ��:      ����ͨ���¼��߼�
// ˵  ��:		
// ��������:    2014��11��25��
// ������:       
//    :       
//--------------------------------------------------------------------
#ifndef _Skill_Event_Func_Common_H_
#define _Skill_Event_Func_Common_H_


#include "SkillEventBase.h"

//////////////////////////////////////////////////////////////////////////
//���BUFF
//EVENT_EXEC_ADDBUFFER = 1,
class SkillEventAddBuffer : public SkillEventBase
{
public:
	// ���м����¼�
	// 1    :���BUFFER
	// ����1: ��ӵ�BUFF��ConfigID
	virtual bool Execute(IKernel* pKernel, const IVarList& vEventParams, const PERSISTID& self,
		const PERSISTID& skill, const int64_t uuid, const int iEventType,
		const IVarList& params, IVarList& outMsgParams);
private:
	// buff����
	enum BuffTime
	{
		BOSS_ADD_BUFF,			// BOSS�ӵ�buff
		NORMAL_NPC_ADD_BUFF,	// ��ͨС�ּӵ�buff

		MAX_BUFF_TYPE_NUM
	};

	enum
	{
		BASE_RATE_NUM = 100		// ���ʻ���
	};

	// �ҵ���ǰbuff��ʱ�����
	const float GetBufferTimeRatio(IKernel* pKernel, const PERSISTID& self, const char* strBuffTimeRatio);
};

#endif