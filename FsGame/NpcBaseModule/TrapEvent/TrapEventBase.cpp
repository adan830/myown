//--------------------------------------------------------------------
// �ļ���:      TrapEventBase.cpp
// ��  ��:      ���״����¼�����
// ˵  ��:		���еĵ��״����¼����̳д˻���
// ��������:    2015��07��13��
// ������:        
//    :       
//--------------------------------------------------------------------
#include "FsGame/NpcBaseModule/TrapEvent/TrapEventBase.h"
#include "TrapEventFuncBuffer.h"
#include "TrapEventFuncDestory.h"

TrapEventBase::TrapEventBase()
{

}

TrapEventBase::~TrapEventBase()
{

}

// ���������¼�
TrapEventBase* TrapEventBase::CreateTrapEvent(const SpringEvent func_id)
{
	TrapEventBase* pEventBase = NULL;

	switch(func_id)
	{
	//����BUFF
	case SPRINGEVENT_SINGLE_BUFF:
		pEventBase = new TrapEventSingleBuff;
		break;
	//Ⱥ��BUFF
	case SPRINGEVENT_MUTI_BUFF:
		pEventBase = new TrapEventMutiBuff;
		break;
	case SPRINGEVENT_DESTORY_OBJ:
		pEventBase = new TrapEventFuncDestory;
		break;
	}

	return pEventBase;
}

// ��ʼ��
bool TrapEventBase::OnInit(IKernel* pKernel)
{
	return true;
}
