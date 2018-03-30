//--------------------------------------------------------------------
// �ļ���:		NpcDefine.h
// ��  ��:		NPCģ�鹫��ͷ�ļ�
// ˵  ��:		
//				
// ��������:	2014��06��27��
// ��������:	2014��06��27��
// ������:		zhangqs(����ɽ)
// ��Ȩ����:	������ţ���ֿƼ��ɷ����޹�˾
//--------------------------------------------------------------------

#ifndef __NpcDefine_H__
#define __NpcDefine_H__

#pragma region
//NPC��س�������
const char* const NPC_RESOURCE_NPC = "ResourceNpc";
const char* const NPC_REVIVE_NPC = "ReviveNpc";
#pragma endregion

#pragma region
//������س�������
const char* const SCENE_NATION_BATTLE_SCENE = "scene";
const char* const SCENE_NATION_BATTLE_SCENE_ASSIGN = "NationBattleScene";
const char* const SCENE_GUILD_DEFEND_ACTIVITY_SCENE_ASSIGN = "GuildDefendActivityScene";
#pragma endregion

// ������ҵ�npc����
enum BattleNpc
{
	NORMAL_BATTLE_NPC = 0,	// ����npc
	ARENA_BATTLE_NPC,		// ������npc
	WORK_NPC				// ��npc
};
#endif // __NpcDefine_H__
