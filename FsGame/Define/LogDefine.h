//--------------------------------------------------------------------
// �ļ���:		LogDefine.h
// ��  ��:		Log���Ͷ���
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:        ( )
//    :	    
//--------------------------------------------------------------------

#ifndef __LogDefine_H__
#define __LogDefine_H__
#include "FunctionEventDefine.h"

// Log�ȼ�
enum LogLevelEnum
{
	LOG_TRACE_PERFORMANCE,
	LOG_TRACE = 1,			/**< ������Լ�����־ */
	LOG_INFO,				/**< ϵͳ������־ */
	LOG_WARNING,			/**< ������־ */
	LOG_ERROR,				/**< ������־ */
	LOG_CRITICAL			/**< ���ش���(������������) */
};

// $@ 17 = ���������־���� 17000-17999
enum LOG_ROLE_UPGRADE_TYPE
{
	LOG_ROLE_UPGRADE_MIN = 17000,		// $ 17000 = ���������־������Сֵ
	LOG_ROLE_UPGRADE_PLAYER,			// $ 17001 = �������
	LOG_ROLE_UPGRADE_SKILL,				// $ 17002 = ��������
	LOG_ROLE_UPGRADE_PASSIVE_SKILL,		// $ 17003 = ������������
	LOG_ROLE_UPGRADE_WING_LEVEL,		// $ 17004 = ��ҳ��ȼ�����
	LOG_ROLE_UPGRADE_WING_STEP,			// $ 17005 = ��ҳ��׼�����

	LOG_ROLE_UPGRADE_MAX = 17999,		// $ 17999 = ���������־�������ֵ
};

// $@ 20 = ������־��¼ 20000-20999
enum LOG_FRIEND_TYPE
{
	LOG_FRIEND_MIN = 20000,			// $ 20000 = ���������־��ʼ
	LOG_FRIEND_ADD,					// $ 20001 = ��Ӻ���
	LOG_FRIEND_DELETE,				// $ 20002 = ɾ������
	LOG_FRIEND_GIVE,				// $ 20003 = ���ͺ���
	LOG_FRIEND_GET,					// $ 20004 = ���ܺ�������

	LOG_FRIEND_MAX = 20999,			// $ 20999 = ���������־����
};

// $@ 31 = ʱװ��־ 21000-21999
enum LOG_FASHION_TYPE
{
	LOG_FASHION_UNLOCK_EQUIP = 21000,	// $ 21000 = װ������ʱװ


	LOG_FASHION_MAX = 21999,	// $ 21999 = ʱװ��־�������ֵ    
};


// �춯����
enum LOG_ACT_EVENT_TYPE
{
	LOG_ACT_EVENT_GAIN = 0,			// �춯����-����
	LOG_ACT_EVENT_LOSE = 1,			// �춯����-����
};

// �춯״̬
enum LOG_ACT_STATE_TYPE
{
	LOG_ACT_STATE_FAIL = 0,			// �춯״̬-ʧ��
	LOG_ACT_STATE_SUCCESS = 1,			// �춯״̬-�ɹ�
};
/****************************************���ص����ݿ�-end-**************************************************************************************/



#endif // __LogDefine_H__
