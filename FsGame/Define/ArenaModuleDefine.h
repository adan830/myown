//---------------------------------------------------------
//�ļ���:       ArenaModuleDefine.h
//��  ��:       ��������һЩ����
//˵  ��:       
//          
//��������:      2015��06��25��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------
#ifndef _FSGAME_ARENA_MODULE_DEFINE_H_
#define _FSGAME_ARENA_MODULE_DEFINE_H_

// ��������ս�����������С
#define ARENA_CHALLENGE_CHANCE_MAX_SIZE     100

// �������ͻ�����Ϣ
enum ArenaC2SMsg
{
	ARENA_C2S_QUERY_SELF_INFO	= 1,				// ��ѯ�Լ��ľ���������
	ARENA_C2S_GET_CANDIDATE,						// ��ȡ��ѡ��(������ս)
	ARENA_C2S_CHALLENGE_SOMEONE,					// ������սĳ�� wstring ��ս��ҵ�����
	ARENA_C2S_GIVE_UP,								// ������ս
	ARENA_C2S_CHALLENGE_CLEAR_CD,					// ���þ�������սCD
	ARENA_C2S_REQUEST_FIGHT_RESULT,					// ����ս������
};

// ��������������Ϣ
enum ArenaS2CMsg
{
	/*!
	* @brief	�ظ��Լ��ľ���������
	* @param	int ������������ս����
	* @param	int ����������ս����
	* @param	int ����������
	* @param	int ս����
	* @param	int ս��
	* @param	string �������а�
	*/
	ARENA_S2C_REV_SELF_INFO		= 1,

	/*!
	* @brief	�ظ���ս��ѡ������
	* @param	player_uid,player_account,player_name,player_career,player_sex,player_nation,player_level,player_online_type,player_online,player_vip_level
	*/
	ARENA_S2C_REV_CANDIDATE_INFO,

	ARENA_S2C_COUNTDOWN_BEFORE_FIGHT,				// ����ǰ�ĵ���ʱ��ʼ int ����ʱ(��λ:S)
	ARENA_S2C_FIGHT,								// ���� int һ�ֵ�ʱ��(��λ:S)

	/*!
	* @brief	��ս���� 
	* @param	int ��� 0 ʤ 1 ��
	* @param	string ���ҽ���
	* @param	string ��Ʒ����
	* @param	int ԭ��������
	* @param	int ������
	* @param	int ��ʷ�������
	* @param	int ͻ����ʷ��������Ľ���
	*/
	ARENA_S2C_CHALLENGE_END,		

	/*!
	* @brief	������ս�����ɹ�
	* @param	int ��ǰ��ս����
	*/
	ARENA_S2C_BUY_CHANCE_SUCCESS,	

	/*!
	* @brief	�ظ�ս������
	* @param	int ��ǰս����n
	����ѭ��n��
	* @param	int ս�� 0ʤ 1��
	* @param	wstring  �з�����
	* @param	int  սǰ����
	* @param	int  ս������
	* @param	int64  ս��ʱ��
	*/
	ARENA_S2C_REV_FIGHT_RESULT_DATA,

	/*!
	* @brief	ս��������ʾ
	*/
	ARENA_S2C_FIGHT_RESULT_CHANGE_TIP,
};

//��public��������ͨ�Ŷ�����Ϣ
enum ArenaPubSubMsgID
{
    ARENA_PUB_SUB_MSG_NULL                   = 1, // 
    ARENA_PUB_SUB_MSG_GIVE_RANK_REWARD       = 2, // ֪ͨ������������
    ARENA_PUB_SUB_MSG_CHALLENGE_REQUEST      = 3, // ��ս����
    ARENA_PUB_SUB_MSG_SWAP_RANK              = 4, // ��������
    ARENA_PUB_SUB_MSG_PROMOTE                = 5, // �������
    ARENA_PUB_SUB_MSG_GET_CANDIDATE          = 6, // ��ȡ��ѡ��
    ARENA_PUB_SUB_MSG_UNLOCK_PLAYER          = 7, // ������ս˫��
    ARENA_PUB_SUB_MSG_ADD_TO_RANKLIST        = 8, // ����Ҽ������а�
    ARENA_PUB_SUB_MSG_CHALLENGE_END          = 9, // ��ս����
	ARENA_PUB_SUB_MSG_GM_RESET_ARENA_STATE   = 10,// gm����һ����ҵľ�����״̬

	/*!
	* @brief	����һ��ս���������
	* @param	int ս�� 0ʤ 1��
	* @param	wstring  �з�����
	* @param	int  սǰ����
	* @param	int  ս������
	* @param	int64  ս��ʱ��
	*/
	ARENA_MSG_ADD_FIGHT_RESULT_DATA			 = 11,
};

// ֪ͨ��command��Ϣ
enum ArenaPlayerNotify
{
    ARENA_PLAYER_NOTIFY_RANK            = 1, // �����仯
    ARENA_PLAYER_NOTIFY_CHALLENGE       = 2,  // �ھ�������ս
    ARENA_PLAYER_NOTIFY_CHALLENGE_WIN   = 3,  // �ھ�������ս���ʤ��
};

// ���������а�Ĺؼ���
enum ArenaRankListCol
{
    ARENA_RANK_COL_UID = 0,
};

// ��ҵľ�����״̬
//enum RoleArenaState
//{
//    ARENA_STATE_NOT_ARENA   = 0,   // ���ھ�����
//    ARENA_STATE_IN_ARENA    = 1,   // ���ھ�����
//};

// �������ı�ʶ��
enum ArenaFlag
{
    ARENA_FLAG_NOT_ARENA    = 0, // Ĭ��״̬���ھ�����
    ARENA_FLAG_ENTERING     = 1, // ���ڽ��뾺����
    ARENA_FLAG_READY        = 2, // �ھ������о���
    ARENA_FLAG_IN_BATTLE    = 4, // �ھ�������ս��
    ARENA_FLAG_END          = 5, // ������������
    ARENA_FLAG_ESCAPE_RETURN  = 6, // ��������ֻ���
};

enum ArenaChallengeResult
{
    ARENA_RET_CHALLENGE_WIN        = 0,    // ��ս��ʤ
    ARENA_RET_CHALLENGE_LOSE       = 1,    // ��սʧ��
    ARENA_RET_TIME_OUT              = 2,    // ��ʱδ�ֳ�ʤ��
    ARENA_RET_ONE_DIE               = 3,    // һ������
    ARENA_RET_ESCAPE                = 4,    // ��ս������
};

// ����������
enum ArenaCode
{
    ARENA_CODE_NULL                     = 0, // ľ������Ĵ���
    
    ARENA_CODE_SUCCESS                  = 1, // �ɹ���
    ARENA_CODE_FAILED                   = 2, // ʧ����
    
    ARENA_CODE_ERROR                    = 10, // δ֪ԭ��Ĵ���
    ARENA_CODE_ONE_IN_CHALLENE          = 11, // һ������ս��
    ARENA_CODE_ONE_NOT_FOUND            = 12, // ˫������ͬһ������
    ARENA_CODE_CHALLENGEE_LOW_RANK      = 13, // �������ε����Լ�
    ARENA_CODE_RANK_LIST_FULL           = 14, // ���а�����
    
};

// gm����
enum
{
	UNLOCK_ARENA_STATE = 0,			// gm�������������״̬
	LOCK_ARENA_STATE,				// gm��������������״̬
};

// SNS��ض���
#define SNS_ARENA_SPACE				L"sns_arena_space"

#define  SNS_ARENA_FIGHT_RESULT_REC	"arena_fight_result_rec"		// ����ս�����ݱ�
enum 
{
	SNS_ARENA_COL_FIGHT_RESULT,				// ս�����
	SNS_ARENA_COL_FIGHT_ENEMY_NAME,			// �з�����
	SNS_ARENA_COL_FIGHT_PRE_RANK,			// ս��ǰ������
	SNS_ARENA_COL_FIGHT_CUR_RANK,			// ս���������
	SNS_ARENA_COL_FIGHT_TIME,				// ս��ʱ��

	MAX_ARENA_COL_NUM,
	MAX_ARENA_ROW_NUM = 20
};

#endif