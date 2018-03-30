//--------------------------------------------------------------------
// �ļ���:		ILogicCreator.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��12��25��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_ILOGICCREATOR_H
#define _PUBLIC_ILOGICCREATOR_H

#include "Macros.h"

// ILogicCreator
// �߼�������

class ILogic;

// �߼���������
struct ILogicCallback
{
	const char* m_strName;
	void* m_pMidFunc;
	bool m_bReturnTable;
	ILogicCallback* m_pNext;
};

class ILogicCreator
{
public:
	ILogicCreator(ILogicCreator* pNext)
	{  
		m_pNext = pNext; 
		m_pCallback = NULL;
	}

	virtual ~ILogicCreator() = 0;

	// �������ֿռ�
	virtual const char* GetSpace() const = 0;

	// ��������
	virtual const char* GetName() const = 0;

	// ����
	virtual ILogic* Create() = 0;
	
	// ɾ��
	virtual void Destroy(ILogic* p) = 0;
	
	// �����һ��
	ILogicCreator* GetNext() const { return m_pNext; }

	// ��÷�������
	ILogicCallback* GetCallbackLink() const { return m_pCallback; }

	// ���÷�������
	void SetCallbackLink(ILogicCallback* value) { m_pCallback = value; }

private:
	ILogicCreator();

private:
	ILogicCreator* m_pNext;
	ILogicCallback* m_pCallback;
};

inline ILogicCreator::~ILogicCreator() {}

#endif // _PUBLIC_ILOGICCREATOR_H

