//---------------------------------------------------------
//�ļ���:       ToolBoxModule.h
//��  ��:       ������Ʒ��ز���,������Ʒ��ʹ��
//˵  ��:       
//          
//��������:      2014��11��22��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#ifndef FSGAME_PLAYER_BASE_MODULE_TOOL_BOX_MODULE_H
#define FSGAME_PLAYER_BASE_MODULE_TOOL_BOX_MODULE_H

#include "Fsgame/Define/header.h"

#include <map>
#include <string>
#include <vector>
#include "Define/ToolBoxDefine.h"

class ContainerModule;
class ToolItemModule;
class CapitalModule;
class AsynCtrlModule;

class ToolBoxModule: public ILogicModule
{
public:
    virtual bool Init(IKernel* pKernel);
    virtual bool Shut(IKernel* pKernel);

    bool LoadResource(IKernel* pKernel);

public:

    // ��������
    enum NotifyFlag 
    {
        NEED_NOTIFY = 1, // 0000 0001  ��Ҫ֪ͨ
        NOTIFY_DONE = 2, // 0000 0010  �Ѿ�֪ͨ��
    };

    // ��ȡ����
    static int GetToolBoxCapacity();
    // ��ȡ��������߱�������
    static int GetItemBoxCapacity();
	// ��ʱ��������
	static int GetTemporaryBoxCapacity();
    // ��ȡʵ������߱�������
    static int GetPowerBoxCapacity();
    // ��ȡ���ı�������
    static int GetBadgeBoxCapacity();
    // ��ȡ��Ƭ��������
    static int GetChipBoxCapacity();

    // ����Ƿ����ʹ����Ʒ
    static bool CanPlayerUseItem(IKernel* pKernel, const PERSISTID& self);

private:
 
    // װ�������������
    struct EquipGotNotifySetting
    {
		EquipGotNotifySetting(): equip_type(""), level(0), info_id(0){}
        std::string equip_type;     // װ������
        int level;                  // װ���ȼ�
        int info_id;				// ������Ϣ
    };

    // ��������
    static bool LoadToolBoxConfig(IKernel* pKernel, const char *config_file);
    

    // װ����������
    static bool LoadEquipNofitySetting(IKernel* pKernel, const char *config_file);

    // ��Ʒ����
    static int OnAfterAdd(IKernel * pKernel, const PERSISTID & container, 
        const PERSISTID & sender, const IVarList & args);

    // ������ݼ�����Ϻ�
    static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ��ҽ��볡����
    static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ItemBox ʵ�崴����
    static int OnItemBoxCreate(IKernel* pKernel, const PERSISTID& item_box,
        const PERSISTID& sender, const IVarList& args);

    // ����ͻ��˷�����Ϣ
    static int OnCustomMsg(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ����ͻ��˷��͵���ʽ��Ϣ
    static int OnCustomOldMsg(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ������Ʒ
    static int OnSellItem(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ��������
    static int OnUnlockCell(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ������
    static int OnArrange(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ��չ����
    static int OnExpand(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ֱ��ʹ�õ���
    static int OnCustomUseItem(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

	// ����ʱ������ȡ��Ʒ
	static int OnCustomPickupTemporayBox(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

    // �۳����
    static bool DeductGold(IKernel *pKernel, const PERSISTID &self, 
        int gold_type, int amount, int log_type = 0);

	// ������Ʒ������ʹ��
	static int RemindUseItem(IKernel * pKernel, const PERSISTID & container, int pos);
	
	// �����Ʒ���֪ͨ
	static int BroadcastOnGot(IKernel * pKernel, const PERSISTID& self, const PERSISTID & container, int pos);
	
	// ��ȡװ������������
	static const EquipGotNotifySetting* GetEquipGotNotifyConfig(const std::string& equip_type, int level);

    //������������
    static void UnLock(IKernel* pKernel, const PERSISTID& container, int confirmAdd, int& appendAdd);

public:
    //ƽ�����俪���ĸ��ӵ�4������
    static bool AssignPos(IKernel* pKernel, const PERSISTID& self);
private:

    struct ToolBoxConfig
    {
		ToolBoxConfig(): capacity(0), max_capacity(0), locked_size(0), gold_type_to_unlock(0), gold_to_unlock_cell(0),
			arrange_cooldown_ms(0), threshold_free_cell(0), reminder_interval(0),
			temporary_box_cap(TEMPORARY_BOX_DEFAULT_CAPACITY){}
        int capacity;               // ����(������)
        int max_capacity;           // ���������
        int locked_size;            // ����������
        int gold_type_to_unlock;    // ����������Ҫ�Ľ������
        int gold_to_unlock_cell;    // ����һ��������Ҫ�Ľ��
        int arrange_cooldown_ms;    // ����������ȴʱ��(����)
        int threshold_free_cell;    // ���и��ӵ����ѷ�ֵ, �����и�����С�ڵ��������ֵ��ʱ���������
        int reminder_interval;      // ����������һ��, ��λ��

		int temporary_box_cap;		// ��ʱ��������(������)
    };

    struct ItemBoxConfig:ToolBoxConfig 
    {
    };

    struct PowerBoxConfig:ToolBoxConfig 
    {
    };

    struct BadgeBoxConfig:ToolBoxConfig 
    {
    };

    struct ChipBoxConfig:ToolBoxConfig 
    {
    };

	static std::string m_strItemType;		// ��Ʒ����
	static std::string m_strItemConfigId;	// ��Ʒid

    static ToolBoxConfig m_ToolBoxConfig;   //�ܱ�������
    static ItemBoxConfig m_ItemBoxConfig;   //�������������
    static PowerBoxConfig m_PowerBoxConfig; //ʵ�����������
    static BadgeBoxConfig m_BadgeBoxConfig; //��������
    static ChipBoxConfig m_ChipBoxConfig;   //��Ƭ����
    
    static std::vector<EquipGotNotifySetting> m_EquipGotNotifySetting;

    static ContainerModule* m_pContainerModule;
    static ToolItemModule* m_pToolItemModule;
    static CapitalModule* m_pCapitalModule;
	static AsynCtrlModule* m_pAsynCtrlModule;

public:
    static ToolBoxModule* m_pToolBoxModule;
};

#endif 