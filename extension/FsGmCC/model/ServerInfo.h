//--------------------------------------------------------------------
// �ļ���:		ServerInfo.h
// ��  ��:		��������web��ע����Ϣ����
// ˵  ��:		
// ��������:		2016��10��25��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MODEL_SERVER_INFO_H__
#define __MODEL_SERVER_INFO_H__
#include <string>
#include <vector>

struct SrvInfo
{
	long long Identity;
	long long GameId;
	long long ChannelId;
	long long ProductionId;
	long long ServerId;
	long long AreaId;
	long long DeployId;

	SrvInfo()
	{
		memset(this, 0, sizeof(SrvInfo));
	}

};
#endif 