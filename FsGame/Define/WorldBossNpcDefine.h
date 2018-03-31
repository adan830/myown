//--------------------------------------------------------------------
// �ļ���:      WorldBossNpcDefine.h
// ��  ��:      WorldBossNpcDefine����ض���
// ˵  ��:		
// ��������:    2015��06��23��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#ifndef __WorldBossNpc_Define_H__
#define __WorldBossNpc_Define_H__

// û���ҵ���Ӧ�����
#define SCENE_NOT_EXIST 0;

// �涨ʱ����û���˻�ɱBOSS
#define LAST_KILL_BOSS_NAME L"sys"
// GM��ʶ
#define FLAG_WORLD_BOSS_GM "WorldBossGM"
// �ID
#define WORLD_BOSS_ACTIVE_CFG_ID "active_cfg_id"

// ���ʶ
#define FLAG_WORLD_BOSS_VISIT "WorldBossVisitFlag"

// ���ʶ
#define FLAG_WORLD_BOSS_CONFIG_DATA_INDEX "WorldBossConfigDataIndex"

enum FlagWorldBossVisit
{
	FLAG_WORLD_BOSS_VISIT_CLOSED  = 0,   // �����,�ر�
	FLAG_WORLD_BOSS_VISIT_NOTICED = 1,   // ׼��֪ͨ�׶�
	FLAG_WORLD_BOSS_VISIT_ACTIVED = 2,   // ��ʼ���볡��
	FLAG_WORLD_BOSS_VISIT_BORNED = 3,    // BOSS����
};


// ����boss�����
enum WorldBossActivityType
{
	WBAT_NONE_WORLD_BOSS = 0,
	WBAT_AM_WORLD_BOSS,				// ���������boss
	WBAT_PM_WORLD_BOSS				// ���������boss
};

#define WORLD_BOSS_DAMAGE_REC "world_boss_damage_rec"		// ����boss�˺���

enum
{
	WBD_COL_PLAYER_RANK,						// �������
	WBD_COL_PLAYER_NAME,						// �������
	WBD_COL_PLAYER_TOTAL_DAMAGE,				// ���˺�

	MAX_WBD_COL_NUM,
};

#define WORLD_BOSS_ELITE_NPC_REC "world_boss_elite_npc_rec"		// ����boss��Ӣ�ֱ�
enum
{
	WBEN_COL_NPC_OBJECT,						// npc�����
	WBEN_COL_NPC_INDEX,							// npc���

	MAX_WBEN_COL_NUM,
};


// �����PUB�����ϻ״̬��
#define WORLD_BOSS_ACTIVE_STATUS_REC "world_boss_active_status_rec"
enum WorldBossActiveStatus_COL
{
	WORLD_BOSS_STATUS_REC_COL_SCENEID = 0,   // ����ID
	WORLD_BOSS_STATUS_REC_COL_STATUS  = 1,   // �״̬ ��ӦWorldBossActiveStatus
	WORLD_BOSS_STATUS_REC_COL_ID      = 2,   // �����ID
};

// ���BOSS�ɳ���Ϣ
#define PUB_WORLD_BOSS_GROW_UP_REC "world_boss_grow_up_rec"
enum WorldBossGroupUp_COL
{
	WORLD_BOSS_GROW_UP_REC_COL_SCENEID      = 0,	// ����ID
	WORLD_BOSS_GROW_UP_REC_COL_LEVEL,				// boss�ȼ�

	MAX_WORLD_BOSS_GROW_ROWS = 50
};

// ����Boss�����б�
#define WORLDBOSS_ATTACK_LIST_REC "worldboss_attack_list_rec"
enum 
{
	WORLDBOSS_ATTACK_LIST_REC_PLAYER_NAME	= 0,    // ������������
	WORLDBOSS_ATTACK_LIST_REC_DAMAGE				// ���˺�
};

// ����������
#define PUB_KILL_WORLD_BOSS_PLAYER_NAME_REC "kill_world_boss_player_name_rec"
enum 
{
	KWBP_PUB_COL_SCENE_ID = 0,
	KWBP_PUB_COL_KILLER_NAME,						// ɱ��boss��ҵ�����
	KWBP_PUB_COL_BOSS_STATE,						// boss ״̬

	MAX_KWBP_PUB_COL_NUM,
	MAX_KWBP_PUB_ROW_NUM = 100
};

#define BOSS_ACTIVE_OPEN_TIMES "boss_active_open_times"
enum
{
	BOSS_ACTIVE_OPEN_TIMES_REC_SCENE      = 0,
	BOSS_ACTIVE_OPEN_TIMES_REC_OPEN_TIMES = 1,
};

// ����BOSS�״̬
enum WorldBossActiveStatus
{
	STATE_WBOSS_ACTIVE_CLOSE       = 0,      // ��ر�
	STATE_WBOSS_ACTIVE_NOTICE      = 1,      // ���������
	STATE_WBOSS_ACTIVE_OPEN        = 2,      // ���ʽ����
};    


// �ͻ�������Ϣ����
enum CS_WorldBossSubMsgID
{
	CS_WORLD_BOSS_ACTIVE_TRANSMIT            = 0,	// ����� ���͵������ int sceneid
	CS_WORLD_BOSS_ACTIVE_BACK_TO_LAST_SCENE,		// ���;�˳�  ������һ������,�Ҳ����ͻػʳ�
	CS_WORLD_BOSS_ACTIVE_QUERY_ACTIVE_STATUS,		// ��ѯ��ǰ�״̬
	CS_WORLD_BOSS_ACTIVE_QUERY_BASE_INFO,			// ��ѯ����boss��ɱ�Ļ�������
// 	CS_WORLD_BOSS_ACTIVE_QUERY_CURR_DAY_TYPE,		//  ��ʱ���� ��ѯ����Ļ����
// 	CS_YEAR_BOSS_BUY_BUFF,							// ��ʱ���� ���޻����buff int-����������� 
// 	CS_YEAR_BOSS_QUERY_COOLDOWN_TIME,				// ��ʱ���� ���޻��ѯ����buff CDʱ��	

};
// ���������Ϣ����
enum SC_WorldBossSSubMsgId
{
	SC_WORLD_BOSS_ACTIVE_OPEN          = 0,             // �����  intʣ��ʱ��
	/*!
	* @brief	�������
	* @param	int			�ҵ�����
	* @param	int			�ҵ��˺�
	* @param	string		�ҵĽ���
	*/
	SC_WORLD_BOSS_ACTIVE_ACTOR_AWARD,					

	SC_WORLD_BOSS_ACTIVE_CLOSE,							// ����� 
	/*!
	 * @brief	 ���������
	 * @param	int		n������boss����
	 ����ѭ��n��
	 * @param	int		����id
	 * @param	wstring	��ɱboss�������
	 * @param	int		Boss״̬ WorldBossActiveStatus 0�ر� 2����
	 */
	SC_WORLD_BOSS_ACTIVE_BASE_INFO,		

	SC_WORLD_BOSS_START_PLAY_CG,					// ��ʼ����CG

	/*!
	* @brief	ʵʱ��������
	* @param	int		ǰn��
	����ѭ��n��
	* @param	int		�������
	* @param	wstring �������
	* @param	int		����˺�ֵ
	*/
	SC_WORLD_BOSS_ACTIVE_HURT_RANK,					// ʵʱ�����˺��б�  

	/*!
	* @brief	ʵʱ��������
	* @param	int			�ҵ�����
	* @param	int			�ҵ��˺�
	*/
	SC_WORLD_BOSS_SELF_HURT_INFO,					// ����Լ���ʵʱ��������

	/*!
	* @brief	����npc����
	* @param	int		n����Ӣ��
	����ѭ��n��
	* @param	int		���
	*/
	SC_WORLD_BOSS_LIVE_ELITE_NPC,							// �����ڹֵ�״̬

	/*!
	* @brief	npc����
	* @param	int		npc���
	*/
	SC_WORLD_BOSS_ELITE_NPC_DEAD,							// ĳ����Ӣ������

	/*!
	* @brief	����봰��
	*/
	SC_WORLD_BOSS_ACTIVE_SHOW_ENTRY_PANEL,

	SC_WORLD_BOSS_ACTIVE_LEVEL_LIMIT,             // ��ʱ���� �ȼ����� 
	SC_WORLD_BOSS_ACTIVE_CURR_TYPE,             // ��ʱ���� �������� int����� int��������
	SC_YEAR_BOSS_ADD_BUFF_SUCCESS,             // ��ʱ���� ����buff�ɹ�   int-ͭ��buff�㼶   int-Ԫ��buff�㼶 
	SC_YEAR_BOSS_ADD_BUFF_FAIL,             // ��ʱ���� ����buffʧ��. 
	SC_YEAR_BOSS_QUERY_COOLTIME,             // ��ʱ����  ��ȴʱ��.         

};

// ����������������������Ϣ����
enum SP_WorldBossSubMsgId 
{
	//SP_WORLD_BOSS_ACTIVE_STATUS_QUERY    = 0,  // ��ѯ״̬
	SP_WORLD_BOSS_ACTIVE_STATUS_SET      = 1,  // ����״̬
	//SP_WORLD_BOSS_ACTIVE_TYPE_SET        = 2,  // �������û����
	SP_WORLD_BOSS_BE_KILL_TIME_SET       = 3,  // ����BOSS����ɱ����
	SP_WORLD_BOSS_BE_KILL_TIME_SET_GM    = 4,  // ����BOSS����ɱ����
	//SP_WORLD_BOSS_CREATE_TIME_SET        = 5,  // ����BOSS����ʱ��
	//SP_WORLD_BOSS_TODAY_CHECKED          = 6,  // ���õ����Ƿ���������
	//SP_WORLD_BOSS_ACTIVE_MODEL           = 7,  // ���ûģʽ

	SP_NATION_BATTLE_OPEN				= 10,	// ��ս����
	SP_WORLD_BOSS_OPEN					= 11,	// ����boss����
	SP_GUILD_DEFEND_OPEN				= 12,	// ���ս����
	SP_SAVE_ACTIVE_TIMES				= 13,	// ��¼���������
	SP_UPDATE_BOSS_REC_INFO				= 14,	// ��¼boss������
};
// ������������������������Ϣ����
enum PS_WorldBossSubMsgId
{
	PS_QUERY_ACTIVE_STATUS            = 0,   // ��ѯ״̬���
};

enum SS_WorldBossSubMsgId
{
	SS_WORLD_BOSS_ACTIVE_RESTART        = 0,   // �����
	SS_WORLD_BOSS_ACTIVE_UPDATE_DATA    = 1,   // ����member������
	SS_WORLD_BOSS_ACTIVE_END            = 2,   // �����
};

// �����
enum ActiveTypeCode
{
	ACTIVE_TYPE_MIN           = 0,
	ACTIVE_TYPE_NATION_BATTLE = 1,   // ��ս
	AVTIVE_TYPE_GUILD         = 2,   // ���
	ACTIVE_TYPE_WORLD_BOSS    = 3,   // ����BOSS�����磩
	AVTIVE_TYPE_GVG_GUILD     = 4,   // �����վ
	ACTIVE_TYPE_WORLD_BOSS_AM = 5,   // ����BOSS�����磩
	ACTIVE_TYPE_YEAR_BOSS_AM  = 6,   // ����BOSS�����磩
	ACTIVE_TYPE_YEAR_BOSS     = 7,   // ����BOSS�����磩
	ACTIVE_TYPE_MAX,
};

// �����
enum IsActiveChecked
{
	ACTIVE_IS_CHECKED_OFF     = 0,
	ACTIVE_IS_CHECKED_ON      = 1,
};

// ����
enum ActiveResult
{
	FAIL_KILL_WORLD_BOSS = 0,		// ��ɱ����bossʧ��
	SUC_KILL_WORLD_BOSS				// ��ɱ����boss�ɹ�
};

enum ActiveRule
{
	ACTIVE_RULE_IGNORE  = 0,
	ACTIVE_RULE_OPEN    = 1,
};

#define TIMER_WORLDBOSS_ACTIVE 5  // ��

enum BranchActiveType
{
	ACTIVE_MODEL_TYPE_MIN       = 0,
	ACTIVE_MODEL_TYPE_WORLDBOSS = 1,
    ACTIVE_MODEL_TYPE_YEARBOSS  = 2,
	ACTIVE_MODEL_TYPE_MAX,
};

enum BossActiveBuffPayType
{
	BOSS_ACTIVE_BUFF_PAY_TYPE_COPPER = 1,
	BOSS_ACTIVE_BUFF_PAY_TYPE_SILVER = 2,
};

#endif

