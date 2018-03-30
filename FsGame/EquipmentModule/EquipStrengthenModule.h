//--------------------------------------------------------------------
// �ļ���:      EquipStrengthenModule.h
// ��  ��:      װ��ǿ��
// ˵  ��:
// ��������:    2014��11��1��
// ������:        
// �޸���:		
//    :       
//--------------------------------------------------------------------
#ifndef FSGAME_ITEMBASE_EQUIP_STRENGTHEN_MODULE_H_
#define FSGAME_ITEMBASE_EQUIP_STRENGTHEN_MODULE_H_

#include "Fsgame/Define/header.h"
#include "EquipStrengthenDefine.h"

class EquipStrengthenModule : public ILogicModule
{
public:
    //��ʼ��
    virtual bool Init(IKernel* pKernel);

    //�ͷ�
    virtual bool Shut(IKernel* pKernel);

	// ������Դ
	bool LoadRes(IKernel* pKernel);

	//����������Դ
	bool LoadConsumeRes(IKernel* pKernel);

	// ����ǿ����������
	bool LoadStrLimitRes(IKernel* pKernel);

	// ����ǿ����װ����
	bool LoadStrSuitRes(IKernel* pKernel);

	// ����ǿ���������
	bool LoadStrApperRes(IKernel* pKernel);

	// �ͷ������ļ�
	bool ReleaseResource(IKernel* pKernel);

private:
	// ǿ����װ���
	void SuitCheck(IKernel* pKernel, const PERSISTID& self);

	// ȡ��ǿ������
	bool GetStrConsume(const int str_level, Consume_Vec& capitals, Consume_Vec& items);

	// ȡ��ǿ�����Ƶȼ�
	const int GetLimitStrLevel(IKernel* pKernel, const PERSISTID& self, int equip_index);

	// ȡ�����ǿ���ȼ�
	const int QueryMaxStrenLvl(IKernel* pKernel, const PERSISTID& self);

	// ȡ�����ǿ���ȼ�
	const int GetMinStrenLvl(IKernel* pKernel, const PERSISTID& self);

	// ȡ����װ���������԰�
	const int GetSuitActivePkg(const int min_str_lvl);

public:
    //һ��ǿ��
    int AutoStrengthenEquip(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// player����ִ��ǿ��
	int ExecuteStrengthenEquip(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ȡ��ָ����λ��ǿ���ȼ�
	const int GetStrLvlByPos(IKernel* pKernel, const PERSISTID& self, 
		const int target_pos);

	// ȡ��ǿ�����
	const int GetStrApperance(const char *equip_id, const int str_lvl);
	
private:
	// �ͻ�����Ϣ
	static int OnCustomEquipStrengthen(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ���¼���װ��ǿ������
	static void ReloadStrengthenConfig(IKernel* pKernel);

public:

	// ʵ���൥��ָ��
	static EquipStrengthenModule * m_pInstance;

	// ǿ���ȼ�����
	StrengthenLimitVec m_StrengthenLimitVec;

	// ǿ������
	StrenthenConsumeVec m_StrenthenConsumeVec;

	// ǿ����װ
	StrengthenSuitList m_StrengthenSuitList;

	// ǿ�����
	StrengthenApperanceVec m_StrengthenApperanceVec;
};

#endif 
