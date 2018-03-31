//--------------------------------------------------------------------
// �ļ���:		SnsDefine.h
// ��  ��:		��Ϸ��������sns���ݷ�����֮�䴫�ݵ���Ϣ����
// ˵  ��:		��Ϸ��������sns���ݷ�����������Ϣʱ��ǰ�����ֶ����ǣ�
//				fast_string snsspace, wstring snsdata, int msgid
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )   
//    :	    
//--------------------------------------------------------------------

#ifndef __SnsDefine_h__
#define __SnsDefine_h__

//SnsSpace����
const char* const SPACE_SNS = "sns";

//�������ݵ���ϢID��Χ����ϢID�ı�Ų����ڴ˷�Χ��
#define SNSDATA_MSG_ID_MIN      1000
#define SNSDATA_MSG_ID_MAX      1999

//��������SnsData����Ϣ
enum
{
	//�޸����ԣ���ʽ��string pubspace, wstring pubdata, int msgid, string prop, var value, ...;
	SNSDATA_MSG_SET_PROP = 1000,
	//�������ԣ���ʽ��string pubspace, wstring pubdata, int msgid, string prop, int value, ...;
	SNSDATA_MSG_INC_PROP = 1001,
	//���ĳ��������ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, ...;
	SNSDATA_MSG_CLEAR_REC = 1100,
	//ɾ��ĳ�����ĳ�����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int row, ...;
	SNSDATA_MSG_REMOVE_REC_ROW = 1101,
	//���ĳ�����һ�����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, ...;
	SNSDATA_MSG_ADD_REC_ROW_VALUE = 1102,
	//����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int row, int col, var value, ...;
	SNSDATA_MSG_SET_REC_VALUE = 1103,
	//����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int row, int col, var value, ...;
	SNSDATA_MSG_INC_REC_VALUE = 1104,
	//���ݹؼ�ֵɾ��ĳ�����ĳ�����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int keycol, var keyvalue;
	SNSDATA_MSG_REMOVE_REC_ROW_BYKEY = 1105,
	//���ݹؽ�ֵ���ù����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int keycol, var keyvalue, int col, var value, ...;
	SNSDATA_MSG_SET_REC_VALUE_BYKEY = 1106,
	//���ݹؽ����ӹ����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int keycol, var keyvalue, int col, var value, ...;
	SNSDATA_MSG_INC_REC_VALUE_BYKEY = 1107,
	//���ݹؽ����ӹ����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string snsspace, wstring snsdata, int msgid
	SNSDATA_MSG_UPDATE_CONFIG = 1108,
	//�޸����ѭ������
	SNSDATA_MSG_SET_MAX_CIRCULATE_COUNT = 1109,
};

enum
{
	OFFLINE = 0, //����
	ONLINE = 1, //����
};

// SNS��Ϣ(���߼�����������SNS����������Ϣ)
enum                                
{
	// ���������
	SNS_MSG_PUSH_PLAYER_OFFLINE = 2000,
	// ����ע����Ϣ
	SNS_MSG_PUSH_PLAYER_REGISTER = 2001,
	// ���͹㲥��Ϣ
	SNS_MSG_PUSH_PLAYER_SEND_MSG = 2002, //wstring ���⣬wstring����
	// ��ѯĳ��������е�ս������,�������ԡ�����
	SNS_MSG_QUERY_PLAYER_FIGHT_DATA = 2003,
	// ȡ���������������
	SNS_MSG_CANCEL_PUSH_OFFLINE_PLAYER = 2004,
	// ������߶�ʱ��Ϣ
	SNS_MSG_PUSH_PLAYER_OFFLINE_TIME = 2005, //int 0����1����
	// ���͹㲥��Ϣ
	SNS_MSG_PUSH_PLAYER_SEND_MSG_SINGLE = 2006,
	//�����������(֧��:int,int64,float,double,sting) cmd_id(int32),"attr_name,attr_name",{ident_id(int64),target_uid(char)}..=>{ident_id,"attr_name=attr_value,attr_name=attr_value"}
	SNS_MSG_QUERY_PLAYER_PROPERTY = 2007,

	//////////////////////////////////////////////////////////////////////////
	// ����ϵͳ��Ϣ����
	SNS_MSG_FRIEND_APPLY = 2100,            // ��������
	SNS_MSG_FRIEND_DELETE,					// ���ѽ��
	SNS_MSG_FRIEND_RESULT,                  // ������
	SNS_MSG_FRIEND_QUERY_FRIEND_APPLY,      // ��ѯ����
	SNS_MSG_FRIEND_QUERY_FRIEND_DELETE,     // ɾ������
	SNS_MSG_FRIEND_QUERY_FRIEND_RESULT,     // ��ѯ���
	SNS_MSG_FRIEND_PUSH_APPLY,              // ��������
	SNS_MSG_FRIEND_PUSH_DELETE,             // ���ͽ����ϵ
	SNS_MSG_FRIEND_PUSH_RESULT,             // ���ͽ��
	SNS_MSG_FRIEND_GIFT_GIVE,				// �����ͻ�
	SNS_MSG_FRIEND_GIFT_GET,				// �ͻ���ȡ
	SNS_MSG_FRIEND_GIFT_PUSH,				// �ͻ�����
	SNS_MSG_FRIEND_PUSH_ADD_INTIMACY,		// �����������ܶȱ���
	SNS_MSG_FRIEND_ADD_INTIMACY_GET,		// ��ȡ�������ܶ�
	SNS_MSG_FRIEND_ADD_INTIMACY_RESULT,		// ��ȡ�������ܶȽ��
	SNS_MSG_FRIEND_ADD_DEL_SWORN,				// ���ɾ������
	SNS_MSG_FRIEND_QUERY_DEL_SWORN,				// ��ȡɾ������
	SNS_MSG_FRIEND_PUSH_DEL_SWORN,				// ����ɾ������
	SNS_MSG_FRIEND_ADD_ENEMY,				//���ӵ���
	SNS_MSG_FRIEND_QUERY_ENEMY,				//��ȡ����
	SNS_MSG_FRIEND_PUSH_ENEMY,				//���ص���

	//////////////////////////////////////////////////////////////////////////
	// ���������
	SNS_MSG_ARENA_SAVE_FIGHT_RESULT = 2200,					// ����������ս������
	SNS_MSG_ARENA_QUERY_FIGHT_RESULT,					 // ��������ѯ����ս������
	SNS_MSG_ARENA_PUSH_FIGHT_RESULT,					 // ��������ս������

};

#endif // __SnsDefine_h__
