//--------------------------------------------------------------------
// �ļ���:      EquipForging.h
// ��  ��:      װ������
// ˵  ��:
// ��������:    2017��04��19��
// ������:       tongzt
// �޸���:		
//    :       
//--------------------------------------------------------------------
#ifndef FSGAME_ITEMBASE_EQUIP_FORGING_MODULE_H_
#define FSGAME_ITEMBASE_EQUIP_FORGING_MODULE_H_

#include "Fsgame/Define/header.h"
#include "EquipForgingDefine.h"

class EquipForgingModule : public ILogicModule
{
public:
    // ��ʼ��
    virtual bool Init(IKernel* pKernel);

    // �ͷ�
    virtual bool Shut(IKernel* pKernel);

    // ������Դ
    bool LoadResource(IKernel* pKernel);

	// ����
	void Forging(IKernel *pKernel, const PERSISTID &self, 
		const char *drawing);

	// ���¼��ض���װ������
	static void ReloadForgingConfig(IKernel* pKernel);
    
private:   
    // ��������
    bool LoadForgingRule(IKernel* pKernel);
		
	// ȡ�ö������
	const ForgingRule* GetForgingRule(const char *drawing);

	// �������Ʒ��
	const int RandomOutputColor(const OutputColorWeightVec& output_list, const int sum_weight);

	// ������Ϣ
	static int OnCustomEquipmentForging(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
	
public:

	static EquipForgingModule * m_pInstance;
	
private: 
    ForgingRuleVec m_ForgingRuleVec;
};

#endif 
