//--------------------------------------------------------------------
// �ļ���:	SystemInfo_Common.h
// �� ��:	ͨ��ģ��ϵͳ��Ϣ����
// ˵ ��:		
// ��������:
// ������:	tzt    
//--------------------------------------------------------------------
#ifndef _SystemInfo_Common_H_
#define _SystemInfo_Common_H_


// CommonModule ͨ��ϵͳ(3001-5000)����ϵͳ��ϸ���֡�	
enum SystemInfo_Common
{
#pragma region // ActionMutex���ܻ��� 3001-3100
	SYSTEM_INFO_ID_3001 = 3001,		// ��������У��޷������������������˳����ٳ���
#pragma endregion

#pragma region // AsynCtrlModule�����л�3101-3200	
	SYSTEM_INFO_ID_3101 = 3101,		// ������ͻ���޷�����,���˳���ǰ�������ٽ��д���
#pragma endregion

#pragma region	// GMModule GMģ��3201-3300
	SYSTEM_INFO_ID_3201 = 3201,		// ִ��GM����ʧ��!
	SYSTEM_INFO_ID_3202,			// ��ǰ����С�����Ƽ��𣬲�����ִ�д�����
#pragma endregion

#pragma region	// ContainerModule ����3301-3400
	SYSTEM_INFO_ID_3301 = 3301,		// �����ռ䲻��
	SYSTEM_INFO_ID_3302,			// ��������̫Ƶ��(������ȴ��){0:ʣ����ȴʱ��s}
#pragma endregion


};


#endif