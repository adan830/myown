//--------------------------------------------------------------------
// �ļ���:		PayedOrder.h
// ��  ��:		֧���ɹ��Ķ���
// ˵  ��:		
// ��������:		2016��9��26��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MODEL_PAYED_ORDER_INFO_H__
#define __MODEL_PAYED_ORDER_INFO_H__
#include <string>

//-----------------------------------------
// �����ݽṹ
//-----------------------------------------
// ��Ʒ��
struct GoodsItem
{
	std::string		ItemId;			// ��ƷID
	int				Number;			// ��Ʒ����
};

// ��Ʒ˵��
struct Product
{
	std::string ProductId;	// ��ƷID
	std::vector<GoodsItem> Items;		// ��Ʒ�б�
	std::vector<GoodsItem> Rewards;		// ������Ʒ�б�
	std::vector<GoodsItem> Gifts;		// �׳���Ʒ�б�
	std::vector<GoodsItem> Extra;		// �������������������б����Items;
	float Price;			// ��Ʒ�۸�
	int Rule;				// ��Ʒ���� EmProductRule
	int Times;				// �ɹ������ 0 ������( ���Ruleʹ��)
	int State;				// ��ǰ״̬
};

// �����������
struct Payment
{
	std::string RoleId;			// ���uid
	std::string OrderId;		// ����Id
	std::string ProductId;		// ��Ʒid
	float		Amount;			// ֧�����
	int			State;			// ����״̬ EmOrderState
	int			PayType;		// ֧������ EmBillType
	int64_t		OrderTime;		// �µ�ʱ��
	int64_t		PayedTime;		// ֧��ʱ��
};

// �µ�
struct ProductOrder
{
	std::string RoleId;			// ��ɫid
	std::string RoleName;		// ��ɫ��
	std::string Token;			// �ͻ���token
	std::string ProductId;		// ��Ʒid
};

// �µ�����
struct ProductOderInfo
{
	std::string RoleId;			// ��ɫid
	std::string ProductId;		// ��Ʒid
	std::string OrderId;		// ����id
	std::string Extra;			// ��չ�ַ���
	int			State;			// ����״̬ EmOrderState
	int			PayType;		// ֧������(StateΪ֧��ʱ��Ч) EmBillType
	float		Price;			// ʵ��֧�����
};

// ����״̬֪ͨ
struct OrderNotify
{
	std::string RoleId;				// ���uid
	std::string OrderId;			// ����ID

	int State;						// ״̬ EmOrderState
};
#endif 