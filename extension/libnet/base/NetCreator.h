//--------------------------------------------------------------------
// �ļ���:		NetCreator.h
// ��  ��:		���紴����
// ˵  ��:		
// ��������:		2016��5��20��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __NET_CREATOR_H__
#define __NET_CREATOR_H__

#include "INetService.h"

class NetCreator
{
public:
	static INetService* Create( INetIoDesc* pDesc, bool bHttp = false );
	static void Free(INetService* service);

	static INetConnectionFactory* CreateConnectionFactory(bool bHttp = false);
	static void FreeConnectionFactory(INetConnectionFactory* factory);
};

#endif // END __NET_CREATOR_H__