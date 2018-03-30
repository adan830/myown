//--------------------------------------------------------------------
// �ļ���:		GuildModule.cpp
// ��  ��:		����ϵͳ
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __GuildModule_H__
#define __GuildModule_H__

#include "Fsgame/Define/header.h"
#include "../Define/GuildDefine.h"

class CapitalModule;
class VirtualRecModule;
class ItemBaseModule;
class ChatModule;
class SystemMailModule;

class GuildModule : public ILogicModule
{
public:
    // ��ʼ��
    virtual bool Init(IKernel* pKernel);
    // �ر�
    virtual bool Shut(IKernel* pKernel);

    // �����Ƿ��Ѿ�����
    bool IsGuildExist(IKernel* pKernel, const wchar_t* guildName);
    // ��ȡ�����������
    IPubData* GetPubData(IKernel* pKernel, const wchar_t* name);
    // ��ȡ���ṫ�����ݼ�¼
    IRecord* GetPubRecord(IKernel* pKernel, const wchar_t* pubDataName, const char* recName);
    // ���������ڹ�������
    const wchar_t* GetPlayerGuildName(IKernel* pKernel, const PERSISTID& self);
    // ��ȡ�������¼
    IRecord* GetGuildRecord(IKernel* pKernel, const PERSISTID& self, int& row);
	// ��ȡ�������¼
	IRecord* GetGuildRecord(IKernel* pKernel, const wchar_t* guildName, int& row);
    // ��ȡ��Ա����¼
    IRecord* GetMemberRecord(IKernel* pKernel, const PERSISTID& self, int& row);
	// ��ȡ��Ա����¼
    IRecord* GetMemberRecord(IKernel* pKernel, const wchar_t* guildName);
    // ��ð�����UID
    const char* GetGuildCaptainUid(IKernel* pKernel, const wchar_t *guildName);
	// ������Դ
	bool LoadResource(IKernel* pKernel);
	// ��ȡ���ȼ�
	int GetGuildLevel(IKernel* pKernel, const wchar_t* guildName);
	// ������Ȩ���Ƿ�����
	bool CheckPositionPriv(IKernel* pKernel, const PERSISTID & self, GuildPriv iPrivType);

	// ˢ������߹���ļ���������ʾ
	static bool UpdateNewApplyToAuditor(IKernel* pKernel, const PERSISTID& player);
	// �������Ƿ��ǹ���᳤
	bool CheckIsCaptain(IKernel* pKernel, const PERSISTID& self);

	// ׼������
	static int OnReady(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);
	//����й�������֪ͨ
	void InformGuildApply(IKernel*pKernel, const PERSISTID & self);
	

	void CustomMessageToGuildMember(IKernel*pKernel, const wchar_t* guildName, const IVarList& msg);
	void CommandToGuildMember(IKernel*pKernel, const wchar_t* guildName, const IVarList& msg);

	void SendGuildAward(IKernel* pKernel, const std::string &award, const wchar_t * guildName);

	void CustomSysInfoByGuild(IKernel * pKernel, const wchar_t* guildName, int tips_type,
		const char * fast_string_id, const IVarList & para_list);
	// �����֯��ֵ
	int GetGuildNumValue(IKernel*pKernel, const wchar_t* guildName, GUILD_NUM_TYPE guildNumType);
	//��֯�����Ƿ����
	bool GuildShortNameExist(IKernel*pKernel,const wchar_t* shortName);

	//��ý����ȼ�
	int GuildBuildingLevel(IKernel *pKernel,const wchar_t* guildName, GUILD_BUILDING_TYPE buildType);
	//��֯ģ�齱��
	void RewardGuildValue(IKernel *pKernel, const wchar_t*guildName, GUILD_NUM_AWARD type, int addValue, GUILD_NUM_CHANGE res, const wchar_t*playerName = NULL);
	//������֯��������
	void AddGuildBuildingExp(IKernel *pKernel,const wchar_t*guildName,int addValue);
	//������֯��ֵ
	void AddGuildNumValue(IKernel *pKernel, const wchar_t*guildName, GUILD_NUM_TYPE type, int value, GUILD_NUM_CHANGE res, const wchar_t*playerName = NULL);
	//���Ӽ����о�����
	void AddGuildSkillExp(IKernel *pKernel,const wchar_t*guildName,int addValue);
	//������֯��־
	void AddGuildLog(IKernel*pKernel, const PERSISTID& self,int type, const IVarList& args);
	// ���������ڹ���᳤������
	const wchar_t* GetPlayerGuildMasterName(IKernel* pKernel, const PERSISTID& self);

	// �۳���֯��ֵ
	static bool DecGuildValue(IKernel* pKernel, const GUILD_NUM_TYPE type, 
		const wchar_t *guild_name, const int value, const GUILD_NUM_CHANGE src);
private:
    // �������
    static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
    // �������
    static int OnPlayerDisconnect(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
    // ��ҵȼ��仯
    static int OnLevelChange(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args);
	// ս�����仯�ص�
	static int OnBattleAbilityChange(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);

	 //��ְ֯λ
	static int C_OnGuildPositionChange(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);

	//vip�ȼ�
	static int C_OnPlayerdVipLv(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);
    ///////////////////////////////////////////////////////////////////////////
    // ���ܺ���
    ///////////////////////////////////////////////////////////////////////////
    // ���ò�������Ҽ��빫������
    int SetOutLineJoinGuild(IKernel* pKernel, const PERSISTID& self, const wchar_t* playerName);
  
    // �򹫹������������������״̬�仯��Ϣ
    void SendOnlineStateToPub(IKernel* pKernel, const PERSISTID& self, int onlineState);
    // �򹫹�����������������Ա仯����Ϣ���ȼ���ս������������������
    void SendPropsChangeToPub(IKernel* pKernel, const PERSISTID& self);
    
    // ���wchar_t�Ƿ���Ч
    bool IsWidStrValid(const wchar_t* widStr);
    // ����ܷ񴴽�����
    bool CheckCanCreateGuild(IKernel* pKernel, const PERSISTID& self, const wchar_t* guildName);
    // ����Ƿ���Խ��������Ƿ���빫��Ĳ���
    bool CheckCanOptJoinGuild(IKernel* pKernel, const PERSISTID& self, const wchar_t* applyName);
  
    // ����Ƿ�����˳�����
    bool CheckCanQuit(IKernel* pKernel, const PERSISTID& self);
    // ����Ƿ�����߳���Ա
    bool CheckCanFire(IKernel* pKernel, const PERSISTID& self, const wchar_t* member);
    // ����Ƿ���Խ�ɢ����
    bool CheckCanDismiss(IKernel* pKernel, const PERSISTID& self);
    // ��������뿪����
    bool SetLeaveGuild(IKernel* pKernel, const PERSISTID& self, int64_t leaveDate,bool isSetQuitGuildTime =false);
	// ���ְλ�Ƿ����Ȩ��
	bool CheckPositionPriv(IKernel* pKernel, int position, GuildPriv iPrivType);
    // ���л������Ϣ�����ڷ��Ϳͻ�����ʾ�İ���б�
    bool SerialGuildInfo(IKernel *pKernel, const PERSISTID& self, CVarList &varGuild, int randVal, const wchar_t *guildName);
	// ��ð������������Ϣ
    GuildUpLevelConfig_t *GetGuildUpLevelConfig(int lvl);
	// ���ְλ��Ӧ�̵깺�����
    int GetGuildShopMemberLimit(int guildPosition, int rawLimit);
	// ��ð��ְλ������Ϣ
    GuildPositionConfig_t *GetGuildPositionConfig(int position);
	// ������˴���
	bool CheckFireLimit(IKernel* pKernel, const PERSISTID& self);
	//��ȡ��֯������������
	int GetGuildSkillLevelUpSpend(int skillID,int skillLevel);
	//ˢ����֯����
	void RefreshGuildSysmbol(IKernel*pKernel, const PERSISTID& self);
	//��֯ÿ�չ�������
	static int ResetDaily(IKernel* pKernel, const PERSISTID& self, int slice);
public:
	///////////////////////////////////////////////////////////////////////////
	// �ͻ�����Ϣ�ص�
	static int OnCustomMessage(IKernel* pKernel, const PERSISTID& self, 
        const PERSISTID& sender, const IVarList& args);
	// �㲥�ͻ�����Ϣ
	static void BroadCastGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& broadcast);
	static void BroadCastGuild(IKernel* pKernel, const wchar_t* pwszGuildName, const IVarList& broadcast);
private:
    // ���������������
    int OnCustomCreateGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // �����ȡ�����б�����
    int OnCustomGetGuildList(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ����������������
    int OnCustomFindGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ��������������
    int OnCustomGetTableData(IKernel* pKernel, const PERSISTID& self, int msgType);
    // ����������빫�������
    int OnCustomApplyJoinGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	// ����һ��������빫������
	int OnCustomOneKeyApplyJoinGuild(IKernel* pKernel, const PERSISTID& self);

    // ����ȡ��������빫�������
    int OnCustomCancelApplyJoinGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	// �����ȡ������Ϣ������
	int OnCustomGetGuildInfo(IKernel* pKernel, const PERSISTID& self);
	// ������֯��ֵ����
	int OnCustomGuildNumInfo(IKernel*pKernel, const PERSISTID& self,const wchar_t*guildName);
	
	
	//��ù��������Ϣ
	int OnCustomGuildSysmbol(IKernel*pKernel, const PERSISTID& self, const wchar_t * guildName);
	//��֯������Ϣ
	int OnCustomGuildBuilding(IKernel*pKernel, const PERSISTID& self);
	// ��֯������Ϣ
	int OnCustomGuildSetInfo(IKernel*pKernel, const PERSISTID& self);
	// ʹ���ر�
	int OnCustomUseRareTreasure(IKernel*pKernel, const PERSISTID& self,int rareTreasureType,int num);
	//����ر��б�
	int OnCustomGetRareTreasureList(IKernel*pKernel, const PERSISTID& self);
	//�����֯��ֵ�������
	int OnCustomGetNumBuyRecord(IKernel*pKernel, const PERSISTID& self);
	
	// ������ܼ��빫�������
	int OnCustomAcceptJoinGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

    // ����ܾ����빫�������
    int OnCustomRefuseJoinGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

    // ���������Ա�б�
    int OnCustomGetGuildMemberList(IKernel *pKernel, const PERSISTID &self, const IVarList &args);

    // ����������������б�
    int OnCustomGetGuildApplyList(IKernel *pKernel, const PERSISTID &self, const IVarList &args);

    // �����޸Ĺ�������
    int OnCustomChangeNotice(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	// �����޸���������
	int OnCustomChangeDeclaration(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // �����˳�����
    int OnCustomQuit(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // �����߳�����
    int OnCustomFire(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // �����ɢ����
    int OnCustomDismiss(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // �������
    int OnCustomDonate(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ������ְ
    int OnCustomPromotion(IKernel* pKernel, const PERSISTID& self, const IVarList& args, int type);
    // ���󹫻��̵��б�
    int OnCustomGetShopList(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ������빫���CD
    int OnCustomGetJoinCD(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // �������̵���Ʒ
    int OnCustomBuyItem(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	// �����ƽ�����֮λ
    int OnCustomCaptainTransfer(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	// ��������־
	int OnCustomGetLog(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	// ���������־
	int OnCustomGetDonateLog(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	
	//�Զ�ͬ��
	void OnCustomSetAutoAgree(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	//�������Ἴ�� 
	void OnCustomUnLockSkill(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	//��ù��Ἴ���б�
	void OnCustomGetSkillList(IKernel*pKernel,const wchar_t*guildName,CVarList&var);
	//��������
	void OnCustomSkillLevelUp(IKernel*pKernel, const PERSISTID& self,const IVarList& args);
	//��֯��������
	void GuildSkillLevelUp(IKernel*pKernel, const PERSISTID & self,int level , int newLevel);
	//��֯��������
	void OnGuildBuildingOperateLv(IKernel*pKernel,const PERSISTID& self,const IVarList& args);
	//��֯��ֵ����
	void OnGuildNumBuy(IKernel*pKernel, const PERSISTID& self, const IVarList& args);
	//�޸���֯���
	void OnCustomChangeGuildShortName(IKernel*pKernel, const PERSISTID& self, const IVarList& args);
	//��ȡ���������
	void OnCustomGetJianKangDuGif(IKernel*pKernel,const PERSISTID& self);
	//��ʼ�о�����
	void OnCustomGuildSkillLvOperate(IKernel*pKernel, const PERSISTID& self, const IVarList& args);

	//��ȡ�ֺ���Ϣ
	int OnCustomGetBonusInfo(IKernel*pKernel, const PERSISTID& self);
	//��ȡ��֯�ʽ�
	int OnCustomGetGuildWelfare(IKernel*pKernel, const PERSISTID& self);
	//�ֺ�
	int OnCustomGiveOutBonus(IKernel*pKernel, const PERSISTID& self, const IVarList& args);
	//���������֯
	int OnCustomInviteJoinGuild(IKernel*pKernel, const PERSISTID& self, const IVarList& args);
	//ͬ�����������֯
	int OnCustomAgreeInviteJoinGuild(IKernel*pKernel, const PERSISTID& self, const IVarList& args);
	//�����Ը
	void OnCustomAddPetition(IKernel*pKernel, const PERSISTID& self, const IVarList& args);
	//��ȡ��Ը
	void OnCustomGetPetition(IKernel*pKernel, const PERSISTID& self, const IVarList& args);
	//���������֯��Ϣ
	void OnCustomGetOtherGuildInfo(IKernel*pKernel, const PERSISTID& self, const IVarList& args);

	// ��ȡ���߿ɴ����б�
	void OnCustomTeachList(IKernel* pKernel, const PERSISTID& self, const IVarList& args, int nOffset);
	// ����Է����� args: nick[widestr] type[1:req 2:push]
	void OnCustomTeachReq(IKernel* pKernel, const PERSISTID& self, const IVarList& args, int nOffset);
	// ��Ӧ���Է����� args: nick[widestr] type[1:req 2:push] agree[1: ͬ�� 0: �ܾ�]
	void OnCustomTeachAck(IKernel* pKernel, const PERSISTID& self, const IVarList& args, int nOffset);
	// // �ͻ��˸��߷�����׼������ args: none
	void OnCustomTeachReady(IKernel* pKernel, const PERSISTID& self, const IVarList& args, int nOffset);


    ///////////////////////////////////////////////////////////////////////////
    // ���������ڲ���Ϣ
    ///////////////////////////////////////////////////////////////////////////
    // �ڲ�����ص�
    static int OnCommandGuild(IKernel* pKernel, const PERSISTID& self, 
        const PERSISTID& sender, const IVarList& args);
	// ��ұ�ɱ����֯�㲥
	static int OnCommandBeKilled(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args);
	// ���ȼ��ı�
	static int OnGuildLevelChange( IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args );
    // �����������Ϣ
    int OnCommandCreateGuildResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ����������빫������Ϣ
    int OnCommandApplyJoinGuildResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ����ȡ��������빫������Ϣ
    int OnCommandCancelApplyJoinGuildResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ���������߽�������������Ϣ
    int OnCommandAcceptResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ���������߼��빫��������Ϣ
    int OnCommandApplyJoinResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ���������߳ɹ��ܾ����������Ϣ
    int OnCommandRefuseJoin(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // �����˳�������
    int OnCommandQuitResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // �����ɢ������
    int OnCommandDismissResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // �����뿪����
    int OnCommandLeaveGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ���������
    int OnCommandDonateResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    // ��������Ʒ�۳�����ֵ�����Ϣ
    int OnCommandBuyItemResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// �ճ��
	static int OnCommandDailyActivity(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	//----------------------------------------------------------------------------------
	// ����
	void OnCommandTeachRequest(IKernel* pKernel, const PERSISTID& self, const IVarList& args, int nOffset);
	void OnCommandTeachAck(IKernel* pKernel, const PERSISTID& self, const IVarList& args, int nOffset);
	void OnCommandTeachFin(IKernel* pKernel, const PERSISTID& self, const IVarList& args, int nOffset);
	void OnCommandTeachFailed(IKernel* pKernel, const PERSISTID& self, const IVarList& args, int nOffset);

    ///////////////////////////////////////////////////////////////////////////
    // ��������������Ϣ
    ///////////////////////////////////////////////////////////////////////////
    // ���������ݷ������´�����Ϣ
    static int OnPublicMessage(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);


private:
	// ְλ�����䶯��ˢ��������ʾ
	int UpdateNewApplyByPosition(IKernel* pKernel, const PERSISTID& player);
	// ˢ����������߹���ļ���������ʾ
	int UpdateNewApplyToAllAuditor(IKernel* pKernel, const wchar_t* guildName);
	// ��ȡ����������¼��
	int GetGuildApplyCount(IKernel* pKernel, const wchar_t* guildName);
	
	// ˢ�°��ְλ
	void UpdateGuildInfo(IKernel* pKernel, const PERSISTID& self);

    //У���Ƿ񹫻��ڼ�
	bool CheckGuildActivity(IKernel* pKernel, const PERSISTID& self);

    // ˢ�°��ȼ�
	void UpdateGuildLevelToPlayer(IKernel* pKernel, const PERSISTID& self);
	//ˢ����֯����
	void RefreshGuildSkill(IKernel*pKernel, const PERSISTID& self);
	
	//����̵���Ʒ��Ϣ
	const GuildShopConfigItem_t* GetShopItemInfo(int itemIndex);
	////�Ƿ�����Ч�Ĺ�����
	//bool IsVaildGuildName(const wchar_t*guildName);
	static void ReloadConfig(IKernel* pKernel);

	// �������ߴ���
	void OnTeahHandleClientReady(IKernel* pKernel, const PERSISTID& self);
	// ͬ����������
	void SyncTeachCount(IKernel* pKernel, const PERSISTID& self);
	// ����ͬ��������Ϣ
	void SendToClientTeachCount(IKernel* pKernel, const PERSISTID& self, int nTeachCount, int nReciveCount) const;
	EmTeachResult CheckTeachAble(IKernel* pKernel, const PERSISTID& self, const wchar_t* pszName, EmTeachMethod method, bool bActive);
	// �������
	EmTeachResult SwapResult(EmTeachResult orig);
	// �����������
	void CleanTeach(IKernel* pKernel, IGameObj* player);
	// ׼������
	bool PrepareTeach(IKernel* pKernel, const PERSISTID& player, const wchar_t* pszTarget, EmTeachType type);
	// ����Ƿ������
	void CheckTeachPrepare(IKernel* pKernel, const PERSISTID& player);
	// ����Ƿ�׼������
	void CheckTeachReady(IKernel* pKernel, const PERSISTID& player);
	// ��������
	static int H_TeachAutoRun(IKernel* pKernel, const PERSISTID& self, int slice);

public:
    static GuildModule* m_pGuildModule;
private:
    static CapitalModule  *m_pCapitalModule;            // �ʽ�ģ��
    static ItemBaseModule *m_pItemBaseModule;           // ����ģ��
    static VirtualRecModule* m_pVirtualRecModule;       // �����ģ��
	static ChatModule* m_pChatModule;					// ����ģ��
	static SystemMailModule* m_pMailModule;				// �ʼ�ģ��

    GuildCreateConfig_t m_GuildCreateConfig;        // ��������������Ϣ
    GuildUpLevelConfigVector m_GuildUpLevelConfig;  // ����ȼ�������Ϣ
    GuildDonateConfigVector  m_GuildDonateConfig;   // �������������Ϣ
    GuildPositionConfigVector m_GuildPositionConfig;// ����ְλ����
	GuildShopConfigVector m_GuildShopConfig;        // �����̵�
	GuildSkillConfigMap		m_guildSkill;				//��֯����
	GuildSkillLevelUpSpendMap  m_guildSkillLevelUpCost; //  ��֯������������
	
	
	GUILDPREPAREINFO m_guildPrepareInfo;
};

#endif //__GuildModule_H__