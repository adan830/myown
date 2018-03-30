//--------------------------------------------------------------------
// �ļ���:		ReLoadConfigModule.h
// ��  ��:		���¼�������
// ˵  ��:		
// ��������:	2016��09��13��
// ��������:	2016��09��13��
// ������:		������   
//--------------------------------------------------------------------

#ifndef __ReLoadConfigModule_H__
#define __ReLoadConfigModule_H__

#include "Fsgame/Define/header.h"
#include <string>
#include <map>

// ���¶�ȡ���ûص�
typedef void(__cdecl* RELOAD_CONFIG_CALLBACK)(IKernel *pKernel);

class ReLoadConfigModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init( IKernel* pKernel );

	// �ͷ�
	virtual bool Shut( IKernel* pKernel );

	// ע���������ûص�
	void RegisterReloadConfig(const char* strKey, RELOAD_CONFIG_CALLBACK pReloadCall);

	// ����������������
	bool SendReloadConfigCommand(IKernel* pKernel, const char* strKey);
private:
	// ���������ļ�
	static int OnCommandRereadLoadResource(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

	typedef std::map< std::string, RELOAD_CONFIG_CALLBACK > ReloadConfigCallMap;
public:
	static ReLoadConfigModule* m_pReLoadConfigModule;
	
	ReloadConfigCallMap m_mapReloadConfig;			// ���ö�ȡ�ص�
};

#define RELOAD_CONFIG_REG(key, call) ReLoadConfigModule::m_pReLoadConfigModule->RegisterReloadConfig(key, call);

#endif	// __GMModule_H__