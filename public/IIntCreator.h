//--------------------------------------------------------------------
// �ļ���:		IIntCreator.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��1��31��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_IINTCREATOR_H
#define _PUBLIC_IINTCREATOR_H

#include "Macros.h"

// IIntCreator
// ���ܽӿڴ�����

class IInterface;

class IIntCreator
{
public:
	IIntCreator(IIntCreator* pNext) { m_pNext = pNext; }
	
	virtual ~IIntCreator() = 0;

	// �������ֿռ�
	virtual const char* GetSpace() const = 0;
	
	// ��������
	virtual const char* GetName() const = 0;
	
	// ����
	virtual IInterface* Create() = 0;
	
	// ɾ��
	virtual void Destroy(IInterface* p) = 0;
	
	// �����һ��
	IIntCreator* GetNext() const { return m_pNext; }

private:
	IIntCreator();

private:
	IIntCreator* m_pNext;
};

inline IIntCreator::~IIntCreator() {}

#endif // _PUBLIC_IINTCREATOR_H

