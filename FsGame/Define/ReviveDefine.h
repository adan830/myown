//--------------------------------------------------------------------
// �ļ���:		ReviveDefine.h
// ��  ��:		��������
// ˵  ��:		
//				
// ��������:	2018��02��23��
// ��������:	2018��02��23��    
//--------------------------------------------------------------------

#ifndef __REVIVE_DEFINE_H__
#define __REVIVE_DEFINE_H__

//(ini\SystemFunction\Revive\RevivePanel.xml)��RelivePanel�ֶ�
enum ReviveType
{
	NEARBY_REVIVE,		// �ͽ�����
	CITY_REVIVE,		// �ʳǸ���
	LOCAL_REVIVE,		// ԭ�ظ���
	WAIT_HELP,			// �ȴ�����

	MAX_REVIVE_NUM,
};

// ����Ϣ
enum ReviveC2SMsg
{
	CS_REQUEST_REVIVE,  // ���󸴻� int ��������		
};

enum ReviveS2CMsg
{
	SC_DEAD_PANEL,		// ���������Ϣ string panel int gold
};
#endif // __RewardDefine_H__
