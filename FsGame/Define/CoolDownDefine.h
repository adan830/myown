#if !defined __CoolDownDefine_h__
#define __CoolDownDefine_h__

//��ȴ���Ͷ���
enum COOLDOWN_CATEGORY
{
    COOLDOWN_CATEGOTYID_PUB             = 0,            // ������ȴ(Ŀǰ���ڵ�����Ʒ��������Ч��)
    COOLDOWN_CATEGOTYID_SKILL           = 1,            // ���ܹ�����ȷ
    COOLDOWN_CATEGOTYID_ITEM            = 2,            // ���߹�����ȷ

    //������Ƶ����ȴ����
    //[BEGING, END] ��֮���ֵ�벻Ҫʹ��
    COOLDOWN_CAREGOTYID_CHATTYPE_BEGIN  = 50,
	COOLDOWN_CHAT_WHISPER,						// ˽��CD
	COOLDOWN_CHAT_WORLD,						// ����CD
	COOLDOWN_CHAT_GUILD,						// ���CD
	COOLDOWN_CHAT_TEAM,							// ���CD
	COOLDOWN_CHAT_WORLD_NEW,					// С����CD
    COOLDOWN_CAREGOTYID_CHATTYPE_END    = 100,

    // �����Զ�����ȴ200-2000																															 
    COOLDOWN_CATEGOTYID_RIDESPURT       = 200,
    COOLDOWN_ARRANGE_TOOLBOX,					// ����������ȴʱ��
    COOLDOWN_CAPITAL_MONITOR ,					// ������ʽ�������ʾ
    COOLDOWN_ARRANGE_ITEMBOX,					// �������ĵ��߱�������ȴʱ��
	COOLDOWN_NPC_ESCAPE,						// npc������ΪCD

    // ������ȴID
    COOLDOWN_SKILL_BEGIN                = 2000,
	COOLDOWN_PlAYER_NORMAL_SKILL		= 5000,

	// ����buffCD
	COOLDOWN_SKILL_BUFF					= 19000,
    COOLDOWN_SKILL_END                  = 19999, 

    // ������ȴID
    COOLDOWN_ITEM_BEGIN                 = 24000, 
    COOLDOWN_ITEM_END                   = 29999, 
};
#endif