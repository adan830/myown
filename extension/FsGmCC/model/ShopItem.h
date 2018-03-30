//--------------------------------------------------------------------
// �ļ���:		ShopItem.h
// ��  ��:		�̳���Ʒ����
// ˵  ��:		
// ��������:		2016��11��27��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MODEL_SHOP_ITEM_H__
#define __MODEL_SHOP_ITEM_H__
#include <string>
#include <vector>

struct ShopItem
{
	std::string		id;					// ÿ�����Ψһ��ʶ
	std::wstring	name;				// ����
	std::wstring	desc;				// ����
	std::string		goods;				// ������Ʒ����ʽ: ��Ʒ1:����,��Ʒ2:����...��
	int				price;				// ���ۼ۸�
	int				color;				// Ʒ��
	int				count;				// ������
	int				level;				// �������Ƶ���ҵȼ���С�ڵ���0 ������
	int				vip;				// �������Ƶ�vip�ȼ���С�ڵ���0 ������
	int				purchaseNum;		// �������ƵĹ����������Ϊˢ�����ͣ���Ϊÿ��ɹ�����
	int				refreshType;		// �Ƿ�ÿ��ˢ�£�0Ϊ����ˢ��
	int				discount;			// �ۿ�(���� ��10Ϊ�ٷֱ� 1�۾���10%)
	int				catalog;			// ����
	time_t			onShowTime;			// �ϼ�ʱ��
	time_t			offShowTime;		// �¼�ʱ��
	time_t			onSellTime;			// ����ʱ��
	time_t			offSellTime;		// ֹͣʱ��

	time_t			newGoodTime;		// ��Ʒ��ʾ��ֹʱ��
};

// �̳Ƿ����ǩ
struct ShopTag
{
	int id;				// ��ǩid
	std::wstring name;	// ��ǩ��
};

struct ActivityItem
{
	int				value;				// Ŀ��ֵ
	std::wstring	name;				// ����
	std::string		goods;				// ������Ʒ����ʽ: ��Ʒ1:����,��Ʒ2:����...��
	int				count;				// ������
};

struct ActivityInfo
{
	std::wstring	name;				// ����
	std::wstring	desc;				// ����
	int actType;						// �����
	time_t			startTime;			// ��ʼʱ��
	time_t			endTime;			// ����ʱ��
	std::vector<ActivityItem> items;		// ���
};

struct ConfigLoadInfo
{
	std::string key;
	std::string path;
	int32_t		catalog;
	int32_t		cfgid;
};
#endif 