//--------------------------------------------------------------------
// �ļ���:      TrapEventFuncBuffer.h
// ��  ��:      ���崥��Buff�¼��߼�
// ˵  ��:		
// ��������:    2015��07��13��
// ������:        
//    :       
//--------------------------------------------------------------------
#include "FsGame/NpcBaseModule/TrapEvent/TrapEventManagerModule.h"
#include "FsGame/NpcBaseModule/TrapEvent/TrapEventFuncBuffer.h"

#include "FsGame/Define/BufferDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"
#include "utils/extend_func.h"

#include "FsGame/Interface/FightInterface.h"

//����BUFF
//SPRINGEVENT_SINGLE_BUFF = 3, 
bool TrapEventSingleBuff::Execute(IKernel* pKernel, const PERSISTID& self,
								  const PERSISTID& target, const IVarList& vEventParams)
{
	if (!pKernel->Exists(self) || !pKernel->Exists(target))
	{
		return false;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	//�ж�����Ƿ����
	IGameObj * pTargetObj = pKernel->GetGameObj(target);
	if (pTargetObj == NULL)
	{
		return false;
	}

	//��ȡ������
	IRecord * SelfRecord = pSelfObj->GetRecord("SpringInfoRec");
	if (SelfRecord == NULL)
	{
		return false;
	}

	//�������������������̴���
	if( SelfRecord->GetRows() >= SelfRecord->GetRowMax() )
	{
		return false;
	}

	//��ȡ���������
	PERSISTID owner = pSelfObj->QueryObject("Master");
	PERSISTID sender = self;
	if (pKernel->Exists(owner))
	{
		sender = owner;
	}

	//��ȡ��Ϣ
	const char * BuffID = vEventParams.StringVal(0);

	//�鿴�Ƿ��Ѿ����ӹ���Ŀ��
	int iRow = SelfRecord->FindObject(0, target);
	if (iRow >= 0)
	{
		return false;
	}
    int delayTime = 0;
    if (pSelfObj->FindAttr("DelayTime"))
    {
        delayTime = pSelfObj->QueryInt("DelayTime");
    }
    
    if (delayTime > 0)
    {
        ADD_COUNT_BEAT(pKernel, self, "TrapNpcModule::HB_Delay_Spring", delayTime, 1);
    }
    else
    {
        //������BUFF
        FightInterfaceInstance->AddBuffer(pKernel, target, sender, BuffID);

        //������봥�����б�
        CVarList arg_list;
        arg_list << target << BuffID;
        SelfRecord->AddRowValue(-1, arg_list);
    }
	return true;
}

// ��ʼ��
bool TrapEventMutiBuff::OnInit(IKernel* pKernel)
{
	DECL_HEARTBEAT(TrapEventMutiBuff::HB_Delay_Spring);

	return true;
}
// �ӳ�����
int TrapEventMutiBuff::HB_Delay_Spring(IKernel* pKernel, const PERSISTID& self, int silce)
{
	OnAddBuff(pKernel, self);
	return 0;
}

//Ⱥ��BUFF
//SPRINGEVENT_MUTI_BUFF = 4, 
bool TrapEventMutiBuff::Execute(IKernel* pKernel, const PERSISTID& self,
								const PERSISTID& target, const IVarList& vEventParams)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	//�ж�����Ƿ����
	IGameObj * pTargetObj = pKernel->GetGameObj(target);
	if (pSelfObj == NULL || pTargetObj == NULL)
	{
		return 0;
	}

	int delayTime = 0;
	if (pSelfObj->FindAttr("DelayTime"))
	{
		delayTime = pSelfObj->QueryInt("DelayTime");
	}

	if (delayTime > 0)
	{
		ADD_COUNT_BEAT(pKernel, self, "TrapEventMutiBuff::HB_Delay_Spring", delayTime, 1);
	}
	else
	{
		OnAddBuff(pKernel, self);
	}
	return true;
}

void TrapEventMutiBuff::OnAddBuff(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return;
	}

	//��ȡ������
	IRecord * SelfRecord = pSelfObj->GetRecord("SpringInfoRec");
	if (SelfRecord == NULL)
	{
		return;
	}

	//��ȡ���������
	PERSISTID owner = pSelfObj->QueryObject("Master");
	PERSISTID sender = self;
	if (pKernel->Exists(owner))
	{
		sender = owner;
	}

	IGameObj* pSenderObj = pKernel->GetGameObj(sender);
	if (pSenderObj == NULL)
	{
		return;
	}

	//��ȡ��Ϣ
	const char * BuffID = pSelfObj->QueryString("EventValue");
	float iEventRange = pSelfObj->QueryFloat("EventRange");
	if (iEventRange <= 0)
	{
		return;
	}

	CVarList CResult;
	pKernel->GetAroundList(self, iEventRange, 0, 0, CResult);
	int nCount = static_cast<int>(CResult.GetCount());

	//�������ð�ȫ�����˻���Ӧ������
	LoopBeginCheck(a);
	for (int i = 0; i < nCount; ++i)
	{
		LoopDoCheck(a);
		//��֤������Ч��(��Χ����������������ѷ�Npc����)
		PERSISTID obj = CResult.ObjectVal(i);
		IGameObj* pObj = pKernel->GetGameObj(obj);
		if (pObj == NULL)
		{
			continue;
		}

		if (!pKernel->Exists(obj) || pSenderObj->QueryInt(FIELD_PROP_CAMP) == pObj->QueryInt(FIELD_PROP_CAMP) || pObj->QueryInt(FIELD_PROP_CANT_BE_ATTACK) > 0)
		{
			continue;
		}

		//�鿴�Ƿ��Ѿ����ӹ���Ŀ��
		int iRow = SelfRecord->FindObject(0, obj);
		if (iRow >= 0)
		{
			return;
		}
		//������BUFF
		FightInterfaceInstance->AddBuffer(pKernel, obj, sender, BuffID);

		//������봥�����б�
		CVarList arg_list;
		arg_list << obj << BuffID;
		SelfRecord->AddRowValue(-1, arg_list);
	}
}
