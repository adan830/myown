// -------------------------------------------
// �ļ����ƣ� ShopUtils.cpp
// �ļ�˵���� �̵깤����
// �������ڣ� 2016/03/29
// �� �� �ˣ�  tongzt
// �޸��ˣ�
// -------------------------------------------

#include "ShopUtils.h"
#include "ShopBase.h"
#include "ShopManagerModule.h"
#include "FsGame/CommonModule/ContainerModule.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/Define/ServerCustomDefine.h"

#include <algorithm>
#include "utils/XmlFile.h"
#include "utils/string_util.h"
#include "utils/util_func.h"
#include "utils/custom_func.h"
#include "CommonModule/FunctionEventModule.h"
#include "ItemModule/ToolItem/ToolItemModule.h"
#include "../DropModule.h"
#include "Define/GameDefine.h"
#include "SystemFunctionModule/CapitalModule.h"


const char* const SHOP_MANAGER_FILE = "ini/SystemFunction/Shop/PersonalShop/ShopManager.xml";	// �̵��������
const char* const ITEM_PLANTS_FILE = "ini/SystemFunction/Shop/PersonalShop/ItemPlants.xml";	// ��Ʒ��Ŀ��

bool ShopUtils::Init(IKernel* pKernel)
{
	if (!ShopUtilClass::Instance()->LoadResource(pKernel))
	{
		return false;
	}

	return true;
}

bool ShopUtils::Shut(IKernel* pKernel)
{
	return ShopUtilClass::Instance()->ReleaseResource(pKernel);
}

// ������Դ
bool ShopUtils::LoadResource(IKernel* pKernel)
{
	/*<Property ID="1" StartLevel="32" ShopRuleFile="ini/rule/equip_build/equip_smelt_shop.xml"
	 *<ItemConfigFile="ini/rule/equip_build/equip_smelt_config.xml" FreeMaxCount="3" RefreshMaxCount="5" 
	 *<CapitalType="2" RefreshCostValue="10,20,40,80,160" FirstFree="0"/>*/

	ReleaseResource(pKernel);

	// ������Ʒ��Ŀ��
	LoadItemPlants(pKernel);

	// �̵����������
	std::string shop_manager_path = pKernel->GetResourcePath();
	shop_manager_path += SHOP_MANAGER_FILE;
	CXmlFile xml(shop_manager_path.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = shop_manager_path;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}

	LoopBeginCheck(a);
	for (int i = 0; i < (int)xml.GetSectionCount(); ++i)
	{
		LoopDoCheck(a);
		const char *secName = xml.GetSectionByIndex(i);
		if (StringUtil::CharIsNull(secName))
		{
			continue;
		}

		// ���Ϸ����̵�����
		int shop_type = atoi(secName);
		if (shop_type <= SHOP_TYPE_NONE 
			|| shop_type >= SHOP_TYPE_TOTAL)
		{
			continue;
		}

		// �ų��ظ����̵�����
		if (NULL != GetShopManagerData(shop_type))
		{
			continue;
		}
		ShopManagerData shop_manager;
		shop_manager.shop_type =shop_type;
		shop_manager.start_level = xml.ReadInteger(secName, "StartLevel", 0);
		shop_manager.shop_rule_file = xml.ReadString(secName, "ShopRuleFile", "");
		shop_manager.max_free_count = xml.ReadInteger(secName, "FreeMaxCount", 0);
		shop_manager.shop_capital_type = xml.ReadInteger(secName, "CapitalType", 0);
		xml.ReadList<int>(secName, "RefreshCostValue", ",", &shop_manager.refresh_cost_value);
		shop_manager.first_free = xml.ReadInteger(secName, "FirstFree", 0);
		xml.ReadList<std::string>(secName, "GridLimitProperty", ",", &shop_manager.grid_limit_property);
		xml.ReadList<int>(secName, "GridLimitCondition", ",", &shop_manager.grid_limit_condition);
		shop_manager.shop_vip_logic_name = xml.ReadString(secName, "ShopVipLogicName", "");
		xml.ReadList<int>(secName, "CanDiscount", ",", &shop_manager.discount_limit_list);

		m_vecShopManagerData.push_back(shop_manager);
	}

	// ��ȡ�̵�������á���Ʒ����
	LoopBeginCheck(b);
	for (int i = 0; i < (int)m_vecShopManagerData.size(); ++i)
	{
		LoopDoCheck(b);
		ShopManagerData shop_manager_data = (ShopManagerData)m_vecShopManagerData[i];
		const char *shop_rule_file = shop_manager_data.shop_rule_file.c_str();

		if (StringUtil::CharIsNull(shop_rule_file) )
		{
			continue;
		}

		// ��ȡ�̵����
		LoadShopRule(pKernel, shop_rule_file , (ShopTypes)shop_manager_data.shop_type);
	}

	return true;
}

// ��Դ�ͷ�
bool ShopUtils::ReleaseResource(IKernel* pKernel)
{
	return ReleaseAll(pKernel);
}

// �ͷ�
bool ShopUtils::ReleaseAll(IKernel* pKernel)
{
	// �ͷ��̵��������
	m_vecShopManagerData.clear();

	// �ͷ��̵��������
	m_vecGeneralShop.clear();

	// �ͷ���Ʒ��Ŀ��
	m_ItemPlantsMap.clear();

	return true;
}

// ������Ʒ��Ŀ������
bool ShopUtils::LoadItemPlants(IKernel* pKernel)
{
	/*<Property ID="1" ItemConfigID="item000001" CapitalType="1" 
	 *<ItemColor="1" ItemPrice="100" DiamondPrice="0"/>*/

	// ��Ʒ��Ŀ��
	std::string item_plants_file = pKernel->GetResourcePath();
	item_plants_file += ITEM_PLANTS_FILE;
	CXmlFile xml(item_plants_file.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = item_plants_file;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}

	LoopBeginCheck(c);
	for (int i = 0; i < (int)xml.GetSectionCount(); ++i)
	{
		LoopDoCheck(c);
		const char *secName = xml.GetSectionByIndex(i);
		if (StringUtil::CharIsNull(secName))
		{
			continue;
		}

		// ��Ʒ��Ŀ������
		const char *config_id = xml.ReadString(secName, "ItemConfigID", "");
		if (StringUtil::CharIsNull(config_id))
		{
			continue;
		}

		const int capital_type = xml.ReadInteger(secName, "CapitalType", 0);
		const int item_color = xml.ReadInteger(secName, "ItemColor", -1);

		ItemPlants item_plants;

		// �����۸�
		item_plants.main_price = xml.ReadFloat(secName, "ItemPrice", 0.0f);

		// Ԫ���Ӹ�
		item_plants.diamond_price = xml.ReadFloat(secName, "DiamondPrice", 0.0f);

		// �ۿ��б�
		const char *discount_str = xml.ReadString(secName, "DiscountList", "");
		item_plants.discount_weight_sum = 0;

		// �����ۿ۵�Ȩ��
		AssignDiscountWeight(discount_str, item_plants.discount_weight_sum, item_plants.discount_list);

		// ƴ��key = ��Ʒid_��������_��ƷƷ��
		char key[128] = {0};
		SPRINTF_S(key,"%s_%d_%d", config_id, capital_type, item_color);

		m_ItemPlantsMap.insert(std::make_pair(key, item_plants));
	}

	return true;
}

// �����ۿ۵�Ȩ��ֵ
void ShopUtils::AssignDiscountWeight(const char *str, int &weight_sum, DiscountList &discount_list)
{
	if (StringUtil::CharIsNull(str))
	{
		return;
	}

	// ȡ�ۿۺ�ÿ���ۿ۵�Ȩ��
	CVarList discounts;
	util_split_string(discounts, str, ";");

	// Ϊÿ���ۿ۷���Ȩ��
	LoopBeginCheck(d);
	for (int dis_num = 0; dis_num < (int)discounts.GetCount(); ++dis_num)
	{
		LoopDoCheck(d);
		DiscountRule discount_rule;
		CVarList one_discount;
		util_split_string(one_discount, discounts.StringVal(dis_num), ",");
		discount_rule.discount = StringUtil::StringAsFloat(one_discount.StringVal(0));	// �ۿ�
		int weigh_num = atoi(one_discount.StringVal(1));	// Ȩ��
		weigh_num =  weigh_num > 0 ? weigh_num : 1;

		// �ۿ�Ȩ�����ޡ����ޡ���Ȩ��
		discount_rule.lower_bound = weight_sum;
		weight_sum += weigh_num;
		discount_rule.upper_bound = weight_sum;

		discount_list.push_back(discount_rule);
	}

	return;
}

/*!
* @brief	 �����̵�������� 
* @param rule_file	�̵�����ļ�·��
* @param shop_type	�̵�����
* @return bool
*/
bool ShopUtils::LoadShopRule(IKernel* pKernel, const char* rule_file, const ShopTypes shop_type)
{
	// �����ļ��ĸ�ʽ��
	//<Property ID="1" Level="10" Index="equip_level_10" ColorLevel="3" 
	//< Amount="1" Odds="700" FusionPrice="4800" DiamondPrice="0"/>

	std::string rule_path = pKernel->GetResourcePath();
	rule_path += rule_file;

	CXmlFile xml(rule_path.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = rule_path;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}

	CVarList sec_list;
	xml.GetSectionList(sec_list);
	GeneralShop general_shop;
	general_shop.shop_type = (int)shop_type;	// �̵�����

	// �̵�������
	int sec_count = (int)sec_list.GetCount();
	LoopBeginCheck(e);
	for (int i = 0; i < sec_count; ++i)
	{
		LoopDoCheck(e);
		const char *sec = sec_list.StringVal(i);

		ShopRule shop_rule;
		shop_rule.player_level = xml.ReadInteger(sec, "Level", 0);
		shop_rule.grid_index = xml.ReadInteger(sec, "GridIndex", 0);
		shop_rule.weight_sum = 0;	// ��Ȩ�س�ʼ��Ϊ0��ͨ��������и�ֵ
		const char *drop_info = xml.ReadString(sec, "DropID", "");
		if (StringUtil::CharIsNull(drop_info))
		{
			continue;
		}

		// ȡ�������ÿ���������Ȩ��
		CVarList drop_list;
		util_split_string(drop_list, drop_info, ";");

		// Ϊÿ�����������Ȩ��
		LoopBeginCheck(f);
		for (unsigned int drop_num = 0; drop_num < drop_list.GetCount(); ++drop_num)
		{
			LoopDoCheck(f);
			DropPack drop_pack;
			CVarList drop;
			util_split_string(drop, drop_list.StringVal(drop_num), ",");
			drop_pack.drop_id = drop.StringVal(0);	// �����ID
			int weigh_num = atoi(drop.StringVal(1));	// �����Ȩ��
			weigh_num =  weigh_num > 0 ? weigh_num : 1;

			// �����Ȩ�����ޡ����ޡ���Ȩ��
			drop_pack.lower_bound = shop_rule.weight_sum;
			shop_rule.weight_sum += weigh_num;
			drop_pack.upper_bound = shop_rule.weight_sum;

			shop_rule.drop_pack_list.push_back(drop_pack);
		}

		if (shop_rule.grid_index < SHOP_GRID_INDEX_MIN
			|| shop_rule.grid_index > SHOP_GRID_INDEX_MAX)
		{
			continue;
		}

		ShopRuleMap::iterator it = general_shop.shopRuleMap.find(shop_rule.player_level);
		if (it == general_shop.shopRuleMap.end())
		{
			std::vector<ShopRule> rule_list;
			rule_list.push_back(shop_rule);
			general_shop.shopRuleMap.insert(ShopRuleMap::value_type(shop_rule.player_level, rule_list));
		}
		else
		{
			std::vector<ShopRule> &rule_list = it->second;
			rule_list.push_back(shop_rule);
		}
	}

	m_vecGeneralShop.push_back(general_shop);

	return true;
}

/*!
* @brief ����Ȩ��ȡ��Ʒ�����ۿ�
* @param discount_list ��Ʒ�����ۿ۱�
* @param weight_sum �ۿ۱���Ȩ��ֵ
* @return float ������Ʒ�����ۿ�
*/
const float ShopUtils::GetDiscountByBound(const DiscountList &discount_list, const int weight_sum)
{
	// ����Ȩ��ȡ�۸�
	int seed = util_random_int(weight_sum); // �������0~weight_sum
	LoopBeginCheck(g);
	for (int dis_num = 0; dis_num < (int)discount_list.size(); ++dis_num)
	{
		LoopDoCheck(g);	
		const DiscountRule &discount_rule = discount_list[dis_num];

		// ����Ȩ��
		if (seed >= discount_rule.lower_bound
			&& seed < discount_rule.upper_bound)
		{
			return discount_rule.discount;
		}
	}

	return 0.0f;
}

/*!
* @brief ѡ��һ���̵����
* @param grid_index �̵��������
* @param rule_list �̵��������
* @return ShopRule* ָ��Ψһ�̵����
*/
const ShopRule* ShopUtils::ChooseShopRule(const int grid_index, const std::vector<ShopRule> *rule_list)
{
	if (NULL == rule_list)
	{
		return NULL;
	}

	if (rule_list->empty())
	{
		return NULL;
	}


	std::vector<ShopRule>::const_iterator it = rule_list->begin();
	std::vector<ShopRule>::const_iterator it_end = rule_list->end();

	LoopBeginCheck(h);
	for (; it != it_end; ++it)
	{
		LoopDoCheck(h);
		const ShopRule *rule = &(*it);

		// �ҵ���Ӧ���ӵĵ����
		if (grid_index == rule->grid_index)
		{
			return rule;
		}
	}

	return NULL;
}

/*!
* @brief	 ѡ��һ������������ȡ�ã�
* @param items	һ���̵����
* @return const char* ����õ�һ�������ID
*/
const char* ShopUtils::ChooseDropPack(const ShopRule *shop_rule)
{
	if (NULL == shop_rule)
	{
		return "";
	}

	// �����
	const DropPackList &drop_pack_list = shop_rule->drop_pack_list;

	if (drop_pack_list.empty())
	{
		return "";
	}

	// �����
	int seed = util_random_int(shop_rule->weight_sum); // 0~shop_rule->weight_sum
	LoopBeginCheck(j);
	for (int i = 0; i < (int)drop_pack_list.size(); ++i)
	{
		LoopDoCheck(j);
		const DropPack &drop_pack = drop_pack_list[i];
		if (seed >= drop_pack.lower_bound 
			&& seed < drop_pack.upper_bound)
		{
			if (!(drop_pack.drop_id.empty()))
			{
				return drop_pack.drop_id.c_str();
			}
		}
	}

	return "";
}

/*!
* @brief	 �̵�����Ƿ�ɴ��� 
* @param shop_type	�̵�����
* @param row_index	Ҫ�������Ʒ����
* @return int	 0��ʾ�ɴ��ۣ�1��ʾ����
*/
int ShopUtils::ShopGridCanDiscount(IKernel *pKernel, const PERSISTID &self, 
								   const int shop_type, int row_index)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ȡ���̵�Ļ�������
	const ShopManagerData *shop_manager_data = GetShopManagerData(shop_type);
	if (NULL == shop_manager_data)
	{
		return 0;
	}

	// ���������������̵��������һһ��Ӧ
	int discount_index = (int)shop_manager_data->discount_limit_list.size() - 1;
	// ȡ�úϷ���������
	if (discount_index < 0)
	{
		return 0;
	}

	// �ҵ���Ӧ�����������������һ��ֵΪ׼
	if (discount_index >= row_index)
	{
		discount_index = row_index;
	}
	else
	{
		return 0;
	}

	return shop_manager_data->discount_limit_list[discount_index];
}

/*!
* @brief	  ȡ�ö�Ӧ���͵��̵�������� 
* @param shop_type	�̵�����
* @return �õ����̵��������
*/
const ShopManagerData* ShopUtils::GetShopManagerData(const int shop_type)
{
	// ���ݼ���
	struct ShopManagerDataFinder
	{
		ShopManagerDataFinder(int shop_type):_shop_type(shop_type){};

		bool operator ()(const ShopManagerData &data)
		{
			return _shop_type == data.shop_type;
		}

		int _shop_type;
	};

	// ����shop_type��Ӧ������
	ShopManagerDataVector::const_iterator iter = std::find_if(ShopUtilClass::Instance()->m_vecShopManagerData.begin(),
		ShopUtilClass::Instance()->m_vecShopManagerData.end(), ShopManagerDataFinder(shop_type));
	if (iter == ShopUtilClass::Instance()->m_vecShopManagerData.end())
	{
		return NULL;
	}

	return &(*iter);
}

/*!
* @brief	  ȡ�ö�Ӧ���͵��̵���� 
* @param shop_type	�̵�����
* @return �õ����̵����
*/
const GeneralShop* ShopUtils::GetShopRule(const int shop_type) const
{
	// ��������
	struct GeneralShopFinder
	{
		GeneralShopFinder(int shop_type):_shop_type(shop_type){};

		bool operator ()(const GeneralShop &data)
		{
			return _shop_type == data.shop_type;
		}

		int _shop_type;
	};

	// ����shop_type���̵����
	GeneralShopVector::const_iterator iter = std::find_if(ShopUtilClass::Instance()->m_vecGeneralShop.begin(),
		ShopUtilClass::Instance()->m_vecGeneralShop.end(), GeneralShopFinder(shop_type));
	if (iter == ShopUtilClass::Instance()->m_vecGeneralShop.end())
	{
		return NULL;
	}

	return &(*iter);
}

/*!
* @brief ȡ�ö�Ӧ��Ʒ��Ŀ��
* @param shop_type �̵����ͣ��ݴ�ȡ�û������ͣ�
* @param config_id ��ƷID
* @param item_color ��ƷƷ��
* @param main_price ����ȡ�õ���Ʒ��������
* @param diamond_price ����ȡ�õ���ƷԪ������
* @param discount ����ȡ�õ���Ʒ�ۿ�
* @return bool ����ֵ trueȡ�� falseδȡ��
*/
bool ShopUtils::GetItemPlants(IKernel* pKernel, const int shop_type, 
							  const char *config_id, const int item_color, float &main_price, float &diamond_price, float &discount)
{
	// ����shop_typeȡ���̵����
	const ShopManagerData *shop_manager = GetShopManagerData(shop_type);
	if (NULL == shop_manager)
	{
		return false;
	}

	// ��������
	int capital_type = shop_manager->shop_capital_type;

	// ƴ��key = ��Ʒid_��������_��ƷƷ��
	char key[128] = {0};
	SPRINTF_S(key,"%s_%d_%d", config_id, capital_type, item_color);

	ItemPlants item_plants;
	ItemPlantsMap::const_iterator iter = m_ItemPlantsMap.find(key);
	if (iter == m_ItemPlantsMap.end())
	{
		// δ�ҵ�
		extend_warning(LOG_ERROR, "[%s][%d]Can't get items price from plants[%s]", __FILE__, __LINE__, key);
		return false;
	}

	// �ҵ����ʵļ�Ŀ��
	item_plants = iter->second;

	// �����۸�
	main_price = item_plants.main_price;

	// ����Ԫ���۸�
	diamond_price = item_plants.diamond_price;

	// �ۿ�
	discount = GetDiscountByBound(item_plants.discount_list, item_plants.discount_weight_sum);

	return true;
}

// �� float ת��INT, ����ȡ��
int ShopUtils::Float2IntRoundUp(float f)
{
	int ret = (int) f;
	int temp = (int) (f * 10);

	if (temp % 10 == 0)
	{
		return ret;
	}
	else
	{
		return ret + 1;
	}
}

/*!
* @brief �����̵���������һ����Ӧ�ȼ����̵���Ʒ
* @param shop_type �̵�����
* @param grid_index �̵��������
* @param output_item	��������Ʒ��һ����
* @return bool
*/
bool ShopUtils::GenerateShopItem(IKernel *pKernel, const PERSISTID &self, 
								 const int shop_type, const int grid_index, IVarList& output_item)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	const GeneralShop *general_shop = GetShopRule(shop_type);
	if (NULL == general_shop)
	{
		return false;
	}

	int player_level = pSelfObj->QueryInt("Level");

	int level_x0 = player_level/10 * 10;	    // ���� 10, 20, 30 �����ļ���
	int level_x5 = player_level/10 * 10 + 5;	// ���� 15, 25, 35 ��������

	const std::vector<ShopRule> *rule_list = NULL;

	// Ϊ����ҵ����ʵĵȼ���
	int test_level = level_x0;	// player_level >= level_x0 ���� player_level < level_x5
	if (player_level >= level_x5)
	{
		test_level = level_x5;	// player_level >= level_x5 ���� player_level < level_x0
	}

	// �ҵ����õ���ߵȼ��Ĺ����б�
	LoopBeginCheck(k);
	while (test_level >= 0)
	{
		LoopDoCheck(k);
		ShopRuleMap::const_iterator it = general_shop->shopRuleMap.find(test_level);
		if (it == general_shop->shopRuleMap.end())
		{
			test_level -= 5;	// δ�ҵ���������һ���ȼ���
		}
		else
		{
			rule_list = &(it->second);
			break;
		}
	}

	if (NULL == rule_list)
	{
		return false;
	}

	// ѡȡһ������
	const ShopRule *shop_rule = ChooseShopRule(grid_index, rule_list);

	return OutPutItems(pKernel, self, shop_rule, shop_type, grid_index, output_item);
}

/*!
* @brief	 ������Ʒ
* @param shop_type	�̵�����
* @param grid_index	�̵����
* @param output_items	�س���Ʒ��һ��������
* @return bool
*/
bool ShopUtils::OutPutItems(IKernel *pKernel, const PERSISTID &self, 
							const ShopRule *shop_rule, const int shop_type, const int grid_index, IVarList& output_item)
{
	if (NULL == shop_rule)
	{
		return false;
	}

	// ���ȡ��һ�������
	const char *drop_pack = ChooseDropPack(shop_rule);
	if (StringUtil::CharIsNull(drop_pack))
	{
		return false;
	}

	CVarList output_of_drop;
	// �������������Ʒ
	DropModule::m_pDropModule->GetItemsFromDropID(pKernel, drop_pack, output_of_drop, self);
	int item_counts = (int)output_of_drop.GetCount()/2;
	if (item_counts < 1)
	{
#if defined _DEBUG
		output_item << "1"
							  << -1
							  << 1
							  << 0
							  << 0
							  << 100;
		extend_warning(LOG_ERROR, "[%s][%d]Can't get items from dropID[%s]", __FILE__, __LINE__, drop_pack);
		return false;
#endif
		output_item << "Item2010001"
							  << -1
							  << 1
							  << 999999
							  << 0
							  << 100;
		extend_warning(LOG_ERROR, "[%s][%d]Can't get items from dropID[%s]", __FILE__, __LINE__, drop_pack);
		return false;
	}

	// ���һ����Ʒ
	int item_index = util_random_int(item_counts);
	const char *item_config = output_of_drop.StringVal(item_index*2);
	int item_amount = output_of_drop.IntVal(item_index*2 + 1);
	int item_color = 1;	//item_amount ����������Ʒ�۸�

	if (StringUtil::CharIsNull(item_config))
	{
		return false;
	}

	// ����ƷǱ������Ʒ��װ����Ʒ��Ϊ����������Ϊ1
	if (ToolItemModule::IsCanWearItem(pKernel, item_config))	// ��װ�����߷���
	{
		item_color = item_amount;
		item_amount = 1;
	}

	//��Ʒ����
	float main_price = 0.0f;
	float diamond_price = 0.0f;
	float discount = 0.0f;
	GetItemPlants(pKernel, shop_type, item_config, item_color, main_price, diamond_price, discount);

	// �жϸ����Ƿ�ɴ���
	discount = discount > 0 ? discount : 100.0f;
	discount = ShopGridCanDiscount(pKernel, self, shop_type, grid_index) == 0 ? discount : 100.0f;

	// �����Ʒ��Ϣ
	output_item << item_config
					      << item_color
						  << item_amount
						  << main_price
						  << diamond_price
						  << discount;

	return true;
}

/*!
* @brief	 ��ȡ��һ��ˢ�µıس���Ʒ
* @param shop_type	�̵�����
* @param grid_index	�̵����
* @param output_items	�س���Ʒ��һ��������
* @return bool
*/
bool ShopUtils::GenerateShopItemForFirstRefresh(IKernel *pKernel, const PERSISTID &self, 
												const int shop_type, IVarList& output_items)
{
	const GeneralShop *general_shop =  GetShopRule(shop_type);
	if (NULL == general_shop)
	{
		return false;
	}

	// �ҵ���һ��ˢ�µĹ���
	const std::vector<ShopRule> *rule_list = NULL;
	ShopRuleMap::const_iterator it_rule_map = general_shop->shopRuleMap.find(SHOP_FIRST_REFRESH_FLAG);	// ��һ��ˢ�����ù���
	if (it_rule_map == general_shop->shopRuleMap.end())
	{
		return false;
	}
	else
	{
		rule_list = &(it_rule_map->second);
	}

	if (rule_list->empty())
	{
		return false;
	}

	std::vector<ShopRule>::const_iterator it_rule = rule_list->begin();
	std::vector<ShopRule>::const_iterator it_rule_end = rule_list->end();

	int grid_idx = 0;
	LoopBeginCheck(l);
	for (; it_rule != it_rule_end; ++it_rule)
	{
		LoopDoCheck(l);
		const ShopRule *shop_rule = &(*it_rule);
		if (NULL == shop_rule)
		{
			continue;
		}

		// ��Ʒ����
		if (OutPutItems(pKernel, self, shop_rule, shop_type, grid_idx, output_items))
		{
			++grid_idx;
		}
	}

	return true;
}

/*!
* @brief	 �̵�������޿��ƣ�������Ʒ�����Ƿ����� 
* @param shop_type	�̵�����
* @param row_index	Ҫ�������Ʒ����
* @return int	0��ʾ�����ޣ�1��ʾ������
*/
const int ShopUtils::CalGridLimitCondition(IKernel *pKernel, const PERSISTID &self, 
										   const int shop_type, int row_index)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ȡ���̵�Ļ�������
	const ShopManagerData *shop_manager_data = GetShopManagerData(shop_type);
	if (NULL == shop_manager_data)
	{
		return 0;
	}

	// ����������������Ʒ�ĸ�������һһ��Ӧ
	int property_index = (int)shop_manager_data->grid_limit_property.size() - 1;
	// ȡ�úϷ�������������
	if (property_index < 0)
	{
		return 0;
	}
	// �ҵ���Ӧ���������������������һ������Ϊ׼
	if (property_index > row_index)
	{
		property_index = row_index;
	}

	// ����������
	const char *property_name = shop_manager_data->grid_limit_property[property_index].c_str();
	if (StringUtil::CharIsNull(property_name))
	{
		return 0;
	}

	// �������Բ�����
	if (!pSelfObj->FindAttr(property_name))
	{
		return 0;
	}

	// �������� 
	int property_type = pSelfObj->GetAttrType(property_name);
	int limit_value = 0;
	// Ĭ��ת��Ϊint������ֵ
	if (VTYPE_INT == property_type)
	{
		limit_value = pSelfObj->QueryInt(property_name);
	}
	else if (VTYPE_INT64 == property_type)
	{
		limit_value = (int)pSelfObj->QueryInt64(property_name);
	}
	else if (VTYPE_FLOAT == property_type)
	{
		limit_value = (int)pSelfObj->QueryFloat(property_name);
	}
	else if (VTYPE_DOUBLE == property_type)
	{
		limit_value = (int)pSelfObj->QueryDouble(property_name);
	}
	else
	{
		// �����ܷ���ֵ�͵�����
		return 0;
	}

	// ����������������������Ʒ�ĸ�������һһ��Ӧ
	int condition_index = (int)shop_manager_data->grid_limit_condition.size() - 1;
	if (condition_index < 0)
	{
		return 0;
	}

	// �ҵ���Ӧ�������������������㣬�����һ������Ϊ׼
	if (condition_index > row_index)
	{
		condition_index = row_index;
	}

	// ����������
	if (limit_value < shop_manager_data->grid_limit_condition[condition_index])
	{
		return 1;
	}

	return 0;
}

/*!
* @brief	  ȡ�ò�ͬ�����̵긶��ˢ�µ��������� 
* @param shop_type	�̵�����
* @return int	����ˢ�µ����������������VipLevel�й�
*/
const int ShopUtils::GetShopMaxBuyCount(IKernel *pKernel, const PERSISTID &self, const int shop_type)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// �̵��������
	const ShopManagerData *shop_manager_data = GetShopManagerData(shop_type);
	if (NULL == shop_manager_data)
	{
		return 0;
	}

	return 0/* VipModule::m_pVipModule->GetNumber(pKernel, self, shop_manager_data->shop_vip_logic_name)*/;
}

/*!
* @brief	 �����̵�����ȡ���´�ˢ�¼۸�
* @param shop_type	�̵�����
* @param cur_refresh_count	��ǰ���ˢ�´���
* @param cur_pay_refresh_count	��ǰ����ˢ�´���
* @return int �����´�ˢ�¼۸� -1��ʾ�Ƿ�����
*/
const int ShopUtils::GetNextRefreshCostValue(IKernel* pKernel, const PERSISTID& self, 
											 const int shop_type, int cur_refresh_count /*= 0*/, int cur_pay_refresh_count /*= 0*/)
{
	const ShopManagerData *shop_manager_data = GetShopManagerData(shop_type);
	if (NULL == shop_manager_data)
	{
		return -1;
	}

	// ��ǰ���ˢ�´��� = ��ǰˢ�´��� - ��ǰ����ˢ�´���
	int cur_free_refresh_count = cur_refresh_count - cur_pay_refresh_count;
	if (shop_manager_data->max_free_count > cur_free_refresh_count)	// �������ˢ�£�Ĭ���´�ˢ�¼۸�Ϊ0
	{
		return 0;
	}

	// �����´�ˢ�¼۸�
	int cost_index = (int)shop_manager_data->refresh_cost_value.size() - 1;
	if (cost_index < 0)
	{
		return 0;
	}

	if (cost_index > cur_pay_refresh_count)
	{
		cost_index = cur_pay_refresh_count;
	}

	// ���ض�Ӧ�����ü۸�
	return shop_manager_data->refresh_cost_value[cost_index];
}

/*!
* @brief	 ����ֶ�ˢ���̵�
* @param shop_type	�̵�����
* @param free_refresh	�Ƿ񸶷�ˢ��
* @return void
*/
void ShopUtils::DoRefreshShop(IKernel* pKernel, const PERSISTID& self, 
							  const int shop_type, bool free_refresh /*= true*/)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// �̵�����
	IRecord *pShopManagerRec = pSelfObj->GetRecord(FIELD_RECORD_SHOP_MANAGER_REC);
	if (NULL == pShopManagerRec)
	{
		return;
	}

	// �̵겻���ڻ�δ����
	int exist_row = pShopManagerRec->FindInt(SHOP_REC_TYPE, shop_type);
	if (exist_row < 0)
	{
		return;
	}

	// �̵겻����
	const ShopManagerData *shop_manager_data = GetShopManagerData(shop_type);
	if (NULL == shop_manager_data)
	{
		return;
	}

	// ����̵�ָ��
	ShopBase *pShop = ShopManagerModule::m_pShopManagerModule->GetShop(shop_type);
	if (NULL == pShop)
	{
		return;
	}
	const ShopRefreshInfo* refresh_info = pShop->GetShopRefreInfo();
	if (NULL == refresh_info)
	{
		return;
	}

	// ��ǰˢ�´���
	int cur_refresh_count = pShopManagerRec->QueryInt(exist_row, SHOP_REC_CUR_REFRESH_COUNT);
	// ����ˢ�´���
	int pay_refresh_count = pShopManagerRec->QueryInt(exist_row, SHOP_REC_CUR_PAY_REFRESH_COUNT);
	// ���ˢ��������
	int max_free_refresh_count = pShopManagerRec->QueryInt(exist_row, SHOP_REC_MAX_FREE_REFRESH_COUNT);
	// ˢ�¼۸�
	int refresh_cost = pShopManagerRec->QueryInt(exist_row, SHOP_REC_REFRESH_COSTVALUE);

	// ����ˢ����������
	int max_pay_refresh_count = GetShopMaxBuyCount(pKernel, self, shop_type);

	// ���ˢ��ʣ�����
	int remain_free_count = max_free_refresh_count - (cur_refresh_count - pay_refresh_count);
	if (remain_free_count <= 0 && pay_refresh_count >= max_pay_refresh_count && free_refresh == false)
	{
		// ����ˢ�´���������
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19002, CVarList());
		return;
	}

	// ��Ѵ��������꣬���и���ˢ�´���
	if (remain_free_count <= 0 && free_refresh)
	{
		// ֪ͨ�ͻ��˵�������ˢ�µĽ���
		CVarList pay_msg;
		pay_msg << SERVER_CUSTOMMSG_BUY_INFO
						<< refresh_info->vip_buy_type
						<< refresh_cost		// ���򻨷�
						<< pay_refresh_count	// ��ǰ�������
						<< max_pay_refresh_count;	// ��������
		pKernel->Custom(self, pay_msg);
		return;
	}

	// ����ˢ�¼۸�
	if (refresh_cost > 0)
	{
		// �ʽ��Ƿ�
		__int64 sliver_have = CapitalModule::m_pCapitalModule->GetCapital(pKernel, self, CAPITAL_GOLD);
		if (refresh_cost > sliver_have)
		{
			// Ԫ������
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_7011, CVarList());

			return;
		}

		// �۳��ʽ�
		if (DC_SUCCESS != CapitalModule::m_pCapitalModule->DecCapital(pKernel, self, 
			CAPITAL_GOLD, refresh_cost, refresh_info->lose_capital_log))
		{
			// ��Ԫ��ʧ��
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_7011, CVarList());
			return;
		}

		// ��¼һ�θ���ˢ�´���
		pShopManagerRec->SetInt(exist_row, SHOP_REC_CUR_PAY_REFRESH_COUNT, ++pay_refresh_count);
	}

	// ִ��ˢ��
	++cur_refresh_count;	// ��ǰˢ�´�������	
	// �����´�ˢ�¼۸�������ˢ�´���
	int next_refresh_cost = GetNextRefreshCostValue(pKernel, self, shop_type, 
		cur_refresh_count, pay_refresh_count);

	next_refresh_cost = next_refresh_cost < 0 ? 0 : next_refresh_cost;

	pShopManagerRec->SetInt(exist_row, SHOP_REC_CUR_REFRESH_COUNT, cur_refresh_count);
	pShopManagerRec->SetInt(exist_row, SHOP_REC_REFRESH_COSTVALUE, next_refresh_cost);

	// ����ˢ��
	RefreshShopItem(pKernel, self, shop_type);

	// �淨��־-�̵�ˢ��
	/*GamePlayerActionLog log;
	log.actionType    =   refresh_info->game_action_log;
	log.actionState   =    LOG_GAME_ACTION_JOIN;
	LogModule::m_pLogModule->SaveGameActionLog(pKernel, self, log);

	LogModule::m_pLogModule->OnShopRefresh(pKernel, self, shop_type, refresh_cost);*/
}

/*!
* @brief	 ��ҵ�һ��ˢ���̵꣨Ϊ�����ṩ����Ѳ�����ˢ�£�
* @param shop_type	�̵�����
* @return void
*/
void ShopUtils::OnFirstTimeRefreshShop(IKernel* pKernel, const PERSISTID& self, 
									   const int shop_type)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// ����̵�ָ��
	ShopBase *pShop = ShopManagerModule::m_pShopManagerModule->GetShop(shop_type);
	if (NULL == pShop)
	{
		return;
	}

	// ��Ʒ��
	IRecord * pRec = pSelfObj->GetRecord(pShop->GetShopRecName());
	if (NULL == pRec)
	{
		return;
	}

	pRec->ClearRow();

	CVarList dropItems; // ������Ʒ��
	int total_cell_count = pRec->GetRowMax(); // �̵�����
	int item_attr_count = 6; // ÿ����Ʒ��5������

	// ��ȡ�ñ���Ҫ������Ʒ
	GenerateShopItemForFirstRefresh(pKernel, self, shop_type, dropItems);

	// �����̵�ʣ��ĸ���
	int used_cell_count = (int)dropItems.GetCount()/item_attr_count;

	// ʣ�µĸ��Ӱ�����������������Ʒ
	LoopBeginCheck(m);
	for (int i = used_cell_count; i < total_cell_count; ++i)
	{
		LoopDoCheck(m);
		GenerateShopItem(pKernel, self, shop_type, i, dropItems);
	}

	int item_count = (int)(dropItems.GetCount()/item_attr_count);
	if(item_count > total_cell_count)
	{
		item_count = total_cell_count;
	}

	int item_index = -1;
	LoopBeginCheck(n);
	for (int i = 0; i < item_count; ++i)
	{
		LoopDoCheck(n);
		std::string itemConfig = dropItems.StringVal(++item_index);
		int itemColor = dropItems.IntVal(++item_index);
		int itemAmount = dropItems.IntVal(++item_index);
		int price = dropItems.IntVal(++item_index);
		int diamond = dropItems.IntVal(++item_index);
		float discount = dropItems.FloatVal(++item_index);

		if (!itemConfig.empty() && itemAmount > 0)
		{
			CVarList new_row;
			new_row << itemConfig;
			new_row << itemColor;
			new_row << itemAmount;
			new_row << price;
			new_row << diamond;
			new_row << discount;

			pRec->AddRowValue(-1, new_row);
		}
	}
}

/*!
* @brief	 �̵꿪��
* @param shop_type	�̵�����
* @return void
*/
void ShopUtils::OpenShop(IKernel* pKernel, const PERSISTID& self, const int shop_type)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// �̵�����
	IRecord *pShopManagerRec = pSelfObj->GetRecord(FIELD_RECORD_SHOP_MANAGER_REC);
	if (NULL == pShopManagerRec)
	{
		return;
	}

	// ��ҵȼ�
	int self_level = pSelfObj->QueryInt("Level");

	const ShopManagerData *shop_manager_shop = ShopUtilClass::Instance()->GetShopManagerData(shop_type);
	if (NULL == shop_manager_shop)
	{
		return;
	}

	// �Ѿ�����
	if (pShopManagerRec->FindInt(SHOP_REC_TYPE, shop_type) >= 0)
	{
		return;
	}

	// δ����������Ƿ�ﵽ�����ȼ�
	if (self_level < shop_manager_shop->start_level)
	{
		return;
	}

	// �����´�ˢ�¼۸�
	int next_refresh_cost = ShopUtilClass::Instance()->GetNextRefreshCostValue(pKernel, self, shop_type);
	if (-1 == next_refresh_cost)
	{
		return;
	}

	// �����̵꣬�̵��������һ����¼
	CVarList row_value;
	row_value << shop_type	// �̵�����
					  << 0	// ��ǰˢ�´���
					  << 0	// ��ǰ����ˢ�´���
					  << shop_manager_shop->max_free_count		// ���ˢ��������
					  << shop_manager_shop->shop_capital_type	// �̵�ͨ�û�������
					  << next_refresh_cost	// ˢ�»���
					  << shop_manager_shop->first_free;	// �Ƿ��һ���ֶ�ˢ��
	pShopManagerRec->AddRowValue(-1, row_value);

	// ����̵�ָ��
	ShopBase *pShop = ShopManagerModule::m_pShopManagerModule->GetShop(shop_type);
	if (NULL == pShop)
	{
		return;
	}

	// ��һ��ˢ���̵�
	IRecord * pRec = pSelfObj->GetRecord(pShop->GetShopRecName());
	if (NULL == pRec)
	{
		return;
	}

	if (pRec->GetRows() == 0)
	{
		ShopUtilClass::Instance()->RefreshShopItem(pKernel, self, shop_type);
	}
}

/*!
* @brief	 �̵���Ʒˢ��ͨ�ô���
* @param shop_type	�̵�����
* @return void
*/
void ShopUtils::RefreshShopItem(IKernel *pKernel, const PERSISTID &self, const int shop_type)
{
	// ��Ҳ�����
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// �̵�����
	IRecord *pShopManagerRec = pSelfObj->GetRecord(FIELD_RECORD_SHOP_MANAGER_REC);
	if (NULL == pShopManagerRec)
	{
		return;
	}

	// �̵겻���ڻ�δ����
	int exist_row = pShopManagerRec->FindInt(SHOP_REC_TYPE, shop_type);
	if (exist_row < 0)
	{
		return;
	}

	// ����̵�ָ��
	ShopBase *pShop = ShopManagerModule::m_pShopManagerModule->GetShop(shop_type);
	if (NULL == pShop)
	{
		return;
	}

	// �̵겻����
	IRecord * pRec = pSelfObj->GetRecord(pShop->GetShopRecName());
	if (NULL == pRec)
	{
		return;
	}

	pRec->ClearRow();	// �����һ����Ʒ��

	// ������ҵȼ�, ����̵�
	CVarList dropItems;
	LoopBeginCheck(o);
	for (int i = 0; i < pRec->GetRowMax(); ++i)
	{
		LoopDoCheck(o);

		// ������ҵȼ�Ϊ�ض���������һ���̵���Ʒ
		ShopUtilClass::Instance()->GenerateShopItem(pKernel, self, shop_type, i, dropItems);
	}

	int item_index = -1;
	int item_count = (int)(dropItems.GetCount()/6);
	LoopBeginCheck(p);
	for (int i = 0; i < item_count; ++i)
	{
		LoopDoCheck(p);
		std::string itemConfig = dropItems.StringVal(++item_index);
		int itemColor = dropItems.IntVal(++item_index);
		int itemAmount = dropItems.IntVal(++item_index);
		int price = dropItems.IntVal(++item_index);
		int diamond = dropItems.IntVal(++item_index);
		float discount = dropItems.FloatVal(++item_index);

		if (!itemConfig.empty() && itemAmount > 0)
		{
			CVarList new_row;
			new_row << itemConfig;
			new_row << itemColor;
			new_row << itemAmount;
			new_row << price;
			new_row << diamond;
			new_row << discount;

			pRec->AddRowValue(-1, new_row);
		}
	}

	// ���²�ѯ�̵���Ϣ�����͸��ͻ���
	ShopUtilClass::Instance()->RetrieveShop(pKernel, self, shop_type);
}

/*!
* @brief	 �̵�ˢ�´���ͨ�ô���
* @param shop_type	�̵�����
* @return void
*/
void ShopUtils::RefreshShopTimes(IKernel *pKernel, const PERSISTID &self, const int shop_type)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// �̵�����
	IRecord *pShopManagerRec = pSelfObj->GetRecord(FIELD_RECORD_SHOP_MANAGER_REC);
	if (NULL == pShopManagerRec)
	{
		return;
	}

	// �̵겻���ڻ�δ����
	int exist_row = pShopManagerRec->FindInt(SHOP_REC_TYPE, shop_type);
	if (exist_row < 0)
	{
		return;
	}

	// �����̵�ĵ�ǰˢ�´������Ѹ���ˢ�´������´�ˢ�¼۸�
	int refresh_cost_value = ShopUtilClass::Instance()->GetNextRefreshCostValue(pKernel, self, shop_type);

	refresh_cost_value = refresh_cost_value < 0 ? 0 : refresh_cost_value;

	pShopManagerRec->SetInt(exist_row, SHOP_REC_CUR_REFRESH_COUNT, 0);	// ��ǰˢ�´���
	pShopManagerRec->SetInt(exist_row, SHOP_REC_CUR_PAY_REFRESH_COUNT, 0);	// ��ǰ����ˢ�´���
	pShopManagerRec->SetInt(exist_row, SHOP_REC_REFRESH_COSTVALUE, refresh_cost_value);	// �´�ˢ�¼۸�

	// ֪ͨ�ͻ���ˢ�³ɹ�
	CVarList args;
	args << SERVER_CUSTOMMSG_SHOP_INFO
			<< SC_SUB_SHOP_REFRESH_STAT
			<< shop_type
			<< pShopManagerRec->QueryInt(exist_row, SHOP_REC_MAX_FREE_REFRESH_COUNT)	// ʣ�����ˢ�´���
			<< refresh_cost_value; // �´�ˢ�¼۸�
	pKernel->Custom(self, args);
}

/*!
* @brief	����ֶ�ˢ���̵�
* @param shop_type	�̵�����
* @return void
*/
void ShopUtils::OnPlayerRefreshShop(IKernel* pKernel, const PERSISTID& self, const int shop_type, bool free_refresh)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// �̵�����
	IRecord *pShopManagerRec = pSelfObj->GetRecord(FIELD_RECORD_SHOP_MANAGER_REC);
	if (NULL == pShopManagerRec)
	{
		return;
	}

	// �̵겻���ڻ�δ����
	int exist_row = pShopManagerRec->FindInt(SHOP_REC_TYPE, shop_type);
	if (exist_row < 0)
	{
		return;
	}

	// �ж��Ƿ���ҪΪ������һ����Ѳ�����ˢ�� 0��ʾ��Ҫ 1��ʾ����Ҫ
	int first_time_refresh = pShopManagerRec->QueryInt(exist_row, SHOP_REC_REFRESH_ISFIRST);
	if (0 == first_time_refresh)
	{
		// ��һ��ˢ���̵�����ѵģ�ͬʱҲ��ռ�����ˢ�»���
		ShopUtilClass::Instance()->OnFirstTimeRefreshShop(pKernel, self, shop_type);

		// ���ñ�ǩΪ��ʹ�ù���Ѳ�����
		pShopManagerRec->SetInt(exist_row, SHOP_REC_REFRESH_ISFIRST, 1);

		// ���²�ѯ�̵���Ϣ���͸��ͻ���
		ShopUtilClass::Instance()->RetrieveShop(pKernel, self, shop_type);
	}
	else
	{
		// ��������ˢ��
		ShopUtilClass::Instance()->DoRefreshShop(pKernel, self, shop_type, free_refresh);
	}
}

/*!
* @brief	�����̵���Ʒ���� 
* @param shop_type	�̵�����
* @param buy_index	Ҫ�������Ʒ����
* @param buy_amount	Ҫ�������Ʒ����
* @return void
*/
void ShopUtils::DoBuyShopItem(IKernel *pKernel, const PERSISTID &self, 
							  const int shop_type, int buy_index, int buy_amount)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// �̵�����
	IRecord *pShopManagerRec = pSelfObj->GetRecord(FIELD_RECORD_SHOP_MANAGER_REC);
	if (NULL == pShopManagerRec)
	{
		return;
	}

	// �̵겻����
	int exist_row = pShopManagerRec->FindInt(SHOP_REC_TYPE, shop_type);
	if (exist_row < 0)
	{
		return;
	}

	// ����̵�ָ��
	ShopBase *pShop = ShopManagerModule::m_pShopManagerModule->GetShop(shop_type);
	if (NULL == pShop)
	{
		return;
	}
	const ShopBuyLog *buy_log = pShop->GetShopBuyLog();
	if (NULL == buy_log)
	{
		return;
	}

	// ȡ���̵꽻�׻�������
	int capital_type = pShopManagerRec->QueryInt(exist_row, SHOP_REC_TRADE_CAPITALTYPE);

	// �����Ƿ����
	PERSISTID tool_box = pKernel->GetChild(self, L"ItemBox");
	if (!pKernel->Exists(tool_box))
	{
		return;
	}

	// �̵겻����
	IRecord * pRec = pSelfObj->GetRecord(pShop->GetShopRecName());
	if (NULL == pRec)
	{
		return;
	}

	// ��������
	if (buy_index < 0 || buy_index >= pRec->GetRows())
	{
		return;
	}

	// ����Ʒ�Ƿ�������
	if (1 == ShopUtilClass::Instance()->CalGridLimitCondition(pKernel, self, shop_type, buy_index))
	{
		// û�й������Ʒ��Ȩ��
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19003, CVarList());
		return;
	}

	// ��Ʒ��������
	int item_amount = pRec->QueryInt(buy_index, COMMON_SHOP_REC_NUM);
	if (buy_amount > item_amount)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19004, CVarList());
		return;
	}

	//������Ʒ����������
	int item_leftAmount = item_amount - buy_amount;

	// ��ù�����Ʒid��Ʒ�ʺ�����
	const char * item_config = pRec->QueryString(buy_index, COMMON_SHOP_REC_CONFIG);
	int item_color = pRec->QueryInt(buy_index, COMMON_SHOP_REC_COLORLVL);
	

	// �˴�Ǳ����,(��Ʒ��Ʒ�ʺ������Ĺ�ϵ)
	int real_amount = buy_amount; // ��Ʒ��ʵ����
	if (ToolItemModule::IsCanWearItem(pKernel, item_config))
	{
		// �����װ������ģ����Ʒ�ʸ��������� ��Ϊ�ڴ�����Ʒ��ʱ��װ�����������Իᱻ����Ʒ��
		buy_amount = item_color;
		real_amount = 1;
		item_leftAmount = 0;
	}
	if (StringUtil::CharIsNull(item_config) || item_amount < 0)
	{
		return;
	}

	// �����ռ��Ƿ�
	int nBindState = FunctionEventModule::GetItemBindState(buy_log->logSrcFunction);
	if (ContainerModule::TryAddItem(pKernel, tool_box, item_config, buy_amount, nBindState) < real_amount)
	{
		// �����ռ䲻��
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_7004, CVarList());
		return;
	}

	// �ۿ�
	float discount = pRec->QueryFloat(buy_index, COMMON_SHOP_REC_DISCOUNT);

	// ��Ҫ���׻��Ҽ۸�
	int price = pRec->QueryInt(buy_index, COMMON_SHOP_REC_PRICE) * real_amount;
	price = ShopUtilClass::Instance()->Float2IntRoundUp(price*discount/100);

	// Ԫ���������ͣ��Ǳ���
	int need_diamond = pRec->QueryInt(buy_index, COMMON_SHOP_REC_DIAMOND) * real_amount;
	need_diamond = ShopUtilClass::Instance()->Float2IntRoundUp(need_diamond*discount/100);
	if (price <= 0 && need_diamond <= 0)	// �۸��쳣
	{
		return;
	}

	// �۸��Ƿ�
	int64_t have_capital = CapitalModule::m_pCapitalModule->GetCapital(pKernel, self, capital_type);
	int64_t have_diamond = CapitalModule::m_pCapitalModule->GetCapital(pKernel, self, CAPITAL_GOLD);
	if ((int64_t)price > have_capital)
	{
		// ���׻��Ҳ���
		CustomSysInfo(pKernel, self, buy_log->lack_capital, CVarList());
		return;
	}

	// ��ʯ�Ƿ�
	if ((int64_t)need_diamond > have_diamond)
	{
		// Ԫ������
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_7011, CVarList());
		return;
	}

	// �ۻ���
	if (price > 0 && DC_SUCCESS != CapitalModule::m_pCapitalModule->DecCapital(pKernel, self, 
		capital_type, price, buy_log->logSrcFunction))
	{
		// �ۻ���ʧ��
		CustomSysInfo(pKernel, self, buy_log->pay_failed, CVarList());
		return;
	}

	// ��Ԫ��
	if (need_diamond > 0 && DC_SUCCESS != CapitalModule::m_pCapitalModule->DecCapital(pKernel, self, 
		CAPITAL_GOLD, need_diamond, buy_log->logSrcFunction))
	{
		// ��Ԫ��ʧ��
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_7012, CVarList());
		// �����ѿۻ���
		if (price > 0)
		{
			// ��������ʧ�ܿ۵���Ԫ
			CapitalModule::m_pCapitalModule->IncCapital(pKernel, self, capital_type,
				price, FUNCTION_EVENT_ID_SHOPING_FAILED_RET);
		}

		return;
	} 

	// ������Ʒ
	const int put_cnt = ContainerModule::PlaceItem(pKernel, tool_box, item_config, buy_amount, 
					(EmFunctionEventId)buy_log->logSrcFunction, true, nBindState);

	// ����ɹ�
	if (put_cnt > 0)
	{
		pRec->SetInt(buy_index, COMMON_SHOP_REC_NUM, item_leftAmount);

		// ֪ͨ�ͻ��˹�����Ʒ����Ϣ
		CVarList buy_args;
		buy_args << SERVER_CUSTOMMSG_SHOP_INFO
						<< SC_SUB_SHOP_BUY_SUC
						<< shop_type
						<< buy_index
						<< item_config
						<< item_leftAmount
						<< real_amount;
		pKernel->Custom(self, buy_args);
	}
	else	// ����ʧ�ܣ������ѿ۽��
	{
		// ����ʧ��
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19001, CVarList());

		// �����ѿۻ���
		if (price > 0)
		{
			// ��������ʧ�ܿ۵Ļ���
			CapitalModule::m_pCapitalModule->IncCapital(pKernel, self, capital_type,
				price, FUNCTION_EVENT_ID_SHOPING_FAILED_RET);
		}

		// �����ѿ�Ԫ��
		if (need_diamond > 0)
		{
			// ��������ʧ�ܿ۵���Ԫ
			CapitalModule::m_pCapitalModule->IncCapital(pKernel, self, CAPITAL_GOLD,
				need_diamond, FUNCTION_EVENT_ID_SHOPING_FAILED_RET);
		}

		// ��¼����ʧ����Ʒ
		extend_warning(LOG_WARNING, "[%s][%d] buy failed[%d:%s:%d:%d]", __FILE__, __LINE__, 
			shop_type, item_config, price, need_diamond);
	}

	// �淨��־-�̵깺��
	/*GamePlayerActionLog actionLog;
	actionLog.actionType = buy_log->logSrcFunction;
	actionLog.actionState = LOG_GAME_ACTION_JOIN;
	LogModule::m_pLogModule->SaveGameActionLog(pKernel, self, actionLog);

	int nLogCapitalType = need_diamond > 0 ? CAPITAL_GOLD : capital_type;
	int nLogCapitalCost = need_diamond > 0 ? need_diamond : price;
	LogModule::m_pLogModule->OnShopExchange(pKernel, self, shop_type, nLogCapitalType, nLogCapitalCost, item_config, item_amount);*/
}

/*!
* @brief	��ѯ�̵���ϸ��Ϣ����Ʒ��Ϣ�����ˢ�´���������ˢ�½��ȣ�
* @param shop_type	�̵�����
* @return void
*/
void ShopUtils::RetrieveShop(IKernel* pKernel, const PERSISTID& self, 
							 const int shop_type)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// �̵�����
	IRecord *pShopManagerRec = pSelfObj->GetRecord(FIELD_RECORD_SHOP_MANAGER_REC);
	if (NULL == pShopManagerRec)
	{
		return;
	}

	// �̵겻���ڻ�δ����
	int exist_row = pShopManagerRec->FindInt(SHOP_REC_TYPE, shop_type);
	if (exist_row < 0)
	{
		return;
	}

	// ����̵�ָ��
	ShopBase *pShop = ShopManagerModule::m_pShopManagerModule->GetShop(shop_type);
	if (NULL == pShop)
	{
		return;
	}

	// �̵겻����
	IRecord * shop_rec = pSelfObj->GetRecord(pShop->GetShopRecName());
	if (NULL == shop_rec)
	{
		return;
	}

	// ��������
	int capital_type = pShopManagerRec->QueryInt(exist_row, SHOP_REC_TRADE_CAPITALTYPE);

	// ������ˢ�´���
	int max_free_count = pShopManagerRec->QueryInt(exist_row, SHOP_REC_MAX_FREE_REFRESH_COUNT);
	// ��ˢ�´���
	int cur_refresh_count = pShopManagerRec->QueryInt(exist_row, SHOP_REC_CUR_REFRESH_COUNT);
	// �Ѹ���ˢ�´���
	int cur_pay_refresh_count = pShopManagerRec->QueryInt(exist_row, SHOP_REC_CUR_PAY_REFRESH_COUNT);
	// ʣ����Ѵ���
	int remain_free_count = max_free_count - (cur_refresh_count - cur_pay_refresh_count);
	remain_free_count = remain_free_count < 0 ? 0 : remain_free_count;

	// �´�ˢ����Ҫ�ļ۸�
	int next_refresh_cost = pShopManagerRec->QueryInt(exist_row, SHOP_REC_REFRESH_COSTVALUE);
	next_refresh_cost = next_refresh_cost < 0 ? 0 : next_refresh_cost;

	// ��Ʒ�б�
	CVarList items_in_shop;
	int items_count = shop_rec->GetRows();
	int items_in_shop_count = 0;

	LoopBeginCheck(q);
	for (int i = 0; i < items_count; ++i)
	{
		LoopDoCheck(q);
		CVarList item;
		shop_rec->QueryRowValue(i, item);
                   
		// ��Ҫ����ÿ����Ʒ�����Ƿ�������
		items_in_shop << i 
								 << item 
								 << ShopUtilClass::Instance()->CalGridLimitCondition(pKernel, self, shop_type, i);
		++items_in_shop_count;
	}

	// ��ѯ�����̵������Ʒ
	CVarList shop_info;
	shop_info << SERVER_CUSTOMMSG_SHOP_INFO
					 << SC_SUB_SHOP_RETRIEVE
					 << shop_type	// �̵�����
					 << capital_type	// �̵꽻�׻���
					 << remain_free_count	// ʣ�����ˢ�´���
					 << next_refresh_cost	// �´�ˢ�»���
					 << items_in_shop_count	// ��Ʒ����
					 << items_in_shop;	// ��Ʒ��Ϣ

	pKernel->Custom(self, shop_info);
}

// ��������̵�ĵ������������Ʒ���۸��Ƿ���������
void ShopUtils::ValidateShopConf(IKernel *pKernel, const PERSISTID &self)
{
	LoopBeginCheck(x)
		for (int shops = 0; shops < (int)m_vecGeneralShop.size(); ++shops)
		{
			LoopDoCheck(x);
			const GeneralShop &general_shop = m_vecGeneralShop[shops];
			const ShopRuleMap &shop_rule_map = general_shop.shopRuleMap;
			const int shop_type = general_shop.shop_type;
			ShopRuleMap::const_iterator it1 = shop_rule_map.begin();
			LoopBeginCheck(y)
				while (it1 != shop_rule_map.end())
				{
					LoopDoCheck(y);
					const std::vector<ShopRule> &shop_rule_list = it1->second;
					LoopBeginCheck(z)
						for (int rules = 0; rules < (int)shop_rule_list.size(); ++rules)
						{
							LoopDoCheck(z);
							const ShopRule & shop_rule = shop_rule_list[rules];
							const DropPackList &drop_pack_list = shop_rule.drop_pack_list;
							LoopBeginCheck(aa)
								for (int drops = 0; drops < (int)drop_pack_list.size(); ++drops)
								{
									LoopDoCheck(aa);
									const DropPack & drop_pack = drop_pack_list[drops];
									const char *drop_id = drop_pack.drop_id.c_str();
									CVarList output_of_drop;
									// �������������Ʒ
									DropModule::m_pDropModule->GetItemsFromDropID(pKernel, drop_id, output_of_drop, self);
									int item_counts = (int)output_of_drop.GetCount()/2;
									if (item_counts < 1)
									{
										extend_warning(LOG_ERROR, "[%s][%d]Can't get items from dropID[%s] of Level[%d] from shop_type[%d]", 
											__FILE__, __LINE__, drop_id, shop_rule.player_level, shop_type);
										continue;
									}
									// ���һ����Ʒ
									LoopBeginCheck(bb)
										for (int indexs = 0; indexs < item_counts; ++indexs)
										{
											LoopDoCheck(bb);
											const char *item_config = output_of_drop.StringVal(indexs*2);
											int item_amount = output_of_drop.IntVal(indexs*2 + 1);
											int item_color = 1;	// ����������Ʒ�۸�

											if (StringUtil::CharIsNull(item_config))
											{
												continue;
											}	

											// ����ƷǱ������Ʒ��װ����Ʒ��Ϊ����������Ϊ1
											if (ToolItemModule::IsCanWearItem(pKernel, item_config))	// ��װ�������ʯ
											{
												item_color = item_amount;
												item_amount = 1;
											}

											float main_price = 0.0f;
											float diamond_price = 0.0f;
											float discount = 0.0f;
											GetItemPlants(pKernel, shop_type, item_config, item_color, main_price, diamond_price, discount);
										}
								}
						}

						++it1;
				}
		}

}