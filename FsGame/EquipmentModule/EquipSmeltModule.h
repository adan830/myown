//--------------------------------------------------------------------
// �ļ���:      EquipSmeltModule.h
// ��  ��:      װ������
// ˵  ��:
// ��������:    2014��11��27��
// ������:        
// �޸���:		
//    :       
//--------------------------------------------------------------------
#ifndef EquipSmeltModule_h__
#define EquipSmeltModule_h__

#include "Fsgame/Define/header.h"
#include "EquipSmeltDefine.h"

class EquipSmeltModule : public ILogicModule
{

public:
	//��ʼ��
	virtual bool Init(IKernel* pKernel);

    //����
    virtual bool Shut(IKernel* pKernel);

private:
	// ���������������
	bool LoadSmeltSetting(IKernel* pKernel, const char* file);

	// ����������ԭ��, ���ز�������ֵ
	const int ConsumeRawMaterial(IKernel* pKernel, const PERSISTID& self, 
		const IVarList &uid_list);

	// ��� @item �Ƿ������ @items ��
	bool HasItem(const IVarList &items, const PERSISTID &item);

	// ȡ���������ֵ
	const float GetOutputSmeltValue(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID &item);
  
public:
	// ִ������
	int DoSmeltEquip(IKernel* pKernel, const PERSISTID& self, 
		const IVarList& args);

private:
	// ����װ��
	static int OnCustomEquipSmelt(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// �������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

public:
	//ʵ��ָ��
	static EquipSmeltModule *m_pEquipSmeltModule;
};
#endif // EquipSmeltModule_h__