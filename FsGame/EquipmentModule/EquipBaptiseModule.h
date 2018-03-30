//--------------------------------------------------------------------
// �ļ���:      EquipBaptiseModule.h
// ��  ��:      ϴ��ģ��
// ˵  ��:		
// ��������:		2018��03��12��
// ������:       tzt      
//--------------------------------------------------------------------

#ifndef _EquipBaptiseModule_h__
#define _EquipBaptiseModule_h__


#include "Fsgame/Define/header.h"
#include "EquipBaptiseDefine.h"

#include <string>
#include <map>
#include <vector>

class EquipBaptiseModule : public ILogicModule
{
public:

	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ͷ�
	virtual bool Shut(IKernel* pKernel);

	// ���ü���
	bool LoadRes(IKernel* pKernel);

	// 
	bool ReleaseRes(IKernel* pKernel);

private:
	// ����ϴ����������
	bool LoadBPRes(IKernel* pKernel);

	// ����ϴ������ȡֵ����
	bool LoadBPRangeRes(IKernel* pKernel);

	// ����ϴ��������������
	bool LoadBPLimitRes(IKernel* pKernel);

	// ����ϴ����������
	bool LoadBConsumeRes(IKernel* pKernel);

	// ȡ��ϴ�����Ա�
	const String_Vec *GetPropList(const int index, const int job);

	// ȡ��ϴ������ȡֵ
	const int GetPropValue(const int index, const int job, 
		const int baptise_type, const char *prop_name);

	// ȡ��ϴ����������ֵ
	const int GetPropLimitValue(const int index, const int job,
		const int str_lvl, const char *prop_name);

	// ȡ��ϴ������
	const BaptiseConsume *GetBaptiseConsume(const int baptise_type);

	// ƴ��json�ַ���
	void ConvertToJsonStr(const String_Vec& str_list, const Integer_Vec int_list, 
		std::string& json_str);

	// ����ϴ��ֵ
	void CorrectBaptiseValue(IKernel* pKernel, const PERSISTID& self, 
		const int equip_pos, const char *prop_name, const int str_lvl, int& final_value);

	// ָ����λϴ�������Ƿ�ȫ���ﵽ����
	bool IsAllPropMaxed(IKernel* pKernel, const PERSISTID& self,
		const int equip_pos);

public:
	// ִ��һ��ϴ��
	void ExecuteBaptise(IKernel* pKernel, const PERSISTID& self, 
		const IVarList& args);

	// ����ϴ�����
	void SaveResult(IKernel* pKernel, const PERSISTID& self,
		const int equip_pos);

	// ����ϴ�����
	void GiveupResult(IKernel* pKernel, const PERSISTID& self,
		const int equip_pos);

	// ��������
	void LockProp(IKernel* pKernel, const PERSISTID& self,
		const IVarList& args);

	// ��������
	void UnLockProp(IKernel* pKernel, const PERSISTID& self,
		const IVarList& args);

	// ��ʼ��ÿ��װ����λ��ϴ����Ϣ�����ߵ��ã�
	void InitBaptise(IKernel* pKernel, const PERSISTID& self);

	// ȡ����ҵĲ�λ��ϴ�������б�
	void GetPropNameList(IKernel* pKernel, const PERSISTID& self,
		const int equip_pos, String_Vec& prop_name_list);

private:
	// �������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ���׼������
	static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
		
	// ����ͻ��˷�����Ϣ
	static int OnCustomMsg(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
   
	// ���¼���װ������
	static void ReloadEquipConfig(IKernel* pKernel);

private:
	BaptisePropVec m_BaptisePropVec;
	BPRangeVec m_BPRangeVec;
	BPLimitValueVec m_BPLimitValueVec;
	BConsumeVec m_BConsumeVec;
     
public:
	static EquipBaptiseModule* m_pEquipBaptiseModule;

};

#endif