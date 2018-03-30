//--------------------------------------------------------------------
// �ļ���:		LibEventHttpConnection.h
// ��  ��:		libevent http�������Ӷ���
// ˵  ��:		
// ��������:		2016��5��26��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __NET_LIBEVENT_HTTP_CONNECTION_H__
#define __NET_LIBEVENT_HTTP_CONNECTION_H__

#include "base/NetConnection.h"
#include "public/FastStr.h"
#include "action/IHttpAction.h"
#include "event2/http.h"
#include "event2/buffer.h"

class LibEventHttpConnection : public NetConnection
{
public:
	typedef struct evhttp_request* request_ptr;
	typedef std::map<request_ptr, std::shared_ptr<INetAction>> RequestList;
protected:

	struct evhttp_connection*	m_httpConnection;
	struct event*				m_pNotify;

	// δ��������
	RequestList					m_requests;	
	bool m_bIsSSL;
public:
	LibEventHttpConnection();

	bool Initilize(struct evhttp_connection* phttp, struct event* pNotify);

	// ������յ�����
	void OnRequest(request_ptr request, std::shared_ptr<INetAction>& action);
	// �ͻ����յ���������Ӧ
	void OnResponse(request_ptr req);

	// ��ȡ�����Ӷ���
	struct evhttp_connection* GetHttpContext() const
	{
		return m_httpConnection;
	}

	void SetSSL(bool ssl)
	{
		m_bIsSSL;
	}

	bool IsSSL() const
	{
		return m_bIsSSL;
	}

	virtual bool Send(std::shared_ptr<INetAction>& action);
	virtual bool Reply(std::shared_ptr<INetAction>& action);

	virtual bool Close(EMNetReason reason /* = EM_NET_REASON_PEER_DISCONNECT */);
	virtual void OnClosed(EMNetReason reason, int nErrCode);

	size_t GetReqLength()
	{
		return m_requests.size();
	}
protected:
	virtual void PostSend(){ assert(false); };
private:
	void SetDefaultHeader(IHttpRequest* req,
			struct evkeyvalq* header,
			const char* key,
			const char* defValue)
	{
		const char* value = req->GetHeader(key);
		if (NULL == value || value[0] == 0)
		{
			evhttp_add_header(header, key, defValue);
		}
	}
};

#endif // END __NET_LIBEVENT_HTTP_CONNECTION_H__