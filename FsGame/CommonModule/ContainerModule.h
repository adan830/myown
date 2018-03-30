//---------------------------------------------------------
//�ļ���:       ContainerModule.h
//��  ��:       ����
//˵  ��:       
//          
//��������:      2014��10��31��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------
#ifndef FSGAME_COMMON_CONTAINER_MODULE_H_
#define FSGAME_COMMON_CONTAINER_MODULE_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/ContainerDefine.h"

#include <vector>
#include <list>
#include <map>
#include "FsGame/Define/FunctionEventDefine.h"

class LogModule;
class AsynCtrlModule;
class CoolDownModule;
class ItemBaseModule;
//class EquipmentModule;

class ContainerModule : public ILogicModule
{
public:

    //��ʼ��
    bool Init(IKernel* pKernel);
    //�ͷ�
    bool Shut(IKernel* pKernel);


public:
	//////////////////////////////////////////////////////////////////////////
	// ����ά���ӿ�
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ��ѯ�ӿ�
	//////////////////////////////////////////////////////////////////////////

	// ��ѯ��Ʒ�������е�λ�ã�����0��ʾ������
	static int GetItemPos(IKernel* pKernel, const PERSISTID& container,
		const PERSISTID& request_item);

	//���ָ����Ʒ������
	static int GetItemCount(IKernel* pKernel, const PERSISTID& container,
		const char* configid, int match = ITEM_MATCH_ALL);

	// �������в���ָ������Ʒ, �����ҵ��ĵ�һ����Ʒ
	// �Ҳ�����ʱ�򷵻�һ����Ч��PERSISTID
	static PERSISTID FindItem(IKernel* pKernel, const PERSISTID& container,
		const char* configid, int match = ITEM_MATCH_ALL);
	// �������в���ָ������Ʒ, ���ȷ����ҵ��󶨵ĵ�һ����Ʒ
	// �Ҳ�����ʱ�򷵻�һ����Ч��PERSISTID
	static PERSISTID FindItemBindPrior(IKernel* pKernel, const PERSISTID& container,
		const char* configid, int match = ITEM_MATCH_ALL);

	// �������в���ָ�����Ե���Ʒ, ͨ��UID��
	// �Ҳ�����ʱ�򷵻�һ����Ч��PERSISTID,����ָ�������౳��������ʹ��ToolBox
	static PERSISTID FindItemByUid(IKernel* pKernel, const PERSISTID& container,
		const char * uid);

	// ����@self�Ƿ���Բ���ָ������container
	static bool CanOperateContainer(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& container, int optype, int pos);

	//�����������Ч����(�ų�������)������ָ�������౳��������ʹ��ToolBox
	static int GetAvailableCapacity(IKernel* pKernel, const PERSISTID& container);

	//��������е�������Ʒ������ָ�������౳��������ʹ��ToolBox
	static int GetAllItems(IKernel* pKernel, const PERSISTID& container, IVarList& res);

	//���һ�����е�λ��, ����0��ʾ�޿���λ�ã�����ָ�������౳��������ʹ��ToolBox
	static int GetFreePos(IKernel* pKernel, const PERSISTID& container);

	// ��ȡ�����п���λ�õ�����,����ָ�������౳��������ʹ��ToolBox
	static int GetFreePosCount(IKernel* pKernel, const PERSISTID& container);

	//////////////////////////////////////////////////////////////////////////
	// ������Ʒ�ӿ�
	//////////////////////////////////////////////////////////////////////////

	// ���԰�@item��������@container��, ���ؿ��Է��������
	// �˲�����������������Ʒ, �����Ǽ���ɷ��������
	// @destpos = 0 ��ʾ��ָ������λ��
	static int TryAddItem(IKernel* pKernel, const PERSISTID& container,
		const PERSISTID& item, int destpos = 0);

	// ���԰�ָ������@item����Ʒ��������@container��, ���ؿ��Է��������
	// �˲�����������������Ʒ, �����Ǽ���ɷ��������
	static int TryAddItem(IKernel* pKernel, const PERSISTID& container, SBoxTestItem* request);

	// ���԰�ָ������@count����Ʒ@config��������@container��, ���ؿ��Է��������
	// �˲�����������������Ʒ, �����Ǽ���ɷ�������� frozenTime: ����ʱ��
	static int TryAddItem(IKernel* pKernel, const PERSISTID& container,
		const char* config, int count, int bound = ITEM_NOT_BOUND, int64_t frozenTime = 0);

	// ���԰Ѷ����Ʒ����������, ȫ�����Է��뷵��true
	static bool TryAddItems(IKernel* pKernel, const PERSISTID& container,VectorBoxTestItem *item_list);
	// ���԰Ѷ����Ʒ����������, ȫ�����Է��뷵��true
	static bool TryAddItems(IKernel* pKernel, BoxItemData& item_datas);

	// �������Ʒ��������,, ʧ�ܷ���false
	// �����ȷ��ȫ�����룬���ȵ���TryAddItems()�����ж�
	static bool PlaceItems( IKernel* pKernel, BoxItemData& item_datas, EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true );
	static bool PlaceItems(IKernel* pKernel, const PERSISTID& container, VectorBoxTestItem *item_list,
							EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true);

	// ����Ʒ��ŵ�������(destpos==0,��ָ��λ��), ���ط��������
	// ʹ��TryAddItem() ���Բ�����Ʒ�ܷ�ȫ������
	static int PlaceItem(IKernel* pKernel, const PERSISTID& container, const PERSISTID& item,
				EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true, int destpos = 0);

	//����Ʒ��ŵ�������, ���ط�������� frozenTime: ����ʱ��
	static int PlaceItem(IKernel* pKernel, const PERSISTID& container,
				const char *config_id, int request_amount,
				EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true,
				int bound = ITEM_NOT_BOUND, int64_t frozenTime = 0);

	// ����Ʒ���뱳�������ط��������.  frozenTime: ����ʱ��
	// @saved_items �ﷵ��ʵ�ʱ���Ķ��������, ��ʽΪ:
	//      [CONFIG_ID][UNIQUE_ID][SAVED_AMOUNT]...[CONFIG_ID][UNIQUE_ID][SAVED_AMOUNT]
	static int PlaceItem(IKernel* pKernel, const PERSISTID& container,
		const char *config_id, int request_amount, CVarList &saved_items, 
		EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true,
		int bound = ITEM_NOT_BOUND, int64_t frozenTime = 0);

	//��Ʒ���뱳���������ռ䲻�������ʱ����, ��ʱ�����ռ䲻�㷵��false
	static bool PlaceItemsToBagOrTemp(IKernel* pKernel, const PERSISTID& self,
					const char *config_id, int request_amount, 
					EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true,
					int bound = ITEM_NOT_BOUND, int64_t frozenTime = 0);
	static bool PlaceItemsToBagOrTemp(IKernel* pKernel, const PERSISTID& self, VectorBoxTestItem *item_list,
					EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true);
public:
	// ���Դ�������ɾ��ָ����Ʒ������, ����ɾ��ָ������������true
	// �˲�����������ɾ����Ʒ
	static bool TryRemoveItems(IKernel* pKernel, const PERSISTID& container,
		const char* configid, int count, int match = ITEM_MATCH_ALL);
	static bool TryRemoveItems( IKernel* pKernel, const PERSISTID& container, 
		const std::map<std::string, int>& items, int match = ITEM_MATCH_ALL);

	// ��������ɾ��ָ����Ʒ������, ����ɾ��������, ��պ�����
	static int RemoveItem(IKernel* pKernel, const PERSISTID& container,
		const PERSISTID& item, int count,
		EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsConsume = true );
	// ��������ɾ��ָ����Ʒ������, ����ɾ��������
	static int RemoveItems(IKernel* pKernel, const PERSISTID& container,
		const char* configid, int count,
		EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsConsume = true, 
		int match = ITEM_MATCH_ALL);
	// ��������ɾ��ָ����Ʒ������, ȫ��ɾ������true, ���򷵻�false result����ɾ������
	static bool RemoveItems( IKernel* pKernel, const PERSISTID& container, 
		const std::map<std::string, int>& items, std::map<std::string, int>* pResult /*= NULL*/, 
		EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsConsume = true,
		int match = ITEM_MATCH_ALL);

	// ��������ɾ��ָ����Ʒ������, ����ɾ�������� �����Ƴ��󶨵ģ����ܱ�RemoveItemsҪ��
	static int RemoveItemsBindPrior(IKernel* pKernel, const PERSISTID& container,
		const char* configid, int count, 
		EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsConsume = true, 
		int match = ITEM_MATCH_ALL);
	// ��������ɾ��ָ����Ʒ������, ȫ��ɾ������true, ���򷵻�false result����ɾ������ �����Ƴ��󶨵ģ����ܱ�RemoveItemsҪ��
	static bool RemoveItemsBindPrior(IKernel* pKernel, const PERSISTID& container,
		const std::map<std::string, int>& items, std::map<std::string, int>* pResult = NULL, 
		EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsConsume = true,
		int match = ITEM_MATCH_ALL);
public:
	// �������������ָ�������౳��������ʹ��ToolBox
	static bool ClearAllItems(IKernel* pKernel, const PERSISTID& self, const PERSISTID& container);

	//������Ʒ������������
	bool LoadBoxRule(IKernel* pKernel);

	//��Ʒ����������Script
	static const char* GetBoxScript(IKernel* pKernel, const char* itemConfig);

	//��Ʒ��������
	static PERSISTID GetBoxContainer(IKernel* pKernel, const PERSISTID& self, const char* itemConfig);

	//////////////////////////////////////////////////////////////////////////
	// ��������ӿ�
	//////////////////////////////////////////////////////////////////////////

	//���������е���Ʒ, @cooldown_ms ��ȴʱ��(����)
	static bool ArrangeItem(IKernel* pKernel, const PERSISTID& container, int cooldown_ms = 0);

	// �����ַ�������Ʒ ��ð�źͶ��ŷָ� ����:"��Ʒid:����,��Ʒid:����"
	static bool ParseItems(IKernel* pKernel, const char* strItems, VectorBoxTestItem& itemList, int num = 1, int nBindState = ITEM_NOT_BOUND);

	// ����Ʒ�б������ϲ�, ���غϲ��������
	static int MergeItemData(IKernel* pKernel, const PERSISTID& defBox, const VectorBoxTestItem& itemList, BoxItemData& boxItemData);

	// �Ƿ���Ҫ��¼��Ʒ��־
	static bool IsNeedSaveItemLog(IKernel* pKernel, const char* szItemId, int nColorLv);
private:
    //�ص�����
    static int OnNoAdd(IKernel* pKernel, const PERSISTID& container,
        const PERSISTID& sender, const IVarList& args);

    static int OnNoRemove(IKernel * pKernel, const PERSISTID & container, 
        const PERSISTID & sender, const IVarList & args);

    static int OnAfterAdd(IKernel * pKernel, const PERSISTID & container, 
        const PERSISTID & sender, const IVarList & args);

    static int OnRemove(IKernel * pKernel, const PERSISTID & container, 
        const PERSISTID & sender, const IVarList & args);

	//////////////////////////////////////////////////////////////////////////
	// ���ڻص��Ľӿ�
	//////////////////////////////////////////////////////////////////////////

	/// ������ҵ���Ϣ
	static int OnClientMsg(IKernel* pKernel, const PERSISTID& owener, 
		const PERSISTID& sender, const IVarList& args);

	// �ƶ�������Ʒ
	static int OnMoveItem(IKernel* pKernel, const PERSISTID& owener, 
		const PERSISTID& sender, const IVarList& args);

private:
    //����Ʒ��ŵ�������, ���ط��������
    static int PlaceItem(IKernel* pKernel, const PERSISTID& container, SBoxTestItem *request,
		EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true);

    // ����Ʒ���뱳�������ط��������. 
    // @saved_items �ﷵ��ʵ�ʱ���Ķ��������, ��ʽΪ:
    //      [CONFIG_ID][UNIQUE_ID][SAVED_AMOUNT]...[CONFIG_ID][UNIQUE_ID][SAVED_AMOUNT]
	static int PlaceItem(IKernel* pKernel, const PERSISTID& container,
						SBoxTestItem *request, CVarList &saved_items,
						EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true);


    //////////////////////////////////////////////////////////////////////////
    // �޸���Ʒ�ӿ�
    //////////////////////////////////////////////////////////////////////////       

private:
    // ������Ʒ������, ����ʵ�����ӵ�����
    static int IncItemAmount(IKernel* pKernel, const PERSISTID &item, int amount,
        bool trigger_event = true);

    // ɾ����Ʒ������, ����ʵ��ɾ��������
    static int DecItemAmount(IKernel* pKernel, const PERSISTID &item, int amount,
        bool trigger_event = true);

private:
    static int GetAllItems(IKernel* pKernel, const PERSISTID& container, VectorBoxTestItem *item_list);

    // �Ƿ���Ժϲ���Ʒ
    static bool CanMergeItem(IKernel* pKernel, const PERSISTID& srcitem, const PERSISTID& destitem);

    // �Ƿ���԰�@request ����Ʒ�ϲ���@dest_item ��
    static bool CanMergeItem(IKernel* pKernel, const PERSISTID& dest_item, const SBoxTestItem *request);

    // �Ƿ���Ժϲ�������Ʒ
    static bool CanMergeItem(IKernel* pKernel, const SBoxTestItem *dest_item, const SBoxTestItem *request_item);

	// �ж�self�Ƿ���������ӵ����
	static bool SelfIsOwner(IKernel* pKernel, const PERSISTID& self, const PERSISTID& container);

private:
    // ���԰�@item��������@container��, ���ؿ��Է��������
    // �˲�����������������Ʒ, �����Ǽ���ɷ��������
    static int TryAddItemAuto(IKernel* pKernel, const PERSISTID& container,
                               const PERSISTID& srcitem);

    // ���԰�@item��������@container�е�ָ��λ��@pos��, ���ؿ��Է��������
    // �˲�����������������Ʒ, �����Ǽ���ɷ��������
    static int TryAddItemToPosition(IKernel* pKernel, const PERSISTID& container,
                                     const PERSISTID& srcitem, int pos);

    //�Զ���ŵ�������, ���ط��������
    static int PlaceItemAuto(IKernel* pKernel, const PERSISTID& owener, const PERSISTID& container,
					const PERSISTID& item, EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true);

    //���ڵ�������ָ��λ��, ���ط��������
	static int PlaceItemToPosition(IKernel* pKernel, const PERSISTID& owener, const PERSISTID& container,
		const PERSISTID& item, int destpos, EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS, bool bIsGain = true);		

	// ��������ɾ���ɴ�����Ʒ(װ�����ʯ)
	static int RemoveWearItem(IKernel* pKernel, const PERSISTID& player, const PERSISTID& item, EmFunctionEventId srcFunctionId = FUNCTION_EVENT_ID_SYS);
private:
	// �����Ʒ����  (nColorLv ֻ��װ��ʱ����)
	static void OnGainItem(IKernel* pKernel, const PERSISTID& player, const char* szItemId, int nNum, int nTatalNum, EmFunctionEventId src, int nColorLv);

	// ������Ʒ����	(nColorLv ֻ��װ��ʱ����)
	static void OnLoseItem(IKernel* pKernel, const PERSISTID& player, const char* szItemId, int nNum, int nTatalNum, EmFunctionEventId src);

private:
    // ��������Ĳ�����־λ
    static bool CheckContainerOperFlag(IKernel* pKernel, const PERSISTID& container, int opType);

    // �ж��Ƿ���һ����Ч��λ��(�����ĸ��Ӳ�����Ч��λ��)
    static bool IsValidPos(IKernel* pKernel, const PERSISTID& container, int pos);
       
    // ��ȡ����λ���У������Ǹ�
    static int GetMaxUsablePos(IKernel* pKernel, const PERSISTID& container);

	static void ReloadConfig(IKernel* pKernel);
public:

    static ContainerModule* m_pContainerModule;

public:
	static AsynCtrlModule* m_pAsynCtrlModule;
    static CoolDownModule* m_pCoolDownModule;
    static ItemBaseModule* m_pItemBaseModule;
    //static EquipmentModule* m_pEquipmentModule;

private:
	// itemtypeֵ������
	struct ItemTypeLim 
	{
		int		nMinVal;		// ��Сֵ
		int		nMaxVal;		// ���ֵ
		ItemTypeLim() : nMinVal(0),
						nMaxVal(0)
		{

		}
	};

    std::map<std::string, ItemTypeLim> m_mapBoxRule;

};

#endif // FSGAME_COMMON_CONTAINER_MODULE_H_
