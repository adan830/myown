#ifndef __GUILD_BASE_INFO_H__
#define __GUILD_BASE_INFO_H__
#include "GuildBase.h"


/*
��������˵����
1��GUILD_LIST_REC : �����б����������
2��GuildSortHelperSet : �����������֮�󣬽����й������򵽴�set��
3��GUILD_SORT_REC : ���ź����set�еĹ���ŵ�sort��¼��
4��ÿ����������������Ա仯ʱ����Ҫ��set���������򣬲����仯����ͬ����sort��¼�У�
��һ���Ĵ����Ǻ�С�ģ�����μ�����
5�������Կռ任ȡʱ�䣬ͬʱ������ش���Ҳ��֮ǰ������

6��������ѡ�񣺿���ȥ��sort��¼���Ӷ�������Ҫ��������Ĺ��ܶ��ŵ�public���������д���
*/
// ���ڹ�������ĸ����ṹ
struct GuildSortHelper_t
{
	GuildSortHelper_t() : guildName(L""), guildLevel(0), curMember(0), randDate(0), fightAbiliey(0) {}
	GuildSortHelper_t(const wchar_t* name, int nation, int lvl, int member, __int64 t, int ability)
		: guildName(name), guildLevel(lvl), curMember(member), randDate(t), fightAbiliey(ability) {}

	bool operator == (const GuildSortHelper_t &other) const
	{
		return guildName == other.guildName &&
			guildLevel == other.guildLevel &&
			curMember == other.curMember;
	}
	bool operator >= (const GuildSortHelper_t &other) const
	{
		if (fightAbiliey == other.fightAbiliey)
		{
			if (guildLevel == other.guildLevel)
			{
				if (curMember == other.curMember)
				{
					return randDate < other.randDate;   // ʱ��С������ǰ��
				}
				else
				{
					return curMember > other.curMember;
				}
			}
			else
			{
				return guildLevel > other.guildLevel;
			}
		}
		else{
			return fightAbiliey > other.fightAbiliey;
		}
		return false;
	}
	std::wstring guildName;
	int guildLevel;
	int curMember;
	__int64 randDate;
	int fightAbiliey;
};

typedef std::set<GuildSortHelper_t, std::greater_equal<GuildSortHelper_t> > GuildSortHelperSet;
typedef GuildSortHelperSet::iterator GuildSortHelperSetIter;










class GuildBaseManager :public GuildBase
{
public:
	GuildBaseManager():GuildBase(){
		m_pInstance = this;
	}
	~GuildBaseManager()
	{
		m_pInstance = NULL;
	}

	// ����������
	bool DumpGuildSortHelper(IRecord *guildRec, int row, GuildSortHelper_t *helper);
	void SortGuild(const GuildSortHelper_t *helperOld, const GuildSortHelper_t *helperNew);
	//������֯��Ա�������ֵ	
	void AddMemberGuildDevoteMax(const wchar_t *guildName ,const wchar_t*playerName,int value);
	bool CreateGuild(IPubKernel*pPubKernel, const wchar_t*guildName, int nation, int guildIndentifying, const wchar_t*guildShortName);

	//���벹���ӿ� ÿ����֯�������һ��
	virtual bool Patch(IPubData* guildData);
private:
	void PatchRecord(IPubData* guildData);
private:
	virtual bool Init(IPubKernel* pPubKernel);
	bool  InitPubGuildData(IPubKernel* pPubKernel);
	// ��ʼ����᳡��������Դ��
	int InitGuildStationGroupIDRec();

	// ��ʼ����᳡��������Դ����
	int InitGuildStationGroupIDData();
	// ��������
	bool LoadResource(IPubKernel* pPubKernel);
	// ���������������
	void CheckAndRefreshSortRecord();

	/********************/



	

	/****************************/


	///////////////////////////////////////////////////////////////////////////
	// ��������������Ϣ����
	///////////////////////////////////////////////////////////////////////////

	int OnPublicMessage(IPubKernel* pPubKernel, int source_id,
		int scene_id, const IVarList& msg);

	//��Ը
	void OnPetition(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);
	
	// �������������Ϣ
	int OnCreateGuild(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	
	// ����������빫�����Ϣ
	int OnApplyJoinGuild(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// һ��������빫����Ϣ
	int OnOneKeyApplyJoinGuild(IPubKernel* pPubKernel, int sourceId, int sceneId, const IVarList& args);

	// ����ȡ��������빫�����Ϣ
	int OnCancelApplyJoinGuild(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// ������ܼ��빫���������Ϣ
	int OnAcceptJoinGuild(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);
	// ����ͬ�������֯
	int OnInviteAgreeJoinGuild(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// ����ܾ����빫���������Ϣ
	int OnRefuseJoinGuild(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// �������Ա����״̬�仯����Ϣ
	int OnMemberOnlineStateChange(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// ����ɾ��������Ҽ��빫���¼����һ��
	int OnDeleteOutlineJoinRecord(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// �����޸Ĺ���
	int OnChangeNotice(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// �����޸�����
	int OnChangeDeclaration(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// �����˳���������
	int OnQuit(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// �����߳���������
	int OnFire(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// �����ɢ��������
	int OnDismiss(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// �������������
	int OnDonate(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// ����������
	int OnPromotion(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);
	// ���³�Ա����
	int OnUpdateProps(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// ��������������
	int OnUpdateApplyProps(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);
	// ��������������״̬
	void OnUpdateApplyOnline(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);
	//��ɢ����
	int DismissGuild(IPubKernel* pPubKernel, const wchar_t* guildName,
		int sourceId, int sceneId, const wchar_t *playerName);

	// ��������ƽ�
	int OnCaptainTransfer(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);
	// �᳤�ƽ�
	int TransferCaptain(const wchar_t* guildName, const wchar_t *oldCaptain, const wchar_t *newCaptain);
	
	// ����������Ϣ
	int OnSaveGuidChannelInfo(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	//�����輧�Զ�����ʱ��
	void OnSetDancingGirlAutoOpenTime(IPubKernel* pPubKernel, const wchar_t*guildName, int time);
	//���ù�����
	void OnSetGuildShortName(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);

	// ���´�������
	void OnUpdateTeachNum(IPubKernel* pPubKernel, const wchar_t*guildName, int sourceId, int sceneId, const IVarList& args, int nOffset);

	
	///////////////////////////////////////////////////////////////////////////
	// �ظ�������������Ϣ����
	///////////////////////////////////////////////////////////////////////////
	// ���ʹ�������Ľ��������������
	void SendCreateGuildResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
		const wchar_t* guildName, const wchar_t* playerName, int result);

	// ���ͽ���������빫��Ľ��������������
	void SendAcceptJoinGuildResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
		const wchar_t* guildName, const wchar_t* playerName,
		const wchar_t* applyName, int onLine, int result);

	// ���ʹ�������Ľ��
	void SendRequestResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
		const wchar_t* guildName, const wchar_t* playerName, int msgId, int result);

	// �����˳�����ظ�
	void SendQuitResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
		const wchar_t* guildName, const wchar_t* playerName, int result, const wchar_t* captainName);

	// �����߳�����ظ�
	void SendFireResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
		const wchar_t* guildName, const wchar_t* playerName, int result, int onLine, const wchar_t* memberName);

	// ֪ͨ��Ա�����ɢ
	void SendGuildGone(IPubKernel* pPubKernel, int sourceId, int sceneId,
		const wchar_t* guildName, const wchar_t* playerName, int onLine, const wchar_t* memberName);

	// ���ṱ�׽��
	void SendGuildDonate(IPubKernel* pPubKernel, int sourceId, int sceneId,
		const wchar_t* guildName, const wchar_t* playerName, int result,const IVarList & args);

	

	// ֪ͨ�����ƽ����
	void SendCaptainResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
		const wchar_t* oldCaptain, int result, const wchar_t* newCaptain, int oldCaptainPosition, const wchar_t* guildName);

	void SendChangeGuildShortNameResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
	const wchar_t* guildName,	 const wchar_t* playerName, int result);


	///////////////////////////////////////////////////////////////////////////
	// ���ܺ���
	///////////////////////////////////////////////////////////////////////////
	
	// ��ӹ��ᵽ�����б���
	bool AddGuildToGuildList(const wchar_t* guildName, const char* playerNameUid,
		const wchar_t* playerName, int playerLevel, int nation);

	// ��ӹ���˽�����ݱ��
	bool AddGuildTable( IPubData* pGuildData);

	// ��������˵���������б�����
	bool AddPlayerToApplyTable(IPubData* pGuildData, const char* pPlayerNameUid, const wchar_t* playerName,
		int playerSex, int playerLevel, int ability, int career,int vipLv);
	//֪ͨ����������µ�����
	void SendToGuildMemApplyNew(IPubKernel* pPubKernel, IPubData* pGuildData, int sourceId, int sceneId);
	// ���һ����¼���������б���
	bool AddRecordToApplyListTable(const char *playerUid, const wchar_t* playerName, const wchar_t* guildName);

	// ɾ����������б��еļ�¼
	bool DelPlayerFromApplyTable(IPubData* pGuildData, const wchar_t* playerName);

	// ɾ���������б������빫�����¼
	bool DelRecordFromApplyListTable(const wchar_t* playerName, const wchar_t* guildName);

	// ����Ƿ񻹿��Լ��빫��
	bool CanJoinGuild(const wchar_t* guildName,  int maxMember);

	// ��ȡ�����˵���Ϣ
	bool GetApplyInfo(const wchar_t* guildName,
		const wchar_t* applyName, int &sex, int& career, int& level, int& fight,int& vipLv);

	// ɾ�������ߵ�ȫ�������¼
	bool DelAllApplyRecord(const wchar_t* applyName);

	// ɾ����������һ�������е������¼
	bool DelGuildApplyRecord(const wchar_t* guildName, const wchar_t* applyName);

	// ɾ��һ������ȫ���������¼
	bool DelGuildAllApplyRecord(const wchar_t* guildName, bool needDeleteRecord);

	// ��������߼��빫����
	bool AddApplyJoinGuild(const wchar_t* guildName, const char* pApplyNameUid, const wchar_t* applyName,
		int sex, int career, int level, int fight, int onLine,int vipLv);

	// �޸Ĺ����б��¼�й��ᵱǰ����
	bool ChangeGuildCurMember(const wchar_t* guildName);

	// ������������߼��빫���¼
	bool AddOutlineJoinGuild(const wchar_t* guildName, const char* applyUid, const wchar_t* applyName);

	// ��ȡ�������ӿڼ���Ҽ�¼������
	IRecord* GetGuildDataOpt(const wchar_t* guildName, const wchar_t* playerName,
		const char* tableName, int playNameCol, int& row);

	// ��ȡ�������ӿ�
	IRecord* GetGuildDataRecOpt(const wchar_t* guildName, const char* tableName);

	// �򹫻��˽�����ݱ����д������
	bool SetGuildData(const wchar_t* guildName, const wchar_t* playerName,
		const char* tableName, int playNameCol, int col, const IVarList& value);

	// �ӹ����˽�����ݱ���л�ȡ����
	bool GetGuildData(const wchar_t* guildName, const wchar_t* playerName,
		const char* tableName, int playNameCol, int col, CVarList& value);

	// �򹫻�Ĺ������ݱ����д������
	bool SetGuildPubData(const wchar_t* guildName, const char* tableName,
		int guildNameCol, int col, const IVarList& value);

	// �ӹ���Ĺ������ݱ���л�ȡ����
	bool GetGuildPubData(const wchar_t* guildName, const char* tableName,
		int guildNameCol, int col, CVarList& value);

	// ���������ҵ���������˳�������
	//bool AddOutLineQuitGuild(const wchar_t* guildName, const char* pMemberNameUid, const wchar_t* memberName);
	

	// ͳ�Ƹ�����ɫ�������Ƿ񳬹����ֵ
	bool IsPositionFull(IRecord* memberList, int position, int maxNumber);

	//��ӹ��ᾭ��
	int OnAddGuildDefendExp(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);
	
	// ����µİ�᳡��������Դ����
	int AddGuildStationGroupIDData(const wchar_t* guildName);
	// �������������Ϊn�� ÿ������Ϊ1
	bool RandomSplitNumber(int value, int num, IVarList& list);
	//����ͨ����ֵ����Ϊ�¶����ֵ
	bool UpdateMemberPositionVal(IPubData* pPubGuildData);

	//ˢ�¹���ս����
	void RefreshGuildFightAbility(IPubKernel * pPubKernel, const wchar_t* guildName);
	//�Զ�ͬһ���빫��
	bool AutoAgreeJoinGuild(IPubKernel *pPubKernel, const wchar_t* guildName, const wchar_t* playerName, int sourceId, int sceneId);
public:
	//��־���ַ�
	void GetGuildLogWstring(const IVarList& args,std::wstring& logWsting);
	//��������֯�����־
	void SendAllGuildLog(int type, const IVarList& args);
	//��ͬ��Ӫ�����־
	void SendSameNationGuildLog( int nation, int type, const IVarList& args);
	//�����֯��־
	void SendGuildLog(const wchar_t *guildName, int type, const IVarList& args);
	//static void AutoImpeachTimer(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name);
	static void DancingGirlReset(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name);
	// ���˴�������
	static void FireNumResetTimerCb(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name);

	//�����Ը
	void ClearPetition(IPubKernel* pPubKernel, const wchar_t* guildName, int type);
	// �Ƴ���Ը���
	void RemovePetition(IPubKernel* pPubKernel, const wchar_t* guildName, const wchar_t* pszName);

	// ��ù�������
	IPubData* FindGuildData(const wchar_t* guildName) const;
public:
	// ��¼�����־
	int RecordGuildLog(const wchar_t* guildName, int type, const wchar_t* args);
private:
	///////////////////////////////////////////////////////////////////////////
	//���� 
	///////////////////////////////////////////////////////////////////////////	
	// ��������
// 	bool LoadGuldImpeach(IPubKernel* pPubKernel);
// 	//��ʼ����
// 	int OnStartImpeach(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);
// 	//��������
// 	int OnUndoImpeach(IPubKernel* pPubKernel, const wchar_t* guildName, const char* pSelfUid);
// 	//��Ӧ����
// 	int OnRespondImpeach(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);
// 	//���㵯��
// 	bool SettleImpeach(IPubKernel* pPubKernel, const wchar_t* data_name);
// 	//�Զ�����
// 	static void AutoImpeach(IPubKernel* pPubKernel, const char* space_name, const wchar_t* guildName, const wchar_t* capName);
// //��ʼ������ɼ�鶨ʱ��
// 	void StartImpeachTimer(IPubKernel* pPubKernel, const wchar_t* guildName);
// 	//������ɼ�鶨ʱ���ص�
// 	static int OnImpeachExamine(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name, int time);
	
	public:
	static GuildBaseManager* m_pInstance;


	public:
		void OnZeroClock( IPubKernel* pPubKernel );
	private:

	GuildUpLevelConfigVector m_GuildUpLevelConfig;  // ����ȼ�������Ϣ
	GuildSortHelperSet m_GuildSortHelperSet;    // ���ڹ�������

	static int m_autoImpeachMaxTime; // �Զ�������������ʱ��
	static int m_memOffLineTimes;	 //�Զ�����ʱ,ɸѡ��֯������Ա����ʱ�� 
	static int m_stationGroup;  //פ�س�������
};

#endif