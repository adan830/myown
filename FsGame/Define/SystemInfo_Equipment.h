//--------------------------------------------------------------------
// �ļ���:	SystemInfo_Equipment.h
// �� ��:	װ��ϵͳ��Ϣ����
// ˵ ��:		
// ��������:
// ������:	tzt    
//--------------------------------------------------------------------
#ifndef _SystemInfo_Equipment_H_
#define _SystemInfo_Equipment_H_


// EquipmentModule װ��ϵͳ(5001-7000)����ϵͳ��ϸ���֡�
enum SystemInfo_Equipment
{
#pragma region // JewelModule��ʯ��5001-5100��	

#pragma endregion

#pragma region // EquipStrengthenModuleǿ����5101-5200��	
	SYSTEM_INFO_ID_5101 = 5101,		// ǿ���Ѵﵽ���ޣ�������ǿ��
#pragma endregion

#pragma region // EquipForgingModule���죨5201-5300��	
	SYSTEM_INFO_ID_5201 = 5201,		// ְҵ��ƥ��
#pragma endregion

#pragma region // FashionModuleʱװ��5301-5400��	
	SYSTEM_INFO_ID_5301 = 5301,		// ǰ��ʱװδ����
	SYSTEM_INFO_ID_5302,			// ��ǰʱװ�ѽ���
	SYSTEM_INFO_ID_5303,			// ��ǰʱװְҵ��ƥ��
	SYSTEM_INFO_ID_5304,			// ��ǰʱװ�Ա�ƥ��
#pragma endregion

};


#endif