//--------------------------------------------------------------------
// �ļ���:	SystemInfo_Item.h
// �� ��:	��Ʒϵͳ��Ϣ����
// ˵ ��:		
// ��������:
// ������:	tzt    
//--------------------------------------------------------------------
#ifndef _SystemInfo_Item_H_
#define _SystemInfo_Item_H_


// ItemModule ��Ʒ(7001-9000)����ϵͳ����ϸ���֡�
enum SystemInfo_Item
{

#pragma region // ToolItemModule ����7001-7100
	SYSTEM_INFO_ID_7001 = 7001,	// ����������ʹ�õ���
	SYSTEM_INFO_ID_7002,		// ��Ʒ��ȴ��
	SYSTEM_INFO_ID_7003,		// �ȼ�����, ��Ҫ�ȼ�{0}
	SYSTEM_INFO_ID_7004,		// �����ռ䲻��
	SYSTEM_INFO_ID_7005,		// ��ϧ��Ʒ��ʾ:{0:��Ӫ}{1:vip}{2:���}��{3:��Դ}���{4:��Ʒ}args<<��Ӫ<<����<<vip<<��Դ<<id<<Ʒ��
	SYSTEM_INFO_ID_7006,		// �������������ѣ�����{0}������
	SYSTEM_INFO_ID_7007,		// �ɹ�����, ���{0}��ͭ��
	SYSTEM_INFO_ID_7008,		// �ɹ�����{0}��������, ����{1}��Ԫ��
	SYSTEM_INFO_ID_7009,		// ��������ɹ�, ���{0}������
	SYSTEM_INFO_ID_7010,		// �����ռ䲻��, ��Ʒ��ͨ���ʼ�����
	SYSTEM_INFO_ID_7011,        // Ԫ������
	SYSTEM_INFO_ID_7012,		// ����Ԫʧ��
	SYSTEM_INFO_ID_7013,		// ��Ԫ��ʾ
	SYSTEM_INFO_ID_7014,		// ͭ����ʾ
	SYSTEM_INFO_ID_7015,		// ����ֵ��ʾ
	SYSTEM_INFO_ID_7016,		// ������ʾ

	SYSTEM_INFO_ID_7017,		// ����ϳ�ʧ��,����δ����
	SYSTEM_INFO_ID_7018,		// ����ϳ�ʧ��,�Ѵ��ڴ�����
	SYSTEM_INFO_ID_7019,		// ����ϳ�ʧ��,��Ƭ����
	SYSTEM_INFO_ID_7020,		// ��������ʧ��,��ҵȼ�����
	SYSTEM_INFO_ID_7021,		// ��������ʧ��,����ʯ����
	SYSTEM_INFO_ID_7022,		// ��������ʧ��,���ϲ�������Ƭ����
#pragma endregion
	
};


#endif