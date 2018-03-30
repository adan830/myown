//--------------------------------------------------------------------
// �ļ���:		IActionBinder.h
// ��  ��:		�����󶨽ӿ�
// ˵  ��:		
// ��������:		2016��5��30��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __I_ACTION_BINDER_H__
#define __I_ACTION_BINDER_H__

#include "IActionHandler.h"

class IActionBinder
{
public:
	virtual ~IActionBinder() = 0;

	virtual bool BindAction( int nActionId, ACTION_EVENT action, int nPriority ) = 0;
	virtual bool BindAction( const char* nActionKey, ACTION_EVENT action, int nPriority ) = 0;
};
inline IActionBinder::~IActionBinder() {};



#endif // END __I_ACTION_BINDER_H__