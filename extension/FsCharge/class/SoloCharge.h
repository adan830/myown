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
#include "action\IHttpAction.h"
#include "ServerManager.h"
#include <set>
#define LOGIN_ERROR "cas:authenticationFailure"
#define LOGIN_SUCCESS "cas:authenticationSuccess"
#define LOGIN_ATTRIBUTES "cas:attributes"
#define LOGIN_WHITELISTLIMIT "cas:whiteListLimit"
#define LOGIN_SERVER_LIMIT "cas:openTimeLimit"
#define LOGIN_CAS_NAME "cas:name"

#define LOGIN_TIMER_OVER 300
#ifdef ANALY_BY_USER
class SensorsAnalyticsCollector;
typedef SensorsAnalyticsCollector GameAnalytics;
#else
class SensorsAnalyticsCollectorEx;
typedef SensorsAnalyticsCollectorEx GameAnalytics;
#endif

class EntHttpService;
class SoloCharge : public EntNetBase
{
public:
	SoloCharge();
	virtual bool Init(const IVarList& args) override;
	virtual bool Shut() override;

	// ��������
	bool Startup();
	// ֹͣ����
	bool Stop();
	virtual void Execute(float seconds);

	virtual EMPackResult Pack(INetConnection* conn, FsIStream& stream, std::shared_ptr<INetAction>& action, size_t* nOutLen = NULL) const;
	virtual EMPackResult UnPack(INetConnection* conn, FsIStream& stream, std::shared_ptr<INetAction>& action, size_t* nOutLen = NULL) const;

	virtual int GetNetKey() const;

public:
	unsigned int GetClientVersion() const { return m_nClientVersion;  }
	void SetClientVersion(unsigned int nVersion) { m_nClientVersion = nVersion; }

	const char* GetAppId() const { return m_strAppId.c_str(); }
	void SetAppId(const char* pszAppId) { if (NULL != pszAppId) m_strAppId = pszAppId; }

	bool GetVerify( ) const { return m_bEnableVerify; }
	void SetVerify(bool bEnable) { m_bEnableVerify = bEnable; }

	// δ����˺ŵ�½
	bool GetAccIncompleted() const { return m_bAccInCompleted; }
	void SetAccIncompleted(bool bEnable) { m_bAccInCompleted = bEnable; }

	// ���õ�½����֤���û�����Ч����
	void SetNoVerifyUser(const char* pszAcc, int nValidNum);
	void ClearNoVerifyUser();
	std::string DumpNoVerifyUser();
protected:
	virtual void OnInit();
	//virtual void OnConnection(const SERID& serid, const SESSID& sessid, std::shared_ptr<INetAction>& pAction);
	//virtual void OnClose(const SERID& serid, const SESSID& sessid, std::shared_ptr<INetAction>& pAction);


	// ע��
	void OnActionServerRegister(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ��½
	void OnActionLogin(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// �˳�
	void OnActionLogout(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ����
	void OnActionKeep(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// �Զ�����Ϣ
	void OnCustom(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	// ������Ϣ
	void OnAction(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	//ɾ����ʱ����
	void DelOverTimerReq();
private:
	void OnHandleGameReady(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action, NetWorldMessage_Custom* pReq, size_t nArgsOffset);
	//void RequestGameState();
	int CheckClientVersion(unsigned int nClientVersion) const;
public:
	void OnLoginResult(const char*name,int isResult,const char* verifyStr);
	

protected:
//#define TEST
#ifdef TEST
	NetTextMessagePacker m_packer;
#else
	NetWorldMessagePacker m_packer;
#endif
	struct LogicReq
	{
		SESSID							m_sessid;
		std::shared_ptr< INetAction >	m_action;
		time_t							m_timestamp;
	};


	// ��½������
	struct LoginStringInfo
	{
		std::string strLoginString;		// ��½����Ϣ
		time_t		tExpiredTime;		// ����ʱ��
	};

	EntHttpService* m_pHttpService;
	// �߼�����<����, ������Ϣ>
	typedef std::map<std::string, LogicReq> USERLOGICREQ;
	std::map<std::string,LogicReq>  m_userLogicReq;

	// ��½�е���ң������ظ�����
	std::set<std::string>		m_userPendding;	

	typedef std::map<std::string, LoginStringInfo> MapLoginStringCache;
	MapLoginStringCache m_logonStringChche;

	// �����־
	GameAnalytics* m_pAnaCollector;

	//ServerInfo* m_pWorldSrv;
	//time_t m_tcheckTime;
	bool			m_bGameReady;
	unsigned int	m_verifyKey;
	time_t			m_nThreshold;
	unsigned int	m_nClientVersion;		// ����ͨ������Ϳͻ��˰汾

	// �ͻ�������web��ʶ
	std::string		m_strAppId;

	// ����֤�û�<�û����� ʹ�ô���>
	std::map<std::string, int>	m_mapNoVeifyUsers;
	// �Ƿ������˺���֤����(Ĭ��Ϊtrue, Ϊfalseʱ������֤���ܣ��κε�½�����سɹ�)
	bool			m_bEnableVerify;

	// �Ƿ�����δ��ɵ��˺��ظ���½
	bool			m_bAccInCompleted;
};

#endif // END __ENT_SOLO_CHARGE_H__