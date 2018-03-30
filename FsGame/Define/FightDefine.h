//------------------------------------------------------------------------------
// �ļ���:      FightDefine.h
// ��  ��:      ս��������غ궨��
// ˵  ��:
// ��������:    2013��3��18��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#ifndef __FIGHTDEFINE_H__
#define __FIGHTDEFINE_H__

// ս����Ϊ״̬ (�� : FightActionState)
#define  FIGHT_ACTION_STATE "FightActionState"

//#define LEAVE_FIGHT_CD_TIME 5   //5��

//�˺�����
enum DAMAGE_CATE
{
    DAMAGE_CATE_DEFAULT = 0, //ȱʡ�˺�����
    DAMAGE_CATE_SKILL = 1,  //�����˺�
    DAMAGE_CATE_BUFFER = 2, //buffer�˺�
};

//��������
enum HEALTHY_CATE
{
    HEALTHY_CATE_DEFAULT = 0, //ȱʡ��������
    HEALTHY_CATE_SKILL = 1, //��������
    HEALTHY_CATE_BUFFER = 2, //buffer����
};

// �������
enum EHIT_TYPE
{
    HIT_TYPE_INVALID = -1,
    //������
    PHY_HIT_TYPE_MISS,//δ����
//    PHY_HIT_TYPE_DODGE,//����
    PHY_HIT_TYPE_VA,//����
    PHY_HIT_TYPE_NORMAL,//һ������
};
//������buffʱ������
enum {
	CONTROL_BUFF_FOREVER = 0, //������buff
	CONTROL_BUFF_TIMES = 1,  //ʱ����buff

};

// ������buff����
enum ControlBuff
{
	CB_CANT_MOVE,			// �����ƶ�
	CB_CANT_ATTACK,			// ���ܹ���
	CB_CANT_BE_ATTACK,		// ���ܱ�����
	CB_CANT_USE_SKILL,		// ����ʹ�ü���
	CB_STIFF_STATE,			// Ӳֱ״̬��

	MAX_CB_BUFF_NUM		
};

// ������buffid��ʱЧ
const char* const CONTROL_TIME_BUFF_ID[MAX_CB_BUFF_NUM] = {"buff_CantMove",
													"buff_CantAttack",
													"buff_CantBeAttack",
													"buff_CantUseSkill",
													"buff_StiffState"};

// ������buffid����
const char* const CONTROL_FOREVER_BUFF_ID[MAX_CB_BUFF_NUM] = {"buff_Forever_CantMove",
													"buff_Forever_CantAttack",
													"buff_Forever_CantBeAttack",
													"buff_Forever_CantUseSkill",
													"buff_Forever_StiffState"};

// ս����Ϊ״̬
enum FightActionState
{
	FAS_NORMAL_STATE = 0,			// ����״̬
	FAS_HIT_BACK_STATE,				// ����״̬
	FAS_HIT_DOWN_STATE,				// ����״̬
	FAS_HIT_FLY_STATE,				// ����״̬
	FAS_FLOAT_STATE,				// ����״̬

	FAS_STATE_NUM
};

// Ŀ���ܻ�����
enum HitTargetType
{
	HTT_NORMAL = 0,        			//ԭ���ܻ�
	HTT_HIT_BACK,     	   			//����
	HTT_HIT_DOWN,     	   			//����
	HTT_HIT_FLY,      	   			//����
	HTT_FLOAT,        	   			//����
	HTT_HIT_LEFT,     	   			//���ܻ�
	HTT_HIT_RIGHT,    	   			//���ܻ�
};

// ����״̬
enum PlayerStatus
{
	PLAYER_STATUS_LIVING = 0,			// ����
	PLAYER_STATUS_DEAD = 1,		        // ����
};

#endif // __FIGHTDEFINE_H__
