#include "..\..\public\Inlines.h"
//------------------------------------------------------------------------------
// �ļ���:      CampaignDefine.h
// ��  ��:		�
// ˵  ��:
// ��������:    2017��2��13��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#ifndef __CAMPAIGN_DEFINE_H__
#define __CAMPAIGN_DEFINE_H__

// �����
#define PUB_CAMPAIGN_MAIN_REC "pub_campaign_main_rec"		// �������״̬����

enum
{
	CMR_PUB_COL_ACTIVITY_ID,					// �ID			 VTYPE_INT
	CMR_PUB_COL_ACTIVITY_TYPE,					// �����			 VTYPE_INT
	CMR_PUB_COL_STATE,							// �״̬			 VTYPE_INT
	CMR_PUB_COL_END_TIME,						// �״̬����ʱ��	 VTYPE_INT64

	MAX_CMR_PUB_COL_NUM,
	MAX_CMR_PUB_ROW_NUM = 10
};

// �״̬
enum CampaignState
{
	CAMPAIGN_CLOSE_STATE = -1,							// ��ر�״̬
	CAMPAIGN_BEGIN_STATE,								// ���ʼ�ĵ�һ��״̬
};

// �����
enum CampaignType
{
	CT_NONE_TYPE = -1,
	CT_ASURA_BATTLE,				// ����ս��

	MAX_CAMPAIGN_TYPE_NUM
};

// ��ڲ�����Ϣ��Ϣ
enum CampaignComand
{
	CAMPAIGN_STATE_CHANGE_MSG,		// �״̬�ı� int gametype, int state
	CAMPAIGN_BOSS_INVADE_SEND_REWARD, // boss���ֽ�������
};

// �Զ�����Ӫ
enum CampType
{
	CAMP_NONE,
	CAMP_RED,			// �췽
	CAMP_BLUE,			// ����

	CAMP_TYPE_TOTAL
};

// ��ȡ��Ӫ����id
inline const char* GetCampTxtId(int nCampId)
{
	static char strTxtId[64] = { 0 };
	SPRINTF_S(strTxtId, "%s%d", "Camp_", nCampId);

	return strTxtId;
}
/************************************************************************/
/* ����ս�������ض���                                               */
/************************************************************************/
// ����ս���״̬
enum AsuraBattleState
{
	ASURA_BATTLE_CLOSE = -1,	// ��ر�
	ASURA_BATTLE_READY,			// �׼��״̬
	ASURA_BATTLE_START,			// ���ʼ
};

// �ͻ���->����� ����Ϣ
enum
{
	CS_ASURA_BATTLE_ENTER,					// ��������ս��
	CS_ASURA_BATTLE_COLLECTION,				// �ɼ�ս��	
	CS_ASURA_BATTLE_BUY_BUFF,				// ����buff	 string buffid
	CS_ASURA_BATTLE_EXIT,					// �˳�����ս��		
	CS_ASURA_BATTLE_SUBMIT,					// �ύս��
	CS_ASURA_BATTLE_QUERY_RANK_LIST,		// ��ѯս�����а�
};

// �����->�ͻ��� ����Ϣ
enum
{
	/*!
	* @brief	��Ӫս������
	* @param	int			����
	����ѭ��2��	
	* @param	int			��Ӫ����
	* @param	int			�ܻ���
	* @param	int			��ս״̬
	* @param	int			�ɼ�����
	* @param	float x,z	���ֵ�һ��ҵ�λ��
	*/
	SC_ASURA_BATTLE_FIGHT_INFO,			

	/*!
	* @brief	����buff�ɹ�
	*/
	SC_ASURA_BATTLE_BUY_BUFF_SUC,

	/*!
	* @brief	�����
	* @param	int			��������
	* @param	int			��͢����
	* @param	int			�Լ�������
	* @param	int			�Լ��Ļ���
	* @param	string		���ν���
	* @param	string		ʤ������
	* @param	int			ǰn��
	����ѭ��n��
	* @param	int			����
	* @param	wstring		�����
	* @param	int			����
	*/
	SC_ASURA_BATTLE_SETTLE_INFO,

	/*!
	* @brief	��ʾ����buff
	*/
	SC_ASURA_BATTLE_BUY_BUFF_TIP,

	/*!
	* @brief	����ս����ʼ
	*/
	SC_ASURA_BATTLE_START,

	/*!
	* @brief	ս��ʵʱ���а�
	* @param	int			�Լ�������

	* @param	int			������ɱ����
	* @param	int			ǰn��
	����ѭ��n��
	* @param	int			����
	* @param	wstring		�����
	* @param	int			����
	* @param	int			��ɱ����
	* @param	int			��������
	* @param	int			��������

	* @param	int			��͢��ɱ����
	* @param	int			ǰn��
	����ѭ��n��
	* @param	int			����
	* @param	wstring		�����
	* @param	int			����
	* @param	int			��ɱ����
	* @param	int			��������
	* @param	int			��������
	*/
	SC_ASURA_BATTLE_FIGHTINT_RANK_INFO,

	/*!
	* @brief	bossս������
	* @param	int			�������˺�
	* @param	int			��͢���˺�
	* @param	float		bossѪ���ٷֱ�
	*/
	SC_ASURA_BATTLE_FIGHT_BOSS_DATA,

	/*!
	* @brief	�������ʳɹ�
	*/
	SC_ASURA_BATTLE_SUBMIT_MATERIAL,
};

// ��Ӫ�ɼ��׶�
enum CAMPCollectState
{
	CAMP_COLLECT_START,			// �ﱸ�׶�
	CAMP_COLLECT_READY,			// ��ս�׶�
	CAMP_COLLECT_FIGHT,			// ȫ����װ�׶�
};

// ��һ��ֱ�
enum 
{
	 CAMP_PLAYER_NONE = -1,
	 CAMP_PLAYER_COL_NAME,		// �����
	 CAMP_PLAYER_COL_SCORE,		// ����
	 CAMP_PLAYER_COL_KILL,		// ɱ����
	 CAMP_PLAYER_COL_DEAD,		// ������
	 CAMP_PLAYER_COL_ASSIST,	// ������
};

#define SYS_INFO_ASURA_BATTLE_READY_NOTICE			"sys_info_asura_battle_ready_notice"	// ����ս��������ʼ
#define SYS_INFO_ASURA_BATTLE_START_NOTICE			"sys_info_asura_battle_start_notice"    // ����ս����ʼ

#define SYS_INFO_ASURA_BATTLE_ENTER_FAILED_LEVEL "sys_info_asura_battle_enter_failed1"	// ��ҵȼ�����
#define SYS_INFO_ASURA_BATTLE_ENTER_FAILED_CLOSE "sys_info_asura_battle_enter_failed2"	// ����ս��δ����

#define SYS_INFO_ASURA_BATTLE_COLLECT_FAILED_NOT_EXIST "sys_info_asura_battle_collect_failed1"	// ս�ʲ�������

#define SYS_INFO_ASURA_BATTLE_KILL_PLAYER_NOTICE "sys_info_asura_battle_kill_player_notice"	// {0}��{1}ɱ���� {2}��{3}
#define SYS_INFO_ASURA_BATTLE_BUY_BUFF_FAILED_HAD "sys_info_asura_battle_buy_buff_failed1"	// �Ѿ���buff�� 
#define SYS_INFO_ASURA_BATTLE_BUY_BUFF_FAILED_CAPITAL "sys_info_asura_battle_buy_buff_failed2"	// Ǯ����

#define SYS_INFO_ASURA_BATTLE_TIME_LEFT_TIP "sys_info_asura_battle_time_left_tip"	// ʣ�����ʱ����ʾ
#define SYS_INFO_ASURA_BATTLE_FIRST_TIP "sys_info_asura_battle_first_tip"		// {0}��{1}��{2}��{3}������һ,��ȥ��ɱ
#define SYS_INFO_ASURA_BATTLE_DRAW_RESULT "sys_info_asura_battle_draw_result"		// ƽ�ֽ��
#define SYS_INFO_ASURA_BATTLE_WIN_RESULT "sys_info_asura_battle_win_result"			// {0}ʤ����
#define SYS_INFO_ASURA_BATTLE_BOSS_BORN "sys_info_asura_battle_boss_born"			// Boss��������
#define SYS_INFO_ASURA_BATTLE_BOSS_TIP "sys_info_asura_battle_boss_tip"				// Boss��ʾ����
#define SYS_INFO_ASURA_BATTLE_BOSS_DEAD "sys_info_asura_battle_boss_dead"			// Boss�������� boss��ɱ,{0}��Ӫʤ��,ȫԱ������{1}��
#define SYS_INFO_ASURA_BATTLE_COLLECT_STATE_CHANGE "sys_info_asura_battle_collect_state_change"			// {0}��Ӫ�ﵽ��{1}�ɼ�״̬
#define SYS_INFO_ASURA_BATTLE_ADD_SCORE_KILL "sys_info_asura_battle_add_score_kill"			// ��ɱ{0}��û���{1}
#define SYS_INFO_ASURA_BATTLE_ADD_SCORE_ASSIST "sys_info_asura_battle_add_score_assist"		// ��ɱ{0},����������û���{1}


#endif // __CAMPAIGN_DEFINE_H__
