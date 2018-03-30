//---------------------------------------------------------
//�ļ���:       RewardModule.h
//��  ��:       ���Ž��������
//˵  ��:       
//          
//��������:      2015��03��30��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#ifndef REWARD_MODULE_H_
#define REWARD_MODULE_H_

#include "Fsgame/Define/header.h"
#include "Fsgame/Define/GameDefine.h"
#include "public/IVarList.h"
#include "FsGame/Define/RewardDefine.h"
#include <map>
#include <vector>
#include "Fsgame/Define/FunctionEventDefine.h"

class ToolBoxModule;
class ContainerModule;
class CapitalModule;
class SystemMailModule;
class LevelModule;

class RewardModule : public ILogicModule
{
public:

	virtual bool Init(IKernel* pKernel);

	virtual bool Shut(IKernel* pKernel);

public:
	// ��������
	struct AwardData 
	{
		std::string strType;	// ����
		int			nAddVal;	// ���ӵ�ֵ
	};

	typedef std::vector<AwardData> AwardVec;

	struct AwardEx
	{
		AwardEx() : name(""), srcFunctionId(EmFunctionEventId(-1)) {}

		//int				nAwardId;	// ����id
		std::string		name;    // ���������֣�Ҫ���ʼ�������(ini/mail/mail_config)�ı���һ��
		// �����ʼ����Ž���
		std::string		strCapitals;	
		std::string		strItems;

		// ֱ�ӷ��Ž���
		AwardVec		capitals;
		AwardVec		items;

		// ***ע�⣺���鲻��ͨ���ʼ��������
		// ����Ҳ����ߵ�ʱ�򣬾���ᱻ����
		AwardVec		exps;

		// NOTE: ���еľ��� ��Ʒ ������Դ��ͳһʹ��	
		EmFunctionEventId srcFunctionId;		// ������Դ����ID(��Ӧ(EmFunctionEventId))
	};

	typedef std::map<int, AwardEx*> AwardMap;
public:
	// ���Է������������
	bool TryRewardPlayer(IKernel *pKernel, const PERSISTID &player, int nAwardId, int operation_on_full);

    // �������������
    // �� @operation_on_full = REWARD_MAIL_ON_FULL ��ʱ��, ��ͨ��ʹ��Ԥ������ʼ��������.
    // ����ʼ�ģ�����в�������ͨ��@mail_param ����
	bool RewardPlayerById(IKernel *pKernel, const PERSISTID &player, 
		int nAwardId, int operation_on_full = REWARD_MAIL_ON_FULL, const IVarList &mail_param = CVarList());

	// ͨ����ɫ�����Ž���, ֱ��ͨ���ʼ�������ɫ
	// ����, @award.name ���ʼ�ģ�������, ����ʼ�ģ�����в�������ͨ��@mail_param ����
	//bool RewardByRoleName(IKernel *pKernel, const wchar_t *role_name, int nAwardId, const IVarList &mail_param = CVarList());

    // ͨ���ʼ������������
	bool RewardViaMail(IKernel *pKernel, const wchar_t *role_name, int nAwardId, const IVarList &mail_param, const wchar_t *sender_name = L"");


	// �������ɶ�̬����,Ȼ�󷢷Ÿ����

	// ��������Ʒ������Ϊ��������
	// ��ʽ��  "CapitalCopper:10,CapitalGold:10,CapitalSmelt:10,CapitalPublicOffice:10,CapitalBattleSoul:10,CapitalBlackShop:10,CapitalGuild:10"
	static bool GenerateAwardData(const std::string& strAward, RewardModule::AwardEx& award);

	// �������������
	// �� @operation_on_full = REWARD_MAIL_ON_FULL ��ʱ��, ��ͨ��ʹ��Ԥ������ʼ��������.
	// ����, @award.name ���ʼ�ģ�������, ����ʼ�ģ�����в�������ͨ��@mail_param ����
	bool RewardPlayer(IKernel *pKernel, const PERSISTID &player, const AwardEx *award, const IVarList &mail_param, int operation_on_full = REWARD_MAIL_ON_FULL);

	//չʾ��Ʒ
	static void ShowItem(IKernel*pKernel,const PERSISTID &player,const std::string& awardStr);
private:
	// ��ȡȫ����������
	bool LoadAllRewardConfig(IKernel *pKernel);

	// ��ȡ����
	bool LoadRewardConfig(IKernel *pKernel, const char* strPath);

    // ͨ���ʼ����ͽ���
	bool InnerRewardViaMail(IKernel *pKernel, const wchar_t *role_name, const AwardEx* award, const IVarList &mail_param, const wchar_t *sender_name = L"");
    
    // ֱ�ӽ��������
    bool InnerRewardDirectly(IKernel *pKernel, const PERSISTID &player, const AwardEx *award);

    // �������ֱ�ӼӸ����
	bool AddExp(IKernel *pKernel, const PERSISTID &player, const AwardVec& exp_list, int exp_from);

    // �ʽ����ֱ�ӼӸ����, @capital �ĸ�ʽ��capital_name:����,capital_name:����,
	bool AddCapital(IKernel *pKernel, const PERSISTID &player, const AwardVec& capital_list, EmFunctionEventId capitalEventID);
    
    // �Ƿ���Ա��浽������
	bool CanPutInToolBox(IKernel *pKernel, const PERSISTID &player, const AwardVec& items_list, int nBindState);
    
    // ���뱳����
	bool PutInToolBox(IKernel *pKernel, const PERSISTID &player, const AwardVec& item_list, EmFunctionEventId itemEventID, int nBindState);

	// ��������Ʒ������Ϊ��������
	// ��ʽ��  "CapitalCopper:10,CapitalGold:10,CapitalSmelt:10,CapitalPublicOffice:10,CapitalBattleSoul:10,CapitalBlackShop:10,CapitalGuild:10"
	static void ParseAddAwardData(const char* strAwardData, AwardVec& vecAward);

	// ����һ��� nAwardType (��ӦAwardType)
	static void ParseAddOneAwardData(const char* strAward, int nAwardType, AwardEx& award);

	// ��ѯ��������
	const AwardEx* RewardModule::QueryAwardById(int nAwardId);

	static bool ItemsToString(const AwardVec &items_list, std::string& strItems, const char* pszSp1 = ",", const char* pszSp2 = ":");
	static bool CapitalsToString(const AwardVec &capital_list, std::string& strCapitals, const char* pszSp1 = ",", const char* pszSp2 = ":");
public:
	// ����Ʒ��������ְҵ����Ʒ�ų�
	// ��ʽ��  "ConfigID:����,ConfigID:����"
// 	static bool FilterAwardByJob(IKernel* pKernel, const PERSISTID& self, AwardVec& items);
// 	static bool FilterAwardByJob(IKernel* pKernel, int job, AwardVec& items);

public:

	static RewardModule *m_pRewardInstance;
    
private:
	AwardMap		m_mapAllAwardData;		// ���н�������

	static ContainerModule *m_pContainerModule;
	static CapitalModule *m_pCapitalModule;
	static SystemMailModule *m_pMailModule;
    static LevelModule *m_pLevelModule;
};

#endif