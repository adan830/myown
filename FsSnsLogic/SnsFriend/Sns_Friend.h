//--------------------------------------------------------------------
// �ļ���:      SnsFriend.h
// ��  ��:      ������Ϣ�洢
// ˵  ��:
// ��������:    2014��4��12��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef Sns_Friend_h__
#define Sns_Friend_h__


#include "server/ISnsKernel.h"
//#include "server/ISnsCallee.h"
//#include "server/IRecord.h"
#include "../ISnsLogic.h"

#define SNS_FRIEND_MAXROW	100
#define SNS_FRIEND_GIFT_MAXROW	1024
#define SNS_FRIEND_DEL_SWORN_MAXROW 20 
#define SNS_FRIEND_ENEMY_MAXROW 50
#define SNS_FRIEND_APPLY_RECORD         "FriendApplyRecord"
#define SNS_FRIEND_DELETE_RECORD        "FriendDeleteRecord"
#define SNS_FRIEND_RESULT_RECORD        "FriendApplyResultRecord"
#define SNS_FRIEND_GIFT_RECORD			"FriendGiftRecord"
#define SNS_FRIEND_ADD_INITMACY			"FriendAddInitmacy"
#define SNS_FRIEND_DEL_SWORN            "FriendDelSworn"
#define SNS_FRIEND_ADD_ENEMY			"FriendEnemy"

enum FriendApplyRecCols
{
    SNS_FRIEND_APPLY_COL_SENDER_UID = 0,
    SNS_FRIEND_APPLY_COL_SENDER_NAME,
    SNS_FRIEND_APPLY_COL_RECEIVER_UID,
    SNS_FRIEND_APPLY_COL_RECEIVER_NAME,
    SNS_FRIEND_APPLY_COL_LEVEL,
	SNS_FRIEND_APPLY_COL_JOB,
	SNS_FRIEND_APPLY_COL_SEX,
    SNS_FRIEND_APPLY_COL_FIGHT_CAPACITY,
	SNS_FRIEND_APPLY_COL_GUILD_NAME,
    SNS_FRIEND_APPLY_COL_TYPE,
	SNS_FRIEND_APPLY_COL_VIP_LEVEL,
};

enum FriendDeleteRecCols
{
	SNS_FRIEND_DELETE_COL_SENDER_UID = 0,
	SNS_FRIEND_DELETE_COL_SENDER_NAME,
	SNS_FRIEND_DELETE_COL_RECEIVER_UID,
	SNS_FRIEND_DELETE_COL_RECEIVER_NAME,
};

enum FriendResultRecCols
{
    SNS_FRIEND_RESULT_COL_SENDER_UID = 0,
    SNS_FRIEND_RESULT_COL_SENDER_NAME,
    SNS_FRIEND_RESULT_COL_RECEIVER_UID,
    SNS_FRIEND_RESULT_COL_RECEIVER_NAME,
    SNS_FRIEND_RESULT_COL_LEVEL,
	SNS_FRIEND_RESULT_COL_JOB,
	SNS_FRIEND_RESULT_COL_SEX,
    SNS_FRIEND_RESULT_COL_FIGHT_CAPACITY,
	SNS_FRIEND_RESULT_COL_GUILD_NAME,
    SNS_FRIEND_RESULT_COL_RESULT,
};

enum FriendGiftRecCols
{
	SNS_FRIEND_GIFT_COL_SENDER_UID = 0,
	SNS_FRIEND_GIFT_COL_GIFT_COUNT,
};

enum FriendAddInitmacyRecCols
{
	SNS_FRIEND_ADD_INTIMACY_COL_SENDER_UID = 0,
	SNS_FRIEND_ADD_INTIMACY_COL_VALUE = 1,
};

enum FriendDelSwornCols
{
	SNS_FRIEND_DEL_SWORN_COL_NAME = 0,
};

enum FriendEnemyCols
{
	SNS_FRIEND_ENEMY_COL_UID = 0,
	SNS_FRIEND_ENEMY_COL_NAME,
	SNS_FRIEND_ENEMY_COL_LEVEL,
	SNS_FRIEND_ENEMY_COL_JOB,
	SNS_FRIEND_ENEMY_COL_SEX,
	SNS_FRIEND_ENEMY_COL_ABILITY,
	SNS_FRIEND_ENEMY_COL_GUILDNAME,
	SNS_FRIEND_ENEMY_COL_ACCOUNT,
	SNS_FRIEND_ENEMY_COL_HATER, //  ���ֵ
	SNS_FRIEND_ENEMY_COL_TIMES, //  ʱ��
	SNS_FRIEND_ENEMY_COL_VIP_LV, //  vip lv

	SNS_FRIEND_ENEMY_COL_MAX,


};

class SnsFriend : public ISnsLogic
{
public:

    SnsFriend();
    ~SnsFriend();

	// �߼����󱻴���
	int OnModuleCreate(ISnsKernel* pKernel, const IVarList& args);

	// SNS���������е�SNS�����Ѿ��������׼������(�����ɴ�OnSnsLoad֮��)
	int OnReady(ISnsKernel* pKernel, const IVarList& args);

	// SNS�������ڴ��д���
	int OnCreate(ISnsKernel* pKernel, const char* uid, const IVarList& args);

	// ����SNS����
	int OnLoad(ISnsKernel* pKernel, const char* uid, const IVarList& args);

	//�յ�����member����Ϣ
	int OnMessage(ISnsKernel* pKernel, int distribute_id, int server_id,
		int memeber_id, int scene_id, const char* uid, const IVarList& args);

private:

    // �������߼Ӻ���
    void FriendApply(ISnsKernel *pKernel, const IVarList &msg);

	// ���������ѹ�ϵ
	void FriendDelete(ISnsKernel *pKernel, const IVarList &msg);

	// ��ȡ��������
	void QueryApply(ISnsKernel *pKernel,const char *uid, 
							int server_id, int memeber_id, int scene_id, const IVarList &msg);

	// ��ȡ�����ϵ����
	void QueryDelete(ISnsKernel *pKernel, const char *uid, 
							int server_id, int memeber_id, int scene_id, const IVarList &msg);

    // ����������
    void ApplyResult(ISnsKernel *pKernel, const IVarList &msg);

    // ��ȡ������
    void QueryResult(ISnsKernel *pKernel, const char *uid, 
							int server_id, int memeber_id, int scene_id, const IVarList &msg);

	// �����ͻ�
	void FriendGiftGive(ISnsKernel *pKernel, const char *uid,
		int server_id, int memeber_id, int scene_id, const IVarList &msg);
	// ��ȡ�����ͻ�
	void FriendGiftGet(ISnsKernel *pKernel, const char *uid,
		int server_id, int memeber_id, int scene_id, const IVarList &msg);

	// �����������ܶ�
	void AddIntimacy(ISnsKernel *pKernel, const char *uid, const IVarList &msg);
	//��ȡ�������ܶ�
	void QueryAddIntimacy(ISnsKernel *pKernel, const char *uid,
		int server_id, int memeber_id, int scene_id, const IVarList &msg);
	void AddDelSworn(ISnsKernel *pKernel, const char *uid,
		int server_id, int memeber_id, int scene_id, const IVarList &msg);
	//��ȡɾ�������б�
	void QueryDelSworn(ISnsKernel *pKernel, const char *uid,
		int server_id, int memeber_id, int scene_id, const IVarList &msg);
	//���ӵ���
	void AddEnemy(ISnsKernel *pKernel, const char *uid,
		int server_id, int memeber_id, int scene_id, const IVarList &msg);
	void QueryEnemy(ISnsKernel *pKernel, const char *uid,
		int server_id, int memeber_id, int scene_id, const IVarList &msg);


private:

    // ����������ѱ�
    void CreateFriendRecords(ISnsKernel* pPubKernel, const char *uid);

    // ����һ��SNS��
    void _create_record(ISnsKernel *pKernel, ISnsData *pSnsData, const char *recName, const IVarList &coltype, int rows);
};
#endif // Sns_Friend_h__
