//--------------------------------------------------------------------
// �ļ���:		NoticeInfo.h
// ��  ��:		������Ϣ����
// ˵  ��:		
// ��������:		2016��9��14��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MODEL_NOTICE_H__
#define __MODEL_NOTICE_H__
#include <string>
#include <stdint.h>

enum EmNotifyType
{
	EM_NOTIFY_NOTICE = 1,		// ����
	EM_NOTIFY_ACTIVITY,			// �
	EM_NOTIFY_PAYED,			// ֧��
	EM_NOTIFY_SIGNIN,
	EM_NOTIFY_STOREITEM,
	EM_NOTIFY_CONFIG,
};

enum EmNotifyOp
{
	EM_NOTIFY_OP_ADD,
	EM_NOTIFY_OP_DEL,
	EM_NOTIFY_OP_EDIT,
};

struct NotifyInfo
{
	int32_t			NotifyType;		// ֪ͨ����
	int32_t			OpType;			// ��������(��ɾ��) 
	std::string		AccId;			// �˺�id
	std::string		RoleId;			// ����id
	std::string		Content;		// ������չ
};

struct NoticeData
{
	std::string		Name;			// ����
	std::string		Content;		// ����
	int64_t			NoticeId;		// ֪ͨid
	int64_t			CreateTime;		// ����ʱ��
	int64_t			EndTime;		// ����ʱ��
	int64_t			Interval;		// ���ż��
	int32_t			Loop;			// ѭ������
	int32_t			Type;			// ֪ͨ����
};
#endif 