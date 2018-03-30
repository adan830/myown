//--------------------------------------------------------------------
// �ļ���:		EntRpc.h
// ��  ��:		RPC���Ռ��F
// ˵  ��:		
// ��������:		2016��9��14��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __ENT_RPC_SERVICE_H__
#define __ENT_RPC_SERVICE_H__

#include <list>
#include "EntNetBase.h"
#include "NetTextAction.h"
#include "CmdInfo.h"
#include "NoticeInfo.h"
#include "MailData.h"
#include "RpcServer.h"
#include "FsThreadQueue.h"
#include "RpcHandler.h"
#include "PlayerData.h"
#include "GameInfo.h"
#include "FsDelayQueue.h"

class SoloGmcc;
class EntHttp;
class EntRpc : public EntNetBase, public IRpcServer
{
public:
	EntRpc();
	~EntRpc();
	virtual bool Init(const IVarList& args) override;
	virtual bool Shut() override;

	// ��������
	bool Startup();
	// ֹͣ����
	bool Stop();
	virtual void Execute(float seconds);
	virtual int GetNetKey() const;

	GameInfo& GetGameInfo();
	const GameInfo& GetGameInfo() const;

	// ����rpc����
	bool StartRpc();
	bool StopRpc();

public:
	void OnRpcCommand( const CmdInfo& cmd );
	void OnRpcNotify(const NotifyInfo& notify);
	void OnRpcMail(const MailData& data);
	void OnRpcPropQuery(const CmdPropQuery& query);
	void OnRpcRecordQuery(const CmdRecordQuery& query);

	void OnRpcPayment(const Payment& payment);
	void OnRpcGoods();

	virtual void OnRpcMallItems() override;
	virtual void OnRpcConfig(int nType, int nConfigId) override;


	void OnPropData(const CmdPropData& data);
	void OnRecordData(const CmdRecordData& data);

	// ��������
	void OnSetServerHeartbeat(int nHeatBeat);
	// ������Ϸ��
	void OnSetGameName(const char* pszName);
	// ��Ϸ��id
	void OnSetGameAreaId(long long nAreaId);
	// �ϱ�������״̬
	void OnReportState(EmMemberState state);
	// �ϱ���������
	void OnReportOnline( int nCount );
	// �ϱ���������Ա��Ϣ
	void OnReportMemberInfo(const MemberInfo& info);
	// �ϱ��������Ͽ���Ϣ
	void OnReportDeadAll();
private:
	static void RpcThread(void* p);
protected:
	// gmcc ���ӿ�
	SoloGmcc* m_pGmcc;
	// gmcc http�ӿ�
	EntHttp* m_pHttp;
	// rpc����
	RpcServer						m_rpcServer;
	boost::shared_ptr<RpcHandler>	m_rpcHandler;
	// ���̶߳���
	//FsThreadQueue					m_mainQueue;
	FsDelayQueue					m_mainQueue;

	// ��Ϸ����������
	GameInfo						m_gameInfo;

	CThread*						m_pThread;
};

#endif // END __ENT_RPC_SERVICE_H__