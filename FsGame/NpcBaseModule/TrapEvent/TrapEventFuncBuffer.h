//--------------------------------------------------------------------
// �ļ���:      TrapEventFuncBuffer.h
// ��  ��:      ����NPC����Buff�¼��߼�
// ˵  ��:		
// ��������:    2015��07��13��
// ������:        
//    :       
//--------------------------------------------------------------------
#ifndef _Trap_Event_Func_Buffer_H_
#define _Trap_Event_Func_Buffer_H_
#include "TrapEventBase.h"

//����BUFF
//SPRINGEVENT_SINGLE_BUFF = 3, 
class TrapEventSingleBuff : public TrapEventBase
{
public:
	virtual bool Execute(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& target, const IVarList& vEventParams);
};


//Ⱥ��BUFF
//SPRINGEVENT_MUTI_BUFF = 4, 
class TrapEventMutiBuff : public TrapEventBase
{
public:
	// ��ʼ��
	virtual bool OnInit(IKernel* pKernel);

	// �ӳ�����
	static int HB_Delay_Spring(IKernel* pKernel, const PERSISTID& self, int silce);

	// ���м����¼�
	virtual bool Execute(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& target, const IVarList& vEventParams);
private:
	static void OnAddBuff(IKernel* pKernel, const PERSISTID& self);
};

#endif