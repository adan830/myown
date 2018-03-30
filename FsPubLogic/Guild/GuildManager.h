//--------------------------------------------------------------------
// �ļ���:		GuildManager.h
// ��  ��:		����ϵͳ������
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:		 
//    :	    
//--------------------------------------------------------------------

#ifndef __PubGuildManager_h__
#define __PubGuildManager_h__

#include "../Public/IPubLogic.h"
#include "../../server/IKernel.h"
#include "../../system/WinPortable.h"
#include "../../FsGame/Define/GuildDefine.h"
class GuildBase;


class PubGuildManager : public IPubLogic
{
public:
	PubGuildManager();
	virtual ~PubGuildManager();
	static IPubLogic* Instance();
	// �������ݷ���������
	virtual int OnPublicStart(IPubKernel* pPubKernel);
	// �������ݷ������ر�
	virtual int OnServerClose(IPubKernel* pPubKernel);
	// �������ݿռ��е�������������
	virtual int OnPubSpaceLoaded(IPubKernel* pPubKernel);
	// ����������������
	virtual int OnPubDataLoaded(IPubKernel* pPubKernel, const wchar_t* data_name);
	// ����������������Ϣ
	virtual int OnPublicMessage(IPubKernel* pPubKernel, int source_id, 
		int scene_id, const IVarList& msg);
	const char* GetName();
	// ���ü�ʱ��
	void ResetGuildTimer(IPubKernel* pPubKernel, const port_date_time_t& now);
	//�ص���������
	bool CallBackAllSub(std::function<bool(GuildBase*)> func);

	//�ص�������֯
	bool CallBackAllGuild(std::function<bool(IPubData*)>);

	// ��ᶨʱ�ص�ˢ��
	void UpdateGuildTimer(IPubKernel* pPubKernel);

	// ��ȡ�������
	static IPubData* GetGuildPubData(IPubKernel* pPubKernel, const wchar_t* guildname);
private:
	
    static void ShopResetTimerCb(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name);
   
	///////////////////////////////////////////////////////////////////////////
	// ��������������Ϣ����
	///////////////////////////////////////////////////////////////////////////
	// �����򹫻���Ʒ���۳����˹��׶�
	int OnBuyShopItem(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args);
	bool PubGuildManager::OnPublicCommand(IPubKernel* pPubKernel, int cmd_id, const IVarList& args);
	bool PubGuildManager::OnFilterMessage(IPubKernel* pPubKernel, const wchar_t* guildname, int msgid, int source_id, int scene_id, const IVarList& args);
	// ֪ͨ������Ʒ�۳����˹���ֵ���
	void SendBuyItemResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
		const wchar_t* guildName, const wchar_t* playerName, int result, int index, const char* itemId, int itemNum, int consume);
	//�����������
	void AddGuildSub(GuildBase* pBase);
	//��ʼ����ʱ��
	bool InitTimer(IPubKernel* pPubKernel);
	static int GuildTimerCallback(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name, int time);
	//��֯��Ϣ����
	static void GuildPubReset(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name);
public:
	static PubGuildManager* m_pInstance;
	std::wstring m_ServerId;        // ���������
	IPubSpace* m_pPubSpace;         // ���ṫ��������
	IPubData* m_pGuildPubData;      // ���ṫ��������
    GuildTimerCallbackVector  m_GuildTimer; // ����������ʱ��
	GuildBossLevelConfig m_guildBossLevelConfig;	//����boss����	
	std::vector<GuildBase*>  m_guildSubManage;
	
};

#endif //__PubGuildManager_h__