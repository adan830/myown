//--------------------------------------------------------------------
// �ļ���:		PushDef.h
// ��  ��:		���ͷ�����
// ˵  ��:		
// ��������:		2016��9��17��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __NET_PUSH_DEFINE_H__
#define __NET_PUSH_DEFINE_H__

// ֪ͨ����
enum EmPushType
{
	EM_PUSH_NOTIFICATION = 0x01,		// ֪ͨ
	EM_PUSH_CUSTOM = 0x02,				// �Զ���(ֻ��android֧��)
};

// ֪ͨ����������
enum EmPushRecvierType
{
	EM_PUSH_RECVIER_TAG = 2,			// ��ǩ
	EM_PUSH_RECVIER_ALIAS = 3,			// ����
	EM_PUSH_RECVIER_BROADCAST = 4,		// �㲥
	EM_PUSH_RECVIER_REGISTRATIONID = 5,// �������id
};

// �ͻ���ƽ̨����
enum EmClientPlatform
{
	EM_CLIENT_IOS = 0x01,
	EM_CLIENT_ANDROID = 0x02,
	EM_CLIENT_WINPHONE = 0x04,

	EM_CLIENT_ALL = EM_CLIENT_IOS | EM_CLIENT_ANDROID | EM_CLIENT_WINPHONE,
};



#endif  // __NET_PUSH_DEFINE_H__ 