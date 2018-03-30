//----------------------------------------------------------
// �ļ���:      PassiveSkillUpgrade.h
// ��  ��:      ������������ϵͳ
// ˵  ��:		
// ��������:    2015��6��24��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------

#include "PassiveSkillSystem.h"
#include "..\Define\ClientCustomDefine.h"
#include "utils\XmlFile.h"
#include "utils\extend_func.h"
#include "..\Define\LogDefine.h"
#include "Impl\SkillDataQueryModule.h"
#include "..\CommonModule\LogModule.h"
#include "utils\string_util.h"
#include "..\SystemFunctionModule\CapitalModule.h"
#include "..\CommonModule\ContainerModule.h"
#ifndef FSROOMLOGIC_EXPORTS
#include "..\SocialSystemModule\SystemMailModule.h"
#endif
#include "..\Define\CommandDefine.h"
#include "..\CommonModule\LuaExtModule.h"
#include "utils\custom_func.h"
//#include "..\Define\SkillUpgradeSysInfoDefine.h"
#include "..\CommonModule\PropRefreshModule.h"
#include "..\Define\ModifyPackDefine.h"
#include "..\Interface\FightInterface.h"
#include "FsGame\CommonModule\SwitchManagerModule.h"
#include "..\Define\Skilldefine.h"
#include "..\Define\ServerCustomDefine.h"
#include "PassiveSkillModifyPack.h"
//#include "SkillUpgradeModule.h"
#include "FsGame/Define/Fields.h"
#include "FsGame/CommonModule\FunctionEventModule.h"
//#include "Define\BattleAbilityDefine.h"

SkillDataQueryModule*	PassiveSkillSystem::m_pSkillDataQueryModule	= NULL;
ContainerModule*		PassiveSkillSystem::m_pContainerModule			= NULL;
CapitalModule*			PassiveSkillSystem::m_pCapitalModule			= NULL;

#ifndef FSROOMLOGIC_EXPORTS
SystemMailModule*		PassiveSkillSystem::m_pSystemMailModule		= NULL;
#endif // FSROOMLOGIC_EXPORTS

int nx_passive_skill_level_up(void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ����������
	CHECK_ARG_NUM(state, nx_passive_skill_level_up, 3);
	// ����������
	CHECK_ARG_OBJECT(state, nx_passive_skill_level_up, 1);
	CHECK_ARG_INT(state, nx_passive_skill_level_up, 2);
	CHECK_ARG_INT(state, nx_passive_skill_level_up, 3);

	// ��ȡ����
	PERSISTID self = pKernel->LuaToObject(state, 1);
	int nSkill = pKernel->LuaToInt(state, 2);
	int nSkillLv = pKernel->LuaToInt(state, 3);

	PassiveSkillSystemSingleton::Instance()->UpdatePassiveSkill(pKernel, self, nSkill, nSkillLv, PSS_BY_SOUL);
	//PassiveSkillSystem::OnCustomOnPassiveSkillUpgrade(pKernel, self, PERSISTID(), CVarList() << CLIENT_CUSTOMMSG_PASSIVE_SKILL_UPGRADE << nSkill);
	return 0;
}

int nx_passive_skill_reset(void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ����������
	CHECK_ARG_NUM(state, nx_passive_skill_reset, 1);
	// ����������
	CHECK_ARG_OBJECT(state, nx_passive_skill_reset, 1);

	// ��ȡ����
	PERSISTID self = pKernel->LuaToObject(state, 1);

	PassiveSkillSystem::OnCustomResetPassiveSkill(pKernel, self, PERSISTID(), CVarList() << CLIENT_CUSTOMMSG_RESET_PASSIVE_SKILL);
	return 0;
}

int nx_learn_all_passive_skill(void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ����������
	CHECK_ARG_NUM(state, learn_all_passive_skill, 1);
	// ����������
	CHECK_ARG_OBJECT(state, learn_all_passive_skill, 1);

	// ��ȡ����
	PERSISTID self = pKernel->LuaToObject(state, 1);

	PassiveSkillSystemSingleton::Instance()->LearnAllPassiveSkill(pKernel, self);
	return 0;
}

// ��ʼ��
bool PassiveSkillSystem::Init(IKernel* pKernel)
{
	pKernel->AddEventCallback("player", "OnRecover", PassiveSkillSystem::OnPlayerRecover, 100);

	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_PASSIVE_SKILL_UPGRADE, PassiveSkillSystem::OnCustomOnPassiveSkillUpgrade);
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_RESET_PASSIVE_SKILL, PassiveSkillSystem::OnCustomResetPassiveSkill);

	m_pSkillDataQueryModule = (SkillDataQueryModule*)pKernel->GetLogicModule("SkillDataQueryModule");
	m_pContainerModule		= (ContainerModule*)pKernel->GetLogicModule("ContainerModule");
	m_pCapitalModule		= (CapitalModule*)pKernel->GetLogicModule("CapitalModule");
	
	Assert(NULL != m_pSkillDataQueryModule && NULL != m_pContainerModule && NULL != m_pCapitalModule);

#ifndef FSROOMLOGIC_EXPORTS
	m_pSystemMailModule		= (SystemMailModule*)pKernel->GetLogicModule("SystemMailModule");
	Assert(NULL != m_pSystemMailModule);
#endif // FSROOMLOGIC_EXPORTS

	// �����������԰��߼�
	if(!PassiveSkillModifyPackSingleton::Instance()->Init(pKernel))
	{
		return false;
	}

	// gm����
	DECL_LUA_EXT(nx_passive_skill_level_up);
	DECL_LUA_EXT(nx_passive_skill_reset);
	DECL_LUA_EXT(nx_learn_all_passive_skill);
	return true;
}

/*!
 * @brief	���ӻ���±������� ���ﱻ�������õ�
 * @param	self �����(�����ǳ���Ҳ���������)
 * @param	nSkillId ����id
 * @param	nSkillLevel ���ܵȼ�
 * @param	nSkillSrc  ������Դ
 * @return	bool
 */
bool PassiveSkillSystem::UpdatePassiveSkill(IKernel* pKernel, const PERSISTID &self, int nSkillId, int nSkillLevel, int nSkillSrc)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
	// �ҵ��������ܱ�
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return false;
	}
	// �Ƿ��������������
	int nRows = pPassiveSkillRec->FindInt(PS_COL_SKILL_ID, nSkillId);
	// û�еĻ�
	if (-1 == nRows)
	{
		// �����µı�������
		pPassiveSkillRec->AddRowValue(nRows, CVarList() << nSkillId << nSkillLevel << nSkillSrc);
	}
	else
	{
		// ˢ�±������ܵȼ�
		pPassiveSkillRec->SetInt(nRows, PS_COL_SKILL_LEVEL, nSkillLevel);
	}

	const PassiveSkillLevelInfo* pSkillInfo = m_pSkillDataQueryModule->GetPassiveSkillLevelInfo(nSkillId, nSkillLevel);
	if (NULL == pSkillInfo)
	{
		return false;
	}

	OnPassiveSkillChange(pKernel, self, nSkillId, pSkillInfo);
	return true;
}

/*!
 * @brief	�Ƴ���������
 * @param	self �����(�����ǳ���Ҳ���������)
 * @param	nSkillId����id
 * @return	bool
 */
bool PassiveSkillSystem::RemovePassiveSkill(IKernel* pKernel, const PERSISTID &self, int nSkillId)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
	// �ҵ��������ܱ�
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return false;
	}
	// �Ƿ��������������
	int nRows = pPassiveSkillRec->FindInt(PS_COL_SKILL_ID, nSkillId);
	// û�еĻ�
	if (-1 == nRows)
	{
		return false;
	}
	int nSkillLevel = pPassiveSkillRec->QueryInt(nRows, PS_COL_SKILL_LEVEL);
	int nSkillSrc = pPassiveSkillRec->QueryInt(nRows, PS_COL_SKILL_SRC);
	// ������
	if (TYPE_PLAYER  == pSelfObj->GetClassType() && PSS_BY_STUDY == nSkillSrc)
	{
		return false;
	}
	pPassiveSkillRec->RemoveRow(nRows);
	OnRemovePassiveSkill(pKernel, self, nSkillId, nSkillLevel);
	return true;
}
// ���ұ������ܵȼ�,û�ҵ�����0
int PassiveSkillSystem::QueryPassiveSkillLevel(IKernel* pKernel, const PERSISTID &self, int nSkillId)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}
	// �ҵ��������ܱ�
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return 0;
	}
	// û�ҵ�����
	int nRows = pPassiveSkillRec->FindInt(PS_COL_SKILL_ID, nSkillId);
	if (-1 == nRows)
	{ 
		return 0;
	}

	return pPassiveSkillRec->QueryInt(nRows, PS_COL_SKILL_LEVEL);
}

// ��Ӧ��������������Ϣ(����־)
int PassiveSkillSystem::OnCustomOnPassiveSkillUpgrade(IKernel* pKernel, const PERSISTID &self,
	const PERSISTID & sender, const IVarList & args)
{
	if (args.GetCount() != 2 || args.GetType(0) != VTYPE_INT || args.GetType(1) != VTYPE_INT)
	{
		return 0;
	}

#ifndef FSROOMLOGIC_EXPORTS
	//�������ܿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_PASSIVE_SKILL, self))
	{
		return 0;
	}
#endif // FSROOMLOGIC_EXPORTS

	// �����ļ���id
	int nSkillId = args.IntVal(1);
	
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}
	int nSelfJob = pSelfObj->QueryInt("Job");
	const PassiveSkill* pSkillData = m_pSkillDataQueryModule->GetPassiveSkillConfig(nSkillId);
	// ������
	if (NULL == pSkillData || pSkillData->nCareer != nSelfJob)
	{
		return 0;
	}

	// �ҵ��������ܱ�
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return 0;
	}

	// �Ƿ�ѧ���ı�������
	int nRows = pPassiveSkillRec->FindInt(PS_COL_SKILL_ID, nSkillId);

	// �������ļ��ܼ���
	int nRequestLevel = nRows != -1 ? pPassiveSkillRec->QueryInt(nRows, PS_COL_SKILL_LEVEL) + 1 : 1;

	// ��ȡ��ǰ���ܵ���ߵȼ�
// 	int nMaxLevel = (int)pSkillData->vUpgradeInfoList.size();
// 	if (nRequestLevel > nMaxLevel)
// 	{
// 		::CustomSysInfo(pKernel, pSelfObj->GetObjectId(), TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_PS_UPGRADE_FAILED_MAX_LEVEL, CVarList());
// 		return 0;
// 	}
	// ��ȡ��ǰ���ܵȼ�������
	const PassiveSkillLevelInfo* pSkillInfo = m_pSkillDataQueryModule->GetPassiveSkillLevelInfo(nSkillId, nRequestLevel);
	if (NULL == pSkillInfo)
	{
		return 0;
	}

	// ��鱻������������ǰ������
	if (!PassiveSkillSystemSingleton::Instance()->CheckUpgradeTerm(pKernel, pSelfObj, pSkillData, pSkillInfo, nRequestLevel))
	{
		return 0;
	}
	
	// ����Ƿ�������������
	if (!PassiveSkillSystemSingleton::Instance()->UpgradeCostPlayerAssets(pKernel, pSelfObj, pSkillInfo))
	{
		return 0;
	}
	if (-1 == nRows)
	{
		// �����µı�������
		pPassiveSkillRec->AddRowValue(nRows, CVarList() << nSkillId << nRequestLevel << PSS_BY_STUDY);
	}
	else
	{
		// �����������ܵȼ�
		pPassiveSkillRec->SetInt(nRows, PS_COL_SKILL_LEVEL, nRequestLevel);
	}
	
#ifndef FSROOMLOGIC_EXPORTS
	// ��¼��������������־
	RoleUpgradeLog log;

	log.type           = LOG_ROLE_UPGRADE_PASSIVE_SKILL;
	log.levelBefore    = nRequestLevel - 1;
	log.levelAfter     = nRequestLevel;
	std::string strSkilId	   = StringUtil::IntAsString(pSkillData->nSkillId);
	log.skillID		   = strSkilId.c_str();
	LogModule::m_pLogModule->SaveRoleUpgradeLog(pKernel, self, log);

	// ֪ͨ�ɾ�ģ����±������������ɾ�
// 	AchievementModule::m_pAchievementModule->UpdateSelfCheckAchievement(pKernel, self, 
// 		SUBTYPE_GROW_SKILL_PASSIVE_LEVEL, CVarList());

#endif

	// ��Ӧ�������ܵȼ������ı仯
	PassiveSkillSystemSingleton::Instance()->OnPassiveSkillChange(pKernel, self, pSkillData->nSkillId, pSkillInfo);

	// ս��������
// 	pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PROPERTY_TYPE);
// 	pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PASSIVE_SKILL_TYPE);

	return 0;
}

// ��Ӧ��������������Ϣ(����־)
int PassiveSkillSystem::OnCustomResetPassiveSkill(IKernel* pKernel, const PERSISTID &self,
	const PERSISTID & sender, const IVarList & args)
{
	if (args.GetCount() != 1 || args.GetType(0) != VTYPE_INT)
	{
		return 0;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

#ifndef FSROOMLOGIC_EXPORTS
	//�������ܿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_PASSIVE_SKILL, self))
	{
		return 0;
	}
#endif // FSROOMLOGIC_EXPORTS

	// ����Ƿ���Ҫ���ñ�������
// 	bool bCheckIsCanReset = PassiveSkillSystemSingleton::Instance()->IsHavePassiveSkill(pKernel, self);
// 	if (!bCheckIsCanReset)
// 	{
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_PS_RESET_FAILED_NONE_PS, CVarList());
// 		return false;
// 	}

	// ��鼼�������Ƿ���������
	if (!PassiveSkillSystemSingleton::Instance()->ResetCostItem(pKernel, self))
	{
		return 0;
	}

	// ִ�б����������ò���
	PassiveSkillSystemSingleton::Instance()->ExecuteResetPassiveSkill(pKernel, self);

	// ս��������
// 	pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PROPERTY_TYPE);
// 	pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PASSIVE_SKILL_TYPE);
	//::CustomSysInfo(pKernel, pSelfObj->GetObjectId(), TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_PS_RESET_SUCCESS, CVarList() << kTotalMoney.nMoneyNum << nTotalPoint);
	return 0;
}

//�ص����� ��Ҽ����������
int PassiveSkillSystem::OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
						   const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ս���������ٴμ������� �������Ե���
#ifndef FSROOMLOGIC_EXPORTS 
	PassiveSkillSystemSingleton::Instance()->AddAllPassiveSkillPro(pKernel, pSelfObj);
#endif // FSROOMLOGIC_EXPORTS
	return 0;
}
// ��鼼�ܵ���������
bool PassiveSkillSystem::CheckUpgradeTerm(IKernel* pKernel, IGameObj* pSelfObj, const PassiveSkill* pMainInfo, const PassiveSkillLevelInfo* pSkillInfo, const int nRequestSkillLevel)
{
	if (NULL == pSelfObj || NULL == pSkillInfo || NULL == pMainInfo)
	{
		return false;
	}

	bool bLevelUp = true;
	do 
	{
		// �ҵ��������ܱ�
		IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
		if (NULL == pPassiveSkillRec)
		{
			bLevelUp = false;
			break;
		}

		// �ҵ���ǰ�ü���
		int nFindRow = pPassiveSkillRec->FindInt(PS_COL_SKILL_ID, pSkillInfo->nPreSkillId);
		if (-1 == nFindRow)
		{
			bLevelUp = false;
			break;
		}

		// ͳ����ͬ���͵����ĵļ��ܵ�
		int nRows = pPassiveSkillRec->GetRows();
		int nTotalCost = 0;
		LoopBeginCheck(a);
		for (int i = 0; i < nRows; ++i)
		{
			LoopDoCheck(a);
			int nSkillId = pPassiveSkillRec->QueryInt(i, PS_COL_SKILL_ID);
			int nSkillLevel = pPassiveSkillRec->QueryInt(i, PS_COL_SKILL_LEVEL);
			int nSkillSrc = pPassiveSkillRec->QueryInt(i, PS_COL_SKILL_SRC);
			if (nSkillSrc != PSS_BY_STUDY)
			{
				continue;
			}
			const PassiveSkill* pSkillData = m_pSkillDataQueryModule->GetPassiveSkillConfig(nSkillId);
			if (NULL == pSkillData)
			{
				continue;;
			}
			if (pSkillData->nType != pMainInfo->nType)
			{
				continue;
			}

			int nCostSkillPoint = StatOneSkillCostPoint(nSkillId, nSkillLevel, pSkillData);
			nTotalCost += nCostSkillPoint;
		}
	} while (0);
	
	return bLevelUp;
}

// �۳���ҲƲ�(Ǯ,����,���ܵ�)
bool PassiveSkillSystem::UpgradeCostPlayerAssets(IKernel* pKernel, IGameObj* pSelfObj, const PassiveSkillLevelInfo* pSkillInfo)
{
	if (NULL == pSelfObj || NULL == pSkillInfo)
	{
		return false;
	}

	// ������Ϊ��,˵����������Ҫ����
	bool bNeedItem = !pSkillInfo->strItemId.empty();

	// ���������Ƿ�Ϊ��
	PERSISTID toolBox = pKernel->GetChild(pSelfObj->GetObjectId(), ITEM_BOX_NAME_WSTR);
	if (!pKernel->Exists(toolBox))
	{
		return false;
	}

	// ���Ǯ�����ܵ㡢���Ϲ�����
// 	if(!m_pCapitalModule->CanDecCapital(pKernel, pSelfObj->GetObjectId(), pSkillInfo->nMoneyType, pSkillInfo->nMoneyCost))
// 	{
// 		::CustomSysInfo(pKernel, pSelfObj->GetObjectId(), TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_PS_UPGRADE_FAILED_GOLD, CVarList());
// 		return false;
// 	}
// 
// 	int nCurSkillPoint = pSelfObj->QueryInt(SKILL_POINT_NAME);
// 	if(nCurSkillPoint < pSkillInfo->nSkillPoint)
// 	{
// 		::CustomSysInfo(pKernel, pSelfObj->GetObjectId(), TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_PS_UPGRADE_FAILED_SKILL_POINT, CVarList());
// 		return false;
// 	}
// 	if(bNeedItem && !m_pContainerModule->TryRemoveItems(pKernel, toolBox, pSkillInfo->strItemId.c_str(), pSkillInfo->nItemCost))
// 	{
// 		::CustomSysInfo(pKernel, pSelfObj->GetObjectId(), TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_PS_UPGRADE_FAILED_ITEM, CVarList());
// 		return false;
// 	}

	// ��ʼ��Ǯ�����ܵ㡢����
	m_pCapitalModule->DecCapital(pKernel, pSelfObj->GetObjectId(), pSkillInfo->nMoneyType, pSkillInfo->nMoneyCost, FUNCTION_EVENT_ID_UPGRADE_PASSIVE_SKILL);
	
//	pSelfObj->SetInt(SKILL_POINT_NAME, nCurSkillPoint - pSkillInfo->nSkillPoint);
	
	// ����Ҫ�۵���ֱ�ӷ���
	if (!bNeedItem)
	{
		return true;
	}

	//�е��ߣ��۵���
	if (FunctionEventModule::GetItemBindPriorUse(FUNCTION_EVENT_ID_UPGRADE_PASSIVE_SKILL))
	{
		m_pContainerModule->RemoveItemsBindPrior(pKernel, toolBox, pSkillInfo->strItemId.c_str(), pSkillInfo->nItemCost, FUNCTION_EVENT_ID_UPGRADE_PASSIVE_SKILL, true);
	}
	else
	{
		m_pContainerModule->RemoveItems(pKernel, toolBox, pSkillInfo->strItemId.c_str(), pSkillInfo->nItemCost, FUNCTION_EVENT_ID_UPGRADE_PASSIVE_SKILL, true);
	}
	return true;
}

// ��Ӧ����������ı仯
void PassiveSkillSystem::OnPassiveSkillChange(IKernel* pKernel, const PERSISTID &self, int nSkillId, const PassiveSkillLevelInfo* pSkillInfo)
{
	if (NULL == pSkillInfo)
	{
		return;
	}
	const PSPropInfoVec& propvec = pSkillInfo->vPropInfoList;
	int nSize = (int)propvec.size();
	CVarList playerprops;
	LoopBeginCheck(d)
	for (int i = 0;i < nSize;++i)
	{
		LoopDoCheck(d)
		const PassiveSkillPropInfo& data = propvec[i];
		// ͳ�����иı����������
		if (PS_ADD_PLAYER_PROPERTY == data.nOptType)
		{
			playerprops << data.strPropName;
			playerprops << data.uPropAdd.fPropAdd;
		}
		else if (PS_ADD_SKILL_PROPERTY == data.nOptType) // �ı似������
		{
			PERSISTID skill = FightInterfaceInstance->FindSkill(pKernel, self, data.strSkillID.c_str());
			m_pSkillDataQueryModule->OnSkillPropChange(pKernel, skill, self);
		}
		else if (PS_CHANGE_SKILL_CONFIG == data.nOptType)// Ӱ�켼������
		{
			// �ټ����µ�����
			CVarList msg;
			msg << COMMAND_PASSIVE_SKILL_AFFECT_SKILL_CONFIG << 1 << data.strSkillID.c_str() << data.strPropName.c_str();
			pKernel->Command(self, self, msg);
		}
		else if (PS_ADD_NEW_BUFF == data.nOptType) // ������buff
		{
			FightInterfaceInstance->AddBuffer(pKernel, self, self, data.strSkillID.c_str());
		}
	}
	
	// ��Ӱ���������Ըı��
	if (playerprops.GetCount() > 0)
	{
		// ���������
		pKernel->Command(self, self, CVarList() << COMMAND_PASSIVE_SKILL_REMOVE_PROP << nSkillId);	
		// �ټ����µ�����
		CVarList msg;
		msg << COMMAND_PASSIVE_SKILL_ADD_PROP << nSkillId << (int)playerprops.GetCount() / 2 << playerprops;
		pKernel->Command(self, self, msg);
	}
}

// �黹ĳ��������Ǯ�����ܵ㡢����
void PassiveSkillSystem::GiveBackOneSkillAssets(IKernel* pKernel, int& outSkillPoint, int nSkillId, int nSkillLevel)
{
	const PassiveSkill* pSkill = m_pSkillDataQueryModule->GetPassiveSkillConfig(nSkillId);
	if (NULL == pSkill)
	{
		return;
	}

	int nTotalSkillPoint = StatOneSkillCostPoint(nSkillId, nSkillLevel, pSkill);

	// ��¼���ܵ�
	outSkillPoint += nTotalSkillPoint;
}

// ��ȡ���ܵ�����
bool PassiveSkillSystem::LoadSkillPointAddConfig(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += "ini/Skill/passive_skill_point_config.xml";

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	m_vecSkillPointRec.clear();

	const int iSectionCount = (int)xmlfile.GetSectionCount();
	m_vecSkillPointRec.reserve(iSectionCount);

	LoopBeginCheck(f)
	for (int i = 0; i < iSectionCount; i++)
	{
		LoopDoCheck(f)
		const char* section = xmlfile.GetSectionByIndex(i);

		SkillPoint data;
		data.nMeridianLevel	= xmlfile.ReadInteger(section, "Level", 1);
		data.nAddSkillPoint	= xmlfile.ReadInteger(section, "SkillPointAdd", 1);
			
		m_vecSkillPointRec.push_back(data);
	}
	return true;
}

// ��ѯ��Ӧ�ȼ����ӵļ��ܵ�
int PassiveSkillSystem::QueryAddSkillPoint(int nPlayerLevel)
{
	int nAddPoint = 0;
	int nSize = (int)m_vecSkillPointRec.size();
	LoopBeginCheck(h)
	for (int i = 0;i < nSize;++i)
	{
		LoopDoCheck(h)
		const SkillPoint& data = m_vecSkillPointRec[i];
		if (data.nMeridianLevel == nPlayerLevel)
		{
			nAddPoint = data.nAddSkillPoint;
			break;
		}
	}

	return nAddPoint;
}

// ����������Ʒ��鼰�۳�
bool PassiveSkillSystem::ResetCostItem(IKernel* pKernel, const PERSISTID &self)
{
	// ���������Ƿ�Ϊ��
	PERSISTID toolBox = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
	if (!pKernel->Exists(toolBox))
	{
		return false;
	}

// 	if(!m_pContainerModule->TryRemoveItems(pKernel, toolBox, m_kResetCost.strItemId.c_str(), m_kResetCost.nItemNum))
// 	{
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, SYS_INFO_PS_RESET_FAILED_ITEM, CVarList());
// 		return false;
// 	}

	//�е��ߣ��۵���
// 	if (FunctionEventModule::GetItemBindPriorUse(FUNCTION_EVENT_ID_RESET_PASSIVE_SKILL))
// 	{
// 		m_pContainerModule->RemoveItemsBindPrior(pKernel, toolBox, m_kResetCost.strItemId.c_str(), m_kResetCost.nItemNum, FUNCTION_EVENT_ID_RESET_PASSIVE_SKILL, true);
// 	}
// 	else
// 	{
// 		m_pContainerModule->RemoveItems(pKernel, toolBox, m_kResetCost.strItemId.c_str(), m_kResetCost.nItemNum, FUNCTION_EVENT_ID_RESET_PASSIVE_SKILL, true);
// 	}
	return true;
}

// ɾ���������ܵĴ���
bool PassiveSkillSystem::OnRemovePassiveSkill(IKernel* pKernel, const PERSISTID &self, int nSkillId, int nSkillLevel)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}
	const PassiveSkillLevelInfo* pSkillInfo = m_pSkillDataQueryModule->GetPassiveSkillLevelInfo(nSkillId, nSkillLevel);
	if (NULL == pSkillInfo)
	{
		return false;
	}
 
	const PSPropInfoVec& propvec = pSkillInfo->vPropInfoList;
	int nSize = (int)propvec.size();
	CVarList prop;
	// �����Ƿ��иı����������
	LoopBeginCheck(i)
	for (int i = 0;i < nSize;++i)
	{
		LoopDoCheck(i)
		const PassiveSkillPropInfo& data = propvec[i];

		// ͳ�����иı����������
		if (PS_ADD_PLAYER_PROPERTY == data.nOptType)
		{
			// �������
			pKernel->Command(self, self, CVarList() << COMMAND_PASSIVE_SKILL_REMOVE_PROP << nSkillId);
		}
		else if (PS_ADD_SKILL_PROPERTY == data.nOptType) // �ı似������
		{
			PERSISTID skill = FightInterfaceInstance->FindSkill(pKernel, self, data.strSkillID.c_str());
			m_pSkillDataQueryModule->OnSkillPropChange(pKernel, skill, self);
		}
		else if (PS_CHANGE_SKILL_CONFIG == data.nOptType)// Ӱ�켼������
		{
			// �ټ����µ�����
			CVarList msg;
			msg << COMMAND_PASSIVE_SKILL_AFFECT_SKILL_CONFIG << 0 << data.strSkillID.c_str() << data.strPropName.c_str();
			pKernel->Command(self, self, msg);
		}
		else if (PS_ADD_NEW_BUFF == data.nOptType) // ɾ��buff
		{
			FightInterfaceInstance->RemoveBuffer(pKernel, self, data.strSkillID.c_str());
		}
	}
	return true;
}

// ��ӱ�������Ӱ�������
void PassiveSkillSystem::AddAllPassiveSkillPro(IKernel* pKernel, IGameObj* pSelfObj)
{
	if (NULL == pSelfObj)
	{
		return;
	}
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return;
	}
	int nRows = pPassiveSkillRec->GetRows();
	LoopBeginCheck(c)
	for (int i = 0;i < nRows;++i)
	{
		LoopDoCheck(c)
		int nSkillId = pPassiveSkillRec->QueryInt(i, PS_COL_SKILL_ID);
		int nSkillLevel = pPassiveSkillRec->QueryInt(i, PS_COL_SKILL_LEVEL);

		// ��ȡ��ǰ���ܵȼ�������
		const PassiveSkillLevelInfo* pSkillInfo = m_pSkillDataQueryModule->GetPassiveSkillLevelInfo(nSkillId, nSkillLevel);
		if (NULL == pSkillInfo)
		{
			continue;
		}
		PassiveSkillSystemSingleton::Instance()->OnPassiveSkillChange(pKernel, pSelfObj->GetObjectId(), nSkillId, pSkillInfo);
	}
}

// ִ�б�����������
void PassiveSkillSystem::ExecuteResetPassiveSkill(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}
	// �ҵ��������ܱ�
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return;
	}

	int nRecCount = pPassiveSkillRec->GetRows();

	// ��¼�����ļ��ܵ�
	int nTotalPoint = 0;

	// ��¼��ǰ���б������ܵ�״̬
	std::string strInfo = "";

	// ͳ�Ʒ���������ʲ�
	LoopBeginCheck(b)
	for (int i = 0;i < nRecCount;++i)
	{
		LoopDoCheck(b)
		int nSkillId	= pPassiveSkillRec->QueryInt(i, PS_COL_SKILL_ID);
		int nSkillLevel = pPassiveSkillRec->QueryInt(i, PS_COL_SKILL_LEVEL);
		int nSkillSrc = pPassiveSkillRec->QueryInt(i, PS_COL_SKILL_SRC);
		// ��������;����õı�������
		if (PSS_BY_STUDY != nSkillSrc)
		{
			continue;
		}
		// �˼���ûѧ
		if (0 == nSkillLevel)
		{
			continue;
		}
		// ����һ�����ܵ������ʲ�
		PassiveSkillSystemSingleton::Instance()->GiveBackOneSkillAssets(pKernel, nTotalPoint, nSkillId, nSkillLevel);

		// ��¼��ǰ����״̬
		strInfo.append(StringUtil::IntAsString(nSkillId));
		strInfo.append(",");
		strInfo.append(StringUtil::IntAsString(nSkillLevel));
		strInfo.append(";");

		// ��ռ��ܵȼ�
		pPassiveSkillRec->SetInt(i, PS_COL_SKILL_LEVEL, 0);

		// ���������Ƴ�����
		PassiveSkillSystemSingleton::Instance()->OnRemovePassiveSkill(pKernel, self, nSkillId, nSkillLevel);
	}

#ifndef FSROOMLOGIC_EXPORTS
	strInfo = strInfo + std::string("Add SkillPoint:") + StringUtil::IntAsString(nTotalPoint);

	// ��¼��־
// 	LogModule::m_pLogModule->SavePlayerLog(pKernel, self, LOG_PLAYER_RESET_PS,
// 		util_string_as_widestr(strInfo.c_str()).c_str());
#endif // FSROOMLOGIC_EXPORTS

	// �������ܵ�
	int nOldSkillPoint = pSelfObj->QueryInt(SKILL_POINT_NAME);

	// ���ܵ����ޱ���
// 	int nMaxSkillPoint = PassiveSkillSystemSingleton::Instance()->ComputeCurLevelMaxSkillPoint(pKernel, pSelfObj);
// 	int nGiveBackPoint = __min(nOldSkillPoint + nTotalPoint, nMaxSkillPoint);

	//pSelfObj->SetInt(SKILL_POINT_NAME, nGiveBackPoint);
}

// ����Ƿ���ѧϰ�ı�������
bool PassiveSkillSystem::IsHavePassiveSkill(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// �ҵ��������ܱ�
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return 0;
	}
	int nRecCount = pPassiveSkillRec->GetRows();

	// ����Ƿ���Ҫ���ñ�������
	bool bCheckIsCanReset = false;
	LoopBeginCheck(a)
	for (int i = 0;i < nRecCount;++i)
	{
		LoopDoCheck(a)
			int nSkillSrc = pPassiveSkillRec->QueryInt(i, PS_COL_SKILL_SRC);
		// ���Գ���ӵı�������
		if (PSS_BY_PET == nSkillSrc)
		{
			continue;
		}
		int nSkillLevel = pPassiveSkillRec->QueryInt(i, PS_COL_SKILL_LEVEL);
		if (nSkillLevel > 0)
		{
			bCheckIsCanReset = true;
			break;
		}
	}

	return bCheckIsCanReset;
}

// ͳ��ĳ���������ĵļ��ܵ�
int PassiveSkillSystem::StatOneSkillCostPoint(int nSkillId, int nSkillLevel, const PassiveSkill* pMainInfo)
{
	if (NULL == pMainInfo || nSkillId <= 0 || nSkillLevel <= 0)
	{
		return 0;
	}
	// ������
	const PSLevelInfoVec& vPSLevelInfo = pMainInfo->vUpgradeInfoList;
	int nSize = (int)vPSLevelInfo.size();
	if (nSize < nSkillLevel)
	{
		return 0;
	}
	int nTotalCost = 0;
	LoopBeginCheck(r);
	for (int i = 0;i < nSkillLevel;++i)
	{
		LoopDoCheck(r);
		const PassiveSkillLevelInfo& kInfo = vPSLevelInfo[i];
		nTotalCost += kInfo.nSkillPoint;
	}

	return nTotalCost;
}

// ѧϰ���б�������(���ԡ�����ʹ��)
void PassiveSkillSystem::LearnAllPassiveSkill(IKernel* pKernel, const PERSISTID &self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return;
	}
	pPassiveSkillRec->ClearRow();
	// ��ѯ������ѧ�ı�������
	CVarList skills;
	m_pSkillDataQueryModule->QueryPassiveSkillIdByJob(pKernel, skills, self);

	// ѧϰ��������
	int nCount = (int)skills.GetCount();
	LoopBeginCheck(e)
	for (int i = 0;i < nCount;++i)
	{
		LoopDoCheck(e)
		int nSkillId = skills.IntVal(i);
		// �����µı�������
		pPassiveSkillRec->AddRowValue(-1, CVarList() << nSkillId << 1 << PSS_BY_STUDY);

		// ��ȡ��ǰ���ܵȼ�������
		const PassiveSkillLevelInfo* pSkillInfo = m_pSkillDataQueryModule->GetPassiveSkillLevelInfo(nSkillId, 1);
		if (NULL == pSkillInfo)
		{
			return;
		}
		
		// ��Ӧ�������ܱ仯
		OnPassiveSkillChange(pKernel, self, nSkillId, pSkillInfo);
	}

	// ս��������
// 	pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PROPERTY_TYPE);
// 	pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PASSIVE_SKILL_TYPE);
}