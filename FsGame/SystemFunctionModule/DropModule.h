//--------------------------------------------------------------------
// �ļ���:      DropModule.h
// ��  ��:      ����ϵͳģ��
// ˵  ��:      ���ڴ���NPC���������Ʒ
// ��������:    2014��11��3��
// ������:        
//    :       
//--------------------------------------------------------------------
#ifndef FSGAME_GENERAL_DROP_MODULE_H
#define FSGAME_GENERAL_DROP_MODULE_H

#include "Fsgame/Define/header.h"
#include <vector>
#include <string>
#include <map>

//����������ϵ���Ʒ�����
#define ITEM_DROP_RECORD "item_drop_record"
#define ITEM_DROP_DEFAULT_CD_TIME 60

enum
{
	ITEM_DROP_RECORD_ITEM_ID = 0,
	ITEM_DROP_RECORD_ITEM_COUNT,
	ITEM_DROP_RECORD_TIEM_CD_TIME,
};

//�ؾ����䱣���
#define SECRET_DROP_RECORD "secret_drop_rec"

enum
{
    ITEM_DROP_CONFIG_ID = 0,
    ITEM_DROP_AMOUNT    = 1,
};

class ContainerModule;
class CapitalModule;

class DropModule : public ILogicModule
{

public:

	// ��ʼ��
	virtual bool Init(IKernel * pKernel);

	// �ͷ�
	virtual bool Shut(IKernel * pKernel);

    bool LoadResource(IKernel * pKernel);

	//����뿪����
	static int OnPlayerLeaveScene(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

	// ������佱��
	void AllocateDropAward(IKernel* pKernel, const PERSISTID& self, const PERSISTID& deadnpc, const IVarList& killsers);

    // ���� @drop_id �ó��������Ʒ��config_id, ���� @drop_items
    // ��� drop_id �ö��ŷָ�
    // @drop_items �ɶ��� [��ƷconfigID][��Ʒ����] ��� 
    static int GetItemsFromDropID(IKernel *pKernel, const char* drop_id, IVarList& output_items,
        const PERSISTID &player);

	//���������Ʒ������ҵ���Ʒ�����"item_drop_record"�У�����player.xml���壩
	//��ĸ�ʽ3���ֶΣ�@itemid + @itemnumƴ�ӵı�ʾ������Ʒ���ַ�����@itemnum�ֶΣ�@itemcdtime�ֶ�
	static bool SaveItemsToPLayerDropRecord(IKernel *pKernel, const PERSISTID& player, std::string& item_id, int item_num);

	//�����ص�������ÿ5���Ӽ��һ��������ϵĵ�����Ʒ������ʱδ�����Ʒɾ��
	static int HB_CheckDropRecord(IKernel* pKernel, const PERSISTID& self, int slice);

	//��ʵ���䣬�ͻ���ʰȡ������Ʒ���Զ�����Ϣ�ص�����
	static int OnCustomPickUpDropItem(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	//�����ʵ������Ʒ�ӿ�
	// args �ĸ�ʽ
	//      [drop_item_id][color_level or drop_item_num][lock_state]
	static int PlayerDropItems(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ������Ƿ����
	static bool IsDropPkgExist(const char *pkg);
	// ���͵��������Ʒ���ʼ�
	static  void SendDropRecordItemByMail(IKernel*pKernel,const PERSISTID& self);
    
private:

    // �������Ʒ
    struct DropItem 
    {
        DropItem():item_type(0), drop_condition(""), item_config_id(""), amount(0), 
            weight(0), weight_min(0), weight_max(0){};
        int item_type;              // �������Ʒ������(����:��Ʒ���ӵ����)
        std::string drop_condition; // ��������(@Level>10,@Job=1)
        std::string item_config_id; // ��Ʒconfig_id
        int amount;                 // ��������
        int weight;                 // �ڵ�����е�Ȩ�ؼ�������
        int weight_min;
        int weight_max;
    };

    // ����ID����Ŀ
    struct DropEntry
    {
        DropEntry():drop_package(""), drop_all(0), drop_count(0){};
        std::string drop_package;   // �����
        int drop_all;               // �Ƿ�������ȫ����Ʒ
        int drop_count;             // ����İ�����
    };

    // ������е���Ʒ, ÿ����������԰������������Ʒ
    // key = package_name
    typedef std::map<std::string, std::vector<DropItem> > DropItemMap;

    // ����ID, ÿ������ID���԰������������Ŀ(�����)
    // key = drop_id, value = drop_entry
    typedef std::map<std::string, std::vector<DropEntry> > DropEntryMap;

    // �������Ʒ������
    typedef std::map<std::string, int > DropItemTypeMap;

private:

    // ���б��е�����Ʒ
    static int DropItemFromList(IKernel *pKernel, const std::vector<DropItem> * drop_items, 
        const DropEntry *drop_entry, const PERSISTID& player, IVarList &outpu_items);

    // ���Ӱ��е���, �������Ʒ����@result_list
    static int DropFromSubPackage(IKernel *pKernel, const std::string& sub_pck_name, const PERSISTID& player,
        std::vector<DropItem>& result_list);

    // ���˲����Բ���������Ʒ, �ѿ��Բ���������Ʒ����@result_list
    static int FilterItems(IKernel *pKernel, const std::vector<DropItem> * drop_items,
        const PERSISTID& player, std::vector<DropItem>& result_list);

    // ��@drop_tems �и���Ȩ�أ� �ѷ�����������Ʒ����@result_list
    static int SelectItemByWeight(std::vector<DropItem>& drop_items,
        std::vector<DropItem>& result_list);

    // �շ���ϵ�������
    static bool MatchDropCondition(IKernel *pKernel, const DropItem& drop_item, const PERSISTID& player);

    // ����һ������ [min, max] ��������������Ϊ��������
    // ���max��0, ����0
    static int GenerateDropFactor(int min, int max);

    // ������������Ʒ��Ȩ��
    static int CalculateItemWeight(std::vector<DropItem>& drop_items);

    // ���㲼�����ʽ�Ľ��
    static bool BoolExp(IKernel *pKernel, const PERSISTID& player, const char* src_exp);

    // ��������ֵ������洢�� @var_value��, ���ʹ����@var_type ��
    // ����ʧ�ܷ���false
    static bool SolveVariable(IKernel *pKernel, const PERSISTID& player,
        const std::string& var_name, std::string& var_value, int *var_type);

    // ��Ⲽ�����ʽ��ֵ
    static bool SolveBoolExp(int var_type, const std::string& left_val, const std::string& op,
        const std::string& right_val);

    //////////////////////////////////////////////////////////////////////////
    // ������������صĽӿ�
    //////////////////////////////////////////////////////////////////////////

    // ȡ����Ʒ����
    static int GetDropItemType(std::string &type_name);

    // ���ݵ���ID��ȡ��������
    static const std::vector<DropEntry> * GetDropEntry(const char *drop_id);

    // ���ݵ������õ�����Ʒ
    static const std::vector<DropItem> * GetPackageItems(const std::string &package_name);

    // ��ȡ�Ӱ��ĵ�����Ʒ
    static const std::vector<DropItem> * GetSubpackageItems(const std::string &package_name);

    // ��������
    bool LoadDropItemTypeConfig(IKernel *pKernel);

    // �����
    bool LoadDropPackageConfig(IKernel *pKernel);

    // �ӵ����
    bool LoadSubpacketConfig(IKernel *pKernel);

    // ����ID
    bool LoadDropIDConfig(IKernel *pKernel);

	static void ReloadConfig(IKernel* pKernel);
private:
    static DropItemMap m_PackageItems; // �������
    static DropEntryMap m_DropEntry;   // ����ID
    static DropItemTypeMap m_DropItemType; // �������Ʒ����
    static DropItemMap m_SubpackageItems;  // �ӵ����
public:

	static DropModule*          m_pDropModule;
	static ContainerModule*		m_pContainerBaseModule;
	static CapitalModule*       m_pCapitalModule;

};

#endif // End of include guard