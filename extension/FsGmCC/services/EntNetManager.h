//--------------------------------------------------------------------
// �ļ���:		EntNetManager.h
// ��  ��:		�Ʒ�������������
// ˵  ��:		
// ��������:		2016��5��20��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __ENT_NET_MANAGER_H__
#define __ENT_NET_MANAGER_H__

#include "base/NetManager.h"
#include "base/NetServiceCreator.h"

enum EMNetKey
{
	EM_NET_KEY_INVALID,

	EM_NET_KEY_TCP = 1,		
	EM_NET_KEY_HTTP,
	EM_NET_KEY_ECHO,
	EM_NET_KEY_RPC,

	EM_NET_KEY_MAX
};

class EntNetManager : public NetManager
{
private:
	NetTcpCreator	m_tcpCreator;
	NetHttpCreator	m_httpCreator;
	EntNetManager() : NetManager()
	{
		this->AddCreator(EM_NET_KEY_TCP, &m_tcpCreator);
		this->AddCreator(EM_NET_KEY_HTTP, &m_httpCreator);
		this->AddCreator(EM_NET_KEY_ECHO, &m_tcpCreator);
	}

	EntNetManager(const EntNetManager& other) = delete;
	EntNetManager(EntNetManager&& other) = delete;
	EntNetManager& operator=(const EntNetManager& other) = delete;
	EntNetManager& operator=(EntNetManager&& other) = delete;

public:
	static EntNetManager& Instance()
	{
		static EntNetManager s_Inst;
		return s_Inst;
	}
	~EntNetManager()
	{
		for (int i = EM_NET_KEY_INVALID + 1; i < EM_NET_KEY_MAX; ++i)
		{
			StopServer(i);
			RemoveServer(i);

			INetServiceCreator* creator = LookupCreator(i);
			if (NULL != creator)
			{
				RemoveCreator(i);
			}
		}
	}
};
#endif // END __ENT_NET_MANAGER_H__