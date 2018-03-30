#ifndef _PK_MODEL_DEFINE_H_
#define _PK_MODEL_DEFINE_H_

// ��ϵ����
enum TestRelationType
{		
	TRT_FRIEND,			// ��֤�Ƿ�Ϊ�ѷ�
	TRT_ENEMY			// ��֤�Ƿ�Ϊ�з�
};

//��������ģʽ����
enum SceneAttackType
{
	SCENE_ATTACK_PUBLIC_SCENE = 0,	//Ұ�⹫������(ͨ����������pkģʽȷ������Ŀ��)
	SCENE_ATTACK_CAMP,				//ֻҪ��Ӫ��ͬ�Ϳɹ���
	SCENE_ATTACK_GUILD,				//ֻҪ���᲻ͬ�Ϳɹ���
	SCENE_ATTACK_PEACE,				//��ƽģʽ
	SCENE_ATTACK_PVE,				//ֻ�ܴ��ģʽ

	SCENE_ATTACK_TYPE_MAX
};

// ��Ҹ��˹���ģʽ
enum PKType
{							
    PT_PEACE,					// ��ƽģʽ
	PT_GUILD,					// ����ģʽ
	PT_KILL_ALL,				// ɱ¾ģʽ
	PT_ATT_CRIME,				// �ƶ�ģʽ

	PT_MAX_NUM
};

// ���״̬
enum CrimeState
{				
	CS_NORMAL_STATE,				// ����״̬
	CS_GRAY_STATE,					// ����(��ʱ)
	CS_RED_STATE					// ����
};

// ���ͷ�
enum CrimePublish
{
	CP_CAPITAL_RATE,				// ͭǮ�ͷ�ϵ��
	CP_EXP_RATE						// ����ͷ�ϵ��
};

enum PKC2SMsg
{
	PK_C2S_CHANGE_PK_MODEL,				// �ı�PKģʽ int model
	PK_C2S_COST_MONEY_CLEAN_CRIME,		// ����Ԫ��������ֵ int cost
	PK_C2S_SET_DEFAULT_MODEL,			// ���ó���pkĬ��ģʽ int ����id int ģʽ����
};

enum PKS2CMsg
{
	PK_S2C_COST_MONEY_SUCCESS,			// ����Ԫ���������ֵ�ɹ�
};

#define	SYS_INFO_CLEAR_CRIME_VALUE_FAILED_GREATER		"sys_info_clear_crime_value_failed1"		// ���ֵ̫��
#define	SYS_INFO_CLEAR_CRIME_VALUE_FAILED_MONEY			"sys_info_clear_crime_value_failed2"		// ���Ҳ���
#endif