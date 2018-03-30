//--------------------------------------------------------------------
// �ļ���:      SkillDataQueryModule.h
// ��  ��:      �������Բ��ҽӿ� 
// ˵  ��:      
// ��������:    2014��10��23��
// ������:      liumf
//    :       
//--------------------------------------------------------------------
#ifndef __SKILL_DATA_QUERY_MANAGER_H__
#define __SKILL_DATA_QUERY_MANAGER_H__

#include "FsGame/Define/header.h"
#include "utils/StringPod.h"
#include "utils/string_util.h"
#include "utils/XmlFile.h"
#include "FsGame/SkillModule/Impl/SkillDefineDataStruct.h"
#include "utils/IniFile.h"

class LevelModule;

// ���ܳɳ�����
struct SkillGrowProperty
{
	int	nArrGrowProperty[MAX_SKILL_GP_NUM]; // ���гɳ�����
	SkillGrowProperty()
	{
		memset(nArrGrowProperty, 0, sizeof(int) * MAX_SKILL_GP_NUM);
	}
};

// ����ֵ
union ValType
{
	float	fPropAdd;
	int		nPropAdd;
};
// ������������
struct PassiveSkillPropInfo
{
	int						nOptType;			// ������������(PassiveSkill)
	// Ӱ����������ID(���ʱΪ"")(nOptTypeΪPS_ADD_NEW_SKILLʱ,strSkillIDΪ�¼���id;nOptTypeΪPS_ADD_NEW_BUFFʱ,strSkillIDΪ��buffid)
	std::string				strSkillID;			
	ValType					uPropAdd;			// ���Լ�ֵ
	std::string				strPropName;		// ������ (nOptTypeΪPS_ADD_SKILL_EFFECTʱ,strPropNameΪ����Ч����)
	PassiveSkillPropInfo() : strSkillID("")
		,nOptType(0)
		,strPropName("")
	{

	}
};

typedef std::vector<PassiveSkillPropInfo> PSPropInfoVec;

// ���������������������ġ���������Ч��
struct PassiveSkillLevelInfo
{
	// �����ı�Ҫ����
	int						nMeridianLevel;		// ��Ҫ����ҵȼ�
	int						nPreSkillId;		// ǰ�ü���id
	int						nPreSkillLevel;		// ǰ�ü��ܵȼ�

	// ����������
	int						nSkillPoint;		// ���ĵļ��ܵ�
	int						nMoneyType;			// ���ĵĻ�������
	int						nMoneyCost;			// ���ѵĻ�������
	std::string				strItemId;			// ���ĵ���Ʒid
	int						nItemCost;			// ���ĵ���Ʒ����

	PSPropInfoVec			vPropInfoList;		// �ı���������

	PassiveSkillLevelInfo() : nMeridianLevel(0),
							nPreSkillId(0),
							nPreSkillLevel(0),
							nSkillPoint(0),
							nMoneyType(0),
							nMoneyCost(0),
							nItemCost(0)
	{
		vPropInfoList.clear();
	}

	~PassiveSkillLevelInfo()
	{
		vPropInfoList.clear();
	}
};

typedef std::vector<PassiveSkillLevelInfo> PSLevelInfoVec;

// ��������
struct PassiveSkill 
{
	PassiveSkill() : nSkillId(0), nCareer(0), nType(0)
	{
		vUpgradeInfoList.clear();
	}
	int						nSkillId;			// ��������id
	int						nCareer;			// ����ְҵ
	int						nType;				// ������������ ��(PassiveSkillType)
	int						nAddEffectType;	// ���ӵ���Ч��������
	PSLevelInfoVec			vUpgradeInfoList;	// ������������(�����±�Ϊ���ܵȼ�)
	~PassiveSkill()
	{
		vUpgradeInfoList.clear();
	}
};

// ��������
struct MultiSkillInfo 
{
	MultiSkillInfo() : nMultiSkillId(0)
	{
		vecSkill.clear();
	}
	int						nMultiSkillId;		// ��Ӧid
	std::vector<std::string>	vecSkill;		// �������ܱ�
	
	bool operator==(const MultiSkillInfo& rhs) const
	{
		return nMultiSkillId == rhs.nMultiSkillId;
	}
};

// ������������
struct HitTimeData 
{
	int				nHitTime;		// ����ʱ��
	int				nHitCount;		// ���д���
};

class SkillDataQueryModule : public ILogicModule
{
public:
	typedef std::map<std::string, std::string>          KeyToValueMap;
	typedef std::map<std::string, KeyToValueMap>        SkillSectionMap;

public:
    typedef std::vector<std::string> PathConfigVector;
    typedef std::map<std::string, PathConfigVector> DataPathConfigArray;

    typedef std::vector<std::string> EventParamVector;
    typedef std::map<int, EventParamVector> EventParamsMap;

	typedef std::vector<SkillGrowProperty> SkillGrowPropertyVec;
	typedef std::map<std::string, SkillGrowPropertyVec> GrowPropertyRecMap;
	typedef std::map<int, PassiveSkill*> PassiveSkillInfoMap;
	typedef std::vector<MultiSkillInfo> MultiSkillInfoVec;
	typedef std::vector<HitTimeData>	HitTimeDataVec;
	typedef std::map<std::string, HitTimeDataVec>	HitTimeConfigMap;
public:
    SkillDataQueryModule();
    virtual ~SkillDataQueryModule(){}

    virtual bool Init(IKernel* pKernel);
    virtual bool Shut(IKernel* pKernel);

public:
	bool LoadResource(IKernel* pKernel);
    // ��ȡĳ���͵������ļ�
    bool GetSkillConfigPath(const char* type, IVarList& result) const;
    bool GetBuffConfigPath(const char* type, IVarList& result) const;

    // ���ڼ������ݵĽӿ�
	const SkillBaseData* GetSkillBase(const char * szSkillID) const;
	const SkillMainData* GetSkillMain(IKernel* pKernel, const PERSISTID& skill) const;
	const SkillMainData* GetSkillMain(IGameObj* pSkillObj) const;

	// nGPType��Ӧ(SkillGrowProperty)
	const int QuerySkillGrowProperty(IKernel* pKernel, const PERSISTID& skill, int nGPType);

	// ��ȡ��Ҽ���ԭʼ����
	SkillMainData* GetSkillMainConfig(const char* szSkillID) const;
    const SkillHitRangeData* GetHitRangeProp(const char* szSkillID) const;
    const SkillHitRangeData* GetHitRangePropByID(const char* hitrangeID) const;
    const SkillEffectData* GetSkillEffectProp(const char* effectID) const;

	// ��Ӧ��������
	bool OnSkillPropChange(IKernel* pKernel, const PERSISTID& skill, const PERSISTID& self);
	
    // ����Buff���ݵĽӿ�
	const BuffBaseData* GetBuffBaseData(const char* szBuffID) const;
	const BuffEffectData* GetBuffEffectProp(const char* effectID) const;

	// ��ȡ��������ͻ��˼���ʱ��� nType��ӦSkillTimeDiff
	// �����������еļ���ʱ��Ҫ�ȿͻ��˵ļ���ʱ���Զ�һЩ,�����������ͷż���ʱ,ż������CDʧ�ܵ����
	//int	GetSkillTimeDiff(IKernel* pKernel, const PERSISTID& self, int nType);

	// ��ȡ������������
	const PassiveSkill* GetPassiveSkillConfig(const int nSkillId) const;
	const PassiveSkillLevelInfo* GetPassiveSkillLevelInfo(const int nSkillId, const int nSkillLevel) const;

	// ��ѯPassiveSkill������Ч������ nPSType��Ӧ(PassiveSkillType)
	bool QueryPassiveSkillEffect(IKernel* pKernel,  IVarList& outData, const PERSISTID& self, const PERSISTID& skill, const int nPSType, const char* strPropName);

	// ����ְҵ�ҵ����б�������id(���ԡ�������)
	bool QueryPassiveSkillIdByJob(IKernel* pKernel,  IVarList& outData, const PERSISTID& self);

	// ��ȡһ��������������
	const MultiSkillInfo* QueryMultiSkillInfo(int nMultiSkillId);

	// ��ȡ��������
	const HitTimeDataVec* QueryHitTimeList(const char* strSkillId);

	// ��ȡ����������д���
	int GetMaxHitNum(IKernel* pKernel, IGameObj* pSkillObj);
private:
    void ReleaseResource();

    bool LoadSkillProp();
    bool LoadBufferProp();
    bool LoadNpcNormalSkillProp();

    // ����SKILL����
    bool LoadSkillPathConfig();
    bool LoadSkillEventFunIdParam();

	// ���ؼ��ܹ�ʽ
	bool LoadSkillFormula();

	// ���ر�����������
 	bool LoadPassiveSkillData();
 
 	// ��ȡ����������������
 	bool LoadPassiveSkillProperty(PassiveSkillLevelInfo& outSkillPro, const std::string& strInfo);

	// ����BUFFER����
    bool LoadBuffPathConfig();

	// ���ؼ��ܶ����˺���
	bool LoadSkillGrowPropertyRec();

	// ����ĳ�����ܵĶ����˺�
	bool LoadOneSkillGrowProperty(SkillGrowPropertyVec& vecExtraHurt, const std::string& strSkillId);

	// ���ؼ��ܷ�������ͻ��˵ļ���ʱ���
	//bool LoadSkillTimeDiffConfig();

	// ������������
	bool LoadMultiSkillConfig();

	// ���ݼ��ܳɳ������ҵ�����ֵ
	int GetPropertyBySkillGPType(const SkillMainData* pMainData, int nGPType);

	// ��ѯ�ɳ����Ե�ֵ
	int GetGrowProperty(IKernel* pKernel, const PERSISTID& skill, int nGPType);

	// ��ѯPassiveSkill������
	bool QueryPassiveSkillProp(IKernel* pKernel, IVarList& outData, const int nPassiveSkillId,
		const int nPassiveSkillLV, const int nPSType, const char* strPropName);

    template<class T>
    bool LoadProp(const PathConfigVector& vPath, TStringPod<char, T*>& map);
    template<class T>
    bool LoadProp(const PathConfigVector& vPath, const EventParamsMap& eventParamsMap, TStringPod<char, T*>& map);
    template<class T>
    void ReleaseProp(TStringPod<char, T*>& map);
    template<class T>
    const T*  GetPropPoint(const char* strKeyID, const TStringPod<char, T*>& map) const;
	// ����������������
	void ParseHitTimeData();
private:
	// ��Լ������õ����ݳ�Ա
    TStringPod<char, SkillBaseData*>         m_SkillBase;
    TStringPod<char, SkillMainData*>         m_SkillMainData;
    TStringPod<char, SkillHitRangeData*>     m_SkillHitRangeData;
    TStringPod<char, SkillEffectData*>       m_SkillEffectData;
	TStringPod<char, BuffEffectData*>		 m_BuffEffectData;
	std::vector<int>						 m_vSkillTimeDiff;
	
	// ���Buffer���õ����ݳ�Ա
	TStringPod<char, BuffBaseData*>         m_BuffBase;

    SkillSectionMap                         m_SkillUseLimitData;
	GrowPropertyRecMap						m_mapSkillGrowProperty;		// ���ܳɳ����Բ�ѯ��
	PassiveSkillInfoMap						m_mapPassiveSkill;			// �����������ݱ�
	MultiSkillInfoVec						m_vecMultiSkillInfo;		// ������������
	HitTimeConfigMap						m_mapHitTimeData;			// ��������
private:
    // ����·����
    DataPathConfigArray m_vSkillDataPathConfigArray;
    DataPathConfigArray m_vBuffDataPathConfigArray;

	// �����¼����ݱ���
	EventParamsMap m_eventParamsMap;

    // �����ļ���Ŀ¼
    std::string m_strWorkPath;

	StringBuffer m_StringBuffer;

	static LevelModule*			m_pLevelModule;
public:
	// ��ģ��ָ��
	static SkillDataQueryModule* m_pInstance;
};

#endif // __SKILL_DATA_QUERY_MANAGER_H__
