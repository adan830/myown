#ifndef _BufferDefine_H_
#define _BufferDefine_H_

#define BUFFER_HEART_INTERVAL 500

// Buffer�׶ζ���
enum BUFFER_STAGE
{
    BUFFER_STAGE_NONE = 0,
    BUFFER_STAGE_BEGIN,         // ��ʼʹ�ü��ܣ�ʹ���ߣ�Ŀ������
    BUFFER_STAGE_EFFECT,        // ���Ч��������
    BUFFER_STAGE_BREAK,         // ��ϣ�ԭ��
    BUFFER_STAGE_FINISH,        // ������ԭ��
};

// Buffer���ԭ��
enum BUFFER_REMOVE
{
    BUFFER_REMOVE_NONE = 0,
    BUFFER_REMOVE_BREAK,         // ���
    BUFFER_REMOVE_REPLACE,       // �滻
    BUFFER_REMOVE_TIMEEND,       // ����
    BUFFER_REMOVE_CLEAR,         //���
    BUFFER_REMOVE_OFFLINE,       //�������
    BUFFER_REMOVE_DEAD,          //�������
    BUFFER_REMOVE_TIMEOVER,      //�����ﵽ
};

//BUFFER��Ч������
enum BUFFER_EFFECT_TYPE
{	
    BUFFER_EFFECTTYPE_0, //0:˲ʱBUFFER, ˲ʱbuff����ʹ��
    BUFFER_EFFECTTYPE_1, //1:��X������Ч
    BUFFER_EFFECTTYPE_2, //2:��X����Y���¼���Ч
	BUFFER_EFFECTTYPE_3, //3:��X������Ч,ÿY�뷢��һ��
    BUFFER_EFFECTTYPE_4, //4:������ʱ������Ч,�������Կ۵�0ʱ������ÿY�봥��һ��
	BUFFER_EFFECTTYPE_5, //5:������buff,�ڲ��߼��������ӻ�ɾ��buff ����:����

	BUFFER_EFFECTTYPE_MAX, //Buffer�������͵����ֵ
};

//BUFFER�ļ�������
enum
{
    BUFFER_TIMETYPE_ONLINE, //ֻ��������ʱ��
    BUFFER_TIMETYPE_ALLTIME,    //������ʵʱ�䣨��������ʱ�䣩
	BUFFER_TIMETYPE_MAX,		//����ʱ����ֵ
};

//Buffer�滻�ķ�ʽ(�滻������滻������й�)
enum EBufferReplaceType
{
	BUFFER_REPLACE_NO,//��
	BUFFER_REPLACE_DIRECT, //ֱ�Ӱ�������滻
	BUFFER_REPLACE_REFRESH_TIME,//ˢ��ʱ��
	BUFFER_REPLACE_NOT_REPLACE//���滻
};

//Buffer�¼���Ŀ������
enum EBufferTargetType
{
    BUFFERTARGE_INTO, //�������
    BUFFERTARGE_SELECT, //ѡ�ж���
    BUFFERTARGE_SELF,  //�Լ�
    BUFFERTARGE_SPRINGER, //�¼��Ĵ�����
    BUFFERTARGE_SENDER, //Buffer��ʩ����
    BUFFERTARGE_TARGET //������Ŀ�꣨�����¼����ã�
};

//buffer�¼�����
enum EBufferEventType
{
    BUFFEVENT_UNKNOW = 0,
    BUFFEVENT_ACTIVE = 1,               // 1. ����buff
    BUFFEVENT_DEACTIVE = 2,             // 2. �ر�buff
    BUFFEVENT_TIMER = 3,                // 3. ��ʱ����timer
    BUFFEVENT_BEDAMAGE = 4,             // 4. ������
    BUFFEVENT_BEFORE_BEDAMAGE = 5,      // 5. Ŀ���ܵ��˺�֮ǰ���¼�
	BUFFEVENT_BEFORE_HIT = 6,			// 6. ��������Ŀ��ǰ
	BUFFEVENT_GAINT_DEAD = 7,			// 7. ��������
	BUFFEVENT_BE_VA = 8,				// 8. ������
	BUFFEVENT_DAMAGE = 9,				// 9. �˺�Ŀ��

    BUFFEVENT_MAX
};

//buffer�¼�������Ӱ��
enum EBufferAffectType
{
	BUFFAFFECT_BENEFICIAL = 0,			// 0 ����BUFF
	BUFFAFFECT_DAMGAE	= 1,			// 1 �˺�BUFF
	BUFFAFFECT_HARM		= 2,			// 2 ����BUFF
	BUFFAFFECT_CONTROL	= 3,			// 3 ����BUFF

	BUFFAFFECT_MAX
};

// buff�������
enum BufferClear
{
	BUFFER_SWITCH_CLEAR,	    		// �г������
	BUFFER_OFFLINE_CLEAR,       		// �������
	BUFFER_DEAD_CLEAR,          		// �������
};

// buff��Դ����
enum BufferSrc
{
	BUFFER_NORMAL_SRC,					// ������
	BUFFER_OFFICIAL_SRC					// ����buff
};

// debuff����
enum DeBuffType
{
	NONE_DEBUFF,						// ����debuff
	DEBUFF_NORMAL,						// һ���debuff,����Ҫ����˥��
	DEBUFF_SILENT,						// ��Ĭ���debuff
	DEBUFF_VERTIGO,						// ѣ�����debuff
	DEBUFF_CANT_MOVE,					// �������debuff
	DEBUFF_DEC_SPEED,					// �������debuff
	DEBUFF_DEC_DEFEND,					// �������debuff

	MAX_DEBUFF_TYPE_NUM
};

#define BUFF_ADD_STATICS_REC "buff_add_rec"

// buffͳ�Ʊ�
enum BuffAddRecCols
{
	BASR_BUFF_ADD_TIME_COL,
	BASR_BUFF_TYPE_COL
};

#endif

