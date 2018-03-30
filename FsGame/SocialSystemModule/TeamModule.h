//--------------------------------------------------------------------
// �ļ���:		TeamModule.h
// �� ��:		    ���ģ��
// ˵ ��:		
// ��������:	2014��11��21��
// ������:		 
//    :	    
//--------------------------------------------------------------------

#ifndef _TEAM_MODULE_H_
#define _TEAM_MODULE_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/RequestDefine.h"
#include "FsGame/Define/TeamDefine.h"
#include "FsGame/SocialSystemModule/FriendModule.h"
#include <map>
#include <string>
#include <vector>
#include <set>

class RequestModule;
class CloneSceneModule;
class CloneSceneInfoWrap;
class CBattleWarModule;

// ������Ҿ���
#define TEAM_NEARBY_RANGE	20.0f


// ����ģ��
class TeamModule : public ILogicModule
{
public:
    // ��ʼ��
    virtual bool Init(IKernel* pKernel);

    // �ͷ�
    virtual bool Shut(IKernel* pKernel);

public:

    // �Լ��Ƿ��ڶ�����
    virtual bool IsInTeam(IKernel* pKernel, const PERSISTID& self);

    // �Լ��Ƿ�ӳ�
    virtual bool IsTeamCaptain(IKernel* pKernel, const PERSISTID& self);

    // �Ƿ���ʰȡ��Ʒ��Ȩ��
    virtual bool HaveRightPickUp(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target_obj , const wchar_t* member_name);

    // ��ö��������б�
    virtual bool GetTeamMemberList(IKernel* pKernel, const PERSISTID& self, IVarList& lst);

	// ��ö�Ա�����б�
	virtual void GetTeamMemberList(IKernel* pKernel, const int team_id, IVarList& lst);

	// ��ȡ�������¸�����������Ϣ
	virtual bool GetNearByTeamInfo(IKernel* pKernel, PERSISTID playID, IVarList& lst);

    // ��öӳ���
    virtual const wchar_t* GetCaptain(IKernel* pKernel, const PERSISTID& self);

    // ��ȡ��ӹ���������ָ��
    virtual IPubData* GetTeamPubData(IKernel* pKernel);

	// ���¸���ID���������ݷ�����
	void UpdateCloneToDomain(IKernel* pKernel, const PERSISTID& self, int clone_id);

	// ��Ӷ����Ա���Իص�
	void AddCriticalCallBack(IKernel* pKernel, const PERSISTID& self);

	// ��������Ա���Իص�
	void RemoveCriticalCallBack(IKernel* pKernel, const PERSISTID& self);

	//���񼤻��
	static int OnCommandTask(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��ȡdomain������
	static const std::wstring& GetDomainName(IKernel * pKernel);

	// �Է��Ƿ��Լ�����
	static bool IsTeamMate(IKernel* pKernel, const PERSISTID& self, const wchar_t* target_name);

	//�ܷ��߳�����
	static bool IsBeKickTeam(IKernel* pKernel, const PERSISTID& self, const wchar_t* target_name);
	// �����Ƿ�����
	static bool IsFull(IKernel* pKernel, const PERSISTID& obj);
	//�Ƿ����Զ��������
	static bool IsAutoJoin(IKernel*pKernel, int teamID);
	//�ﵽ����ȼ�
	bool IsAchieveTeamLevel(IKernel*pKernel, const PERSISTID& self,int teamID);

	//���õ�ǰ����ܷ���
	void SetTeamMemKick(IKernel*pKernel, const PERSISTID& self,int kickState);

	//��ȡ����Ŀ������
	static int GetTeamObjType(IKernel*pKernel, int teamID);
	//�������ͻ��id
	int GetTeamIndexByType(int type);
	//����objID ��ȡtype
	int GetTeamIndexByObjectID(int objID);
	//�Զ����Ķ���Ŀ��
	void AutoChangeTeamIndex(IKernel*pKernel, const PERSISTID& self,int index);
	//�����Ƿ��ܼ������TEAM_JOIN_TEAM_ON, //�ܹ�������� TEAM_JOIN_TEAM_OFF, //���ܹ��������
	void SetJoinTeamState(IKernel* pKernel, const PERSISTID& self, int state);

	// ����׼��״̬
	void UpdateReadyState(IKernel* pKernel, const PERSISTID& self, int ready_state);

	void CustomMessageToTeamMember(IKernel*pKernel, const PERSISTID& self, const IVarList&msg,bool customSelf = false);
	//�Ƿ��ǻ����
	bool IsActivityScene(IKernel*pKernel);
private:

    // �������
    static int OnRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// �뿪��Ϸ
	static int OnStore(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // ���׼��
    static int OnReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ����
	static int OnContinue(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // �������ݷ������´�����Ϣ
    static int OnPublicMessage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // �����������
    static bool CanRequestJoinTeam(IKernel* pKernel, const PERSISTID& self, REQUESTTYPE type, const wchar_t* targetname, const IVarList& paras);

    static bool CanBeRequestJoinTeam(IKernel* pKernel, const PERSISTID& self, REQUESTTYPE type, const wchar_t* srcname, const IVarList& paras);

    static bool RequestJoinTeamSuccess(IKernel* pKernel, const PERSISTID& self, REQUESTTYPE type, const wchar_t* targetname, const IVarList& paras);

    static bool GetRequestParas(IKernel* pKernel, const PERSISTID& self, REQUESTTYPE type, IVarList& paras);

    static bool RequestJoinTeamResult(IKernel* pKernel, const PERSISTID& self, REQUESTTYPE type, const wchar_t* targetname, int result);

    // ��ӿͻ�����Ϣ
    static int OnCustomMessage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��ӷ������Ϣ
	static int OnCommandMessage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // ��������
    static int OnCustomCreateTeam(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // �߳�����
    static int OnCustomKickOut(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // �˶�
    static int OnCustomLeave(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    //��ɢ����
    static int OnCustomDestroy(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // �ƽ��ӳ�
    static int OnCustomChangeCaptain(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��������
	static int OnCustomNearByTeam(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ���¶�Աλ��
	static int OnCustomUpdateMemberPosition(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ����Ƚ�
	static bool CompareDistance(const NearbyTeamRecommend& first, const NearbyTeamRecommend& second);

    //����������
    static int OnCustomClearApply(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
    //��ȡ�����б�
    static int OnCustomApplyList(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	//���ö��鹫������
	static int OnCustomSetViewState(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	//���ö����Զ���������
	static int OnCustomSetAutoJoin(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	//��ȡ������Ϣ
	static int OnCustomGetTeamInfo(IKernel* pKernel, const PERSISTID& self);
	//���ö���Ŀ��
	static int OnCustomSetTeamObject(IKernel* pKernel, const PERSISTID& self, int index);
	//���ö���ȼ�����
	static int OnCustomSetTeamLimitLevel(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	//���Ͷ����б�
	static void OnCustomSendTeamList(IKernel*pKernel, const PERSISTID& self, int objID);
	//��ǰ��������ƥ��״̬
	bool IsAutoMatch(IKernel*pKernel, const PERSISTID& self);
	//ȥ�ӳ�����
	static void OnCustomTransferCapTainSite(IKernel*pKernel, const PERSISTID& self);
	//�ټ�
	static void OnCustomConvene(IKernel*pKernel, const PERSISTID& self);
	//���ø���
	static void OnCustomSetFollow(IKernel*pKernel, const PERSISTID& self,int state);
	//��ȡ�Ƽ�����б�
	static void OnCustomRequestRecommandList(IKernel*pKernel, const PERSISTID& self);
	
private:
    // �����Ա����
    static int OnCommandTeamMemberRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // �����Ա�仯
    static int OnCommandTeamMemberChange(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // ���鴴�����
    static int OnCommandCreateTeamResult(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // �������
    static int OnCommandJoinTeam(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // ��ӳ�Ա
    static int OnCommandAddMember(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // ɾ����Ա
    static int OnCommandRemoveMember(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    //��ն���
    static int OnCommandClearTeam(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // ���³�Ա��Ϣ
    static int OnCommandUpdateMemberInfo(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // ���¶�����Ϣ
    static int OnCommandUpdateTeamInfo(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ������������б���Ϣ
	static int OnCommandClearNearbyTeam(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ���¶����Աλ��
	static int OnCommandUpdateMemberPosition(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	// ��ȡ�������
	static void OnCommandGetTargetPos(IKernel*pKernel, const PERSISTID& self, const IVarList& args);
	//���͵�Ŀ�긽��
	static void OnCommandTransferToTarget(IKernel*pKernel, const PERSISTID& self, const IVarList& args);

private:
	// �ȼ����� 
	static int OnCommandLevelUp(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID &sender, const IVarList &args);

    // Ѫ���ص�
    static int C_OnHPChange(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);

    // ���Ѫ���ص�
    static int C_OnMaxHPChange(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);

    // �������仯
    static int C_OnGuildNameChange(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);

	// ְҵ�仯�ص�
	static int C_OnJobChanged(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);

	// ս�����仯�ص�
	static int C_OnHighestBattleAbilityChange(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);
	// ս��ս��״̬�ص�
	static int C_OnFightState(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);
	// ���¶�Աλ������
	static int H_UpdateMemberPos(IKernel* pKernel, const PERSISTID& self, int slice);

	// VIP�����ص�
	static int C_OnVIPLevelChanged(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);

private:

    // ��������������Ϣ����ɫ������
    void RefreshAllTeamInfo(IKernel* pKernel, const PERSISTID& self, int team_id);

    // �����ɫ���������ж�����Ϣ
    void ClearTeamInfo(IKernel* pKernel, const PERSISTID& self);

    // ���¶���������Ϣ����ɫ������
    void RefreshTeamMainInfo(IKernel* pKernel, const PERSISTID& self, int col);

    // ���¶����Ա��Ϣ����ɫ������
    void RefreshTeamMemberInfo(IKernel* pKernel, const PERSISTID& self, const wchar_t* memeber_name, int col);

    // ��ӳ�Ա
    void AddTeamMember(IKernel* pKernel, const PERSISTID& self, const wchar_t* member_name);

    // ɾ����Ա
    void RemoveTeamMember(IKernel* pKernel, const PERSISTID& self, const wchar_t* member_name);

    // ���½�ɫ���Ե��������ݷ�����
    void UpdatePropToDomain(IKernel* pKernel, const PERSISTID& self, const IVarList& prop_list);

    // �Լ����ߴ���
    void OnSelfRecover(IKernel* pKernel, const PERSISTID& self);

	//������ҽ������
	static void OnCommandCheckConditionRsp(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	//�������
	static bool OnCommandCheckConditionReq(IKernel* pKernel, const PERSISTID& self, int reqType, int index,const wchar_t*ResultinfromName);
	//�����Ա�����Ƿ�����
	static int OnCommandCheckCondition(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	//��ǰ�����Ƿ��������
	bool CanJoinTeam(IKernel*pKernel,int teamID);
	//�Զ�ƥ��
	static void AutoMatch(IKernel*pKernel, const PERSISTID &self, int autoMatch, int objIndex = 0);
public:
	//��öӳ���
	const wchar_t *GetTeamCaptain(IKernel*pKernel,int teamID );

	//�����ܹ�ƥ��Ĺ���
	static bool AddCheckTeamRule(int typeID, CheckTeamCondition rule);

	//����Ƿ������������
	static bool DealCheckTeamCondition(IKernel*pKernel, const PERSISTID& self, int index, const wchar_t*ResultinfromName );
	bool LoadRes(IKernel*pKernel);
	bool LoadTeamRule(IKernel*pKernel);
	bool LoadActivitySccene(IKernel*pKernel);

	static bool IsTeamValid(IKernel*pKernel, const PERSISTID& self, int objIndex);
	//����Ƿ��������ȼ�
	 static int GetObjMinLimitLevel( int objIndex);
	 //���ö������Ƶȼ�
	 static void ResetTeamLimitLevel(IKernel*pKernel, const PERSISTID& self,  int objIndex);
	//��ö���������Ŀ��id
	int GetTeamIndexObjID(int index);
	//�����ҷ��ؽ��
	static bool AddCheckTeamResult(int typeID, CheckTeamResult rule);
	//��ȡ����Ŀ������
	static int GetTeamObjIndex(IKernel *pKernel, int teamID);
	static int GetTeamObjID(IKernel*pKernel, int teamID);
	//���ݴ���һ�����Ͷ���
	bool CreateTeamByType(IKernel*pKernel, const PERSISTID& self, int teamType);

	static void ReloadConfig(IKernel*pKernel);

public:
	static void LeaveTeam(IKernel* pKernel, const PERSISTID& player);

public:
    // ģ��ָ��
    static TeamModule* m_pTeamModule;
    static RequestModule* m_pRequestModule;
	static FriendModule* m_pFriendModule;
	// ������������
	static std::wstring m_domainName; 

	struct TeamRule
	{
		TeamRule()
		{
			m_teamType = 0;
			m_objectID = 0;
			m_limitLevel = 0;
		};

		struct TeamWeekTime
		{
			int weekDay; //�ܼ�
			int daySec; //ÿ������
			TeamWeekTime() :weekDay(0), daySec(0)
			{}
		};

		int m_teamType;
		int m_objectID; //Ŀ��id ����������ؼ����ǳ���id
		int m_limitLevel;
		
		std::string m_finishAward;
		TeamWeekTime m_openTime;
		TeamWeekTime m_endTime;

	};


	//���ƥ��
	static std::map<int, CheckTeamCondition> m_check_team_condition;
	//��������
	static std::map<int, CheckTeamResult> m_check_team_result;

	static std::map<int, TeamRule> m_TeamRule;
	//�����	
	std::set<int> m_activityScene;

};

#endif //_TEAM_MODULE_H_