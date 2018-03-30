//--------------------------------------------------------------------
// �ļ���:		GuildDefine.h
// ��  ��:		����ϵͳ-������Ͷ���
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __GuildDefine_H__
#define __GuildDefine_H__

#include <vector>
#include <string>
#include <set>
#include <map>
#include <functional>
#include "utils/util_config.h"
// ��������ַ�������
#define GUILD_CUSTOM_STRING_1    "ui_form_guild_1"   // �����˻���ȴʱ�䣬�޷�������ߴ������
#define GUILD_CUSTOM_STRING_2    "ui_form_guild_2"   // ��������Ѵ�����
#define GUILD_CUSTOM_STRING_3    "ui_form_guild_3"   // �˹���30������
#define GUILD_CUSTOM_STRING_4    "ui_form_guild_4"   // ����Ѽ�����
#define GUILD_CUSTOM_STRING_5    "ui_form_guild_5"   // ��������ظ����޷�����
#define GUILD_CUSTOM_STRING_6    "ui_form_guild_6"   // ������ư���������Ż�Ƿ��ַ�
#define GUILD_CUSTOM_STRING_7    "ui_form_guild_7"   // Ԫ������
#define GUILD_CUSTOM_STRING_8    "ui_form_guild_8"   // ��������������Ѵ�����
#define GUILD_CUSTOM_STRING_9    "ui_form_guild_9"   // �Ѿ�������ð��
#define GUILD_CUSTOM_STRING_10   "ui_form_guild_10"  // �ð�������б�����
#define GUILD_CUSTOM_STRING_11   "ui_form_guild_11"  // ��������Ƿ��ַ�
#define GUILD_CUSTOM_STRING_12   "ui_form_guild_12"  // �������Ϊ��
#define GUILD_CUSTOM_STRING_13   "ui_form_guild_13"  // ��Ҳ�����
#define GUILD_CUSTOM_STRING_14   "ui_form_guild_14"  // ����Ȩ�޲���
#define GUILD_CUSTOM_STRING_15   "ui_form_guild_15"  // �ð�������Ѵ�����
#define GUILD_CUSTOM_STRING_16   "ui_form_guild_16"  // ������������
#define GUILD_CUSTOM_STRING_17   "ui_form_guild_17"  // �����ռ䲻��
#define GUILD_CUSTOM_STRING_18   "ui_form_guild_18"  // ����ͬһ������
#define GUILD_CUSTOM_STRING_19   "ui_form_guild_19"  // ��᲻����
#define GUILD_CUSTOM_STRING_20   "ui_form_guild_20"  // ͭ�Ҳ���
#define GUILD_CUSTOM_STRING_21   "ui_form_guild_21"  // ��ṱ��ֵ����
#define GUILD_CUSTOM_STRING_22   "ui_form_guild_22"  // �����Ѿ�����
#define GUILD_CUSTOM_STRING_23   "ui_form_guild_23"  // �̵���Ʒ�������㹺��
#define GUILD_CUSTOM_STRING_24   "ui_form_guild_24"  // ��ְλ�����Ѵ����ޣ�ְλ�䶯ʧ��
#define GUILD_CUSTOM_STRING_25   "ui_form_guild_25"  // xxx�����˰��
#define GUILD_CUSTOM_STRING_26   "ui_form_guild_26"  // ����ʧ�ܣ����������ѹ���
#define GUILD_CUSTOM_STRING_27   "ui_form_guild_27"  // ���׳ɹ�
#define GUILD_CUSTOM_STRING_28   "ui_form_guild_28"  // ����ʧ��
#define GUILD_CUSTOM_STRING_29   "ui_form_guild_29"  // ��������ַ�����
#define GUILD_CUSTOM_STRING_30   "ui_form_guild_30"  // ����ְλ��Ȩ��������
#define GUILD_CUSTOM_STRING_31   "ui_form_guild_31"  // ��Ȩ����
#define GUILD_CUSTOM_STRING_32   "ui_form_guild_32"  // ��Ȩ����
#define GUILD_CUSTOM_STRING_33   "ui_form_guild_33"  // ��Ȩ����
#define GUILD_CUSTOM_STRING_34   "ui_form_guild_34"  // �����ڼ� ������Ա������䶯
#define GUILD_CUSTOM_STRING_35   "ui_form_guild_35"  // �޸Ĺ���ɹ�
#define GUILD_CUSTOM_STRING_36   "ui_form_guild_36"  // �޸Ĺ���ʧ��
#define GUILD_CUSTOM_STRING_37   "ui_form_guild_37"  // ���˹���,��������
#define GUILD_CUSTOM_STRING_38   "ui_form_guild_38"  // ���԰����Ƿ��ַ�
#define GUILD_CUSTOM_STRING_39   "ui_form_guild_39"  // �޸����Գɹ�
#define GUILD_CUSTOM_STRING_40   "ui_form_guild_40"  // �޸�����ʧ��
#define GUILD_CUSTOM_STRING_48   "ui_form_guild_48"  // �����в���ִ�иò���
#define GUILD_CUSTOM_STRING_63   "ui_form_guild_63"  // {0}�ֻ��ˣ��������ɻ�ö�����������Ҳ�Ҫ����ร�
#define GUILD_CUSTOM_STRING_64   "ui_form_guild_64"  // ��ϲ������ˣ�
#define GUILD_CUSTOM_STRING_65   "ui_form_guild_65"  // �����ˣ�
#define GUILD_CUSTOM_STRING_68   "ui_form_guild_68"  // ���ⳬʱ��
#define GUILD_CUSTOM_STRING_69	 "ui_form_guild_str_69"	 //���״����ù���
#define GUILD_CUSTOM_STRING_70	 "ui_form_guild_str_70"	  //������ʱ
#define GUILD_CUSTOM_STRING_71   "ui_form_guild_str_71"  // ��Ȩ�Զ�ͬ��
#define GUILD_CUSTOM_STRING_72	 "ui_form_guild_str_72"   //��Ȩ��������
#define GUILD_CUSTOM_STRING_73	 "ui_form_guild_str_73"	 // �����������蹫��ȼ�����
#define GUILD_CUSTOM_STRING_74	 "ui_form_guild_str_74"	 // ���ﹱ����
#define GUILD_CUSTOM_STRING_75	 "ui_form_guild_str_75"	 // �����輧���蹫��ȼ�����
#define GUILD_CUSTOM_STRING_76	 "ui_form_guild_str_76"	 // �輧��������
#define GUILD_CUSTOM_STRING_77	 "ui_form_guild_str_77"	 // �輧�ѿ�����
#define GUILD_CUSTOM_STRING_78	 "ui_form_guild_str_78"	 // �����輧����ʱ��
#define GUILD_CUSTOM_STRING_79	 "ui_form_guild_str_79"	 // �����ѽ���
#define GUILD_CUSTOM_STRING_80	 "ui_form_guild_str_80"	 // ��ǰ��֯û�иü����޷�����
#define GUILD_CUSTOM_STRING_81	 "ui_form_guild_str_81"	 // ��������ʧ�ܹ��ײ���
#define GUILD_CUSTOM_STRING_82	 "ui_form_guild_str_82"	 // ���˹��ײ���
#define GUILD_CUSTOM_STRING_83	 "ui_form_guild_str_83"	 // ��֯�����Ѿ��������ȼ��޷�����
#define GUILD_CUSTOM_STRING_84	 "ui_form_guild_str_84"	 // ��֯�����ѵ�����ҵȼ��޷�����
#define GUILD_CUSTOM_STRING_85	 "ui_form_guild_str_85"	 // {0}��֯�ܾ�������빫�������
#define GUILD_CUSTOM_STRING_86	 "ui_form_guild_str_86"	 // ����������ȴʱ��û��
#define GUILD_CUSTOM_STRING_87	 "ui_form_guild_str_87"	 // ��ǰ�������󲻴���
#define GUILD_CUSTOM_STRING_88	 "ui_form_guild_str_88"	 // ��ǰ������������
#define GUILD_CUSTOM_STRING_89	 "ui_form_guild_str_89"	 // �������߲�����
#define GUILD_CUSTOM_STRING_90	 "ui_form_guild_str_90"	 // ���ջ�����������
#define GUILD_CUSTOM_STRING_92	 "ui_form_guild_str_92"	 //  �̵�û�г��۵�ǰ��Ʒ��ˢ��
#define GUILD_CUSTOM_STRING_93	 "ui_form_guild_str_93"	 //  ��ǰ�޸��̵���Ʒ�б���򳬹�
#define GUILD_CUSTOM_STRING_94	 "ui_form_guild_str_94"	 //  ��ǰ�б�����Ʒ�ȼ���������֯�ȼ�
#define GUILD_CUSTOM_STRING_95	 "ui_form_guild_str_95"	 //  �����б���ɹ�
#define GUILD_CUSTOM_STRING_96	 "ui_form_guild_str_96"	 //  ��Ԫ����
#define GUILD_CUSTOM_STRING_97	 "ui_form_guild_str_97"	 //  ��Ȩ�����̵������Ʒ
#define GUILD_CUSTOM_STRING_98	 "ui_form_guild_str_98"	 //  �Ѿ���������ս����boss 
#define GUILD_CUSTOM_STRING_99	 "ui_form_guild_str_99"	 //  �ж�Ա���ڵ�ǰ����
#define GUILD_CUSTOM_STRING_100	 "ui_form_guild_str_100"  //  {0} ��֪��ǰ��֯�ĳ�Ա
#define GUILD_CUSTOM_STRING_101	 "ui_form_guild_str_101"  //  ��������֯��������
#define GUILD_CUSTOM_STRING_102	 "ui_form_guild_str_102"  //  ��֯�����Ѵ���
#define GUILD_CUSTOM_STRING_103	 "ui_form_guild_str_103"  //  ��֯�ʽ���
#define GUILD_CUSTOM_STRING_105	 "ui_form_guild_str_105"  //  �������������ȡ
#define GUILD_CUSTOM_STRING_106	 "ui_form_guild_str_106"  //  ������֯�ʽ��Ѵ�����
#define GUILD_CUSTOM_STRING_107	 "ui_form_guild_str_107"  //  �Ѿ��м������о�
#define GUILD_CUSTOM_STRING_108	 "ui_form_guild_str_108"  //  ���ܲ�����
#define GUILD_CUSTOM_STRING_109	 "ui_form_guild_str_109"  //  ��û�����Ȩ��
#define GUILD_CUSTOM_STRING_110	 "ui_form_guild_str_110"  //  �Ѵﵽ���ջ�ȡ�ĸ�����֯�ʽ�����
#define GUILD_CUSTOM_STRING_111	 "ui_form_guild_str_111"  //  ��Ҳ����߲����������֯
#define GUILD_CUSTOM_STRING_112	 "ui_form_guild_str_112"  //  ����ÿ�η��������





//������֯��
#define GUILD_INVITE_GUILD_NAME "InviteGuildName"
//��֯�ֺ��ʼ�
#define GUILD_BONUS_MAIL "guild_bonus_mail"

#define GUILD_CAPTAIN_MAIL "guild_captain_mail"

// ���ṫ������������
#define GUILD_PUB_DATA L"guild_pubdata"

// ���ṫ�����ݱ�(GUILD_PUB_DATA+serverId)
// �����б���
#define GUILD_LIST_REC "guild_list_rec"
// ���������
#define GUILD_SORT_REC "guild_sort_rec"
// �������б���
#define GUILD_APPLY_REC "guild_apply_rec"
// ��¼��������Ҽ��빫��ı���������������ʱ����������˹��ᣩ
#define GUILD_OUTLINE_JOIN_REC "guild_outline_join_rec"
// ��¼����������˳�����ı��
#define GUILD_OUTLINE_QUIT_REC "guild_outline_quit_rec"
// �����̵���߹������
#define GUILD_SHOP_BUY_REC "guild_shop_buy_rec"
// �����Ա����ĵ��߼���
#define GUILD_MEMBER_BUY_REC "guild_member_buy_rec"

//��֯����
#define GUILD_SYSMBOL_REC "guild_symbol_rec"

// ������ݰ汾��
#define  GUILD_DATA_VERSION_ID 1
//�����������Ʒ�ʼ�
#define GUILD_MUTUAL_GIVE_MAIL "guild_mutual_give_mail"
//�߳������ʼ�
#define MAIL_GUILD_FIRE "guild_fire"
// ��֯���������ȼ�
#define GUILD_MUTUAL_LIMIT_LEVEL 2
#define GUILD_MUTUAL_CD_TIMES	8*60*60
// ��᳡���������Դ��
#define GUILD_STATION_GROUP_REC "guild_station_group_rec"   
enum
{
	GUILD_STATION_GROUP_REC_COL_GUILD_NAME    = 0,        	   // ����� VTYPE_WIDESTR
	GUILD_STATION_GROUP_REC_COL_GROUPID,                       // ����ID   VTYPE_INT
	GUILD_STATION_GROUP_REC_COL_MAX,			
};

// ����˽�����ݱ�(guildName+serverId)
// �����Ա���
#define GUILD_MEMBER_REC "guild_member_rec"
// ������빫����ұ��
#define GUILD_JOIN_REC "guild_join_rec"
// ���������¼��
#define GUILD_CHAT_HISTORY_REC "guild_chat_history_rec"

// �������������
#define GUILD_LIST_VIRTUALREC_NAME "Game.GuildRec"

// �����̵����ö�ʱ������
#define GUILD_TIMER_NAME "guild_timer_name"
// �����Զ�������鶨ʱ������
#define GUILD_DOWN_LEVEL_TIMER_NAME "guild_dnlevel_timer"


//�������ݰ汾
const char* const GUILD_PUB_DATA_VERSION_ID = "pub_data_version_id";
//��֯������Ϣ
#define GUILD_SET_REC "guild_set_rec"

//Ϣ����ʱ��
#define STOPPING_WAR_TIME "stopping_war_times"


// ÿ�����󹫻��б��ȡ�������
const int REQUEST_GUILD_LIST_PRE_COUNT = 15;

// �˳�������ٴ��������򴴽�����ȴʱ�䣨��λ���ӣ�
const int GUILD_JOIN_CD = 24*60;
const int  ONE_DAY_SEC = 24 * 60 * 60;
const int GUILD_ONE_KEY_JOIN_CD = 1;
// ����������󳤶�,��λΪwchar_t
const int GUILD_NAME_MAX_LENGTH = 6;
// ��������빫����������
const int GUILD_APPLY_MAX_NUM = 10;
// ���ṫ����󳤶�
const int GUILD_NOTICE_MAX_LENGTH = 50;
// ������󳤶�
const int GUILD_DECLARATION_MAX_LENGTH = 18;

// �����б�����������
const int GUILD_LIST_REC_ROW_MAX = 10000;
// ������������������
const int GUILD_SORT_REC_ROW_MAX = 10000;
// �������б�����������
const int GUILD_APPLY_LIST_ROW_MAX = 10000;
// ��������Ҽ��빫��ı����������
const int GUILD_OUTLINE_JOIN_ROW_MAX = 10000;
// ����������˳�����ı����������
const int GUILD_OUTLINE_QUIT_ROW_MAX = 10000;

// �����Ա���������
const int GUILD_MEMBER_REC_ROW_MAX = 100000;
// ������빫����ұ���������
const int GUILD_JOIN_REC_ROW_MAX = 50;

// ���ᶯ̬����������
const int GUILD_TRENDS_REC_ROW_MAX = 100;
// �����̵깺�����������
const int GUILD_SHOP_BUY_REC_ROW_MAX = 100000;
// �����Ա�������������
const int GUILD_MEMBER_BUY_REC_ROW_MAX = 100000;
// �����Ա���ױ���������
const int GUILD_MEMBER_DONATE_REC_ROW_MAX = 100000;
// ���������¼�������
const int GUILD_CHAT_HISTORY_ROW_MAX = 30;

// �����б�����
enum
{
    GUILD_LIST_REC_COL_NAME = 0,                      // ������ VTYPE_WIDESTR
	GUILD_LIST_REC_COL_CAPTAIN_UID,                   // �᳤UID VTYPE_STRING
    GUILD_LIST_REC_COL_CAPTAIN,                       // �᳤���� VTYPE_WIDESTR
	GUILD_LIST_REC_COL_CAPTAIN_LEVEL,                 // �᳤�ȼ� VTYPE_INT
    GUILD_LIST_REC_COL_ANNOUNCEMENT,                  // ���� VTYPE_WIDESTR
	GUILD_LIST_REC_COL_RANK_DATE,                     // ���������ȼ�ֵ������ʱ�� VTYPE_INT64
//	GUILD_LIST_REC_COL_NATION,                        // ������������ VTYPE_INT
    GUILD_LIST_REC_COL_MEMBER_LOGIN_TIME,             // �����Ա�����½�˳�ʱ���¼ VTYPE_INT64(��ɢʹ��)
	GUILD_LIST_REC_COL_DECLARATION,					  // ���� VTYPE_WIDESTR
	GUILD_LIST_REC_COL_FIGHT_ABILITY,				  // ����ս���� VTYPE_INT
    GUILD_LIST_REC_COL_COUNT,
};

// ���������
enum
{
	GUILD_SORT_REC_COL_NAME = 0,                      // �������� VTYPE_WIDESTR
	GUILD_SORT_REC_COL_NATION,                        // ������������ VTYPE_INT
	GUILD_SORT_REC_COL_COUNT,
};

// �������б�����
enum
{
	GUILD_APPLY_REC_COL_NAME_UID = 0,   // ����������UID VTYPE_STRING
	GUILD_APPLY_REC_COL_NAME,           // ���������� VTYPE_WIDESTR
	GUILD_APPLY_REC_COL_GUILD,          // �������Ĺ��Ἧ�� VTYPE_WIDESTR
	GUILD_APPLY_REC_COL_COUNT,
};

// ��¼��������Ҽ��빫��ı�����
enum
{
	GUILD_OUTLINE_JOIN_REC_COL_PLAYER_NAME_UID = 0,     // �������UID VTYPE_STRING
    GUILD_OUTLINE_JOIN_REC_COL_PLAYER_NAME,             // ������� VTYPE_WIDESTR
    GUILD_OUTLINE_JOIN_REC_COL_GUILD_NAME,              // �������� VTYPE_WIDESTR
    GUILD_OUTLINE_JOIN_REC_COL_JOIN_DATE,               // ����ʱ�� VTYPE_INT64
    GUILD_OUTLINE_JOIN_REC_COL_COUNT,
};

enum GUILD_MAINTAIN_STATE
{
	MAINTAIN_FAILED, //ά��ʧ��
	MAINTAIN_SUCCESS,//ά���ɹ�

};

// ����̵깺�������
enum
{
    GUILD_SHOP_BUY_REC_COL_ITEMID = 0,              // GuildName+serverId+ItemId+_index VTYPE_WIDESTR
    GUILD_SHOP_BUY_REC_COL_BUY_COUNT,                // ������� VTYPE_INT
    GUILD_SHOP_BUY_REC_COL_COUNT,
};

// �����Ա���������
enum 
{
    GUILD_MEMBER_BUY_REC_COL_UID_ITEMID = 0,        // ���UID+ItemId+_index VTYPE_STRING
    GUILD_MEMBER_BUY_REC_COL_BUY_COUNT,             // ������� VTYPE_INT
    GUILD_MEMBER_BUY_REC_COL_COUNT,
};

//�Զ�ͬ����빫��
enum 
{
	GUILD_AUTO_AGREE_ON = 1,//
	GUILD_AUTO_AGREE_OFF = 0, //
};



// ����˽�б��
// �����Ա�����
enum
{
	GUILD_MEMBER_REC_COL_NAME_UID = 0,		    // ��Ա����UID VTYPE_STRING
    GUILD_MEMBER_REC_COL_NAME,				    // ��Ա���� VTYPE_WIDESTR
    GUILD_MEMBER_REC_COL_SEX,                   // ��Ա�Ա� VTYPE_INT
    GUILD_MEMBER_REC_COL_POSITION,			    // ְλ VTYPE_INT
	GUILD_MEMBER_REC_COL_LEVEL,				    // �ȼ� VTYPE_INT
	GUILD_MEMBER_REC_COL_CAREER,			    // ְҵ VTYPE_INT
	GUILD_MEMBER_REC_COL_FIGHT_ABILITY,		    // ս���� VTYPE_INT
    GUILD_MEMBER_REC_COL_DEVOTE,			    // �ܹ��׶� VTYPE_INT
	GUILD_MEMBER_REC_COL_OUTLINE_DATE,		    // ���һ������ʱ�� VTYPE_INT64
	GUILD_MEMBER_REC_COL_ONLINE,			    // ����״̬ VTYPE_INT
    GUILD_MEMBER_REC_COL_DEVOTE_SILVER,         // ��Ҿ�����Ԫ���� VTYPE_INT
	GUILD_MEMBER_REC_COL_VIP_LEVEL,				// vip�ȼ�
	GUILD_MEMBER_REC_COL_FLAG_BOX_NUM,			// ս�챦�����

	GUILD_MEMBER_REC_COL_JOIN_TIME,				// ���빫��ʱ�� VTYPE_INT64
	GUILD_MEMBER_REC_COL_TEACH_NUM,				// ���촫������ VTYPE_INT
	GUILD_MEMBER_REC_COL_BE_TEACH_NUM,			// ���챻�������� VTYPE_INT

    GUILD_MEMBER_REC_COL_COUNT,
};

// ������빫����ұ����
enum
{
	GUILD_JOIN_REC_COL_PLAYER_NAME_UID = 0,		// �������UID VTYPE_STRING
    GUILD_JOIN_REC_COL_PLAYER_NAME,		        // ������� VTYPE_WIDESTR
    GUILD_JOIN_REC_COL_SEX,                     // ��Ա�Ա� VTYPE_INT
	GUILD_JOIN_REC_COL_LEVEL,			        // �ȼ� VTYPE_INT
	GUILD_JOIN_REC_COL_CAREER,			        // ְҵ VTYPE_INT
	GUILD_JOIN_REC_COL_FIGHT_ABILITY,	        // ս���� VTYPE_INT
	GUILD_JOIN_REC_COL_VIP,				       // vip VTYPE_INT
	GUILD_JOIN_REC_COL_ONLINE,					// �Ƿ����� VTYPE_INT
    GUILD_JOIN_REC_COL_COUNT,
};

// ���ᶯ̬�����
enum
{
    GUILD_TRENDS_REC_COL_NAME_UID = 0,      // �������UID VTYPE_STRING
    GUILD_TRENDS_REC_COL_NAME,              // ������� VTYPE_WIDESTR
    GUILD_TRENDS_REC_COL_LEVEL,             // ��ҵȼ� VTYPE_INT
    GUILD_TRENDS_REC_COL_DATE,              // ���� VTYPE_INT64
    GUILD_TRENDS_REC_COL_TYPE,              // ��̬���� VTYPE_INT
    GUILD_TRENDS_REC_COL_PARAM1,            // ����1 VTYPE_INT
    GUILD_TRENDS_REC_COL_PARAM2,            // ����2 VTYPE_INT
    GUILD_TRENDS_REC_COL_PARAM3,            // ����3 VTYPE_INT
    GUILD_TRENDS_REC_COL_COUNT,
};

// ������־
const char* const GUILD_LOG_REC = "guild_log_record";
enum
{
	GUILD_LOG_COL_type = 0,		// ��־���� int
	GUILD_LOG_COL_time = 1,		// ��־ʱ�� int_64
	GUILD_LOG_COL_args = 2,		// ��־���� wstring �Զ��ŷָ�
	GUILD_LOG_COL_TOTAL,
};


enum GUILD_BUILDING_TYPE
{
	BUILD_BUILDING_TYPE_JU_YI_TING=1,  //������
	BUILD_BUILDING_TYPE_JIN_KU,		 //���
	BUILD_BUILDING_TYPE_XIANG_FANG,  //�᷿
	BUILD_BUILDING_TYPE_CANGKU,		 //�ֿ�
	BUILD_BUILDING_TYPE_SHU_YUAN,    //��Ժ
	BUILD_BUILDING_TYPE_SHANG_DIAN,	 //�̵�
	BUILD_BUILDING_TYPE_MAX,

};
//��֯����
#define GUILD_BUILDING_REC "guild_building_rec"
//��֯����
enum{

	GUILD_BUILDING_REC_COL_TYPE ,	//��������
	GUILD_BUILDING_REC_COL_LEVEL,	//�����ȼ�
	GUILD_BUILDING_REC_COL_EXP,		//������ǰ����
	GUILD_BUILDING_REC_COL_STATE,	//����״̬
	GUILD_BUILDING_REC_COL_ISBAO,	//�Ƿ�ʹ���ر�
	GUILD_BUILDING_REC_COL_MAX,


};

// ����״̬
enum GUILD_BUILDING_STATE
{
	GUILD_BUILDING_STATE_CAN_LEVELUP,			//���Խ�������
	GUILD_BUILDING_STATE_LEVELUPING,    //������
	GUILD_BUILDING_STATE_STOP,			//ֹͣ
};

// �����Ƿ�ʹ���ر�
enum GUILD_BUILDING_USED_BAO
{
	GUILD_BUILDING_BAO_NONE,		// δʹ��
	GUILD_BUILDING_BAO_USED,		// ʹ�� 
};

#define GUILD_NUM_REC "guild_num_rec"
enum
{
	GUILD_NUM_REC_COL_TYPE ,
	GUILD_NUM_REC_COL_ENABLE_VALUE, //����ʹ�õ��ʽ�
	GUILD_NUM_REC_COL_LOCK_VALUE,   //�������ʽ�
	GUILD_NUM_REC_COL_MAX,

};
enum{
	GUILD_OPERATE_START_LVUP,
	GUILD_OPERATE_STOP_LVUP,

};
//��֯�ֺ��¼
#define GUILD_BONUS_REC  "guild_bonus_rec"
// ��֯�ֺ��¼ 
enum GUILD_BONUS_REC_COL
{
	GUILD_BONUS_REC_COL_NAME, //�����
	GUILD_BONUS_REC_COL_TIMES,//�ֺ����
	GUILD_BONUS_REC_COL_MAX,
};



//��֯���Էֺ���ʽ�
#define GUILD_BONUS_VALUE "guild_bonus_value"
//�ֺ챻�������ʽ���һ�ܲſ��Է���
#define GUILD_BONUS_LOCK_VALUE "guild_bonus_lock_value"

enum GUILD_GIVE_OUT_BONUS_TYPE
{
	GUILD_GIVE_OUT_BONUS_ALL,   //������
	GUILD_GIVE_OUT_BONUS_ELITE, //��Ӣ������
	GUILD_GIVE_OUT_BONUS_ASSIGN, //ָ��

};

// ��֯����
enum GUILD_NUM_AWARD
{

	GUILD_AWARD_TYPE_CAPITAL = 1,	// ��֯�ʽ�
	GUILD_AWARD_TYPE_ANDINGDU,		// ������
	GUILD_AWARD_TYPE_FANRONGDU,		// ���ٶ�
	GUILD_AWARD_TYPE_XINGDONGLI,	// �ж���
	GUILD_AWARD_TYPE_JIANKANDU,		// ������
	GUILD_AWARD_TYPE_BUILDING_EXP,	// ��֯�����
	
};



//��֯��ֵ����
enum GUILD_NUM_TYPE
{
	GUILD_NUM_CAPITAL=1, //��֯�ʽ�
	GUILD_NUM_ANDINGDU,//������
	GUILD_NUM_FANRONGDU, //���ٶ�
	GUILD_NUM_XINGDONGLI,//�ж���
	GUILD_NUM_JIANKANDU,//������
	


};
//�ر�����
enum GUILD_RARE_TREASURE_TYPE
{
	GUILD_RARE_TREASURE_NONE = 0, //��
	GUILD_RARE_TREASURE_WELKIN ,//�ر��칬
	GUILD_RARE_TREASURE_STOPPING_WAR ,//�ر�Ϣ��
	GUILD_RARE_TREASURE_MAX,

};
//�ر�ʹ�÷���
enum GUILD_USER_RARE_TREASURE_RET
{
	GUILD_USER_RARE_TREASURE_RET_ERROR = -1,//δ֪����
	GUILD_USER_RARE_TREASURE_RET_SUCCESS = 0,//�ɹ�
	GUILD_USER_RARE_TREASURE_RET_STOPPING_WAR_IS_USING, //�Ѿ�ʹ���ر�
};





//��֯��ֵ����
#define GUILD_NUM_BUY_REC "guild_num_buy_rec"
enum
{
	GUILD_NUM_BUY_REC_COL_TYPE, //��֯��ֵ��������
	GUILD_NUM_BUY_REC_COL_TIMES,//��֯��ֵ�������
	GUILD_NUM_BUY_REC_COL_MAX,
};




//��֯����
#define GUILD_PUB_SKILL_REC "guild_pub_skill_rec"
enum {
	GUILD_PUB_SKILL_REC_COL_SKILLID, //����id
	GUILD_PUB_SKILL_REC_COL_LEVEL,   //���ܵȼ�
	GUILD_PUB_SKILL_REC_COL_EXP,     //���ܾ���
	GUILD_PUB_SKILL_REC_COL_STUDY_STATE,   //�о�״��
	GUILD_PUB_SKILL_REC_COL_MAX,


};
//��֯����
enum GUILD_PUB_SKILL_STUDY_STATE{

	GUILD_PUB_SKILL_STUDY_STATE_CAN_STUDY,//���Խ����о�
	GUILD_PUB_SKILL_STUDY_STATE_STUDYING,//����������
	GUILD_PUB_SKILL_STUDY_STATE_STOP,		//��ͣ��


};

enum GUILD_SKILL_TYPE
{
	GUILD_SKILL_TYPE_NOMAL,  //��ͨ����
	GUILD_SKILL_TYPE_SPECIAL,//���⼼��
};

#define GUILD_RARE_TREASURE_REC "guild_rare_treasure_rec"
enum
{
	GUILD_RARE_TREASURE_REC_COL_TYPE,//�ر�����
	GUILD_RARE_TREASURE_REC_COL_NUM, //�ر�����
	GUILD_RARE_TREASURE_REC_COL_MAX,
};



//ÿ�չ�����֯�ʽ�
#define GUILD_GAIN_CAPITAL_REC "gulid_gain_capital_rec"

enum
{
	GUILD_GAIN_CAPITAL_REC_COL_NAME,         //�����
	GUILD_GAIN_CAPITAL_REC_COL_VALUE,        //ÿ�ջ����֯�ʽ�ֵ
	GUILD_GAIN_CAPITAL_REC_COL_EXTRA_AWARD_SATE,  //���⽱��״̬ 0����������ȡ�� 1��������ȡ
	GUILD_GAIN_CAPITAL_REC_COL_MAX,


};

#define DAILY_GET_GUILD_CAPITAL "dailygetguildcapital" //ÿ�ջ�ȡ������֯�ʽ�


// �����Ա��Ը������ ��ʱ�� [�������ڣ�����ʱ�����]
const int   C_MAX_GUILD_PETITION_AUCTION = 100000;
const char  C_REC_GUILD_PETITION_AUCTION[] = "guild_petition_auction_rec";
enum
{
	E_FIELD_GUILD_PETITION_AUCTION_WCHAR_NAME = 0,	// ��Ա���� VTYPE_WIDESTR
};




const int GUILD_LOG_REC_MAX_ROW		= 256;	// ���������
const int GUILD_LOG_REC_PAGE_COUNT	= 20;	// ÿҳ����
const int GUILD_LOG_DONATE_COUNT	= 4;	// ������־�·�����
const int GUILD_SKILL_MAX_ROW		= 50;	// �������
const int GUILD_MUTUAL_RANK_REC_MAX_ROW = 100;   //��֯������
const int GUILD_SHOP_SELL_LIST_MAX_ROW = 100;	//�̵�ɳ�����Ʒ�б�
const int GUILD_TOMORROW_SHOP_SELL_LIST_MAX_ROW = 100;//�̵����տɳ�����Ʒ�б�
const int GUILD_BUILDING_REC_MAX_ROW = 50;	//��֯��������
const int GUILD_NUM_REC_MAX_ROW      = 10;	//��֯��ֵ
const int GUILD_SET_ERC_MAX_ROW = 20;		//��֯������Ϣ
const int GUILD_RARE_TREASURE_MAX_ROW = 50; //��֯�ر�����
const int GUILD_BONUS_MAX_ROW = 10000;		//��֯�ֺ����
// ������־���� �����ͻ��˻ص������в��Ҷ�Ӧ��
enum GuildLogType
{
	GUILD_LOG_TYPE_NONE				= 0,	// ������
	GUILD_LOG_TYPE_JOIN				= 1,	// ������		���� �������
	GUILD_LOG_TYPE_LEAVE			= 2,	// �˳����		���� �������
	GUILD_LOG_TYPE_BE_LEAVE			= 3,	// ���߳����	���� ������ �������
	GUILD_LOG_TYPE_ADD_GUILD_NUM	= 4,	// ������֯��ֵ		���� ԭ��  ������� ��֯��ֵ���� ���� 
	GUILD_LOG_TYPE_DEC_GUILD_NUM	= 5,	// ������֯��ֵ		���� ԭ�� ������� ��֯��ֵ���� ���� 
	GUILD_LOG_TYPE_UP_LEVEL			= 6,	// �������		���� ������� ���ȼ�
	GUILD_LOG_TYPE_DOWN_LEVEL		= 7,	// ��ή��		���� ������� ���ȼ�
	GUILD_LOG_TYPE_ACTIVITY			= 8,	// ���		���� ���ز��� ������
	GUILD_LOG_TYPE_PROMOTION		= 9,	// ��ְ			���� ������� ��ְ����(��һ�����֣���Ҫת��Ϊ��ְ����)
	GUILD_LOG_TYPE_DEMOTION			= 10,	// ��ְ			���� ������� ��ְ����(��һ�����֣���Ҫת��Ϊ��ְ����)
    GUILD_LOG_TYPE_IMPEACH_START    = 11,   // ��������     ���� ����������
    GUILD_LOG_TYPE_IMPEACH_SUCCEED  = 12,   // �����ɹ�     ���� ����������
    GUILD_LOG_TYPE_IMPEACH_FAIL     = 13,   // ����ʧ��     ���� ����������
    GUILD_LOG_TYPE_PROMOTION_NEW	= 14,	// ��ְ			����  ������ ������� ��ְ(�����ж�Ӧ��key "GuildLevelְλ����ֵ")
    GUILD_LOG_TYPE_DEMOTION_NEW		= 15,	// ��ְ			���� ������ ������� ��ְ(�����ж�Ӧ��key "GuildLevelְλ����ֵ")
	GUILD_LOG_TYPE_DEC_DONATE		= 16,	// �۳��ﹱ		���� ����� ����ԭ�� ��ǰ�ﹱ �۳��ﹱ ��ǰ�ﹱ
	GUILD_LOG_TYPE_DONATE_TRADE_SILVER = 17,	// ������Ԫ		���� ������� ��������
	GUILD_LOG_TYPE_BUILDING         =18,    //��֯������־     ���� int  ����
	GUILD_LOG_TYPE_SKILL            = 19,   //������־
	GUILD_LOG_USE_RARE_TREASURE     = 20,   // ʹ����֯�ر�   ���� int wstring �����   
	GUILD_LOG_BONUS					 = 21,    // �ֺ�  wstring ����� int �ܹ��ֺ�
	GUILD_LOG_GUILD_BATTLE			= 22,   //��֯ս��
	GUILD_LOG_MEMBER_EVENT			= 23,   //��֯��Ա�¼�
	GUILD_LOG_ACTIVITY_EVENT		= 24,   //��֯��¼�
};
//�ֺ���־
enum GUILD_LOG_BONUS_TYPE
{
	GUILD_LOG_BONUS_TYPE_ADD_BONUS,		  //��ӷֺ�
	GUILD_LOG_BONUS_TYPE_GIVE_OUT_BONUS, //�ֺ�
};

//��֯��Ա�¼�����
enum GUILD_LOG_MEMBER_EVENT_TYPE
{
	GUILD_LOG_MEMBER_EVENT_PLAYER_LV_UP, //������� wstring ����� int �ȼ�
	GUILD_LOG_MEMBER_EVENT_UNLOCK_ACHIEVEMENT, //������� wstring string �ɾ�
	GUILD_LOG_MEMBER_EVENT_GET_TITLE,		//�ƺ�  wstring �����  int �ƺ�id
	GUILD_LOG_MEMBER_EVENT_GET_EQUIPMENT, //ϡ��װ��
};
//��֯��¼�����
enum GUILD_LOG_ACTIVITY_EVENT_TYPE
{
	GUILD_LOG_ACTIVITY_EVENT_TYPE_KIDNAP ,//��Ʊwstring ����� 
	GUILD_LOG_ACTIVITY_EVENT_TYPE_FINISH_TASK,//��֯����wstring ����� 

};


//1���Ǹ��������õģ�2���Ǹ����ط��õ�
enum GUILD_LOG_GUILD_BATTLE_TYPE
{
	GUILD_LOG_GUILD_BATTLE_TYPE_OCCUPY_BEGIN,//����ս���Ŀ�ʼ int towerid
	GUILD_LOG_GUILD_BATTLE_TYPE_OCCUPY_END1,//����ս���Ľ��� int towerid atta_name
	GUILD_LOG_GUILD_BATTLE_TYPE_OCCUPY_END2,//����ս���Ľ��� int towerid atta_name

	GUILD_LOG_GUILD_BATTLE_TYPE_PLUNDER_BEGIN,//�Ӷ�ս���Ŀ�ʼint towerid 

	GUILD_LOG_GUILD_BATTLE_TYPE_PLUNDER_END,//�Ӷ�ս���Ľ��� int towerid atta_name

	GUILD_LOG_GUILD_BATTLE_TYPE_OCCUPY_ACTIVITY_BEFORE1,//����׼��ս��ʼ 
	GUILD_LOG_GUILD_BATTLE_TYPE_OCCUPY_ACTIVITY_BEFORE2,//����׼��ս��ʼ 

	GUILD_LOG_GUILD_BATTLE_TYPE_OCCUPY_ACTIVITY_BEGIN1,//����ս��ʼ int towerid atta_name own_name
	GUILD_LOG_GUILD_BATTLE_TYPE_OCCUPY_ACTIVITY_BEGIN2,//����ս��ʼ int towerid atta_name own_name
	
	GUILD_LOG_GUILD_BATTLE_TYPE_OCCUPY_ACTIVITY_END1,//����ս���� int towerid int (���) atta_name own_name
	GUILD_LOG_GUILD_BATTLE_TYPE_OCCUPY_ACTIVITY_END2,//����ս��int towerid int (���) atta_name own_name��

	GUILD_LOG_GUILD_BATTLE_TYPE_PLUNDER_ACTIVITY_BEFORE1,//�Ӷ�׼��ս��ʼ 
	GUILD_LOG_GUILD_BATTLE_TYPE_PLUNDER_ACTIVITY_BEFORE2,//�Ӷ�׼��ս��ʼ

	GUILD_LOG_GUILD_BATTLE_TYPE_PLUNDER_ACTIVITY_BEGIN1,//�Ӷ�ս��ʼ  int towerid atta_name own_name
	GUILD_LOG_GUILD_BATTLE_TYPE_PLUNDER_ACTIVITY_BEGIN2,//�Ӷ�ս��ʼ int towerid atta_name own_name

	GUILD_LOG_GUILD_BATTLE_TYPE_PLUNDER_ACTIVITY_END1,//�Ӷ�ս���� int towerid atta_name own_name money item
	GUILD_LOG_GUILD_BATTLE_TYPE_PLUNDER_ACTIVITY_END2,//�Ӷ�ս���� int towerid atta_name own_name money iteme
	

};

enum GUILD_LOG_TYPE_BUILDING_TYPE{
	BUILDING_LOG_TYPE_START_BUILDING,        //���콨���� ����  wstring ����� int �������� ���� int�ȼ�
	BUILDING_LOG_TYPE_CONTINUE_BUILDING,     //���������� ���� wstring ����� int �������� ���� int�ȼ�
	BUILDING_LOG_TYPE_STOP_BUILDING,        //ֹͣ������ ����  wstring ����� int �������� ���� int�ȼ�
    BUILDING_LOG_TYPE_LV_UP_SUCCESS,		//�����ɹ�  ���� int �������� ���� int�ȼ�
	BUILDING_LOG_TYPE_MAINTAIN_ADD_STUDY_EXP, //ά���ɹ������о����� int �Ӿ���
	BUILDING_LOG_TYPE_MAINTAIN_DEC_FRD,    //ά����֯�ʽ���۳����ٶ� int ���ĵķ��ٶ�
	BUILDING_LOG_TYPE_MAINTAIN_FAIL,    //ά��ʧ��
};

enum GUILD_LOG_TYPE_SKILL_TYPE{
	SKILL_LOG_TYPE_UNLOCK, //����   ����id  wstring �����
	SKILL_LOG_TYPE_START_STUDY,  //�о� ����id  wstring �����
	SKILL_LOG_TYPE_CONTINUE_STUDY,//�����о� ����id  wstring �����
	SKILL_LOG_TYPE_STOP_STUDY, //ֹͣ�о� ����id  wstring �����
	SKILL_LOG_TYPE_SKILL_LV_UP, //��������  ����id ���ܵȼ�
};

enum GUILD_NUM_CHANGE
{

	GUILD_NUM_CHANGE_GM, //		gm ����
	GUILD_NUM_CHANGE_DONATE, //������
	GUILD_NUM_CHANGE_BUILD_LVUP, //��������
	GUILD_NUM_CHANGE_BUY_GUILD_NUM,//������֯��ֵ
	GUILD_NUM_CHANGE_CHANGE_GUILD_NOTIFY,//��֯����
	GUILD_NUM_CHANGE_CHANGE_SHORT_NAME,	//�޸Ķ���
	GUILD_NUM_CHANGE_CHANGE_RECOVER,	//ÿ�ջָ�
	GUILD_NUM_CHANGE_CHANGE_MAINTAIN,	//ά��
	GUILD_NUM_CHANGE_DAILY_GET,			//�����ȱ��׻�ȡ
	GUILD_NUM_CHANGE_SKILL_STUDY,		//�����о�
	GUILD_NUM_CHANGE_SKILL_UNLOCK,		//���ܽ���
	GUILD_NUM_CHANGE_PLUNDER_AUCTION_DEC,	//�����ߣ�  ���Ŀ۳�
	GUILD_NUM_CHANGE_PLUNDER_AUCTION_BACK,  //�����ߣ�  ���ķ���
	GUILD_NUM_CHANGE_PLUNDER_ADD,	//�Ӷ�ս���Ӷ��ߣ�������
	GUILD_NUM_CHANGE_PLUNDER_DEC,	//�Ӷ�ս�����Ӷ��ߣ����۳�
	GUILD_NUM_CHANGE_SHOP,			 //��֯�̵�
	GUILD_NUM_CHANGE_OCCUPY_AUCTION_DEC,	//��֯�Ǿ��Ŀ۳�
	GUILD_NUM_CHANGE_OCCUPY_AUCTION_BACK,   //��֯�Ǿ��ķ���
	GUILD_NUM_CHANGE_TOWN_ADD,			    //��֯��
	GUILD_NUM_CHANGE_TASK,   //����
	GUILD_NUM_CHANGE_KIDNAP,			    //��Ʊ
	GUILD_NUM_CHANGE_DANCING,			    //�輧�۳�
	GUILD_NUM_CHANGE_DAILY_GET_GUILD_CAPITAL_NOT_ENOUGHT, //ÿ�ջ�ȡ�ʽ��� �۳�������
	GUILD_NUM_CHANGE_MAINTAIN_FAILED_DEC,	//ά��ʧ�� �۳�������
	GUILD_NUM_CHANGE_NOT_OPEN_GUILD_ACTIVITY,//δ������֯� �۳�������
	GUILD_NUM_CHANGE_PLUNDER_AUCTION_INIT_DEC,	 //���Ӷ��ߣ� ����ǰ���۳�����  <����Ҫ��־>
	GUILD_NUM_CHANGE_PLUNDER_AUCTION_INIT_BACK,  //���Ӷ��ߣ� �Ӷ�󣬷��ؽ���  <����Ҫ��־>
};



// �����Ա��ְλ
enum
{
	GUILD_POSITION_NONE = -1,
	GUILD_POSITION_CAPTAIN = 0,             // �᳤
	GUILD_POSITION_ASSISTANT_CAPTAIN,       // ������
	GUILD_POSITION_GENERAL,                 // ����
	GUILD_POSITION_ELDER,                   // ��Ӣ
	GUILD_POSITION_MEMBER,                  // һ���Ա
    GUILD_POSITION_COUNT,
};

//�ͻ���ְλ�ֵ�ǰ��
const wchar_t* const GUILD_LEVEL =  L"GuildLevel";

// ���ᶯ̬����
enum
{
	GUILD_TRENDS_TYPE_JOIN = 0,          // ���빫��
	GUILD_TRENDS_TYPE_INVITE_JOIN,       // ��������빫��
	GUILD_TRENDS_TYPE_QUIT,              // �˳�����
	GUILD_TRENDS_TYPE_FIRE,              // ���߳�����
	GUILD_TRENDS_TYPE_BUILD,             // ���蹫��
	GUILD_TRENDS_TYPE_UPLEVEL,           // ��������
};

// ����Ȩֵ����
enum GuildPriv
{
	GUILD_PRIV_SET_POSITION = 1,           //����ְλ
	GUILD_PRIV_RATIFY,		            //��׼���
	GUILD_PRIV_FIRE,			        //����
	GUILD_PRIV_ANNOUNCEMENT,	        //��������
	GUILD_PRIV_STATION_DANCING_GIRL,	//����輧
	GUILD_PRIV_AUTO_AGREE,				//����Զ�ͬ��
	GUILD_PRIV_UNLOCK_SKILL,			//��������
	GUILD_PRIV_BUILDING_LEVEL_UP,		//��������
	GUILD_PRIV_GUILD_NUM_BUY,		   // ��֯��ֵ����
	GUILD_PRIV_CHANGE_SHORT_NAME,	    //�޸���֯���
	GUILD_PRIV_START_STUDY_SKILL,	    //��ʼ�о�����
	GUILD_PRIV_USE_RARE_TREASURE,		//ʹ��Ϣ����
	GUILD_PRIV_GIVE_OUT_BONUS,			//�ֺ�
};

enum{
	Normal_Donate = 0, //��ͨ�ﹱ
	Boss_Donate = 1// boss �ﹱ
};


// ��֯����
enum
{
	GUILD_MUTUAL_HELP_REC_COL_INDEX,			// index
	GUILD_MUTUAL_HELP_REC_COL_NAME,				//�����
	GUILD_MUTUAL_HELP_REC_COL_POSITION,			//ְλ
	GUILD_MUTUAL_HELP_REC_COL_ITEM_ID,			//����id
	GUILD_MUTUAL_HELP_REC_COL_TIMES,			//��ǰ�Ѿ���õĵ�������
	GUILD_MUTUAL_HELP_REC_COL_TIMES_MAX,		//�ɻ�õ����������ֵ
	GUILD_MUTUAL_HELP_REC_COL_MAX,				
};
enum 
{
	GUILD_MUTUAL_RANK_REC_COL_NAME, //�����
	GUILD_MUTUAL_RANK_REC_COL_DONATE, //����ֵ
	GUILD_MUTUAL_RANK_REC_COL_MAX,
};
enum 
{
	GUILD_SHOP_SELL_LIST_REC_COL_ITEM,//�̵�ɳ�����Ʒ�б�
	GUILD_SHOP_SELL_LIST_COL_MAX,
};
enum 
{
	GUILD_TOMORROW_SHOP_SELL_LIST_COL_ITEM,//�����̵�ɳ�����Ʒ�б�
	GUILD_TOMORROW_SHOP_SELL_LIST_COL_MAX,
};




enum 
{
	GUILD_SYSMBOL_REC_COL_GUILD_NAME,		//��֯��
	GUILD_SYSMBOL_REC_COL_IDENTIFYING,      //��֯��ʶ
	GUILD_SYSMBOL_REC_COL_SHORT_NAME,       //��֯���
};




enum GUILD_SET_REC_COL
{
	GUILD_SET_REC_COL_GUILD_NAME,
	GUILD_SET_REC_COL_AUTO_AGREE,					  // �Զ�ͬ��   VTYPE_INT
	GUILD_SET_REC_COL_AUTO_AGREE_MIN_LEVEL,		  // �Զ�ͬ����Ҫ��С�ȼ�
	GUILD_SET_REC_COL_AUTO_AGREE_MAX_LEVEL,		  // �Զ�ͬ����Ҫ���ȼ�
	GUILD_SET_REC_COL_AUTO_AGREE_ABILITY,		  // �Զ�ͬ����Ҫս����
	GUILD_SET_REC_COL_DANCING_GIRL,				  // �輧      VTYPE_INT
	GUILD_SET_REC_COL_AUTO_OPEN_DANCING,			  // ��֯�輧ԤԼ����ʱ�� VTYPE_INT
	GUILD_SET_REC_COL_MAINTAIN_STATE,				 //  ��֯ά��״̬    VTYPE_INT
};
//��Ը��
#define GUILD_PETITION_REC "guild_petition_rec"
enum GUILD_PETITION_REC_COL
{
	GUILD_PETITION_REC_COL_NAME, //�����
	GUILD_PETITION_REC_COL_TYPE, //����

};
//��Ը����
enum GUILD_PETITION_TYPE
{
	GUILD_PETITION_TYPE_JU_YI_TING = 1,  //����������
	GUILD_PETITION_TYPE_JIN_KU,		 //�������
	GUILD_PETITION_TYPE_XIANG_FANG,  //�᷿����
	GUILD_PETITION_TYPE_CANGKU,		 //�ֿ�����
	GUILD_PETITION_TYPE_SHU_YUAN,    //��Ժ����
	GUILD_PETITION_TYPE_SHANG_DIAN,	 //�̵�����
	GUILD_PETITION_TYPE_MAX,
};




// ���������������ݽṹ
typedef struct CreateGuildConfig_s
{
	CreateGuildConfig_s()
		:m_LevelLimit(0),
		Silver(0),
		m_nFireLimit(0)
	{}

	int m_LevelLimit;   // ��ҵȼ�����
	int Silver;       // Ԫ����������
	int m_nFireLimit;	// ����һ�����˵Ĵ�������
}GuildCreateConfig_t;

// ����ȼ����ýṹ
typedef struct GuildUpLevelConfig_s
{
    GuildUpLevelConfig_s() : m_Level(0),
		m_MaxMember(0), 
		m_DeputyCaptainNum(0),
		m_GeneralNum(0),
		m_Elder(0)
	{}

    int m_Level;
    int m_MaxMember;            // ����Ա����
    int m_DeputyCaptainNum;     // ����������
    int m_GeneralNum;             // ��������
	int m_Elder;				// ��Ӣ����
   

}GuildUpLevelConfig_t;
typedef std::vector<GuildUpLevelConfig_t> GuildUpLevelConfigVector;


struct GuildBoss {

	GuildBoss() {
		m_Level = 0;
		m_LevelUpExp = 0;
	}

	int m_Level;
	int m_LevelUpExp;
};

typedef std::map<std::string, GuildBoss> GuildBossLevelConfig;

// ������
typedef struct GuildDonateConfig_s
{
    GuildDonateConfig_s() : m_Id(0), m_CapitalNumber(0), 
		 m_RewardPlayerDonate(0), m_GuildDonate(0) {}

    int m_Id;
    int m_CapitalNumber;        // ��ʾ���׵Ļ�������
    int m_RewardPlayerDonate;   // ��ʾ��һ�õİﹱֵ
	int m_GuildDonate;			// ��֯�ﹱ����
} GuildDonateConfig_t;
typedef std::vector<GuildDonateConfig_t> GuildDonateConfigVector;

// ����ְ��Ȩ��������Ϣ�ṹ
typedef struct GuildPositionConfig_s
{
	GuildPositionConfig_s()
		: m_Position(0),
		m_privList(0)
	{}

	int m_Position;                    // ְλ	
	int m_privList;					   //Ȩ���б�
}GuildPositionConfig_t;
typedef std::vector<GuildPositionConfig_t> GuildPositionConfigVector;
typedef GuildPositionConfigVector::iterator GuildPositionConfigVectorIter;



struct GuildSkillLevelUpSpend{
	// �ʽ�
	std::map<int, int> m_vCurrency;
	// ��Ʒ
	std::map<std::string, int> m_vItems;
};
typedef std::map<int,std::map<int, GuildSkillLevelUpSpend> > GuildSkillLevelUpSpendMap;
//��֯��������key itemid,value �����ܾ��״���

struct GuildMutual{
	GuildMutual() :m_donate(0), m_limitTimes(0){}
	int m_donate;
	int m_limitTimes;
};


enum
{
	GUILD_DONATE_DEC_DANCING_GIRL,		//�ﹱ�۳��ٻ��輧
	GUILD_DONATE_DEC_UNLOCK_SKILL,		//��������
	GUILD_DONATE_DEC_CHANGE_SHOP_LIST,	//�޸��̵���Ʒ

};



struct GuildSkillConfig{
	int m_initLevel{ 0 };
	int m_unlockShuYuanLevel{ 0 };
	bool m_autoUnlock{ false };
	int  m_unlockCost;
	int m_startStudyCost;
	GUILD_SKILL_TYPE m_skillType{ GUILD_SKILL_TYPE::GUILD_SKILL_TYPE_NOMAL };
};
struct GuildSkillLv{
	int m_exp;
	int m_shuYuanLv;//��Ҫ��Ժ�ȼ�

};
typedef std::map<int, GuildSkillConfig> GuildSkillConfigMap;
typedef std::map<int, std::map<int, GuildSkillLv>> GuildSkillLvExp;
//�̵������Ʒ����key��֯�ȼ� value �ɳ�����Ʒ��
typedef std::map<int, int> GuildShopSellNum;


// ���ڹ���᳤�Զ��ƽ���ѡ���»᳤����
struct GuildMemberSortHelper_t
{
	GuildMemberSortHelper_t() : memberName(L""), position(0), contrib(0), level(0), ability(0) {}
    GuildMemberSortHelper_t(const wchar_t *n, int p, int c, int l, int a) :
        memberName(n), position(p), contrib(c), level(l), ability(a) {}

    bool operator >= (const GuildMemberSortHelper_t &other) const 
    {
        if (position == other.position) {
            if (contrib == other.contrib) {
                if (level == other.level) {
                    if (ability == other.ability) {

                    }
                    else {
                        return ability > other.ability;
                    }
                }
                else {
                    return level > other.level;
                }
            }
            else {
                return contrib > other.contrib;
            }
        }
        else {
            return position < other.position;   // ��ֵС��ְ���
        }
        return false;
    }

    std::wstring memberName;
    int position;
    int contrib;
    int level;
    int ability;
};
typedef std::multiset<GuildMemberSortHelper_t, std::greater_equal<GuildMemberSortHelper_t> > GuildMemberSortHelperSet;

struct GuildShopConfigItem_t
{	
	GuildShopConfigItem_t(): m_Index(0), m_ItemId(""), m_MemberLimit(0), m_GuildTotal(0), m_UnlockLevel(0){}
	int m_Index;
    std::string m_ItemId;
    int m_MemberLimit;
    int m_GuildTotal;
    int m_UnlockLevel;
	int m_addGuildCapital{0}; //������֯�ﹱ
	std::vector<std::pair<int, int>> m_decCapital;//��Ҫ�۳��Ļ���
};
typedef std::vector<GuildShopConfigItem_t> GuildShopConfigVector;


#define GUILD_PUB_RESET "guild_pub_reset" 
#define GUILD_BUILDING_MAINTAIN "guild_building_maintain"
class IPubKernel;
typedef void (*GuildTimerCb) (IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name);

struct GuildTimerCallback_t
{
    GuildTimerCallback_t() : m_TimerName(""),
		m_CallbackName(""),
		m_Hour(0), 
		m_Minute(0),
		m_CallbackFunc(NULL)
	{}
    GuildTimerCallback_t(int h, int m) : m_Hour(h),
		m_Minute(m),
		m_CallbackFunc(NULL)
	{}

    bool operator == (const GuildTimerCallback_t &other) const
    {
        return m_Hour == other.m_Hour && m_Minute == other.m_Minute;
    }
    bool operator < (const GuildTimerCallback_t &other) const 
    {
        if (m_Hour < other.m_Hour)
        {
            return true;
        }
        if (m_Hour == other.m_Hour)
        {
            return m_Minute < other.m_Minute;
        }
        return false;
    }
    int operator - (const GuildTimerCallback_t &other) const
    {
        int hours = m_Hour - other.m_Hour;
        int minutes = m_Minute - other.m_Minute;
        return hours * 3600 + minutes * 60;
    }
    
    std::string m_TimerName;
    std::string m_CallbackName;
    int m_Hour;
    int m_Minute;
    GuildTimerCb m_CallbackFunc;
};
typedef std::vector<GuildTimerCallback_t> GuildTimerCallbackVector;




struct GuildPrepareInfo{
	int m_nation{ 0 };
	std::wstring m_shortName;
	int m_guildIdentifying{ 0 };
	


};


typedef std::map<std::wstring, GuildPrepareInfo> GUILDPREPAREINFO;


#define MAIL_TEACH_REWARD	"mail_teach_reward"

#define CONFIG_PATH_TECAH_REWARD	"ini/rule/guild/guild_teach_reward.xml"
// ������������
DECLARE_CONFIG_OBJECT_BEGIN(CfgTeachReward)

#define CONFIG_CLASS_NAME CfgTeachReward
#define PROPLIST      \
	DECLARE_CONFIG_PROP_FIELD_EX(ID, "��Ҵ����ȼ�", 0, size_t)	\
	DECLARE_CONFIG_PROP_FIELD(SendReward, "�����߽���", string) \
	DECLARE_CONFIG_PROP_FIELD(GetReward, "�����߽���", string)

#include "utils/util_config_template.h"
DECLARE_CONFIG_OBJECT_END()


#define TEMP_FIELD_TEACH_TARGET "_temp_teach_target"		// ������������
#define TEMP_FIELD_TEACH_TYPE "_temp_teach_type"			// ��������
#define TEMP_FIELD_TEACH_STATE "_temp_teach_state"			// ����״̬

// ����״̬
enum EmTeachState
{
	EM_TEACH_STATE_NONE,				// ��ʼ״̬
	EM_TEACH_STATE_ENTER,				// ����״̬
	EM_TEACH_STATE_READY,				// ׼������
	EM_TEACH_STATE_COMPLETED,			// �����
};

// ��������
enum EmTeachType
{
	EM_TEACH_SENDER = 1,		// ������
	EM_TEACH_RECIVER = 2,		// ������
};

// ��������ʽ
enum EmTeachMethod
{
	EM_TEACH_METHOD_REQ = 1,	// ����
	EM_TEACH_METHOD_PSH ,		// ����
};

// ������Ӧ���
enum EmTeachAckResult
{
	EM_TEACH_ACK_ACCEPT = 1,	// ����
	EM_TEACH_ACK_REFUSE,		// �ܾ�
};

// �������
enum EmTeachResult
{
	EM_TEACH_RESULT_SUCCEED,			// �ɹ�
	EM_TEACH_RESULT_OFFLINE,			// �Է�������
	EM_TEACH_RESULT_NO_GUILD,			// δ������֯��ɢ����֯�����Դ���
	EM_TEACH_RESULT_PROTECT,			// �Է��ڴ���������(������֯����16Сʱ)
	EM_TEACH_RESULT_PROTECT_SELF,		// �Լ��ڴ���������(������֯����16Сʱ)
	EM_TEACH_RESULT_NO_PLAYER,			// ���ǹ��س�Ա
	EM_TEACH_RESULT_NUM_LIMIT,			// �Է�������������
	EM_TEACH_RESULT_NUM_LIMIT_SELF,		// �Լ�������������
	EM_TEACH_RESULT_IN_ACTIVITY,		// �Է��ڻ��
	EM_TEACH_RESULT_IN_ACTIVITY_SELF,	// �Լ��ڻ��
	EM_TEACH_RESULT_LEVEL_LOWER,		// �����ȼ�̫��
	EM_TEACH_RESULT_REQ_TIMEOUT,		// �������ʱ��
	EM_TEACH_RESULT_ENT_FAILED,			// ��ҽ��볡��ʧ��
	EM_TEACH_RESULT_PLAYER_DATA_ERROR,	// ������ݴ���
	EM_TEACH_RESULT_REFUSE,				// �ܾ�����
};

#endif //__GuildDefine_H__