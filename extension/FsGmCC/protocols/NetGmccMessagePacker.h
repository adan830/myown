//--------------------------------------------------------------------
// �ļ���:		NetVarListPacker.h
// ��  ��:		
// ˵  ��:		
// ��������:	2016��9��17��
// ������:		���
// ��Ȩ����:	���ݾ����������޹�˾
//--------------------------------------------------------------------
#include "NetWorldAction.h"

#ifndef __NET_GMCC_MESSAGE_PACKER_H__
#define __NET_GMCC_MESSAGE_PACKER_H__

// ��Ϣ�����
class NetGmccMessagePacker : public NetVarListPacker
{
protected:
	virtual INetVarListActionProtocol* CreateAction(int nKey) const;
};

#endif 