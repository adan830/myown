//--------------------------------------------------------------------
// �ļ���:		NetConnection.h
// ��  ��:		�������Ӷ���
// ˵  ��:		
// ��������:		2016��5��19��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __NET_CONNECTION_H__
#define __NET_CONNECTION_H__

#include "INetConnection.h"
#include "public/FastStr.h"
#include <map>
#include <string>
#include "utils/FsSpinLock.h"
#include "DataBuffer.h"

// ��ֻ֤���������̶߳���
class NetConnection : public INetConnection
{
protected:
	INetService*	m_pService;
	NID				m_nId;

	NETFD			m_nFD;		// ���
	int				m_ud;		// �û�����

	TFastStr<char, 16>	m_strHost;	// ip
	unsigned short		m_nPort;	// �˿�

	DataBuffer		m_dbSend;		// ���ͻ�����
	DataBuffer		m_dbRecv;		// ���ջ�����

	EMConnState m_eState;	// ����״̬
	EMNetReason m_eReason;	// �Ͽ�ԭ��

	bool		m_bIsAccept;		// �Ƿ��Ƿ���������
	bool		m_bIsPassived;		// �����ر�
	bool		m_bSucceed;			// �Ƿ�ɹ����ӹ�

	bool		m_bIsSending;
public:
	NetConnection(size_t nSndBuf, size_t nReadBuf);

	NetConnection& SetConnId(NID id)
	{
		m_nId = id;
		return *this;
	}
	virtual NID GetConnId() const
	{
		return m_nId;
	}

	NetConnection& SetFD(const NETFD& fd)
	{
		m_nFD = fd;
		return *this;
	}
	virtual NETFD FD() const
	{
		return m_nFD;
	}


	NetConnection& SetAccept( bool bAccept )
	{
		m_bIsAccept = bAccept;
		return *this;
	}
	virtual bool IsAccept() const
	{
		return m_bIsAccept;
	}
	// �Ƿ񱻶��ر�
	virtual bool IsPassiveClosed() const
	{
		return m_bIsPassived;
	}
	// �Ƿ�������
	virtual bool IsClosed() const
	{
		return m_eState == EM_CONN_STATE_CLOSED;
	}
	// �Ƿ�����
	virtual bool IsConnected() const
	{
		return m_eState == EM_CONN_STATE_CONNECTED;
	}
	// ����״̬
	virtual void SetState(EMConnState state)
	{
		m_eState = state;
		if (m_eState == EM_CONN_STATE_CONNECTED)
		{
			m_bSucceed = true;
		}
	}
	// ��ȡ״̬
	virtual EMConnState GetState() const
	{
		return m_eState;
	}
	// �Ƿ����ӳɹ���
	virtual bool IsSucceed() const
	{
		return m_bSucceed;
	}

	NetConnection& SetRemotIP( const char* ip )
	{
		m_strHost = NULL == ip ? "" : ip;
		return *this;
	}
	virtual const char* RemoteIP() const
	{
		return m_strHost.c_str();
	}

	NetConnection& SetRemotePort(unsigned short nPort)
	{
		m_nPort = nPort;
		return *this;
	}
	virtual unsigned short RemotePort() const
	{
		return m_nPort;
	}

	virtual EMNetReason DiedReason() const
	{
		return m_eReason;
	}

	virtual INetService* GetService() const
	{
		return m_pService;
	}

	virtual NetConnection& SetService(INetService* pService)
	{
		m_pService = pService;
		return *this;
	}

	// Ӧ�ò��Զ�������
	virtual int GetUD() const
	{
		return m_ud;
	}
	virtual void  SetUD(int ud)
	{
		m_ud = ud;
	}

	virtual bool Send(const char* szMsg, size_t nLen);

	virtual void Reset();

	// ���ͻص�
	virtual void OnSend(const char* data, size_t nLen);
	// �յ�����
	virtual bool OnRecv(const char* data, size_t nLen);

	// ���ӳɹ��ص�
	virtual void OnConnected();
	// �Ѿ��رջص�
	virtual void OnClosed( EMNetReason reason, int nErrorCode );

	// ����ر�
	virtual bool Close(EMNetReason reason = EM_NET_REASON_PEER_DISCONNECT);

	DataBuffer& GetRecvBufer()
	{
		return m_dbRecv;
	}

	DataBuffer& GetSendBuffer()
	{
		return m_dbSend;
	}

protected:
	virtual void PostSend() = 0;
	//{
	//	char*	pb = NULL;
	//	size_t	nSize = 0;
	//	bool bExp = false;
	//	if ( m_bIsSending.compare_exchange_strong(bExp, true) )
	//	{
	//		LockGuard<SpinLock> guard(m_senderLocker);
	//		pb = m_dbSend.GetBuffer() + m_dbSend.GetReadPos();
	//		nSize = m_dbSend.GetValidSize();
	//		if (nSize <= 0)
	//			m_bIsSending.exchange(false);
	//	}

	//	if ( pb != NULL && nSize > 0 )
	//	{
	//		RealSend( pb, nSize );
	//	}
	//}

	//virtual bool RealSend(const char* data, size_t nLen)
	//{
	//	OnSend( data, nLen );
	//	return true;
	//}
};

#endif // END __NET_CONNECTION_H__