//--------------------------------------------------------------------
// �ļ���:		IModuleCreator.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��5��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IROOM_MODULE_CREATOR_H
#define _SERVER_IROOM_MODULE_CREATOR_H

#include "../public/Macros.h"
#include "IRoomLogicModule.h"

// �߼�ģ�鴴�����ӿ�

class IRoomModuleCreator
{
public:
	IRoomModuleCreator(IRoomModuleCreator* pNext) { m_pNext = pNext; }
	virtual ~IRoomModuleCreator() = 0;

	// �߼�ģ������
	virtual const char* Name() = 0;
	// �����߼�ģ��
	virtual IRoomLogicModule* Create() = 0;
	// ɾ���߼�ģ��
	virtual void Destroy(IRoomLogicModule* pModule) = 0;
	
	IRoomModuleCreator* Next() { return m_pNext; }
	
private:
	IRoomModuleCreator();
	IRoomModuleCreator(const IRoomModuleCreator &);
	IRoomModuleCreator & operator=(const IRoomModuleCreator &);

private:
	IRoomModuleCreator* m_pNext;
};

inline IRoomModuleCreator::~IRoomModuleCreator() {}

// �߼�ģ�鴴��������
extern IRoomModuleCreator* g_pRoomModuleCreator;

// �����߼�ģ�鴴����
#define ROOM_LOGIC_MODULE_CREATOR(a) \
class a##ModuleCreator: public IRoomModuleCreator { public: \
a##ModuleCreator(IRoomModuleCreator*& pCreator): IRoomModuleCreator(pCreator) { pCreator = this; } \
virtual ~a##ModuleCreator() {} \
virtual const char* Name() { return #a; } \
virtual IRoomLogicModule* Create() { return NEW a; } \
virtual void Destroy(IRoomLogicModule* pModule) { ::delete pModule; } }; \
a##ModuleCreator a##_ModuleCreator(g_pRoomModuleCreator);

#endif // _SERVER_IMODULECREATOR_H

