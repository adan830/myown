// -------------------------------------------
// �ļ����ƣ� ShopManagerModule.cpp
// �ļ�˵���� �̵����ģ��
// �������ڣ� 2016/03/29
// �� �� �ˣ�  tongzt
// �޸��ˣ�    
// -------------------------------------------

#include "ShopManagerModule.h"
#include "ShopUtils.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/Define/CommandDefine.h"

#include "utils/extend_func.h"
#include "CommonModule/ReLoadConfigModule.h"
#include "ShopGold.h"

/* 
** ��̬��Ա��ʼ�� */
ShopManagerModule *ShopManagerModule::m_pShopManagerModule = NULL;

// ��Դ����
inline int nx_reload_shop_resource(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);

	ShopUtilClass::Instance()->Init(pKernel);

	return 1;
}

// ��������̵����Ʒ�Ƿ�����
inline int nx_check_all_item(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	// ����������
	CHECK_ARG_NUM(state, nx_check_all_item, 1);

	// ���ÿ����������
	CHECK_ARG_OBJECT(state, nx_check_all_item, 1);
	PERSISTID self = pKernel->LuaToObject(state, 1);

	ShopUtilClass::Instance()->ValidateShopConf(pKernel, self);

	return 1;
}

// ��������̵����Ʒ�Ƿ�����
inline int nx_refresh_shop(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	// ����������
	CHECK_ARG_NUM(state, nx_refresh_shop, 1);

	// ���ÿ����������
	CHECK_ARG_OBJECT(state, nx_refresh_shop, 1);
	PERSISTID self = pKernel->LuaToObject(state, 1);

	LoopBeginCheck(b);
	for (int i = SHOP_TYPE_NONE + 1; i < SHOP_TYPE_TOTAL; ++i)
	{
		LoopDoCheck(b);
		ShopUtilClass::Instance()->RefreshShopItem(pKernel, self, i);
	}

	return 1;
}

// ģ���ʼ��
bool ShopManagerModule::Init( IKernel* pKernel )
{
	m_pShopManagerModule = this;
	Assert(m_pShopManagerModule != NULL);

	// ����
	pKernel->AddEventCallback("player", "OnRecover", ShopManagerModule::OnPlayerRecover);

	// �ͻ�����Ϣ�ص�
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_SHOP_MANAGER, ShopManagerModule::OnCustomShop);

	// �ͻ��˹�����������Ϣ�ص�
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_BUY_INFO, ShopManagerModule::OnCustomBuyTimes);

	// �������
	pKernel->AddIntCommandHook("player", COMMAND_LEVELUP_CHANGE, ShopManagerModule::OnCommandLevelUp);

	// ��Դ����
	if (!ShopUtilClass::Instance()->Init(pKernel))
	{
		Assert(false);
		return false;
	}

	// ģ��ע��
	if (!InitShopTemplate(pKernel))
	{
		Assert(false);
		return false;
	}

	// GM����
	DECL_LUA_EXT(nx_reload_shop_resource);
	DECL_LUA_EXT(nx_check_all_item);
	DECL_LUA_EXT(nx_refresh_shop);

	RELOAD_CONFIG_REG("ShopConfig", ShopManagerModule::ReloadConfig);

	return true;
}

// �ر�
bool ShopManagerModule::Shut( IKernel* pKernel )
{
	return ShopUtilClass::Instance()->Shut(pKernel);
}

// ����̵�ָ��
ShopBase* ShopManagerModule::GetShop(const int shop_type)
{
	if (shop_type > SHOP_TYPE_NONE 
		&& shop_type < SHOP_TYPE_TOTAL)
	{
		return m_pArrayShop[shop_type];
	}

	return NULL;
}

// ע���̵�ģ��
bool ShopManagerModule::RegistShopTemplate(IKernel* pKernel, ShopBase *pShopBase)
{
	if (NULL == pShopBase)
	{
		return false;
	}

	// �Ƿ�����
	if (pShopBase->GetShopType() <= SHOP_TYPE_NONE 
		|| pShopBase->GetShopType() >= SHOP_TYPE_TOTAL)
	{
		delete pShopBase;
		extend_warning(pKernel,"Register Shop Failed");
		return false;
	}

	// ע��
	if ( NULL == m_pArrayShop[pShopBase->GetShopType()])
	{
		if (pShopBase->Init(pKernel))
		{
			m_pArrayShop[pShopBase->GetShopType()] = pShopBase;
			return true;
		}
		
		return false;
	}

	return false;
}

// ��ʼ���̵�ģ��
bool ShopManagerModule::InitShopTemplate(IKernel* pKernel)
{
	for (int i = SHOP_TYPE_NONE + 1; i < SHOP_TYPE_TOTAL; ++i)
	{
		m_pArrayShop[i] = NULL;
	}

	// Ԫ���̵�
	if (!RegistShopTemplate(pKernel, NEW ShopGold))
	{
		return false;
	}

	//// �����̵�
	//if (!RegistShopTemplate(pKernel, NEW ShopSmelt))
	//{
	//	return false;
	//}

	//// ��ѫ�̵�
	//if (!RegistShopTemplate(pKernel, NEW ShopExploit))
	//{
	//	return false;
	//}

	//// �������̵�
	//if (!RegistShopTemplate(pKernel, NEW ShopArena))
	//{
	//	return false;
	//}

	

	//// ����ֵ�̵�
	//if (!RegistShopTemplate(pKernel, NEW ShopChivalry))
	//{
	//	return false;
	//}

	return true;
}

// �̵꿪��
int ShopManagerModule::ExecOpenShop( IKernel* pKernel, const PERSISTID& self )
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ������������̵��Ƿ�ɿ���
	LoopBeginCheck(a);
	for (int i = SHOP_TYPE_NONE + 1; i < SHOP_TYPE_TOTAL; ++i)
	{
		LoopDoCheck(a);
		ShopBase *pShop = m_pArrayShop[i];
		if (NULL != pShop)
		{
			pShop->OpenShop(pKernel, self);
		}
	}

	return 0;
}

/*!
* @brief	�����ѯ
* @param shop_type	�̵�����
* @return int
*/
int ShopManagerModule::ExecQueryShop(IKernel *pKernel, const PERSISTID &self, 
									 const int shop_type)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// �Ϸ�����֤
	if (shop_type <= SHOP_TYPE_NONE 
		|| shop_type >= SHOP_TYPE_TOTAL)
	{
		return 0;
	}

	// �̵�ָ��
	ShopBase *pShop = GetShop(shop_type);
	if (NULL == pShop)
	{
		return 0;
	}

	// �̵��ѯ
	pShop->QueryShop(pKernel, self);

	return 0;
}

/*!
* @brief	�����̵�ˢ��
* @param shop_type	�̵�����
* @return int
*/
int ShopManagerModule::ExecRefreshShop( IKernel *pKernel, const PERSISTID &self, 
									   const int shop_type )
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// �Ϸ�����֤
	if (shop_type <= SHOP_TYPE_NONE 
		|| shop_type >= SHOP_TYPE_TOTAL)
	{
		return 0;
	}

	// �̵�ָ��
	ShopBase *pShop = GetShop(shop_type);
	if (NULL == pShop)
	{
		return 0;
	}

	// ����ֶ�ˢ��
	pShop->HandRefresh(pKernel, self);

	return 0;
}

/*!
* @brief	�����̵긶��ˢ��
* @param vip_buy_type	��������
* @return int
*/
int ShopManagerModule::ExecPay2RefreshShop(IKernel *pKernel, const PERSISTID &self, 
										   const int vip_buy_type)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	LoopBeginCheck(b);
	for (int i = SHOP_TYPE_NONE + 1; i < SHOP_TYPE_TOTAL; ++i)
	{
		LoopDoCheck(b);
		// �̵�ָ��
		ShopBase *pShop = m_pArrayShop[i];
		if (NULL == pShop)
		{
			continue;
		}

		// ˢ����Ϣ
		const ShopRefreshInfo* refresh_info = pShop->GetShopRefreInfo();
		if (NULL == refresh_info)
		{
			continue;
		}

		// ���㸶������
		if (vip_buy_type == refresh_info->vip_buy_type)
		{
			// �����ֶ�ˢ��
			pShop->HandRefresh(pKernel, self, false);
		}
	}

	return 0;
}

/*!
* @brief	����ͬ���͵��̵�����
* @param shop_type	�̵�����
* @param shop_type	�������Ʒ����
* @return int
*/
int ShopManagerModule::ExecShopping( IKernel *pKernel, const PERSISTID &self, 
									const int shop_type, const int buy_index, int buy_amount)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// �Ϸ�����֤
	if (shop_type <= SHOP_TYPE_NONE 
		|| shop_type >= SHOP_TYPE_TOTAL)
	{
		return 0;
	}

	// �̵�ָ��
	ShopBase *pShop = GetShop(shop_type);
	if (NULL == pShop)
	{
		return 0;
	}

	// ������Ʒ
	pShop->Buy(pKernel, self, buy_index, buy_amount);

	return 0;
}

// ע���̵���ض�ʱ��
int ShopManagerModule::RegistShopResetTimer(IKernel *pKernel, const PERSISTID &self)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ע���̵궨ʱ��
	LoopBeginCheck(c);
	for (int i = SHOP_TYPE_NONE + 1; i < SHOP_TYPE_TOTAL; ++i)
	{
		LoopDoCheck(c);
		// �̵�ָ��
		ShopBase *pShop = m_pArrayShop[i];
		if (NULL == pShop)
		{
			continue;
		}

		// ע��
		pShop->RegistResetTimer(pKernel, self);
	}

	return 0;
}

// �ȼ��ı�ص�
int ShopManagerModule::OnCommandLevelUp(IKernel *pKernel, const PERSISTID &self, 
										const PERSISTID &sender, const IVarList &args)
{
	// ����Ƿ���δ���ŵ��̵꣬������
	return m_pShopManagerModule->ExecOpenShop(pKernel, self);
}

// �������
int ShopManagerModule::OnPlayerRecover(IKernel* pKernel,const PERSISTID& self,
									  const PERSISTID& sender,const IVarList& args)
{
	// ���߼���Ƿ�����Ҫ�������̵�
	m_pShopManagerModule->ExecOpenShop(pKernel, self);

	return m_pShopManagerModule->RegistShopResetTimer(pKernel, self);
}

// �ͻ�����Ϣ�ص�
int ShopManagerModule::OnCustomShop( IKernel* pKernel, const PERSISTID& self, 
									const PERSISTID& sender, const IVarList& args )
{
	// �����Ҳ�����
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// �����Ϸ��Լ��
	if (args.GetCount() < 3)
	{
		return 0;
	}

	int sub_msg = args.IntVal(1);	// ����ָ��
	int shop_type = args.IntVal(2);	// �̵�����

	// ��ѯ�̵���Ϣ
	if (CS_SUB_SHOP_RETRIEVE == sub_msg)	
	{
		return m_pShopManagerModule->ExecQueryShop(pKernel, self, shop_type);
	}
	// ����ֶ�ˢ���̵�
	else if (CS_SUB_SHOP_REFRESH == sub_msg)	
	{
		return m_pShopManagerModule->ExecRefreshShop(pKernel, self, shop_type);
	}
	// ������Ʒ
	else if (CS_SUB_SHOP_BUY == sub_msg)	
	{
		// ���ָ��������Ʒ������
		if (args.GetCount() < 5)
		{
			return 0;
		}

		// ����
		int buy_index = args.IntVal(3);
		int buy_amount = args.IntVal(4);
		return m_pShopManagerModule->ExecShopping(pKernel, self, shop_type, buy_index, buy_amount);
	}

	return 0;
}

// �ͻ��˹�����������Ϣ�ص�
int ShopManagerModule::OnCustomBuyTimes( IKernel* pKernel, const PERSISTID& self, 
										const PERSISTID& sender, const IVarList& args )
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	if (args.GetCount() < 2)
	{
		return 0;
	}

	// ��������
	int vip_buy_type = args.IntVal(1);
	
	// ����ˢ���̵�
	return m_pShopManagerModule->ExecPay2RefreshShop(pKernel, self, vip_buy_type);
}

void ShopManagerModule::ReloadConfig(IKernel *pKernel)
{
	ShopUtilClass::Instance()->Init(pKernel);
}