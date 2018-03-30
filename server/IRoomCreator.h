//--------------------------------------------------------------------
// �ļ���:		ICreator.h
// ��  ��:		
// ˵  ��:		
// ��������:	2003��12��30��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_I_ROOM_CREATOR_H
#define _SERVER_I_ROOM_CREATOR_H

#include "../public/Macros.h"
#include "IRoomCallee.h"

// �߼��ӿڴ�����

class IRoomCreator
{
public:
	IRoomCreator(IRoomCreator* pNext) { m_pNext = pNext; }
	virtual ~IRoomCreator() = 0;

	virtual const char* Name() = 0;
	virtual int Type() = 0;
	virtual IRoomCallee* Create() = 0;
	virtual void Destroy(IRoomCallee* pCallee) = 0;
	
	IRoomCreator* Next() { return m_pNext; }
	
private:
	IRoomCreator();
	IRoomCreator(const IRoomCreator &);
	IRoomCreator & operator=(const IRoomCreator &);

private:
	IRoomCreator* m_pNext;
};

inline IRoomCreator::~IRoomCreator() {}

// �߼��ӿڴ���������
extern IRoomCreator* g_pRoomCreator;

#define ROOM_CALLEE_CREATOR(a, p, t) \
class a##Creator: public IRoomCreator { public: \
a##Creator(ICreator*& pCreator): IRoomCreator(pCreator) { pCreator = this; } \
virtual ~a##Creator() {} \
virtual const char* Name() { return p; } \
virtual int Type() { return t; } \
virtual IRoomCallee* Create() { return NEW a; } \
virtual void Destroy(IRoomCallee* pCallee) { ::delete pCallee; } }; \
a##Creator a##_Creator(g_pRoomCreator);

#define ROOM_SCRIPT_CREATOR(a, t) ROOM_CALLEE_CREATOR(a, #a, t)

#endif // _SERVER_ICREATOR_H

