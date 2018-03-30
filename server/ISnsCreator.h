//--------------------------------------------------------------------
// �ļ���:		ICreator.h
// ��  ��:		
// ˵  ��:		
// ��������:	2003��12��30��	
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _ISNSCREATOR_H
#define _ISNSCREATOR_H

//#include "Macros.h"
#include "ISnsCallee.h"

// ICreator


class ISnsCreator
{
public:
	ISnsCreator(ISnsCreator * pNext) { m_pNext = pNext; }
	virtual ~ISnsCreator() = 0;

	virtual const char * Name() = 0;
	virtual ISnsCallee * Create() = 0;
	virtual void Destroy(ISnsCallee * pCallee) = 0;

	ISnsCreator * Next() { return m_pNext; }

private:
	ISnsCreator();
	ISnsCreator(const ISnsCreator &);
	ISnsCreator & operator=(const ISnsCreator &);

private:
	ISnsCreator * m_pNext;
};

inline ISnsCreator::~ISnsCreator() {}

extern ISnsCreator * g_pSnsCreator;

#define SNS_CALLEE_CREATOR(a, p) \
class a##Creator: public ISnsCreator { public: \
	a##Creator(ISnsCreator * & pCreator): ISnsCreator(pCreator) { pCreator = this; } \
	virtual ~##a##Creator() {} \
	virtual const char * Name() { return p; } \
	virtual ISnsCallee * Create() { return NEW a; } \
	virtual void Destroy(ISnsCallee * pCallee) { ::delete pCallee; } }; \
	a##Creator a##_Creator(g_pSnsCreator);

#define SNS_SCRIPT_CREATOR(a) SNS_CALLEE_CREATOR(a, #a)

typedef ISnsCreator * (__cdecl * DLL_CREATOR)();

#endif // _ICREATOR_H

