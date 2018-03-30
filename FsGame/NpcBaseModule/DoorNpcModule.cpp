//--------------------------------------------------------------------
// �ļ���:      DoorNpcModule.h
// ��  ��:		������NPCģ��
// ˵  ��:		
// ��������:    2014��12��6��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#include "DoorNpcModule.h"
#include "utils/util_func.h"
#include "FsGame/CommonModule/LuaScriptModule.h"
#include "utils/XmlFile.h"
#include "FsGame/CommonModule/AsynCtrlModule.h"
#include "utils/exptree.h"
#include "../Define/SnsDefine.h"
#include "../Define/CommandDefine.h"
#include "CommonModule/LandPosModule.h"
#include "Define/Fields.h"

AsynCtrlModule * DoorNpcModule::m_pAsynCtrlModule = NULL;
// ��ʼ��
bool DoorNpcModule::Init(IKernel* pKernel)
{
	m_pAsynCtrlModule = (AsynCtrlModule*)(pKernel->GetLogicModule("AsynCtrlModule"));
	Assert( m_pAsynCtrlModule != NULL );

	pKernel->AddEventCallback("DoorNpc","OnSpring", OnSpring);
	pKernel->AddEventCallback("DoorNpc", "OnEndSpring", OnEndSpring);

	return true;
}

// �ͷ�
bool DoorNpcModule::Shut(IKernel* pKernel)
{
	return true;
}

// ����ص�
int DoorNpcModule::OnSpring(IKernel* pKernel, const PERSISTID& self, 
						  const PERSISTID& sender, const IVarList& args)
{
	//�鿴��Ӧ������
	if ( !pKernel->Exists(self) || !pKernel->Exists(sender) )
	{
		return 0;
	}

	// �����Ŷ���
	IGameObj * doorObj = pKernel->GetGameObj(self);
	if ( doorObj == NULL )
	{
		return 0;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(sender);
	if ( pPlayer == NULL )
	{
		return 0;
	}
	if (pPlayer->GetClassType() != TYPE_PLAYER)
	{
		return 0;
	}


	// ������Ҳ����� [4/8/2015 liumf]
	if (pPlayer->QueryInt("Online") == OFFLINE)
	{
		return 0;
	}

	//����������й��� ���жϴ����Ŵ���Ŀ����Ҹ���ǰ�����Ź����Ƿ���ͬ
	//int doorNation = doorObj->QueryInt(FIELD_PROP_NATION);
// 	if (doorNation == NATION_JIANGHU || doorNation == NATION_COURT)
// 	{
// 		int sendNation = pPlayer->QueryInt(FIELD_PROP_NATION);
// 		if (sendNation != doorNation)
// 		{
// 			return 0;
// 		}
// 	}



	// ������������CG����ʱ�رմ����� [2015.07.30 tongzt]
// 	if (QuestModule::m_pQuestModule->TriggerPlayCG(pKernel, sender))
// 	{
// 		// ��¼������Npc
// 		if(!pPlayer->FindData("QuestDoorNPC"))
// 		{
// 			pPlayer->AddDataObject("QuestDoorNPC", self);
// 		}
// 		else
// 		{
// 			pPlayer->SetDataObject("QuestDoorNPC", self);
// 		}
// 		return 0;
// 	}

	//����Զ�Ѱ·��
	if (pPlayer->FindData("AutoWalk"))
	{
		int nAutoWalk = pPlayer->QueryDataInt("AutoWalk");
		if (nAutoWalk == 1)
		{
			//�Զ�Ѱ·״̬ �� onSpring ���첽������ ������ʱ�����Ŷ��� 
			if(!pPlayer->FindData("InDoorNPC"))
			{
				pPlayer->AddDataObject("InDoorNPC", self);
			}
			else
			{
				pPlayer->SetDataObject("InDoorNPC", self);
			}
			return 0;
		}
		else
		{	//�Ƴ���ʱ����
			pPlayer->RemoveData("InDoorNPC");
			pPlayer->RemoveData("AutoWalk");
		}
	}

	//��ȡ��Ӧ������ź�����
	const char * iTranslateSceneID = doorObj->QueryString("TranslateSceneID");
	len_t fTranslatePosX = doorObj->QueryFloat("TranslatePosX");
	len_t fTranslatePosY = doorObj->QueryFloat("TranslatePosY");
	len_t fTranslatePosZ = doorObj->QueryFloat("TranslatePosZ");
	len_t fOrient		 = doorObj->QueryFloat("TranslateAy");

	//��ȡ����
	PERSISTID scene = pKernel->GetScene();
	//ת����Ӧ�ĳ�����
	ExpTree exp;
	int nNewScene = static_cast<int>(exp.CalculateEvent(pKernel, sender, scene, PERSISTID(), PERSISTID(), iTranslateSceneID));

	// �ܷ���
	if (!AsynCtrlModule::m_pAsynCtrlModule->CanSwitchLocate(pKernel, pPlayer->QueryWideStr("Name"), nNewScene))
	{
		return 0;
	}

	// ���ⳡ���ж�
// 	bool bInTrueasuryScene = false;
// 	if (!FrontierTreasuryModule::TestEntryTreasury(pKernel, pPlayer, nNewScene, bInTrueasuryScene))
// 	{
// 		if (bInTrueasuryScene)
// 		{
// 			return 0;
// 		}
// 	}
	int nSceneId_ = pKernel->GetSceneId();
// 	if (bInTrueasuryScene)
// 	{
// 		// ��¼���ص�
// 		float x_ = 0, y_ = 0, z_ = 0, o_ = 0;
// 		pKernel->GetSceneBorn(x_, y_, z_, o_);
// 		y_ = pKernel->GetMapHeight(x_, z_);
// 		LandPosModule::m_pLandPosModule->SetPlayerLandPosi(pKernel, sender, nSceneId_, x_, y_, z_);
// 	}

	if ( nNewScene == nSceneId_ )
	{
		pKernel->MoveTo( sender, fTranslatePosX, fTranslatePosY, fTranslatePosZ, fOrient);
	}
	else
	{
		m_pAsynCtrlModule->SwitchLocate( pKernel, sender, nNewScene, fTranslatePosX, 
			fTranslatePosY, fTranslatePosZ, fOrient );
	}
	return 0;
}

int DoorNpcModule::OnEndSpring(IKernel* pKernel, const PERSISTID& self, 
				const PERSISTID& sender, const IVarList& args)
{

	IGameObj *pPlayer = pKernel->GetGameObj(sender);
	if (NULL == pPlayer)
	{
		return 0;
	}

	// ������������ʱ����
	if(pPlayer->FindData("InDoorNPC"))
	{
		pPlayer->RemoveData("InDoorNPC");
	}

	return 0;
}