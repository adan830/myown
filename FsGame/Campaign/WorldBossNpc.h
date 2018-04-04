//--------------------------------------------------------------------
// �ļ���:      WorldBossNpc.h
// ��  ��:      ����Bossģ��
// ˵  ��:		
// ��������:    2015��6��23��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#ifndef __WorldBossNpc_H__
#define __WorldBossNpc_H__

#include "Fsgame/Define/header.h"
#include <vector>
#include <map>

#include "FsGame/SystemFunctionModule/RewardModule.h"
#include "CommonModule/CommRuleDefine.h"

class ChannelModule;
class RewardModule;
class DropModule;
class LandPosModule;
class CapitalModule;
class CoolDownModule;
class ActYearBoss;
class SnsPushModule;

// ����ʱ����
struct TimeInterval_t
{
	TimeInterval_t():
		m_BeginTime(0),
		m_EndTime(0){}
	int m_BeginTime;   // ��ʼʱ��(������)
	int m_EndTime;     // ����ʱ��(������)
};

// ����ʱ����
struct GMTimeInfo
{
	GMTimeInfo() :
		m_NoticeBeginTime(0),
		m_NoticeEndTime(0),
		m_ActiveBeginTime(0),
		m_ActiveEndTime(0)
	{

	}

	int m_NoticeBeginTime;   // ֪ͨ��ʼʱ��(������)
	int m_NoticeEndTime;     // ֪ͨ����ʱ��(������
	int m_ActiveBeginTime;   // ���ʼʱ��(������)
	int m_ActiveEndTime;     // �����ʱ��(������)
};

// ���������
struct WorldBossActive_t
{
	WorldBossActive_t():
		m_ID(0),
		m_SceneID(0),
		m_NoticeBeginTime(0), 
		m_NoticeEndTime(0),
		m_ActiveBeginTime(0), 
		m_ActiveEndTime(0), 
		m_PlayerLevel(0)
		{
			m_Weeks.clear();
		}
	int                        m_ID;                  // ID
	int                        m_SceneID;             // ����ID
	std::vector<int>           m_Weeks;               // ����
	int                        m_NoticeBeginTime;     // ���濪ʼʱ��(������)
	int                        m_NoticeEndTime;       // �������ʱ��(������)
	int                        m_ActiveBeginTime;     // ���ʼʱ��(������)
	int                        m_ActiveEndTime;       // �����ʱ��(������)
	std::string				   m_strBossID;           // BOSSId
	PosInfo					   m_BossPos;             // BOSS��������
	PosInfo					   m_PlayerPos;			 // ��Ҵ�������
	int                        m_PlayerLevel;         // ��Ҳμӻ��͵ȼ�
};

// AI�˺�ֵ���ݿ����ṹ
struct CopyAIHurt_t
{
	CopyAIHurt_t():
		m_player_name(L""), 
		m_damage(0){}

	std::wstring m_player_name; // �������
	int m_damage;				// �˺�ֵͳ��

	bool operator < (const CopyAIHurt_t& ch) const
	{
		return m_damage > ch.m_damage;
	}

	bool operator > (const CopyAIHurt_t& ch) const
	{
		return m_damage < ch.m_damage;
	}
};

// ������������
struct RankAward_t
{
	RankAward_t():
		m_MinRank(0),
		m_MaxRank(0),
		m_nWinRewardId(0),
		m_nFailRewardId(0){}
	int m_MinRank;             // ��С����
	int m_MaxRank;             // �������
	int m_nWinRewardId;        // ʤ������id
	int m_nFailRewardId;       // ʧ�ܽ���id
};

// ����
struct WorldBossAward_t
{
	WorldBossAward_t(): m_BossCfgID("")
		{
			m_vecRankAward.clear();
		}

	std::string                m_BossCfgID;      // boss ID
	int						   m_nLastHurtAward;  // �˺�����
	std::vector<RankAward_t>   m_vecRankAward;   // �������� 
};

// ���������
struct ConstConfig_t
{
	ConstConfig_t() : nEndRankNum(0),
					nReadyTime(0),
					nBornCGTime(0),
					nLeaveSceneDelay(0),
					nActiveRankNum(0)
	{

	}

	int nEndRankNum;				// ���������ʾ�����
	int nReadyTime;					// ׼��ʱ��
	int nBornCGTime;				// �����󲥷�CGʱ��
	int nLeaveSceneDelay;			// �������뿪�����ӳ�ʱ��
	int nActiveRankNum;				// ���ʵʱ��������
};

// buff������Ϣ
struct BuffConfig
{
	BuffConfig():
		index(0),
		buffId(""),
		buffPrice(0),
		moneyType(0),
		limitBuy(0),
		coolDownBuy(0),
		buyRatio(0.0),
		addFight(0.0){}

	int         index;             // buff�㼶
	std::string buffId;            // buff ID
	int         buffPrice;         // buff �۸�
	int         moneyType;         // ����Ǯ������
	int         limitBuy;          // ���ƹ������
	int         coolDownBuy;       // ��ȴʱ��
	double      buyRatio;          // �������
	double      addFight;          // ���ӹ����ٷֱ�
};


// ����boss�ɳ�����
struct WorldBossGrowUp 
{
	int			nSceneId;			// ˢboss�ĳ���
	int			nKillDurationTime;	// boss����������(��λ:s)(�ڴ�ʱ����ɱ��boss,��ȼ�����)
	int			nBasePackageId;		// ��ʼ����id
	int			nIncrement;			// ÿ�����ӵĵȼ�
	int			nMaxLevel;			// ���ȼ�
};

typedef std::vector<WorldBossGrowUp> WorldBossGrowUpVec;

class WorldBossNpc : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);
	// �ͷ�
	virtual bool Shut(IKernel* pKernel);

public:
	// ���ػ�����ļ�
	static bool LoadActiveInfoResource(IKernel* pKernel);
	// ���ؽ��������ļ�
	static bool LoadAwardResource(IKernel* pKernel);
	// ���ػ�����ļ�
	static bool LoadActiveRuleResource(IKernel* pKernel);
	// ��������boss�ɳ�����
	bool LoadWorldBossGrowUpConfig(IKernel* pKernel);

	// �ж�����boss�����Ƿ�ɽ���
	bool IsSceneEnter(IKernel* pKernel, const PERSISTID& self, int nSceneId);
public:
	// ��������
	static int OnCreateScene(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// ����NPC����
	static int OnBossBeKilled(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// �ͻ�����Ϣ����
	static int OnCustomMessage(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ��ҽ��볡��׼������
	static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ��Ҷ�������
	static int OnPlayerContinue(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ���������ȴ��ص�
	static int OnCustomReady(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// �ڲ�����ص�
	static int OnCommandMessage(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);
	
	// GM����
	static int OnCommandGMRestartActive(IKernel* pKernel, const PERSISTID& self, 
		const IVarList& args);

	// GM����
	static int OnCommandGMEndActive(IKernel* pKernel, const PERSISTID& self, 
		const IVarList& args);

	// ֪ͨ�����
	static int NoticeActiveFinish(IKernel* pKernel, const PERSISTID& self, 
		bool bossBeKilled);

	// ��ɫ���ݻָ�
	static int OnRecover(IKernel* pKernel, const PERSISTID& player,
		const PERSISTID& sender, const IVarList& args);

	// ��ҽ��볡����ص�
	static int OnPlayerEntryScence(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// ����BOSS���˺�
	static int WorldBossNpc::OnCommandBossBeDamaged(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);
public:
	// ���һ������
	static int ActiveLastDamageAward(IKernel* pKernel, const PERSISTID& boss, 
		const PERSISTID& killer);

	// ���뽱��
	//static int ActiveActorAward(IKernel* pKernel, const PERSISTID& CurrScene);

	// �������� nResult ActiveResult
	static int ActiveRankAward(IKernel* pKernel, const PERSISTID& CurrScene, int nResult);

	// ��Ʒ����
	static int DropItems(IKernel* pKernel,const PERSISTID& Attacker, 
		const PERSISTID& boss, const std::string& strItemList);

public:
	// �Ƿ������޻����
	static int QueryBossActiveType();

private:
	// ����BOSS
	static PERSISTID CreateWorldBoss(IKernel* pKernel, const WorldBossActive_t* pActiveInfo, const int& iCurrSceneID,  std::string& strBossPos, int iActiveTime, int iWeek);

	// ��BOSS���Ͽ����˺�����
	static bool CopyAndSortHurtRecordFromAI(IKernel* pKernel, 
		const PERSISTID& self, const PERSISTID& sender);

	// ���һ�ֻ����
	static bool ClearActiveData(IKernel* pKernel);

	// ��ѯ����boss�״̬
	void OnQueryWorldBossState(IKernel* pKernel, const PERSISTID& self);

	// �ظ�����boss�������ݲ�ѯ
	int OnQueryBaseInfo(IKernel* pKernel, const PERSISTID& self);
private:
	// ��ȡ��ǰ�������Ϣ
	static WorldBossActive_t* GetActiveInfoCfg(IKernel* pKernel, const int iID);

	// ��ȡ��ǰ�������Ϣ
	static WorldBossActive_t* GetActiveInfoCfgBySceneId(IKernel* pKernel, const int nSceneId);

	// ��������ҳ������
	static int GetActiveSceneByPlayer(IKernel* pKernel, const PERSISTID& self);

	// ��ȡ�����������Ϣָ��
	static WorldBossAward_t* GetBossAward(IKernel* pKernel, const char* strBossCfgID);

	// ���������ļ���ʱ���ַ���
	static int ParseTime(const char* timesStr);

	static void ParseBossID(const char* strBossIDs, std::map<int, std::string>& mapBossIDs);

	// ������ڼ�
	static void ParseWeek(const char* weeksStr,std::vector<int>& weeks);

	// ��ȡ��ǰʱ��״̬
	static int GetWorldBossActiveState(IKernel* pKernel, const PERSISTID& self, 
		const WorldBossActive_t* pActiveInfo, int iCurrSec);

	static int GetGMWorldBossActiveState(IKernel* pKernel, const PERSISTID& self, 
		const WorldBossActive_t* pActiveInfo, int iCurrSec);

	// ��ȡ�ʣ��ʱ��
	static int GetRemainTime(const WorldBossActive_t* pActiveInfo);

	// ��ѯ��ǰ�Ļ
	static int QueryCurActivity(IKernel* pKernel);

	// ��������ڵ�����npc
	static void ClearSceneNpc(IKernel* pKernel);

	static void ReloadConfig(IKernel* pKernel);
	//////////////////////////////////��������////////////////////////////////////////
public:
	// ��ȡ�ռ�
	static IPubData* GetPubData(IKernel * pKernel);

	// ��ȡ�ռ���
	static const std::wstring& GetDomainName(IKernel * pKernel);

	// ����Ϣ��Pub������
	static bool SendMsgToPubServer(IKernel* pKernel, const IVarList &msg);

	// ��ȡ�״̬
	static int GetActiveStatusFromPUB(IKernel* pKernel, int iSceneID);

	// ��֤�Ƿ�Ϊ����boss����
	static bool IsWorldBossScene(int iSceneID);
private:
	// ���û״̬
	static int OnSaveActiveStatusToPUB(IKernel* pKernel, 
		const int iSceneID, const int iStatus, const int iID);

	// ��ȡ�����ID
	static int GetActiveCfgIDFromPUB(IKernel* pKernel, int iSceneID);
	
	// �����ʱ����
	void AddSceneData(IKernel* pKernel, const PERSISTID& Scene);

	// member����ͬ��
	void SyncMemberData(IKernel* pKernel);

	//��㲥
	void ActiveNotice(IKernel* pKernel, const PERSISTID& Scene);

	// ���ʼ
	void ActiveOpening(IKernel* pKernel, const PERSISTID& Scene, const WorldBossActive_t* pActiveInfo, const tm* pNow, int iState, int nConfigIndex);

	// �����
	void ActiveClosed(IKernel* pKernel, const PERSISTID& Scene, int iState);
	//--------------------------------����----------------------------------//
private:
	// ��ʱ���ͻ����������Ϣ
	static int HB_CheckNotice(IKernel* pKernel, const PERSISTID& self, int slice);

	// ����Boss��������
	static int HB_CheckWorldBossActive(IKernel* pKernel, const PERSISTID& self, int slice);

	// ��������ӳ�����((BOSS����ɱ))
	static int HB_SendCustomMsg(IKernel* pKernel, const PERSISTID& self, int slice);

	// ��ʼ����CG
	static int HB_StartPlayCG(IKernel* pKernel, const PERSISTID& self, int slice);

	// ��ʼˢBoss
	static int HB_BossBorn(IKernel* pKernel, const PERSISTID& self, int slice);

	// ʵʱ��ʾ�˺��б�
	static int HB_ShowAttackRank(IKernel* pKernel, const PERSISTID& self, int slice);

	// �뿪��������
	static int HB_LeaveScene(IKernel* pKernel, const PERSISTID& self, int slice);

	// ��ȡ����boss�����԰� 
	int	QueryCurPackageId(IKernel* pKernel);

	// ��ȡ�ɳ�����
	const WorldBossGrowUp* GetGrowUpData(int nSceneId);

	// ����boss�ȼ�
	void UpdateGrowUpLevel(IKernel* pKernel, bool bBeKilled);
public:
	static WorldBossNpc* m_pWorldBossNpc;            // ����ָ��
	static ChannelModule* m_pChannelModule;          // Ƶ��ģ��
	static RewardModule* m_pRewardModule;            // �콱ģ��
	static DropModule* m_pDropModule;                // ����ģ��
	static LandPosModule* m_pLandPosModule;          // ����
	static CapitalModule* m_pCapitalModule;          // ����

public:
	//----------------------------������������--------------------------------//
	static std::wstring m_domainName;

	// �˺�copy������
	typedef std::vector<CopyAIHurt_t> CopyAIHurtVector;
	
	typedef std::vector<WorldBossAward_t> BossAwardVec;
	typedef std::vector<WorldBossActive_t> WorldBossActiveVec;

private:
	//----------------------------���������ļ�����----------------------------//
	static std::vector<int> m_SceneScopeVec;
	//static std::map<int, std::vector<int>> m_SceneScopeMap;
	static BossAwardVec m_vecBossAward;
	static WorldBossActiveVec m_vecActiveInfo;
	
	//WBAllOtherNpcMap	m_mapAllOtherNpc;		// ����boss����С������
	WorldBossGrowUpVec	m_vecWorldBossGrowUp;	// ����boss�ɳ�����

	// ��������
	typedef ConstConfig_t ConstConfig;
	static ConstConfig m_kConstConfig;
	


	// GM ���ûʱ��
	static GMTimeInfo GMTime;

	// �ʱ��
	static TimeInterval_t m_NoticeTimeCount;
	static TimeInterval_t m_ActiveTimeCount;

	static int			m_nCustomSceneId;			// ֪ͨ�ͻ��˵ĳ���id

	
// 	static std::map<int, BuffConfig> m_CopperBuffInfoMap;
// 	static std::map<int, BuffConfig> m_SilverBuffInfoMap;
};

#endif