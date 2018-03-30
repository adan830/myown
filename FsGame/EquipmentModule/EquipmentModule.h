//--------------------------------------------------------------------
// �ļ���:      EquipmentModule.h
// ��  ��:      װ��ģ��
// ˵  ��:		
// ��������:    2014��10��24��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef EquipmentModule_h__
#define EquipmentModule_h__


#include "Fsgame/Define/header.h"
#include "EquipDefine.h"

#include <string>
#include <map>
#include <vector>

class DropModule;

class EquipmentModule : public ILogicModule
{
public:

	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ͷ�
	virtual bool Shut(IKernel* pKernel);

public:

    // װ�������״̬
    struct SoulState
    {
    
        SoulState() : equip_all_set(false), soul_all_set(false), soul_level(0), soul_class(0)
        {}
    
        bool equip_all_set; // �Ƿ񴩴�������װ��
        bool soul_all_set;  // �Ƿ����������
        int soul_level;     // ��������е���͵ȼ�
        int soul_class;     // ��������е���ͽ� (һ�����...N�����)
    };
    
    // �򵥵�װ����Ϣ
    struct EquipInfo
    {
        std::string config_id;
        int color;
        EquipInfo():config_id(""),
                    color(0)
        {

        }
    };

	// �������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ���׼������
	static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
		
	// װ�����ഴ��ʱ�Ļص�
	static int OnEquipBoxClassCreate(IKernel* pKernel, int index);

	//װ�����Ļص�
	static int OnEquipBoxCreate(IKernel* pKernel, const PERSISTID& equipbox,
		const PERSISTID& sender, const IVarList& args);

	static int OnEquipBoxAfterAdd(IKernel* pKernel, const PERSISTID& equipbox,
		const PERSISTID& sender, const IVarList& args);

	static int OnEquipBoxRemove(IKernel* pKernel, const PERSISTID& equipbox,
		const PERSISTID& sender, const IVarList& args);

	static int OnEquipBoxNoAdd(IKernel* pKernel, const PERSISTID& equipbox,
		const PERSISTID& item, const IVarList& args);

	static int OnEquipBoxNoRemove(IKernel* pKernel, const PERSISTID& equipbox,
		const PERSISTID& sender, const IVarList& args);

	static int GetEquipCount( IKernel* pKernel, const PERSISTID& self );
	static int GetEquipCountByColor( IKernel* pKernel, const PERSISTID& self, int nColor );
	static int GetEquipCountByGreaterColor( IKernel* pKernel, const PERSISTID& self, int nMinColor );

    // ����ɫ���ϵ�����Ƕ���״̬
    static bool CheckSoul(IKernel* pKernel, const PERSISTID& self,
        SoulState *soul_state);
        
    // ��ȡ��ɫ���ϵ�������Ϣ, ����NULL ��ʾ��λ��û��װ��
    static const char* GetEquipConfig(IKernel* pKernel, const PERSISTID& self, int equip_index);

    // ��ȡ��ɫ���ϵ�������Ϣ
    static bool GetEquipInfo(IKernel* pKernel, const PERSISTID& self, int equip_index, EquipInfo *info);

	//�ܷ����װ�������������̳���Ƿ�����ͬ��װ����
	//static bool CanPutIntoEquip(IKernel* pKernel,const PERSISTID& self,const PERSISTID& item);
public:
	//�ж�һ��װ���Ƿ��װ������ҵ�װ������ָ��λ��
	EQUIPCHECK_RESULT CanEquip(IKernel* pKernel, const PERSISTID& self, const PERSISTID& item, int pos);
	//���װ���Ƿ���Ա�����
	EQUIPCHECK_RESULT CanChangeEquip(IKernel* pKernel, const PERSISTID& owner, const PERSISTID& equip);

	//���ĳ�����ӵ�װ������
	std::string GetEquipType(int index);
	//���ĳ�����Ͷ�Ӧ�ĸ���
	int GetEquipIndex(const char * szType);
	// �Ƿ�ƥ��
	bool IsMatch(int classlimit, int condition);

	//ˢ�¿���װ��
	bool RefreshVisible(IKernel* pKernel, const PERSISTID& self);

	// ���������ļ�
    void LoadResource(IKernel* pKernel);

	// �ͷ������ļ�
	void ReleaseResource(IKernel* pKernel);

	//����װ������
	void ReportEquipError(IKernel* pKernel, const PERSISTID& equip, EQUIPCHECK_RESULT result);

	// ����װ��, @equip_prop_percent ��ʾȡ������Է�Χ��ȡֵ��Χ��[0, 100], Ĭ��ֵ��-1, ��ʾ��ָ����Χ
	bool CreateEquip(IKernel * pKernel, const PERSISTID & container, const char * config, int count, int equip_prop_percent = -1);
    
    // ���ϳ��
    void OnPutonEquipment(IKernel * pKernel, const PERSISTID &owner, const PERSISTID & equipment);
    
    // ���³��
    void OnTakeoffEquipment(IKernel * pKernel, const PERSISTID &owner, const PERSISTID & equipment);

    // ���װ���������, @equip_prop_percent ��ʾȡ������Է�Χ��ȡֵ��Χ��[0, 100], Ĭ��ֵ��-1, ��ʾ��ָ����Χ
    int SetRandEquipProp(IKernel* pKernel, const PERSISTID & self, 
        const PERSISTID & equip, int color_level, int prop_percent = -1);

    // ��ø����������ֵ
    int GetRandAddProp(int minValue, int maxValue, int prop_percent = -1);

	// ���¼���װ������
	static void ReloadEquipConfig(IKernel* pKernel);
     
public:

	static EquipmentModule* m_pEquipmentModule;
    static DropModule * m_pDropModule;

	std::map<int, std::string> m_EquipIndexName;
	std::map<std::string, int> m_EquipNameIndex;
};

#endif // EquipmentModule_h__