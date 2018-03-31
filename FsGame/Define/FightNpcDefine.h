//--------------------------------------------------------------------
// �ļ���:		FightNpcDefine.h
// ��  ��:		NPCģ�鹫��ͷ�ļ�
// ˵  ��:		
//				
// ��������:	2018��03��30��
// ��������:	2018��03��30��
//--------------------------------------------------------------------

#ifndef __BOSS_DEFINE_H__
#define __BOSS_DEFINE_H__

//����BOSS��Ϣ�����
#define SCENE_BOSS_TOTAL_REC "scene_boss_total_rec"   

enum BossState
{
	BS_BOSS_SLEEP = -1,								// boss��ˢ״̬
	BS_BOSS_BORN,									// boss����
	BS_BOSS_DEAD,									// boss����
};

enum
{
	SCENE_BOSS_INFO_CONFIG = 0,						// bossid
	SCENE_BOSS_DEAD_TIME,							// boss����ʱ�� 1��ʾboss����,Ҫˢboss
	SCENE_BOSS_INFO_SCENEID,						// ���ڳ���

	SCENE_BOSS_INFO_MAX,
};


// ������ҵ�npc����
enum BattleNpc
{
	NORMAL_BATTLE_NPC = 0,	// ����npc
	ARENA_BATTLE_NPC,		// ������npc
	WORK_NPC				// ��npc
};
#endif // __BOSS_DEFINE_H__
