//--------------------------------------------------------------------
// �ļ���:		SoloCharge.h
// ��  ��:		�Ʒѵ�½��ʵ������
// ˵  ��:		
// ��������:		2016��5��18��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __ENT_SOLO_CHARGE_H__
#define __ENT_SOLO_CHARGE_H__

#include "EntNetBase.h"
#include "NetWorldAction.h"
#include "MailData.h"
#include "NoticeInfo.h"
#include "CmdInfo.h"
#include "ExchGiftInfo.h"
#include "NetGmccMessagePacker.h"
#include "PayedOrder.h"
#include "PlayerData.h"
#include "ServerInfo.h"
#include "ShopItem.h"
#include "ProtocolsID.h"

class EntHttp;
class EntRpc;
struct ServerInfo;
class SoloGmcc : public EntNetBase
{
public:
	SoloGmcc();
	virtual bool Init(const IVarList& args) override;
	virtual bool Shut() override;

	// ��������
	bool Startup();
	// ֹͣ����
	bool Stop();
	virtual void Execute(float seconds);
	virtual int GetNetKey() const;

	void SetGameName(const char* pszName);
	const char* GetGameName() const;

	void SetHeartbeat(int nHeartbeat);
	int GetHeartbeat();
protected:
	virtual void OnInit();
	virtual void OnConnection(const SERID& serid, const SESSID& sessid, std::shared_ptr<INetAction>& pAction);
	virtual void OnClose(const SERID& serid, const SESSID& sessid, std::shared_ptr<INetAction>& pAction);

	// ע��
	void OnActionServerRegister(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ����
	void OnActionKeep(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ������Ϣ
	void OnAction(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ��Ϸ��������Ӧȷ����Ϣ
	void OnGameAck(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ��Ϸ��������������
	void OnGameReady(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ��Ϸ�����������Ϣ
	void OnGameReqSrvInfo(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// �һ���Ʒ��
	void OnExchGift(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ͬ������֪ͨ
	void OnSyncNotice(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ��ѯ����������ݷ���
	void OnQueryPropData(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ��ѯ��ұ����ݷ���
	void OnQueryRecordData(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ����֪ͨ
	void OnPushNotification(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ��Ʒ��Ϣ����
	void OnShopItemList(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ����״̬ȷ��
	void OnOrderNotify(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// �ͻ����µ�
	void OnClientOrder(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ��ѯ����
	void OnGameQueryRebate(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ��ȡ�����ɹ�
	void OnGamePickedRebate(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// �������ϱ�����
	void OnReportOnlineCount(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);


	//----------------------------------------------------------------------------------------
	// world server ��Ӧ��Ϣ
	//----------------------------------------------------------------------------------------
	// gmִ�л�Ӧ
	void OnGmResult(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// �ʼ����
	void OnMailResult(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ֪ͨ���
	void OnNoticeResult(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
public:
	//----------------------------------------------------------------------------------------
	// ���⹦�ܽӿ�
	//----------------------------------------------------------------------------------------
	// ��ʼ��profile�ɹ�
	void OnProfileInited(const SrvInfo& info);
	/*@function:��ϷGM(�߳����, ����.....)
	*/
	void SendGameCommand(const CmdInfo& cmd);
	/*@function:�����ʼ�
	*/
	void SendMail(const MailData& mail);
	/*@function:���͹���
	*/
	void SendNotice( const NoticeData& notice );
	// ɾ��֪ͨ
	void DeleteNotice(int64_t nNoticeId);
	/*@function:�������
	*/
	void SendExchGiftItems(const ExchGiftResult& exchResult);

	// ������֪ͨ
	void SendPayment(const Payment& payment);
	void SendProductOrder(const ProductOderInfo& order);
	void SendProducts(const std::vector<Product>& products);
	// ��ֵ����
	void SendPayedRebateInfo(const std::string rid, int nType, int nRebate);
	// ���ͳ�ֵ���
	void SendPayedRebateResult(const std::string rid, int nResult, int nRebate);

	// ��ѯ�������
	bool QueryPlayerProps(const CmdPropQuery& query);
	// ��ѯ������
	bool QueryPlayerRecordData(const CmdRecordQuery& query);

	//-------------------------------------------------
	// �̳���Ϣ
	//-------------------------------------------------
	bool ShopPushItem(const std::wstring& name, const std::wstring& desc, int64 startTime, int64 endTime, 
									const std::vector<ShopTag>& catalogs, const std::vector<ShopItem>& items);
	bool ShopRemoveItem(const std::vector<std::string>& removes);
	bool ShopQueryItem();

	// ���û����
	void SetActivityConfig(ActivityInfo data);
	// ����������Ϣ
	void SendLoadConfigInfo(const std::vector<ConfigLoadInfo>& cfgInfo);
protected:
	void OnInnerGameReady();
	void SendSrvInfo();
	void CheckGameReady();
protected:
//#define TEST
#ifdef TEST
	NetTextMessagePacker m_packer;
#else
	NetGmccMessagePacker m_packer;
#endif

	EntHttp* m_pHttpService;
	EntRpc* m_pRpcService;
	ServerInfo* m_pWorldSrv;

	time_t	m_tcheckTime;
	SrvInfo m_srvInfo;
	bool	m_bSelfReady;
	bool	m_bGameReady;

	std::string m_strGameName;
	int		m_nHeatbeat;		// �������(s)

	// ��Ҫȷ�ϵ���Ϣ
	struct PendingInfo
	{
		std::shared_ptr<INetAction> action;		// �����е�action
		float elapsed;						// ����ʱ��
	};

	std::map<uint64_t, PendingInfo> m_mapPendings;
	uint64_t m_nSeq;								// ������
};

#endif // END __ENT_SOLO_CHARGE_H__