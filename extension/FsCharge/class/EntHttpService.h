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

class SoloCharge;
class EntHttpService : public EntNetBase
{
public:
	EntHttpService();
	virtual bool Init(const IVarList& args) override;
	virtual bool Shut() override;

	virtual bool Startup() override;
	virtual bool Stop() override;

	void SetLoginVerifyUrl( const char* url )
	{
		m_strLoginVerifyUrl = url;
	};
	const char* GetLoginVerifyUrl() const
	{
		return m_strLoginVerifyUrl.c_str();
	}

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

	void AddPostUser(const char* pszUserPost);
	void RemovePostUser(const char* pszUserPost);
	void ClearPostUser();
	std::string DumpPostUser();
public:
	virtual void OnAction(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);
	virtual void OnReply(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);

	virtual int GetNetKey() const;

protected:
	virtual INetIoDesc* GetIoDesc();

	virtual void OnInit();
	void DumpAction(std::shared_ptr<INetAction>& action);
public:
	//��½
	void OnGameLogin(const char*token, const char*name, const char*verifyStr, int64_t gameId, int64_t nAreaId, const char* pszIP, const char* pszAppid);
private:
	// ��½����
	void OnRspGameLogin( std::shared_ptr<INetAction>& action );
private:
	SoloCharge* m_pMainEntity;
	std::string m_strLoginVerifyUrl;

	std::vector<std::string> m_vecAllowPostUser;

	int m_nReqReuseNum;
	int m_nReqRetries;
};

#endif // END __INT_HTTP_SERVER_H__