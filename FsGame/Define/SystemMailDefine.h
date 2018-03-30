//--------------------------------------------------------------------
// �ļ���:		SystemMailDefine.h
// ��  ��:		ϵͳ�ʼ�����
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __SystemMailDefine_H__
#define __SystemMailDefine_H__

#define SYSTEM_MAIL_MAX 100

#define SYSTEM_MAIL_REC "system_mail_rec"
#define GLOBAL_MAIL_REC "global_mail_rec"
//ȫ���ʼ�����
#define GLOBAL_MAIL_UID "global_mail_uid"
enum SystemMailRecordCol
{
    MAIL_REC_COL_SERIAL_NO=0,	//��ˮ�� string
    MAIL_REC_COL_SENDER_NAME,	//���������� widestr
    MAIL_REC_COL_LIFE_TIME,		//����ʱ�� int64
    MAIL_REC_COL_POST_TIME,		//����ʱ�� int64
    MAIL_REC_COL_MAIL_TYPE,		//�ʼ����� byte
    MAIL_REC_COL_TITLE,			//�ʼ����� widestr
    MAIL_REC_COL_CONTENT,		//�ʼ����� widestr
    MAIL_REC_COL_MONEY,			//��������Ǯ string
    MAIL_REC_COL_ITEM,			//��������Ʒ string
    MAIL_REC_COL_READ_FLAG,		//�Ѷ���־ byte
    MAIL_REC_COL_PICK_FLAG,		//������ȡ��־ byte
	MAIL_REC_COL_PROPERTY,		//���������� string
};

enum
{
	DOMAIN_MAIL_REC_MAIL_ID,			//�ʼ�id
	DOMAIN_MAIL_REC_MAIL_NAME,          //�ʼ���
	DOMAIN_MAIL_REC_MAIL_OVERTIMER,     //��ʱʱ��
	DOMAIN_MAIL_REC_MAIL_CONTEXT,		//����
	DOMAIN_MAIL_REC_REV_NATION,			//��������(0û������ 1����2��͢ 3�й�������)
};

enum 
{
	S2S_COMMAND_MAIL_ADD_MAIL, //����ʼ�

};


enum SystemMailSubMsg 
{

    MAIL_SUB_MSG_MAIL_COUNT     = 1, // ��ȡ�ʼ�������
    MAIL_SUB_MSG_RETRIEVE       = 2, // ��ȡ�ʼ��б�
    MAIL_SUB_MSG_DELETE         = 3, // ɾ��һ���ʼ�
    MAIL_SUB_MSG_DELETE_ALL     = 4, // ɾ��ȫ���ʼ�
    MAIL_SUB_MSG_PICK           = 5, // ��ȡһ���ʼ�
    MAIL_SUB_MSG_PICK_ALL       = 6, // ��ȡȫ���ʼ�
    MAIL_SUB_MSG_READ           = 7, // ��ȡ�ʼ�
    MAIL_SUB_MSG_UNREAD_MAIL    = 8, // ��δ���ʼ�
	
};

enum SystemMailResultCode
{
    MAIL_CODE_FAILED                    = 0, // ʧ��
    MAIL_CODE_SUCCESS                   = 1, // �ɹ�
    MAIL_CODE_ATTACHEMENT_UNPICKED      = 2, // ����δ��ȡ
    MAIL_CODE_INVALID_MAIL              = 3, // ��Ч�ʼ�
    MAIL_CODE_TOOL_BOX_INSUFFICIENCY    = 4, // �����ռ䲻��
    MAIL_CODE_INVALID_ATTACHMENT        = 5, // ��Ч�ĸ���
    MAIL_CODE_MAIL_NOT_FOUND            = 6, // �ʼ�������
    MAIL_CODE_SUCCESS_WITH_INFO         = 7, // �ɹ����ж�����Ϣ
};

enum SystemMailReadFlag
{
    MAIL_FLAG_UNREAD = 0, // δ���ʼ�
    MAIL_FLAG_READ   = 1, // �Ѷ��ʼ�
};

enum SystemMailPickFlag
{
    MAIL_FLAG_UNPICKED = 0, // ����δ��ȡ
    MAIL_FLAG_PICKED   = 1, // ��������ȡ
};

enum SystemMailAttachmentFlag
{
    MAIL_FLAG_NO_ATTACHMENT     = 0, // û�и���
    MAIL_FLAG_HAS_ATTACHMENT    = 1, // �и���
};

enum SystemMailInnerCode
{

};

#endif //__SystemMailDefine_H__
