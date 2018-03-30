//---------------------------------------------------------
//�ļ���:       LookDefine.h
//��  ��:       ��Ϣ�鿴����
//˵  ��:       
//��������:   
//������:                
//---------------------------------------------------------
#ifndef _FSGAME_LookDefine_H_
#define _FSGAME_LookDefine_H_

#include <string>

#define SHOWOFF_ITEM_PUB_RECORD_NAME     "ShowoffItemPubRec"
#define SHOWOFF_ITEM_PUB_RECORD_MAX_ROW  500

#define SHOWOFF_ITEM_PUB_RECORD_COL_COUNT        4
#define SHOWOFF_ITEM_PUB_RECORD_ROLE_UID_COL     0
#define SHOWOFF_ITEM_PUB_RECORD_ROLE_NAME_COL    1
#define SHOWOFF_ITEM_PUB_RECORD_UID_COL          2
#define SHOWOFF_ITEM_PUB_RECORD_PROP_COL         3

// ����Ϣ����
enum
{
    SHOWOFF_PUB_SAVE_ITEM   = 1, // ֪ͨpub������,������ҫ����Ʒ��Ϣ
    SHOWOFF_PUB_REMOVE_ITEM = 2,
    SHOWOFF_PUB_CLEAR_ALL   = 3,
};


#endif
