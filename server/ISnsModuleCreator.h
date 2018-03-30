//--------------------------------------------------------------------
// �ļ���:		IModuleCreator.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��5��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IMODULECREATOR_H
#define _SERVER_IMODULECREATOR_H

#include "../public/Macros.h"
#include "ILogicModule.h"

// �߼�ģ�鴴�����ӿ�

class IModuleCreator
{
public:
	IModuleCreator(IModuleCreator* pNext) { m_pNext = pNext; }
	virtual ~IModuleCreator() = 0;

	// �߼�ģ������
	virtual const char* Name() = 0;
	// �����߼�ģ��
	virtual ILogicModule* Create() = 0;
	// ɾ���߼�ģ��
	virtual void Destroy(ILogicModule* pModule) = 0;
	
	IModuleCreator* Next() { return m_pNext; }
	
private:
	IModuleCreator();
	IModuleCreator(const IModuleCreator &);
	IModuleCreator & operator=(const IModuleCreator &);

private:
	IModuleCreator* m_pNext;
};

inline IModuleCreator::~IModuleCreator() {}

// �߼�ģ�鴴��������
extern IModuleCreator* g_pModuleCreator;

// �����߼�ģ�鴴����
#define LOGIC_MODULE_CREATOR(a) \
class a##ModuleCreator: public IModuleCreator { public: \
a##ModuleCreator(IModuleCreator*& pCreator): IModuleCreator(pCreator) { pCreator = this; } \
virtual ~a##ModuleCreator() {} \
virtual const char* Name() { return #a; } \
virtual ILogicModule* Create() { return NEW a; } \
virtual void Destroy(ILogicModule* pModule) { ::delete pModule; } }; \
a##ModuleCreator a##_ModuleCreator(g_pModuleCreator);

#endif // _SERVER_IMODULECREATOR_H

