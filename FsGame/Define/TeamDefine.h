//--------------------------------------------------------------------
// �ļ���:	  TeamDefine.h
// �� ��:	��ӹ���ͷ�ļ�
// ˵ ��:		
// ��������:	2014��11��19��
// ������:		 
//    :	    
//--------------------------------------------------------------------
#ifndef _FSGAME_TEAM_DEFINE_H_
#define _FSGAME_TEAM_DEFINE_H_
#define FORBID_TEAM_SCENEID  701
// �����
#define TEAM_REC_NAME "team_rec"

//������������
#define TEAM_MAIN_REC_NAME "team_main_rec"

#define TEAM_SCENE_CONFIG "ini/SocialSystem/Team/TeamType.xml"
#define TEAM_ACTIVITY_SCENE_CONFIG "ini/SocialSystem/Team/ActivityScene.xml"
// ���������ж���
enum
{
    TEAM_MAIN_REC_COL_TEAM_ID,                  // ����ID ��TYPE��VTYPE_INT��
    TEAM_MAIN_REC_COL_TEAM_MEMBER_REC_NAME,		// ��Ա�����ƣ�TYPE��VTYPE_STRING��
    TEAM_MAIN_REC_COL_TEAM_CAPTAIN,             // �ӳ����ƣ�TYPE��VTYPE_WIDESTR��
    TEAM_MAIN_REC_COL_PASSWORD,                 // �������루TYPE��VTYPE_WIDESTR��
    TEAM_MAIN_REC_COL_TEAM_UNIQUE_ID,           // �����ΨһID��TYPE��VTYPE_STRING��
	TEAM_MAIN_REC_COL_TEAM_INFO,                // ������Ϣ��TYPE��VTYPE_WIDESTR��
	TEAM_MAIN_REC_COL_TEAM_STATE,				// ����״̬(ƥ��״̬)��TYPE��VTYPE_INT��
	TEAM_MAIN_REC_COL_TEAM_DIG_INFO,			// �����ڱ���Ϣ ��TYPE��VTYPE_STRING��
	TEAM_MAIN_REC_COL_TEAM_VIEW,				// ���鹫��״̬ ��TYPE��VTYPE_INT��
	TEAM_MAIN_REC_COL_TEAM_AUTO_JOIN,			// �����Զ����� (TYpe: VTYPE_INT)
	TEAM_MAIN_REC_COL_TEAM_OBJECT_INDEX,		// Ŀ������ID(TYpe: VTYPE_INT)
	TEAM_MAIN_REC_COL_TEAM_AUTO_MATCH,			// �Զ�ƥ��(TYpe: VTYPE_INT)
	TEAM_MAIN_REC_COL_TEAM_LIMIT_MIN_LEVEL,		// ��͵ȼ�
	TEAM_MAIN_REC_COL_TEAM_LIMIT_MAX_LEVEL,		// ��ߵȼ�
	TEAM_MAIN_REC_COL_TEAM_OFF_JOIN_TEAM,		// ��ֹ�������
    TEAM_MAIN_REC_COL_COUNT,
};

// �����Ա���ж���
enum
{
	TEAM_REC_COL_UID,           //��ԱUID
    TEAM_REC_COL_NAME,           //��Ա��
	TEAM_REC_COL_BGLEVEL,		 //�ȼ�
	TEAM_REC_COL_BGJOB,          //ְҵ
	TEAM_REC_COL_SEX,          //�Ա�
	TEAM_REC_COL_FIGHT,          //ս����
	TEAM_REC_COL_ISOFFLINE,		 //�ϴ�����ʱ��
	TEAM_REC_COL_TEAMWORK,		 //�ڶ�����ְλ      

    TEAM_REC_COL_SCENE,          //�������
    TEAM_REC_COL_BUFFERS,		 //buffer�б�
    TEAM_REC_COL_HP,             //��ǰ����ֵ
    TEAM_REC_COL_GUILDNAME,		 //������
    TEAM_REC_COL_MAXHP,          //�������ֵ
    TEAM_REC_COL_FOLLOW_STATE,   //����״̬
    TEAM_REC_COL_READY,			// ׼��״̬
	TEAM_REC_COL_CLONE_ID,		 //����id
	TEAM_REC_COL_ONLINE,		 //Online
	TEAM_REC_COL_POSITION_X,		 //x����
	TEAM_REC_COL_POSITION_Z,		 //z����
	TEAM_REC_COL_BE_KICK,		//�ܷ��߳�����(0�ǿ��ԣ�1�ǲ�����)
	TEAM_REC_COL_FIGHT_STATE,	 //ս��״̬	
	TEAM_REC_COL_VIP_LV,			//vip
    TEAM_REC_COL_COUNT,
};
enum TeamType
{
	TeamNone = 0,//��
	TeamSecret,//װ������
	TeamEscort,//Ѻ��
	TeamWorldBoss,//����boss
	TeamCamp,//��Ӫս
	TeamWanted,//����
	TeamBorder,//�߾�
	TeamGuild,//��֯����
	TeamBianstone,//�ʯ����
};
// ׼��״̬
enum TeamReadyStates
{
	TEAM_READY_STATE_NONE = 0,	// ��״̬
	TEAM_READY_STATE_READY,	// ׼��״̬
};

// ����ʰȡ���䷽ʽ����
enum
{
    TEAM_PICK_MODE_FREE,        // ����ʰȡ
    TEAM_PICK_MODE_RANDOM,      // �������
    TEAM_PICK_MODE_ROLL,        // Roll��ʰȡ
    TEAM_PICK_MODE_CAPTAIN,     // �ӳ�ʰȡ
    TEAM_PICK_MODE_NEED,        // ����ʰȡ
    TEAM_PICK_MODE_COUNT,
};

enum
{
	TEAM_STATE_IDLE = 0,	//������
	TEAM_STATE_GAMING = 1,	//��Ϸ��
};


enum 
{
	TEAM_PRIVATE = 0,  //˽�ж���
	TEAM_PUBLIC = 1,   //��������
};
enum 
{
	TEAM_AUTO_JOIN_CLOSE, //�Զ��������ر�
	TEAM_AUTO_JOIN_OPEN,  //�Զ���������

};

// �������Ͷ���

// ������ְλ
enum
{
    TYPE_TEAM_PLAYER,    // ��Ա
    TYPE_TEAM_CAPTAIN,   // �ӳ�
};

//�뿪����
enum LEAVE_TYPE
{
    LEAVE_TYPE_EXIT,      //�Լ����
    LEAVE_TYPE_EXPEL,	  //���ӳ��߳�
    LEAVE_TYPE_OFFLINE,	  //���ߣ������ɢ��
};

enum TEAM_MATCH
{
    TEAM_REQUEST_NO,       //������Ȩ��
    TEAM_REQUEST_YES,      //���Է�������
};


enum TEAM_AUTO_MATCH
{
	TEAM_AUTO_MATCH_OFF, // �ر�
	TEAM_AUTO_MATCH_ON, //��
	TEAM_AUTO_MATCH_BUSY,//��æ

};

enum
{
	TEAM_JOIN_TEAM_ON, //�ܹ��������
	TEAM_JOIN_TEAM_OFF, //���ܹ��������


};


enum TEAM_IS_KICK
{
	TEAM_IS_KICK_OFF = 0, //�ܱ���(Ĭ���ܱ���)
	TEAM_IS_KICK_ON = 1,  //������
};

// ���¶�Աλ�ÿ���
enum UpdatePosiSwitch
{
	UPDATE_POSI_ON = 1,	// ��
	UPDATE_POSI_OFF = 2,	// �ر�
};

// �ͻ��˶�����Ϣ
enum CUSTOM_MESSAGE_TEAM
{
    CLIENT_CUSTOMMSG_TEAM_CREATE = 1,					//��������
    CLIENT_CUSTOMMSG_TEAM_KICKOUT = 2,                  // �߳�����
    CLIENT_CUSTOMMSG_TEAM_DESTROY = 3,                  // ��ɢ����
    CLIENT_CUSTOMMSG_TEAM_LEAVE = 4,                    // �˳�����
    CLIENT_CUSTOMMSG_TEAM_CHANGE_CAPTAIN = 5,			//�ƽ��ӳ�
	CLIENT_CUSTOMMSG_TEAM_NEARBY = 6,				//��������
	CLIENT_CUSTOMMSG_TEAM_MEMBER_POSITION_UPDATE = 7,	//����ˢ�¶����Աλ��
    CLIENT_CUSTOMMSG_TEAM_CLEAR_APPLY = 8,              //�������
    CLIENT_CUSTOMMSG_TEAM_APPLY_LIST = 9,              //��ȡ�����б�
	CLIENT_CUSTOMMSG_TEAM_SET_VIEW = 10,			 //���ù���״̬
    CLIENT_CUSTOMMSG_TEAM_REFRESH = 11,              //֪ͨˢ���б�
	CLIENT_CUSTOMMSG_TEAM_SET_AUTO_JOIN = 12,		 //�Զ�����
	CLIENT_CUSTOMMSG_TEAM_GET_TEAM_INFO = 13,		  //������Ϣ
	CLIENT_CUSTOMMSG_TEAM_SET_TEAM_OBJECT_INDEX = 14,	 //���ö���Ŀ������
	CLIENT_CUSTOMMSG_TEAM_SET_TEAM_LIMIT_LEVEL = 15,	 //���ö������Ƶȼ�
	CLIENT_CUSTOMMSG_TEAM_ENTRY_SECRET_SCENE =  16,			//��������ؾ�
	CLIENT_CUSTOMMSG_TEAM_AUTO_MATCH =  17,			//�Զ�ƥ��

	CLIENT_CUSTOMMSG_TEAM_GET_TEAM_LIST =  18,			//��ȡ�����б�
	CLIENT_CUSTOMMSG_TEAM_MATCH_TEAM_INFO = 19,		//��ȡ�Զ�ƥ��������Ϣ
	CLIENT_CUSTOMMSG_TEAM_TRANSFER_CAPTAIN_SITE = 20,  //���Ͷӳ����ڳ���
	CLIENT_CUSTOMMSG_TEAM_JOIN_TEAM_RESULT			= 21,  //�������(�ɹ�����)
	CLIENT_CUSTOMMSG_TEAM_REQ_ADD_TEAM = 22,		 //���������� int �������� string ���� int �ȼ� int ս�� int �Ա� int ְҵ
	CLIENT_CUSTOMMSG_TEAM_CONVENE = 23,				//�ټ�
	CLIENT_CUSTOMMSG_TEAM_FULL = 24,				//��������
	CLIENT_CUSTOMMSG_SET_TEAM_FOLLOW = 25,			//���ö������״̬

	// ����ʱ�޲���
	// ����ʱ
	/*!
	* @brief	�����Ƽ��б�
	* @param	int			����n
	����ѭ��n��
	* @param	wstring		����
	* @param	int			�ȼ�
	* @param	int			����
	* @param	int			ְҵ
	* @param	int			�Ա�
	* @param	int			ս����
	*/
	CLIENT_CUSTOMMSG_TEAM_REQUEST_RECOMMAND_LIST = 26,			//��ȡ����Ƽ��б�

};

enum TEAM_TYPE
{
	TEAM_TYPE_NONE  = 0,//������
	TEAM_TYPE_CLONE = 1,//����
};
//������
enum TEAM_CHECK_CONDITION_MSG
{
	COMMAND_TEAM_CHECK_REQ = 1, //�������
	COMMAND_TEAM_CHECK_RSP = 2, //���������
};
//�������
enum TEAM_CHECK_CONDITION_REQ
{
	
	COMMAND_TEAM_CHECK_NONE = 0,//������
	COMMAND_TEAM_CHECK_ENTRY_SECRET_CONDITION = 1,	//����ؾ����
	COMMAND_TEAM_CHECK_ESCORT = 2,	// ���Ѻ��
	COMMAND_TEAM_CHECK_TEAMOFFER = 5,	// �������
	COMMAND_TEAM_CHECK_GUILD_INBREAK = 8,//��֯�������
};
// ��ӷ���˶�����Ϣ
enum COMMAND_TEAM_SECOND_MSG
{
	// ��������
	COMMAND_TEAM_CREATE_MSG = 1,
	// ��ӳ�Ա
	COMMAND_TEAM_ADDMEM_MSG = 2,
	// �뿪����
	COMMAND_TEAM_LEAVE_MSG = 3,
	// �ƽ��ӳ�
	COMMAND_TEAM_CHGCAP_MSG = 4,
	// T������
	COMMAND_TEAM_KICKOUT_MSG = 5,
	// ��Աλ�ø���
	COMMAND_TEAM_UPDATE_MEMBER_POSI_MSG = 6, 
	//�������
	COMMAND_TEAM_TEAM_JOIN = 7,
	//�����Ա
	COMMAND_TEAM_TEAM_ADD_MUMBER = 8,

	//��ȡĿ������
	COMMAND_GET_TARGET_POS_REQ,
	//���Ŀ���Ӧ
	COMMAND_GET_TARE_POS_RSP,
	//��������б��е���
	COMMAND_DEL_PLAYER_APPLAY,

};

// ��������id�;���
struct NearbyTeamRecommend 
{
	NearbyTeamRecommend(): teamid(0), distance(0.0f){}
	int teamid;
	float distance;
	PERSISTID playid;
};

//������������
#define MAX_MEMBER_IN_NORMALTEAM 4

#define FOR_EACH_TEAM_MEMBER(pKernel, player) \
    CVarList teamList; \
    TeamModule::m_pTeamModule->GetTeamMemberList(pKernel,player,teamList); \
    for(int i=0; i<static_cast<int>(teamList.GetCount()); i++)




typedef bool(*CheckTeamCondition)(IKernel*pKernel, const PERSISTID& self, const IVarList &var);

typedef void(*CheckTeamResult)(IKernel*pKernel, const PERSISTID& self, const IVarList &var);

typedef bool(*MatchFinal)(IKernel*pKernel, const PERSISTID& self,int objID);

#endif // _FSGAME_TEAM_DEFINE_H_
