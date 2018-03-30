//--------------------------------------------------------------------
// �ļ���:      SkillEventBase.h
// ��  ��:      �����¼�����
// ˵  ��:		���еļ����¼����̳д˻���
// ��������:    2014��11��24��
// ������:       
//    :       
//--------------------------------------------------------------------
#ifndef _Skill_Event_Base_H_
#define _Skill_Event_Base_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/SkillEventFuncIdDefine.h"

// ���м����¼�����
class SkillEventBase
{
public:
	SkillEventBase();
	virtual ~SkillEventBase();

	// ���������¼�
	static SkillEventBase* CreateSkillEvent(const EEventExecType func_id);

	// ��ʼ��
	virtual bool OnInit(IKernel* pKernel);

	// ���м����¼�
	virtual bool Execute(IKernel* pKernel, const IVarList& vEventParams, const PERSISTID& self,
		const PERSISTID& skill, const int64_t uuid, const int iEventType,
		const IVarList& params, IVarList& outMsgParams) = 0;
protected:

private:

};

#endif