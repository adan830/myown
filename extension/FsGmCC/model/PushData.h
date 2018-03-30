//--------------------------------------------------------------------
// �ļ���:		PushData.h
// ��  ��:		�������ݶ���
// ˵  ��:		
// ��������:		2016��10��20��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MODEL_GAME_PUSH_DATA_H__
#define __MODEL_GAME_PUSH_DATA_H__
#include <string>
#include <map>
#include <vector>
#include "PushDef.h"

struct PushData
{
	int msgType;						// ��Ϣ����(EmPushType)
	int recvierPlatform;				// ����ƽ̨(EmClientPlatform���ֵ)
	std::map<int, std::vector<std::string> > recivers;	// <��������(EmPushRecvierType�е�һ��ֵ), �����߱�ʶ>

	std::string title;					// ����
	std::string content;				// ����
	std::string sound;					// ����
	int builderid;						// android��ʾ��ʽ
	int badge;							// Сͼ������
	int content_available;				// ���ͻ��� (ios7�����)
	std::map<std::string, std::string>	extra;	// ��չ�ֶ�

public:
	PushData()
		: msgType(EM_PUSH_NOTIFICATION)
		, recvierPlatform( EM_CLIENT_ALL )
		, builderid(0)
		, content_available( 0 )
		, badge(0)
	{

	}
};

#endif 