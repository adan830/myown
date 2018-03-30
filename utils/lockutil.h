//-------------------------------------------------------------------- 
// �ļ���:		LockUtil.h 
// ��  ��:		
// ˵  ��:		
// ��������:	2002��7��9��	
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _UTILS_LOCKUTIL_H
#define _UTILS_LOCKUTIL_H

#include "../public/Macros.h"

#ifdef FX_SYSTEM_WINDOWS
	#include "../system/WinLock.h"
	#include "../system/WinLockVal.h"
#endif // FX_SYSTEM_WINDOWS

#ifdef FX_SYSTEM_LINUX
	#include "../system/LinuxLock.h"
	#include "../system/LinuxLockVal.h"
#endif // FX_SYSTEM_LINUX

/*
#include <windows.h>

// ������Դ���������

class CLockUtil
{
public:
	CLockUtil() 
	{ 
		InitializeCriticalSection(&m_csHandle); 
	}

	~CLockUtil() 
	{ 
		DeleteCriticalSection(&m_csHandle); 
	}

	// ����
	void Lock() 
	{ 
		EnterCriticalSection(&m_csHandle); 
	}

	// �ͷ�
	void Unlock() 
	{	
		LeaveCriticalSection(&m_csHandle); 
	}

private:
	CRITICAL_SECTION m_csHandle;
};
*/

// �ýṹ���������Զ��ӽ���

class CAutoLock
{
public:
	explicit CAutoLock(CLockUtil& lock): m_Lock(lock)
	{
		m_Lock.Lock();
	}
	
	~CAutoLock() 
	{ 
		m_Lock.Unlock(); 
	}

private:
	CAutoLock();
	CAutoLock(const CAutoLock&);
	CAutoLock& operator=(const CAutoLock&);

private:
	CLockUtil& m_Lock;
};

/*
// CLockVal
// ����������

class CLockVal
{
public:
	CLockVal()
	{
		m_nVal = 0;
	}

	int Inc()
	{
		return InterlockedIncrement(&m_nVal);
	}

	int Dec()
	{
		return InterlockedDecrement(&m_nVal);
	}

	int Set(int val)
	{
		return InterlockedExchange(&m_nVal, val);
	}

	int Get() const
	{
		return m_nVal;
	}

public:
	LONG m_nVal;
};
*/

#endif // _UTILS_LOCKUTIL_H
