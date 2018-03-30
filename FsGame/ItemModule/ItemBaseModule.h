//--------------------------------------------------------------------
// �ļ���:		ItemBaseModule.h
// ��  ��:		��Ʒ��������
// ˵  ��:		
// ��������:	2014��10��17��
// ������:		 
//    :	   
//--------------------------------------------------------------------
#ifndef _ItemBaseModule_H
#define _ItemBaseModule_H

#include "Fsgame/Define/header.h"
#include "Fsgame/Define/FunctionEventDefine.h"

class EquipmentModule;
// #ifndef FSROOMLOGIC_EXPORTS
// class BianstoneModule;
// #endif

class ItemBaseModule: public ILogicModule
{
public:
	virtual bool Init(IKernel* pKernel);
	virtual bool Shut(IKernel* pKernel);

	// ������Ʒ����
	bool LoadItemPathConfig(IKernel* pKernel);
	
	//��Ʒ����
	static bool ItemSort(IKernel * pKernel, CVarList args, CVarList &returnList);
private:
	static int OnCreateClass(IKernel* pKernel, int index);

	static int OnCreate(IKernel * pKernel, const PERSISTID & self, 
		const PERSISTID & sender, const IVarList & args);

	static int OnLoad(IKernel * pKernel, const PERSISTID & self, 
		const PERSISTID & sender, const IVarList & args);

	static int OnRecover(IKernel * pKernel, const PERSISTID & self, 
		const PERSISTID & sender, const IVarList & args);

	//���Իص�
	static int C_OnAmountChanged(IKernel * pKernel, const PERSISTID & self, const char* property, const IVar & old);

public:
	//������Ʒ, @equip_prop_percent ��ʾװ��������Է�Χ��ȡֵ��Χ��[0, 100], Ĭ��ֵ��-1, ��ʾ��ָ����Χ
	PERSISTID CreateItem(IKernel * pKernel, const PERSISTID & container, const char * config, int count,
							EmFunctionEventId nSrcFromId = FUNCTION_EVENT_ID_SYS, bool bIsGain = false,
							int equip_prop_percent = -1);

	// ���¼�����Ʒ��������
	static void ReloadItemBaseConfig(IKernel* pKernel);

	static ItemBaseModule * m_pItemBaseModule;

private:

	static EquipmentModule * m_pEquipmentModule;
// #ifndef FSROOMLOGIC_EXPORTS
// 	static BianstoneModule* m_pBianstoneModule;
// #endif
};

#endif // _ItemBaseModule_H

