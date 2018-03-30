//--------------------------------------------------------------------
// �ļ���:		RewardDefine.h
// ��  ��:		����
// ˵  ��:		
//				
// ��������:	2018��02��23��
// ��������:	2018��02��23��    
//--------------------------------------------------------------------

#ifndef __RewardDefine_H__
#define __RewardDefine_H__

const char* const STR_LEVEL_EXP = "LevelExp";		// ��Ҿ����ʶ

// �������
enum
{
	REWARD_MAIL_ON_FULL = 1, // �������˾�ͨ���ʼ�����
	REWARD_DISCARD_ON_FULL = 2, // �������˾Ͷ����Ų��µ�
	REWARD_FAILED_ON_FULL = 3, // �������˾ͷ���ʧ��
	REWARD_MAIL = 4, // ͨ���ʼ�����
};

enum  //������Ϣ
{
	S2C_SHOW_ITEM_LIST = 0,	//��ʾ��õ���Ʒ int ��Ʒ���� 

};

// ��������
enum AwardType
{
	CAPITAL_AWARD,
	ITEM_AWARD,
	EXP_AWARD
};

#endif // __RewardDefine_H__
