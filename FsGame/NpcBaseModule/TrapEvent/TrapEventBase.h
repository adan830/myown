//--------------------------------------------------------------------
// �ļ���:      TrapEventBase.h
// ��  ��:      ���״����¼�����
// ˵  ��:		���еĵ��״����¼����̳д˻���
// ��������:    2015��07��13��
// ������:        
//    :       
//--------------------------------------------------------------------
#ifndef _Trap_Event_Base_H_
#define _Trap_Event_Base_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define//TrapEventFuncIdDefine.h"

// ���м����¼�����
class TrapEventBase
{
public:
	TrapEventBase();
	virtual ~TrapEventBase();

	// ���������¼�
	static TrapEventBase* CreateTrapEvent(const SpringEvent func_id);

	// ��ʼ��
	virtual bool OnInit(IKernel* pKernel);

	// ���м����¼�
	virtual bool Execute(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& target, const IVarList& vEventParams) = 0;
};

#endif