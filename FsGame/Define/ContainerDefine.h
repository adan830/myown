//---------------------------------------------------------
//�ļ���:       ContainerDefine.h
//��  ��:       ������һЩ����
//˵  ��:       
//          
//��������:      2014��10��31��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------
#ifndef _FSGAME_CONTAINER_DEFINE_H_
#define _FSGAME_CONTAINER_DEFINE_H_
#include <vector>
#include <map>

#define CONTAINER_START_POS 1     // ����λ�õ���ʼλ��

#define TEXT_COLOR_LEVEL       "ColorLevel"
#define TEXT_UNIQUE_ID         "UniqueID"
#define TEXT_AMOUNT            "Amount"
#define TEXT_MAX_AMOUNT        "MaxAmount"
#define TEXT_ITEM_TYPE         "ItemType"
#define TEXT_LOCKED_SIZE       "LockedSize" // ����������������
#define TEXT_CAN_OP            "CanOP"      // �Ƿ���Բ���
#define TEXT_CAN_ADD           "CanAdd"
#define TEXT_CAN_REMOVE        "CanRemove"
#define TEXT_CAN_ARRANGE       "CanArrange" //�Ƿ��������
// #define TOOL_BOX_NAME          "ToolBox"
// #define TOOL_BOX_NAME_WSTR     L"ToolBox"

#define ITEM_BOX_NAME_WSTR      L"ItemBox"      //�������������
// #define POWER_BOX_NAME_WSTR     L"PowerBox"     //ʵ�����������
// #define BADGE_BOX_NAME_WSTR     L"BadgeBox"     //��������
// #define CHIP_BOX_NAME_WSTR      L"ChipBox"      //��Ƭ����
#define ITEM_BOX_NAME           "ItemBox"      //�����������������
// #define POWER_BOX_NAME          "PowerBox"     //ʵ���������������
// #define BADGE_BOX_NAME          "BadgeBox"     //������������
// #define CHIP_BOX_NAME           "ChipBox"      //��Ƭ��������

#define EQUIP_BOX_NAME  "EquipBox"
#define EQUIP_BOX_NAME_WSTR  L"EquipBox"

// ӡ������
#define MARKER_BOX_NAME	"MarkerBox"
#define MARKER_BOX_NAME_WSTR	L"MarkerBox"

// ��ʱ����
#define   TEMPORARY_BAG_BOX_NAME_WSTR	L"TemporaryBagBox"


#define TEXT_ITEM_BIND_STATUS   "BindStatus"  // ��Ʒ������
// ��״̬
enum 
{
	ITEM_NOT_BOUND	= 0, // �ǰ�
	ITEM_BOUND		= 1, // ��
};

// ��Ʒƥ��״̬
enum 
{
	ITEM_MATCH_BOUND = 0x01 << 0,		// ƥ�����Ʒ
	ITEM_MATCH_NOT_BOUND = 0x01 << 1,	// ƥ��ǰ���Ʒ 

	ITEM_MATCH_FROZEN = 0x01 << 2,		// ƥ�����ж������Ʒ
	ITEM_MATCH_NOT_FROZEN = 0x01 << 3,	// �������зǶ�����Ʒ

	ITEM_MATCH_BOUND_AND_FROZEN = 0x01 << 4, // ƥ��󶨲������
	ITEM_MATCH_BOUND_NOT_FROZEN = 0x01 << 5, // ƥ��󶨲��������

	ITEM_MATCH_NOT_BOUND_AND_FROZEN = 0x01 << 6, // ƥ��ǰ󶨲������
	ITEM_MATCH_NOT_BOUND_NOT_FROZEN = 0x01 << 7, // ƥ��ǰ󶨲��������

	ITEM_MATCH_ALL = ITEM_MATCH_BOUND | ITEM_MATCH_NOT_BOUND | ITEM_MATCH_FROZEN | ITEM_MATCH_NOT_FROZEN
					| ITEM_MATCH_BOUND_AND_FROZEN | ITEM_MATCH_BOUND_NOT_FROZEN 
					| ITEM_MATCH_NOT_BOUND_AND_FROZEN | ITEM_MATCH_NOT_BOUND_NOT_FROZEN,		// ������Ʒ
};

#define ITEM_TYPE_GIFT          361 // ���������

// ����������Ϣ����
enum
{
    CONTAINER_SUBMSG_SALE = 1,		// ����
    CONTAINER_SUBMSG_UNLOCK = 2,	// ����
    CONTAINER_SUBMSG_ARRANGE = 3,	// ����
    CONTAINER_SUBMSG_EXPAND = 4,	// ��������
	CONTAINER_SUBMSG_MOVEITEM = 5,	// �ƶ�������Ʒ(��װ����)
};

//�ͻ��˲�������������
enum
{
    CONTAINER_OPTYPE_ALL,   //����Ȩ��
    CONTAINER_OPTYPE_ADD,   //�����Ʒ
    CONTAINER_OPTYPE_REMOVE,//������Ʒ
};

// �򵥵���Ʒ
struct SBoxTestItem
{
    std::string config_id;
    int amount;
    int bind_status;		// ��״̬
	PERSISTID item;			// ��Ʒ���������PERSISTID
	int64_t frozenTime;		// �������ʱ��

	SBoxTestItem() : config_id(""), amount(0), bind_status(ITEM_BOUND), frozenTime(0){}
};
typedef std::vector<SBoxTestItem> VectorBoxTestItem;
typedef std::map<int64_t, VectorBoxTestItem> BoxItemData;	// <����������Ʒ�б�>

//����������
enum
{
    CONTAINER_ARRANGE_SUCCESS = 0,   //����ɹ�
    CONTAINER_ARRANGE_FAIL = 1,      //����ʧ�ܣ�Ҳ��������������
};


#endif // _FSGAME_CONTAINER_DEFINE_H_
