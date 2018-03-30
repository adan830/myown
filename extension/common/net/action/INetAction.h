//--------------------------------------------------------------------
// �ļ���:		INetAction.h
// ��  ��:		���綯������
// ˵  ��:		
// ��������:		2016��5��19��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __I_NET_ACTION_H__
#define __I_NET_ACTION_H__

#include "IAction.h"
#include <memory>

typedef IMessage INetMessage;
typedef IRequest INetRequest;
typedef IResponse INetResponse;
typedef IAction INetAction;

#define NetAction		Action
#define NetReqAction	ReqAction

class INetConnection; 
enum EMPackResult
{
	EM_PACK_SUCCESS,		// �ɹ�
	EM_PACK_BROKEN,			// �ϰ�
	EM_PACK_UNREGISTER,		// δע��
	EM_PACK_TIMEOUT,		// �ظ���ʱ
	EM_PACK_ERROR,			// ������
	EM_PACK_RAW_MSG,		// ԭʼ��
	EM_PACK_EMPTY,			// �հ�
};
class INetActionPacker
{
public:
	virtual ~INetActionPacker() = 0;
	virtual EMPackResult Pack(INetConnection* conn, FsIStream& stream, std::shared_ptr<INetAction>& action, size_t* nOutLen = NULL) const = 0;
	virtual EMPackResult UnPack(INetConnection* conn, FsIStream& stream, std::shared_ptr<INetAction>& action, size_t* nOutLen = NULL) const = 0;
};
inline INetActionPacker::~INetActionPacker(){}


#endif // END __I_NET_ACTION_H__