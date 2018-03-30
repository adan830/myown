//---------------------------------------------------------
//�ļ���:       SnsDataDefine.h
//��  ��:       
//˵  ��:       
//          
//��������:      2014��12��25��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------
#ifndef FSGAME_SNS_DATA_DEFINE_H_
#define FSGAME_SNS_DATA_DEFINE_H_

#include <string>

#define SNS_DATA_SECTION_ATTR                 "PlayerAttribute"
#define SNS_DATA_SECTION_EQUIPMENT            "PlayerEquipment"
#define SNS_DATA_SECTION_SKILL                "PlayerSkill"
#define SNS_DATA_SECTION_PLAYER_PET           "PlayerPet"
#define SNS_DATA_SECTION_PLAYER_RIDE          "PlayerRide"
#define SNS_DATA_SECTION_EQUIPMENT_BAPTISE    "PlayerEquipmentBaptise"
#define SNS_DATA_SECTION_WING_EQUIPMENT       "WingEquipment"

// ����/����SNS���ݵ�ʱ���ʹ�õı�ʶ
// 31 �����ѡ���һ������
enum
{
    SNS_DATA_DO_NOTHING         = 31,  // ɶ����������ϢID
    CREATE_SNS_DATA_DEFAULT     = 32,  // ����Ĭ�ϵ�SNS����
    LOAD_SNS_DATA_DEFAULT       = 33,  // ����Ĭ�ϵ�SNS����
    CREATE_SNS_DATA_OFFLINE     = 34,  // �������ߵ�SNS����
    LOAD_SNS_DATA_OFFLINE       = 35,  // �������ߵ�SNS����
    CREATE_SNS_DATA_MSGPUSH     = 36,  // ����MsgPush��SNS����
    LOAD_SNS_DATA_MSGPUSH       = 37,  // ����MsgPush��SNS����
	CREATE_SNS_DATA_RED_PACKET  = 38,  // ����RedPacket��SNS����
	LOAD_SNS_DATA_RED_PACKET    = 39,  // ����RedPacket��SNS����
	SNS_DATA_CREATE_PLAYER      = 40,  // ����SNS����
	SNS_DATA_QUERY_PLAYER       = 41,  // ��ѯ��ҵ�SNS����
};

// �ͻ���ͨ�ŵ�����Ϣ����
enum
{
    SNS_DATA_SUBMSG_QUERY_OFFLINE_DATA     = 1,   // ��ѯ��������
};

// ������֮��ͨ�ŵ�����Ϣ����
enum
{
    SNS_DATA_SAVE_PLAYER_DATA       = 1,   // ������������
    SNS_DATA_QUERY_PLAYER_DATA      = 2,   // ��ѯ��������
    SNS_DATA_SHOW_ME_YOUR_DATA      = 3,   // ������ѯ���Լ�������
    SNS_DATA_HERE_IS_DATA_YOU_WANT  = 4,   // �յ�Ŀ���û�������
    SNS_DATA_MODIFY_PLAYER_DATA     = 5,   // �޸��������
    SNS_DATA_CREATE_PLAYER_ENTRY    = 6,   // ��SNS�ϴ��������Ŀ
};

// ��ʶ��˭������SNS���ݵ�
enum
{
    SNS_DATA_REQUEST_FROM_NA                = 1,
    SNS_DATA_REQUEST_FORM_RANK_LIST         = 2,    //��������ʹ��
	SNS_DATA_REQUEST_FORM_CHAT              = 3,
	SNS_DATA_REQUEST_FORM_FRIEND_UPDATE     = 4,
	SNS_DATA_REQUEST_FORM_FRIEND_ENEMY      = 5,
	SNS_DATA_REQUEST_FORM_FRIEND_FIND       = 6,
	SNS_DATA_REQUEST_FORM_NATION_OFFICIAL   = 7,
    SNS_DATA_REQUEST_FORM_ADD_FRIEND_APPLY  = 8,
	SNS_DATA_REQUEST_FORM_RANK_LIST_PLAYER  = 9,    //���а�-��һ�����Ϣ(���ԡ�װ����ϴ��)
	SNS_DATA_REQUEST_FORM_RANK_LIST_PET     = 10,   //���а�-��ҳ�����Ϣ
	SNS_DATA_REQUEST_FORM_RANK_LIST_RIDE    = 11,   //���а�-���������Ϣ
	SNS_DATA_REQUEST_FORM_RANK_LIST_WING    = 12,   //���а�-��ҳ����Ϣ
	SNS_DATA_REQUEST_FORM_FRIEND_BLACK		= 13,		// ���º�����
	SNS_DATA_REQUEST_FORM_MASTERAPRTC_UPDATE = 14,	 //ʦͽ-����
	SNS_DATA_REQUEST_FORM_ADD_MASTERAPRTC_APPLY = 15,	 //ʦͽ-��������
	SNS_DATA_REQUEST_FORM_TEAM_PLAYER_LIST	= 16, // ���-������Ϣ
};

#endif // FSGAME_SNS_DATA_DEFINE_H_