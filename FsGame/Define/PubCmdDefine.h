//---------------------------------------------------------
//�ļ���:       PubCmdDefine.h
//��  ��:       ������������
//˵  ��:       �Ӷ�(Plunder),
//
//��������:     2017��08��01��
//������:       kevin 
// ��Ȩ����:	WhalesGame Technology co.Ltd  
//---------------------------------------------------------

#ifndef _FSPUB_CMD_DEFINE_H_
#define _FSPUB_CMD_DEFINE_H_

//���������
enum EPubCommandType
{
	E_PUB_CMD_UNKNOWN = 0,
	//1~99 system.cmd
	E_PUB_CMD_READY = 1,			// ׼������

	E_PUB_CMD_ACTIVITY_STATUS = 10, // �״̬
	E_PUB_CMD_EVERY_DAY_ZERO  = 11,	// ÿ���賿
	E_PUB_CMD_EVERY_DAY_RESET = 12,	// ÿ������ ��ʱ�䣺 05:00

	E_PUB_CMD_EVERY_PER_HOUR   = 13,	// ÿСʱ
	E_PUB_CMD_EVERY_PER_MINUTE = 14,	// ÿ����
	E_PUB_CMD_EVERY_PER_SECOND = 15,	// ÿ����
	//100~199 logic.cmd
	E_PUB_CMD_GUILD_DISMISS        = 100,	//��ɢ����
	E_PUB_CMD_GUILD_CAPTAIN_CHANGE = 101,	//����᳤���
	E_PUB_CMD_MAX,
};

#endif
