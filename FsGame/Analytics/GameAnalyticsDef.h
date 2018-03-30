//--------------------------------------------------------------------
// �ļ���:	GameAnalyticsDef.h
// ��  ��:	��Ϸ���ݷ����ռ�������궨��
// ˵  ��:	
// ��������:	2016��08��24��
// ������:	���
// ������:            
//--------------------------------------------------------------------
#ifndef __GAME_ANALYTICS_DEFINED_H__
#define __GAME_ANALYTICS_DEFINED_H__

// ��������ģ��
enum EGameModule
{
	EGAME_MODULE_BASE = 1,			// ����ģ��
	EGAME_MODULE_BAG,				// ����ģ��
	EGAME_MODULE_EQUIP,				// װ��ģ��

	EGAME_MODULE_TASK,				// ����ģ��
	EGAME_MODULE_ACHIEVEMENT,		// �ɾ�ϵͳ
	EGAME_MODULE_GUILD,				// ����ģ��

	EGAME_MODULE_FIGHT,				// ս��ģ��
	EGAME_MODULE_SKILL,				// ����ģ��
	EGAME_MODULE_RIDE,				// ����ģ��

	EGAME_MODULE_ACTIVITY,			// �ģ��
	EGAME_MODULE_SOCIAL,			// �罻ģ��
};

// ������Ϸ����
enum EGameAction
{
	//// ����( 1-199 )
	//EM_ACTION_RND_NATION = 1,				// ���ѡ�����
	EM_ACTION_ROLE_CREATE,					// ����
	//EM_ACTION_GM_SUPPLY,					// GM����
	//EM_ACTION_PAY,							// ��ֵ
	//EM_ACTION_SHARE,						// ����Ȧ����
	//EM_ACTION_VIP_LEVELUP,					// vip����

	//// ����ģ��(200-399)
	//EM_ACTION_SALE_ITEM,					// ������Ʒ
	//EM_ACTION_BUY_ITEM,						// ������Ʒ
	//EM_ACTION_BUY_UNTREAD,					// �����˻�
	//EM_ACTION_USE_ITEM,						// ʹ����Ʒ


	//// ����(-99)
	//EM_ACTION_TASK_ACCEPT,						// �������� ��Ҫ����: ����id ��������
	//EM_ACTION_TASK_CANCEL,						// ȡ������ ��Ҫ����: ����id ��������
	//EM_ACTION_TASK_GIVE,						// �ύ���� ��Ҫ����: ����id
	//EM_ACTION_TASK_AWARD,						// ��ȡ������ ��Ҫ����: ����id �Ƿ�ɹ�
	//EM_ACTION_TASK_CLEAR,						// ������� ��Ҫ����: ����id �Ƿ�ɹ�
	//EM_ACTION_TASK_REFRESH,						// ˢ������
	//// TEST
	//EM_ACTION_TASK_FAILED_AWARD,				// ��ȡ����ʧ��
	//EM_ACTION_TASK_ACTIVITY_CANCEL,				// �����ճ������

	////// �ɾ�(100-199)
	//EM_ACTION_ACHIEVMENT_COMPLETED,				// ��ɳɾ� ��Ҫ����: �ɾ�id
	//EM_ACTION_ACHIEVMENT_AWARD,					// ��ȡ�ɾ� ��Ҫ����: �ɾ�id


	//// ����
	//EM_ACTION_GUILD_CREATE,						// ��������
	//EM_ACTION_GUILD_UNTREAT,					// ���������˻�
	//EM_ACTION_GUILD_ACTIVITY,					// ����


	//// ս��
	//EM_ACTION_FIGHT_KILL_PLAYER,			// ɱ��
	//EM_ACTION_FIGHT_KILL_NPC,				// ɱ��
	//EM_ACTION_FIGHT_ENTRY_SCENE,			// ���볡��/����
	//EM_ACTION_FIGHT_SECRET_PASS,			// �ؾ�ͨ��
	//EM_ACTION_FIGHT_SECRET_STAR,			// �ؾ�3�ǽ���

	//// ����
	//EM_ACTION_SKILL_LEVELUP,				// ��������

	//// ����
};

#endif