//--------------------------------------------------------------------
// �ļ���:      SkillEventFuncSkill.h
// ��  ��:      �����¼��߼�
// ˵  ��:		
// ��������:    2014��11��24��
// ������:       
//    :       
//--------------------------------------------------------------------
#ifndef _Skill_Event_Func_Skill_H_
#define _Skill_Event_Func_Skill_H_

#include "SkillEventBase.h"
#include <vector>
#include <map>

// 	// ��˸
// EVENT_EXEC_FLICKER_MOVE = 301,
class SkillEventFlickerMove : public SkillEventBase
{
public:
	// ��ʼ��
	virtual bool OnInit(IKernel* pKernel);

	// ���м����¼�
	// ����1:�˺�ϵ��
	virtual bool Execute(IKernel* pKernel, const IVarList& vEventParams, const PERSISTID& self,
		const PERSISTID& skill, const int64_t uuid, const int iEventType,
		const IVarList& params, IVarList& outMsgParams);

};

// �ٻ�ͼ��(������Χ����npc)
// EVENT_EXEC_CALL_TOTEM = 302,
class SkillEventCallTotem : public SkillEventBase
{
public:
	// ��ʼ��
	virtual bool OnInit(IKernel* pKernel);

	// ���м����¼�
	// ����1:�˺�ϵ��
	virtual bool Execute(IKernel* pKernel, const IVarList& vEventParams, const PERSISTID& self,
		const PERSISTID& skill, const int64_t uuid, const int iEventType,
		const IVarList& params, IVarList& outMsgParams);
private:
	
	// ��������
	static int H_TotemAttack(IKernel* pKernel, const PERSISTID& self, int slice);
};
#endif