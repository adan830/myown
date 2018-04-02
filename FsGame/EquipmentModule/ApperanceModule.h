//--------------------------------------------------------------------
// �ļ���:      ApperanceModule.h
// ��  ��:      ���ģ��
// ˵  ��:		
// ��������:    2018��03��14��
// ������:      tzt      
//--------------------------------------------------------------------
#ifndef ApperanceModule_h__
#define ApperanceModule_h__

#include "Fsgame/Define/header.h"
#include "EquipDefine.h"
#include "ApperanceDefine.h"

class ApperanceModule : public ILogicModule
{
public:
    // ��ʼ��
    virtual bool Init(IKernel* pKernel);

    // �ͷ�
    virtual bool Shut(IKernel* pKernel);

private:
	// ����������ȼ�����
	bool LoadApperancePriRes(IKernel* pKernel);

	// ������۴������԰�����
	bool LoadApperancePkgRes(IKernel* pKernel);

	// ȡ�������ʾ���ȼ�
	const int GetApperancePri(const int src);

	// ȡ����۴������԰�
	const int GetApperancePkg(const int apperance_id);

	// �ı����
	void Change(IKernel *pKernel, const PERSISTID &self,
		const ApperanceType type);

public:
    // ����һ�����
	void Add(IKernel *pKernel, const PERSISTID &self, 
		const ApperanceType type, const ApperanceSource src, const int apperance_id);

	// ����������ͺ���Դɾ��һ�����
	void Remove(IKernel *pKernel, const PERSISTID &self,
		const ApperanceType type, const ApperanceSource src);

public:
	// �����Ա�ȡ��װ�����
	const int GetEquipApperance(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &equip);

	// �����Ա�ȡ��װ��ʱװ
	const int GetEquipFashion(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &equip);

	// ȡ��ָ�����͵����ID
	const int GetApperanceByType(IKernel *pKernel, const PERSISTID &self,
		const ApperanceType type);

	// ȡ��ָ�������������
	const char *GetApperanceProp(const ApperanceType type);

	// ��������Ƿ�Ϸ�
	bool IsValidAppearance(const ApperanceType type);

	// ����װ��λ��ȡ���������
	const ApperanceType GetApperanceType(const EQUIP_POS pos);

	// ��ʼ���������԰�
	void InitPackage(IKernel *pKernel, const PERSISTID &self);

private:
	// ��װ��
	static int OnEquipBoxAfterAdd(IKernel* pKernel, const PERSISTID& equipbox,
		const PERSISTID& sender, const IVarList& args);

	// ��װ��
	static int OnEquipBoxRemove(IKernel* pKernel, const PERSISTID& equipbox,
		const PERSISTID& sender, const IVarList& args);

	// �����Ʒ
	static int OnItemBoxAfterAdd(IKernel* pKernel, const PERSISTID& item_box,
		const PERSISTID& sender, const IVarList& args);

	// ǿ���ȼ��仯
	static int OnStrLvlChanged(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ������ݻָ����
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

private:
	ApperancePriVec m_ApperancePriVec;
	ApperancePkgVec m_ApperancePkgVec;

public:
	static ApperanceModule *m_pThis;
};

#endif // ApperanceModule_h__