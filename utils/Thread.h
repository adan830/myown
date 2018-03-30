//--------------------------------------------------------------------
// �ļ���:		Thread.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��10��31��
// ������:		 
//    :	   
//--------------------------------------------------------------------

#ifndef _UTILS_THREAD_H
#define _UTILS_THREAD_H

#include "../public/Macros.h"

#ifdef FX_SYSTEM_WINDOWS
	#include "../system/WinThread.h"
#endif // FX_SYSTEM_WINDOWS

#ifdef FX_SYSTEM_LINUX
	#include "../system/LinuxThread.h"
#endif // FX_SYSTEM_LINUX

/*
#include <windows.h>
#include <process.h>

// CThread
// �߳�

class CThread
{
private:
	typedef void (__cdecl* THREAD_FUNC)(void*);
	
	// �̺߳���
	static void __cdecl WorkerProc(void* lpParameter)
	{
		CThread* pthis = (CThread*)lpParameter;
		
		THREAD_FUNC func = pthis->m_ThreadFunc;
		void* context = pthis->m_pContext;
		int sleep_ms = pthis->m_nSleep;
		
		while (!pthis->m_bQuit)
		{
			func(context);
			
			Sleep(sleep_ms);
		}
	}
	
public:
	CThread(THREAD_FUNC func, void* context, int sleep_ms, int stack_size)
	{
		m_ThreadFunc = func;
		m_pContext = context;
		m_nSleep = sleep_ms;		
		m_nStackSize = stack_size;
		m_bQuit = false;
		m_hThread = NULL;
	}
	
	~CThread()
	{
	}
	
	// �����߳�
	bool Start()
	{
		m_bQuit = false;
	
		m_hThread = (HANDLE)_beginthread(WorkerProc, m_nStackSize, this);
			
		return true;
	}
	
	// ֹͣ�߳�
	bool Stop()
	{
		m_bQuit = true;
	
		if (m_hThread != NULL)
		{
			WaitThreadExit(m_hThread);
			m_hThread = NULL;
		}
	
		return true;
	}
	
private:
	CThread();
	CThread(const CThread&);
	CThread& operator=(const CThread&);
	
	// �ȴ��߳̽���
	bool WaitThreadExit(HANDLE handle)
	{
		DWORD exit_code;

		if (GetExitCodeThread(handle, &exit_code) == FALSE)
		{
			return false;
		}

		while (exit_code == STILL_ACTIVE)
		{
			Sleep(0);

			if (GetExitCodeThread(handle, &exit_code) == FALSE)
			{
				return false;
			}
		}

		return true;
	}

private:
	THREAD_FUNC m_ThreadFunc;
	void* m_pContext;
	int m_nSleep;
	int m_nStackSize;
	bool m_bQuit;
	HANDLE m_hThread;
};
*/

#endif // _UTILS_THREAD_H

