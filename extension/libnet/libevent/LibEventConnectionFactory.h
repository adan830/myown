//--------------------------------------------------------------------
// �ļ���:		LibEventConnectionFactory.h
// ��  ��:		�������Ӵ�������
// ˵  ��:		
// ��������:		2016��5��19��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __NET_LIBEVENT_CONNECTION_CREATOR_H__
#define __NET_LIBEVENT_CONNECTION_CREATOR_H__

#include "LibEventConnection.h"

class LibEventConnectionFactory : public INetConnectionFactory
{
public:

	virtual INetConnection* CreateConnection(size_t nSndBuf, size_t nRecvBuf, bool bIsAccept)
	{
		LibEventConnection* conn = new LibEventConnection( nSndBuf, nRecvBuf );
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

#endif // END LibEventConnectionFactory