//--------------------------------------------------------------------
// �ļ���:      SkillDataQueryModule.h
// ��  ��:      �������Բ��ҽӿ� 
// ˵  ��:      
// ��������:    2014��10��23��
// ������:      liumf
//    :       
//--------------------------------------------------------------------

#include <string>
#include "SkillDataQueryModule.h"
#include "utils/extend_func.h"
#include "utils/exptree.h"
#include "../../Define/FightPropertyDefine.h"
#include "../../Define/GameDefine.h"
#include "../../Define/SnsDefine.h"
#include "../../CommonModule/LevelModule.h"
//#include "../PassiveSkillSystem.h"
#include <algorithm>
#include "../PassiveSkillSystem.h"

#define SKILL_PATH_CONFIG				"skill/SkillPathConfig.xml"
#define BUFF_PATH_CONFIG				"skill/BuffPathConfig.xml"
#define SKILL_EVENT_FUNID_PARAMS_FILE	"skill/SkillEventFunIdParams.xml"
#define SKILL_GROW_PROPERTY_PATH		"skill/char/SkillPropertyGrow/"
//#define SKILL_TIME_DIFF_PATH			"skill/char/char_skill_time_diff.xml"
#define MULTI_SKILL_PATH				"skill/char/multi_skill.xml"
#define MAX_SECTION_HIT_TIME 400		// ��Ҫ�ֶε�����ʱ��

SkillDataQueryModule*	SkillDataQueryModule::m_pInstance = NULL;
LevelModule*			SkillDataQueryModule::m_pLevelModule = NULL;

SkillDataQueryModule::SkillDataQueryModule()
:m_SkillBase(600)
,m_SkillMainData(700)
,m_SkillHitRangeData(600)
,m_SkillEffectData(600)
,m_BuffBase(500)
,m_BuffEffectData(200)
{

}

bool SkillDataQueryModule::Init(IKernel* pKernel)
{
	m_StringBuffer.Init(300*1024, 1024);
	m_pInstance = this;
	m_strWorkPath = std::string(pKernel->GetResourcePath()) + std::string("ini/");

	m_pLevelModule = (LevelModule*)pKernel->GetLogicModule("LevelModule");
	Assert(NULL != m_pLevelModule && NULL != m_pInstance);

	LoadResource(pKernel);
    return true;
}

bool SkillDataQueryModule::LoadResource(IKernel* pKernel)
{
	ReleaseResource();

	LoadSkillProp();
	LoadBufferProp();
	//LoadSkillFormula();
	LoadSkillGrowPropertyRec();
//	LoadSkillTimeDiffConfig();
//	LoadPassiveSkillData();
//	LoadMultiSkillConfig();
//	LoadNationBattleSkillTips();

	return true;
}

bool SkillDataQueryModule::Shut(IKernel* pKernel)
{
    ReleaseResource();
    return true;
}

bool SkillDataQueryModule::GetSkillConfigPath(const char* type, IVarList& result) const
{
    result.Clear();
    //��ҪԤ�ȼ��ص������ļ�
    DataPathConfigArray::const_iterator it = m_vSkillDataPathConfigArray.find(type);
    if (it != m_vSkillDataPathConfigArray.end())
    {
        const PathConfigVector& paths = (*it).second;
		LoopBeginCheck(a)
        for (PathConfigVector::const_iterator iter = paths.begin(); iter != paths.end(); ++iter)
        {
			LoopDoCheck(a)
            std::string strFile = (*iter);
            result << strFile.c_str();
        }
        return true;
    }
    return false;
}

bool SkillDataQueryModule::GetBuffConfigPath(const char* type, IVarList& result) const
{
    result.Clear();
    //��ҪԤ�ȼ��ص������ļ�
    DataPathConfigArray::const_iterator it = m_vBuffDataPathConfigArray.find(type);
    if (it != m_vBuffDataPathConfigArray.end())
    {
        const PathConfigVector& paths = (*it).second;

		LoopBeginCheck(b)
        for (PathConfigVector::const_iterator iter = paths.begin(); iter != paths.end(); ++iter)
        {
			LoopDoCheck(b)
            std::string strFile = (*iter);
            result << strFile.c_str();
        }
        return true;
    }
    return false;
}

const SkillBaseData* SkillDataQueryModule::GetSkillBase(const char * szSkillID) const
{
    return GetPropPoint(szSkillID, m_SkillBase);
}

const SkillMainData* SkillDataQueryModule::GetSkillMain(IKernel* pKernel, const PERSISTID& skill) const
{
	if (!pKernel->Exists(skill))
	{
		return NULL;
	}

	IGameObj* pSkillObj = pKernel->GetGameObj(skill);
	const SkillMainData* pMainData = GetSkillMain(pSkillObj);

	return pMainData;
}

const SkillMainData* SkillDataQueryModule::GetSkillMain(IGameObj* pSkillObj) const
{
	if (NULL == pSkillObj)
	{
		return NULL;
	}

	const char* szMainPackID = pSkillObj->QueryString(SKILL_CONFIGID);
	SkillMainData* pMainData = GetSkillMainConfig(szMainPackID);

	return pMainData;
}

// nGPType��Ӧ(SkillGrowProperty)
const int SkillDataQueryModule::QuerySkillGrowProperty(IKernel* pKernel, const PERSISTID& skill, int nGPType)
{
	const SkillMainData* pMainData = GetSkillMain(pKernel, skill);
	if (NULL == pMainData)
	{
		return NO_VALID_GROW_PROPERTY;
	}

	int nQueryVal = NO_VALID_GROW_PROPERTY;
	// ����гɳ�����
	if (pMainData->GetIsGrowProperty())
	{
		nQueryVal = GetGrowProperty(pKernel, skill, nGPType);
	}

	// û�гɳ����Ի�û�鵽���ȡĬ������
	if (NO_VALID_GROW_PROPERTY == nQueryVal)
	{
		nQueryVal = GetPropertyBySkillGPType(pMainData, nGPType);
	}

	return nQueryVal;
}

const SkillHitRangeData* SkillDataQueryModule::GetHitRangeProp(const char* szSkillID) const 
{
    const SkillMainData* pMainData = GetPropPoint(szSkillID, m_SkillMainData);
    if (pMainData)
    {
        const StringArray& hitRangeList = pMainData->GetHitRangeID();
        if (hitRangeList.GetCount() > 0)
        {
            return GetPropPoint(hitRangeList.StringVal(0), m_SkillHitRangeData);
        }
    }

    return NULL;
}

const SkillHitRangeData* SkillDataQueryModule::GetHitRangePropByID(const char* hitrangeID) const
{
    return GetPropPoint(hitrangeID, m_SkillHitRangeData);
}

const SkillEffectData* SkillDataQueryModule::GetSkillEffectProp(const char* effectID) const
{
    return GetPropPoint(effectID, m_SkillEffectData);
}

// ��Ӧ��������
bool SkillDataQueryModule::OnSkillPropChange(IKernel* pKernel, const PERSISTID& skill, const PERSISTID& self)
{
	IGameObj* pSkillObj = pKernel->GetGameObj(skill);
	if (NULL == pSkillObj)
	{
		return false;
	}

	const SkillMainData* pData = GetSkillMain(pSkillObj);
	if (NULL == pData)
	{
		return false;
	}
	int		nAttackAdd = 0;
	float	fSkillDamageRate = 0.0f;
	// �������õ�����ֵ
	if (pData->GetIsGrowProperty())
	{
		// ���ܸ��ӹ�����
		nAttackAdd = GetGrowProperty(pKernel, skill, SKILL_GP_ATTACK_ADD);
		// �˺��ٷֱ�
		int nSkillRate = GetGrowProperty(pKernel, skill, SKILL_GP_DAMAGE_RATE);
		fSkillDamageRate = (float)nSkillRate / 100.0f;
	}
	else
	{
		fSkillDamageRate = pData->GetDamageRate();
	}

	// ͳ�Ʊ�������Ӱ�������ֵ
	if (pKernel->Exists(self))
	{
		CVarList args;
		if (QueryPassiveSkillEffect(pKernel, args, self, skill, PS_ADD_SKILL_PROPERTY, "SkillAttackAdd"))
		{
			int nPSAddAttack = args.IntVal(0);
			nAttackAdd += nPSAddAttack;
		}
		args.Clear();
		if (QueryPassiveSkillEffect(pKernel, args, self, skill, PS_ADD_SKILL_PROPERTY, "SkillDamageRate"))
		{
			int nPSDamageRate = args.IntVal(0);
			fSkillDamageRate += (float)nPSDamageRate / 100.0f;
		}
	}

	// ˢ�¼�������
	pSkillObj->SetInt("SkillAttackAdd", nAttackAdd);
	pSkillObj->SetFloat("SkillDamageRate", (float)fSkillDamageRate);
	return true;
}

const BuffBaseData* SkillDataQueryModule::GetBuffBaseData(const char* szBuffID) const
{
    return GetPropPoint(szBuffID, m_BuffBase);
}

const BuffEffectData* SkillDataQueryModule::GetBuffEffectProp(const char* effectID) const 
{
	return GetPropPoint(effectID, m_BuffEffectData);
}

// ��ȡ��������ͻ��˼���ʱ��� nType��ӦSkillTimeDiff
// int	SkillDataQueryModule::GetSkillTimeDiff(IKernel* pKernel, const PERSISTID& self, int nType)
// {
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (NULL == pSelfObj || nType >= MAX_SKILL_TIME_DIFF_NUM || nType < 0)
// 	{
// 		return 0;
// 	}
// 	
// 	// npcû�в�ֵ
// 	if (pSelfObj->GetClassType() == TYPE_NPC)
// 	{ 
// 		return 0;
// 	}
// 	int nTimeDiff = 0;
// 	int nOnline = pSelfObj->QueryInt("Online");
// 	// ֻ���������ȡʱ���,����ȡ����ֵ
// 	if (ONLINE == nOnline)
// 	{
// 		nTimeDiff = m_vSkillTimeDiff[nType];
// 	}
// 
// 	return nTimeDiff;
// }

// ��ȡ������������
const PassiveSkill* SkillDataQueryModule::GetPassiveSkillConfig(const int nSkillId) const
{
	PassiveSkill* pSkillData = NULL;
	do 
	{
		PassiveSkillInfoMap::const_iterator iter = m_mapPassiveSkill.find(nSkillId);
		if (iter == m_mapPassiveSkill.end())
		{
			break;
		}
		pSkillData = iter->second;
	} while (0);
	
	return pSkillData;
}

const PassiveSkillLevelInfo* SkillDataQueryModule::GetPassiveSkillLevelInfo(const int nSkillId, const int nSkillLevel) const
{
	const PassiveSkillLevelInfo* pSkillInfo = NULL;
	do 
	{
		const PassiveSkill* pSkillData = GetPassiveSkillConfig(nSkillId);
		if(NULL == pSkillData)
		{
			break;
		}

		if (nSkillLevel <= 0 || nSkillLevel > (int)pSkillData->vUpgradeInfoList.size())
		{
			break;
		}

		pSkillInfo = &pSkillData->vUpgradeInfoList[nSkillLevel - 1];
	} while (0);
	
	return pSkillInfo;
}

// ��ѯPassiveSkill������Ч������
bool SkillDataQueryModule::QueryPassiveSkillEffect(IKernel* pKernel,  IVarList& outData, const PERSISTID& self,
												   const PERSISTID& skill, const int nPSType, const char* strPropName)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	const SkillMainData* pSkillMainData = GetSkillMain(pKernel, skill);
	if (NULL == pSkillMainData || !pKernel->Exists(self))
	{
		return false;
	}

	// û�ҵ��������ܱ�
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return false;
	}

	outData.Clear();
	CVarList passiveskills;
	pSkillMainData->GetPassiveSkill().ConvertToVarList(passiveskills);
	int nCount = (int)passiveskills.GetCount();
	if (0 == nCount)
	{
		return false;
	}
	bool bFindProp = false;
	LoopBeginCheck(c)
	for (int i = 0;i < nCount;++i)
	{
		LoopDoCheck(c)
		int nPassiveSkillId = passiveskills.IntVal(i);
		int nPassiveSkillLevel = PassiveSkillSystemSingleton::Instance()->QueryPassiveSkillLevel(pKernel, self, nPassiveSkillId);
		// ˵���˼���ûѧ
		if (0 == nPassiveSkillLevel)
		{
			continue;
		}
		// ���˸��Ӽ���Ч������,�����ڶ����������Ӱ��һ���������ܵ�ͬһ������
		if(QueryPassiveSkillProp(pKernel, outData, nPassiveSkillId, nPassiveSkillLevel, nPSType, strPropName))
		{
			if (PS_ADD_SKILL_EFFECT != nPSType)
			{
				break;
			}
		}
	}
	if (outData.GetCount() > 0)
	{
		bFindProp = true;
	}
	
	return false;
}

// ����ְҵ�ҵ����б�������id(���ԡ�������)
bool SkillDataQueryModule::QueryPassiveSkillIdByJob(IKernel* pKernel,  IVarList& outData, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
	outData.Clear();
	int nJob = pSelfObj->QueryInt("Job");
	PassiveSkillInfoMap::iterator iter;
	LoopBeginCheck(a);
	for (iter = m_mapPassiveSkill.begin(); iter != m_mapPassiveSkill.end(); ++iter)
	{
		LoopDoCheck(a);
		PassiveSkill* pData = iter->second;
		if (NULL == pData)
		{
			continue;
		}
		if (pData->nCareer == nJob)
		{
			outData << pData->nSkillId;
		}
	}

	return true;
}

// ��ȡһ��������������
const MultiSkillInfo* SkillDataQueryModule::QueryMultiSkillInfo(int nMultiSkillId)
{
	MultiSkillInfo info;
	info.nMultiSkillId = nMultiSkillId;
	MultiSkillInfoVec::iterator iter = find(m_vecMultiSkillInfo.begin(), m_vecMultiSkillInfo.end(), info);
	if (iter != m_vecMultiSkillInfo.end())
	{
		return &(*iter);
	}

	return NULL;
}

// ��ȡ��������
const SkillDataQueryModule::HitTimeDataVec* SkillDataQueryModule::QueryHitTimeList(const char* strSkillId)
{
	HitTimeConfigMap::iterator iter = m_mapHitTimeData.find(strSkillId);
	if (iter == m_mapHitTimeData.end())
	{
		return NULL;
	}

	return &iter->second;
}

// ��ȡ����������д���
int SkillDataQueryModule::GetMaxHitNum(IKernel* pKernel, IGameObj* pSkillObj)
{
	if (NULL == pSkillObj)
	{
		return 0;
	}

	const char* strSkillId = pSkillObj->GetConfig();
	HitTimeConfigMap::iterator iter = m_mapHitTimeData.find(strSkillId);
	if (iter == m_mapHitTimeData.end())
	{
		return 1; // û�ж����������,������д���Ϊ1
	}

	return (int)iter->second.size();
}

void SkillDataQueryModule::ReleaseResource()
{
    ReleaseProp(m_SkillBase);
    ReleaseProp(m_SkillMainData);
    ReleaseProp(m_SkillHitRangeData);
    ReleaseProp(m_SkillEffectData);
    ReleaseProp(m_BuffBase);
	ReleaseProp(m_BuffEffectData);

    m_SkillUseLimitData.clear();

	LoopBeginCheck(e)
	// ���ܶ����˺������
	for (GrowPropertyRecMap::iterator it = m_mapSkillGrowProperty.begin();it != m_mapSkillGrowProperty.end();++it)
	{
		LoopDoCheck(e)
		SkillGrowPropertyVec& rec = it->second;
		rec.clear();
	}
	m_mapSkillGrowProperty.clear();
	
	// ����ʱ���
	//m_vSkillTimeDiff.clear();

	// �����������
	LoopBeginCheck(f)
	for (PassiveSkillInfoMap::iterator it = m_mapPassiveSkill.begin();it != m_mapPassiveSkill.end();++it)
	{
		LoopDoCheck(f)
		SAFE_DELETE(it->second);
	}
	m_mapPassiveSkill.clear();

	// �����������
	LoopBeginCheck(s)
	int nSize = (int)m_vecMultiSkillInfo.size();
	for (int i = 0;i < nSize;++i)
	{
		LoopDoCheck(s)
		m_vecMultiSkillInfo[i].vecSkill.clear();
	}
	m_vecMultiSkillInfo.clear();
}

// ��ȡ����ԭʼ����
SkillMainData* SkillDataQueryModule::GetSkillMainConfig(const char* szSkillID) const
{
	const char* szMainPackID = szSkillID;
	return const_cast<SkillMainData*>(GetPropPoint(szMainPackID, m_SkillMainData));
}


bool SkillDataQueryModule::LoadSkillProp()
{
    if (!LoadSkillPathConfig())
    {
        return false;
    }

    if(!LoadSkillEventFunIdParam())
    {
        return false;
    }

	LoopBeginCheck(g)
    for (DataPathConfigArray::const_iterator iter = m_vSkillDataPathConfigArray.begin();iter != m_vSkillDataPathConfigArray.end(); ++iter)
    {
		LoopDoCheck(g)
        const std::string &strFlag = (*iter).first;
        const PathConfigVector &vPath = (*iter).second;

        if (strFlag.compare("SkillHitRange") == 0)
        {
            LoadProp(vPath, m_SkillHitRangeData);
        }
        else if (strFlag.compare("SkillNew") == 0)
        {
            LoadProp(vPath, m_SkillBase);
        }
        else if (strFlag.compare("SkillMain") == 0)
        {
            LoadProp(vPath, m_SkillMainData);
			ParseHitTimeData();
        }
        else if (strFlag.compare("SkillEffect") == 0)
        {
            LoadProp(vPath, m_eventParamsMap, m_SkillEffectData);
        }
    }

    return true;
}

bool SkillDataQueryModule::LoadSkillPathConfig()
{
    m_vSkillDataPathConfigArray.clear();
    std::string strPath = m_strWorkPath + SKILL_PATH_CONFIG;
    CXmlFile xml(strPath.c_str());
    std::string errMsg;
    if (!xml.LoadFromFile(errMsg))
    {
        // ·�������ļ��޷���
        extend_warning(LOG_ERROR, errMsg.c_str());
        return false;
    }

    CVarList vSectionList;
    xml.GetSectionList(vSectionList);
	LoopBeginCheck(h)
    for (size_t index = 0; index < vSectionList.GetCount(); ++index)
    {
		LoopDoCheck(h)
        const char* section = vSectionList.StringVal(index); 
        if (NULL == section)
        {
            continue;
        }

        const char* type = xml.ReadString(section, "Type", "");
        const char* path = xml.ReadString(section, "Path", "");
        if (NULL == type || '\0' == *type || NULL == path || '\0' == *path)
        {
            continue;
        }
		DataPathConfigArray::iterator iter = m_vSkillDataPathConfigArray.find(type);
		if (iter == m_vSkillDataPathConfigArray.end())
		{
			PathConfigVector pathVec;
			pathVec.push_back(path);
			m_vSkillDataPathConfigArray.insert(make_pair(type, pathVec));
		}
		else
		{
			iter->second.push_back(path);
		}
    }

    return true;
}

bool SkillDataQueryModule::LoadSkillEventFunIdParam()
{
    m_eventParamsMap.clear();

    //���·��
    std::string path = m_strWorkPath + SKILL_EVENT_FUNID_PARAMS_FILE;
    CXmlFile xml(path.c_str());
    if (!xml.LoadFromFile())
    {
        extend_warning(LOG_ERROR,"[SkillEventManager::LoadResource] no found file:%s.",path.c_str());
        return false;
    }
    //��ȡ��̬���ݱ��ļ�
    CVarList section_list;
    xml.GetSectionList(section_list);
    //�Ȱ�ini �Ķ�������Ϊ���������
    size_t section_count = section_list.GetCount();
    //ѭ������ÿ����
	LoopBeginCheck(i)
    for (size_t i = 0; i < section_count; ++i)
    {
		LoopDoCheck(i)
        //ID
        const char* str_func_id = section_list.StringVal(i);
        int func_id = StringUtil::StringAsInt(str_func_id, 0);

        //����
        CVarList item_list;
        xml.GetItemValueList(str_func_id, "Params", item_list);
        if (item_list.GetCount() != 1)
        {
            extend_warning(LOG_ERROR, "[SkillDataQueryModule::LoadEventParamsResource] Params count != 1 : Skill_EventFunIdParams.xml");
            return false;
        }
        std::vector<std::string> eventparams_list;
        eventparams_list.clear();
        const char* itemvalue = item_list.StringVal(0);
        if (itemvalue == NULL)
        {
            return false;
        }
        CVarList params;
        StringUtil::SplitString(params, itemvalue, ",");
		LoopBeginCheck(j)
        for (size_t iparam = 0; iparam < params.GetCount(); iparam++)
        {
			LoopDoCheck(j)
            eventparams_list.push_back(params.StringVal(iparam));
        }
        m_eventParamsMap.insert(std::make_pair(func_id, eventparams_list));
    }
    return true;
}

// ���ر�����������
bool SkillDataQueryModule::LoadPassiveSkillData()
{
	// ��ȡ����·��
	DataPathConfigArray::iterator iter = m_vSkillDataPathConfigArray.find("PassiveSkill");
	if (iter == m_vSkillDataPathConfigArray.end())
	{
		return false;
	}
	PathConfigVector& pathVec = iter->second;
	// �������������������ļ�·��
	if (pathVec.size() != 2)
	{
		return false;
	}
	std::string strPath = m_strWorkPath + pathVec[0];

	// ��ȡ��������ע������
	CXmlFile xml(strPath.c_str());
	if (!xml.LoadFromFile())
	{
		extend_warning(LOG_ERROR,"[SkillDataQueryModule::LoadPassiveSkillData] no found file:%s.",strPath.c_str());
		return false;
	}

	CVarList vSectionList;
	xml.GetSectionList(vSectionList);
	LoopBeginCheck(l)
	for (size_t index = 0; index < vSectionList.GetCount(); ++index)
	{
		LoopDoCheck(l)
		const char* section = vSectionList.StringVal(index);
		if (NULL == section)
		{
			continue;
		}
		// ��ȡ����id
		PassiveSkill* pData = NEW PassiveSkill;
 		pData->nSkillId		= StringUtil::StringAsInt(section);
		pData->nCareer		= xml.ReadInteger(section, "CareerID", 1);
		pData->nType		= xml.ReadInteger(section, "Type", 0);
		pData->nAddEffectType = xml.ReadInteger(section, "AddEffectType", 0);

		m_mapPassiveSkill.insert(std::make_pair(pData->nSkillId, pData));
	}

	// ��ȡ����·��
	strPath = m_strWorkPath + pathVec[1];

	// ��ȡ�������ܵ���ϸ����
	xml.SetFileName(strPath.c_str());
	if (!xml.LoadFromFile())
	{
		extend_warning(LOG_ERROR,"[SkillDataQueryModule::LoadPassiveSkillData] no found file:%s.",strPath.c_str());
		return false;
	}

	vSectionList.Clear();
	xml.GetSectionList(vSectionList);
	LoopBeginCheck(a);
	for (size_t index = 0; index < vSectionList.GetCount(); ++index)
	{
		LoopDoCheck(a);
		const char* section = vSectionList.StringVal(index);
		if (NULL == section)
		{
			continue;
		}
		// key���ɼ���id+���ܵȼ�ƴ�Ӷ���.ǰ5λΪ����id����λΪ���ܵȼ�.����:10001001
		int nSection = StringUtil::StringAsInt(section);
		int nSkillId	= nSection / 1000;
		int nSkillLevel = nSection % 1000; // ���ܵȼ���1����
		if(nSkillLevel <= 0)
		{	
			// ���ܵȼ�Ϊ0 �ͻ��˻��õ�
			if (nSkillLevel != 0)
			{
				std::string strInfo = strPath + std::string(section) + "level too large";
				extend_warning(LOG_WARNING, strInfo.c_str());
			}
			continue;
		}

		// �ҵ���Ӧ�����ݴ洢λ��
		PassiveSkillInfoMap::iterator it = m_mapPassiveSkill.find(nSkillId);
		if (it == m_mapPassiveSkill.end())
		{
			std::string strInfo = strPath + std::string(section) + "skillid is wrong";
			extend_warning(LOG_WARNING, strInfo.c_str());
			continue;
		}
		PassiveSkill* pData = it->second;
		if (NULL == pData)
		{
			continue;
		}

		// ��ʼ��ȡÿ���ȼ�������
		PassiveSkillLevelInfo kSkillUpgrade;

		// ��ȡ��Ҫ������ȼ�����
		kSkillUpgrade.nMeridianLevel	= xml.ReadInteger(section, "MeridianLevel", 0);
		// ��ȡ��Ҫ��ǰ�ü��ܵȼ�����
		std::string strTemp			= xml.ReadString(section, "SkillNeedTerm", "");
		CVarList args;
		// ��������Ҫ����ǰ������
		if (strTemp.empty())
		{
			kSkillUpgrade.nPreSkillId		= 0;
			kSkillUpgrade.nPreSkillLevel	= 0;
		}
		else
		{
			StringUtil::SplitString(args, strTemp, ",");
			if (args.GetCount() != 2)
			{
				std::string strInfo = strPath + std::string(section) + "SkillNeedTerm is wrong";
				extend_warning(LOG_WARNING, strInfo.c_str());
				continue;
			}
			kSkillUpgrade.nPreSkillId		= args.IntVal(0);
			kSkillUpgrade.nPreSkillLevel	= args.IntVal(1);
		}
		// ���ĵļ��ܵ�
		kSkillUpgrade.nSkillPoint	= xml.ReadInteger(section, "CostSkillPoint", 1);
		// ���ĵĻ�������
		kSkillUpgrade.nMoneyType	= xml.ReadInteger(section, "MoneyType", 0);
		// ���ĵĻ�����
		kSkillUpgrade.nMoneyCost	= xml.ReadInteger(section, "CostMoney", 1000);

		// ��ȡ���ĵĲ���
		strTemp			= xml.ReadString(section, "CostItem", "");
		args.Clear();
		StringUtil::SplitString(args, strTemp, ",");
		// ��������Ҫ����
		if (strTemp.empty())
		{
			kSkillUpgrade.strItemId		= "";
			kSkillUpgrade.nItemCost		= 0;
		}
		else
		{
			if (args.GetCount() != 2)
			{
				std::string strInfo = strPath + std::string(section) + "CostItem is wrong";
				extend_warning(LOG_WARNING, strInfo.c_str());
				continue;
			}
			kSkillUpgrade.strItemId		= args.StringVal(0);
			kSkillUpgrade.nItemCost		= args.IntVal(1);
		}
		
		// ��ȡ������������
		strTemp			= xml.ReadString(section, "PropList", "");
		LoadPassiveSkillProperty(kSkillUpgrade, strTemp);

		pData->vUpgradeInfoList.push_back(kSkillUpgrade);
	}	
	return true;
}

// ��ȡ����������������
bool SkillDataQueryModule::LoadPassiveSkillProperty(PassiveSkillLevelInfo& outSkillPro, const std::string& strInfo)
{
	CVarList args;
	StringUtil::SplitString(args, strInfo, ";");

	int nCount = (int)args.GetCount();
	if (0 == nCount)
	{
		return false;
	}
	outSkillPro.vPropInfoList.resize(nCount);
	LoopBeginCheck(m)
	for (int i = 0;i < nCount;++i)
	{
		LoopDoCheck(m)
		PassiveSkillPropInfo& skillprop = outSkillPro.vPropInfoList[i];
		const char* strTemp = args.StringVal(i);
		CVarList temppro;
		StringUtil::SplitString(temppro, strTemp, ",");
		
		const char* strFlag = temppro.StringVal(0);
		if (strcmp(strFlag, ADD_PLAYER_PROPERTY_FLAG) == 0)
		{
			skillprop.nOptType		= PS_ADD_PLAYER_PROPERTY;
			skillprop.strPropName	= temppro.StringVal(1);
			skillprop.uPropAdd.fPropAdd	= temppro.FloatVal(2);
		}
		else if (strcmp(strFlag, ADD_SKILL_PROPERTY_FLAG) == 0)
		{
			skillprop.nOptType		= PS_ADD_SKILL_PROPERTY;
			skillprop.strSkillID	= temppro.StringVal(1);
			skillprop.strPropName	= temppro.StringVal(2);
			skillprop.uPropAdd.nPropAdd	= temppro.IntVal(3);
		}
		else if (strcmp(strFlag, CHANGE_SKILL_CONFIG_FLAG) == 0)
		{
			skillprop.nOptType		= PS_CHANGE_SKILL_CONFIG;
			skillprop.strSkillID	= temppro.StringVal(1);
			skillprop.strPropName	= temppro.StringVal(2);
			skillprop.uPropAdd.nPropAdd	= temppro.IntVal(3);
		}
		else if (strcmp(strFlag, ADD_SKILL_EFFECT_FLAG) == 0)
		{
			skillprop.nOptType		= PS_ADD_SKILL_EFFECT;
			skillprop.strSkillID	= temppro.StringVal(1);
			skillprop.strPropName	= temppro.StringVal(2);
		}
		else if (strcmp(strFlag, ADD_NEW_BUFF_FLAG) == 0)
		{
			skillprop.nOptType		= PS_ADD_NEW_BUFF;
			skillprop.strSkillID	= temppro.StringVal(1);
		}
	}
	return true;
}

bool SkillDataQueryModule::LoadBuffPathConfig()
{
    m_vBuffDataPathConfigArray.clear();
    std::string strPath = m_strWorkPath + BUFF_PATH_CONFIG;
    CXmlFile xml(strPath.c_str());
    std::string errMsg;
    if (!xml.LoadFromFile(errMsg))
    {
        // ·�������ļ��޷���
        extend_warning(LOG_WARNING, errMsg.c_str());
        return false;
    }

    CVarList vSectionList;
    xml.GetSectionList(vSectionList);
	LoopBeginCheck(n)
    for (size_t index = 0; index < vSectionList.GetCount(); ++index)
    {
		LoopDoCheck(n)
        const char* section = vSectionList.StringVal(index);
        if (NULL == section)
        {
            continue;
        }

        const char* type = xml.ReadString(section, "Type", "");
        const char* path = xml.ReadString(section, "Path", "");
        if (NULL == type || '\0' == *type || NULL == path || '\0' == *path)
        {
            continue;
        }
		DataPathConfigArray::iterator iter = m_vBuffDataPathConfigArray.find(type);
		if (iter == m_vBuffDataPathConfigArray.end())
		{
			PathConfigVector pathVec;
			pathVec.push_back(path);
			m_vBuffDataPathConfigArray.insert(make_pair(type, pathVec));
		}
		else
		{
			iter->second.push_back(path);
		}
    }

    return true;
}

// ���ؼ��ܶ����˺���
bool SkillDataQueryModule::LoadSkillGrowPropertyRec()
{
	// ���ܹ�ʽ����
	LoopBeginCheck(o)
	TStringPod<char, SkillMainData*>::iterator it = m_SkillMainData.Begin();
	for(; it != m_SkillMainData.End(); ++it)
	{
		LoopDoCheck(o)
		SkillMainData* pData = it.GetData();
		if (NULL == pData)
		{
			continue;
		}
		// ���ڼ��ܳɳ�
		if (pData->GetIsGrowProperty())
		{
			const char* strSkillId = pData->GetID();
			SkillGrowPropertyVec vecGrowProperty;
			if(LoadOneSkillGrowProperty(vecGrowProperty, strSkillId))
			{
				m_mapSkillGrowProperty.insert(std::make_pair(strSkillId, vecGrowProperty));
			}
		}
	}
	return true;
}

// ����ĳ�����ܵĶ����˺�
bool SkillDataQueryModule::LoadOneSkillGrowProperty(SkillGrowPropertyVec& vecExtraHurt, const std::string& strSkillId)
{
	std::string strPath = m_strWorkPath + SKILL_GROW_PROPERTY_PATH + strSkillId + ".xml";
	CXmlFile xml(strPath.c_str());
	std::string errMsg;
	if (!xml.LoadFromFile(errMsg))
	{
		// ·�������ļ��޷���
		extend_warning(LOG_WARNING, errMsg.c_str());
		return false;
	}

	vecExtraHurt.clear();
	CVarList vSectionList;
	xml.GetSectionList(vSectionList);
	int nSize = (int)vSectionList.GetCount();
	vecExtraHurt.resize(nSize);
	LoopBeginCheck(p)
	for (int index = 0; index < nSize; ++index)
	{
		LoopDoCheck(p)
		const char* section = vSectionList.StringVal(index);
		if (NULL == section)
		{
			continue;
		}
		// IDΪ��Ӧ�ĵȼ�
		int nLevel  = StringUtil::StringAsInt(section);
		if (nLevel > nSize || nLevel < 1)
		{
			continue;
		}
		// �ȼ���1����
		SkillGrowProperty& data = vecExtraHurt[nLevel - 1];
		data.nArrGrowProperty[SKILL_GP_ATTACK_ADD]		= xml.ReadInteger(section, "AttackAdd", 0);
		data.nArrGrowProperty[SKILL_GP_COOL_DOWN_TIME]	= xml.ReadInteger(section, "CoolDownTime", NO_VALID_GROW_PROPERTY);
		data.nArrGrowProperty[SKILL_GP_RESTORE_HP_RATE]	= xml.ReadInteger(section, "RestroeHPRate", NO_VALID_GROW_PROPERTY);
		data.nArrGrowProperty[SKILL_GP_DAMAGE_RATE]		= xml.ReadInteger(section, "SkillDamageRate", 0);
	}

	return true;
}

// ���ؼ��ܷ�������ͻ��˵ļ���ʱ���
// bool SkillDataQueryModule::LoadSkillTimeDiffConfig()
// {
// 	std::string strPath = m_strWorkPath + SKILL_TIME_DIFF_PATH;
// 	CXmlFile xml(strPath.c_str());
// 	std::string errMsg;
// 	if (!xml.LoadFromFile(errMsg))
// 	{
// 		// ·�������ļ��޷���
// 		extend_warning(LOG_WARNING, errMsg.c_str());
// 		return false;
// 	}
// 
// 	CVarList vSectionList;
// 	xml.GetSectionList(vSectionList);
// 	int nSize = (int)vSectionList.GetCount();
// 	m_vSkillTimeDiff.resize(nSize);
// 	LoopBeginCheck(q)
// 	for (int index = 0; index < nSize; ++index)
// 	{
// 		LoopDoCheck(q)
// 		const char* section = vSectionList.StringVal(index);
// 		if (NULL == section)
// 		{
// 			continue;
// 		}
// 		// IDΪ��Ӧ������
// 		int nIndex  = StringUtil::StringAsInt(section);
// 		if (nIndex >= nSize || nIndex < 0)
// 		{
// 			continue;
// 		}
// 		m_vSkillTimeDiff[nIndex] = xml.ReadInteger(section, "TimeDiff", 0);
// 	}
// 	return true;
// }

// ������������
bool SkillDataQueryModule::LoadMultiSkillConfig()
{
	std::string strPath = m_strWorkPath + MULTI_SKILL_PATH;
	CXmlFile xml(strPath.c_str());
	std::string errMsg;
	if (!xml.LoadFromFile(errMsg))
	{
		// ·�������ļ��޷���
		extend_warning(LOG_WARNING, errMsg.c_str());
		return false;
	}

	CVarList vSectionList;
	xml.GetSectionList(vSectionList);
	int nSize = (int)vSectionList.GetCount();
	m_vecMultiSkillInfo.reserve(nSize);
	LoopBeginCheck(r)
	for (int index = 0; index < nSize; ++index)
	{
		LoopDoCheck(r)
		const char* section = vSectionList.StringVal(index);
		if (NULL == section)
		{
			continue;
		}
		MultiSkillInfo info;
		info.nMultiSkillId = StringUtil::StringAsInt(section);
		const char* strSkillArray = xml.ReadString(section, "SkillArray", "");
		if (StringUtil::CharIsNull(strSkillArray))
		{
			continue;
		}

		// ��ȡһ����������id
		CVarList params;
		StringUtil::SplitString(params, strSkillArray, ",");
		int nSkillCount = (int)params.GetCount();
		info.vecSkill.resize(nSkillCount);
		LoopBeginCheck(s)
		for (int iparam = 0; iparam < nSkillCount; ++iparam)
		{
			LoopDoCheck(s)
			info.vecSkill[iparam] = params.StringVal(iparam);
		}

		m_vecMultiSkillInfo.push_back(info);
	}
	return true;
}	

// ���ݼ��ܳɳ������ҵ�����ֵ
int SkillDataQueryModule::GetPropertyBySkillGPType(const SkillMainData* pMainData, int nGPType)
{
	if (NULL == pMainData)
	{
		return NO_VALID_GROW_PROPERTY;
	}
	int nVal = NO_VALID_GROW_PROPERTY;
	switch(nGPType)
	{
	case SKILL_GP_COOL_DOWN_TIME:
		nVal = pMainData->GetCoolDownTime();
		break;
	}

	return nVal;
}

// ��ѯ�ɳ����Ե�ֵ
int SkillDataQueryModule::GetGrowProperty(IKernel* pKernel, const PERSISTID& skill, int nGPType)
{
	IGameObj* pSkillObj = pKernel->GetGameObj(skill);
	if (pSkillObj == NULL)
	{
		return 0;
	}

	int nQueryVal = NO_VALID_GROW_PROPERTY;
	do 
	{
		if (nGPType < 0 || nGPType >= MAX_SKILL_GP_NUM)
		{
			break;
		}
		// ��ȡԭ����.
		const char* strConfig = pKernel->GetConfig(skill);
		if (StringUtil::CharIsNull(strConfig))
		{
			break;
		}
		GrowPropertyRecMap::const_iterator iter = m_mapSkillGrowProperty.find(strConfig);
		if (iter != m_mapSkillGrowProperty.end())
		{
			int nSkillLevel = pSkillObj->QueryInt("Level");
			const SkillGrowPropertyVec& rec = iter->second;
			if (nSkillLevel > (int)rec.size() || nSkillLevel < 1)
			{
				break;
			}
			
			nQueryVal = rec[nSkillLevel - 1].nArrGrowProperty[nGPType];
		}
	} while (0);
	return nQueryVal;
}


// ��ѯPassiveSkill������
bool SkillDataQueryModule::QueryPassiveSkillProp(IKernel* pKernel, IVarList& outData, const int nPassiveSkillId, 
												 const int nPassiveSkillLV, const int nPSType, const char* strPropName)
{
	// �ҵ��������ܶ�Ӧ������
	const PassiveSkillLevelInfo* pSkillInfo = GetPassiveSkillLevelInfo(nPassiveSkillId, nPassiveSkillLV);
	if (NULL == pSkillInfo)
	{
		return false;
	}
	const PSPropInfoVec& vecProp = pSkillInfo->vPropInfoList;
	int nSize = (int)vecProp.size();
	LoopBeginCheck(r)
	for (int i = 0;i < nSize;++i)
	{
		LoopDoCheck(r)
		const PassiveSkillPropInfo& kInfo = vecProp[i];
		// nOptTypeΪPS_ADD_SKILL_EFFECTʱ,strPropNameΪ����Ч����
		if (PS_ADD_SKILL_EFFECT == nPSType)
		{
			if (kInfo.nOptType == nPSType)
			{
				outData << kInfo.strPropName;
			}
		}
		else
		{
			// �����������ֵ����Ϊfloat
			if (kInfo.nOptType == nPSType && strcmp(strPropName ,kInfo.strPropName.c_str()) == 0)
			{
				PS_ADD_PLAYER_PROPERTY == nPSType ? outData << kInfo.uPropAdd.fPropAdd : outData << kInfo.uPropAdd.nPropAdd;
			}
		}
	}

	bool bFindPro = false;
	if (outData.GetCount() > 0)
	{
		bFindPro = true;
	}

	return bFindPro;
}

bool SkillDataQueryModule::LoadBufferProp()
{
    if (!LoadBuffPathConfig())
    {
        return false;
    }

	LoopBeginCheck(s)
    for (DataPathConfigArray::const_iterator iter = m_vBuffDataPathConfigArray.begin();iter != m_vBuffDataPathConfigArray.end(); ++iter)
    {
		LoopDoCheck(s)
        const std::string &strFlag = (*iter).first;
        const PathConfigVector &vPath = (*iter).second;

		if (strFlag.compare("BuffNew") == 0)
        {
			LoadProp(vPath, m_BuffBase);
        }
		else if (strFlag.compare("BuffEffect") == 0)
		{
			LoadProp(vPath, m_eventParamsMap, m_BuffEffectData);
		}
    }

    return true;
}

template<class T>
bool SkillDataQueryModule::LoadProp(const PathConfigVector& vPath, TStringPod<char, T*>& map)
{
	fast_string strPath;
	std::string errMsg;
	LoopBeginCheck(a);
	for (PathConfigVector::const_iterator vit = vPath.begin(); vit != vPath.end(); ++vit)
	{
		LoopDoCheck(a);
		strPath = m_strWorkPath.c_str();
		strPath += (*vit).c_str();

		CXmlFile xml(strPath.c_str());
		if (!xml.LoadFromFile(errMsg))
		{
			extend_warning(LOG_WARNING, errMsg.c_str());
			return false;
		}

		size_t uSectionCount = xml.GetSectionCount();
		LoopBeginCheck(b);
		for (size_t i = 0; i < uSectionCount; ++i)
		{
			LoopDoCheck(b);
			const char* szSectionName = xml.GetSectionByIndex(i);
			T* pData = new (T); 
			if (pData)
			{
				pData->SetPropData(xml, szSectionName, m_StringBuffer);
				map.Add(szSectionName, pData);
			}
		}
	}

	return true;
}

template<class T>
bool SkillDataQueryModule::LoadProp(const PathConfigVector& vPath, const EventParamsMap& eventParamsMap, TStringPod<char, T*>& map)
{
	fast_string strPath;
	std::string errMsg;
	LoopBeginCheck(t)
	for (PathConfigVector::const_iterator vit = vPath.begin(); vit != vPath.end(); ++vit)
	{
		LoopDoCheck(t)
		strPath = m_strWorkPath.c_str();
		strPath += (*vit).c_str();

		CXmlFile xml(strPath.c_str());
		if (!xml.LoadFromFile(errMsg))
		{
			extend_warning(LOG_WARNING, errMsg.c_str());
			return false;
		}

		size_t uSectionCount = xml.GetSectionCount();
		LoopBeginCheck(a);
		for (size_t i = 0; i < uSectionCount; ++i)
		{
			LoopDoCheck(a);
			const char* szSectionName = xml.GetSectionByIndex(i);
			T* pData = new (T);
			if (pData)
			{
				pData->SetPropData(xml, szSectionName, eventParamsMap, m_StringBuffer);
				map.Add(szSectionName, pData);
			}
		}
	}

	return true;
}

template<class T>
void SkillDataQueryModule::ReleaseProp(TStringPod<char, T*>& map)
{
    typename TStringPod<char, T*>::iterator it = map.Begin();
	LoopBeginCheck(a);
	for (; it != map.End(); ++it)
    {
		LoopDoCheck(a);
        T* pData = it.GetData();
        if (pData)
        {
            delete(pData);
        }
    }
    map.Clear();
}

template<class T>
const T*  SkillDataQueryModule::GetPropPoint(const char* strKeyID, const TStringPod<char, T*>& map) const
{
	if(NULL == strKeyID)
	{
		return NULL;
	}
    typename TStringPod<char, T*>::const_iterator it = map.Find(strKeyID);
    if (it != map.End())
    {
        return it.GetData();
    }

    return NULL;
}

void SkillDataQueryModule::ParseHitTimeData()
{
	// ���ܹ�ʽ����
	LoopBeginCheck(o);
	TStringPod<char, SkillMainData*>::iterator it = m_SkillMainData.Begin();
	for (; it != m_SkillMainData.End(); ++it)
	{
		LoopDoCheck(o);
		SkillMainData* pData = it.GetData();
		if (NULL == pData)
		{
			continue;
		}
		const char* strSkillId = pData->GetID();
		if (strcmp(strSkillId, "M4300017_S1") == 0)
		{
			int a = 0;
		}
		const StringArray& sarHitTimeList = pData->GetExtraHitTimes();
		int nDataCount = (int)sarHitTimeList.GetCount();
		if (nDataCount <= 1)
		{
			continue;
		}
		int nCurHitTime = 0;
		int nCurHitCount = 0;
		int nNextHitTime = 0;
		HitTimeDataVec vHitTimeData;
		LoopBeginCheck(u);
		for (int i = 0; i < nDataCount;++i)
		{
			LoopDoCheck(u);
			if (i == 0)
			{
				nCurHitTime = atoi(sarHitTimeList.StringVal(i));
			}
			else
			{
				int nHitTime = atoi(sarHitTimeList.StringVal(i));
				nNextHitTime += nHitTime;
				++nCurHitCount;
				if (nNextHitTime >= MAX_SECTION_HIT_TIME)
				{
					HitTimeData data;
					data.nHitTime = nCurHitTime;
					data.nHitCount = nCurHitCount;

					vHitTimeData.push_back(data);
					nCurHitTime = nNextHitTime;
					nCurHitCount = 0;
					nNextHitTime = 0;
				}
				if (i == nDataCount - 1 && nNextHitTime < MAX_SECTION_HIT_TIME)
				{
					++nCurHitCount;
					HitTimeData data;
					data.nHitTime = nCurHitTime;
					data.nHitCount = nCurHitCount;

					vHitTimeData.push_back(data);
				}
			}
		}

		if (m_mapHitTimeData.find(strSkillId) == m_mapHitTimeData.end())
		{
			m_mapHitTimeData.insert(std::pair<std::string, HitTimeDataVec>(strSkillId, vHitTimeData));
		}
	}
}