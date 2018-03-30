//--------------------------------------------------------------------
// �ļ���:      LogModule.h
// ��  ��:      ��־ģ��
// ˵  ��:
// ��������:    2014��10��20��
// ��������:    2015��09��10��
// ������:       
// ������:       
//    :       
//--------------------------------------------------------------------
#ifndef __LOGMODULE_H__
#define __LOGMODULE_H__

#include "Fsgame/Define/header.h"
#include "FsGame/Define/LogDefine.h"

//#ifndef FSROOMLOGIC_EXPORTS

#include "log4cplus/logger.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/consoleappender.h"
#include "log4cplus/loggingmacros.h"
#include "log4cplus/layout.h"
#include "log4cplus/configurator.h"

//#endif
#include "FsGame/Analytics/IGameAnalyticsCollector.h"
#include "extension/FsGmCC/model/ServerInfo.h"





//��Ϸ�淨��־�ṹ
struct GamePlayerActionLog
{
	GamePlayerActionLog()
		:name(L""),
		nation(0),
		actionType(0),
		actionState(0),
	    comment(L"")
	{}
	std::wstring     name;                 //�ǳ�
	int              nation;               //����
	int              actionType;           //�淨���� [ LogDefine: LOG_GAME_ACTION_TYPE ]
	int              actionState;          //�淨״̬ [ LogDefine: LOG_GAME_ACTION_STATE] 
    std::wstring     comment;              //��չ�ֶ�:�ɲ���
}; 

//��ɫ��¼��־�ṹ
struct RoleLog
{
	RoleLog()
		:ip(""),
		loginTime(0),
		logoutTime(0),
		mac(""),
		osType(""),
		comment(L"")
	{}
	std::string     ip;              //��½IP (varchar)
	time_t          loginTime;       //����ʱ�� (datetime)
	time_t          logoutTime;      //����ʱ��  (datetime)
	std::string     mac;             //�ͻ���MAC (varchar)
	std::string     osType;          //�豸ϵͳ���� (varchar)
    std::wstring    comment;         //��չ�ֶ�:�ɲ���
};

//������־�ṹ
struct TaskLog
{
	TaskLog()
		:taskID(0),
		type(0),
		opt_type(0), 
		state(0),
		comment(L"")
	{}
	int	taskID;          // ����ID 
	int	type;            // ����
	int state;			 // ״̬
	int	opt_type;		 // �������ͣ�0������ 1��������
    std::wstring	comment;         // ��չ�ֶ�:�ɲ���
};

//������־�ṹ
struct MoneyLog
{
	MoneyLog()
		:capitalType(0),
		eventID(0),
		eventType(0),
		eventSerial(""),
		before(0),
		money(0),
		after(0),
        senderName(L""),
        senderUid(""),
		state(0),
		comment(L"")
	{}
	int             capitalType;     //�������� (int)
	int             eventID;         //�춯;�� [ LogDefine: LOG_CAPITAL_EVENT_TYPE ]
	int             eventType;       //�춯���� [ LogDefine: LOG_ACT_EVENT_TYPE ]
	std::string     eventSerial;     //�춯���к�
	__int64         before;          //�춯ǰ��Ǯ����
	__int64         money;           //�춯���
	__int64         after;           //�춯���Ǯ����
	std::wstring    senderName;      //��һ������ǳ�
	std::string     senderUid;       //��һ�����uid
	int             state;           //�춯״̬
    std::wstring    comment;         //��չ�ֶ�:�ɲ���
};

//��Ʒ��־�ṹ
struct ItemLog
{
	ItemLog()
		:eventID(0),
		eventType(0),
		eventSerial(""),
		itemID(""),
		itemUid(""),
		itemLifeTime(0),
		before(0),
		itemNum(0),
		after(0),
		colorLevel(-1),
		senderName(L""),
		senderUid(""),
		state(0),
		comment(L"")
	{}
	int             eventID;         //�춯;��ID [ LogDefine: LOG_ITEM_TYPE ]
	int             eventType;       //�춯����   [ LogDefine: LOG_ACT_EVENT_TYPE ]
	std::string     eventSerial;     //�춯���к�
	std::string     itemID;          //��ƷID
	std::string     itemUid;         //��Ʒuid
	int             itemLifeTime;    //��Ʒ��Чʱ��
	int             before;          //�춯ǰ��Ǯ����
	int             itemNum;         //��Ʒ�춯����
	int             after;           //�춯���Ǯ����)
	int             colorLevel;      //װ��Ʒ��(int)��ʼ��
	std::wstring    senderName;      //��һ������ǳ�
	std::string     senderUid;       //��һ�����uid
	int             state;           //�춯״̬
    std::wstring    comment;         //��չ�ֶ�:�ɲ���
};

//��ɫ�����ṹ
struct RoleUpgradeLog
{
	RoleUpgradeLog()
		:type(0),
		levelBefore(0),
		levelAfter(0),
		skillID(""),
		comment(L"")
	{}
	int             type;            //�������� (varchar)  LOG_ROLE_UPGRADE_TYPE
	int             levelBefore;     //����ǰ�ȼ� (int)
	int             levelAfter;      //������ȼ� (int)
	std::string     skillID;         //����ID (varchar)
    std::wstring    comment;         //��չ�ֶ�:�ɲ���
};

//�ͷ���־�ṹ
struct PunishLog
{
	PunishLog()
		:gmAccount(""),
		type(0),
		minutes(0),
		comment(L"")
	{}
	std::string     gmAccount;       //������ͨ��֤ (varchar)
	int             type;            //�ͷ����� (int)
	int             minutes;         //�ͷ�ʱ�� (int)
    std::wstring    comment;         //��չ�ֶ�:�ɲ���
};

//�����־�ṹ
struct GuildLog
{
	GuildLog()
		:account("0"),
	    playerName(L""),
	    guildName(L""),
		grade(0),
		byRoleName(L""),
		operateType(0),
        capitalType(0),
		capitalNum(0),
		donateNum(0),
		comment(L"")
	{}
	std::string     account;         //�����˻�
	std::wstring    playerName;      //�������ǳ�
	std::wstring    guildName;       //��������
	int             grade;           //����ȼ�
	std::wstring    byRoleName;      //�������˽�ɫ�� (varchar)
	int             operateType;     //�������� (int)
	int             capitalType;     //�������� (int) 
	int             capitalNum;      //�춯��ֵ (int)
	int             donateNum;       //�ﹱ 
    std::wstring    comment;         //��չ�ֶ�:�ɲ���
};

//BOSS��־�ṹ
struct BossLog
{
	BossLog()
		:sceneID(0),
		bossID(""),
		comment(L"")
	{}
	int             sceneID;         //������� (varchar)
	std::string     bossID;          //boss��� (varchar)
    std::wstring    comment;         //��չ�ֶ�:�ɲ���
};


//������־�ṹ
struct SceneLog
{
	SceneLog()
		:loginTime(0),
		logoutTime(0),
		comment(L"")
	{}
	int64_t         loginTime;       //���볡��ʱ��
    int64_t         logoutTime;      //�뿪����ʱ��
    std::wstring    comment;         //��չ�ֶ�:�ɲ���
};




//�������log
struct PetRideLog
{
	PetRideLog()
		:type(0),
		targetID(""),
		levelBefore(0),
		levelAfter(0),
		skillID(""),
		comment(L"")
	{}
	int             type;            //�������� [ LogDefine: LOG_PET_RIDE_TYPE]  
	std::string     targetID;        //�������ID
	int             levelBefore;     //����ǰ�ȼ�
	int             levelAfter;      //������ȼ�
	std::string     skillID;         //����ID
    std::wstring    comment;         //��չ�ֶ�:�ɲ���
};

// �������log
struct RideBaseLog 
{
	RideBaseLog()
		:opt_type(0), 
		config_id(""),
		cur_step(0),
		cur_star(0)
		{
		}

	int opt_type;
	std::string config_id;
	int cur_step;
	int cur_star;
};

// ��������log
struct RideFeedLog : public RideBaseLog
{
	RideFeedLog()
		: item_id(""),
		item_num(0),
		add_feed_value(0)
		{
		}

	std::string item_id;
	int item_num;
	int add_feed_value;
};

// ��������log
struct RideUpgradeLog : public RideBaseLog
{
	RideUpgradeLog()
	: last_step(0), 
	last_star(0)
	{
	}

	int last_step;
	int last_star;
};

// ���＼������log
struct RideSkillUpgradeLog : public RideBaseLog
{
	RideSkillUpgradeLog()
	:
	old_level(0),
	last_level(0)
	, capital_type(0)
	, capital_num(0)
	, item_num(0)
	{
	}

	std::string skill_id;
	int old_level;
	int last_level;
	int capital_type;
	int capital_num;
	std::string item_id;
	int item_num;
};

// ����Ƥ��log
struct RideSkinLog : public RideBaseLog
{
	RideSkinLog()
	: skin_id("")
	{
	}

	std::string skin_id;
};


// ��������log
struct PetActiveLog : RideBaseLog
{
	PetActiveLog()
	: cur_level(0), 
	color(0), 
	item_id(""), 
	item_num(0)
	{
	}

	int cur_level;
	int color;
	std::string item_id;
	int item_num;
};

// ��������log
struct PetUpgradeLog : RideBaseLog
{
	PetUpgradeLog()
	: cur_level(0),
	last_level(0),
	last_star(0),
	last_step(0), 
	capital_type(0), 
	capital_num(0), 
	item_id(""), 
	item_num(0)
	{
	}

	int cur_level;
	int last_level;
	int last_star;
	int last_step;
	int capital_type;
	int capital_num;
	std::string item_id;
	int item_num;
};

// ������������log
struct PetSkillUpgradeLog : RideBaseLog
{
	PetSkillUpgradeLog()
	: pet_level(0),
	cur_skill_lvl(0),
	last_skill_lvl(0),
	capital_type(0),
	capital_num(0),
	skill_point(0)
	{
	}

	std::string skill_id;
	int pet_level;
	int cur_skill_lvl;
	int last_skill_lvl;
	int capital_type;
	int capital_num;
	int skill_point;
};

// ����������־
struct PetAwakenLog
{
	int opt_type;
	std::string pet_id;
	std::string skills;

	PetAwakenLog() :
		opt_type(0)
	{
	}
};

// Ѻ����ʼlog
struct EscortStartLog 
{
	EscortStartLog()
	: opt_type(0), 
	escort_id(0),
	escort_work(0),
	start_time(0), 
	npc_id(""),
	color(0),
	capital_type(0),
	capital_num(0),
	item_id(""),
	item_num(0)
	{
	}

	int opt_type;
	int escort_id;
	int escort_work;
	int64_t start_time;
	std::string npc_id;
	int color;
	int capital_type;
	int capital_num;
	std::string item_id;
	int item_num;
};


// Ѻ������log
struct EscortEndLog
{
	EscortEndLog()
	: opt_type(0),	
	escort_id(0),
	escort_work(0), 
	start_time(0),
	color(0), 
	end_time(0), 
	result(0)
	{
	}

	int opt_type;
	int escort_work;
	int escort_id;
	int64_t start_time;
	std::string npc_id;
	int color;
	int64_t end_time;
	int result;

};

// �������log
struct TeamOfferLog
{
	TeamOfferLog()
	: opt_type(0),
	offer_num(0),
	offer_level(0),
	pass_time(0),
	offer_result(0)
	{
	}

	int opt_type;
	int offer_num;
	int pass_time;
	int offer_level;
	int offer_result;

};

//PVP�淨log
struct PVPLog
{
	PVPLog()
		:beChallenge(L""),
		beforeRank(0),
		afterRank(0),
        startTime(0),
		endTime(0),
		result(0),
		comment(L"")
	{}
	std::wstring    beChallenge;     //����ս���ǳ�
	int             beforeRank;      //��սǰ������
	int             afterRank;       //��ս������
	int64_t         startTime;       //��ʼʱ��
	int64_t         endTime;         //����ʱ��
	int             result;          //��ս���
    std::wstring    comment;         //��չ�ֶ�:�ɲ���
};

//PVP�淨log
struct RevengeArenaLog
{
	RevengeArenaLog()
		:wsEnemyName(L""),
		startTime(0),
		endTime(0),
		result(0)
	{}
	std::wstring    wsEnemyName;     //����ս���ǳ�
	int64_t         startTime;       //��ʼʱ��
	int64_t         endTime;         //����ʱ��
	int             result;          //��ս���
};


//����ϵͳlog
struct NationLog
{
	NationLog()
		:n_operate_type(0),
		params(""),
		comment(L"")
	{}
	int             n_operate_type;         //��������:[ LogDefine: LOG_NATION_TYPE]
	std::string     params;                 //�������Ͷ�Ӧ����:��Աְλ,���ͶƱ��(û������)
    std::wstring    comment;                //��չ�ֶ�:�ɲ���
	std::string     account;                //�˻�
	std::wstring    name;                   //�ǳ�
};


//�ܾ�log
struct SceretLog
{
	SceretLog()
		:sceretID(0),
		startTime(0),
		endTime(0),
		deadCount(0),
		result(0)
	{}
	int            sceretID;                //�ܾ�����ID
	int64_t        startTime;               //��ʼʱ��
	int64_t        endTime;                 //����ʱ��
	int            deadCount;               //��������
	int            result;                  //���: [ LogDefine: LOG_SCERET_RESULT_TYPE]
    std::wstring   comment;                 //��չ�ֶ�:�ɲ���
};


//�ƺ�log
struct TitleLog
{
	TitleLog()
		:titleID(0),
		operateType(0),
		comment(L"")
	{}
	int            titleID;                 //�ƺ�ID
	int            operateType;             //����: [ LogDefine: LOG_ACT_EVENT_TYPE]
	std::wstring   comment;                 //��չ�ֶ�:�ɲ���
};

//�齱log
struct ExtractAwardLog
{
	ExtractAwardLog()
		:extractType(0),
		params(""),
		awardLevel(0),
		comment(L"")
	{}
	int            extractType;             //����: [ LogDefine: LOG_EXTRACT_AWARD_TYPE]
	std::string    params;                  //��ϸ:����������д.��bossID��(û�пɲ���)
	int            awardLevel;              //�����ȼ�
	std::wstring   comment;                 //��չ�ֶ�:�ɲ���
};

//����log
struct TraceOrderLog
{
	TraceOrderLog()
		:customType(0),
		account(""),
		name(L""),
		strMoney(""),
		traceOrder(""),
		comment(L"")
	{}
	int            customType;              //�Զ�������: [ LogDefine: LOG_CUSTOM_TYPE]    
	std::string	   account;					// �˺�
	std::wstring   name;					// �ǳ�
	std::string    buyIndex;                //������Ʒ����
	std::string    strMoney;                //֧��Ǯ��
	std::string    traceOrder;              //��ϸ:����������д.��bossID��
	std::wstring   comment;                 //��չ�ֶ�:�ɲ���
};

//�齱log
struct MailLog
{
	MailLog()
		:account(""),
		name(L""),
		templateType(""),
		capitalParams(""),
		itemParams(""),
		comment(L"")
	{}
	std::string	   account;					// �˺�
	std::wstring   name;					// �ǳ�
	std::string    templateType;            //�ʼ�ģ������
	std::string    capitalParams;           //���Ҳ���
	std::string    itemParams;              //��Ʒ����
	std::wstring   comment;                 //��չ�ֶ�:�ɲ���
};

//װ����־�ṹ
struct EquipLog
{
	EquipLog()
	:operateType(0),
	equipPos(0),
	equipSlot(0),
	beforeGrade(0),
	afterGrade(0),
	sourceItemID(""),
	targetItemID(""),
	deductItemIDs(""),
	deductItemNums(""),
	capitalType(0),
	capitalNum(0),
	comment(L"")
	{}
	int             operateType;     //��������  (int)
	int             equipPos;        //װ����λ  (int)
	int             equipSlot;       //��λ��λ  (int)
	int             beforeGrade;     //ǿ����ȼ�  (int)
	int             afterGrade;      //ǿ����ȼ�  (int)
	std::string     sourceItemID;    //����ǰ��ƷID (varchar)
	std::string     targetItemID;    //�������ƷID (varchar)
	std::string     deductItemIDs;   //������Ʒ (varchar)    (������ŷָ�)
	std::string     deductItemNums;  //������Ʒ���� (varchar)  (����Ʒһһ��Ӧ)
	int             capitalType;     //��������  (int)
	int             capitalNum;      //���Ļ����� (int)
	std::wstring    comment;         //��չ�ֶ�:�ɲ���
};

// ʱװ��־�ṹ
struct FashionLog
{
	FashionLog()
	:operateType(0),
	fashion_id(""),
	unlock_items(""),
	unlock_capitals(""),
	comment(L"")
	{
	}

	int	operateType;     // ��������  (int)
	std::string	fashion_id;    // ʱװid (varchar)
	std::string	unlock_items;	// ������Ʒ
	std::string	unlock_capitals;  // ��������
	std::wstring comment;         // ��չ�ֶ�:�ɲ���
};

// ��־��������
struct LogBaseInfo
{
	std::string		strAccount;			// �˺�
	std::string		strUid;				// uid
	int				nLevel;				// �ȼ�
	int				nVipLv;				// Vip�ȼ�
	//int				nJob;				// ְҵ
};

// ������־�ṹ
struct MeridianLog
{
	MeridianLog()
	:operateType(0),
	stoneid(""),
	beforeGrade(0),
	afterGrade(0),
	beforeExp(0),
	afterExp(0),
	deductItemIDs(""),
	deductItemNums(""),
	capitalType(0),
	capitalNum(0)
	{}
	int             operateType;     //��������  (int)
	std::string     stoneid;         // �ʯid  (string)
	int             beforeGrade;     //ǿ��ǰ�ȼ�  (int)
	int             afterGrade;      //ǿ����ȼ�  (int)
	int             beforeExp;		 //ǿ��ǰ����  (int)
	int             afterExp;        //ǿ������  (int)
	std::string     deductItemIDs;   //������Ʒ (varchar)    (������ŷָ�)
	std::string     deductItemNums;  //������Ʒ���� (varchar)  (����Ʒһһ��Ӧ)
	int             capitalType;     //��������  (int)
	int             capitalNum;      //���Ļ����� (int)
	std::string		paklist;		 // ���԰��б� (varchar) (id1,id2,id3)
};

#ifdef ANALY_BY_USER
class SensorsAnalyticsCollector;
typedef SensorsAnalyticsCollector GameAnalytics;
#else
class SensorsAnalyticsCollectorEx;
typedef SensorsAnalyticsCollectorEx GameAnalytics;
#endif
class LogModule : public ILogicModule
{
public:
    virtual bool Init(IKernel* pKernel);
    virtual bool Shut(IKernel* pKernel);

	void SetSrvInfo(IKernel* pKernel, const SrvInfo& info);
public:
	// ���ÿͻ����ն���Ϣ
	void OnClientInfo( IKernel* pKernel, const PERSISTID& player, const char* ip, 
							const char* platform, 
							const char* clientver,
							const char* manufacturer,
							const char* deviceid,
							const char* model,
							const char* os,
							const char* osver,
							bool bwifi
							);
#ifdef ANALY_BY_USER
	// ע���û�
	void OnNewUser(
							const char* uid,		// �û�Ψһid (require)
							const char* anid,		// �û�ԭ����id(option)
							const char* nick,		// �ǳ�/�û���(require)
							int sex,				// �Ա�(option)
							__int64 bron,			// ��������(option)
							const char* ip,			// �ն�id(option)
							const char* platform,	// ע��ƽ̨(option)
							bool wifi,				// �Ƿ�ʹ��wifi(option)
							const char* manufacturer,	// ������(option)
							const char* devid,		// �ն��豸Ψһ��ʶ(option)
							const char* model,		// �ն�����(option)
							const char* os,			// �ն�ϵͳ(option)
							const char* os_ver		// �ն�ϵͳ�汾(option)
							);
	// ɾ���˺�
	void OnDelUser(const char* uid);
#endif
	// ��½�ɹ�
	void OnUserLogin(const char* uid,			// �û�Ψһid (require)
							const char* nick,		// �ǳ�/�û���(require)
							const char* ip,			// �ն�ip(option)
							const char* platform,	// ƽ̨(option)
							const char* manufacturer,	// ������(option)
							const char* devid,		// �ն��豸Ψһ��ʶ(option)
							const char* model,		// �ն�����(option)
							const char* os,			// �ն�ϵͳ(option)
							const char* os_ver,		// �ն�ϵͳ�汾(option)
							bool bReset = false) ;	// ���õ�½�¼�
	// �˳���½
	void OnUserLogout(const char* uid, int64_t timestamp = 0, bool bReset = false);
	// ������ɫ
	void OnCreateRole(IKernel* pKernel, const PERSISTID& player);
	// ��ɫ����
	void OnRoleEntry(IKernel* pKernel, const PERSISTID& player, bool bReset = false);
	// ��ɫ�˳� bReset: �Ƿ��������˳�
	void OnRoleExit(IKernel* pKernel, const PERSISTID& player, bool bReset = false);

	//��ɫ��־��¼��־
	bool SaveRoleLog(IKernel* pKernel, const PERSISTID& player, const RoleLog &log);

	//��Ϸ�淨�춯��־
	bool SaveGameActionLog(IKernel* pKernel, const PERSISTID& player, const GamePlayerActionLog &log);

    //������־
    bool SaveTaskLog(IKernel* pKernel, const PERSISTID& player, const TaskLog &log);

	//�����춯��־
	bool SaveMoneyLog(IKernel* pKernel, const PERSISTID& player, const MoneyLog &log);

	//��Ʒ�춯��־
	bool SaveItemLog(IKernel* pKernel, const PERSISTID& player, const ItemLog &log);
    
	//���������־
	bool SaveRoleUpgradeLog(IKernel* pKernel, const PERSISTID& player, const RoleUpgradeLog &log);

	//�����־
	//bool SaveGuildLog(IKernel* pKernel, const PERSISTID& player, const GuildLog &log);

	//��ɱBOSS��־
	bool SaveBossLog(IKernel* pKernel, const PERSISTID& player, const BossLog &log);

	//������־
	bool SaveSceneLog(IKernel* pKernel, const PERSISTID& player, const SceneLog &log);

    // ��¼�����־
    bool SavePlayerLog(IKernel* pKernel, const PERSISTID& player, const int logtype, const wchar_t* comment);

    // ��¼GM��־
    bool SaveGMLog(IKernel* pKernel, const PERSISTID& player,const wchar_t* GMContent, const wchar_t* comment = L"Use GM Command");

    // �ʼ�������־
	bool SaveMailAwardLog(IKernel* pKernel, const MailLog &log);
private:
	//��������
	static int OnCreate(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ��¼���������
	static int H_RecOnlineCount(IKernel* pKernel, const PERSISTID& creator, int slice);

public:
	// ��ֵ
	void OnPaying(IKernel* pKernel, const PERSISTID& player, const char* orderid, const char* itemid, int count, float price, bool first);
	// ��ֵ���
	void OnPayed(IKernel* pKernel, const PERSISTID& player, const char* orderid,
		float amount, int result, bool first, bool bIsReal,
		const char* platform,
		const char* os);
	// ��ֵ���
	void OnPayed( const char* roleid, const char* uid, int level, int vip,
		const char* orderid,
		float amount, float totalamount, float realamount, int paytimes, int result, bool first, bool bIsReal,
		const char* platform,
		const char* os);
	// ���Ӿ���
	void OnIncExp(IKernel* pKernel, const PERSISTID& player,
									int value,					// ����ֵ(require)
									int newValue,				// ���Ӻ��ֵ
									int origValue,				// ԭʼֵ
									int source					// ��Դ
									);
	// ����
	void OnLevelup(IKernel* pKernel, const PERSISTID& player, int level, int original);

	// �����Ϸ��
	void OnGainCoin(IKernel* pKernel, const PERSISTID& player,
		int coid,				// ��Ϸ��id(��/��/ͭ/����....)(require), ��Ϸ�ж���
		int64_t value,			// ����ֵ(require)
		int64_t newvalue,		// ��ǰ��ֵ(require)
		int64_t original,		// ԭʼֵ(require)
		int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
		int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
		const char* extra = NULL// ����˵��(option), ��event��һ��˵��
		);
	// ������Ϸ��
	void OnCostCoin(IKernel* pKernel, const PERSISTID& player,
		int coid,				// ��Ϸ��id(��/��/ͭ/����....)(require), ��Ϸ�ж���
		int64_t value,			// ����ֵ(require)
		int64_t newvalue,		// ��ǰ��ֵ
		int64_t original,		// ԭʼֵ(require)
		int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
		int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
		const char* extra = NULL// ����˵��(option), ��event��һ��˵��
		);

	// �����Ʒ
	void OnGainItem(IKernel* pKernel, const PERSISTID& player,
		const char* itemid,		// ��Ʒid(require)
		int color,				// ��ƷƷ��
		int num,				// �����ֵ(require)
		int count,				// ��ǰӵ����
		int orig,				// ԭ��ӵ����
		int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
		int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
		const char* extra = NULL// ����˵��(option), ��event��һ��˵��
		);
	// ʹ����Ʒ
	void OnCostItem(IKernel* pKernel, const PERSISTID& player,
		const char* itemid,		// ��Ʒid(require)
		int color,				// ��ƷƷ��
		int num,				// �����ֵ(require)
		int count,				// ��ǰӵ����
		int orig,				// ԭ��ӵ����
		int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
		int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
		const char* extra = NULL// ����˵��(option), ��event��һ��˵��
		);

	void OnSkipUpStartScene(IKernel* pKernel, const PERSISTID& player);
	// ����ĳ������(����˵���ܿ���)-----------------------------------------------------
	void OnVisitFunction(IKernel* pKernel, const PERSISTID& player,
			int module,				// ����ģ��(require), ��Ϸ�ж���(��ʲôģ��)
			int func,				// ������(require) ��Ϸ�ж���(����������funcһ��)
			bool result,			// ���(require) true:�ɹ� false: ʧ��
			int value,				// ����˵��(option), ��func��һ��˵��
			const char* extra		// ����˵��(option), ��func��һ��˵��
		);

	//----------------------------------------------------------------------
	// ���ѽ���
	//----------------------------------------------------------------------
	// ���Ѳ����¼�(��ӣ������...)
	void OnFriendEvent(IKernel* pKernel, const PERSISTID& player, const wchar_t* pFriendName, int opType);
	// �����ͻ�
	void OnFriendSendGift(IKernel* pKernel, const PERSISTID& player, const wchar_t* pFriendName, int nGift, int nIntLevel, int nIncIntimacy);
	// �����ջ�
	void OnFriendRecvGift(IKernel* pKernel, const PERSISTID& player, const wchar_t* pFriendName, int nGift, int nIntLevel, int nIncIntimacy);
	// ��������������ܶ�
	void OnFriendTeamFight(IKernel* pKernel, const PERSISTID& player, const wchar_t* pFriendName, int nIntLevel, int nIncIntimacy);
	// ����ֵ�����仯
	void OnFriendCharmChanged(IKernel* pKernel, const PERSISTID& player, int nCharm, int nTotalCharm, int nReason);
	// ����
	void OnSworn(IKernel*pKernel, const PERSISTID& player, const wchar_t* playerName, int isSameSex, int isAdd);

	//------------------------------------------------------------------------------
	// ������
	//------------------------------------------------------------------------------
	//void OnTextChat(IKernel* pKernel, const PERSISTID& player, int nType, const wchar_t* pTarget, const wchar_t* pContent);

	// ����
	//void OnTransaction(IKernel* pKernel, const PERSISTID& player, const wchar_t* pszTarget, const char* pszItem, int nColor, int nNum, int nPrice, int nTax, int nType);

	// ����/���(ʱ�䵥λ:s)
	void OnForbidSpeak(IKernel* pKernel, const PERSISTID& player, int64_t nInterval, int64_t nExpiredTime);
	void OnAllowSpeak(IKernel* pKernel, const PERSISTID& player);
	void OnBlockPlayer(IKernel* pKernel, const PERSISTID& player, int64_t nInterval, int64_t nExpiredTime);
	void OnBlockPlayer(IKernel* pKernel, const char* pszRoleId, int64_t nInterval, int64_t nExpiredTime);
	void OnResumePlayer(IKernel* pKernel, const PERSISTID& player);
	void OnResumePlayer(IKernel* pKernel, const char* pszRoleId);

	void FillCommonArgs(IKernel*pKernel, const PERSISTID&player, KVArgs& args);		
private:
	// ��ȡ��ҵĻ�������
	bool GetPlayerBaseInfo(IKernel *pKernel, const PERSISTID& player, LogBaseInfo& outBaseInfo);
public:
	static LogModule* m_pLogModule;
	
	GameAnalytics* GetGameAnaCollector()
	{
		return m_pAnaCollector;
	}
private:
    // ��ȡ��׼��ʽ����־ʱ��
    std::string GetLogTime(time_t slice = 0);

	GameAnalytics* m_pAnaCollector;

//#ifndef FSROOMLOGIC_EXPORTS
	log4cplus::Logger m_pChatLogger;
//#endif

	static time_t ms_nNextReportCount;	// �´α���ʱ��
};


#endif //__LOGMODULE_H__