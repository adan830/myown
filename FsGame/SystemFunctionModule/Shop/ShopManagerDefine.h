// -------------------------------------------
// �ļ����ƣ� ShopManagerDefine.h
// �ļ�˵���� �̵�������ݶ���
// �������ڣ� 2016/03/29
// �� �� �ˣ�  tongzt
// �޸��ˣ�
// -------------------------------------------

#ifndef _SHOP_MANAGER_DEFINE_H_
#define _SHOP_MANAGER_DEFINE_H_

#include <vector>
#include <string>
#include <map>
#include "FsGame/Define/LogDefine.h"

#define SHOP_GRID_INDEX_MIN 0	// �̵����������Сֵ
#define SHOP_GRID_INDEX_MAX 11	// �̵�����������ֵ
#define SHOP_FIRST_REFRESH_FLAG   -1  // ���ڻ�ȡ��һ��ˢ���̵�Ĺ���ı�ʶ

// �̵������
struct ShopManagerData
{
	ShopManagerData(): 
		shop_type(0), 
		start_level(0), 
		shop_rule_file(""), 
		max_free_count(0), 
		shop_capital_type(0),
		first_free(0), 
		shop_vip_logic_name("")
	{
		refresh_cost_value.clear();
		grid_limit_property.clear();
		grid_limit_condition.clear();
		discount_limit_list.clear();
	}

	int shop_type;												// �̵�����
	int start_level;												// �����ȼ�
	std::string shop_rule_file;								// �̵�����ļ�
	int max_free_count;										// ���ˢ��������
	int shop_capital_type;									// �̵�ʹ�õĻ�������
	std::vector<int> refresh_cost_value;				// ˢ�»���
	int first_free;													// ��һ���ֶ�ˢ���Ƿ���Ѳ��Ҳ�����
	std::vector<std::string> grid_limit_property;	// �̵���ӿ�������������ֵ��
	std::vector<int> grid_limit_condition;			// �̵���ӿ���������Ӧֵ
	std::string shop_vip_logic_name;					// ȡvip�ȼ���Ӧ�����������߼���
	std::vector<int> discount_limit_list;			// �̵�����Ƿ�ɴ������������б�
};

// ���������
struct DropPack
{
	DropPack(): 
		drop_id(""), 
		lower_bound(0), 
		upper_bound(0){}

	std::string drop_id;		// �����ID
	int lower_bound;			// Ȩ������
	int upper_bound;			// Ȩ������

};

typedef std::vector<DropPack> DropPackList;

// �̵���Ʒ����
struct ShopRule
{
	ShopRule(): 
		player_level(0), 
		grid_index(0), 
		weight_sum(0)
	{
		drop_pack_list.clear();
	}

	int player_level;								// ��ҵȼ�
	int grid_index;								// �̵��������

	DropPackList drop_pack_list;			// �����
	int weight_sum;							// ��Ȩ��ֵ
};

// �ۿ۹���
struct DiscountRule
{
	DiscountRule(): 
		discount(0.0f), 
		lower_bound(0), 
		upper_bound(0){}

	float discount;		// �ۿ�
	int lower_bound;	// Ȩ������
	int upper_bound;	// Ȩ������

};

typedef std::vector<DiscountRule> DiscountList; 

// ��Ʒ��Ŀ��
struct ItemPlants
{
	ItemPlants(): 
		main_price(0.0f), 
		diamond_price(0.0f), 
		discount_weight_sum(0)
	{
		discount_list.clear();
	}

	float main_price;			// �����۸�
	float diamond_price;		  // Ԫ���۸�
	DiscountList discount_list;	// �ۿ��б�

	int discount_weight_sum;	// �ۿ���Ȩ��ֵ
	
};

// �̵�Ĺ����, key = player_level
typedef std::map<int, std::vector<ShopRule>> ShopRuleMap;

// ͨ���̵�� key->shop_type�̵����ͣ�value->�̵�����
struct GeneralShop
{
	GeneralShop(): shop_type(0)
	{
		shopRuleMap.clear();
	}

	int shop_type;
	ShopRuleMap shopRuleMap;
};

// ��Ʒ��Ŀ��, key = ��Ʒid_��������_��ƷƷ�ʣ�value->��Ŀ��
typedef std::map<std::string, ItemPlants> ItemPlantsMap;

// ����
typedef std::vector<ShopManagerData> ShopManagerDataVector;
typedef std::vector<GeneralShop> GeneralShopVector;

// �̵�ˢ����־
struct ShopRefreshInfo
{
	int lose_capital_log;
	int game_action_log;
	int vip_buy_type;

	ShopRefreshInfo() : 
		lose_capital_log(FUNCTION_EVENT_ID_SYS),
		//game_action_log(LOG_GAME_ACTION_MIN), 
		vip_buy_type(-1){};
};

// �̵깺����־
struct ShopBuyLog
{
	int lack_capital;	// �ʽ�����Ϣ
	int pay_failed;	// �ۿ�ʧ����Ϣ
	int logSrcFunction;		// ��־��Դ
	int game_action;	// �淨��־

	ShopBuyLog() : 
		lack_capital(0), 
		pay_failed(0), 
		logSrcFunction(FUNCTION_EVENT_ID_SYS)/*,
		game_action(LOG_GAME_ACTION_MIN)*/{};
};

// �̵�����
enum ShopTypes
{
	SHOP_TYPE_NONE = 0,

	SHOP_TYPE_GOLD = 1,		// Ԫ���̵�

	SHOP_TYPE_TOTAL,	
};

// �̵�����
enum ShopManagerRecCols
{
	SHOP_REC_TYPE = 0,									// �̵�����
	SHOP_REC_CUR_REFRESH_COUNT,				// ��ǰˢ�´���
	SHOP_REC_CUR_PAY_REFRESH_COUNT,		// ��ǰ����ˢ�´���
	SHOP_REC_MAX_FREE_REFRESH_COUNT,	// ���ˢ��������
	SHOP_REC_TRADE_CAPITALTYPE,					// �̵�ʹ�õĻ�������
	SHOP_REC_REFRESH_COSTVALUE,				// �´�ˢ�¼۸�
	SHOP_REC_REFRESH_ISFIRST,						// �Ƿ��һ���ֶ�ˢ��

};

// �̵���Ʒ���ֶΣ�ͨ�ã�
enum ShopRecCols
{
	COMMON_SHOP_REC_CONFIG = 0,		// ��Ʒconfig
	COMMON_SHOP_REC_COLORLVL = 1,	// ��ƷƷ��
	COMMON_SHOP_REC_NUM = 2,			// ��Ʒ����
	COMMON_SHOP_REC_PRICE = 3,			// �۸�
	COMMON_SHOP_REC_DIAMOND = 4,	// ��Ҫ��ʯ
	COMMON_SHOP_REC_DISCOUNT = 5,	// �ۿ�
};

//�ͻ���->����� ����Ϣ
enum CSShopSubMsg
{
	/*!
	* @brief	�̵�ˢ��
	* @param	int �̵����ͣ�ShopTypes��
	*/
	CS_SUB_SHOP_REFRESH	= 1,

	/*!
	* @brief	�̵�����
	* @param	int �̵����ͣ�ShopTypes��
	* @param	int ��������
	* @param	int ��������
	*/
	CS_SUB_SHOP_BUY,

	/*!
	* @brief	��ѯ�����̵�
	* @param	int �̵����ͣ�ShopTypes��
	*/
	CS_SUB_SHOP_RETRIEVE,
};

// �����->�ͻ��� ����Ϣ
enum SCShopSubMsg
{
	/*!
	* @brief	����ɹ�
	* @param	int �̵����ͣ�ShopTypes��
	* @param	int ��������
	* @param	string ������Ʒid
	* @param	int ��Ʒʣ������
	* @param	int ��������
	*/
	SC_SUB_SHOP_BUY_SUC	= 1,

	/*!
	* @brief	��ѯ�����̵�ɹ�
	* @param	int �̵����ͣ�ShopTypes��
	* @param	int �̵꽻�׻�������
	* @param	int ʣ�����ˢ�´���
	* @param	int �´�ˢ�»���
	* @param	int ��Ʒ����n
	* @param	����ѭ��n��
	* @param	int ��������
	* @param	string ��ƷID
	* @param	int ��ƷƷ��
	* @param	int ��Ʒ��Ŀ
	* @param	int �۸�
	* @param	int Ԫ���۸�
	* @param	float �ۿ�
	* @param	int �����Ƿ�����(0��ʾ�����ޣ�1��ʾ������)
	*/
	SC_SUB_SHOP_RETRIEVE,

	/*!
	* @brief	�̵��ˢ��ͳ��
	* @param	int �̵����ͣ�ShopTypes��
	* @param	int ʣ�����ˢ�´���
	* @param	int �´�ˢ�¼۸�
	*/
	SC_SUB_SHOP_REFRESH_STAT,
};

#endif  // _SHOP_MANAGER_DEFINE_H_