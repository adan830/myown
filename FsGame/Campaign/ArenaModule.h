//---------------------------------------------------------
//�ļ���:       ArenaModule.h
//��  ��:       ������
//˵  ��:       
//          
//��������:      2015��06��25��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------
#ifndef _FSGAME_ARENAMODULE_H_
#define _FSGAME_ARENAMODULE_H_

#include "Fsgame/Define/header.h"

#include <string>
#include <vector>
#include <map>

#include "FsGame/Define/ArenaModuleDefine.h"

class RankListModule;
class CapitalModule;
class LevelModule;
class CopyOffLineModule;
class LandPosModule;
class RewardModule;
class VipModule;
class DynamicActModule;
class LogModule;

class ArenaModule : public ILogicModule
{    
public:

    enum NotifyStrategy
    {
        DO_NOT_NOTIFY = 0, // ��Ҫ����֪ͨ
    };
    
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);
	
	// �ͷ�
	virtual bool Shut(IKernel* pKernel);
	
	// ������Դ
	virtual bool LoadResource(IKernel* pKernel);
	
	// ����������
	bool LoadArenaConfig(IKernel* pKernel, const char *file_name);
	
    // ��ѡ�˹���
    bool LoadCandidateConfig(IKernel* pKernel, const char *file_name);
    
    // ��ս��������
    bool LoadChallengeRewardConfig(IKernel* pKernel, const char *file_name);
    
    // ������������
    bool LoadRankRewardConfig(IKernel* pKernel, const char *file_name); 
    
    // �������а�����
    bool LoadRankListConfig(IKernel* pKernel, const char *file_name);
    
    // ���������������
    bool LoadPromotionReward(IKernel* pKernel, const char *file_name);
    
    // ������������
    bool LoadRankUpReward(IKernel* pKernel, const char *file_name);
    
    // ��ȡdomain������
    static const std::wstring& GetDomainName(IKernel * pKernel);
    
    // ��ȡ��ҵ�����, ������� result ��, �ɹ�����true
    //  result �ĸ�ʽ�� [����][�ȼ�������][�ȼ�������] 
    //      ���磺 [7][30][39] ��ʾ��ҵ�7�����ȼ����� [30, 39] ֮��
    static bool GetArenaRank(IKernel* pKernel, const PERSISTID& player, CVarList &result);

    // GM ����������, ��������GM
    static void GMReset(IKernel* pKernel, const PERSISTID& player);
    
private:

    // ��ɫ����
    struct Coordinate
    {
		 Coordinate() : x(0.0f), z(0.0f), o(0.0f){}
        len_t x;
        len_t z;
        len_t o;
    };

    // ������������
    struct ArenaConfig
    {
		 ArenaConfig() : open_level(0), scene_id(0), buy_chance_limit(0), chosen_count(0),
			 free_chance(0), count_down(0), round_time(0), wait_before_close(0), 
			 challenge_cd(0), challenge_clear_cd_fee(0), nBroadRank(0)
		 {
			 memset(chance_fee, 0, sizeof(int)*ARENA_CHALLENGE_CHANCE_MAX_SIZE);
		 }
        int open_level;                 // �����ȼ�
        int scene_id;                   // ����������ID
        Coordinate challenger_born;     // ��ս�߳�����
        Coordinate challengee_born;     // ����ս�߳�����
        int buy_chance_limit;           // ÿ�տɹ�����ս��������
        int chosen_count;               // ��ս�ĺ�ѡ����
        std::string npc_config;         // ����NPC��config
        int free_chance;                // ÿ�������ս����
		int challenge_cd;				// ��ս��ȴ
		int challenge_clear_cd_fee;		// �����սCD���ѵ�Ԫ��
        int chance_fee[ARENA_CHALLENGE_CHANCE_MAX_SIZE];   // ������Ѿ���������ķ���
        int count_down;                 // �غϿ�ʼǰ�ĵ���ʱ(��)
        int round_time;                 // �غ�ʱ��(��)(����ʱ����սʧ��)
        int wait_before_close;          // ��ս������ȴ�ʱ��(��)
        std::string rank_reward_name;   // ����������������
        std::string challenger_win;     // ��ս�߻�ʤ��֪ͨ 
        std::string challenger_failed;  // ��ս����ܵ�֪ͨ 
        std::string challengee_win;     // ����ս�߻�ʤ��֪ͨ 
        std::string challengee_failed;  // ����ս����ܵ�֪ͨ 
		int nBroadRank;					// �㲥�仯���ε�����
    };

    // ��ս�ĺ�ѡ�˹���
    struct CandidateRule
    {
		CandidateRule() : lower_rank(0), upper_rank(0), candidate_count(0){}
        int lower_rank;          // �˹���Ҫ�������������
        int upper_rank;          // �˹���Ҫ�������������
        int candidate_count;     // �˹�������ĺ�ѡ����
    };

    // ��������Ʒ
    struct ArenaAward
    {
		ArenaAward() : str_exp(""), str_capital(""), str_item(""){}
        std::string str_exp;					  // ������㹫ʽ
        std::string str_capital;
        std::string str_item;
    };

    // ��ս����
    struct ChallengeReward
    {
		ChallengeReward() : lower_level(0), upper_level(0){}
        int lower_level;
        int upper_level;
        ArenaAward winner_award;
        ArenaAward loser_award;
    };

    // ��������
    struct RankReward
    {
		RankReward() : rank_list_name(""), lower_rank(0), upper_rank(0){}
        std::string rank_list_name;
        int lower_rank;
        int upper_rank;
        ArenaAward award;
    };
    
    // �����������а�
    struct ArenaRankListConfig
    {
		ArenaRankListConfig() : lower_level(0), upper_level(0), name(""), level(0), need_notify(0){}
        int lower_level;
        int upper_level;
        std::string name;
        int level;       // ���а�ĵȼ�
        int need_notify; // ����˰��Ƿ���Ҫ֪ͨ
    };
    
    // �������Ľ�������
    struct PromotionReward
    {
		PromotionReward() : rank_list_name(""), reward_name(""), capital(""), item(""){}
        std::string rank_list_name;
        std::string reward_name;
        std::string capital;
        std::string item;
    };
    
    // ������������
    struct RankUpReward
    {
		RankUpReward() : rank_list_name(""), lower_rank(0), upper_rank(0){}
        std::string rank_list_name;
        int lower_rank;
        int upper_rank;
        
		ArenaAward award;
    };
	
	struct RanklistRecInfo
	{
		RanklistRecInfo() : rec(NULL), level(0){}
        std::string name;
        int level;
        IRecord *rec;
	};
	
public:

    // ��ɫ���ݻָ�
    static int OnRecover(IKernel* pKernel, const PERSISTID& player,
        const PERSISTID& sender, const IVarList& args);
    
    //���볡��
    static int OnAfterEntryScene(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);
 
    // ����
    static int OnReady(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);
 
    // �ͻ�������
    static int OnContinueReady(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);
 
    // ����뿪����
    static int OnLeaveScene(IKernel *pKernel, const PERSISTID &self,
        const PERSISTID &sender, const IVarList &args);

	static int OnPlayerContinue(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	//����sns��Ϣ
	static int OnSnsMessage(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args);

	//--------------------------Sns��������������Ϣ������-------------------------------
	// ��ѯս���ظ�
	static int OnSnsQueryFightResult(IKernel* pKernel, const IVarList& args);
    
    // ��ɫ���ݱ���
    static int OnCreateRobot(IKernel * pKernel, const PERSISTID & player,
        const PERSISTID & sender, const IVarList &args);

    // ��ҵȼ��仯
    static int OnPlayerLevel(IKernel* pKernel, const PERSISTID &self,
        const PERSISTID & sender, const IVarList & args);
        
    // ���Կͻ��˵���Ϣ
    static int OnCustomMessage(IKernel * pKernel, const PERSISTID & player,
        const PERSISTID & sender, const IVarList & args);

    // ���Թ�������Ϣ
    static int OnPublicMessage(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);
    
    // ������ҵ�command
    static int OnPlayerCommand(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);   
        
    // ��ʼս��ǰ�ĵ���ʱ
    static int HB_BeforeFight(IKernel* pKernel, const PERSISTID& self, int slice);
    
    // �غϼ��ļ�ʱ��
    static int HB_RoundCheck(IKernel* pKernel, const PERSISTID& self, int slice);

    // ��ʱ�˳�������
    static int HB_CloseWait(IKernel* pKernel, const PERSISTID& self, int slice);
    
    // �뿪����������
    static int HB_LeaveScene(IKernel* pKernel, const PERSISTID& self, int slice);

    // ���һ�λ�ɱ
    static int OnKillObject(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);
        
    // ����ɱ
    static int OnBeKilled(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // �������
//     static int PromotePlayer(IKernel* pKernel, const PERSISTID& self,
//         const char *current_ranklist_name, const char *new_ranklist_name);
// 
//     // �������
//     static int PromotePlayer(IKernel* pKernel, const PERSISTID& self,
//         const char *current_ranklist_name, const char *new_ranklist_name, const IVarList &row_data);

	// ��ȡ��ѡ����ս��
	static int OnGetChallengeCandidate(IKernel * pKernel, const PERSISTID & player,
		const PERSISTID & sender, const IVarList & args);

	// ������սĳ��
	static int OnChallengeSomeone(IKernel * pKernel, const PERSISTID & player,
		const PERSISTID & sender, const IVarList & args);
private:

    // ��ѯ�Լ��ľ�������Ϣ
    static int OnSelfArenaInfo(IKernel * pKernel, const PERSISTID & player,
        const PERSISTID & sender, const IVarList & args);

    // ���򾺼�������
    static int OnBuyArenaChance(IKernel * pKernel, const PERSISTID & player,
        const PERSISTID & sender, const IVarList & args);
    
    // ������ս
    static int OnGiveUpChallenge(IKernel * pKernel, const PERSISTID & player,
        const PERSISTID & sender, const IVarList & args);
	
	// ������սCD
	static int OnClearChallengeCD(IKernel * pKernel, const PERSISTID & player,
		const PERSISTID & sender, const IVarList & args);

	// ��Ӧս����������
	static int OnRequestFightResult(IKernel * pKernel, const PERSISTID & player,
		const PERSISTID & sender, const IVarList & args);

    // ��ȡ��ҵľ�������Ϣ
    static bool GetSelfArenaInfo(IKernel * pKernel, const PERSISTID & player,
        IVarList& summary);
    
    // ��ս����, @result ��Ӯ���, @reason ԭ��
    static int OnChallengeEnd(IKernel * pKernel, const PERSISTID &challenger, int result, int reason);
    
    // ���ݾ�����������ͽ���
    static bool RewardByChallenge(IKernel * pKernel, const PERSISTID &challenger, int challenger_prev_rank,
		int challenger_new_rank, const wchar_t *challengee_name, int result);

	// ��¼˫����ս������
	void RecordFightResultData(IKernel * pKernel, const PERSISTID &challenger, const wchar_t *challengee_name, int challenger_prev_rank, int challenger_new_rank, int challengee_rank, int nResult);

	// ����һ��ս������
	void SaveOneFightResultData(IKernel * pKernel, const PERSISTID &player, const wchar_t *enemy_name, int challenger_prev_rank, int challenger_new_rank, int nResult);

    // Public ����������ս����Ļ�Ӧ
    static int OnPubChallengeRequest(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

    // Public ����������ҽ����Ļ�Ӧ
    static int OnPubPromote(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

    // Public �������Ժ�ѡ�˵Ļ�Ӧ
    static int OnPubCandidate(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

    // Public ����������ս�����Ļ�Ӧ
    static int OnPubChallengeEnd(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

	// ����һ��ս������
	static int OnAddFightResultData(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);

	// VIP�����ص�
	static int C_OnVIPLevelChanged(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);
public:

   // ��սĳ��
    static bool ChallengeSomeone(IKernel * pKernel, const PERSISTID & player,
        const wchar_t* challengee_name);

    // ���ž�������������
    static bool RewardByArenaRank(IKernel *pKernel);

    static int ResetArenaChanceTimer(IKernel* pKernel, const PERSISTID& self, int slice = 0);

    // ���ž�������
    static bool RewardByPromotion(IKernel* pKernel, const PERSISTID& self, const char *rank_list_name);
    
    // �Ƿ񾺼�������
    static bool IsArenaScene(IKernel * pKernel);
    
	// gm��������Լ��ľ�����״̬ bLockStateΪtrue ��������״̬����(��Ϊ�˸��ʼ�����õ�), false��վ�������ս״̬
	static bool ResetArenaStateByGm(IKernel * pKernel, const wchar_t* strName, int nLockState);

	// �����������, ����п����ڶ�����д������ݣ�ֻ������ߵİ�
	static int PlayerRanklistClean(IKernel * pKernel);
private:

    // ������սѡ��
    static bool UnlockPlayer(IKernel * pKernel, const PERSISTID & player);
    
    // �������
    static void FreezePlayer(IKernel * pKernel, const PERSISTID & player);
    
    // �ⶳ���
    static void UnfreezePlayer(IKernel * pKernel, const PERSISTID & player);

    // �ȴ�һ��ʱ��Ȼ���뿪������
    static void WaitThenLeaveArena(IKernel * pKernel, const PERSISTID & player);
    
    // �����뿪������
    static void LeaveArenaNow(IKernel * pKernel, const PERSISTID & player);
    
    // ������Ƿ���Ա���ս
    static bool IsValidChallengee(const char *challenger_uid,
        CVarList &challengee_info, const char *rank_list_name);
    
    // ���þ���������
    static bool ResetArenaChance(IKernel * pKernel, const PERSISTID & player);
    
    // �����ȥ�ʳ�
    static bool GoToCity(IKernel * pKernel, const PERSISTID & player);
    
    static bool IsRobot(IKernel * pKernel, const char *ranklist_name, const wchar_t *role_name);
    
    // �����������ͻ�ƽ���
    static int RewardByRankUp(IKernel *pKernel, const PERSISTID & player, const std::string &capital_reward, const std::string& item_reward, int new_rank);
    
    //////////////////////////////////////////////////////////////////////////
    // ˽�еĸ�������
    //////////////////////////////////////////////////////////////////////////

    // ȡ�ú�ѡ�˹���
    static const CandidateRule* GetCandidateRule(int player_rank);
    
    // ȡ����ս��������
    static const ChallengeReward* GetChallengeReward(int player_level);
    
    // ȡ��������������
    static const std::vector<RankReward>* GetRankReward(const std::string &rank_list_name);

    // �������а�����ֻ�ý�������
    static const PromotionReward* GetPromotionReward(const std::string &rank_list_name);
    
    // ȡ�þ��������а�ı�
    static IRecord* GetArenaRankListRecord(IKernel * pKernel, const char* rank_list_name);

    // ���ݵȼ�ȡ�ö�Ӧ�ľ��������а�
    static const char* GetRankListName(IKernel * pKernel, int level);

    // ��������
    static bool ParseFee(std::string &str_fee, int *fee_array, int fee_array_size);

    // ��������
    static bool ParseCoordinate(const std::string &str_coordinate, Coordinate& coordinate);

    // ������������Ϣ��public������
    static void SendArenaMsgToPublic(IKernel * pKernel, const IVarList& msg);

    // ���а��һ��
    static bool PrepareDataForArenaRankList(IKernel * pKernel, const PERSISTID & player,
        const char *rank_list_name, IVarList& row_data);
    
    // ����Ҽӵ������������а���
    static bool InsertPlayerToArenaRankList(IKernel * pKernel, const wchar_t *player_name,
                                            const char *rank_list_name, IVarList& row_data);
    
    // ���а��֪ͨ����
    static NotifyStrategy GetRankListNotifyStrategy(std::string &rank_list_name);
    
    // ����������������ȡ����
	static bool GetRankUpReward(std::string &capital_reward, std::string& item_reward, const std::string &rank_list_name, int new_rank, int old_rank);
        
    // �Ƿ���ʷ�������
    static bool IsBestRank(IKernel * pKernel, const PERSISTID & player,
        int old_rank, int new_rank);
    
    // �� float ת��INT, ����ȡ��
   // static int Float2IntRoundUp(float f);
    
    // �Ƚ����а�ĵȼ�
    static bool CompareRanklistLevel(const RanklistRecInfo &a, const RanklistRecInfo &b);
    
	// ���¼�������
	static void ReloadConfig(IKernel * pKernel);

	// ������������ж���
	void FreezeAllObject(IKernel* pKernel, IGameObj* pSelfObj);

	// ���Թ�������Ϣ
	static int OnCreateScene(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);

	// ����������������
	void ExportPubRecData(IKernel* pKernel, int nStart, int nEnd);

	// 
	void ExportLevelRankData(IKernel* pKernel, const char* strRecName, int nStart, int nEnd, int nNo);
private:

    static RankListModule * m_pRankListModule;
    static CapitalModule * m_pCapitalModule;
    static LevelModule * m_pLevelModuel;
    static CopyOffLineModule *m_pCopyOfflineModule;
    static LandPosModule * m_pLandPosModule;
    static RewardModule * m_pRewardModule;
    static VipModule * m_pVipModule;
	static DynamicActModule * m_pDynamicActModule;
	static LogModule * m_pLogModule;
    
    typedef std::map<std::string, std::vector<RankReward> > RankRewardMap;
    typedef std::map<std::string, PromotionReward > PromotionRewardMap;
    
    typedef std::map<std::string, std::vector<RankUpReward> > RankUpRewardMap;
    
    static ArenaConfig m_ArenaConfig; 
    static std::vector<CandidateRule> m_CandidateRule;      // ��������ѡ�˹���
    static std::vector<ChallengeReward> m_ChanllengeReward; // ��������ս����
    static RankRewardMap m_RankReward;                      // ��������������
    static std::vector<ArenaRankListConfig> m_ArenaRankList;      // �����������а�
    static PromotionRewardMap m_PromotionReward;            // ��������
    static RankUpRewardMap m_RankUpReward;             // ������������
    
    static std::wstring m_domainName;  // ������������Domain��������
public:

	static ArenaModule* m_pArenaModule;

};

#endif