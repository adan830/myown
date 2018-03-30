//----------------------------------------------------------
// �ļ���:      ModifyPackDefine.h
// ��  ��:      ����������ģ�鹫��ͷ�ļ�
// ˵  ��:
// ������:      hehui
// ��������:    2012��4��5��
//    :       
//----------------------------------------------------------
#ifndef _FSGAME_MODIFY_PACK_DEFINE_H_
#define _FSGAME_MODIFY_PACK_DEFINE_H_

#include <string>
#include <map>

// ����������������
const char* const CONST_PROP_MODIFY_REC     = "PropModifyRec";			 // buff����ʱ����������(����Ҫ����ս��)

// ������ɫ�������ݱ�ṹ
enum PropModifyRec
{
	PMPS_PROP_NAME = 0,
	PMPS_PROP_VALUE,
	PMPS_PROP_SOURCE,
};

// ��װ������������
const char* const CONST_PROP_MODIFY_BYEQUIP_REC = "PropModifyByEquip";	 // ����������(�������ܡ�������װ����)

// ��װ������������ṹ
enum EnumPropModifyByEquipRec
{
    EPME_PROP_NAME,
    EPME_PROP_VALUE,
    EPME_FROM,
};

// �Ա���������������������
// const char* const PSKILL_PROP_MODIFY_REC     = "PropModifyByPSkill";
// 
// // ��������������ɫ�������ݱ�ṹ
// enum PropModifyByPSkillRec
// {
// 	PMPS_PROP_NAME = 0,
// 	PMPS_PROP_VALUE,
// 	PMPS_PROP_SOURCE,
// };

// ��װ����������������
enum EnumSpecialPropModifyByEquipRec
{
	EPME_SPECIAL_PROP_NAME,
	EPME_SPECIAL_PROP_VALUE,
	EPME_SPECIAL_FROM,
};

enum EModifyPackComputeType//nComputeType�����Ͷ���
{
    EModify_ADD_VALUE	= 0,	//����ֵ
    EModify_ADD_RATE	= 1,	//���Ӱٷֱ�
};

enum ERefreshDataType
{
    EREFRESH_DATA_TYPE_ADD,
    EREFRESH_DATA_TYPE_REMOVE,
};

typedef std::map<std::string, double> MapPropValue;

//����ɫ�����԰�ID���ڻ������԰�ID����ɫ�ȼ����������
const int ITEM_BASE_PACKAGE_COEFFICIENT = 1000; 

#endif // _FSGAME_MODIFY_PACK_DEFINE_H_
