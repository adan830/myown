//--------------------------------------------------------------------
// �ļ���:      WingModule.h
// ��  ��:      ���ģ��
// ˵  ��:
// ��������:    2018��3��15��
// ������:       
//    :       
//--------------------------------------------------------------------
#include "WingModule.h"
#include "FsGame/Define/WingDefine.h"
#include "Define/ClientCustomDefine.h"
#include "utils/XmlFile.h"
#include "utils/extend_func.h"
#include "utils/string_util.h"
#include "Define/Fields.h"
#include "utils/custom_func.h"
#include "CommonModule/ContainerModule.h"
#include "Define/ContainerDefine.h"
#include "CommonModule/LogModule.h"
#include "Define/CommandDefine.h"
#include "CommonModule/ReLoadConfigModule.h"
#include "Define/ResetTimerDefine.h"
#include "CommonModule/LuaExtModule.h"
#include "CommonModule/SwitchManagerModule.h"
#include "CapitalModule.h"
#include "CommonModule/FunctionEventModule.h"
#include "utils/exptree.h"
#include "utils/util_func.h"
#include "Define/ServerCustomDefine.h"
#include "CommonModule/EnvirValueModule.h"
#include "ResetTimerModule.h"
#include "ActivateFunctionModule.h"
#include "CommonModule/PropRefreshModule.h"
#include "Define/StaticDataDefine.h"
#include "CommonModule/CommRuleModule.h"

WingModule* WingModule::m_pWingModule = NULL;
CapitalModule* WingModule::m_pCapitalModule = NULL;
ContainerModule* WingModule::m_pContainerModule = NULL;

//��ʼ��
bool WingModule::Init(IKernel* pKernel)
{
	m_pWingModule = this;

	m_pCapitalModule = (CapitalModule*)pKernel->GetLogicModule("CapitalModule");
	m_pContainerModule = (ContainerModule*)pKernel->GetLogicModule("ContainerModule");
	Assert(NULL != m_pWingModule && NULL != m_pCapitalModule && NULL != m_pContainerModule);

	pKernel->AddEventCallback("player", "OnRecover", WingModule::OnPlayerRecover);
	pKernel->AddEventCallback("player", "OnReady", WingModule::OnPlayerReady);
	pKernel->AddEventCallback("player", "OnContinue", WingModule::OnPlayerReady);

	//����ǿ���ȼ�
	pKernel->AddIntCommandHook("player", COMMAND_WING_LEVEL_UP, WingModule::OnWingLevelUp);

	pKernel->AddIntCommandHook("player", COMMAND_MSG_ACTIVATE_FUNCTION, OnCommandActivateWing);
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_WING, WingModule::OnCustomWingMsg);

	if (!LoadResource(pKernel))
	{
		return false;
	}

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("WingConfig", WingModule::ReloadConfig);
#endif
	// ���嶨ʱ��
	DECL_RESET_TIMER(RESET_WING_STEP_BLESS, WingModule::ResetStepBlessVal);
	return true;
}

// ��ѯ��ҵ����԰�
int WingModule::QueryWingLvPropId(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	int nWingLv = pSelfObj->QueryInt(FIELD_PROP_WING_LEVEL);
	
	const LevelData* pData = QueryLevelData(nWingLv);
	if (NULL == pData)
	{
		return 0;
	}
	return pData->nPropertyPak;
}

// ��ѯ��ǰ����Ĭ�����
int WingModule::GetWingDefaultModel(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}
	// ��ǰ�Ľ׼��ͳ��ȼ�
	int nCurStep = pSelfObj->QueryInt(FIELD_PROP_WING_STEP);
	const StepData* pCurData = QueryStepData(nCurStep);
	if (NULL == pCurData)
	{
		return 0;
	}

	return pCurData->nWingModel;
}

// �ͻ��������Ϣ
int WingModule::OnCustomWingMsg(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self) || args.GetCount() < 2 || args.GetType(0) != VTYPE_INT || args.GetType(1) != VTYPE_INT)
	{
		return 0;
	}

	int nSubMsg = args.IntVal(1);
	switch (nSubMsg)
	{
	case WING_C2S_LEVEL_UP:
		m_pWingModule->OnCustomLevelUp(pKernel, self, args);
		break;
	case WING_C2S_STEP_UP:
		m_pWingModule->OnCustomStepUp(pKernel, self, args);
		break;
	case WING_C2S_WEAR:
		m_pWingModule->OnCustomWear(pKernel, self, args);
		break;
	case WING_C2S_HIDE:
		m_pWingModule->OnCustomHide(pKernel, self, args);
		break;
	}
	return 0;
}

//�ص����� ��Ҽ����������
int WingModule::OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	m_pWingModule->UpdateWingModifyProp(pKernel, self);

	// ��ʱ��ע��
	REGIST_RESET_TIMER(pKernel, self, RESET_WING_STEP_BLESS);
	return 0;
}

int WingModule::OnPlayerReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	return 0;
}

// ��Ӧ����ܼ���
int WingModule::OnCommandActivateWing(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	int nFuncType = args.IntVal(1);
	if (nFuncType == AFM_WING_FUNCTION)
	{
		int nWingLv = pSelfObj->QueryInt(FIELD_PROP_WING_LEVEL);
		// �Ѿ�����
		if (nWingLv > 0)
		{
			return 0;
		}
		// ���ó�ʼ�ȼ�
		pSelfObj->SetInt(FIELD_PROP_WING_LEVEL, 1);
		pSelfObj->SetInt(FIELD_PROP_WING_STEP, 1);

		const StepData* pCurData = m_pWingModule->QueryStepData(1);
		if (NULL != pCurData)
		{
			pSelfObj->SetInt(FIELD_PROP_WING, pCurData->nWingModel);
		}
	}
	return 0;
}

// ��Ӧ�������
int WingModule::OnWingLevelUp(IKernel* pKernel, const PERSISTID& self, const PERSISTID& item, const IVarList& args)
{
	m_pWingModule->UpdateWingModifyProp(pKernel, self);
	return 0;
}

// ��������ף��ֵ
int WingModule::ResetStepBlessVal(IKernel *pKernel, const PERSISTID &self, int slice)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	pSelfObj->SetInt(FIELD_PROP_WING_STEP_BLESS, 0);
	return 0;
}

// ��Ӧ�������
void WingModule::OnCustomLevelUp(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	//����ܿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_WING, self))
	{
		return;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// ��鱻������������ǰ������
	if (!CheckUpgradeTerm(pKernel, pSelfObj))
	{
		return;
	}

	int nRequestLevel = pSelfObj->QueryInt(FIELD_PROP_WING_LEVEL) + 1;
	// ����Ƿ�������������
	if (!LevelCostPlayerAssets(pKernel, self, nRequestLevel))
	{
		return;
	}
	
	// ���³��ȼ�
	pSelfObj->SetInt(FIELD_PROP_WING_LEVEL, nRequestLevel);
	
	// ����Ƿ���ڵ�ǰ�������ȼ�
	int nWingLv = pSelfObj->QueryInt(FIELD_PROP_WING_LEVEL);
	int nWingStep = pSelfObj->QueryInt(FIELD_PROP_WING_STEP);

	const StepData* pStepData = QueryStepData(nWingStep);
	if (NULL == pStepData)
	{
		return;
	}

	// ��������,����ף��ֵ
	if (nWingLv == pStepData->nMaxWingLv)
	{
		int nBlessVal = pSelfObj->QueryInt(FIELD_PROP_WING_STEP_BLESS);
		nBlessVal = __min(nBlessVal + pStepData->nMaxLevelAddBless, MAX_BLESS_VALUE);
		pSelfObj->SetInt(FIELD_PROP_WING_STEP_BLESS, nBlessVal);
	}

	// ��¼���������־
	RoleUpgradeLog log;

	log.type = LOG_ROLE_UPGRADE_WING_LEVEL;
	log.levelBefore = nRequestLevel - 1;
	log.levelAfter = nRequestLevel;
	LogModule::m_pLogModule->SaveRoleUpgradeLog(pKernel, self, log);

	pKernel->Command(self, self, CVarList() << COMMAND_WING_LEVEL_UP << nRequestLevel);
	CVarList msg;
	msg << SERVER_CUSTOMMSG_WING << WING_S2C_LEVEL_UP_SUC;
	pKernel->Custom(self, msg);
}

// ��Ӧ�������
void WingModule::OnCustomStepUp(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	//����ܿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_WING, self))
	{
		return;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// ���������Ƿ�Ϊ��
	PERSISTID toolBox = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
	if (!pKernel->Exists(toolBox))
	{
		return;
	}
	// ��ǰ�Ľ׼��ͳ��ȼ�
	int nCurStep = pSelfObj->QueryInt(FIELD_PROP_WING_STEP);
	int nWingLevel = pSelfObj->QueryInt(FIELD_PROP_WING_LEVEL);

	// �����Ľ׼�
	int nRequestStep = nCurStep + 1;
	const StepData* pCurData = QueryStepData(nCurStep);
	const StepData* pReData = QueryStepData(nRequestStep);
	// �׼�������, pReDataΪ��
	if (NULL == pReData || NULL == pCurData)
	{
		return;
	}

	// �����ȼ�
	if (nWingLevel < pCurData->nMaxWingLv)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19305, CVarList());
		return;
	}

	// �����Ϲ�����
	if (!CommRuleModule::CanDecItems(pKernel, self, pReData->vecConsumeItem))
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19304, CVarList());
		return;
	}

	//�е��ߣ��۵���
	CommRuleModule::ConsumeItems(pKernel, self, pReData->vecConsumeItem, FUNCTION_EVENT_ID_WING);
	// ��ȡ��ǰף��ֵ
	int nBlessVal = pSelfObj->QueryInt(FIELD_PROP_WING_STEP_BLESS);
	// ���㵱ǰ�ĸ���
	ExpTree exp;
	int nProbability = (int)exp.CalculateEvent(pKernel, self, PERSISTID(), PERSISTID(), PERSISTID(), pReData->strProbability.c_str());
	int nRanNum = util_random_int(MAX_BLESS_VALUE);
																							
	CVarList msg;
	msg << SERVER_CUSTOMMSG_WING << WING_S2C_STEP_UP_RESULT;
	// ף��ֵ�������׷�ֵ, ���׳ɹ�
	if (nBlessVal >= pCurData->nStepUpBlessVal && nRanNum < nProbability)
	{
		pSelfObj->SetInt(FIELD_PROP_WING_STEP_BLESS, 0);
		pSelfObj->SetInt(FIELD_PROP_WING_STEP, nRequestStep);
		// ��ʾĬ�����ʱ���¸߽׵ĳ�����
		int nCurWingModel = pSelfObj->QueryInt(FIELD_PROP_WING);
		if (nCurWingModel == pCurData->nWingModel)
		{
			pSelfObj->SetInt(FIELD_PROP_WING, pReData->nWingModel);
		}

		msg << WING_STEP_UP_SUC << 0;
	}
	else
	{
		int nAddBlessVal = pReData->nMinBless + util_random_int(pReData->nMaxBless - pReData->nMinBless);
		int nOldBlessVal = pSelfObj->QueryInt(FIELD_PROP_WING_STEP_BLESS);
		int nCurBlessVal = __min(nAddBlessVal + nOldBlessVal, MAX_BLESS_VALUE);
		pSelfObj->SetInt(FIELD_PROP_WING_STEP_BLESS, nCurBlessVal);

		msg << WING_STEP_UP_FAILED << nCurBlessVal - nOldBlessVal;
	}

	pKernel->Custom(self, msg);

	// ��¼���������־
	RoleUpgradeLog log;

// 	log.type = LOG_ROLE_UPGRADE_WING_STEP;
// 	log.levelBefore = nRequestLevel - 1;
// 	log.levelAfter = nRequestLevel;
// 	LogModule::m_pLogModule->SaveRoleUpgradeLog(pKernel, self, log);
}

// ��Ӧ�������
void WingModule::OnCustomWear(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	//����ܿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_WING, self))
	{
		return;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

// 	int nStep = pSelfObj->QueryInt(FIELD_PROP_WING_STEP);
// 	const StepData* pStepData = QueryStepData(nStep);
// 	if (NULL == pStepData)
// 	{
// 		return;
// 	}

	// ���ó�����
	pSelfObj->SetInt(FIELD_PROP_IS_WEAR_WING, WEAR_WING);

	// ��ʼ���
}

// ��Ӧ���س��
void WingModule::OnCustomHide(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	//����ܿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_WING, self))
	{
		return;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	pSelfObj->SetInt(FIELD_PROP_IS_WEAR_WING, NO_WEAR_WING);

	// ���
}

// ��ȡ����
bool WingModule::LoadResource(IKernel* pKernel)
{
	// ��ȡ�����������
	if (!LoadLevelResource(pKernel))
	{
		return false;
	}

	// ��ȡ�����������
	if (!LoadStepResource(pKernel))
	{
		return false;
	}
	return true;
}

// ��ȡ��������
bool WingModule::LoadLevelResource(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += "ini/SystemFunction/Wing/WingLevelConfig.xml";

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	int nSectionCount = xmlfile.GetSectionCount();

	m_vecLevelData.clear();
	m_vecLevelData.reserve(nSectionCount);

	LoopBeginCheck(e);
	for (int i = 0; i < nSectionCount; ++i)
	{
		LoopDoCheck(e);
		const char* section = xmlfile.GetSectionByIndex(i);
		int nSection = StringUtil::StringAsInt(section);

		LevelData data;
		const char* strCapital = xmlfile.ReadString(section, "ConsumeCapital", "");
		CommRuleModule::ParseConsumeVec(strCapital, data.vecConsumeCapital);

		const char* strItems = xmlfile.ReadString(section, "ConsumeItem", "");
		CommRuleModule::ParseConsumeVec(strItems, data.vecConsumeItem);

		data.nPropertyPak = xmlfile.ReadInteger(section, "PropertyPak", 0);
		m_vecLevelData.push_back(data);
	}

	return true;
}

// ��ȡ��������
bool WingModule::LoadStepResource(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += "ini/SystemFunction/Wing/WingStepConfig.xml";

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	int nSectionCount = xmlfile.GetSectionCount();

	m_vecStepData.clear();
	m_vecStepData.reserve(nSectionCount);

	LoopBeginCheck(e);
	for (int i = 0; i < nSectionCount; ++i)
	{
		LoopDoCheck(e);
		const char* section = xmlfile.GetSectionByIndex(i);
		int nSection = StringUtil::StringAsInt(section);

		StepData data;
		const char* strCostItem = xmlfile.ReadString(section, "CostItem", "");
		CommRuleModule::ParseConsumeVec(strCostItem, data.vecConsumeItem);

		data.strProbability = xmlfile.ReadString(section, "Probability", "");
		data.nMinBless = xmlfile.ReadInteger(section, "MinBless", 0);
		data.nMaxBless = xmlfile.ReadInteger(section, "MaxBless", 0);
		data.nMaxWingLv = xmlfile.ReadInteger(section, "MaxWingLevel", 0);
		data.nStepUpBlessVal = xmlfile.ReadInteger(section, "StepUpBlessVal", 0);
		data.nWingModel = xmlfile.ReadInteger(section, "WingModel", 0);
		data.nMaxLevelAddBless = xmlfile.ReadInteger(section, "MaxLevelAddBless", 0);

		m_vecStepData.push_back(data);
	}

	return true;
}

// ���������������
bool WingModule::CheckUpgradeTerm(IKernel* pKernel, IGameObj* pSelfObj)
{
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ����Ƿ���ڵ�ǰ�������ȼ�
	int nWingLv = pSelfObj->QueryInt(FIELD_PROP_WING_LEVEL);
	int nWingStep = pSelfObj->QueryInt(FIELD_PROP_WING_STEP);

	const StepData* pStepData = QueryStepData(nWingStep);
	if (NULL == pStepData)
	{
		return false;
	}
	
	// ��򼶱��ѵ���󼶱�
	if (nWingLv >= pStepData->nMaxWingLv)
	{
		::CustomSysInfo(pKernel, pSelfObj->GetObjectId(), SYSTEM_INFO_ID_19301, CVarList());
		return false;
	}

	return true;
}

// ���ĳ����������Ʒ�ͽ�Ǯ
bool WingModule::LevelCostPlayerAssets(IKernel* pKernel, const PERSISTID& self, int nLevel)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	const LevelData* pData = QueryLevelData(nLevel);
	// ���������Ƿ�Ϊ��
	PERSISTID toolBox = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
	if (!pKernel->Exists(toolBox) || NULL == pData)
	{
		return false;
	}

	// ���Ǯ�����Ϲ�����
	if (!CommRuleModule::CanDecCapitals(pKernel, self, pData->vecConsumeCapital))
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19302, CVarList());
		return false;
	}
	
	if (!CommRuleModule::CanDecItems(pKernel, self, pData->vecConsumeItem))
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19303, CVarList());
		return false;
	}

	// ��ʼ��Ǯ������
	CommRuleModule::ConsumeCapitals(pKernel, self, pData->vecConsumeCapital, FUNCTION_EVENT_ID_WING);

	//�е��ߣ��۵���
	CommRuleModule::ConsumeItems(pKernel, self, pData->vecConsumeItem, FUNCTION_EVENT_ID_WING);
	return true;
}

// ������ҳ������
bool WingModule::UpdateWingModifyProp(IKernel* pKernel, const PERSISTID& self)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	int nMeridianLevelPropId = WingModule::m_pWingModule->QueryWingLvPropId(pKernel, self);
	PropRefreshModule::UpdateModifyProp(pKernel, self, STATIC_DATA_WING_ATTRIBUTE_PACK, WING_PROPPAK_ID, nMeridianLevelPropId);
	return true;
}

// ��ѯ��������
const WingModule::StepData* WingModule::QueryStepData(int nStep)
{
	// ������0 ����
	int nIndex = --nStep;
	int nSize = m_vecStepData.size();
	if (nIndex < 0 || nIndex >= nSize)
	{
		return NULL;
	}

	return &m_vecStepData[nIndex];
}

// ��ѯ��������
const WingModule::LevelData* WingModule::QueryLevelData(int nLevel)
{
	// ������0 ����
	int nIndex = --nLevel;
	int nSize = m_vecLevelData.size();
	if (nIndex < 0 || nIndex >= nSize)
	{
		return NULL;
	}

	return &m_vecLevelData[nIndex];
}

void WingModule::ReloadConfig(IKernel* pKernel)
{
	m_pWingModule->LoadResource(pKernel);
}