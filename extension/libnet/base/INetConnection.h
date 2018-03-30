//--------------------------------------------------------------------
// �ļ���:		INetConnection.h
// ��  ��:		�������Ӷ�����
// ˵  ��:		
// ��������:		2016��5��19��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
// �޸����ڣ�		2016��06��07��
// �޸�������		�淶���ӻỰ�ӿ�ֻ������������� �������ݲ�����Ӧ���첽post�������̴߳���, ����ֱ�Ӵ����ڲ����
//--------------------------------------------------------------------
#ifndef __INET_CONNECTION_H__
#define __INET_CONNECTION_H__

#include "INetType.h"
#include "action/INetAction.h"

enum EMNetReason
{
	EM_NET_REASON_SELF_DISCONNECT,			// �Լ������Ͽ�
	EM_NET_REASON_PEER_DISCONNECT,			// �Է������Ͽ�
	EM_NET_REASON_SOCKET_ERRROR,			// ��������Ͽ�
	EM_NET_REASON_SND_BUFFER_NO_ENGTH,		// ���ͻ�������С
	EM_NET_REASON_RCV_BUFFER_NO_ENGTH,		// ���ջ�������С
	EM_NET_REASON_PARAM_ERRROR,				// ��������������
	EM_NET_REASON_LOGIC_ERRROR,				// �߼�����
	EM_NET_REASON_CONNECT_FAILED,			// ����ʧ��
};

enum EMConnState
{
	EM_CONN_STATE_NOT_CONNECTED,			// δ����
	EM_CONN_STATE_CONNECTING,				// ������
	EM_CONN_STATE_CONNECTED,				// ���ӳɹ�
	EM_CONN_STATE_CLOSING,					// ���ڹر���
	EM_CONN_STATE_CLOSED,					// �ѹر�
};

class INetService;
class INetConnection
{
public:
	virtual ~INetConnection() = 0;
	// ����id
	virtual NID GetConnId() const = 0;
	// ���Ӿ��
	virtual NETFD FD() const = 0;

	// �����ر�����
	virtual bool Close(EMNetReason reason = EM_NET_REASON_PEER_DISCONNECT) = 0;
	// �Ƿ�������
	virtual bool IsClosed() const = 0;
	// �Ƿ�����
	virtual bool IsConnected() const = 0;
	// ���õ�ǰ״̬
	virtual void SetState(EMConnState state) = 0;
	// ��ȡ��ǰ״̬
	virtual EMConnState GetState() const = 0;
	// �Ƿ��������
	virtual bool IsAccept() const = 0;
	// �Ƿ񱻶��ر�
	virtual bool IsPassiveClosed() const = 0;
	// �Ƿ����ӳɹ���
	virtual bool IsSucceed() const = 0;

	// �Զ�ip
	virtual const char* RemoteIP() const = 0;
	// �Զ˶˿�
	virtual unsigned short RemotePort() const = 0;
	// �Ͽ�ԭ��
	virtual EMNetReason DiedReason() const = 0;
	// �����ķ���
	virtual INetService* GetService() const = 0; 
	
	// Ӧ�ò��Զ�������
	virtual int GetUD() const = 0;
	virtual void  SetUD(int ud) = 0;

	// ��������
	virtual bool Send(const char* data, size_t nLen) = 0;
};
inline INetConnection::~INetConnection(){}

class INetConnectionFactory
{
public:
	virtual ~INetConnectionFactory() = 0;
	virtual INetConnection* CreateConnection( size_t nSndBuf, size_t nRecvBuf, bool bIsAccept ) = 0;
	virtual void FreeConnection(INetConnection* conn) = 0;
};
inline INetConnectionFactory::~INetConnectionFactory(){}

#endif // END __INET_CONNECTION_H__