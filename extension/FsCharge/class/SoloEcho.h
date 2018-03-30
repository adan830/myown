//--------------------------------------------------------------------
// �ļ���:		SoloEcho.h
// ��  ��:		�K�˷��Ռ��F
// ˵  ��:		
// ��������:		2016��6��12��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __ENT_SOLO_ECHO_H__
#define __ENT_SOLO_ECHO_H__

#include "EntNetBase.h"
#include <list>
#include "NetTextAction.h"

class SoloEcho : public EntNetBase
{
public:

	virtual bool Init(const IVarList& args) override;
	virtual bool Shut() override;

	// ��������
	bool Startup();
	// ֹͣ����
	bool Stop();
	virtual void Execute(float seconds);

	virtual EMPackResult Pack(INetConnection* conn, FsIStream& stream, std::shared_ptr<INetAction>& action, size_t* nOutLen = NULL) const;
	virtual EMPackResult UnPack(INetConnection* conn, FsIStream& stream, std::shared_ptr<INetAction>& action, size_t* nOutLen = NULL) const;

	virtual int GetNetKey() const;
	void SendEchoLine(const char* echo);

	// ��ֹ
	void EndEcho();
protected:
	virtual void OnInit();

	void OnCommand(const SERID& serid, const SESSID& sid, std::shared_ptr<INetAction>& action);

	virtual void OnConnection(const SERID& serid, const SESSID& sessid, std::shared_ptr<INetAction>& pAction);
	virtual void OnClose(const SERID& serid, const SESSID& sessid, std::shared_ptr<INetAction>& pAction);

protected:
	NetTextMessagePacker m_packer;
	SESSID				 m_lastID;
};

#endif // END __ENT_SOLO_ECHO_H__