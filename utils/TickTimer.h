//--------------------------------------------------------------------
// �ļ���:		TickTimer.h
// ��  ��:		
// ˵  ��:		
// ��������:	2002��8��9��
// ������:		 
//    :	   
//--------------------------------------------------------------------

#ifndef _UTILS_TICKTIMER_H
#define _UTILS_TICKTIMER_H

#include "../public/Portable.h"

// �;��ȼ�ʱ��

class CTickTimer
{
public:
	CTickTimer()
	{
		m_nLastMilliseconds = 0;
	}
	
	// ��ʼ��
	void Initialize()
	{
		m_nLastMilliseconds = Port_GetTickCount();
	}
	
	// ������ȥ�ĺ�����
	int GetElapseMillisec(int expect = 0)
	{
		unsigned int cur = Port_GetTickCount();
		unsigned int dw = cur - m_nLastMilliseconds;
			
		if (dw >= (unsigned int)expect)
		{
			m_nLastMilliseconds = cur;
		}
			
		return dw;
	}

private:
	unsigned int m_nLastMilliseconds;
};

#endif // _UTILS_TICKTIMER_H
