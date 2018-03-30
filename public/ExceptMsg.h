//--------------------------------------------------------------------
// �ļ���:		ExceptMsg.h
// ��  ��:		
// ˵  ��:		
// ��������:	2002��7��9��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_EXCEPTMSG_H
#define _PUBLIC_EXCEPTMSG_H

#include "Macros.h"

// CExceptMsg
// �쳣��Ϣ

class CExceptMsg
{
public:
	CExceptMsg(const char* msg)
	{
		Assert(msg != NULL);
		
		m_pMsg = msg;
	}

	~CExceptMsg()
	{
	}

	const char* GetMsg() const
	{
		return m_pMsg;
	}

private:
	CExceptMsg();

private:
	const char* m_pMsg;
};

#endif // _PUBLIC_EXCEPTMSG_H
