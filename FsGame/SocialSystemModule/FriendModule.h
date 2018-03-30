//--------------------------------------------------------------------
// �ļ���:      FriendModule.h
// ��  ��:      ����ģ��
// ˵  ��:		
// ��������:    2014��10��17��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef __FriendSystem_H__
#define __FriendSystem_H__

#include "Fsgame/Define/header.h"
#include <vector>
#include "FsGame/Define/FriendDefine.h"

#define CONFIG_FRIEND_REFRESH_FRIEND_RMD_CD 30*1000
#define INTIMACY_AWARD "mail_intimacy_award" 
class RankListModule;
//class PartnerModule;
class CapitalModule;
class RevengeArenaModule;
class TeamModule;

class FriendModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);
	// �ͷ�
	virtual bool Shut(IKernel* pKernel);
	// �������� 
	bool LoadConfig(IKernel* pKernel);
	
	static void ReloadConfig(IKernel* pKernel);

	//���������ӳ�
	bool LoadTeamFriendConfig(IKernel*pKernel);
	//�������ܶȼӳ�
	bool LoadTeamFirendIntimacy(IKernel*pKernel);
	//
	bool LoadFirendIntimacyAward(IKernel*pKernel);
public:
	// �������
	static int OnRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	// �������
	static int OnStore(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	// ���׼������
	static int OnReady(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

	//����sns��Ϣ
	static int OnSnsMessage(IKernel * pKernel, const PERSISTID & self,const PERSISTID & sender, const IVarList & args);

	// �Ƿ�Ϊ����
	static bool IsEnemy(IKernel* pKernel, const PERSISTID& self, const char* strUid);

	// �ͻ�����Ϣ
	static int OnCustomMessage(IKernel * pKernel, const PERSISTID & self,const PERSISTID & target, const IVarList & args);
	// �ڲ���Ϣ
	static int OnCommandMessage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	// sns�ڲ���Ϣ
	static int OnPlayerSnsCommand(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	// ��ɱ�ص�
	static int OnCommandBeKill(IKernel* pKernel, const PERSISTID& self,	const PERSISTID& sender, const IVarList& args);
	// ɱ��Ŀ��ص�
	//static int OnCommandKillTarget(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	// ����Ƚ�
	static bool CompareDistance(const NearbyRecommend& first, const NearbyRecommend& second);
    //�ȼ��仯�ص�
    static int OnCommandLevelChange(IKernel* pKernel, const PERSISTID& self,	const PERSISTID& sender, const IVarList& args);
	//����������ܶ�
	//static int OnCommandTeamAddIntimacy(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	
private:
	// ��Ӻ�������
	void OnCustomAddFriend(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ȷ�Ϻ������
	void OnCustomAcceptAddFriend(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ����ɾ���죨���ԣ��������δ������
	void OnCustomDeleteFriendApply(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ɾ������
	void OnCustomDeleteFriend(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ����ˢ���Ƽ�����
	void OnCustomRefreshFriendRmd(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ���º�����Ϣ
	void OnCustomFriendInfoUpdate(IKernel* pKernel, const PERSISTID& self);
	// ��ӳ���
	void OnCustomAddOnceEnemy(IKernel * pKernel, const PERSISTID & self, const PERSISTID& sender);
	// ɾ������
	void OnCustomDeleteEnemy(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ɾ�����г���
	void OnCustomDeleteAllEnemy(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ���󸽽����
	void OnCustomRequestNearby(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ������ҵ��Ƽ���
	void FindPlayerToRecommend(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ����Ϣ��������������º�����Ϣ
	void OnCustomQueryFriendInfo(IKernel* pKernel, const PERSISTID& self);
	// ������ͺ�
	void OnCustomSendGift(IKernel* pKernel, const PERSISTID& self, const char* targetUid, int nGiftId);
	// ��Ӻ�����
	void OnCustomAddBlackList(IKernel* pKernel, const PERSISTID& self, const wchar_t* targetName);
	// ɾ��������
	void OnCustomDeleteBlackList(IKernel* pKernel, const PERSISTID& self, const wchar_t* targetName);
	// ���������
	void OnCustomClearBlackList(IKernel* pKernel, const PERSISTID& self);
	// һ���������к�������
	void OnCustomConfirmAllApply(IKernel* pKernel, const PERSISTID& self, bool bAccept);
	// �������
	void OnCustomApplaySworn(IKernel*pKernel, const PERSISTID& self, const IVarList & args);
	//��������ظ�
	void OnCustomApplaySwornReply(IKernel*pKernel, const PERSISTID& self, const IVarList & args);
	
	// Ѱ��Ԯ��
	void OnCustomSeekAid(IKernel*pKernel, const PERSISTID& self, const IVarList & args);
	// �޸ĺ��ѳ�ν
	void OnCustomChangeFriendTitle(IKernel*pKernel, const PERSISTID& self, const IVarList & args);
	//ɾ������
	void OnCustomDelSworn(IKernel*pKernel, const PERSISTID& self, const IVarList & args);
	//Ԯ��
	void OnCustomAid(IKernel*pKernel, const PERSISTID& self, const IVarList & args);
	//��ȡ���ܶȽ���
	void OnCustomIntimacyAwards(IKernel*pKernel, const PERSISTID& self);
	//һ����Ӻ���
	void OnCustomAddFriendByOneKey(IKernel*pKernel, const PERSISTID& self, const IVarList &args);
	
	//���ӵ���
	void AddOnceEnemy(IKernel*pKernel, const PERSISTID& self, const IVarList & args);
	//����������ӵ���
	void SwornFriendAddOnceEnemy(IKernel*pKernel, const PERSISTID&self, const IVarList & args);
	// ���º��ѵ�����״̬
	void OnCommandOnlineUpdate(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	// ɾ������
	void OnCommandRemoveFriend(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	// �����������
	void OnCommandAddFriendApply(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ���ѵȼ�����
	void OnCommandFriendLevelUpdate(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ����ս��������
	void OnCommandFriendBattleAbility(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ���Ѱ�����
	void OnCommandFriendGuildName(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
    //��Ӻ��ѷ���
    void OnCommandAddFriendResult(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ���ѻ���������
	void OnCommandFriendFlowerChanged(IKernel * pKernel, const PERSISTID & self, const char* friendUid, int nSumFlower, int nWeekFlower );
	// �����Լ��ͺ������ܶ�
	void OnCommandAddFriendMutualIntiMacy(IKernel*pKernel,const PERSISTID&self,const char* firendUid,int addIntimacy);
	
	// �������ܶ�
	void OnCommandAddFriendIntiMacy(IKernel*pKernel,const PERSISTID&self,const char* firendUid,int addIntimacy,bool isAddDaily = false);

	// ȡ��sns��ѯ����
	void OnCommandGotSnsData(IKernel * pKernel, const PERSISTID & self, const IVarList & args,IVarList &core_msg);
		
	//���Ѷ���id
	void OnCommandUpdateFriendTeamID(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	//�������
	void OnCommandApplaySworn(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	//���ضԷ��Ƿ�ԭ�����
	void OnCommandApplaySwornRsp(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	//����ɹ�
	void OnCommandSwornSuccess(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	//ɾ���������
	void OnCommandDelSworn(IKernel * pKernel, const PERSISTID & self, const IVarList & args);

	bool IsCanSworn(IKernel*pKernel, const PERSISTID& self, const wchar_t* targetName);
	//
	void OnCommandAidGetPosReq(IKernel* pKernel, const PERSISTID & self, const IVarList & args);
	void OnCommandAidGetPosRsp(IKernel* pKernel, const PERSISTID & self, const IVarList & args);
	//void OnCommandSetNation(IKernel* pKernel, const PERSISTID & self, const IVarList & args);
	void OnCommandUpdatetVip(IKernel* pKernel, const PERSISTID & self, const IVarList & args);



	// ˢ�º��Ѽӳ�
	static int OnCommandRefreshFriendGain(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	


	// ɾ���Ƽ����
	int DeleteRecommend(IKernel * pKernel, const PERSISTID & self, const wchar_t* pTargetName );
	// ���һ���Ƽ����
	void AddOneRecommend(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ����һ������
	void UpdateOneFriend(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ���³����Ϣ
	void UpdateOneEnmey(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ���º�������Ϣ
	void UpdateBlackPlayer(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ����һ�����
	void FindOnePlayer(IKernel * pKernel, const PERSISTID & self, const IVarList & args);

    // �������Ϊ������
    void SetPlayerInvited(IKernel * pKernel, const PERSISTID & self, const char *role_uid);  
    // �鿴������Ƿ��Ѿ������� bRemove: �������ɾ��
    bool TestRemovePlayerInvited(IKernel * pKernel, const PERSISTID & self, const char *role_uid, bool bRemove = false );
	//��ý���������
	int GetSwornNum(IKernel*pKernel, const PERSISTID & self,int sex);
	//vip�����Ƽ�����
	void RecommendFriendByVipLv(IKernel*pKernel,const PERSISTID & self);
	// �������
	void RecommendFriendByPlayerLv(IKernel*pKernel, const PERSISTID & self);
	// ������֯
	//void RecommendFriendByAddGuild(IKernel*pKernel, const PERSISTID & self);
	//�Ƿ���Գ�Ϊ����
	bool CanBeFirend(IKernel*pKernel, const PERSISTID & self,const wchar_t * selfName ,const wchar_t*targetName,int targetLv);
	
		
	//��õ�ǰ�������Ӷ������ܶ�
	int GetTeamAddIntimacyValue(int type);
	//ÿ���������ܶ�����
	int GetDailyAddIntimacyLimit();


	//--------------------------Sns��������������Ϣ������-------------------------------
	// ��ѯ���������롱�Ļظ�
	static int OnSnsQuestFriendApplyRPL(IKernel* pKernel, const IVarList& args);
	// ��ѯ�������ϵ���Ļظ�
	static int OnSnsQuestFriendDeleteRPL(IKernel* pKernel, const IVarList& args);
	// ��ѯ���������������Ļظ�
	static int OnSnsQuestFriendApplyResultRPL(IKernel* pKernel, const IVarList& args);
	// ��ȡ�������ͻ����Ļظ�
	static void OnSnsQuestFriendGiftPush(IKernel* pKernel, const IVarList& args);
	// ��ȡ���Ӻ������ܶ�
	static void OnSnsAddFriendIntimacyPush(IKernel* pKernel, const IVarList& args);
	// ɾ���������
	static void OnSnsDelSworn(IKernel* pKernel, const IVarList& args);
	// ���ӵ���
	static void OnSnsAddEnemy(IKernel*pKernel,const IVarList& args);

public:
	// ս�����仯�ص�
	static int C_OnDCBattleAbilityChange(IKernel * pKernel, const PERSISTID & self, const char * property, const IVar & old);
	// ������仯�ص�
	//static int C_OnDCGuildNameChange(IKernel * pKernel, const PERSISTID & self, const char * property, const IVar & old);
	//����id�仯�ص�
	static int C_OnDCTeamIDChange(IKernel*pKernel, const PERSISTID & self, const char * property, const IVar & old);

	// VIP�����ص�
	//static int C_OnVIPLevelChanged(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);
private:
	// ֪ͨ����ˢ�»�����
	void OnPlayerGiftChanged(IKernel* pKernel, IGameObj* pSelf, int nMySumFlower, int nMyWeekFlower);
	// ���ͳ��ֵ
	void OnDecEnmeyHaterT(IKernel* pKernel, IGameObj* pSelf, const char* pEnemyUid, int nHatarT);
	//ˢ�¶����������
	//bool RefreshTeamFriendGain(IKernel*pKernel, const PERSISTID& self);
public:
	// ��Ҵ��
	bool PackPlayerInfo(IKernel* pKernel, IVarList& var, const wchar_t* pTargetName);
	
	void GetNoRepeatRandomNumber(const int nMax, const int Number, std::vector<int> & iVector);

	// ��������������δ������
	int AddFriendApply(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ����ɾ�������ԣ��������δ������
	int DeleteFriendApply(IKernel * pKernel, const PERSISTID & self, const IVarList & args);
	// ɾ������
	void DeleteFriend(IKernel * pKernel, const PERSISTID & self, const wchar_t * targetName);

	// �Ƿ�����Ӻ���
	bool IsCanAddFriend(IKernel * pKernel, const PERSISTID & self, const wchar_t * targetName, bool bShowFailedTip = false, bool bIgnoreFull = false);

	// �����б��Ƿ�����
	bool IsFullFriend(IKernel * pKernel, const PERSISTID & palyer);
	// targetName�Ƿ����Լ��ĺ���
	bool IsFriend(IKernel * pKernel, const PERSISTID & self, const wchar_t * targetName);
	// targetName�Ƿ������ѵĳ�����
	bool IsInEnemyList(IKernel * pKernel, const PERSISTID & self, const wchar_t * targetName);
	// targetName�Ƿ������ѵĺ�������
	bool IsInBlackList(IKernel * pKernel, const PERSISTID & self, const wchar_t * targetName);

	// ������������ж�����Ƿ����
	bool CheckPlayerExist(IKernel* pKernel, const wchar_t* name);

	// ������������ж�����Ƿ����ߣ����ж��Ƿ���ڣ�
	bool CheckPlayerOnline(IKernel* pKernel, const wchar_t* name);
    //������������������
    int FindApplyRow(IKernel * pKernel, const PERSISTID & self, const char* targetUid, ApplyType applyType);
public:
    // ��ȡ������������ұ�
    IRecord * GetPubOnlineRec(IKernel * pKernel);

private:
	// �ɹ����ͻ�
	void OnSendFlower(IKernel* pKernel, IGameObj* pSelf, const char* pszTargetUid, int nFlowers);
	// �ɹ��յ���
	void OnRecvFlower(IKernel* pKernel, IGameObj* pSelf, const char* pszSenderUid, int nFlowers);

	// �������ܶȴ��� �������ܶȵȼ�
	int IncFriendIntimacy(IKernel* pKernel, IGameObj* pSelf, IRecord* pRecord, int nRowIndex, int nExp, bool isAddDaily = false);
	// ���Ӻ������ܶ�
	void IncTeamFriendIntimacy(IKernel* pKernel, IGameObj* pSelf, const CVarList& friendList, int nExp);
	//�ܷ��������ܶ�
	bool IsCanAddIntimacy(IKernel*pKernel,IGameObj* pSelf,const char* friendUid);
	bool IsCanAddIntimacy(int intimacyLevelNow, int intimacyLevelMax,int intimacyExpNow);

	//������ܶ��������辭��
	int GetIntimacyUpNeedExp(int intimacyLevel);
private: 
	// ���������
	static int ResetWeekFlowerNum(IKernel* pKernel, const PERSISTID& self, int slice);
	// ÿ������
	static int ResetDaily(IKernel* pKernel, const PERSISTID& self, int slice);
	// ������ջ���
	void ResetFriedFlower(IKernel* pKernel, IGameObj* pSelf);
	// ���ܺ�������
	bool OnAcceptFriend(IKernel* pKernel, IGameObj* pSelf, int nApplyIndex, IRecord* pRecApply, IRecord* pRecFriends);
	// �ܾ���������
	void OnRefuseFriend(IKernel* pKernel, IGameObj* pSelf, int nApplyIndex, IRecord* pRecApply);
	// ��ȡ�����ռ�����
	static const std::wstring& GetDomainName(IKernel * pKernel);
	//bool IsCanSworn(IKernel*pKernel, const PERSISTID& self, const wchar_t* targetName);

	static std::wstring m_domainName;
public:
	// ��ģ��ָ��
	static FriendModule * m_pFriendModule; 
private:
	// ���а�ģ��
	static RankListModule * m_pRankListModule; 
   
	//���ģ��
	static CapitalModule* m_pCapitalModule;
	//������
	static RevengeArenaModule* m_pRevengeArenaModule;
	// ���
	static TeamModule* m_pTeamModule;

	//��Ӻ�������
	std::map<int, std::string> m_teamFriendGain;
	//������ܶȼӳ��Ǹ�
	std::map<int, std::string> m_teamFriendIntimacy;
	//���ܶȽ���
	std::map<int, std::string> m_teamIntimacyAward;
};

#endif
