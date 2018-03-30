#if !defined __RequestDefine_h__
#define __RequestDefine_h__

//����׶���Ӧ�ý׶�
typedef enum _RequestState
{
    REQUEST_STATE_REQ,
    REQUEST_STATE_ANS
} RequestState;

#define REQUEST_REC "request_rec"

enum Request_Rec
{
    REQUEST_REC_INDEX,      //��������
    REQUEST_REC_NAME,      //������������
    REQUEST_REC_TYPE,        //��������
    REQUEST_REC_TIME,        //����ʱ��
	REQUEST_REC_STATE,      //����״̬(�������)
    REQUEST_REC_JOB,        //ְҵ
    REQUEST_REC_BATTLE_ABILITY, //ս����
	REQUEST_REC_LEVEL,		//�ȼ�
	REQUEST_REC_SEX,		// �Ա�
};

typedef enum
{
    //������
	 REQUESTTYPE_NONE = 0,

    //������������
    REQUESTTYPE_JOIN_SINGLEPLAYER = 1,
    
    //����Է��������
    REQUESTTYPE_INVITE_SINGLEPLAYER = 2,

    // ����
    REQUESTTYPE_COUNT ,

} REQUESTTYPE;

#endif