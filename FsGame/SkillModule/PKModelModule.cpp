//--------------------------------------------------------------------
// �ļ���:		PKModelModule.h
// ��  ��:		PKģʽ
// ˵  ��:		
//				
// ��������:	2017��07��24��
// ��������:	
// ������:		liumf   
//--------------------------------------------------------------------

#include "PKModelModule.h"
#include "FsGame/Define/PKModelDefine.h"
#include "Define/ClientCustomDefine.h"
#include "SceneAttackManage.h"
#include "CriminalSystem.h"
#include "Define/Fields.h"
#include "CommonModule/EnvirValueModule.h"
#include "CommonModule/ReLoadConfigModule.h"
#include "utils/XmlFile.h"
#include "utils/extend_func.h"
#include "utils/string_util.h"
#include "PVPRecAttacker.h"

PKModelModule* PKModelModule::m_pPKModelModule = NULL;

// ��ʼ��
bool PKModelModule::Init(IKernel* pKernel)
{
	m_pPKModelModule = this;
	
	pKernel->AddEventCallback("player", "OnRecover", OnPlayerRecover);
	pKernel->AddEventCallback("player", "OnReady", OnPlayerReady);

	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_PK_MODEL, PKModelModule::OnCustomPKModel);

	SceneAttackManage::Init(pKernel);
	CriminalSystemSingleton::Instance()->Init(pKernel);
	PVPRecAttackerSingleton::Instance()->Init(pKernel);

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("PKModelConfig", PKModelModule::ReloadConfig);
#endif
	return true;
}

// �ر�
bool PKModelModule::Shut(IKernel* pKernel)
{
	return true;
}

// �Ƿ�ɹ���
bool PKModelModule::IsBeAttacked(IKernel* pKernel, const PERSISTID &self, const PERSISTID &target)
{
	return SceneAttackManage::CheckTargetRelation(pKernel, self, target, TRT_ENEMY);
}

// �Ƿ�Ϊ�ѷ�
bool PKModelModule::IsFriendly(IKernel* pKernel, const PERSISTID &self, const PERSISTID &target)
{
	return SceneAttackManage::CheckTargetRelation(pKernel, self, target, TRT_FRIEND);
}

/// \brief ����
int PKModelModule::OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	//m_pPKModelModule->ResetDefaultPKModel(pKernel, self);

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ���ߺ�,Ĭ��PKģʽΪ��ƽ
	pSelfObj->SetInt(FIELD_PROP_PKMODEL, PT_PEACE);
	return 0;
}

int PKModelModule::OnPlayerReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	//m_pPKModelModule->SetCurPkModel(pKernel, self);
	return 0;
}

// ��Ӧ�ͻ�����Ϣ
int PKModelModule::OnCustomPKModel(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (args.GetCount() < 2 || args.GetType(0) != VTYPE_INT || args.GetType(1) != VTYPE_INT)
	{
		return 0;
	}

	int nSubMsg = args.IntVal(1);
	switch (nSubMsg)
	{
	case PK_C2S_CHANGE_PK_MODEL:
		m_pPKModelModule->OnCustomChangePKModel(pKernel, self, args);
		break;
	case PK_C2S_COST_MONEY_CLEAN_CRIME:
		CriminalSystemSingleton::Instance()->OnCustomCostMoneyCleanCrime(pKernel, self, sender, args);
		break;
// 	case PK_C2S_SET_DEFAULT_MODEL:
// 		m_pPKModelModule->OnCustomSetDefaultModel(pKernel, self, args);
// 		break;
	}
	return 0;
}

// ��ӦPKģʽ�л�
void PKModelModule::OnCustomChangePKModel(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	if (args.GetCount() < 3 || args.GetType(2) != VTYPE_INT)
	{
		return;
	}

	int nPKModel = args.IntVal(2);
	IGameObj* pSceneObj = pKernel->GetSceneObj();
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSceneObj || NULL == pSelfObj || nPKModel < PT_PEACE || nPKModel >= PT_MAX_NUM)
	{
		return;
	}

	// ���ڹ�������,���ܸ�pkģʽ
	if (pSceneObj->QueryInt(FIELD_PROP_ATTACK_TYPE) != SCENE_ATTACK_PUBLIC_SCENE)
	{
		return;
	}

	// ���ֱ���ʱ�����л�ģʽ
// 	int nSelfLv = pSelfObj->QueryInt(FIELD_PROP_LEVEL);
// 	int nPkLimitLv = EnvirValueModule::EnvirQueryInt(ENV_VALUE_PK_LIMIT_LEVEL);
// 	if (nSelfLv <= nPkLimitLv)
// 	{
// 		return;
// 	}

	int nOldPKModel = pSelfObj->QueryInt(FIELD_PROP_PKMODEL);
	if (nOldPKModel == nPKModel)
	{
		return;
	}

	pSelfObj->SetInt(FIELD_PROP_PKMODEL, nPKModel);
}

// ��Ӧ����Ĭ��pkģʽ
// void PKModelModule::OnCustomSetDefaultModel(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
// {
// 	if (args.GetCount() < 4 || args.GetType(2) != VTYPE_INT || args.GetType(3) != VTYPE_INT)
// 	{
// 		return;
// 	}
// 
// 	int nSceneId = args.IntVal(2);
// 	int nPKModel = args.IntVal(3);
// 
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (NULL == pSelfObj || nPKModel < PT_PEACE || nPKModel >= PT_MAX_NUM)
// 	{
// 		return;
// 	}
// 
// 	IRecord* pPKModelRec = pSelfObj->GetRecord(FIELD_RECORD_PK_MODEL_REC);
// 	if (NULL == pPKModelRec)
// 	{
// 		return;
// 	}
// 
// 	int nRowIndex = pPKModelRec->FindInt(COLUMN_PK_MODEL_REC_SCENE_ID, nSceneId);
// 	if (-1 == nRowIndex)
// 	{
// 		return;
// 	}
// 
// 	pPKModelRec->SetInt(nRowIndex, COLUMN_PK_MODEL_REC_PKMODEL, nPKModel);
// }

// ����Ĭ�ϵ�pkģʽ
// void PKModelModule::ResetDefaultPKModel(IKernel* pKernel, const PERSISTID& self)
// {
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (NULL == pSelfObj)
// 	{
// 		return;
// 	}
// 
// 	IRecord* pPKModelRec = pSelfObj->GetRecord(FIELD_RECORD_PK_MODEL_REC);
// 	if (NULL == pPKModelRec)
// 	{
// 		return;
// 	}
// 
// 	int nSceneNum = (int)m_vecScenePKModel.size();
// 	if (pPKModelRec->GetRows() < nSceneNum)
// 	{
// 		LoopBeginCheck(b);
// 		for (int i = 0; i < nSceneNum;++i)
// 		{
// 			LoopDoCheck(b);
// 			int nSceneId = m_vecScenePKModel[i].nSceneId;
// 			int nRowIndex = pPKModelRec->FindInt(COLUMN_PK_MODEL_REC_SCENE_ID, nSceneId);
// 			if (nRowIndex == -1)
// 			{
// 				pPKModelRec->AddRowValue(-1, CVarList() << nSceneId << m_vecScenePKModel[i].nDefaultModel);
// 			}
// 		}
// 	}
// }

// �г���������pkģʽ
// void PKModelModule::SetCurPkModel(IKernel* pKernel, const PERSISTID& self)
// {
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (NULL == pSelfObj)
// 	{
// 		return;
// 	}
// 
// 	IRecord* pPKModelRec = pSelfObj->GetRecord(FIELD_RECORD_PK_MODEL_REC);
// 	if (NULL == pPKModelRec)
// 	{
// 		return;
// 	}
// 	int nSceneId = pKernel->GetSceneId();
// 	int nRowIndex = pPKModelRec->FindInt(COLUMN_PK_MODEL_REC_SCENE_ID, nSceneId);
// 	if (nRowIndex != -1)
// 	{
// 		int nPKModel = pPKModelRec->QueryInt(nRowIndex, COLUMN_PK_MODEL_REC_PKMODEL);
// 		pSelfObj->SetInt(FIELD_PROP_PKMODEL, nPKModel);
// 	}
// }

// ���¼�������
void PKModelModule::ReloadConfig(IKernel* pKernel)
{
	CriminalSystemSingleton::Instance()->LoadResource(pKernel);
}