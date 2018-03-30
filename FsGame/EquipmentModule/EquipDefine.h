#ifndef EquipDefine_h__
#define EquipDefine_h__


// һ��װ���Ƿ����װ���ķ��ؽ��
typedef enum
{
    EQUIPCHECK_RESULT_SUCCEED = 1,              //�ɹ�

    //����Ϊ���ʧ��
    EQUIPCHECK_RESULT_ERR_NOTEQUIPITEM = 11,    //����Ʒ����һ��װ��
    EQUIPCHECK_RESULT_ERR_NOEQUIPTYPE = 12,     //û�ж����װ������
    EQUIPCHECK_RESULT_ERR_POSITION = 20,        //����Ʒ��װ��λ�ò�ƥ��
    EQUIPCHECK_RESULT_ERR_SEX = 21,				//����Ʒ���Ա�����������ƥ��
    EQUIPCHECK_RESULT_ERR_LEVEL = 22,           //����Ʒ�ļ�������������ƥ��
    EQUIPCHECK_RESULT_ERR_JOB = 23,             //����Ʒ��ְҵ����������ƥ��

} EQUIPCHECK_RESULT;

enum EQUIP_POS
{
	EQUIP_POS_STR_MIN	= 1,	// ��Ҫ�����װ������Сλ��(������ͨװ��������ʱװ)

	EQUIP_POS_WEAPON	= 1,	// ����
	EQUIP_POS_CLOTH		= 2,	// �·�
	EQUIP_POS_HELMET	= 3,	// ͷ��
	EQUIP_POS_NECKLACE	= 4,	// ����
	EQUIP_POS_RING		= 5,	// ��ָ
	EQUIP_POS_CHARM		= 6,	// ����
    
	EQUIP_POS_STR_MAX	= 6,	// ��Ҫ�����װ�������λ��
	//end
	EQUIP_MAX_PLACEHOLDER,
};

#define EQUIP_STRHENTHEN_REC	 "EquStrengthenRec"
enum
{
	EQUIP_STRHENTHEN_INDEX	= 0,	//װ����������,byte,(1-10)
	EQUIP_STRHENTHEN_LEVEL	= 1,	//����ǿ���ȼ�,int
	EQUIP_STRHENTHEN_SOUL	= 2,	//�������,string(id)
	EQUIP_STRHENTHEN_SOULDEVOUR	= 3,//������������������,int
	EQUIP_STRHENTHEN_JEWEL1	= 4,	//���ӱ�ʯ,string(id1)
	EQUIP_STRHENTHEN_JEWEL2	= 5,	//���ӱ�ʯ,string(id2)
	EQUIP_STRHENTHEN_JEWEL3	= 6,	//���ӱ�ʯ,string(id3)
	EQUIP_STRHENTHEN_JEWEL4	= 7,	//���ӱ�ʯ,string(id4)
};

#define EQUIP_BATTLE_ABILITY_REC "EquBattleAbilityRec"  // װ��ս������

// ս��������ж���
enum BattleAbilityRecCol
{
    EQUIP_BA_MIN_COL        = 0,
    EQUIP_BA_EQUIP_POS	    = 0,	//װ����������,byte,(1-10)
    EQUIP_BA_EQUIPMENT      = 1,	//��װ��ս����,int
    EQUIP_BA_STRHENTHEN     = 2,	//ǿ��ս����,int
    EQUIP_BA_JEWEL1	        = 3,	//��ʯ1ս����, int
    EQUIP_BA_JEWEL2	        = 4,	//��ʯ2ս����, int
    EQUIP_BA_JEWEL3	        = 5,	//��ʯ3ս����, int
    EQUIP_BA_JEWEL4	        = 6,	//��ʯ4ս����, int
    EQUIP_BA_SOUL           = 7,    // ���ս����
	EQUIP_BA_SOUL_CLOTH     = 8,	// �����װս����, �����װ��¼��������һ��
	EQUIP_BA_BAPTISE        = 9,    // ϴ��ս����
	EQUIP_BA_FASHION        = 10,   // ��Ӧ��λ��ʱװ��ս����
    EQUIP_BA_MAX_COL        = 10,
    
};

enum
{
	EQUIP_STRPROP_OPTION_ADD	= 1,//���
	EQUIP_STRPROP_OPTION_REMOVE	= 2,//ɾ��	
	EQUIP_STRPROP_OPTION_UPDATE	= 3,//����
};

#endif // EquipDefine_h__
