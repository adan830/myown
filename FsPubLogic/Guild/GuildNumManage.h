#ifndef __GUILD_NUM_MANAGE_H__
#define __GUILD_NUM_MANAGE_H__
#include "GuildBase.h"
#include "FsGame\Define\GuildDefine.h"

class GuildNumManage:public GuildBase
{

public:
	GuildNumManage(){
		m_pInstance = this;
	}
	bool Init(IPubKernel* pPubKernel);
	bool CreateGuildInit(IPubData* guildData);
	virtual int OnPublicMessage(IPubKernel* pPubKernel, int source_id,
		int scene_id, const IVarList& msg);
	bool LoadResConfig(IPubKernel*pPubKernel);
	
	//������֯��ֵ
	bool AddGuildNumValue(const wchar_t* guildName, GUILD_NUM_TYPE guildNumType, int addValue, GUILD_NUM_CHANGE desc, const wchar_t* playerName = NULL);
	//��õ�ǰ��֯��ֵ
	int GetGuildNumValue(const wchar_t* guildName, GUILD_NUM_TYPE guildNumType);
	//�ܹ������֯��ֵ
	int CanAddGuildNumValue(const wchar_t* guildName, GUILD_NUM_TYPE guildNumType);
	//�۳���֯��ֵ
	bool DecGuildNumValue(const wchar_t* guildName, GUILD_NUM_TYPE guildNumType, int decValue, GUILD_NUM_CHANGE desc, const wchar_t* playerName = NULL);
	//����������ֵ
	bool AddGuildNumLockValue(const wchar_t* guildName, GUILD_NUM_TYPE guildNumType, int lockValue);
	//���������ֵ
	int GetGuildNumLockValue( const wchar_t* guildName, GUILD_NUM_TYPE guildNumType);
	//�Ƿ���Կ۳�
	 bool CanDecGuildNumValue( const wchar_t* guildName, GUILD_NUM_TYPE guildNumType,int value);
	//�۳�������ֵ
	bool DecGuildNumLockValue(const wchar_t* guildName, GUILD_NUM_TYPE guildNumType, int decValue);
	//����������ֵ
	bool GuildUnLockValue(const wchar_t* guildName, GUILD_NUM_TYPE guildNumType, int unLockValue);
	//����������������ֵ
	int GetGuildNumUpperLimit(GUILD_NUM_TYPE type,const wchar_t* guildName);
	//��֯�������޼ӳ�
	int GetGuildBuildingAditionUpperLimit(GUILD_NUM_TYPE type,const wchar_t*guildName);
	//ÿ������
	int GuildNumDailyReset(IPubKernel* pPubKernel);
	//�۳�������
	void DecJianKangDu(const wchar_t*guildName, int ret);

public:
	static GuildNumManage* m_pInstance;

private:



	enum
	{
		GUILD_NUM_BUY_SUCCES,// ����ɹ�
		GUILD_NUM_BUY_GUILD_CAPITAL_NO_ENOUGH,//Ǯ����
		GUILD_NUM_BUY_TIMES_NO_ENOUGH , //��������
		GUILD_NUM_BUY_REACH_MAX, //������ֵ���ֵ
	};
	//���������
	void OnGetJianKangDuGif(IPubKernel* pPubKernel, int sourceId, int sceneId, const IVarList& args);
		

		//������֯��ֵ
	void OnBuyGuildNum(IPubKernel* pPubKernel, int sourceId, int sceneId, const IVarList& args);

	//��֯��ֵ��ʼ��
	bool LoadGuildNumInitConfig(IPubKernel*pPubKernel);
	//��֯����������֯��ֵ����
	bool LoadGuildBuildingAdditoinUpperLimit(IPubKernel*pPubKernel);
	//��֯��ֵ��������
	bool LoadGuildNumBuyCostConfig(IPubKernel*pPubKernel);
	//���ؽ���������
	bool LoadGuildJianKangDuConfig(IPubKernel*pPubKernel);
	//���Ӹ��˹�����֯�ʽ�
	void AddPersoinalGainCapital(IPubData*guildData, const wchar_t*playerName, int value);
	
	void SendGuildNumBuyResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
		const wchar_t* playerName,int guildNum, int result,int value = 0);

	struct GuildNumInit{
		int m_initNum{0};
		int m_upperLimit{0};
	};
	std::map<int,GuildNumInit> m_initGuildNumCofig;

	struct GuildBuildingAddition{
		int m_GuidBuingType{ -1 };
		std::map<int, int> m_addition;
	};

	//��֯��ֵ���ͣ���֯�������ͣ������ȼ�
	std::map<int, GuildBuildingAddition> m_guildBuildingAddition;
	struct GuildNumBuyCost{
		int m_buyMaxTimes{0};
		int m_buyValue{0};
		int m_recoverValue{0};
		std::vector<int> m_buyCost;

	};

	//<��֯��ֵ����
	std::map<int, GuildNumBuyCost> m_buyGuildNumConfig;

	//��֯������
	struct GuildJianKangDuGif{
		int m_lower_limit;
		int m_upper_limit;
		std::string m_award;
	};
	struct GuildJianKangDu{
		int m_dailyRecoverValue{ 0 }; //ÿ�ջָ�
		int m_needGuildCapital{ 0 };      //��Ҫ�ʽ�
		int m_maintainFailed{ 0 };      //ά��ʧ�ܿ۳�������
		int m_guildActivityFialed{ 0 }; //��֯�δ�����۳�������
		int m_guildRecoverFailed{ 0 };   //�ָ�ʧ�ܿ۳�������
		int m_extraAwardNeedGainCapital{0}; //ÿ�ջ�ȡ���������Ҫ����֯����
		std::string m_extraAward;		//�������

		std::vector<GuildJianKangDuGif> m_gif;
	};

	GuildJianKangDu m_guildJianKangDuConfig;


};






#endif