//--------------------------------------------------------------------
// �ļ���:      ModifyPackModule.h
// ��  ��:      ���������������Ŀ���������ά��
// ˵  ��:
// ��������:    2014��11��1��
// ������:      
// �޸���:      
//    :       
//--------------------------------------------------------------------
#ifndef _ModifyPackModule_h
#define _ModifyPackModule_h

#include "Fsgame/Define/header.h"
#include "../Define/ModifyPackDefine.h"

class EquipModifyPackModule : public ILogicModule
{
public:
    //��ʼ��
    virtual bool Init(IKernel* pKernel);

    //����
    virtual bool Shut(IKernel* pKernel);
private:

    //�ص����� ��Ҽ����������
    static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	//��װ��
	static int OnEquipAfterAdd(IKernel* pKernel, const PERSISTID& equipbox,
		const PERSISTID& sender, const IVarList& args);

	//��װ��
    static int OnEquipRemove(IKernel* pKernel, const PERSISTID& equipbox,
		const PERSISTID& sender, const IVarList& args);
	
	//ǿ���ȼ��ı�ص�
	static int OnEquipStrLevelUp(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& equip, const IVarList& args);

	//ǿ����װ�ı�ص�
	static int OnEquipStrSuit(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	//ϴ�����Է����仯
	static int OnEquipBaptisePropChange(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& equip, const IVarList& args);

	//����������Ա仯�ص�
	static int OnEquipAddModifyChange(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& equip, const IVarList& args);

	//��ʯ��Ƕor����or���
	static int OnEquipJewelChange(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& equip, const IVarList& args);      

	// �ȼ��仯�ص�
	static int OnDCLevelChange(IKernel* pKernel, const PERSISTID &self, const PERSISTID & sender, const IVarList & args);

    // ʱװ�仯�ص�
    static int OnFashionChanged(IKernel* pKernel, const PERSISTID &self, const PERSISTID & sender, const IVarList & args);

	// ˢ��ս����
 	static int RefreshEquipAbility(IKernel* pKernel, const PERSISTID& equip, 
 		const PERSISTID& self, const IVarList& args);

private:

    // ��ʼ��������
    void InitPropModifyRec(IKernel* pKernel, const PERSISTID& self);

	/// \brief װ����������ˢ�·�ʽ
	bool UpdateEquipModifyObjProp(IKernel* pKernel, const PERSISTID& self, const PERSISTID& equip, MapPropValue& mapPropValue);
	
	/// \brief ɾ��װ���������Ե�Ӱ��
	bool RemoveEquipModifyObjProp(IKernel* pKernel, const PERSISTID& self, const PERSISTID& equip, MapPropValue& mapPropValue);

	/// \brief ǿ�������޸�
	bool UpdateEquipStrPack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& equip, int oldProp, int newProp);

	/// \brief ϴ�������޸�
	bool UpdateEquipBaptisePack(IKernel* pKernel, const PERSISTID& self, IVarList& args, const PERSISTID& equip);

	/// \brief ������������޸�
	bool UpdateEquipAddModifyPack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& equip, const char * oldProp, const char *  newProp);

	/// \brief ���±�ʯ����
	void UpdateEquipJewelPack(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

    /// \brief ��������������
    void UpdateSoulAppearancePack(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

    /// ʱװ���������Ա仯
    void UpdateFashionPack(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ����Ϣ�洢��Ĳ���
	void HandleSaveRec(IKernel* pKernel, const PERSISTID& self, const PERSISTID& equip, 
		const std::string& strPropName, const float fValue);

public:

    // ��ģ��ָ��
    static EquipModifyPackModule*        m_pInstance;
};

#endif
