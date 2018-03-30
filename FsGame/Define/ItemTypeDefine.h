//--------------------------------------------------------------------
// �ļ���:      ItemTypeDefine.h
// ��  ��:      ��Ʒ���ö���
// ˵  ��:		
// ��������:    2014��10��24��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef ItemTypeDefine_h__
#define ItemTypeDefine_h__

// Ʒ������
enum
{
	COLOR_MIN = -1,

	COLOR_ZERO = 0,
	COLOR_ONE,
	COLOR_TWO,
	COLOR_THREE,
	COLOR_FOUR,
	COLOR_FIVE,

	COLOR_MAX,
};

//��Ʒ����
enum ItemType
{
	//��Ʒ���Ͷ���
	//==========================================
	//1-99������
	ITEMTYPE_NONE = 0,
	//==========================================
	//1-99ʹ�������
	//����Ϊ���ߵľ������
	ITEMTYPE_TOOLITEM_MIN                   = 1,  //ʹ������ߵ���С���
	ITEMTYPE_ADD_PHYPOWER                   = 1,  //������ҩ
	ITEMTYPE_ADD_STAYPOWER                  = 2,  //������ҩ
	//��ħ����(װ�����Ե�����)
	ITEMTYPE_APPEND_PRPERTY                 = 3,  //����װ������
	ITEMTYPE_ADD_TASK                       = 4,  //��������
	ITEMTYPE_CAPITAL                        = 5,  //���ӽ�Ǯ�ĵ���
	ITEMTYPE_COMPOSITE_FORMULA              = 6,  //�ϳ��䷽
	ITEMTYPE_CARRY_MAILBOX                  = 7,  //�������书��
	ITEMTYPE_TASKTOOL                       = 8,  //����ʹ�õ��������
	ITEMTYPE_CALL                           = 9,  //�ٻ�npc����
	ITEMTYPE_ADDBUFFER                      = 10, //�� �Լ���������ң�npc ���buffer
	ITEMTYPE_GOHOME                         = 11, //�س� ��Ʒ
	ITEMTYPE_INLAY                          = 12, //��Ƕ����
	ITEMTYPE_INLAY1                         = 13, //1�౦ʯ��Ƕ
	ITEMTYPE_INLAY2                         = 14, //2��ʯ��Ƕ
	ITEMTYPE_INLAY3                         = 15, //3��ʯ��Ƕ
	ITEMTYPE_INLAY4                         = 16, //4��ʯ��Ƕ
	ITEMTYPE_EVALUATE                       = 17, //��������
	ITEMTYPE_SPLIT_TOOL                     = 18, //��ⱦʯ����
	ITEMTYPE_STRENGTHEN_STONE               = 19, //ǿ��ʯ
	ITEMTYPE_APPEND_PAPER                   = 20, //��ħͼֽ
	ITEMTYPE_ITEM_PACKAGE                   = 21, //��Ʒ���������id����
	ITEMTYPE_ITEM_PATCH						= 22, //��Ƭ����ߣ�n����Ƭ�ϳ�һ����Ʒ��
	ITEMTYPE_ITEM_GIFT						= 23, //��Ʒ��������Ʒid����
	ITEMTYPE_ITEM_TREASURE_PATCH            = 24, //�ᱦ��Ƭ
	ITEMTYPE_ITEM_PATERNOSTER               = 25, //����
	ITEMTYPE_ITEM_INTRODUCE                 = 26, //˵���Ե��ߣ�û���κ����壩
	ITEMTYPE_ITEM_ADD_PROPERTY				= 27, //�����Ե���
    ITEMTYPE_ITEM_CREATE_NPC				= 28, //����һ��NPC
    ITEMTYPE_ADD_PETEXP                     = 30, //���ӳ��ﾭ��
    ITEMTYPE_ITEM_PETSTONE                  = 31, //���ʯ
	ITEMTYPE_MONTH_CARD                     = 32, //�¿�����
	ITEMTYPE_TOOLITEM_MAX                   = 99, //ע��:ʹ�������������ֱ��
	//==========================================
	//���߷������
	//==========================================

	//==========================================
	//100-199װ����
	ITEMTYPE_EQUIP_MIN                      = 100,
	ITEMTYPE_EQUIP_MAX                      = 199,
	ITEMTYPE_WEAPON_MIN                     = 100,
	ITEMTYPE_WEAPON_MAX                     = 129,
	//==========================================
	//����Ϊװ���ľ������
	//==========================================
	//����100~129
	ITEMTYPE_WEAPON_SWORD                   = 101, //��
	ITEMTYPE_WEAPON_MACE                    = 103, //��
	ITEMTYPE_WEAPON_SPEAR                   = 106, //��������
	ITEMTYPE_WEAPON_DOUBLE_SWORD            = 102, //˫�ֽ�
	ITEMTYPE_WEAPON_GUN                     = 115, //ǹе
	ITEMTYPE_WEAPON_WHIP                    = 129, //����
	//����130~199
	ITEMTYPE_EQUIP_BELT                     = 139, //����
	ITEMTYPE_EQUIP_HAT                      = 140, //ͷ��
	ITEMTYPE_EQUIP_CLOTH                    = 141, //����
	ITEMTYPE_EQUIP_PANTS                    = 142, //����
	ITEMTYPE_EQUIP_SHONES                   = 143, //Ь��
	ITEMTYPE_EQUIP_HAND                     = 146, //����
	ITEMTYPE_EQUIP_NECK                     = 147, //����
	ITEMTYPE_EQUIP_MANTLE                   = 148, //����
	ITEMTYPE_EQUIP_RING                     = 149, //��ָ
	ITEMTYPE_EQUIP_SHOULDERS                = 151, //���
	//==========================================
	//װ���������
	//==========================================

	//200-299���������

	//300-499�̳ǵ���
	//==========================================
	//��������1000+
	//==========================================
	//��������Ǯ�������
	ITEMTYPE_DROPMONEY                      = 1100,

	ITEMTYPE_AWARDMONEY                      = 1101,                             //������Ǯ

	ITEMTYPE_AWARDEXP                     = 1102,                                    //��������
	ITEMTYPE_AWARDHP                         = 1103,                                 //����HP

	// ����,ֻ�ܼ�ȡ�õ���ֻ�������̵�,û����������
	ITEMTYPE_ZAWU                           = 1200,
	//����Ϊ�ϳɲ��ϵľ������ 2010 ~ 2200
	//==========================================
	//��ʼ
	ITEMTYPE_COMPOSE_MATRIAL_MIN            = 2010, //��ʼ
	ITEMTYPE_SLOT_ITEM                      = 2011, //�����
	//��ħ����(װ�����Ե�����)
	ITEMTYPE_APPEND_ITEM                    = 2012, //��ħ����
	ITEMTYPE_FUEL                           = 2013, //������ʱ����
	//����
	ITEMTYPE_COMPOSE_MATRIAL_MAX            = 2200, //����

	ITEMTYPE_MARKER_MIN = 3000,				// ӡ����Ʒ������ʼ
	ITEMTYPE_MARKER_ITEM = 3000,			// ӡ����Ʒ
	ITEMTYPE_MARKER_MAX = 3100,				// ӡ����Ʒ������ʼ

	ITEMTYPE_MAX							= 9999,
};

// ��Ʒϡ�г̶ȶ���
enum SpecialType
{
	SPECIAL_TYPE_NORMAL		= 0,		// ��ͨ
	SPECIAL_TYPE_VALUABLE	= 1,		// ����
	SPECIAL_TYPE_SPECIAL	= 2,		// ϡ��
};
// ��Ʒ��Դ�ַ�����
const char* const FROM_SHOP_TEMP			= "shop00";	// �̵�ģ��
const char* const FROM_SHOP_MALL			= "shop005";	// �̳�
const char* const FROM_MELTING				= "Melting001";	// ����

#endif // ItemTypeDefine_h__
