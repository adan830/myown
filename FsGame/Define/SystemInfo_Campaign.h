//--------------------------------------------------------------------
// �ļ���:	SystemInfo_Campaign.h
// �� ��:	�淨ϵͳ��Ϣ����
// ˵ ��:		
// ��������:
// ������:	tzt    
//--------------------------------------------------------------------
#ifndef _SystemInfo_Campaign_H_
#define _SystemInfo_Campaign_H_


// CampaignModule �淨ϵͳ(1-3000) ����ϵͳ��ϸ���֡�
enum SystemInfo_Campaign
{
	
#pragma region 	// ArenaModule ���䳡	(1 - 50)
	SYSTEM_INFO_ID_1 = 1,			// ������æ��������	(�쳣����)
	SYSTEM_INFO_ID_2,				// ��սCD��
	SYSTEM_INFO_ID_3,				// �ﵽ��������
	SYSTEM_INFO_ID_4,				// Ԫ������, ��Ҫ{@n}Ԫ��
	SYSTEM_INFO_ID_5,				// �����սCDʧ�� Ǯ����
	SYSTEM_INFO_ID_6,				// ��������ս, ����߶Է�����ս��
	SYSTEM_INFO_ID_7,				// ��������ս, ����߶Է��ѽ���
	SYSTEM_INFO_ID_8,				// ��������ս���ε����Լ��Ķ���
	SYSTEM_INFO_ID_9,				// ��������ս����ˢ��
	SYSTEM_INFO_ID_10,				// ���{0}�ɹ�ȡ�����{1}��������{2}����
#pragma endregion

#pragma region 	// WorldBossNpc ����boss	(51 - 100)
	SYSTEM_INFO_ID_51 = 51,			// ���Ҫ�ﵽ{0}��
	SYSTEM_INFO_ID_52,			    // ʱ�䵽�������
	SYSTEM_INFO_ID_53,			    // ����BOSS3���Ӻ���� ��������׼����
	SYSTEM_INFO_ID_54,			    // ��ϲ{0}��ɱ������BOSS{1}�������{2}��
	SYSTEM_INFO_ID_55,			    // Ǯ����,��������buff
	SYSTEM_INFO_ID_56,			    // ��������Ѵ�����
#pragma endregion

#pragma region 	// CampaignAsuraBattle ����ս��	(101 - 150)
	SYSTEM_INFO_ID_101 = 101,		// ����ս��������ʼ
	SYSTEM_INFO_ID_102,				// Boss�������� boss��ɱ,{0}��Ӫʤ��,ȫԱ������{1}��
	SYSTEM_INFO_ID_103,				// ����ս��δ����
	SYSTEM_INFO_ID_104,				// ��ҵȼ�����
	SYSTEM_INFO_ID_105,				// �Ѿ���buff��
	SYSTEM_INFO_ID_106,				// Ǯ����
	SYSTEM_INFO_ID_107,				// ��ɱ{0}��û���{1}
	SYSTEM_INFO_ID_108,				// ��ɱ{0},����������û���{1}
	SYSTEM_INFO_ID_109,				// {0}��Ӫ�ﵽ��{1}�ɼ�״̬
	SYSTEM_INFO_ID_110,				// ʣ�����ʱ����ʾ
	SYSTEM_INFO_ID_111,				// {0}��{1}��{2}��{3}������һ,��ȥ��ɱ
	SYSTEM_INFO_ID_112,				// Boss��ʾ����
	SYSTEM_INFO_ID_113,				// Boss��������
	SYSTEM_INFO_ID_114,				// ƽ�ֽ��
	SYSTEM_INFO_ID_115,				// {0}ʤ����
#pragma endregion

#pragma region 	// TeamOfferModule ��Ӽ���	(151 - 200)
	SYSTEM_INFO_ID_151 = 151,		// ����δ����
	SYSTEM_INFO_ID_152,				// ������������
	SYSTEM_INFO_ID_153,				// ���������Ѵ���
	SYSTEM_INFO_ID_154,				// ������������
	SYSTEM_INFO_ID_155,				// ��Ա{0}���ڼ�������
	SYSTEM_INFO_ID_156,				// ��Ա{0}�ڽ��������
	SYSTEM_INFO_ID_157,				// ��Ա{0}�ȼ�����
	SYSTEM_INFO_ID_158,				// ��Ա{0}������������
	SYSTEM_INFO_ID_159,				// ��Ա{0}����
	SYSTEM_INFO_ID_160,				// ��Ա{0}û��ƥ��ļ�������
	SYSTEM_INFO_ID_161,				// ��Ա{0}�ڸ�����
	SYSTEM_INFO_ID_162,				// ���Ƕӳ����޷�����
	SYSTEM_INFO_ID_163,				// ���뼩�����Npc̫Զ
	SYSTEM_INFO_ID_164,				// ��Ӽ���������
	SYSTEM_INFO_ID_165,				// �����ѽ���
	SYSTEM_INFO_ID_166,				// ���ڸ�����
	SYSTEM_INFO_ID_167,				// �޼�������
	SYSTEM_INFO_ID_168,				// ���ڼ�������
	SYSTEM_INFO_ID_169,				// ����δ��ʼ
	SYSTEM_INFO_ID_170,				// ����������
	SYSTEM_INFO_ID_171,				// �����˳�����

	
#pragma endregion
};


#endif