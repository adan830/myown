//--------------------------------------------------------------------
// �ļ���:		Mutex.h
// ��  ��:		
// ˵  ��:		
// ��������:	2006��8��23��	
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _UTILS_MUTEX_H
#define _UTILS_MUTEX_H

#include "../public/Macros.h"

#ifdef FX_SYSTEM_WINDOWS
	#include "../system/WinMutex.h"
#endif // FX_SYSTEM_WINDOWS

#ifdef FX_SYSTEM_LINUX
	#include "../system/LinuxMutex.h"
#endif // FX_SYSTEM_LINUX

/*
#include <windows.h>
#include <string.h>

// ���̻�����

class CWinMutex
{
public:
	static bool Exists(const char* name)
	{
		HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, name);

		if (NULL == mutex)
		{
			return false;
		}

		CloseHandle(mutex);

		return true;
	}

public:
	CWinMutex()
	{
		m_pName = NULL;
		m_hMutex = NULL;
	}

	~CWinMutex()
	{
		Destroy();
	}

	// �������
	const char* GetName() const
	{
		if (NULL == m_pName)
		{
			return "";
		}
		
		return m_pName;
	}
	
	// �Ƿ���Ч
	bool IsValid() const
	{
		return (m_hMutex != NULL);
	}
	
	// ����������
	bool Create(const char* name, bool* exists = NULL)
	{
		Assert(name != NULL);

		size_t name_size = strlen(name) + 1;
		
		char* pName = NEW char[name_size];
		
		memcpy(pName, name, name_size);
		
		if (m_pName)
		{
			delete[] m_pName;
		}
		
		m_pName = pName;
		
		if (exists)
		{
			*exists = false;
		}

		m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, name);

		if (NULL == m_hMutex)
		{
			m_hMutex = CreateMutex(NULL, FALSE, name);

			if (NULL == m_hMutex)
			{
				return false;
			}

			if (exists)
			{
				if (GetLastError() == ERROR_ALREADY_EXISTS)
				{
					*exists = true;
				}
			}
		}
		else
		{
			if (exists)
			{
				*exists = true;
			}
		}

		return true;
	}

	// ɾ����
	bool Destroy()
	{
		if (!CloseHandle(m_hMutex))
		{
			return false;
		}

		m_hMutex = NULL;
		
		if (m_pName)
		{
			delete[] m_pName;
			m_pName = NULL;
		}
		
		return true;
	}

	// ����
	void Lock()
	{
		Assert(m_hMutex != NULL);

		WaitForSingleObject(m_hMutex, INFINITE);
	}

	// ����
	void Unlock()
	{
		Assert(m_hMutex != NULL);

		ReleaseMutex(m_hMutex);
	}

private:
	CWinMutex(const CWinMutex&);
	CWinMutex& operator=(const CWinMutex&);

private:
	char* m_pName;
	HANDLE m_hMutex;
};
*/

// �Զ��ӽ���

class CAutoMutex
{
public:
	explicit CAutoMutex(CMutex& mutex): m_Mutex(mutex)
	{
		Assert(mutex.IsValid());

		m_Mutex.Lock();
	}

	~CAutoMutex()
	{
		m_Mutex.Unlock();
	}

private:
	CAutoMutex();
	CAutoMutex(const CAutoMutex&);
	CAutoMutex& operator=(const CAutoMutex&);

private:
	CMutex& m_Mutex;
};

#endif // _UTILS_MUTEX_H
