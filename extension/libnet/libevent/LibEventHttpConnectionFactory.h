//--------------------------------------------------------------------
// �ļ���:		LibEventHttpConnectionFactory.h
// ��  ��:		�������Ӵ�������
// ˵  ��:		
// ��������:		2016��5��19��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __NET_LIBEVENT_HTTP_CONNECTION_CREATOR_H__
#define __NET_LIBEVENT_HTTP_CONNECTION_CREATOR_H__

#include "LibEventHttpConnection.h"

class LibEventHttpConnectionFactory : public INetConnectionFactory
{
public:

	virtual INetConnection* CreateConnection(size_t nSndBuf, size_t nRecvBuf, bool bIsAccept)
	{
		LibEventHttpConnection* conn = new LibEventHttpConnection();
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

#endif // END LibEventHttpConnectionFactory