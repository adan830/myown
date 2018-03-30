//-------------------------------------------------------------------
// �� �� ����fast_thread_lock.h
// ��    �ݣ�
// ˵    ����
// �������ڣ�2014��2��7��
// �� �� �ˣ�Liangxk
// ��Ȩ���У�������ţ�������޹�˾
//-------------------------------------------------------------------

#if !defined __FAST_THREAD_LOCK_H__
#define __FAST_THREAD_LOCK_H__

#include "../public/Macros.h"
#include <Windows.h>

/// compare and swap
//#define _USE_64_BIT_FLAG_
#if defined InterlockedCompareExchange64
#	define CAS(ptr,old_val,new_val) ((LONGLONG)(old_val) == InterlockedCompareExchange64((ptr),(new_val),(old_val)))
typedef LONGLONG CAS_TYPE;
#else
#	define CAS(ptr,old_val,new_val) ((LONG)(old_val) == InterlockedCompareExchange((volatile LONG *)(ptr),(new_val),(old_val)))
typedef unsigned int CAS_TYPE;
#endif // _USE_64_BIT_FLAG_

class FastThreadLock
{
public:
	FastThreadLock() : m_nLockThreadId(0)
	{

	}
	void Lock()
	{
		CAS_TYPE id = 0x1; /// (uint)GetCurrentThreadId();

		for (; !CAS(&m_nLockThreadId, 0, id); )
		{
			Sleep(0);
		}
	}
	void Unlock()
	{
		m_nLockThreadId = 0;
	}
private:
	CAS_TYPE m_nLockThreadId;
};

class AutoFastLock
{
public:
	AutoFastLock(FastThreadLock* lock) : m_pLock(lock)
	{
		m_pLock->Lock();
	}
	~AutoFastLock()
	{
		m_pLock->Unlock();
	}
private:
	FastThreadLock* m_pLock;
};

#endif // __FAST_THREAD_LOCK_H__