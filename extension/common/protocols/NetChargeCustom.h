//--------------------------------------------------------------------
// �ļ���:		NetChargeCustom.h
// ��  ��:		�Ʒ��Զ����Э��
// ˵  ��:		
// ��������:		2016��10��26��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __NET_WORLD_CHARGE_CUSTOM_H__
#define __NET_WORLD_CHARGE_CUSTOM_H__

enum EmNetworldCustem
{
	NET_WORLD_CUSTOM_LOGIN_READY = 1,				// ׼���ÿ��Ե�½�� GAME->CHARGE ����: �Ƿ�׼������(int) ��Ϸid(int64) ������id(int64) ����id(int64) channelid(����id)int64 nProduction(int64)
	NET_WORLD_CUSTOM_LOGIN_STATE_ACK,				// ��Ӧ��½״̬	CHARGE->GAME ����: ��
};

#endif // END __NET_WORLD_CHARGE_CUSTOM_H__