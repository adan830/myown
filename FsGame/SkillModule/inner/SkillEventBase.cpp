//--------------------------------------------------------------------
// �ļ���:      SkillEventBase.h
// ��  ��:      �����¼�����
// ˵  ��:		���еļ����¼����̳д˻���
// ��������:    2014��11��24��
// ������:       
//    :       
//--------------------------------------------------------------------
#include "FsGame/SkillModule/inner/SkillEventBase.h"
#include "SkillEventFuncSkill.h"
#include "SkillEventFuncCommon.h"
#include "SkillEventFuncBuffer.h"


SkillEventBase::SkillEventBase()
{

}

SkillEventBase::~SkillEventBase()
{

}

// ���������¼�
SkillEventBase* SkillEventBase::CreateSkillEvent(const EEventExecType func_id)
{
	SkillEventBase* pEventBase = NULL;

	switch(func_id)
	{
	// ͨ��
	case EVENT_EXEC_ADDBUFFER:
		pEventBase = new SkillEventAddBuffer;
		break;
	// ����
	case EVENT_EXEC_FLICKER_MOVE:
		pEventBase = new SkillEventFlickerMove;
		break;
	case EVENT_EXEC_CALL_TOTEM:
		pEventBase = new SkillEventCallTotem;
		break;
	// Buffer
	case EVENT_EXEC_BUFF_CHANGE_TARGET_HP:
		pEventBase = new SkillEventBuffChangeHP;
		break;
	}

	return pEventBase;
}

// ��ʼ��
bool SkillEventBase::OnInit(IKernel* pKernel)
{
	return true;
}
