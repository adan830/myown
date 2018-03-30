//-------------------------------------------------------------------
// �� �� ����flexmem.h
// ��    �ݣ�
// ˵    ����
// �������ڣ�2013��9��25��
// �� �� �ˣ�Liangxk
// ��Ȩ���У�������ţ�������޹�˾
//-------------------------------------------------------------------


#if !defined __FLEXMEM_H__
#define __FLEXMEM_H__

#include <stdlib.h>

class FlexMem
{
public:
	FlexMem()
	{
		m_nSize = 0;
		m_pData = 0;
	}

	FlexMem(int size)
	{
		m_nSize = size;
		m_pData = NEW char[size];
	}

	~FlexMem()
	{
		if (m_pData)
		{
			delete[] m_pData;
		}

		m_nSize = 0;
	}

public:
	char* GetBuffer()
	{
		return m_pData;
	}

	bool Resize(int byte_size)
	{
		if (byte_size > m_nSize)
		{
			char* p = NEW char[byte_size];

			if (m_pData)
			{
				// memcpy(p, m_pData, m_nSize);
				delete[] m_pData;
			}
			
			m_pData = p;
			m_nSize = byte_size;
		}

		return true;
	}

	int MaxSize()
	{
		return m_nSize;
	}

private:
	int m_nSize;
	char* m_pData;
};

#endif // __FLEXMEM_H__
