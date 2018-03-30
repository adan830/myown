//--------------------------------------------------------------------
// �ļ���:		PubDefine.h
// ��  ��:		��Ϸ�������빫�����ݷ�����֮�䴫�ݵ���Ϣ����
// ˵  ��:		��Ϸ�������򹫹����ݷ�����������Ϣʱ��ǰ�����ֶ����ǣ�
//				fast_string pubspace, wstring pubdata, int msgid
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------
#ifndef __PubDefine_h__
#define __PubDefine_h__

//PubSpace����
const char* const PUBSPACE_DOMAIN = "domain";
const char* const PUBSPACE_GUILD = "guild";

//����PubSpace����
const wchar_t* const GUILD_LOGIC = L"Guild_logic";
const wchar_t* const DOMAIN_CHANNEL = L"Domain_Channel_%d";

const wchar_t* const DOMAIN_NATION = L"Domain_Nation";

const wchar_t* const DOMAIN_TEAM = L"Domain_Team";

const wchar_t* const DOMAIN_NATIONOFFICIAL = L"Domain_NationOfficial_%d";

const wchar_t* const DOMAIN_LOOK = L"Domain_Look_%d";

//�����������ݵ���ϢID��Χ��Domain��Guild����ϢID�ı�Ų����ڴ˷�Χ��
#define PUBDATA_MSG_ID_MIN      1000
#define PUBDATA_MSG_ID_MAX      1999

//��������PubData����Ϣ
enum
{
	//�޸����ԣ���ʽ��string pubspace, wstring pubdata, int msgid, string prop, var value, ...;
	PUBDATA_MSG_SET_PROP = 1000,
	//�������ԣ���ʽ��string pubspace, wstring pubdata, int msgid, string prop, int value, ...;
	PUBDATA_MSG_INC_PROP = 1001,
	//���ĳ��������ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, ...;
	PUBDATA_MSG_CLEAR_REC = 1100,
	//ɾ��ĳ�����ĳ�����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int row, ...;
	PUBDATA_MSG_REMOVE_REC_ROW = 1101,
	//���ĳ�����һ�����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, ...;
	PUBDATA_MSG_ADD_REC_ROW_VALUE = 1102,
	//����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int row, int col, var value, ...;
	PUBDATA_MSG_SET_REC_VALUE = 1103,
	//����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int row, int col, var value, ...;
	PUBDATA_MSG_INC_REC_VALUE = 1104,
	//���ݹؼ�ֵɾ��ĳ�����ĳ�����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int keycol, var keyvalue;
	PUBDATA_MSG_REMOVE_REC_ROW_BYKEY = 1105,
	//���ݹؽ�ֵ���ù����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int keycol, var keyvalue, int col, var value, ...;
	PUBDATA_MSG_SET_REC_VALUE_BYKEY = 1106,
	//���ݹؽ����ӹ����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int keycol, var keyvalue, int col, var value, ...;
	PUBDATA_MSG_INC_REC_VALUE_BYKEY = 1107,
	//�������ã������ʽ��string pubspace, wstring pubdata, int msgid
	PUBDATA_MSG_UPDATE_CONFIG = 1108,
	//�޸����ѭ������
	PUBDATA_MSG_MAX_CIRCULATE_COUNT = 1109,
	//����ĳ���������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int maxrow, int cols, ...;
	PUBDATA_MSG_ADD_REC = 1110,
};

// DOMAIN�����Ϣ���������빫�������ͨѶ��Ϣ��
enum //Scene->Pub
{
	//����Ѵ����ĳ�������ʽ��string "domain", wstring L"Domain_SceneList", int msgid, int sceneid, string scene_nameid
	SP_DOMAIN_MSG_SCENELIST_ADD_SCENE = 1,
	// ɾ�������ٵĳ�������ʽ��string "domain", wstring L"Domain_SceneList", int msgid, int sceneid, string scene_nameid
	SP_DOMAIN_MSG_SCENELIST_DEL_SCENE,
	//���ӻ����������쳣���ݣ���һ���쳣�ʽ��¼��
	SP_DOMAIN_MSG_CAPITAL_MONITOR,
	// ���ܿ��ص����� ��ʽ��int functionid, int state
	SP_DOMAIN_MSG_FUNCTION_STATE,
	// ͨ�����а����Ϣ, ����ϢID�� RankListDefine.h
	SP_DOMAIN_MSG_GENERAL_RANK_LIST,

	// public����׼������
	SP_SERVICE_READY_PUBLIC,		

	//// ������
	//�������������ߣ���ʽ��string "domain", wstring L"Domain_Team", int msgid, int team_id,wstring player_name
	SP_DOMAIN_MSG_TEAM_ON_RECOVER,
	//�������飬��ʽ��string "domain", wstring L"Domain_Team", int msgid, wstring captain_name, wstring member_name,int team_camp
	SP_DOMAIN_MSG_TEAM_CREATE,
	//��ɢ���飬��ʽ��string "domain", wstring L"Domain_Team", int msgid, int nTeamID, wstring player_name
	SP_DOMAIN_MSG_TEAM_DESTROY,
	//������飬��ʽ��string "domain", wstring L"Domain_Team", int msgid, int nTeamID, wstring player_name, IVarList values
	SP_DOMAIN_MSG_TEAM_JOIN,
	//�뿪���飬��ʽ��string "domain", wstring L"Domain_Team", int msgid, int nTeamID, wstring player_name, int reason(0�Լ��뿪��1���ӳ�����)
	SP_DOMAIN_MSG_TEAM_LEAVE,
	//���¶����Ա��Ϣ����ʽ��string "domain", wstring L"Domain_Team", int msgid, int nTeamID, wstring player_name, int col_1, IVar value_1, ..., int col_n, IVar value_n
	SP_DOMAIN_MSG_TEAM_UPDATE_MEMBER,
	//���¶�����Ϣ(�����ӳ���)����ʽ��string "domain", wstring L"Domain_Team", int msgid, int nTeamID, wstring player_name, int col, IVar value
	SP_DOMAIN_MSG_TEAM_UPDATE_INFO,
	//���ö�Աλ�ú�ְλ����ʽ��string "domain", wstring L"Domain_Team", int msgid, int team_id,int set_param,wstring player_name
	SP_DOMAIN_MSG_TEAM_SET_MEMBER,
	//���ӵ�ƥ���б�string "domain", wstring L"Domain_Team", int msgid,wstring player_name, int col_1, IVar value_1, ..., int col_n, IVar value_n 
	SP_DOMAIN_MSG_TEAM_ADD_AUTO_MATCH_LIST,
	//��ѯƥ�������Ϣ  "domain", wstring L"Domain_Team", int msgid
	SP_DOMAIN_MSG_TEAM_MATCH_INFO,
	//�������
	SP_DOMAIN_MSG_TEAM_PLAYER_STORE,
};

// ���������Ϣ���������빫�������ͨѶ��Ϣ��
enum
{
	///////////////////////////////////////////////////////////////////////////
	// scene_server -> pub_server
	///////////////////////////////////////////////////////////////////////////
	SP_GUILD_MSG_CREATE = 200,              // ���󴴽�����
	SP_GUILD_MSG_APPLY_JOIN,                // ������빫��
	SP_GUILD_MSG_ACCEPT_JOIN,               // ���ܼ��빫�����������
	SP_GUILD_MSG_REFUSE_JOIN,               // �ܾ����빫������
	SP_GUILD_MSG_MEMBER_ONLINE_STATE,       // �����Ա����״̬�ı�
	SP_GUILD_MSG_DELETE_JOIN_RECORD,        // ����ɾ����������Ҽ��빫�����һ����¼
	SP_GUILD_MSG_UPDATE_PROPS,              // ������³�Ա���ԣ��ȼ���ս������
	SP_GUILD_MSG_UPDATE_APPLY_PROPS,        // �������������е����ԣ��ȼ���ս������
	SP_GUILD_MSG_UPDATE_APPLY_ONLINE_STATE, // �����Ա����״̬�ı�
	SP_GUILD_MSG_CANCEL_APPLY_JOIN,         // ȡ�����빫������
	SP_GUILD_MSG_CHANGE_NOTICE,             // �޸Ĺ���
	SP_GUILD_MSG_QUIT,                      // �˳�����
	SP_GUILD_MSG_FIRE,                      // �߳�����
	SP_GUILD_MSG_DISMISS,                   // ��ɢ����
	SP_GUILD_MSG_DONATE,                    // �������
	SP_GUILD_MSG_PROMOTION,                 // ������ְ
	SP_GUILD_MSG_DEMOTION,                  // ���ήְ
	SP_GUILD_MSG_BUY_ITEM,                  // �����򹫻��̵���Ʒ���۳����˹���ֵ
	SP_GUILD_START_ONE_KEY_APPLY_JOIN,		//һ���������
	SP_GUILD_CHANGE_DECLARATION,			// �޸�����
	SP_GUILD_MSG_CAPTAIN_TRANSFER,          // �����ƽ�����
	SP_GUILD_MSG_CHAT_INFO,					// ���󱣴�������Ϣ
	SP_GUILD_RECORD_GUILD_LOG,				// ��¼�����־ int ��־���� ��־����
	SP_GUILD_SET_AUTO_AGREE_STATE,			//���ð���Զ�����
	SP_GUILD_AGREE_INVITE_JOIN_GUILD,		//ͬ��������빫��
	SP_GUILD_TIGGER_TIMER,					//������ʱ��
	SP_GUILD_DEC_GUILD_NUM,					//�۳������ʽ�
	SP_GUILD_ADD_GUILD_NUM,					//������֯��ֵ
	SP_GUILD_UNLOCK_SKILL,					//��֯���ܽ���
	SP_GUILD_BUILDING_LVUP_OPERATE,			//��������
	SP_GUILD_BUILDING_LEVEL_ADD_EXP,		//�������Ӿ���
	SP_GUILD_NUM_BUY,						//���򹫻���ֵ
	SP_GUILD_SKILL_LVUP_OPERATE,				//��ʼ�о�����

	///////////////////////////////////////////////////////////////////////////
	// pub_server -> scene_server  300~399
	///////////////////////////////////////////////////////////////////////////
	PS_GUILD_MSG_CREATE = 300,              // �����������ظ�
	PS_GUILD_MSG_APPLY_JOIN,                // ���빫��������
	PS_GUILD_MSG_ACCEPT_JOIN,               // ��������Ľ��
	PS_GUILD_MSG_REFUSE_JOIN,               // �ܾ��˼��빫�������
	PS_GUILD_MSG_CANCEL_APPLY,              // ȡ��������빫��
	PS_GUILD_MSG_CHANGE_NOTICE,             // �޸Ĺ�����
	PS_GUILD_MSG_QUIT,                      // �˳�����ظ�
	PS_GUILD_MSG_FIRE,                      // �߳�����ظ�
	PS_GUILD_MSG_DISMISS,                   // ��ɢ����ظ�
	PS_GUILD_MSG_DISMISS_PUBLIC,            // ��ɢ����ظ�
	PS_GUILD_MSG_GUILD_GONE,                // ֪ͨ��Ա�����ɢ
	PS_GUILD_MSG_DONATE,                    // ������׽��
	PS_GUILD_MSG_PROMOTION,                 // ������ְ
	PS_GUILD_MSG_DEMOTION,                  // ���ήְ
	PS_GUILD_MSG_GM_MODIFY_PROP,            // GM�޸Ĺ������ԵĽ��
	PS_GUILD_CHANGE_DECLARATION,        	// �޸Ĺ�����
	PS_GUILD_MSG_CAPTAIN_TRANSFER,          // �����ƽ�����
	PS_GUILD_CHANGE_GUILD_SHORT_NAME,		// ��֯����
	PS_GUILD_MSG_NEW_APPLY,					// �µ�����
	PS_GUILD_MSG_MODIFY_DEVOTE,             // ֪ͨ�޸��˸���ʣ�๱��ֵ
	PS_GUILD_MSG_BUY_ITEM,                  // ֪ͨ������Ʒ�۳�����ֵ���
	PS_GUILD_NUM_CHANGE,				    // ����ʽ�䶯
};

//DOMAIN�������´���Ϣ���� 400~499
enum //Pub->Scene
{
	//public����׼������
	PS_SERVICE_READY_PUBLIC = 400,
	//����������Ϣ
	PS_SCENE_DATA_MSG,

	//���ö����Աְλ����ʽ��string "domain", wstring L"Domain_Team", int msgid,wstring player_name
	PS_DOMAIN_SERVERMSG_TEAM_SET_MEMBERS_WORK,

	//�����ж����߳�Ա�����Ƿ�ɹ�����ʽ��string "domain", wstring L"Domain_Team", int msgid, wstring member_name,int nTeamId
	PS_DOMAIN_SERVERMSG_TEAM_SET_PLAYERONRECOVER,

	//���鴴������ʽ��string "domain", wstring L"Domain_Team", int msgid, wstring captain_name, wstring member_name, int nTeamID
	PS_DOMAIN_SERVERMSG_TEAM_CREATE,

	//�����ɢ����ʽ��string "domain", wstring L"Domain_Team", int msgid, wstring member_name
	PS_DOMAIN_SERVERMSG_TEAM_DESTROY,

	//��Ա������飬��ʽ��string "domain", wstring L"Domain_Team", int msgid, wstring player_name, int nTeamID(�����member_name��ɫ��Ҫ����Ϣ)
	PS_DOMAIN_SERVERMSG_TEAM_JOIN,

	//��Ա�뿪���飬��ʽ��string "domain", wstring L"Domain_Team", int msgid, wstring member_name, wstring player_name, int reason(0�Լ��뿪��1���ӳ�����)
	PS_DOMAIN_SERVERMSG_TEAM_LEAVE,

	//��Ա������Ϣ����ʽ��string "domain", wstring L"Domain_Team", int msgid, wstring member_name, wstring player_name, int col
	PS_DOMAIN_SERVERMSG_TEAM_UPDATE_MEMBER,

	//���������Ϣ����ʽ��string "domain", wstring L"Domain_Team", int msgid, wstring member_name, int col
	PS_DOMAIN_SERVERMSG_TEAM_UPDATE_INFO,

	//ˢ�¶����Ա��������ʽ��string "domain", wstring L"Domain_Team", int msgid
	PS_DOMAIN_SERVERMSG_TEAM_REFRSH_NUM,

	//֪ͨ�������ӳ�Ա  ��ʽ�� string "domain", wstring L"Domain_Team", int msgid, wstring member_name, wstring player_name, int nTeamID(�����member_name��ɫ��Ҫ����Ϣ)
	PS_DOMAIN_SERVERMSG_TEAM_ADD_MEMBER,
};

// ������ز������
enum
{
	GUILD_MSG_REQ_FAIL = 0,     // ����ʧ��
	GUILD_MSG_REQ_SUCC = 1,     // ����ɹ�
};

#endif // __PubDefine_h__
