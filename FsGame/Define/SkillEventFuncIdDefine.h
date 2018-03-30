//------------------------------------------------------------------------------
// �ļ���:      SkillEventFuncIdDefine.h
// ��  ��:      �����¼��ص�����ID����
// ˵  ��:
// ��������:    2014��11��24��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#ifndef __SKILLEVENTFUNCIDDEFINE_H__
#define __SKILLEVENTFUNCIDDEFINE_H__

enum EEventExecType
{
	// ͨ�ù���ִ��: 1~200
	// ���ܶ��й���ִ�� 300~598
	// BUffer���й���ִ�� 620~699
	EVENT_EXEC_INVALID = 0,

	// ͨ�ù���ִ��:1~200 ------------------------------------------------------
	//���BUFF
	EVENT_EXEC_ADDBUFFER = 1,

	EVENT_EXEC_COMMON_END,
	//////////////////////////////////////////////////////////////////////////
	// ���ܶ��й���ִ��300~500--------------------------------------------------
	EVENT_EXEC_SKILL_START = 300,
	// ��˸
	EVENT_EXEC_FLICKER_MOVE,

	// �ٻ�ͼ��(������Χ����npc)
	EVENT_EXEC_CALL_TOTEM,

	EVENT_EXEC_SKILL_END,
	//////////////////////////////////////////////////////////////////////////
	// BUffer���й���ִ��600~800 ----------------------------------------------
	EVENT_EXEC_BUFF_START = 599,

    // BufferHP�ı�(��Ѫ���˺�)
    EVENT_EXEC_BUFF_CHANGE_TARGET_HP = 600,

	EVENT_EXEC_BUFF_END,

    EVENT_EXEC_TEST_TEST = 699,
	// ��ǰ���ִ�б��
	MAX_EXECUTE
};

#endif // __SKILLEVENTFUNCIDDEFINE_H__
