#ifndef  __SKILLDEFINE_H__
#define  __SKILLDEFINE_H__

#include <cstring>
//////////////////////////////////////////////////////////////////////////
//
// ��Ҽ��������������
const static int MAX_PLAYER_SKILL_CAPACITY = 512;

// NPC���������������
const static int MAX_NPC_SKILL_CAPACITY = 32;

// ͨ�ûظ�HP�ļ���
const static char* const RESTORE_HP_SKILL_ID = "10000";

// �������ȼ�
const static int MAX_SKILL_LEVEL = 60;

// �����ͷ�ѡ������
enum ESkillSelectType
{
    SKILL_SELECT_TYPE_DEFAULT = 0,          // ˲������ѡ��
    SKILL_SELECT_TYPE_SELECT_OBJECT = 1,    // ����ѡȡ������Ŀ�꣩
	SKILL_SELECT_TYPE_POS = 2,				// ѡȡĳ��λ��
};

// �������÷�Χ����
enum ESkillHitRangeType
{
    SKILL_HIT_RANGE_TYPE_SELECT_OBJECT = 0,     // ����������������
    SKILL_HIT_RANGE_TYPE_SPHERE = 1,            // ���������η�Χ
    SKILL_HIT_RANGE_TYPE_CYLINDER = 2,          // ������Բ����Χ
    SKILL_HIT_RANGE_TYPE_RECTANGULAR = 3,       // �����ھ��η�Χ
    SKILL_HIT_RANGE_TYPE_POS_CYLINDER = 4,      // ����ĳ��λ�õ�Բ����Χ
};


// ������̲����ṹ
struct SkillCastRangeParams
{	
	SkillCastRangeParams(): fRangeDistMax(0.0f), fRangeDistMin(0.0f){}
    float fRangeDistMax;            // �����Ч����
    float fRangeDistMin;            // ��С��Ч����
};

// �������÷�Χ�����ṹ
struct SkillHitRangeParams
{
    SkillHitRangeParams()
    {
        memset((void*)this, 0, sizeof(SkillHitRangeParams));
    }

    ESkillHitRangeType eHitRangeType;    // �������÷�Χ����
    union SkillHitRangeUnion
    {
        struct HitRangeSphere // ���η�Χ
        {
            float fHitRangeSphereDistCentre;    // ���η�Χ-ʩ���߾��������ľ���
            float fHitRangeSphereDistMax;       // ���η�Χ-�����Ч����
            float fHitRangeSphereDistMin;       // ���η�Χ-��С��Ч����
            float fHitRangeSphereTurnAngle;     // ���η�Χ-������Чת��
            float fHitRangeSphereUpAngle;       // ���η�Χ-������Ч����
            float fHitRangeSphereOffsetOrient;  // ���η�Χ-˳ʱ��ת��
            float fHitRangeSphereBasepointOffsetOrient;// ���η�Χ-����˳ʱ��ת��
        } sphere;

        struct HitRangeCylinder // Բ����Χ
        {
            float fHitRangeCylinderDistCentre;          // Բ����Χ-ʩ���߾�Բ�������߾���
            float fHitRangeCylinderDistMax;             // Բ����Χ-�����Ч����
            float fHitRangeCylinderDistMin;             // Բ����Χ-��С��Ч����
            float fHitRangeCylinderTurnAngle;           // Բ����Χ-������Чת��
            float fHitRangeCylinderHeight;              // Բ����Χ-Բ����Ч�߶�
            float fHitRangeCylinderOffsetOrient;        // Բ����Χ-˳ʱ��ת��
            float fHitRangeCylinderBasepointOffsetOrient;// Բ����Χ-����˳ʱ��ת��
        } cylinder;

        struct HitRangeRectangular      // ���η�Χ
        {
            float fHitRangeRectangularDistCentre;   // ���η�Χ-ʩ���߾�������ľ���
            float fHitRangeRectangularLength;       // ���η�Χ-��Ч����
            float fHitRangeRectangularWidth;        // ���η�Χ-��Ч���
            float fHitRangeRectangularHeight;       // ���η�Χ-��Ч�߶�
            float fHitRangeRectangularOffsetOrient; // ���η�Χ-˳ʱ��ת��
            float fHitRangeRectangularBasepointOffsetOrient;// ���η�Χ-����˳ʱ��ת��
        } rectangular;

		struct HitRangePosCylinder // ��ĳ����ΪԲ�ĵ�Բ�η�Χ��
		{
			float fHitRangeRadius;					// Բ�İ뾶
		}poscylinder;
    } skillhitrangeunion ;
};


// �������ö����ϵ
enum EHitTargetRelationType
{
    HIT_TARGET_RELATION_TYPE_NONE = 0x00,
    HIT_TARGET_RELATION_TYPE_SELF = 0x01,       // ����������
    HIT_TARGET_RELATION_TYPE_FRIEND = 0x02,     // �������ѷ�
    HIT_TARGET_RELATION_TYPE_ENIMY = 0x04,      // �����ڵз�

    HIT_TARGET_RELATION_TYPE_ALL = 0xFF,        // ���������������
};

// �������ö�������
enum EHitTargetType
{
    HIT_TARGET_TYPE_NONE = 0x00,
    HIT_TARGET_TYPE_PLAYER = 0x01,              // ���
    HIT_TARGET_TYPE_MONSTER = 0x02,             // ����
    HIT_TARGET_TYPE_FUNCTION_NPC = 0x04,        // ����NPC
    HIT_TARGET_TYPE_TASK_NPC = 0x08,            // ����NPC
    HIT_TARGET_TYPE_BROKEN_NPC = 0x10,          // ����NPC
    HIT_TARGET_TYPE_PET_NPC = 0x20,             // ����NPC

    HIT_TARGET_TYPE_ALL = 0xFF,                 // ���������������
};

// ����CD����С�ٷֱ�
#define  PUBLIC_MIN_VALUE_CD_PERCENT  40

//////////////////////////////////////////////////////////////////////////


// ���ܽ׶ζ���
enum ESkillStageType
{
    SKILL_STAGE_TYPE_NONE = 0,
    SKILL_STAGE_TYPE_BEGIN,         // ��ʼʹ�ü��ܣ�ʹ���ߣ�Ŀ������
    SKILL_STAGE_TYPE_PREPARE,       // ����������ʱ��
    SKILL_STAGE_TYPE_LEAD,          // ����������ʱ��
    SKILL_STAGE_TYPE_EFFECT,        // ���Ч��������
    SKILL_STAGE_TYPE_BREAK,         // ��ϣ�ԭ��
    SKILL_STAGE_TYPE_FINISH,        // ������ԭ�� 	
	
    SKILL_STAGE_TYPE_MAX,
};

// ����ʹ��Ŀ�����Ͷ���
enum ESkillStageTargetType
{
    SKILL_STAGE_TARGET_TYPE_NONE = 0,			// û��Ŀ��
    SKILL_STAGE_TARGET_TYPE_TARGET,             // Ŀ����
    SKILL_STAGE_TARGET_TYPE_POSITION,           // λ��
};

// �������Ч�����Ͷ���
enum ESkillStageEffectType
{
    eSkillStageEffectType_None = 0,
    eSkillStageEffectType_DecHP,            // �۳�HP��hp
    eSkillStageEffectType_Miss,             // δ����:
    eSkillStageEffectType_Hits,             // ���д�����hits
    eSkillStageEffectType_AddHP,            // ҽ�ƣ�hp
    eSkillStageEffectType_VA,               // ������hp, times
	eSkillStageEffectType_Dodge,            // ���ܣ�
//     eSkillStageEffectType_PlayEffect,       // ����Ч��
// 	eSkillStageEffectType_BeHitAttack,		// ����
// 	eSkillStageEffectType_Gaint,            // ����

	eSkillStageEffectType_Max,
};

// �˺�����
enum DamageType
{
	NONE_DAMAGE_TYPE,						// �ǹ�������
	PHYS_DAMAGE_TYPE,						// �������˺�
	MAGIC_DAMAGE_TYPE,						// ħ�������˺�
	BOTH_DAMAGE_TYPE,						// ��ħ����

	MAX_DAMAGE_TYPE_NUM
};
//����ʹ�õ�Ŀ������
enum
{
    USESKILL_TYPE_TARGET,   //Ŀ�����
    USESKILL_TYPE_POSITION, //Ŀ��λ��
};

//����ʹ�ý��
enum EUseSkillErrorCode
{
	USESKILL_RESULT_ERR_OTHER = 0,			//��������

	USESKILL_RESULT_SUCCEED,				//�ͷųɹ�
	USESKILL_RESULT_ERR_FIND_PROPERTY,		//���Ҳ�������
	USESKILL_RESULT_NO_OBJECT,				//�ͷ��߲�����
	USESKILL_RESULT_ERR_NOSKILL,			//��������û���������
	USESKILL_RESULT_ERR_CANTBEATTACK,		//Ŀ�겻���ͷű�����
	USESKILL_RESULT_ERR_DEAD,               //���������ͷ�
	USESKILL_RESULT_ERR_COOLDOWN,			//��������ȴ��
	USESKILL_RESULT_ERR_MP,					//������

	USESKILL_RESULT_ERR_BUFFER_PROPETY,     //buffer���Բ�����
	USESKILL_RESULT_ERR_BUFFER_CONTAINER,   //buffer����������
	USESKILL_RESULT_ERR_BUFFER,				//buffer���󲻴���

	USESKILL_RESULT_ERR_HITING_BUSY ,		//�������м��ܣ�����ʹ����������
	USESKILL_RESULT_ERR_LEADING_BUSY,		//�����������ܣ�����ʹ����������
	USESKILL_RESULT_ERR_PREPARE_BUSY,		//����׼�����ܣ�����ʹ����������
	
	USESKILL_RESULT_ERR_CONFIG_DATA,		//�����������ô���
	USESKILL_RESULT_ERR_OTHERSKILL_ISHITING, //������������ʹ����
	USESKILL_RESULT_ERR_CANTUSESKILL,		 //�߼���ʹ�ü���ʧ��
	USESKILL_RESULT_ERR_PUBLICCD,			//������ȴ��ʹ�ü���ʧ��
	USESKILL_RESULT_ERR_STIFF_STATE,		// ���ڼ���Ӳֱ״̬
// 	USESKILL_RESULT_ERR_SILENT,				// �ڳ�Ĭ��
// 	USESKILL_RESULT_ERR_STORAGE_NUM,		// ������������
// 	USESKILL_RESULT_ERR_STIFF,				// Ӳֱ��

	USESKILL_RESULT_ERR_MAX,				//�������
};

//�ж�ĳ�������Ƿ���ĳ����Χ�ڵĹ��˽��

enum ETargetFilterResult
{
	TARGETFILTER_RESULT_ERR_OTHER = 0,

	TARGETFILTER_RESULT_SUCCEED,				  //�ɹ�
	TARGETFILTER_RESULT_ERR_TOONEAR,              //����̫��
	TARGETFILTER_RESULT_ERR_TOOFAR,               //����̫Զ
	TARGETFILTER_RESULT_ERR_HORI,                 //����ˮƽ�Ƕȷ�Χ
	TARGETFILTER_RESULT_ERR_VERT,                 //���ڴ�ֱ�Ƕȷ�Χ
	TARGETFILTER_RESULT_ERR_TARGETTYPE_REEOR,     //Ŀ���������ô���

};

enum ESkillBreakReasonType
{
    SKILL_BREAK_REASON_TYPE_NONE = 0,
	SKILL_BREAK_REASON_TYPE_DEAD,				  // ������ϼ���
    SKILL_BREAK_REASON_TYPE_CANTUSESKILL,         // CantUseSkill���Ա仯����������ͷŵļ���
    SKILL_BREAK_REASON_TYPE_NEW_SKILL,            // �����·�һ�����ܣ���ϵ�ǰ����
    SKILL_BREAK_REASON_TYPE_CLIENT		          // �ͻ���������ϼ���
};

const char* const CONST_SKILL_NPC_NORMAL_ATTACK = "Skill_Npc_Normal_Attack";
const char* const READY_RANDOM_SKILL = "ReadySkill";

//����Ч�����µ�Motion
#define  MOTION_MODE_HITBACK  0x0101

//ֻͬ�������Motion
#define  MOTION_MODE_ROTATE 0x0201

//����Ч����motion
#define  MOTION_MODE_PULL 0x0301

// �ͻ��˼���������Ϣ
enum CustomSkillSetupMsg
{
    // ��������
    C2S_ESKILL_UPGRADE_SKILL = 1,

	// һ�����༶
	C2S_ESKILL_UPGRADE_MULTI,

    // ���ܽ���
    S2C_ESKILL_SETUP_SKILL_UNLOCKED,

	// �����������óɹ�
	S2C_ESKILL_PASSIVE_SKILL_RESET_SUC,
};

// ����λ�ƶ�������
enum SkillDisplaceObject
{
	DISPLACE_SELF_TYPE,					// ʩ����λ��
	DISPLACE_TARGET_TYPE				// Ŀ��λ��
};

// ����λ������
enum SkillDisplaceType
{
	DISPLACE_NONE_TYPE = 0,				// ��Чֵ
	DISPLACE_USE_SKILL_MOVETO = 1,		// ʹ�ü��ܺ�MOVETOЧ��
	DISPLACE_USE_SKILL_MOTION = 2,		// ʹ�ü��ܺ�MOTIONЧ��
};

// ����λ�Ƶ�motion�ٶ�
#define SKILL_HITBACK_MOTION_SPEED 15.0f

// 0 CoolDownTime     1 PublicCDTime     2 LeadTime          3 HitTime 
// ����ʱ���
enum SkillTime
{
	COOL_DOWN_TIME_DIFF,			// CDʱ���
	PUBLIC_CD_TIME_DIFF,			// ����CDʱ���
	LEAD_TIME_DIFF,					// ����ʱ���
	HIT_TIME_DIFF,					// ����ʱ���

	MAX_SKILL_TIME_DIFF_NUM
};

// �ͻ�������ʧ������
enum ClientHitFailed
{
	VALID_TIME_FAILED,				// ��֤����ʱ��ʧ��
	VALID_HIT_INDEX_FAILED,			// ��֤��������ʧ��
	VALID_DISTANCE_FAILED			// ��֤����ʧ��
};

// ����Я����������
enum SkillForObject
{
	PLAYER_SKILL,					// ��Ҽ���
	PET_SKILL,						// ���＼��
	RIDE_SKILL,						// ���＼��
	NPC_SKILL						// npc����
};

// �ɳ���������
enum SkillGrowPropertyType
{
	SKILL_GP_ATTACK_ADD,			// ���ӹ�����
	SKILL_GP_COOL_DOWN_TIME,		// cd
	SKILL_GP_RESTORE_HP_RATE,		// ���ܻظ�HP����
	SKILL_GP_DAMAGE_RATE,			// �����˺�����

	MAX_SKILL_GP_NUM
};

// �ɳ�������Чֵ
#define NO_VALID_GROW_PROPERTY -1

// ������������
enum PassiveSkillEffect
{
	PS_ADD_PLAYER_PROPERTY,				// �����������
	PS_ADD_SKILL_PROPERTY,				// ���Ӽ�������("SkillAttackAdd","SkillDamageRate")
	PS_CHANGE_SKILL_CONFIG,				// �ı似����������(����:CD,leadtime��)
	PS_ADD_SKILL_EFFECT,				// ���Ӽ���Ч�� SkillEffect
	PS_ADD_NEW_BUFF,					// ������BUFF

	MAX_PASSIVE_SKILL_TYPE_NUM
};

// ��������������ñ�ʶ
const char* const ADD_PLAYER_PROPERTY_FLAG = "PlayerPropAdd";

// ���Ӽ��ܶ�������
const char* const ADD_SKILL_PROPERTY_FLAG = "SkillPropAdd";

// �ı似����������
const char* const CHANGE_SKILL_CONFIG_FLAG = "SkillConfigChange";

// ���Ӽ���Ч�� SkillEffect
const char* const ADD_SKILL_EFFECT_FLAG = "SkillEffectAdd";

// ������Buff
const char* const ADD_NEW_BUFF_FLAG = "AddNewBuff";

// ���������޸ĵ�������
const char* const SKILL_ATTACK_ADD = "SkillAttackAdd";			// ���ܹ�����
const char* const SKILL_DAMAGE_RATE = "SkillDamageRate";		// �����˺��ٷֱ�
const char* const SKILL_CD_TIME_NAME = "CoolDownTime";			// ��ȴʱ��
const char* const SKILL_LEAD_TIME_NAME = "LeadTime";			// ����ʱ��

// ��ұ������ܱ���
const char* const PASSIVE_SKILL_REC_NAME = "passive_skill_rec";

// ���ܵ�������
const char* const SKILL_POINT_NAME = "SkillPoint";

// ����������Դ
enum PassiveSkillSource
{
	PSS_BY_STUDY,					// ѧϰ��õı�������
	PSS_BY_PET,						// ͨ�������õı�������
 	//PSS_BY_ACUPOINT,				// ͨ��������Ѩ��õı�������
	PSS_BY_RIDE,					// ͨ��������׻�õı�������
	PSS_BY_GUILD,					// ��֯
	PSS_BY_SOUL,					// ͨ����ħ��õı�������
// 	PSS_BY_WING,					// ͨ����򱻶�����
// 	PSS_BY_EVOLVE,					// ͨ�����ܽ�����ñ�������
};

// �������ܱ�
enum PassiveSkillRec
{
	PS_COL_SKILL_ID,				// ��������id
	PS_COL_SKILL_LEVEL,				// �������ܵȼ�
	PS_COL_SKILL_SRC,				// ����������Դ

	MAX_PS_COL_NUM
};

// ʹ�ü���ʱ�Ĳٿ�״̬
// enum USOperationState
// {
// 	USO_NONE_OPERATION = 0,			// ���ɲٿ�
// 	USO_CHANGE_ORIENT,				// ��ת��ٿ�
// 	USO_CAN_MOTION					// ���ƶ��ٿ�
// };

// ��������
enum SkillType
{
	NONE_SKILL_TYPE = -1,
	USE_SKILL_TYPE,					// ��������
	PASSIVE_SKILL_TYPE,				// ��������
	INTERACT_SKILL_TYPE				// ��������
};

// ������������
enum PassiveSkillType
{
	NONE_PASSIVE_SKILL_TYPE = -1,
	PST_REN_MAI,					// ����
	PST_DU_MAI,						// ����
	PST_DAI_MAI						// ����
};

#endif //__SKILLDEFINE_H__