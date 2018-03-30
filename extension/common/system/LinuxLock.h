//-------------------------------------------------------------------- 
// �ļ���:		LinuxLock.h 
// ��  ��:		
// ˵  ��:		
// ��������:	2010��12��15��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SYSTEM_LINUXLOCK_H
#define _SYSTEM_LINUXLOCK_H

#include <pthread.h>

// ������Դ���������

class CLockUtil
{
public:
	CLockUtil() 
	{ 
		pthread_mutex_init(&m_Handle, NULL); 
	}

	~CLockUtil() 
	{ 
		pthread_mutex_destroy(&m_Handle); 
	}

	// ����
	void Lock() 
	{ 
		pthread_mutex_lock(&m_Handle); 
	}

	// �ͷ�
	void Unlock() 
	{	
		pthread_mutex_unlock(&m_Handle); 
	}

private:
	pthread_mutex_t m_Handle;
};

#endif // _SYSTEM_LINUXLOCK_H
