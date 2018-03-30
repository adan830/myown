//--------------------------------------------------------------------
// �ļ���:      SkillEventFuncBuffer.h
// ��  ��:      ����Buff�¼��߼�
// ˵  ��:		
// ��������:    2014��11��25��
// ������:       
//    :       
//--------------------------------------------------------------------
#ifndef _Skill_Event_Func_Buffer_H_
#define _Skill_Event_Func_Buffer_H_


#include "SkillEventBase.h"

// Buffer�˺��¼� 
// EVENT_EXEC_BUFF_CHANGE_TARGET_HP = 600,
class SkillEventBuffChangeHP : public SkillEventBase
{
public:
	// ���м����¼�
	virtual bool Execute(IKernel* pKernel, const IVarList& vEventParams, const PERSISTID& self,
		const PERSISTID& buff, const int64_t uuid, const int iEventType,
		const IVarList& params, IVarList& outMsgParams);
};

#endif