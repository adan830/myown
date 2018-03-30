
//--------------------------------------------------------------------
// �ļ���:      Server\FsGame\Define\skilleventmanagerdefine.h
// ��  ��:      
// ˵  ��:      
// ��������:    2014��4��25��
// ������:      ������
// ��Ȩ����:    ������ţ�������޹�˾
//--------------------------------------------------------------------



#ifndef __SKILLEVENTMANAGERDEFINE_H__
#define __SKILLEVENTMANAGERDEFINE_H__


// �¼����Ͷ��嶨��
enum ESkillEventType
{
    UNKNOW_TYPE = 0,

    // ���ܼ������ͷ�
    SKILL_BEGIN = 1,
    // ���ܿ�ʼ׼�����¼�
    SKILL_PREPARE = 2,
    // ����׼��֮����¼�
    SKILL_AFTER_PREPARE = 3,
    // ��������֮ǰ���¼�
    SKILL_BEFORE_HIT = 4,
    // �������в����˺��˶�����¼�
    SKILL_HIT_DAMAGE = 5,
    // ����ʹ�ý������¼�
    SKILL_FINISH = 6,
    // ���ܱ���ϵ��¼�
    SKILL_BREAK = 7,

    // ��������Ŀ��󣬿�Ѫ֮ǰ�������¼�
    SKILL_AFTER_HIT = 8,

    MAX_EVENT_TYPE = 30,
};


// ʹ�ö������Ͷ���
enum ETargetType
{
    // �������
    TARGET_SEND = 0,
    // ѡ�����
    TARGET_SELECT_OBJ = 1,
    // �������
    TARGET_SELF_OBJ = 2,
    // ��������
    TARGET_SPRING_OBJ = 3,
    //
    TARGET_END = 4,
};

enum EParamsType
{
    PARAM_EVENT_SKILL, //�����¼�����
    PARAM_EVENT_BUFF,  //buff�¼�����
};


enum EParamType
{
    EPARAMTYPE_SKILL, // �����¼��ļ��ܶ���
    EPARAMTYPE_TARGET, // Ŀ�����
    EPARAMTYPE_TARGET_XYZ, //Ŀ���
    EPARAMTYPE_TARGET_HURT, // �˺�����
    EPARAMTYPE_BULLET,  // �ӵ�����
};

#endif // __SKILLEVENTMANAGERDEFINE_H__

