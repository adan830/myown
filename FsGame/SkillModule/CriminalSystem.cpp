//--------------------------------------------------------------------
// �ļ���:		CriminalSystem.h
// ��  ��:		���ͷ�ϵͳ
// ˵  ��:					
// ��������:	2017��07��24��
// ������:		liumf   
//--------------------------------------------------------------------
#include "CriminalSystem.h"
#include "FsGame/Define/PKModelDefine.h"
#include "Define/CommandDefine.h"
#include "utils/XmlFile.h"
#include "utils/extend_func.h"
#include "Define/Fields.h"
#include "Define/GameDefine.h"
#include "utils/custom_func.h"
#include "Define/ServerCustomDefine.h"
#include "Define/SnsDefine.h"
#include "SystemFunctionModule/ResetTimerModule.h"
#include "utils/string_util.h"
#include "Interface/FightInterface.h"
#include "SystemFunctionModule/CapitalModule.h"

#define CRIMINAL_CONST_CONFIG_PATH "ini/Skill/CriminalConst.xml"		// ��������·��
#define CRIMINAL_RULE_CONFIG_PATH "ini/Skill/CriminalAddRule.xml"		// ���ֵ���ӹ���
#define CRIMINAL_SCORE_RULE_CONFIG_PATH "ini/Skill/CriminalScoreRule.xml"	// ���ֵɱ�ֻ��ֹ���
#define CRIMINAL_PUBLISH_DATA_CONFIG_PATH "ini/Skill/CriminalPublishRule.xml"	// ���ͷ�����

// ��ʼ��
bool CriminalSystem::Init(IKernel* pKernel)
{
	pKernel->AddEventCallback("player", "OnRecover", OnPlayerRecover);

	pKernel->AddIntCommandHook("player", COMMAND_DAMAGETARGET, OnCommandDamagePlayer);
	pKernel->AddIntCommandHook("PetNpc", COMMAND_DAMAGETARGET, OnCommandDamagePlayer);

	//pKernel->AddIntCommandHook("player", COMMAND_BEKILL, OnCommandBeKill);
	pKernel->AddIntCommandHook("player", COMMAND_KILLTARGET, OnCommandKillPlayer);

	DECL_HEARTBEAT(CriminalSystem::HB_CleanCrimeState);
	DECL_HEARTBEAT(CriminalSystem::HB_DecCrimeValue);

	DECL_CRITICAL(CriminalSystem::C_OnFightStateChanged);
	DECL_CRITICAL(CriminalSystem::C_OnCrimeValueChanged);

	//DECL_RESET_TIMER(RESET_KILL_CRIMER_CHIVALRY, CriminalSystem::ResetAddChivalry);

	if (!LoadResource(pKernel))
	{
		return false;
	}
	return true;
}

// �ر�
bool CriminalSystem::Shut(IKernel* pKernel)
{
	return true;
}

// ���ö�ȡ
bool CriminalSystem::LoadResource(IKernel* pKernel)
{
	if (!LoadConstConfig(pKernel))
	{
		return false;
	}

	if (!LoadCriminalRule(pKernel))
	{
		return false;
	}

	if (!LoadCrimeScorelRule(pKernel))
	{
		return false;
	}

	if (!LoadCrimiePublishData(pKernel))
	{
		return false;
	}

	return true;
}

/// \brief ����
int CriminalSystem::OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	if (OFFLINE == pSelfObj->QueryInt(FIELD_PROP_ONLINE))
	{
		return 0;
	}
	// ���ս��״̬�ص�
	pKernel->AddCritical(self, FIELD_PROP_FIGHT_STATE, "CriminalSystem::C_OnFightStateChanged");	   \

	// �������ֵ�仯�ص�
	pKernel->AddCritical(self, FIELD_PROP_CRIMINAL_VALUE, "CriminalSystem::C_OnCrimeValueChanged");

	int nCrimeValue = pSelfObj->QueryInt(FIELD_PROP_CRIMINAL_VALUE);
	if (nCrimeValue > 0)
	{
		ADD_HEART_BEAT(pKernel, self, "CriminalSystem::HB_DecCrimeValue", CHECK_CRIME_STATE_TIME_INTERVAL);
		int nRedCrimeValue = CriminalSystemSingleton::Instance()->m_kCriminalConst.nRedCrimeValue;
		// ��Ϊ����
		if (nCrimeValue >= nRedCrimeValue)
		{
			pSelfObj->SetInt(FIELD_PROP_CRIME_STATE, CS_RED_STATE);
		}
	}


	return 0;
}

// ������ʱ��������
int CriminalSystem::HB_CleanCrimeState(IKernel * pKernel, const PERSISTID & self, int slice)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}
	int nCrimeState = pSelfObj->QueryInt(FIELD_PROP_CRIME_STATE);
	if (CS_GRAY_STATE == nCrimeState)
	{
		pSelfObj->SetInt(FIELD_PROP_CRIME_STATE, CS_NORMAL_STATE);
	}
	return 0;
}

// ���߼������ֵ
int CriminalSystem::HB_DecCrimeValue(IKernel * pKernel, const PERSISTID & self, int slice)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}
									
	int nCrimeValue = pSelfObj->QueryInt(FIELD_PROP_CRIMINAL_VALUE);
	if (nCrimeValue <= 0)
	{
		return 0;
	}
	int nCrimeOnlineTime = pSelfObj->QueryInt(FIELD_PROP_CRIMIE_ONLINE_TIME);
	nCrimeOnlineTime += slice;
	// �ﵽʱ����,�������ֵ,���¼�ʱ
	int nCurTimeRate = CriminalSystemSingleton::Instance()->m_kCriminalConst.nTimeToCrimeRate;
	if (nCrimeOnlineTime >= nCurTimeRate)
	{
		--nCrimeValue;
		pSelfObj->SetInt(FIELD_PROP_CRIMINAL_VALUE, nCrimeValue);
		nCrimeOnlineTime -= nCurTimeRate;
	}

	pSelfObj->SetInt(FIELD_PROP_CRIMIE_ONLINE_TIME, nCrimeOnlineTime);	
	return 0;
}

// ��Ӧ�˺�ĳ�����
int CriminalSystem::OnCommandDamagePlayer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pAttacker = NULL;
	IGameObj* pTargetObj = NULL;
	PERSISTID target = args.ObjectVal(1);
	if (!IsValidPublicScenePVP(pKernel, self, target, pAttacker, pTargetObj))
	{
		return 0;
	}

	int nSelfCrimeState = pAttacker->QueryInt(FIELD_PROP_CRIME_STATE);
	int nTargetCrimeState = pTargetObj->QueryInt(FIELD_PROP_CRIME_STATE);
	// �������ǰ������,�����������ɫ
	if (nTargetCrimeState > CS_NORMAL_STATE)
	{
		return 0;
	}
	// �Լ��ǰ���
	if (nSelfCrimeState == CS_NORMAL_STATE)
	{
		pAttacker->SetInt(FIELD_PROP_CRIME_STATE, CS_GRAY_STATE);
	}
	else if (CS_GRAY_STATE == nSelfCrimeState)	  // ȥ���ָ����������� 
	{
		DELETE_HEART_BEAT(pKernel, pAttacker->GetObjectId(), "CriminalSystem::HB_CleanCrimeState");
	}

	return 0;
}

// ��Ӧ��ɱ��
// int CriminalSystem::OnCommandBeKill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
// {
// 	IGameObj* pAttacker = NULL;
// 	IGameObj* pTargetObj = NULL;
// 
// 
// 	if (!IsValidPublicScenePVP(pKernel, sender, self, pAttacker, pTargetObj))
// 	{
// 		return 0;
// 	}														 
// 
// 	// �����������  ���㹥���ߵ�����ֵ
// 	int nTargetCrimeState = pTargetObj->QueryInt(FIELD_PROP_CRIME_STATE);
// 	if (CS_RED_STATE == nTargetCrimeState)
// 	{
// 		CVarList attackerlist;
// 		if (!FightInterfaceInstance->GetAttackerList(pKernel, attackerlist, pTargetObj->GetObjectId(), PERSISTID()))
// 		{
// 			return 0;
// 		}
// 
// 		int nCount = (int)attackerlist.GetCount();
// 		LoopBeginCheck(a);
// 		for (int i = 0; i < nCount; ++i)
// 		{
// 			LoopDoCheck(a);
// 			PERSISTID player = attackerlist.ObjectVal(i);
// 			IGameObj* pPlayerObj = pKernel->GetGameObj(player);
// 			if (NULL == pPlayerObj || TYPE_PLAYER != pPlayerObj->GetClassType())
// 			{
// 				continue;
// 			}
// 			CriminalSystemSingleton::Instance()->AddChivalry(pKernel, pPlayerObj);
// 		}
// 	}
// 	return 0;	
// }

// ��Ӧɱ�����
int CriminalSystem::OnCommandKillPlayer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pAttacker = NULL;
	IGameObj* pTargetObj = NULL;
	PERSISTID target = args.ObjectVal(1);
	if (!IsValidPublicScenePVP(pKernel, self, target, pAttacker, pTargetObj))
	{
		return 0;
	}

	// ɱ���������,�������ֵ																				  
	int nTargetCrimeState = pTargetObj->QueryInt(FIELD_PROP_CRIME_STATE);
	if (CS_NORMAL_STATE == nTargetCrimeState)
	{
		int nCrimeValue = pAttacker->QueryInt(FIELD_PROP_CRIMINAL_VALUE);
		int nAddValue = CriminalSystemSingleton::Instance()->QueryAddCriminalValue(pAttacker, pTargetObj);
		if (nAddValue > 0)
		{
			int nNewCrimeValue = __min(nCrimeValue + nAddValue, CriminalSystemSingleton::Instance()->m_kCriminalConst.nMaxCrimeValue);
			pAttacker->SetInt(FIELD_PROP_CRIMINAL_VALUE, nNewCrimeValue);
		}
	}
	return 0;
}

// ����Ԫ���������ֵ
int CriminalSystem::OnCustomCostMoneyCleanCrime(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (args.GetCount() < 3 || args.GetType(2) != VTYPE_INT)
	{
		return 0;
	}
	int nCleanCrimeValue = args.IntVal(2);
	if (nCleanCrimeValue <= 0)
	{
		return 0;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;																							    
	}

	int nCrimeValue = pSelfObj->QueryInt(FIELD_PROP_CRIMINAL_VALUE);
	if (nCleanCrimeValue > nCrimeValue)
	{
		// ���ֵ����
		//::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_CLEAR_CRIME_VALUE_FAILED_GREATER, CVarList());
		return 0;
	}

	// �������ĵĻ�������
	int nCostRate = m_kCriminalConst.nClearCrimeCostRate;
	int nCostMoney = nCleanCrimeValue * nCostRate;
	if (!CapitalModule::m_pCapitalModule->CanDecCapital(pKernel, self, m_kCriminalConst.nClearCrimeMoneyType, nCostMoney))
	{
		//::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_CLEAR_CRIME_VALUE_FAILED_MONEY, CVarList());
		return 0;
	}
	if (CapitalModule::m_pCapitalModule->DecCapital(pKernel, self, m_kCriminalConst.nClearCrimeMoneyType, nCostMoney, FUNCTION_EVENT_ID_CRIME_SYSTEM) != DC_SUCCESS)
	{
		//::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_CLEAR_CRIME_VALUE_FAILED_MONEY, CVarList());
		return 0;
	}

	pSelfObj->SetInt(FIELD_PROP_CRIMINAL_VALUE, nCrimeValue - nCleanCrimeValue);

	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_PK_MODEL << PK_S2C_COST_MONEY_SUCCESS);
	return 0;
}

// �������ֵ����
void CriminalSystem::AddDecCrimeScore(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pDeadNpcObj)
{
	if (NULL == pSelfObj || NULL == pDeadNpcObj)
	{
		return;
	}

	// ������ֵ
	int nCrimeValue = pSelfObj->QueryInt(FIELD_PROP_CRIMINAL_VALUE);
	if (nCrimeValue <= 0)	  // û�����ֵ,���û���
	{
		return;
	}
	int nAddScore = CriminalSystemSingleton::Instance()->QueryAddDecCrimeScore(pDeadNpcObj);
	if (nAddScore <= 0)
	{
		return;
	}
	int nCurScore = pSelfObj->QueryInt(FIELD_PROP_DEC_CRIME_SCORE) + nAddScore;
	int nDecScoreRate = CriminalSystemSingleton::Instance()->m_kCriminalConst.nScroeToCrimeRate;
	if (nCurScore >= nDecScoreRate)
	{
		nCurScore -= nDecScoreRate;
		pSelfObj->SetInt(FIELD_PROP_CRIMINAL_VALUE, --nCrimeValue);
		if (nCrimeValue <= 0)
		{
			pSelfObj->SetInt(FIELD_PROP_DEC_CRIME_SCORE, 0);
			// ����֪ͨ�ͻ���
		}
		else
		{
			pSelfObj->SetInt(FIELD_PROP_DEC_CRIME_SCORE, nCurScore);
		}
	}
	else
	{
		pSelfObj->SetInt(FIELD_PROP_DEC_CRIME_SCORE, nCurScore);
	}
}

// ��ѯ�����ͷ�ϵ�� ���Ǻ�������false
bool CriminalSystem::QueryCiminalRate(IKernel* pKernel, float& fOutPublishRate, const PERSISTID& self, int nFunctionType, int nQueryType)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj || TYPE_PLAYER != pSelfObj->GetClassType() || pSelfObj->QueryInt(FIELD_PROP_CRIME_STATE) != CS_RED_STATE)
	{
		return false;
	}

	bool bResult = false;
	int nSize = (int)m_vecCrimiePublishData.size();
	LoopBeginCheck(e);
	for (int i = 0; i < nSize;++i)
	{
		LoopDoCheck(e);
		const CrimiePublishData& data = m_vecCrimiePublishData[i];
		if (data.nFunctionType == nFunctionType)
		{
			bResult = true;
			fOutPublishRate = nQueryType == CP_CAPITAL_RATE ? data.fCapitalPublishRate : data.fExpPublishRate;
			break;
		}
	}

	return bResult;
}

// ��Ҫ��������֤
bool CriminalSystem::IsValidPublicScenePVP(IKernel* pKernel, const PERSISTID& attacker, const PERSISTID& target, IGameObj*& pAttackerObj, IGameObj*& pTargetObj)
{
	PERSISTID realattacker = get_pet_master(pKernel, attacker);
	pAttackerObj = pKernel->GetGameObj(realattacker);
	pTargetObj = pKernel->GetGameObj(target);
	IGameObj* pSceneObj = pKernel->GetSceneObj();
	int nAttackType = pSceneObj->QueryInt(FIELD_PROP_ATTACK_TYPE);
	if (NULL == pAttackerObj || NULL == pTargetObj || TYPE_PLAYER != pAttackerObj->GetClassType()
		|| TYPE_PLAYER != pTargetObj->GetClassType() || pAttackerObj == pTargetObj || nAttackType != SCENE_ATTACK_PUBLIC_SCENE
		|| pAttackerObj->QueryInt(FIELD_PROP_GROUP_ID) > -1 || pTargetObj->QueryInt(FIELD_PROP_GROUP_ID) > -1)	 // Ұ�ⳡ�����������,����¼���
	{
		return false;
	}

	return true;
}

//ս��״̬�����ı�
int CriminalSystem::C_OnFightStateChanged(IKernel *pKernel, const PERSISTID &self, const char *property, const IVar &old)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	int nFightState = pSelfObj->QueryInt(FIELD_PROP_FIGHT_STATE);
	int nCrimeState = pSelfObj->QueryInt(FIELD_PROP_CRIME_STATE);
	// תΪ��ս״̬
	if (0 == nFightState)
	{
		 // �����ɻ�����Ϊ����������
		if (CS_GRAY_STATE == nCrimeState)
		{
			ADD_COUNT_BEAT(pKernel, self, "CriminalSystem::HB_CleanCrimeState", CriminalSystemSingleton::Instance()->m_kCriminalConst.nGrayToNormalTime, 1);
		}
	}
	return 0;
}

//���ֵ�仯
int CriminalSystem::C_OnCrimeValueChanged(IKernel *pKernel, const PERSISTID &self, const char *property, const IVar &old)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	int nCrimeValue = pSelfObj->QueryInt(FIELD_PROP_CRIMINAL_VALUE);
	int nCrimeState = pSelfObj->QueryInt(FIELD_PROP_CRIME_STATE);
	
	int nRedCrimeValue = CriminalSystemSingleton::Instance()->m_kCriminalConst.nRedCrimeValue;

	if (nCrimeValue > 0)
	{
		// û�м�ʱ�ļ����ֵ������,�ͼ���
		if (!pKernel->FindHeartBeat(self, "CriminalSystem::HB_DecCrimeValue"))
		{
			ADD_HEART_BEAT(pKernel, self, "CriminalSystem::HB_DecCrimeValue", CHECK_CRIME_STATE_TIME_INTERVAL);
		}
	}

	// ��Ϊ����
	if (nCrimeValue >= nRedCrimeValue)
	{
		if (nCrimeState < CS_RED_STATE)
		{
			pSelfObj->SetInt(FIELD_PROP_CRIME_STATE, CS_RED_STATE);
		}
	}
	else
	{
		if (nCrimeState == CS_RED_STATE)
		{
			pSelfObj->SetInt(FIELD_PROP_CRIME_STATE, CS_NORMAL_STATE);
		}
		// �Ѿ�û�����ֵ
		if (nCrimeValue <= 0)
		{
			// ɾ�����ʱ�������
			DELETE_HEART_BEAT(pKernel, self, "CriminalSystem::HB_DecCrimeValue");

			// ����������ֵ�Ļ���
			pSelfObj->SetInt(FIELD_PROP_DEC_CRIME_SCORE, 0);

			// ������ֵ����ʱ��
			pSelfObj->SetInt(FIELD_PROP_CRIMIE_ONLINE_TIME, 0);
		}
	}

	return 0;
}

// ��ȡ��������
bool CriminalSystem::LoadConstConfig(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += CRIMINAL_CONST_CONFIG_PATH;

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	const char* strSectionName = xmlfile.GetSectionByIndex(0);

	m_kCriminalConst.nGrayToNormalTime		= xmlfile.ReadInteger(strSectionName, "GrayToNormalTime", 0) * 1000;
	m_kCriminalConst.nRedCrimeValue			= xmlfile.ReadInteger(strSectionName, "RedCrimeValue", 0);
	m_kCriminalConst.nKillCrimerChivalry	= xmlfile.ReadInteger(strSectionName, "KillCrimerChivalry", 0);
	m_kCriminalConst.nScroeToCrimeRate		= xmlfile.ReadInteger(strSectionName, "ScroeToCrimeRate", 0);
	m_kCriminalConst.nTimeToCrimeRate		= xmlfile.ReadInteger(strSectionName, "TimeToCrimeRate", 0) * 60 * 1000;
	m_kCriminalConst.nClearCrimeCostRate	= xmlfile.ReadInteger(strSectionName, "ClearCrimeCostRate", 0);
	m_kCriminalConst.nJailClearCrimeCostRate = xmlfile.ReadInteger(strSectionName, "JailClearCrimeCostRate", 0);
	m_kCriminalConst.nClearCrimeMoneyType	= xmlfile.ReadInteger(strSectionName, "ClearCrimeMoneyType", 0);
	m_kCriminalConst.nMaxChivalry			= xmlfile.ReadInteger(strSectionName, "MaxChivalry", 0);
	m_kCriminalConst.nMaxCrimeValue			= xmlfile.ReadInteger(strSectionName, "MaxCrimeValue", 0);
	m_kCriminalConst.nForceBeJailCrimeVal	= xmlfile.ReadInteger(strSectionName, "ForceBeJailCrimeVal", 0);

	return true;

}

// ��ȡ���ֵ���ӹ���
bool CriminalSystem::LoadCriminalRule(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += CRIMINAL_RULE_CONFIG_PATH;

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	m_vecCriminalValue.clear();

	const size_t iSectionCount = xmlfile.GetSectionCount();
	m_vecCriminalValue.reserve(iSectionCount);
	LoopBeginCheck(a);
	for (unsigned i = 0; i < iSectionCount; i++)
	{
		LoopDoCheck(a);
		CriminalRule data;
		const char* strSectionName = xmlfile.GetSectionByIndex(i);

		data.nMinLevelDiff = xmlfile.ReadInteger(strSectionName, "LevelMinDiff", 0);
		data.nMaxLevelDiff = xmlfile.ReadInteger(strSectionName, "LevelMaxDiff", 0);
		data.nCriminalValue = xmlfile.ReadInteger(strSectionName, "CriminalValue", 0);

		m_vecCriminalValue.push_back(data);
	}

	return true;
}

// ��ȡ���ֵɱ�ֻ��ֹ���
bool CriminalSystem::LoadCrimeScorelRule(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += CRIMINAL_SCORE_RULE_CONFIG_PATH;

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	m_vecCrimieScore.clear();

	const size_t iSectionCount = xmlfile.GetSectionCount();
	m_vecCrimieScore.reserve(iSectionCount);
	LoopBeginCheck(a);
	for (unsigned i = 0; i < iSectionCount; i++)
	{
		LoopDoCheck(a);
		CrimieScore data;
		const char* strSectionName = xmlfile.GetSectionByIndex(i);

		data.nNpcType = xmlfile.ReadInteger(strSectionName, "NpcType", 0);
		data.nScore = xmlfile.ReadInteger(strSectionName, "DecCrimeScore", 0);

		m_vecCrimieScore.push_back(data);
	}

	return true;
}

// ��ȡ���ͷ���������
bool CriminalSystem::LoadCrimiePublishData(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += CRIMINAL_PUBLISH_DATA_CONFIG_PATH;

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	m_vecCrimiePublishData.clear();

	const size_t iSectionCount = xmlfile.GetSectionCount();
	m_vecCrimiePublishData.reserve(iSectionCount);
	LoopBeginCheck(a);
	for (unsigned i = 0; i < iSectionCount; i++)
	{
		LoopDoCheck(a);
		CrimiePublishData data;
		const char* strSectionName = xmlfile.GetSectionByIndex(i);

		data.nFunctionType = StringUtil::StringAsInt(strSectionName);
		data.fCapitalPublishRate = xmlfile.ReadFloat(strSectionName, "CapitalPublishRate", 0);
		data.fExpPublishRate = xmlfile.ReadFloat(strSectionName, "ExpPublishRate", 0);

		m_vecCrimiePublishData.push_back(data);
	}

	return true;
}

// ��ѯ���ӵ����ֵ
int CriminalSystem::QueryAddCriminalValue(IGameObj* pAttackerObj, IGameObj* pTargetObj)
{
	if (NULL == pAttackerObj || NULL == pTargetObj)
	{
		return 0;
	}
	int nAttackerLv = pAttackerObj->QueryInt(FIELD_PROP_LEVEL);
	int nTargetLv = pTargetObj->QueryInt(FIELD_PROP_LEVEL);

	int nLevelDiff = nAttackerLv - nTargetLv;
	int nAddCriminal = 0;
	LoopBeginCheck(a);
	int nSize = (int)m_vecCriminalValue.size();
	for (int i = 0; i < nSize;++i)
	{
		LoopDoCheck(a);
		const CriminalRule& data = m_vecCriminalValue[i];
		if (nLevelDiff >= data.nMinLevelDiff && nLevelDiff <= data.nMaxLevelDiff)
		{
			nAddCriminal = data.nCriminalValue;
			break;
		}
	}

	return nAddCriminal;
}

// ��ѯ���ӵĻ���
int CriminalSystem::QueryAddDecCrimeScore(IGameObj* pTargetObj)
{
	int nAddScore = 0;
	do 
	{
		if (NULL == pTargetObj || TYPE_NPC != pTargetObj->GetClassType())
		{
			break;
		}

		if (!pTargetObj->FindAttr(FIELD_PROP_NPC_TYPE))
		{
			break;
		}

		int nNpcType = pTargetObj->QueryInt(FIELD_PROP_NPC_TYPE);
		int nSize = (int)m_vecCrimieScore.size();
		LoopBeginCheck(b);
		for (int i = 0; i < nSize;++i)
		{
			LoopDoCheck(b);
			const CrimieScore& data = m_vecCrimieScore[i];
			if (data.nNpcType == nNpcType)
			{
				nAddScore = data.nScore;
				break;
			}
		}
	} while (0);
	
	return nAddScore;
}

// ��������ֵ
// void CriminalSystem::AddChivalry(IKernel* pKernel, IGameObj* pPlayerObj)
// {
// 	if (NULL == pPlayerObj)
// 	{
// 		return;
// 	}
// 
// 	int nCurChivalry = pPlayerObj->QueryInt(FIELD_PROP_KILL_CRIMER_CHIVALRY);
// 	// �ﵽ���ֵ��
// 	if (nCurChivalry >= m_kCriminalConst.nMaxChivalry)
// 	{
// 		return;
// 	}
// 
// 	int nAddValue = __min(m_kCriminalConst.nKillCrimerChivalry, m_kCriminalConst.nMaxChivalry - nCurChivalry);
// 	CapitalModule::m_pCapitalModule->IncCapital(pKernel, pPlayerObj->GetObjectId(), CAPITAL_CHIVALRY, nAddValue, FUNCTION_EVENT_ID_CRIME_SYSTEM);
// 	pPlayerObj->SetInt(FIELD_PROP_KILL_CRIMER_CHIVALRY, nCurChivalry + nAddValue);
// }

// ɱ������õ�����ֵ����
// int CriminalSystem::ResetAddChivalry(IKernel *pKernel, const PERSISTID &self, int slice)
// {
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (NULL == pSelfObj)
// 	{
// 		return 0;
// 	}
// 
// 	pSelfObj->SetInt(FIELD_PROP_KILL_CRIMER_CHIVALRY, 0);
// 	return 0;
// }