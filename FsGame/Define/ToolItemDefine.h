//--------------------------------------------------------------------
// �ļ���:      ToolItemDefine.h
// ��  ��:      ToolItemModule��ض���
// ˵  ��:		
// ��������:    2014��12��11��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef FSGAME_DEFINE_TOOL_ITEM_DEFINE_H
#define FSGAME_DEFINE_TOOL_ITEM_DEFINE_H

// ���߰�״̬
enum EmItemBindState
{
	EM_ITEM_BIND_NONE,		// δ��
	EM_ITEM_BIND_LOCK,		// ��
};

// ״�߽��ױ�־
enum EmItemTradeFlag
{
	EM_ITEM_TRADE_NONE,		// ���ɽ���
	EM_ITEM_TRADE_ABLE,		// �ɽ���
};

// �Ƿ���ϣ����Ʒ
enum EmItemRareFlag
{
	EM_ITEM_RARE_NO,		// ����ϧ��Ʒ
	EM_ITEM_RARE_YES,		// ��ϧ��Ʒ
};

// ���ߵ�ʹ��Ч��
enum ItemEffectType
{
	ITEM_EFFECT_NONE	= -1,
	ITEM_EFFECT_GIFT,		// ʹ�ú����������

	MAX_ITEM_EFFECT_NUM
};

// ��Ʒ����ʹ�� ������
const int USE_ITEM_MAX_COUNT = 50;

// ��ƷЧ������
struct ItemEffectParam
{
	int effect_type;            // Ч������
	std::string effect_value;   // Ч��ֵ
	std::string ext_param01;    // ��չ����1
	std::string ext_param02;    // ��չ����2
	ItemEffectParam():effect_type(0),
		effect_value(""),
		ext_param01(""),
		ext_param02("")
	{

	}
};

#include "utils/util_config.h"
// ��Ʒ�󶨽�һ������
DECLARE_CONFIG_OBJECT_BEGIN_T(CfgItemBindLimit, std::string)

#define CONFIG_CLASS_NAME CfgItemBindLimit
#define PROPLIST      \
	DECLARE_CONFIG_PROP_FIELD(ID, "����id", string)	\
	DECLARE_CONFIG_PROP_FIELD_EX(Color, "��Ϳɰ�Ʒ��", 0, int32)

#include "utils/util_config_template.h"
DECLARE_CONFIG_OBJECT_END()

// ��Ʒ�󶨽�һ������
DECLARE_CONFIG_OBJECT_BEGIN_T(CfgPatchItem, std::string)

#define CONFIG_CLASS_NAME CfgPatchItem
#define PROPLIST      \
	DECLARE_CONFIG_PROP_FIELD(ID, "orig item id", string)	\
	DECLARE_CONFIG_PROP_FIELD(NewItem, "replace new item id", string) \
	DECLARE_CONFIG_PROP_FIELD_EX(Color, "item color", 0, int32 )

#include "utils/util_config_template.h"
DECLARE_CONFIG_OBJECT_END()
#endif // 