//------------------------------------------------------------------------------
// �ļ���:      TrapEventFuncIdDefine.h
// ��  ��:      ���崥���¼��ص�����ID����
// ˵  ��:
// ��������:    2015��07��13��
// ������:        
// ��ע:
//    :       
//------------------------------------------------------------------------------

#ifndef __TRAPEVENTFUNCIDDEFINE_H__
#define __TRAPEVENTFUNCIDDEFINE_H__

enum SpringEvent
{
	SPRINGEVENT_INVALID = 0,

// 	//�����˺�
// 	SPRINGEVENT_SINGLE_DAMAGE = 1,  
// 
// 	//Ⱥ���˺�
// 	SPRINGEVENT_MUTI_DAMAGE = 2,  

	//����BUFF
	SPRINGEVENT_SINGLE_BUFF,  

	//Ⱥ��BUFF
	SPRINGEVENT_MUTI_BUFF,

// 	//ɾ��Ŀ��BUFF
// 	SPRINGEVENT_DELETE_TARGET_BUFF,

	//��ձ�ɾ������Ŀ���buff
//	SPRINGEVENT_CLEAR_REC_TARGET_BUFF = 6,
	//����Ŀ��
	SPRINGEVENT_DESTORY_OBJ,

	SPRINGEVENT_MAX,
};

//�������˽��
enum SPRING_FILTER_RESULT
{
	SPRING_FILTER_RESULT_NULL = 0,            //��ֵ
	SPRING_FILTER_RESULT_SUCCESS,             //�ɹ�
	SPRING_FILTER_RESULT_IGNORE,              //���ԣ�������
};

//���ҹ���
enum SPRING_FILTER_NATION
{
	SPRING_FILTER_NATION_ALL            =  10,  //���еж�
    SPRING_FILTER_NATION_UNLESS_OWN     =  99,  //�������ж�
    SPRING_FILTER_NATION_UNLESS_ALLY    =  98,  //���˹��ж�
};

//�����������
enum SPRING_FILTER_TYPE
{
	SPRING_FILTER_TYPE_ALL              =  0,  //��������
	SPRING_FILTER_TYPE_ONLY_SCENE       =  1,  //����
	SPRING_FILTER_TYPE_ONLY_PLAYER      =  2,  //���
	SPRING_FILTER_TYPE_ONLY_NPC         =  4,  //NPC
	SPRING_FILTER_TYPE_ONLY_GOODS       =  8,  //��Ʒ
};

//�������ٹ���
enum SPRING_DESTROY
{
    SPRING_DESTROY_TIME_OUT             = 0,   //����ʱ������
	SPRING_DESTROY_IMMEDIATELY          = 1,   //����������
    SPRING_DESTROY_EFFECT_TIME_OUT      = 2,   //��Чʱ�������
};


#endif // __TRAPEVENTFUNCIDDEFINE_H__
