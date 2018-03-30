// -------------------------------------------
// �ļ����ƣ� ShopUtils.h
// �ļ�˵���� �̵깤����
// �������ڣ� 2016/03/29
// �� �� �ˣ�  tongzt
// �޸��ˣ�
// -------------------------------------------
#ifndef _SHOP_UTILS_H_
#define _SHOP_UTILS_H_

#include "Fsgame/Define/header.h"
#include "ShopManagerDefine.h"
#include "utils/singleton.h"

class ShopUtils
{
public:
	// ��ʼ��
	static bool Init(IKernel* pKernel);

	// �ͷ�
	static bool Shut(IKernel* pKernel);

public:
	// ������Դ
	bool LoadResource(IKernel* pKernel);

	// �ͷ������ļ�
	bool ReleaseResource(IKernel* pKernel);

	/************************************************************************/
	/*									         ��Դ����														   */
	/************************************************************************/
private:
	// ��Դ�ͷ�
	bool ReleaseAll(IKernel* pKernel);

	// ������Ʒ��Ŀ������
	bool LoadItemPlants(IKernel* pKernel);

	// �����ۿ۵�Ȩ��ֵ
	void AssignDiscountWeight(const char *str, int &weight_sum, DiscountList &discount_list);

	// �����̵��������
	bool LoadShopRule(IKernel* pKernel, const char* rule_file, const ShopTypes shop_type);

	// ����Ȩ��ȡ��Ʒ�����ۿ�
	const float GetDiscountByBound(const DiscountList &discount_list, const int weight_sum);

	// ѡ��һ���̵����
	const ShopRule* ChooseShopRule(const int grid_index, const std::vector<ShopRule> *rule_list);

	// ѡ��һ������������ȡ�ã�
	const char* ChooseDropPack(const ShopRule *shop_rule);

	// �̵�����Ƿ�ɴ���
	// ����ֵ��0��ʾ�ɴ��ۣ�1��ʾ����
	int ShopGridCanDiscount(IKernel *pKernel, const PERSISTID &self, const int shop_type, int row_index);

	// ȡ�ö�Ӧ���͵��̵��������
	const ShopManagerData* GetShopManagerData(const int shop_type);

	// ȡ�ö�Ӧ���͵��̵����
	const GeneralShop* GetShopRule(const int shop_type) const;

	// ȡ�ö�Ӧ��Ʒ��Ŀ��
	bool GetItemPlants(IKernel* pKernel, const int shop_type, const char *config_id, 
		const int item_color, float &main_price, float &diamond_price, float &discount);

	// �� float ת��INT, ����ȡ��
	int Float2IntRoundUp(float f);

	/************************************************************************/
	/*									         ��Ʒ����														   */
	/************************************************************************/

	// ������Ʒ
	bool OutPutItems(IKernel *pKernel, const PERSISTID &self, 
		const ShopRule *shop_rule, const int shop_type, const int grid_index, IVarList& output_item);

	// �����̵���������һ����Ӧ�ȼ����̵���Ʒ
	bool GenerateShopItem(IKernel *pKernel, const PERSISTID &self, 
		const int shop_type, const int grid_index, IVarList& output_item);

	// ��ȡ��һ��ˢ�µıس���Ʒ
	bool GenerateShopItemForFirstRefresh(IKernel *pKernel, const PERSISTID &self, 
		const int shop_type, IVarList& output_items);

	// �̵�������޿��ƣ�������Ʒ�����Ƿ�����
	// ����ֵ��0��ʾ�����ޣ�1��ʾ������
	const int CalGridLimitCondition(IKernel *pKernel, const PERSISTID &self, 
		const int shop_type, int row_index);

	// ȡ�ò�ͬ�����̵����������
	const int GetShopMaxBuyCount(IKernel *pKernel, const PERSISTID &self, 
		const int shop_type);

	// ���ݵ�ǰˢ�´������̵�����ȡ���´�ˢ�¼۸�
	const int GetNextRefreshCostValue(IKernel* pKernel, const PERSISTID& self, 
		const int shop_type, int cur_refresh_count = 0, int cur_pay_refresh_count = 0);

	// ���ˢ���̵�
	void DoRefreshShop(IKernel* pKernel, const PERSISTID& self, 
		const int shop_type, bool free_refresh = true);

	// ��ҵ�һ��ˢ���̵�
	void OnFirstTimeRefreshShop(IKernel* pKernel, const PERSISTID& self, 
		const int shop_type);

	/************************************************************************/
	/*									         �ⲿ�ӿ�														   */
	/************************************************************************/
public:
	// �̵꿪��
	void OpenShop(IKernel* pKernel, const PERSISTID& self, 
		const int shop_type);

	// �̵���Ʒˢ��ͨ�ô��� 
	void RefreshShopItem(IKernel *pKernel, const PERSISTID &self, 
		const int shop_type);

	// �̵�ˢ�´���ͨ�ô���
	void RefreshShopTimes(IKernel *pKernel, const PERSISTID &self, const int shop_type);

	// ���ˢ���̵�
	void OnPlayerRefreshShop(IKernel* pKernel, const PERSISTID& self, 
		const int shop_type, bool free_refresh);

	// �����̵���Ʒ����
	void DoBuyShopItem(IKernel *pKernel, const PERSISTID &self, 
		const int shop_type, int buy_index, int buy_amount);

	// ��ѯ�̵�
	void RetrieveShop(IKernel* pKernel, const PERSISTID& self, 
		const int shop_type);

	//////////////////////////////////��Ʒ���GM����////////////////////////////////////////

	// ��������̵�ĵ������������Ʒ���۸��Ƿ���������
	void ValidateShopConf(IKernel *pKernel, const PERSISTID &self);

private:
	ShopManagerDataVector m_vecShopManagerData;
	GeneralShopVector m_vecGeneralShop;
	ItemPlantsMap m_ItemPlantsMap;
};

// ����ģʽ
typedef HPSingleton<ShopUtils> ShopUtilClass;

#endif	// _SHOP_UTILS_H_