#ifndef __GUILD_BUILDING_MANAGE_H__
#define __GUILD_BUILDING_MANAGE_H__
#include "GuildBase.h"
#include <map>
#include "GuildNumManage.h"
#include "utils\string_util.h"

#define CLOSE_SERVER_UNIX_TIME "close_server_unix_time" //�ط�ʱ���

#define BUILD_EXP_CYCLE_TIME 2000 //����������������

class CheckBuildLvUpCondition{
public:
	virtual int  Run(const wchar_t* guildName) = 0;
	virtual void LoadXml(const char* strValue) = 0;
};

class DoDecCostActionByBuildLvUp{
public:
	virtual bool  Run(const wchar_t* guildName) = 0;
	virtual void LoadXml(const char* strValue) = 0;
};

class GuildBuildingManage :public GuildBase{

public:
	GuildBuildingManage(){
		m_pInstance = this;
	}
	virtual bool Init(IPubKernel* pPubKernel);
	bool CreateGuildInit(IPubData* guildData);
	virtual int OnPublicMessage(IPubKernel* pPubKernel, int source_id,
		int scene_id, const IVarList& msg);
	
	//������ڽ����еĽ���������
	int GetBuildingLvingNum(IPubKernel* pPubKernel, const wchar_t* guildName);
	//���ά���ʽ�����
	int GetMainTainBuildingCost(const wchar_t*guildName);
	static GuildBuildingManage* m_pInstance;
	bool LoadResConfig(IPubKernel*pPubKernel);
	
	
	//���ӽ�������
	void OnAddGuildildBuildExp(IPubKernel*pPubKernel,const wchar_t*guildName,int expValue);

	int GetBuildingLevel(const wchar_t* guildName,GUILD_BUILDING_TYPE type );

	//��ʱά��
	static void TimingMaintain(IPubKernel*pPubKernel, const char* space_name, const wchar_t* data_name);
	//ά���Ƿ�ɹ�
	bool IsMaintain(const wchar_t*guildName);
	// ֪ͨ����Ա ���ȼ��ı�
	int CommandGuildLevelChange(IPubKernel* pPubKernel, const wchar_t* guildName, int level, bool bUp);
	// ֪ͨ����Ա ��������
	void BroadcastBuildingLevelup(IPubKernel* pPubKernel, const wchar_t* guildName, int nLevel, int building);
	bool OnLevelUpBuilding(IPubKernel* pPubKernel, int source_id,
		int scene_id, const IVarList& msg);
	// ���״̬����
	static int Update(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name, int time);

	void AddAllGuildBuildingExp(IPubKernel* pPubKernel,int exp);
private:
	void GuildLvUp(IPubKernel* pPubKernel,const wchar_t*guildName,int level);
	//��֯����������������	
	bool LoadGuildBuingLvUpExpConfig(IPubKernel*pPubKernel);
	//��֯������������
	bool LoadGuildBuingLvUpConfig(IPubKernel*pPubKernel);
	//��֯ά����������
	bool LoadGuildBuildingMaintainCost(IPubKernel*pPubKernel);

	
	void SendGuildingLevelUpResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
		 const wchar_t* playerName, int result);
	int GetBuildingLvExp(int buildingType,int level);
	
	
	//ֹͣ�о�
	bool StopLevelUpBuilding(IPubKernel* pPubKernel, int source_id,
		int scene_id, const IVarList& msg);


	//�����������
	std::map<std::string, std::function<CheckBuildLvUpCondition*(void)> > m_CheckBuildingLvUpConditoinList;

	//<��������,<�����ȼ�,��麯��>>
	std::map<int, std::map<int, std::vector<CheckBuildLvUpCondition*>>> m_checkBuildingLvUpCondition;
	//�۳���������
	std::map<std::string, std::function<DoDecCostActionByBuildLvUp*(void)>> m_DoDecCostActionByBuildingLvUpList;
	//<��������,<�����ȼ�,���ĺ���>>
	std::map<int, std::map<int, std::vector<DoDecCostActionByBuildLvUp*>>> m_doDecCostActionByBuildingLvUp;
	//��������������
	std::map<int, std::map<int,int>> m_guildBuildingLvUpExp;
	//ά����  <��������,<�����ȼ�,����>>
	std::map<int, std::map<int, int>> m_guildBuildingMaintainCost;

};

enum
{
	BUILD_LVUP_RET_ERROR		= -1,
	BUILD_LVUP_RET_SUCCESS,
	BUILD_LVUP_RET_NO_ENOUGH_GUILD_CAPITAL,			 //�����ʽ���
	BUILD_LVUP_RET_NO_ENOUGH_GUILD_FANRONGDU,			 //���ٶȲ���
	BUILD_LVUP_RET_NO_ENOUGH_GUILD_BUILD_TYPE,			 //���������಻��
	BUILD_LVUP_RET_NO_ENOUGH_GUILD_BUILD_LEVEL,		 //������ȼ�����
	BUILD_LVUP_RET_NO_ENOUGH_GUILD_BUILD_JUYITING_LEVEL,//�������ȼ�����
	BUILD_LVUP_STOP_BUILDING_ERROR,//����ֹͣ����ʧ��
	BUILD_LVUP_STOP_BUILDING_SUCCESS,//����ֹͣ����ɹ�
	BUILD_LVUP_BUILDING_SAME_TIME_MAX_NUM,//����ͬʱ���轨���������ֵ   
	BUILD_LVUP_MI_MAO_ENOUGHT, // �ر���������
};




//�����֯�ʽ�
class CheckGuildCapital:public CheckBuildLvUpCondition{

public:
	int  Run(const wchar_t* guildName){
		if (GuildNumManage::m_pInstance->GetGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL) >= m_value)
		{
			return BUILD_LVUP_RET_SUCCESS;
		}
		return BUILD_LVUP_RET_NO_ENOUGH_GUILD_CAPITAL;
	}

	void LoadXml(const char* strValue)
	{
		m_value = StringUtil::StringAsInt(strValue);
	}
private:

	int m_value{0};
};
//���������ʽ�
class CheckMainTainCostAndSelfGuildCapital :public CheckBuildLvUpCondition{

public:
	int  Run(const wchar_t* guildName){
		int cost = m_value + GuildBuildingManage::m_pInstance->GetMainTainBuildingCost(guildName);
		if (GuildNumManage::m_pInstance->GetGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL) >= cost )
		{
			return BUILD_LVUP_RET_SUCCESS;
		}
		return BUILD_LVUP_RET_NO_ENOUGH_GUILD_CAPITAL;
	}
	void LoadXml(const char* strValue)
	{
		m_value = StringUtil::StringAsInt(strValue);
	}
private:

	int m_value{0};
};


//��鷱�ٶ�
class CheckGuildFanRongDu :public CheckBuildLvUpCondition{
public:

	int  Run(const wchar_t* guildName){
		if (GuildNumManage::m_pInstance->GetGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_FANRONGDU) >=m_value)
		{
			return BUILD_LVUP_RET_SUCCESS;
		}
		return BUILD_LVUP_RET_NO_ENOUGH_GUILD_FANRONGDU;
	}

	void LoadXml(const char* strValue)
	{
		m_value = StringUtil::StringAsInt(strValue);
	}

private:
	int m_value{0};
};
//��齨�����ͽ����ȼ�
class ChekcGuildBuildTypeAndLevel :public CheckBuildLvUpCondition{
public:
	int  Run(const wchar_t* guildName){
		const std::wstring guildDataName = guildName + GuildBuildingManage::m_pInstance->m_ServerId;
		IPubData *guildData = GuildBuildingManage::m_pInstance->m_pPubSpace->GetPubData(guildDataName.c_str());
		if (guildData == NULL){
			return BUILD_LVUP_RET_ERROR;
		}
		IRecord * pGuildBuildingRc = guildData->GetRecord(GUILD_BUILDING_REC);
		if (pGuildBuildingRc == NULL){
			return BUILD_LVUP_RET_ERROR;
		}
		int rows = pGuildBuildingRc->GetRows();
		if (m_buildTypeNum > rows){
			return BUILD_LVUP_RET_NO_ENOUGH_GUILD_BUILD_TYPE;
		}
		int reachNum = 0;
		for (int row = 0; row < rows;row++)
		{
			int buildLevel = pGuildBuildingRc->QueryInt(row, GUILD_BUILDING_REC_COL_LEVEL);
			if (buildLevel >= m_buildLevel){
				++reachNum;
			}
		}
		if (m_buildTypeNum > reachNum)
		{
			return BUILD_LVUP_RET_NO_ENOUGH_GUILD_BUILD_LEVEL;
		}
		return BUILD_LVUP_RET_SUCCESS;
	}
	
	void LoadXml(const char* strValue){
		CVarList result;
		StringUtil::SplitString(result,strValue,",");
		if (result.GetCount() < 2){ return; }
		m_buildTypeNum = result.IntVal(0);
		m_buildLevel = result.IntVal(1);
	};

private:
	int m_buildTypeNum{0};
	int m_buildLevel{0};
};
//���������ȼ�
class CheckGuildJuYiTingBuildLv :public CheckBuildLvUpCondition{
public:
	int  Run(const wchar_t* guildName){
		int buildLevel = GuildBuildingManage::m_pInstance->GetBuildingLevel(guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_JU_YI_TING);
		if (m_value > buildLevel)
		{
			return BUILD_LVUP_RET_NO_ENOUGH_GUILD_BUILD_JUYITING_LEVEL;
		}
		return BUILD_LVUP_RET_SUCCESS;
	}
	void LoadXml(const char* strValue){
		m_value = StringUtil::StringAsInt(strValue);
	}

private:

	int m_value{ 0 };

};


#define ADD_GUILD_BUILD_LVUP_CONDITION_FUNC(x)	\
	m_CheckBuildingLvUpConditoinList[#x] = []()->CheckBuildLvUpCondition*{return new x();}



class DoDecGuildCapital :public DoDecCostActionByBuildLvUp{


public:

	bool Run(const wchar_t* guildName){
		GuildNumManage::m_pInstance->DecGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL, m_value, GUILD_NUM_CHANGE::GUILD_NUM_CHANGE_BUILD_LVUP);
		return true;
	}



	void LoadXml(const char* strValue){
		m_value = StringUtil::StringAsInt(strValue);
	}

private :
	int m_value{0};

};

//�۳����ٶ�
class DoDecGuildFanYongDu :public DoDecCostActionByBuildLvUp
{

public:

	bool Run(const wchar_t* guildName){
		GuildNumManage::m_pInstance->DecGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_FANRONGDU, m_value, GUILD_NUM_CHANGE::GUILD_NUM_CHANGE_BUILD_LVUP);
		
		return true;
	}

	void LoadXml(const char* strValue){
	 m_value = StringUtil::StringAsInt(strValue);
	}


private:
	int m_value{0};
};


#define ADD_GUILD_BUILD_LVUP_DO_DEC_ACTION(x) \
	m_DoDecCostActionByBuildingLvUpList[#x] = []()->DoDecCostActionByBuildLvUp*{return new x();};






#endif