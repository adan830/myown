//--------------------------------------------------------------------
// �ļ���:		NetConnectionCreator.h
// ��  ��:		�������Ӵ�������
// ˵  ��:		
// ��������:		2016��5��19��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __NET_CONNECTION_CREATOR_H__
#define __NET_CONNECTION_CREATOR_H__

#include "NetConnection.h"

template<typename T>
class NetConnectionFactory : public INetConnectionFactory
{
public:

	virtual INetConnection* CreateConnection(size_t nSndBuf, size_t nRecvBuf, bool bIsAccept)
	{
		NetConnection* conn = new T( nSndBuf, nRecvBuf );
		conn->SetAccept(bIsAccept);
		return conn;
	}

	virtual void FreeConnection(INetConnection* conn)
	{
		if (NULL != conn)
		{
			delete conn;
		}
	}
};

#endif // END NetConnectionCreator