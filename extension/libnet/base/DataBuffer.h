//--------------------------------------------------------------------
// �ļ���:		DataBuffer.h
// ��  ��:		���ݻ������
// ˵  ��:		
// ��������:		2016��06��07��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __DATA_BUFFER_H__
#define __DATA_BUFFER_H__
#include <string.h>

class DataBuffer
{
	char*	m_pb;
	size_t	m_nMaxSize;
	size_t	m_nWritePos;
	size_t	m_nReadPos;

public:
	DataBuffer(size_t nSize)
	{
		m_pb = new char[nSize];
		m_nMaxSize = nSize;
		m_nWritePos = 0;
		m_nReadPos = 0;
	}

	~DataBuffer()
	{
		assert(NULL != m_pb);
		delete m_pb;
		m_pb = NULL;
	}

	char* GetBuffer() const
	{
		return m_pb;
	}

	size_t GetLeftSpace() const
	{
		return m_nMaxSize > m_nWritePos ? m_nMaxSize - m_nWritePos : 0;
	}

	size_t GetValidSize() const
	{
		return m_nWritePos >= m_nReadPos ? m_nWritePos - m_nReadPos : 0;
	}

	size_t Write( const char* p, size_t nSize )
	{
		if (nSize + m_nWritePos > m_nMaxSize)
		{
			nSize = m_nMaxSize - m_nWritePos;
		}

		if (NULL != p && nSize > 0)
		{
			memcpy(m_pb + m_nWritePos, p, nSize);
		}
		m_nWritePos += nSize;
		return nSize;
	}

	size_t Read( char*p, size_t nSize )
	{
		if (nSize + m_nReadPos > m_nWritePos)
		{
			nSize = m_nWritePos - m_nReadPos;
		}

		if (nSize > 0)
		{
			if ( NULL != p )
			{
				memcpy(p, m_pb + m_nReadPos, nSize);
			}
		}
		m_nReadPos += nSize;
		return nSize;
	}

	size_t GetReadPos() const
	{
		return m_nReadPos;
	}

	size_t GetWritePos() const
	{
		return m_nWritePos;
	}

	bool Rewind()
	{
		if (m_nReadPos == 0)
		{
			return false;
		}

		size_t nSize = GetValidSize();
		if (nSize > 0)
		{
			memcpy(m_pb, m_pb + m_nReadPos, nSize);
			m_nReadPos = 0;
			m_nWritePos = nSize;
		}
		else
		{
			m_nReadPos = m_nWritePos = 0;
		}

		return true;
	}

	void Clear()
	{
		m_nReadPos = m_nWritePos = 0;
	}
};

#endif // END __DATA_BUFFER_H__