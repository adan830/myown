#ifndef PlayerBaseDefine_h__
#define PlayerBaseDefine_h__


//���ְҵ
enum
{
	PLAYER_JOB_MIN		= 0,

	PLAYER_JOB_WARRIOR,	// սʿ
	PLAYER_JOB_STABBER,	// �̿�
	PLAYER_JOB_ARCHER,	// ������
	PLAYER_JOB_MAGES,	// ��ʦ
	PLAYER_JOB_DOCTOR,	// ҩʦ

	PLAYER_JOB_MAX,
};

// ���ְҵ�Ƿ�Ϸ�
inline bool IsValidJob(int job)
{
	return (job > PLAYER_JOB_MIN && job < PLAYER_JOB_MAX);
}

// �Ա����Ͷ���
enum SexType
{
	SEX_MAN = 1,
	SEX_WOMAN = 2,
};

// ����Ա��Ƿ�Ϸ�
inline bool IsValidSex(int sex)
{
	return (sex == SEX_MAN || sex == SEX_WOMAN);
}

// ��ɫ��������
enum RoleTypeFlag
{
    ROLE_FLAG_NORMAL_PLAYER = 0, // �������
    ROLE_FLAG_ROBOT         = 1, // ������
};

// ���ȫ�ֱ�־ֵ(ֵ���ܳ���63)
enum EPlayerGlobalFlag
{
	EM_PLAYER_GLOBAL_FIRST_PAYED,			// �׳��־
	EM_PLAYER_GLOBAL_LIMIT_GIFT_FLAG,		// ��ʱ�����־
	EM_PLAYER_GLOBAL_PAYED_REBATE_PICKED,	// ��ֵ������ȡ(�Ƿ���ȡ����, ��ѯ����û�з�����������Ϊ��ȡ)

	EM_PLAYER_GLOBAL_LIMIT_GIFT_FLAG_1,		// ��ʱ�����־1
	EM_PLAYER_GLOBAL_LIMIT_GIFT_FLAG_2,		// ��ʱ�����־2
	EM_PLAYER_GLOBAL_LIMIT_GIFT_FLAG_3,		// ��ʱ�����־3
	EM_PLAYER_GLOBAL_LIMIT_GIFT_FLAG_4,		// ��ʱ�����־4
};


enum EMsgSwitchRole
{
	EMSG_SWITCH_ROLE_REPLY,					// �л����� param:result(0:�ɹ� ����ֵΪ������(����))
	EMSG_SWITCH_STATUS_1,					// �ɹ��л�״̬1(OnStore����) param:none
	EMSG_SWITCH_STATUS_2,					// �ɹ��л�״̬2(OnLeave����) param:none
};

enum EmSwitchRoleResult
{
	EM_SWITCH_ROLE_SUCCEED,			// �ɹ�
	EM_SWITCH_ROLE_FAILED,			// ʧ��
};

enum EmPropQueryResult
{
	EM_PROP_QUERY_RESULT_SUCCEED,			// �ɹ�
	EM_PROP_QUERY_RESULT_OFFLINE,			// ������
};
#endif // PlayerBaseDefine_h__
