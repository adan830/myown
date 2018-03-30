//--------------------------------------------------------------------
// �ļ���:		MailInfo.h
// ��  ��:		�ʼ���Ϣ����
// ˵  ��:		
// ��������:		2016��9��17��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MODEL_MAIL_DATA_H__
#define __MODEL_MAIL_DATA_H__
#include <string>
#include <stdint.h>

struct MailData
{
	int64_t			MailId;
	int32_t			MailType;
	int64_t			CreateTime;
	int32_t			Status;
	int32_t			Period;

	std::string		Title;
	std::string		Content;
	std::string		senderId;
	std::vector<std::string>	ReciverIds;

	bool			GlobalMail;		// �Ƿ�㲥�ʼ�
	bool			HasAttach;
	std::string		AttachInfo;
};
#endif 