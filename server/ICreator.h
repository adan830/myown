//--------------------------------------------------------------------
// �ļ���:		ICreator.h
// ��  ��:		
// ˵  ��:		
// ��������:	2003��12��30��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_ICREATOR_H
#define _SERVER_ICREATOR_H

#include "../public/Macros.h"
#include "ICallee.h"

// �߼��ӿڴ�����

class ICreator
{
public:
	ICreator(ICreator* pNext) { m_pNext = pNext; }
	virtual ~ICreator() = 0;

	virtual const char* Name() = 0;
	virtual int Type() = 0;
	virtual ICallee* Create() = 0;
	virtual void Destroy(ICallee* pCallee) = 0;
	
	ICreator* Next() { return m_pNext; }
	
private:
	ICreator();
	ICreator(const ICreator &);
	ICreator & operator=(const ICreator &);

private:
	ICreator* m_pNext;
};

inline ICreator::~ICreator() {}

// �߼��ӿڴ���������
extern ICreator* g_pCreator;

#define CALLEE_CREATOR(a, p, t) \
class a##Creator: public ICreator { public: \
a##Creator(ICreator*& pCreator): ICreator(pCreator) { pCreator = this; } \
virtual ~a##Creator() {} \
virtual const char* Name() { return p; } \
virtual int Type() { return t; } \
virtual ICallee* Create() { return NEW a; } \
virtual void Destroy(ICallee* pCallee) { ::delete pCallee; } }; \
a##Creator a##_Creator(g_pCreator);

#define SCRIPT_CREATOR(a, t) CALLEE_CREATOR(a, #a, t)

#endif // _SERVER_ICREATOR_H

