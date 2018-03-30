//--------------------------------------------------------------------
// �ļ���:		EntHttpServer.h
// ��  ��:		http������ʵ��
// ˵  ��:		
// ��������:		2016��5��18��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __ENT_HTTP_SERVER_H__
#define __ENT_HTTP_SERVER_H__

#include "EntNetBase.h"
#include "NoticeInfo.h"
#include "ExchGiftInfo.h"
#include "PayedOrder.h"
#include "PushData.h"
#include "utils/json.h"
#include "cpptempl.h"
#include "action/IHttpAction.h"

class SoloGmcc;
class EntRpc;

struct config_info
{
	std::string name;			// ����
	std::string key;			// ����key
	int32_t		catalog;		// ���÷���
	int32_t		cfgid;			// ������id
	std::string path;			// �����ļ�����·��
	std::string tplpath;		// ģ������·��
};

struct tpl_info
{
	cpptempl::auto_data data;
	const config_info* cfg;
	int64_t		file_version;
};

class EntHttp : public EntNetBase
{
public:
	EntHttp();

	virtual bool Init(const IVarList& args) override;
	virtual bool Shut() override;

	virtual bool Startup() override;
	virtual bool Stop() override;
	virtual void Execute(float seconds);

	virtual int GetNetKey() const;

	// ������ز���
	const char* GetPushUrl() const;
	void SetPushUrl( const char* url );
	const char* GetPushKey() const;
	void SetPushKey(const char* key);
	const char* GetPushSecret() const;
	void SetPushSecret(const char* secret);
	const char* GetPushSound() const;
	void SetPushSound(const char* sound);
	int GetPushBuilderId() const;
	void SetPushBuilderId(int nBuilderId);
	int GetPushTimeLive() const;
	void SetPushTimeLive(int nValue);
	bool IsProduction() const;
	void SetProduction(bool bProduction);

	// �������Դ���
	void SetReqResueNum(int nNum);
	int GetReqResueNum();
	// ����������
	void SetReqRetries(int nNum);
	int GetReqRetries();

	// Ĭ������ͷ(���÷���������Ч)
	void AddReqDefHeader(const char* pszKey, const char* pszValue);
	void RemoveReqDefHeader(const char* pszKey);
	void ClearReqDefHeaders();

protected:
	virtual INetIoDesc* GetIoDesc();

	virtual void OnAction(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	virtual void OnReply(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ��ʼ��
	bool OnRspProfile(std::shared_ptr<INetAction>& action);
	// ͨ��
	bool OnRspNotice(std::shared_ptr<INetAction>& action);
	// �һ���
	bool OnRspExchGift(std::shared_ptr<INetAction>& action);
	// ��Ʒ�б�
	bool OnRspProducts(std::shared_ptr<INetAction>& action);
	// �µ�
	bool OnRspPreOrder(std::shared_ptr<INetAction>& action);
	// ����״̬
	bool OnRspOrdeState(std::shared_ptr<INetAction>& action);
	// ��ֵ����
	bool OnRspRebateInfo(std::shared_ptr<INetAction>& action);
	// ��ֵ�������
	bool OnRspRebateResult(std::shared_ptr<INetAction>& action);
	// ����ȷ��
	bool OnRspConfirmCmd(std::shared_ptr<INetAction>& action);
	// ����
	bool OnRspPushNotification(std::shared_ptr<INetAction>& action);
	// �����
	bool OnRspConfig(std::shared_ptr<INetAction>& action);
	// �̳�����
	bool OnRspMallItems(std::shared_ptr<INetAction>& action);
	// �ʼ�ȷ��
	bool OnRspConfirmMail(std::shared_ptr<INetAction>& action);

private:
	//void OnSetShopItemList(Json::Value& root);
	void OnSetLevelingConfig(Json::Value& root);
public:
	// ��ʼ��Http����
	void OnReqProfile();
	// �һ�����
	void OnReqExchGift( const ExchGiftInfo& info );
	// ����ָ����֪ͨ
	void OnReqNotify(const NotifyInfo& notify);
	// ����������Ч��֪ͨ����
	void OnReqAllData();
	// ����������Ʒ�б�
	void OnReqProducts();
	// ֪ͨ����״̬
	void OnNotifyOrder( const OrderNotify& notify );
	// �µ�����
	void OnReqPreOrder(const ProductOrder& order);
	// ����֪ͨ
	void OnReqPushNotification(const PushData& data);
	// ��������
	void OnReqConfig( int nType );
	// �����̳��б�
	void OnReqMallItems();
	// �ʼ�ȷ��
	void OnReqConfirmMail(int64_t nMailId, int32_t nResult, bool bIsGlobal, const std::map<std::string, int32_t>& resultMap);
	// gm���ȷ��
	void OnReqConfirmGm(int64_t nCmdId, int32_t nResult, const std::string& roleid, const std::string& message);
	// ��ѯ��ֵ����
	void OnReqPayedRebate(const std::string& uid, const std::string& rid);
	// ��ȡ��ֵ����
	void OnReqPayedRebatePick(const std::string& uid, const std::string& rid, const std::wstring& name );
protected:
	void RequestNotifyData( const NotifyInfo* pNotify );
	bool LoadConfig();
	// bAdaCataLog: nCfgIdδ�ҵ�ʱ��ֻȡ������Ϣ
	const config_info* GetConfigData(int32_t nCata, int32_t nCfgId, bool bAdaCataLog = true) const;
	// ����ģ������
	bool ParseConfigTplData(int32_t nCata, const Json::Value& root, std::map<int64_t, tpl_info>& tpldatas, int64_t version) const;
	// ����json����
	bool JsonToTplData(const Json::Value& root, cpptempl::auto_data& data) const;
	// ����ģ������
	void HandleConfigTplData(const std::map<int64_t, tpl_info>& tpldatas) const;
protected:
	// url
	struct RequestInfo
	{
		std::string url;
		int			method;
	};

	virtual void OnInit();

	void FillPushData(Json::Value& root, const PushData& data) const;

	void DumpAction( std::shared_ptr<INetAction>& action );
	// �ط�����
	bool RepRequest( std::shared_ptr<INetAction>& action );

	bool ParseGoodsItem(const Json::Value& items, std::vector<GoodsItem>& goods);

	int HandleCommonRsp(IHttpRequest* req, IHttpResponse* res, Json::Reader& reader, Json::Value& root, Json::Value** ppdata, int& nHttpCode, const char* pszFuncName);
	bool ConfigRequestUrl();
	bool ParseRequest(RequestInfo& req, const char* pszUrl);
	IHttpRequest* InitRequest(const RequestInfo& req, std::shared_ptr<INetAction>& action, int nTag, int nTimeOut);
private:
	SoloGmcc*		m_pMainEntity;
	EntRpc*			m_pRpcService;

	// profile��Ϣ
	std::string m_strGameid;
	std::string m_strAreaId;

	RequestInfo m_urlProfile;				// ��ʼ��
	RequestInfo m_urlNotice;				// ֪ͨ
	RequestInfo m_urlGift;					// �һ���
	RequestInfo m_urlAcitvity;				// �ճ��
	RequestInfo m_urlMall;					// �̳�
	RequestInfo m_urlMailConfirm;			// �ʼ�ȷ��
	RequestInfo m_urlCmdConfirm;			// ����ȷ��
	RequestInfo m_urlGoods;					// ��Ʒ�б�
	RequestInfo m_urlPreOrder;				// �µ�����
	RequestInfo m_urlOrderState;			// ����״̬�޸�
	RequestInfo m_urlRebateInfo;			// ������Ϣ
	RequestInfo m_urlRebatePick;			// ��ȡ����

	// ���Ͳ���
	std::string m_strUrlPush;			// ���͵�ַ
	std::string m_strPushKey;			// ���ͷ�������key
	std::string m_strPushSecret;		// ���ͷ���������Կ
	std::string m_strPushAuthString;	// ���ͷ�����֤��(base64), ��appkey:secret����һ�η��Ͳ���
	std::string m_strDefaultSound;		// Ĭ������
	int			m_nDefaultBuilderId;	// Ĭ����ʾ��ʽ

	int			m_nTimeToLive;			// ��Чʱ��(0:ʹ��Ĭ�ϵ�)
	bool		m_bEnvProduction;		// ��������(trueΪ��ʽ����)

	// ������Ϣ��
	struct ResendAction
	{
		std::shared_ptr<INetAction> action;
		float						elapsed;
	};
	std::map<INetAction*, ResendAction> m_cacheActions;
	// ����ģ��ӳ����Ϣ
	std::map<int64_t, config_info>	m_mapConfigs;

	// �����ļ�id��keyӳ�䣬����ʱת��ʹ��
	std::map<int, std::string> m_mapConfigIds;
	int m_nReqReuseNum;
	int m_nReqRetries;
};

#endif // END __INT_HTTP_SERVER_H__