//--------------------------------------------------------------------
// �ļ���:	SystemInfo_SysFunction.h
// �� ��:	����ϵͳ��Ϣ����
// ˵ ��:		
// ��������:
// ������:	tzt    
//--------------------------------------------------------------------
#ifndef _SystemInfo_SysFunction_H_
#define _SystemInfo_SysFunction_H_


// SystemFunctionModule ����ϵͳ(19001-21000)����ϵͳ��ϸ���֡�
enum SystemInfo_SysFunction
{
	
#pragma region // Shop�̵�ϵͳ(19001-19200)
	SYSTEM_INFO_ID_19001 = 19001,	// ����ʧ��
	SYSTEM_INFO_ID_19002,			// ����ˢ�´���������
	SYSTEM_INFO_ID_19003,			// û�й������Ʒ��Ȩ��
	SYSTEM_INFO_ID_19004,			// ��Ʒ��������
	SYSTEM_INFO_ID_19005,			// Ԫ������

#pragma endregion	

#pragma region // Other����(19201-19300)
	SYSTEM_INFO_ID_19201 = 19201,	// �ù��ܴ�����ȴ�У�{0}������ʹ��
	SYSTEM_INFO_ID_19202,			// �鿴{0}����Ʒʧ�ܣ�������Ʒ��Ϣʧ��
	SYSTEM_INFO_ID_19203,			// �һ�ʧ��
	SYSTEM_INFO_ID_19204,			// ����{0}��ȡ���Ѿ��ﵽÿ�շ�ֵ��ÿ�����ޣ�{1}


#pragma endregion	

#pragma region // ���ϵͳ(19301-19400)
	SYSTEM_INFO_ID_19301 = 19301,	// �������ʧ��,�ѵ���󼶱�
	SYSTEM_INFO_ID_19302,			// �������ʧ��,Ǯ����
	SYSTEM_INFO_ID_19303,			// �������ʧ��,���ϲ���
	SYSTEM_INFO_ID_19304,			// �������ʧ��,���ϲ���
	SYSTEM_INFO_ID_19305,			// �������ʧ��,���ȼ�����
#pragma endregion	

#pragma region // ����ϵͳ(19401-19500)
	SYSTEM_INFO_ID_19401 = 19401,	// Ƥ���ѽ���
	SYSTEM_INFO_ID_19402,			// ǰ��Ƥ��δ����
#pragma endregion	
	
};


#endif