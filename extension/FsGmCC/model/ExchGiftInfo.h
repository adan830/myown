//--------------------------------------------------------------------
// �ļ���:		ExchGiftInfo.h
// ��  ��:		��Ʒ�һ��������
// ˵  ��:		
// ��������:		2016��9��17��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MODEL_EXCH_GIFT_INFO_H__
#define __MODEL_EXCH_GIFT_INFO_H__
#include <string>

struct ExchGiftInfo
{
	std::string		key;			// �һ�key
	std::string		account;		// �˺�	
	std::string		roleid;			// ���uid
};

struct ExchGiftResult
{
	std::string roleId;
	std::string key;
	int result;
	std::string capitals;
	std::string items;
};
#endif 