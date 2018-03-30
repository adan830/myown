//--------------------------------------------------------------------
// �ļ���:		RpcServer.h
// ��  ��:		Rpc����
// ˵  ��:		
// ��������:		2016��9��18��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __RPC_SERVER_H__
#define __RPC_SERVER_H__
#include "thrift/server/TThreadPoolServer.h"
#include "RpcHandler.h"
#include <mutex>
#include <condition_variable>

class RpcServer
{
	apache::thrift::server::TThreadPoolServer* m_pServer;
	std::atomic_bool m_bExitThread;
	bool m_bRunning;
public:
	RpcServer();
	void Start(const boost::shared_ptr<RpcHandler>& handler, int nPort);
	void Stop();
	void Run();
	bool IsRunning();
};

#endif 