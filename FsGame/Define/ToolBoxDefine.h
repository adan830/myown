//---------------------------------------------------------
//�ļ���:       ToolBoxDefine.h
//��  ��:       
//˵  ��:       
//          
//��������:      2014��11��22��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------
#ifndef _FSGAME_TOOL_BOX_DEFINE_H_
#define _FSGAME_TOOL_BOX_DEFINE_H_

#include <string>

enum EmToolBoxResult
{
	EM_TOOL_BOX_RESULT_SUCCEED,		// �ɹ�
	EM_TOOL_BOX_RESULT_NO_SPACE,	// �����ռ䲻��

	EM_TOOL_BOX_RESULT_ERROR,		// ������δ֪��
};

// �ͻ���ͨ�ŵ�����Ϣ����
enum
{
    TOOL_BOX_SUBMSG_SELL     = 1,   // ����
    TOOL_BOX_SUBMSG_UNLOCK   = 2,   // ����
    TOOL_BOX_SUBMSG_ARRANGE  = 3,   // ����
    TOOL_BOX_SUBMSG_EXPAND   = 4,   // ��������
	TOOL_BOX_SUBMSG_MOVEITEM = 5,   // �ƶ�������Ʒ(��װ����)
    TOOL_BOX_SUBMSG_USE_ITEM = 6,   // ʹ����Ʒ
    TOOL_BOX_SUBMSG_USE_ITEM_ON_ITEM = 7,   // ����Ʒʹ����Ʒ(����Ѷ�ҩ����ʹ�õ�������)
    TOOL_BOX_SUBMSG_USE_ITEM_MERGE = 8,     // ��Ʒ�ϳɣ�����Ѷ�����ߺϳ�һ������
	TOOL_BOX_SUBMSG_PICKUP_TEMPORAR_BOX,	// ����ʱ���������Ʒ ����: ���������б�(VarList[int] 1, 2, 3, 4...) ���������ز���: result[EmToolBoxResult] �ɹ���[int]
};

// ��ȡװ������
enum
{
	TOOL_BOX_SUBMSG_SHOW	 = 1,	// չʾ�»����Ʒ int ���� {string configid, string UniqueID, int ��Ʒ����}
	TOOL_BOX_SUBMSG_NEW_USE	 = 2,	// �»����Ʒ��ʾʹ�� string configid, string UniqueID, int ��Ʒ����
};

// ��Ϣ���Ͳ�������Ʒ��������
const int TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM = 3;
// ����չʾ����Ʒ����
enum ShowItem
{
	SHOW_ITEM_CONFIG_ID = 0,
	SHOW_ITEM_UNIQUE_ID,
	SHOW_ITEM_AMOUNT,
};

// ������֮��ͨ�ŵ�����Ϣ����
enum
{
    TOOL_BOX_SUBCOMMAND_USE_ITEM = 1,         // ʹ�õ���
    TOOL_BOX_SUBCOMMAND_USE_ITEM_ON_ITEM = 2, // �Ե���ʹ�õ���
};


#define TEMPORARY_BOX_DEFAULT_CAPACITY		256	// Ĭ����ʱ������С

#endif // _FSGAME_TOOL_BOX_DEFINE_H_
