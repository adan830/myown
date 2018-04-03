//--------------------------------------------------------------------
// �ļ���:		TeamOfferModule.h
// ��  ��:		�������ģ��
// ˵  ��:		
// ��������:		2017��01��13��
// ������:		tongzt
// �޸���:		 
//--------------------------------------------------------------------
#ifndef _TEAM_OFFER_MODULE_H_
#define _TEAM_OFFER_MODULE_H_

#include "Fsgame/Define/header.h"
#include "TeamOfferDefine.h"

class TeamOfferModule : public ILogicModule
{
public:
	// ��ʼ��
	bool Init(IKernel* pKernel);

	// �ر�
	bool Shut(IKernel* pKernel);

	// ������Դ
	bool LoadRes(IKernel* pKernel);

private:
	// ���ضӳ���������
	bool LoadCaptainBox(IKernel* pKernel);

	// ���ؽ�������
	bool LoadAwardRule(IKernel* pKernel);

private:
	// ȡ�öӳ�����
	const int GetCaptainBox(const int offer_num);

	// ȡ�ý�������
	const OfferAwardRule* GetAwardRule(const int offer_num, const int level);
	
private:
	// �ɷ��ȡ��������
	bool CanAcceptOfferTask(IKernel* pKernel, const PERSISTID &self);

	// ��Ա�ɷ�������ͻ
	bool CanTeammemberDo(IKernel* pKernel, const PERSISTID &self);

	// ��Ա�ɷ�������ͷ���
	bool CanTeammemberEntry(IKernel* pKernel, const PERSISTID &self, 
		const int task_id);

	// �Ƿ�ɷ����������
	bool CanLaunch(IKernel* pKernel, const PERSISTID &self, 
		bool check_entry_npc = true);

	// ���ͳ�����֤
	bool IsVaildOfferScene(IKernel* pKernel, const PERSISTID &self, 
		const int task_id, const int scene_id);

	// �������ƽ���ȼ�
	const int GetAverageLvl(IKernel* pKernel, const PERSISTID &self);

public:
	// ���׼������
	void PlayerReady(IKernel* pKernel, const PERSISTID &self, 
		const IVarList& args);

	// �������
	void PlayerOnline(IKernel* pKernel, const PERSISTID &self);

	// ��������
	void Launch(IKernel* pKernel, const PERSISTID &self,
		bool check_entry_npc = true);

	// �ӳ���������ͬ��
	bool SyncTask(IKernel* pKernel, const PERSISTID &self, 
		const IVarList &args);

	// ��Աͬ������
	bool MemberSyncTask(IKernel* pKernel, const PERSISTID &self,
		const IVarList &args);

	// ɾ����������
	void DeleteTask(IKernel* pKernel, const PERSISTID &self);

	// �ӳ�������븱��
	void EntryGroup(IKernel* pKernel, const PERSISTID &self, 
		const int team_id);

	// ��Ա�������
	void FollowEntry(IKernel* pKernel, const PERSISTID &self);

	// �˳�����
	void LeaveGroup(IKernel* pKernel, const PERSISTID &self);

	// ������ͽ������
	void OnResult(IKernel* pKernel, const IVarList& args);

	// ����������ͽ��
	void ProcResult(IKernel* pKernel, const PERSISTID &creator,
		const IVarList& args);

	// �������
	void AddTeam(IKernel* pKernel, const PERSISTID &self);

	// �˳�����
	void QuitTeam(IKernel* pKernel, const PERSISTID &self, 
		const int old_team_id);

	// ����������һ������
	void ContinueNext(IKernel* pKernel, const PERSISTID &self);

	// ��������
	void RewardTeamOffer(IKernel* pKernel, const PERSISTID &self, 
		const int pass_time);

	// ������Ϣ����������
	void SendS2PMsg(IKernel* pKernel, const IVarList& s2p_sub_msg);

	// ȡ�ù���������
	IPubData* GetPubData(IKernel* pKernel);

	// ���¼���������ͻ����
	static void ReloadTeamofferConfig(IKernel* pKernel);

private:
	// �ͻ�����Ϣ
	static int OnCustomMsg(IKernel* pKernel, const PERSISTID &self, 
		const PERSISTID & sender, const IVarList & args);

	// �������Npc��ɱ�ص�
	static int OnOfferNpcBeKilled(IKernel* pKernel, const PERSISTID& offer_npc,
		const PERSISTID& killer, const IVarList& args);

	// �������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// ����뿪����
	static int OnPlayerLeaveScene(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID &sender, const IVarList &args);

	// ���볡��
	static int OnPlayerEntry(IKernel * pKernel, const PERSISTID & self, 
		const PERSISTID & sender, const IVarList & args);

	// �ͻ���׼������
	static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// ����
	static int OnStore(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// ����ڲ���Ϣ
	static int OnCommandMsg(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ˢ�����ڲ���Ϣ
	static int OnCreatorCommandMsg(IKernel *pKernel, const PERSISTID &creator,
		const PERSISTID &sender, const IVarList &args);

	// �������
	static int OnCommandTaskProc(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ����仯�ص�
	static int C_OnTeamChange(IKernel* pKernel, const PERSISTID& self, 
		const char *property, const IVar& old);

	// ������ͽ�������
	static int HB_TeamOfferEnd(IKernel* pKernel, const PERSISTID& creator,
		int slice);

	// ������͸�����������
	static int HB_Contrl(IKernel* pKernel, const PERSISTID& creator,
		int slice);

	// ���������ݷ������´�����Ϣ
	static int OnPublicMessage(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	static const std::wstring & GetDomainName(IKernel * pKernel);

private:
	CaptainBoxVec m_CaptainBoxVec;			// �ӳ�����
	OfferAwardRuleVec m_OfferAwardRuleVec;	// ���˽����ܹ���

	static std::wstring m_domainName;

public:
	static TeamOfferModule *m_pTeamOfferModule;
};

#endif	// _TEAM_OFFER_MODULE_H_
