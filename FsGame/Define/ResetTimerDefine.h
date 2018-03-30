//--------------------------------------------------------------------
// �ļ���:      ResetTimerDefine.h
// ��  ��:      ��ʱ������
// ˵  ��:
// ��������:    2016��03��28��
// ������:        tongzt
// �޸���:        
//--------------------------------------------------------------------

#ifndef  _RESET_TIMER_DEFINE_H
#define _RESET_TIMER_DEFINE_H

#include <map>

// ����ʱ��,ˢ�¼�����ֻ��ָ�����ڴ�ʱ��ĳ���
#define RESET_HEART_BEAT_CYCLE  1

/*
 * ��ʱ����������
 */
enum ResetTimerTypes
{
	RESET_TYPES_NONE = 0,
	RESET_DAILY_TASK,										// �����ճ�����
	RESET_DDY_TOPLIMIT,										// 0�����õ����ۼ������ʽ��¼ֵ
	RESET_ROLE_LOGIN_TIME,									// 0����µ�¼ʱ��
	RESET_WING_STEP_BLESS,									// ���ó������ף��ֵ

	RESET_FRIEND_WEEK_FLOWER_NUM,							// ���ú����ܳ��ջ�
	RESET_DAILY_FRIEND,										// ÿ�����ú��ѹ���

	RESET_SHOP_GOLD,										// Ԫ���̵�ˢ��
	RESET_SHOP_GOLD_TIMES,									// Ԫ���̵�ˢ�´���ˢ��

	RESET_RIDE,												// ����
	RESET_GUILD_DAILY_RESET,								// ��֯ÿ������

	RESET_TYPES_TOTAL,
};

// ��ʱ������
enum TimerTypes
{
	TIMER_TYPE_DAY = 0,		// ��ˢ��
	TIMER_TYPE_WEEK,		// ��ˢ��
};

// ˢ������
struct ResetData
{
	int reset_type;				// ˢ������
	std::string reset_timer;	// ˢ��ʱ��
	int reset_weekday;			// ˢ��weekday

	ResetData() :
		reset_type(0), 
		reset_weekday(-1)
		{
		}
};

// ˢ�º�����ʽ����
typedef int (*ResetTimer)(IKernel* pKernel, const PERSISTID& self, int slice);

typedef std::map<int, ResetTimer> ResetFuncMap;	// ��������-ָ��ӳ��
typedef std::map<int, ResetData>  ResetdDataMap;	// ˢ������-ˢ������

#endif //_RESET_TIMER_DEFINE_H