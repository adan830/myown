//--------------------------------------------------------------------
// �ļ���:		SharePtr.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��2��6��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------
/*
#ifndef _PUBLIC_SHAREPTR_H
#define _PUBLIC_SHAREPTR_H

#include "Macros.h"

// TSharePtr

template<typename TYPE>
class TSharePtr
{
private:
	typedef TSharePtr<TYPE> self_type;

public:
	explicit TSharePtr(TYPE* value = NULL)
	{
		m_pValue = NULL;
		m_pRefs = NULL;

		if (value)
		{
			Create(value);
		}
	}

	TSharePtr(const self_type& src)
	{
		m_pValue = src.m_pValue;
		m_pRefs = src.m_pRefs;

		IncRefs();
	}

	~TSharePtr()
	{
		Destroy();
	}

	self_type& operator=(const self_type& src)
	{
		self_type temp(src);
		Swap(temp);
		return *this;
	}

	TYPE& operator*() const
	{
		return *m_pValue;
	}
	
	TYPE* operator->() const
	{
		return m_pValue;
	}

	void Swap(self_type& src)
	{
		TYPE* temp_value = src.m_pValue;
		int* temp_refs = src.m_pRefs;
		src.m_pValue = m_pValue;
		src.m_pRefs = m_pRefs;
		m_pValue = temp_value;
		m_pRefs = temp_refs;
	}

	void Create(TYPE* value)
	{
		Assert(value != NULL);

		Destroy();

		m_pRefs = NEW int(1);
		m_pValue = value;
	}

	void Destroy()
	{
		DecRefs();

		if ((m_pRefs != NULL) && (GetRefs() <= 0))
		{
			delete m_pValue;
			delete m_pRefs;
		}
		
		m_pValue = NULL;
		m_pRefs = NULL;
	}

	TYPE* Get() const
	{
		return m_pValue;
	}
	
	int GetRefs() const
	{
		if (NULL == m_pRefs)
		{
			return 0;
		}

		return (*m_pRefs);
	}
	
	void IncRefs()
	{
		if (m_pRefs)
		{
			++(*m_pRefs);
		}
	}

	void DecRefs()
	{
		if (m_pRefs)
		{
			--(*m_pRefs);
		}
	}

private:
	TYPE* m_pValue;
	int* m_pRefs;
};

#endif // _PUBLIC_SHAREPTR_H
*/
