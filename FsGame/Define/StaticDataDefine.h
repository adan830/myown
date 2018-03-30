//--------------------------------------------------------------------
// �ļ���:      StaticDataDefine.h
// ��  ��:      ��̬�������Ͷ���
// ˵  ��:      ���ļ��޸���ͬ���޸� �ͻ���lua�ļ�Client\lua\share\static_data_type.lua
// ��������:    
// ������:      
//    :       
//--------------------------------------------------------------------
#ifndef _StaticDataDefine_H
#define _StaticDataDefine_H

enum eStaticDataType
{
    // ��ɫ�������
	STATIC_DATA_LEVELUP_ADDPROP = 1,			/**< �ȼ��ɳ����� */
	STATIC_DATA_NPC_BASE_PACK,					/**< npc����ʵ�����ð� */
	STATIC_DATA_BUFF_BASE_PACK,					/**< BUFF���԰� */
	STATIC_DATA_EQUIP_ADD_PACK,					/**< װ���������԰� */
	STATIC_DATA_EQUIP_RANDINTERVAL_PACK,		/**< װ���������������� */
	STATIC_DATA_EQUIP_MODIFYADD_PACK,			/**< װ���������������� */
	STATIC_DATA_EQUIP_BASE_PACK,				/**< װ���������԰� */
	STATIC_DATA_JEWEL_BASE_PACK,				/**< ��ʯ���԰� */
	STATIC_DATA_STRENGTHEN_PACK,				/**< ǿ�����԰� */
	STATIC_DATA_STRENGTHEN_SUIT_PACK,			/**< ǿ����װ���԰� */
	STATIC_DATA_RIDE_ATTRIBUTE_PACK,			/**< �������԰� */
	STATIC_DATA_WING_ATTRIBUTE_PACK,			/**< ������԰� */
	STATIC_DATA_JADE_ATTRIBUTE_PACK,			/**< �������԰� */
	STATIC_DATA_APPERANCE_PACK,					/**< ��۴������԰� */

	//��������100-199

    STATIC_DATA_MAX,
};

// ȫ�ֳ�������
const char* const GLOBAL_CONST_SEC = "ConstVal";

#endif // _StaticDataDefine_H
