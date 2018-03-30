//--------------------------------------------------------------------
// �ļ���:		IEntCreator.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��1��31��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_IENTCREATOR_H
#define _PUBLIC_IENTCREATOR_H

#include "Macros.h"

// IEntCreator
// ʵ�崴����

class IEntity;

// ʵ����������
struct IEntProperty
{
	const char* m_strName;
	int m_nType;
	void* m_pGetFunc;
	void* m_pSetFunc;
	IEntProperty* m_pNext;
};

// ʵ�巽������
struct IEntMethod
{
	const char* m_strName;
	void* m_pMidFunc;
	bool m_bReturnTable;
	IEntMethod* m_pNext;
};

class IEntCreator
{
public:
	IEntCreator(IEntCreator* pNext)
	{  
		m_pNext = pNext; 
		m_pProperty = NULL;
		m_pMethod = NULL;
	}

	virtual ~IEntCreator() = 0;

	// �Ƿ�Ϊ�����ֻࣨ�����ڼ̳У�
	virtual bool IsAbstract() const { return false; }

	// ��ø�������
	virtual const char* GetParent() const = 0;

	// �������ֿռ�
	virtual const char* GetSpace() const = 0;

	// ��������
	virtual const char* GetName() const = 0;

	// ����
	virtual IEntity* Create() = 0;
	
	// ɾ��
	virtual void Destroy(IEntity* p) = 0;
	
	// �����һ��
	IEntCreator* GetNext()	const { return m_pNext; }

	// �����������
	IEntProperty* GetPropertyLink() const { return m_pProperty; }

	// ������������
	void SetPropertyLink(IEntProperty* value) { m_pProperty = value; }
	
	// ��÷�������
	IEntMethod* GetMethodLink() const { return m_pMethod; }

	// ���÷�������
	void SetMethodLink(IEntMethod* value) { m_pMethod = value; }

private:
	IEntCreator();

private:
	IEntCreator* m_pNext;
	IEntProperty* m_pProperty;
	IEntMethod* m_pMethod;
};

inline IEntCreator::~IEntCreator() {}

#endif // _PUBLIC_IENTCREATOR_H

