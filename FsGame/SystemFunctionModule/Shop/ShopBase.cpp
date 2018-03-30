// -------------------------------------------
// �ļ����ƣ� ShopBase.cpp
// �ļ�˵���� �����̵�
// �������ڣ� 2016/03/29
// �� �� �ˣ�  tongzt
// �޸��ˣ�
// -------------------------------------------

#include "ShopBase.h"
#include "ShopUtils.h"

ShopBase::ShopBase() : m_shop_type(SHOP_TYPE_NONE)
{
}

// ����̵����
const char* ShopBase::GetShopRecName()
{
	return m_shop_rec_name.c_str();
}

// ȡ���̵�����
const ShopTypes ShopBase::GetShopType()
{
	return m_shop_type;
}

// ���ˢ����Ϣ
const ShopRefreshInfo* ShopBase::GetShopRefreInfo()
{
	return &m_refresh_info;
}

// ��ù�����־��Ϣ
const ShopBuyLog* ShopBase::GetShopBuyLog()
{
	return &m_buy_log;
}

/*!
* @brief	�����̵�
* @return void
*/
void ShopBase::OpenShop(IKernel* pKernel, const PERSISTID& self)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	// �̵꿪��
	ShopUtilClass::Instance()->OpenShop(pKernel, self, m_shop_type);
}

/*!
* @brief	��ѯ�̵���ϸ��Ϣ����Ʒ��Ϣ�����ˢ�´���������ˢ�½��ȣ�
* @return void
*/
void ShopBase::QueryShop(IKernel* pKernel, const PERSISTID& self)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	// �̵��ѯ
	ShopUtilClass::Instance()->RetrieveShop(pKernel, self, m_shop_type);
}

/*!
* @brief	 ����ֶ�ˢ���̵�
* @param free_refresh	�Ƿ񸶷�
* @return void
*/
void ShopBase::HandRefresh(IKernel* pKernel, const PERSISTID& self, 
							   bool free_refresh /*= true*/)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	// �ֶ�ˢ���̵�
	ShopUtilClass::Instance()->OnPlayerRefreshShop(pKernel, self, m_shop_type, free_refresh);
}

// ����
void ShopBase::Buy(IKernel *pKernel, const PERSISTID &self, int buy_index, int buy_amount)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	// ������Ʒ
	ShopUtilClass::Instance()->DoBuyShopItem(pKernel, self, m_shop_type, buy_index, buy_amount);
}
