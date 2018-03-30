// -------------------------------------------
// �ļ����ƣ� ShopBase.h
// �ļ�˵���� �����̵�
// �������ڣ� 2016/03/29
// �� �� �ˣ�  tongzt
// �޸��ˣ�
// -------------------------------------------
#ifndef _SHOP_BASE_H_
#define _SHOP_BASE_H_

#include "Fsgame/Define/header.h"
#include "ShopManagerDefine.h"
#include "FsGame/Define/Fields.h"

class ShopBase
{
public:
	ShopBase();

public:
	// ����̵����
	const char* GetShopRecName();

	// ȡ���̵�����
	const ShopTypes GetShopType();

	// ���ˢ����Ϣ
	const ShopRefreshInfo* GetShopRefreInfo();

	// ��ù�����־��Ϣ
	const ShopBuyLog* GetShopBuyLog();

public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel) = 0;

	// ����
	virtual void OpenShop(IKernel* pKernel, const PERSISTID& self);

	// ��ѯ�̵�
	virtual void  QueryShop(IKernel* pKernel, const PERSISTID& self);

	// �ֶ�ˢ���̵�
	virtual void HandRefresh(IKernel* pKernel, const PERSISTID& self, bool free_refresh = true);

	// ����
	virtual void Buy(IKernel *pKernel, const PERSISTID &self, int buy_index, int buy_amount);

	// ע�ᶨʱ��
	virtual void RegistResetTimer(IKernel *pKernel, const PERSISTID &self) = 0;


	/************************************************************************/
	/*									         ��Ҫ��д��ʱ������			*/
	/************************************************************************/

public:
	// ��Ʒˢ�¶�ʱ��
	static int ItemResetTimer(IKernel *pKernel, const PERSISTID &self, int slice);

	// �̵����ö�ʱ��
	static int ShopResetTimer(IKernel *pKernel, const PERSISTID &self, int slice);
	
protected:
	ShopTypes m_shop_type;
	std::string m_shop_rec_name;
	ShopRefreshInfo m_refresh_info;
	ShopBuyLog m_buy_log;

};

#endif // _SHOP_BASE_H_