//--------------------------------------------------------------------
// �ļ���:		TrapNpc.cpp
// ��  ��:		����NPC
// ˵  ��:		
// ��������:	2015��07��11��
// ������:		  
//    :	    
//--------------------------------------------------------------------

#include "TrapNpcModule.h"
#include "utils/extend_func.h"
#include "FsGame/NpcBaseModule/TrapEvent/TrapEventManagerModule.h"
#include "../SkillModule/Impl/TargetFilter.h"
#include "../Define/Skilldefine.h"
#include "utils/custom_func.h"
#include "../Define/GameDefine.h"
#include "utils/string_util.h"
#include "FsGame/Interface/FightInterface.h"

TrapEventManagerModule* TrapNpcModule::m_pTrapEventManager = NULL;
// ��ʼ��
bool TrapNpcModule::Init(IKernel* pKernel)
{
	m_pTrapEventManager = (TrapEventManagerModule*)pKernel->GetLogicModule("TrapEventManagerModule");
	assert(m_pTrapEventManager != NULL);

	pKernel->AddEventCallback("TrapNpc", "OnEntry", TrapNpcModule::OnEntry);
	pKernel->AddEventCallback("TrapNpc", "OnSpring", TrapNpcModule::OnSpring);
	pKernel->AddEventCallback("TrapNpc", "OnEndSpring", TrapNpcModule::OnEndSpring);
	pKernel->AddEventCallback("TrapNpc", "OnDestroy", TrapNpcModule::OnDestroy);
	
    DECL_HEARTBEAT(TrapNpcModule::HB_Delay_Spring);

	return true;
}

// �ͷ�
bool TrapNpcModule::Shut(IKernel* pKernel)
{
	return true;
}

// ���볡��
int TrapNpcModule::OnEntry(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	return 0;
}

// ����ص�
int TrapNpcModule::OnSpring(IKernel* pKernel, const PERSISTID& self, 
					  const PERSISTID& sender, const IVarList& args)
{
	//1����֤�����Ƿ����
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if ( NULL == pSelfObj || !pKernel->Exists(sender) )
	{
		return 0;
	}
	IGameObj* pSenderObj = pKernel->GetGameObj(sender);
	if ( NULL == pSenderObj )
	{
		return 0;
	}

	//2����������������޵еĹ���
	if( !pSenderObj->FindAttr("CantBeAttack") || pSenderObj->QueryInt("CantBeAttack") > 0 )
	{
		return 0;
	}

	//��ȡ�����¼�
	SpringEvent funcID = (SpringEvent)pSelfObj->QueryInt("SpringEvent");
	
	//�ж������Ƿ���Ҫ����
	PERSISTID object = self;
	PERSISTID master = pSelfObj->QueryObject("Master");
	if (pKernel->Exists(master))
	{
		object = master;
	}

	int iSpringType = pSelfObj->QueryInt("SpringType");
	int iSpringRelation = pSelfObj->QueryInt("SpringRelation");// EHitTargetRelationType
	// Ĭ��Ϊ�����ڵз���λ
	if (0 == iSpringRelation)
	{
		iSpringRelation = HIT_TARGET_RELATION_TYPE_ENIMY;
	}
	//����Ŀ������������Ŀ������ת��ƥ��
	int niHitTargetType = TYPE_PLAYER == iSpringType ? HIT_TARGET_TYPE_PLAYER : HIT_TARGET_TYPE_MONSTER;

	if(USESKILL_RESULT_SUCCEED != TargetFilter::SatisfyRelation(pKernel, object, sender, iSpringRelation, niHitTargetType))
	{
		return 0;
	}
	
	m_pTrapEventManager->ExecuteOperate(pKernel, self, sender, funcID);


	// �޸�Npcģ����Դ
    const char* resourceB = pSelfObj->QueryString("ResourceB");
    if( resourceB != NULL )
	{
		pSelfObj->SetString("Resource", resourceB);
	}

	// �Ƿ������ݻ�����
	int iSpringDestroy = pSelfObj->QueryInt("SpringDestroy");
	if (iSpringDestroy == SPRING_DESTROY_IMMEDIATELY)
	{
		pKernel->DestroySelf(self);
	}
	else if( iSpringDestroy == SPRING_DESTROY_EFFECT_TIME_OUT )
	{
		pSelfObj->SetInt("LifeTime", pSelfObj->QueryInt("EffectTime"));
	}
	return 0;
}

//����
int TrapNpcModule::OnEndSpring(IKernel* pKernel, const PERSISTID& self,  const PERSISTID& sender, const IVarList& args)
{
	//1����֤�����Ƿ����
	if ( !pKernel->Exists(self) || !pKernel->Exists(sender) )
	{
		return 0;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if ( NULL == pSelfObj || !pKernel->Exists(sender) )
	{
		return 0;
	}

	//��ȡ�����¼�
	SpringEvent funcID = (SpringEvent)pSelfObj->QueryInt("EndSpringEvent");
	m_pTrapEventManager->ExecuteOperate(pKernel, self, sender, funcID);

	return 0;
}

//�ݻ�
int TrapNpcModule::OnDestroy(IKernel* pKernel, const PERSISTID& self,
						   const PERSISTID& sender, const IVarList& args)
{
	//1����֤��������Ƿ�Ϊ��
	if ( !pKernel->Exists(self) 
		|| !pKernel->Exists(sender) ) 
	{
		return 0;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if ( NULL == pSelfObj || !pKernel->Exists(sender) )
	{
		return 0;
	}

	//2����ȡ�����¼�
	SpringEvent funcID = (SpringEvent)pSelfObj->QueryInt("DestroyEvent");
	m_pTrapEventManager->ExecuteOperate(pKernel, self, sender, funcID);

	return 0;
}

// �ӳ�����
int TrapNpcModule::HB_Delay_Spring(IKernel* pKernel, const PERSISTID& self, int silce)
{
    if (!pKernel->Exists(self))
    {
        return 0;
    }
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return 0;
    }
    IRecord * SelfRecord = pSelfObj->GetRecord("SpringInfoRec");
    if (SelfRecord == NULL)
    {
        return 0;
    }

    //�������������������̴���
    if( SelfRecord->GetRows() >= SelfRecord->GetRowMax() )
    {
        return 0;
    }

    CVarList playerList;
    len_t iSpringRange = pSelfObj->QueryFloat("SpringRange");
    pKernel->GetAroundList(self, iSpringRange, TYPE_PLAYER, 0, playerList);
    if (playerList.GetCount() <= 0)
    {
        return 0;
    }
    const char* BuffID = pSelfObj->QueryString("EventValue");
    if (StringUtil::CharIsNull(BuffID))
    {
        return 0;
    }
    LoopBeginCheck(b);
    for (int i = 0; i < (int)playerList.GetCount(); ++i)
    {
        LoopDoCheck(b);
        PERSISTID player = playerList.ObjectVal(i);
        if (!pKernel->Exists(player))
        {
            continue;
        }
        //������BUFF
        FightInterfaceInstance->AddBuffer(pKernel, player, self, BuffID);

        //������봥�����б�
        CVarList arg_list;
        arg_list << self << BuffID;
        SelfRecord->AddRowValue(-1, arg_list);
    }
    
    return 0;
}