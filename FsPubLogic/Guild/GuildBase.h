#ifndef __GUILD_BASE_H__
#define __GUILD_BASE_H__
#include "server\IPubKernel.h"
#include "FsGame\Define\PubDefine.h"
#include "GuildManager.h"

class GuildBase{
public:
	GuildBase(){
		m_pGuildPubData = PubGuildManager::m_pInstance->m_pGuildPubData;
		m_ServerId = PubGuildManager::m_pInstance->m_ServerId;
		m_pPubSpace = PubGuildManager::m_pInstance->m_pPubSpace;
	};
	virtual ~GuildBase(){};
	virtual bool Init(IPubKernel* pPubKernel) =0;
	virtual int OnPublicMessage(IPubKernel* pPubKernel, int source_id,
		int scene_id, const IVarList& msg){
		return 0;
	};
	virtual bool CreateGuildInit(IPubData* guildData){ return true; };
	// �������ݿռ�����
	virtual const char* GetName(){
		return PUBSPACE_GUILD;
	};
	//���벹���ӿ�
	virtual bool Patch(IPubData* guildData){ return true; };

	virtual void ShutDown(){};
	virtual bool CallBack(){ return true; };
	IPubSpace* m_pPubSpace{NULL};         // ���ṫ��������
	IPubData* m_pGuildPubData{ NULL };      // ���ṫ��������
	std::wstring m_ServerId;        // ���������

};
















#endif