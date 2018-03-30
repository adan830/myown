//--------------------------------------------------------------------
// �ļ���:      AIDefine.h
// ��  ��:      AIϵͳ
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef AIDefine_h__
#define AIDefine_h__
#include <map>
#include <vector>
#include <string>

using namespace std;

/***********************************AIģ���ִ����ض���BEGIN***************************************/

//����AIģ�����ͱ�Ŷ���
enum AI_TEMPLATE_TYPE
{
    AI_TEMP_BASE = 0,       //����ģ��
    AI_TEMP_NORMAL_FIGHT,   //��ͨС��
    AI_TEMP_BOSS,	        //BOSS�������ܣ��ͷŹ���
    AI_TEMP_CONVOY,         //����
    AI_TEMP_INTERCEPT,      //����
    AI_TEMP_PATROLONLY_NPC,	//Ѳ��
    AI_TEMP_BATTLELEAD,		//��ս����ӳ�(������ң�
    AI_TEMP_BATTLENPC,		//��ս��Ա��������ң�
    AI_TEMP_INTERCEPTPLAYER,//����NPC��������ң�
    AI_TEMP_OUTLINEPLAYER,	//�һ���֣�������ң�
	AI_TEMP_MYSTICALBOX,	//���ر���				��û�õ���
    AI_TEMP_NATION_GUARD,	//������������
    AI_TEMP_ARENA_NPC,      //������NPC    
    AI_TEMP_PET,	        //����NPC
    AI_TEMP_PATROLMONSTER,  //Ѳ��Ұ��NPC
	AI_TEMP_SECTOR_SPRING_RANGE,	//���ξ����
	AI_TEMP_SCERET_GUARD,	//�ؾ��ػ�npc
	AI_TEMP_SCERET_PROJECT_OBJ,//�ܱ���Ŀ��
	AI_TEMP_ESCAPE_NPC,      //����npc
	AI_TEMP_FIGHT_PATROL_NPC,//Ѳ��Ұ��npc�ܹ�ս��
	AI_TEMP_BATTLE_BUFF_NPC,// ��Ӫսbuffer Npc
	AI_TEMP_GVG_DOGFACE,	// ������սС��
	AI_TEMP_DEAD_GEN_TRAP,	// ������������
	AI_TEMP_ESCORT,         // Ѻ��
	AI_TEMP_START_SCENE_BOSS,//���鳡��boss
	AI_TEMP_FRO_TOWER,		 // �߾�����
	AI_TEMP_CONTROL_HP_BOSS, //����Ѫ��ֹͣboss
	AI_TEMP_INVADE_BOSS,	//��Ӫ����boss
    AI_TEMP_MAX,
};


//��������AIִ�й������Ͷ���
enum AI_RULE_TYPE
{
    AI_RULE_SPRING = 0,      //����
    AI_RULE_PATROL,          //ִ��Ѳ�߹���
    AI_RULE_SEL_TARTGET,     //ѡ��Ŀ��
    AI_RULE_BEGIN_FIGHT,     //��ʼս��
    AI_RULE_FIGHT,           //ս��
    AI_RULE_BE_DAMAGED,      //���˺�
    AI_RULE_END_FIGHT,       //����ս��
    AI_RULE_ONDEAD,		     //����
    AI_RULE_CHASE_TARGET,    //׷����������
    AI_RULE_CHECK_ATTACKERS, //��鹥���߱�
    AI_RULE_RELIVE,          //����
    AI_RULE_END_PATROL,      //ֹͣѲ��
    AI_RULE_BACK_BORN_POS,   //�س�����
    AI_RULE_BORN,            //����
    AI_RULE_FOLLOW,          //����
    AI_RULE_RECUPERATE,     //������ԭ��
	AI_RULE_DESTORY,        //����
	AI_RULE_ESCAPE,         //����

    AI_RULE_MAX,
};

//���������µ��ӹ������Ͷ���
enum
{
    AIRS_SPRING_BASE = 0,        //��������
    AIRS_SPRING_INTERCEPT,       //���ػ���NPC
    AIRS_SPRING_BATTLENPC,		 //��սNPC
    AIRS_SPRING_OUTLINEPLAYER,	 //������ҹһ�
    AIRS_SPRING_NATION_GUARD,    //������������ս��������
	AIRS_SPRING_SECTOR_SPRING,	 //������Ӧ��npc
	AIRS_SPRING_DEFEND_TOWER,    //��������������
	AIRS_SPRING_GVG,             //���ս
    AIRS_SPRING_MAX
};
//��������µ��ӹ������Ͷ���
enum
{
    AIRS_FOLLOW_PET,       //����
    AIRS_FOLLOW_MAX
};
//Ѳ�߹����µ��ӹ������Ͷ���
enum 
{
    AIRS_PATROL_BASE = 0,        //��������
    AIRS_PATROL_CONVOY,          //����
    AIRS_PATROL_BATTLENPC,		 //��սNPC
    AIRS_PATROL_PET,			//����
	ATRS_PATROL_SECTOR,			//������Ӧncp
	AIRS_PATROL_GVG,			//gvg
    AIRS_PATROL_MAX
};
//ѡ��Ŀ������µ��ӹ������Ͷ���
enum 
{
    AIRS_SEL_TARGET_BASE = 0,     //��������
    AIRS_SEL_TARGET_BATTLENPC,	  //��սNPC
	AIRS_SEL_TARGET_TOWER,	      //����������Ŀ�����
	AIRS_SEL_GVG,	              //gvg���ս
    AIRS_SEL_TARGET_MAX
};
//��ʼս�������µ��ӹ������Ͷ���
enum 
{
    AIRS_BEGIN_FIGHT_BASE = 0,    //��������
    AIRS_BEGIN_FIGHT_BATTLENPC,	  //��սNPC
    AIRS_BEGIN_FIGHT_MAX,
};
//����AI���������µ��ӹ������Ͷ���
enum 
{
    AIRS_FIGHT_BASE = 0,         //��������
    AIRS_FIGHT_BATTLENPC,		 //��սNPC
    AIRS_FIGHT_PET,
	AIRS_FIGHT_STARTSCENEBOSS,	// ���鳡��boss
	AIRS_FIGHT_CONTROL_BOSS,	// Ѫ��boss����
    AIRS_FIGHT_MAX
};
//���˺�ʱ�Ĺ��������µ��ӹ������Ͷ���
enum 
{
    AIRS_BE_DAMAGED_BASE = 0,            //��������
    AIRS_BE_DAMAGED_CONVOY,				 //����
    AIRS_BE_DAMAGED_INTERCEPT,			 //����
    AIRS_BE_DAMAGED_MAX
};
//����ս��AI���������µ��ӹ������Ͷ���
enum 
{
    AIRS_END_FIGHT_BASE = 0,              //��������
    AIRS_END_FIGHT_BATTLENPC = 1,		  //��սNPC����ս��
    AIRS_END_FIGHT_INTERCEPT,			//����
	AIRS_END_FIGHT_GVG,			        //GVG
    AIRS_END_FIGHT_MAX
};
//��ɱ�����������µ��ӹ������Ͷ���
enum 
{
    AIRS_ONDEAD_BASE = 0,               //��������
    AIRS_ONDEAD_CONVOY,                 //����
    AIRS_ONDEAD_BATTLENPC,				//��սNPC
	AIRS_ONDEAD_MYSTICALBOX,			//����
	AIRS_ONDEAD_GVG,					//GVG
	AIRS_ONDEAD_GEN_TRAP,				// ������������
	AIRS_ONDEAD_ESCORT,                 //Ѻ��
	AIRS_ONDEAD_INVADE,                 //����boss
    AIRS_ONDEAD_MAX
};
//׷��������Ai���������µ��ӹ������Ͷ���
enum 
{
    AIRS_CHASE_TARGET_BASE = 0,
	AIRS_CHASE_TARGET_TOWER,			// ����׷������
    AIRS_CHASE_TARGET_MAX
};
//�����ѡ����Ŀ���AI���������µ��ӹ������Ͷ���
enum 
{
    AIRS_CHECK_ATTACKERS_BASE = 0,
	AIRS_CHECK_ATTACKERS_BATTLENPC,
    AIRS_CHECK_ATTACKERS_MAX
};
//������������µ��ӹ������Ͷ���
enum
{
    AIRS_RELIVE_BATTLENPC = 0,
    AIRS_RELIVE_MAX
};

//�������������µ��ӹ������Ͷ���
enum
{
    AIRS_RECUPERATE_BOSS = 0,
    AIRS_RECUPERATE_MAX
};

//���ٹ��������µ��ӹ������Ͷ���
enum
{
	AIRS_DESTROY_BATTLE_LEAD_NPC = 0,
	AIRS_DESTROY_MAX
};

// ���ܹ��������µ��ӹ������Ͷ���
enum
{
	AIRS_ESCAPE_BASE = 0,
	AIRS_ESCAPE_MAX
};

//AI�����ؽ��
enum
{
	AI_RT_NULL = 0, //��ֵ
	AI_RT_SUCCESS,  //�ɹ�
	AI_RT_IGNORE,   //���ԣ�������
	AI_RT_END_FIGHT,//����ս��
	AI_RT_TRUE ,    //������
	AI_RT_FALSE,    //���ؼ�
	AI_RT_OVER,     //���״̬
	AI_RT_FAIL = -1, //ʧ��
};

/***********************************AIģ���ִ����ض���END***************************************/
//���������ݱ��е�λ��
enum
{
	AI_ATTACKER_REC_OBJECT = 0,//��������
	AI_ATTACKER_REC_DAMAGE,    //�ܳ��ֵ
	AI_ATTACKER_REC_LASTTIME,  //�������ʱ��
	AI_ATTACKER_REC_FIRSTTIME,	//��ʼ����ʱ��
	
	AI_ATTACKER_REC_COL_NUM,
};

//AIѲ��ģʽ
enum
{
	AI_PATROL_MODE_ROUND = 0, //0,������ģʽ
	AI_PATROL_MODE_BACK,      //1,����
	AI_PATROL_MODE_RANDOM,    //2,���
	AI_PATROL_MODE_MAX
};

//AIѲ�ߵ��
enum
{
	AI_PATROL_REC_POS_X = 0,  //x,y,z
	AI_PATROL_REC_POS_Z,
	AI_PATROL_REC_POS_TIME,   //ͣ��ʱ��
};

//�����߶���Ϊ�ı�־  
//����ʶ����ʲô״̬�·�����ƶ���Ϊ  MoveState
enum
{
	MOVE_PATROL = 0,
	MOVE_FIGHT,
	MOVE_BACK_BORN,
};

enum 
{
	AI_RANGE_OK,    //����
	AI_RANGE_NEAR,  //̫��
	AI_RANGE_OVER   //̫Զ
};

//�����ж�
enum 
{
	BACTMSG_MOTION_SUCCEED, //�ɹ�����
	BACTMSG_MOTION_FAIL		//����ʧ��
};

//����״̬
enum 
{
	//��ʼ
	AI_BATTLETEAM_STATUS_BORN = 0,
	//�ƶ�
	AI_BATTLETEAM_STATUS_MOVE = 1,
	//ս��
	AI_BATTLETEAM_STATUS_FIGHT = 2,
};

//С�ӱ�ṹ
enum
{
	AI_BATTLEPLAYER_OBJECT = 0, //����
	AI_BATTLEPLAYER_STATUS = 1, //״̬
};

//С�ӳ�Ա״̬
enum
{
	//��ʼ
	AI_BATTLEPLAYER_STATUS_BORN = 0,
	//�ƶ�
	AI_BATTLEPLAYER_STATUS_MOVE = 1,
	//ս��
	AI_BATTLEPLAYER_STATUS_FIGHT = 2,
	//��׷
	AI_BATTLEPLAYER_STATUS_BACK = 3,
};

//��ս����ṹ
enum
{
	AI_BATTLETEAM_SERIAL = 0, //���
	AI_BATTLETEAM_LEADER, //�ӳ�
	AI_BATTLETEAM_STATUS, //״̬
	AI_BATTLETEAM_MOVETYPE, //�ƶ�����
	AI_BATTLETEAM_NEXT_POSX, //�¸�X
	AI_BATTLETEAM_NEXT_POSZ, //�¸�Z
	AI_BATTLETEAM_FIGHT_POSX, //������X
	AI_BATTLETEAM_FIGHT_POSZ, //������Z
};

// ������������
enum
{
	CREATE_TYPE_BATTLELEAD_NPC = 0,	//С�Ӷӳ�
	CREATE_TYPE_BATTLE_NPC,			//��Ա
	CREATE_TYPE_SKILL_NPC,			//����NPC
	CREATE_TYPE_PROPERTY_VALUE,	    //������ֵ�������� Key-Valueֵ�Է�ʽ��ֻ֧���滻��
};

//����������Ͷ���
enum AI_DROP_ASCRIPTION
{
    AI_DROP_FIRST_ATTACKER = 0, //��һ�ι���
    AI_DROP_DAMAGE_MAX,			//���˺����
    AI_DROP_LAST_ATTACKER,		//���һ�ι���
};

//���ر����������
enum
{
	MYSTICALBOX_BOSS = 1,	//Boss
	MYSTICALBOX_ITEM,		//��Ʒ
	MYSTICALBOX_SNARE,		//����
	MYSTICALBOX_OWNER,		//��Ʒֻ��������
};

//NPC����
enum NPC_TYPE:int
{
    NPC_TYPE_ORDINARY = 1,  //��ͨ��
    NPC_TYPE_ELITE,         //��Ӣ
    NPC_TYPE_BOSS,          //BOSS

    NPC_TYPE_BOX = 10,     //����

	NPC_TYPE_BUILDING = 100, // ����npc
	NPC_TYPE_CONVOY = 122, //����npc
	NPC_TYPE_FUNCTION = 200,  // ����npc

	NPC_TYPE_FUNCTION_FORCE_VISUAL = 300 // ǿ�ƿɼ��ľ�̬npc
};

// ����״̬
enum ConvoyStates
{
	CONVOY_STATE_DOING,	// ������
	CONVOY_STATE_END,	// ����
};

#define NPC_SKILL_CONFIG "ini/npc/npc_config/npc_skill.xml"
#define NPC_SKILL_PACKAGE_CONFIG "ini/npc/npc_config/npc_skill_pak.xml"
#define CONVOY_PATH_CONFIG "ini/npc/npc_config/npc_convoy_path.xml"
#define CONVOY_INTERCEPT_CONFIG "ini/npc/npc_config/npc_convoy_intercept.xml"
#define	PATROL_PATH_CONFIG "ini/npc/npc_config/npc_patrol_path.xml"

#define BATTLE_NPC_CONFIG "ini/npc/npc_config/nation_battle/battle_npc_config.xml"
#define BATTLE_NPC_MOVETYPE_CONFIG "ini/npc/npc_config/nation_battle/battle_npc_movetype.xml"
#define BATTLE_TEAM_CONFIG "ini/npc/npc_config/nation_battle/battle_team_config.xml"
#define BATTLE_TEAM_FORMATION_CONFIG "ini/npc/npc_config/nation_battle/battle_formation.xml"

#define BOX_WEIGHT_CONFIG "ini/npc/npc_config/npc_box.xml"
#define GUILD_DEFEND_PATH_CONFIG "ini/npc/npc_config/npc_guild_defend_path.xml"
#define GUILD_DEFEND_NPC_PACK_KEY "ini/ModifyPackage/guild_defend_npc_pack_key.xml"
#define NPC_LEVEL_PACKAGE_FILE "ini/npc/npc_config/npc_level_package.xml"

#define AI_PARTOL_COUNT 4  //����·�������
#define AI_PARTOL_RANDOM_SPACE 1  //����·�������

#define NPC_DROP_INFO_EXP    "npc_drop_info_exp"          //����ֵ����[@0:num]
#define NPC_DROP_INFO_ITEM   "npc_drop_info_item"         //��Ʒ��[@0:config] [@1:num]
#define NPC_DROP_INFO_MONEY  "npc_drop_info_money"		  //��Ǯ����[@0:num]

// NPC���ܱ�
#define NPC_SKILL_REC "boss_skill_rec"
//NPC���ܱ�
enum
{
    BOSS_SKILL_ID = 0,            //���ܱ��
    BOSS_SKILL_VALUE_CUR,         //��ǰȨֵ
};
// ���＼��ʹ��Ȩֵ�ṹ
struct  NpcSkill
{   
    NpcSkill() : skillID(""), maxWeighting(0), addWeighting(0), prior(0), condition(0), time(0), initWeighting(0){}
    //������
	std::string skillID;
    //�ͷ�Ȩֵ
	int maxWeighting;
    //ÿ�ι�������Ȩֵ
	int addWeighting;
    //���ȼ�
	int prior;
    //С�ڵ�Ѫ���ٷֱȲ��ͷ�
	int condition;
    //ÿ׷��һ������Ȩֵ
    int time;
    //��ʼȨֵ
    int initWeighting;
};

//NPC���ܰ�
struct NpcSkillPackage
{
	NpcSkillPackage(): id(""), normalSkill(""), randCount(0)
	{
		skillVec.clear();
	}
    std::string id;     //��ID
    std::string normalSkill;    //NPC��ͨ��������
	std::string bornskill;		// ����������ʹ�õļ���
    int randCount;  //������Ӹ���
    std::vector<std::string> skillVec;   //������ܿ�
};

//����NPC�ṹ
struct Intercept
{
	Intercept(): id(0), npcId(""), num(0), monsterX(0.0f), monsterZ(0.0f){}
	int id;
	std::string npcId;
	int num;
	float monsterX;
	float monsterZ;
};

//Ѱ·NPC�ṹ
struct PatrolPos 
{
	PatrolPos(): PosX(0.0f), PosZ(0.0f), iRestTime(0){}
	float PosX;
	float PosZ;
	int iRestTime;
};

//·��������ṹ
struct BattlePos
{
	BattlePos(): fPosX(0.0f), fPosZ(0.0f), fOrient(0.0f){}
	float fPosX;
	float fPosZ;
	float fOrient;
};

//С��NPC�ṹ
struct BattleNpcStruct
{
	BattleNpcStruct(): BattleLeadNpc(""), BattleNpc("")
	{
		m_MapBattleNpc.clear();
	}
	std::string BattleLeadNpc;
	std::string BattleNpc;
	std::map<int, std::map<int, BattlePos>> m_MapBattleNpc;
};

//���߷�ʽ�ṹ
struct MoveTypePos
{
	MoveTypePos(): fPosX(0.0f), fPosZ(0.0f){}
	float fPosX;
	float fPosZ;
};

typedef std::map<int, std::map<int, std::vector<MoveTypePos>>> MoveTypeDef;

// С������
struct BattleTeam 
{
	BattleTeam() : nTeamId(0),
				nCamp(0),
				nMoveType(0),
				nBornPosIndex(0),
				nLifeTime(0)
	{

	}
	int		nTeamId;			// С��id
	int		nCamp;				// ���ط�
	int		nMoveType;			// AI��������
	int		nBornPosIndex;		// ����λ������
	int		nLifeTime;			// С�Ӵ���ʱ��
};

typedef std::vector<BattleTeam>	BattleTeamVec;

// ���ι���
struct BattleTeamFormation
{
	int scene_id;	// ����ID
	int row;
	int column;	

	BattleTeamFormation(int _id) :
		scene_id(_id),
		row(0),
		column(0)
	{
	}

	bool operator == (const BattleTeamFormation& formation) const
	{
		return formation.scene_id == scene_id;
	}
};
typedef std::vector<BattleTeamFormation> BattleTeamFormationVec;

//��������Ȩ������
struct DropWeight
{
	DropWeight()
		:iType(0),
		DropId(""),
		iWeightBegin(0),
		iWeightEnd(0)
	{}

	int iType;
	std::string DropId;
	int iWeightBegin;
	int iWeightEnd;
};
typedef std::vector<DropWeight> VEC_DROPWEIGHT;

//������伯��
struct BoxWeight
{
	BoxWeight(): configId(""), WeightSum(0)
	{
		vec_drop.clear();
	}
	std::string configId;
	int WeightSum;	//Ȩ���ܺ�
	VEC_DROPWEIGHT vec_drop;
};
typedef std::map<std::string, BoxWeight>	MAP_BOXWEIGHT;

// �ȼ����԰�
struct NpcLevelPackage
{
	NpcLevelPackage() : 
		n_package_id(0), 
		n_upper_level(0), 
		n_lower_level(0)
		{}

	std::string s_config_id;
	int n_package_id;
	int n_upper_level;
	int n_lower_level;
};
typedef std::vector<NpcLevelPackage> NpcLevelPackageVec;

//��ͨ��׷����ѡ�񹥻�Ŀ��˼��Ƶ��
const unsigned int AI_NORMAL_CHASE_RATE = 500;

#endif