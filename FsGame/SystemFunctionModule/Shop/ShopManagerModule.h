// -------------------------------------------
// �ļ����ƣ� ShopManagerModule.h
// �ļ�˵���� �̵����ģ��
// �������ڣ� 2016/03/29
// �� �� �ˣ�  tongzt
// �޸��ˣ�   
// -------------------------------------------
#ifndef _SHOP_MANAGER_MODULE_H_
#define _SHOP_MANAGER_MODULE_H_
#include "Fsgame/Define/header.h"
#include "ShopManagerDefine.h"
#include "ShopBase.h"

class ShopManagerModule : public ILogicModule
{

public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ͷ�
	virtual bool Shut(IKernel* pKernel);

	// ����̵�ָ��
	ShopBase* GetShop(const int shop_type);

private:
	// ע���̵�ģ��
	bool RegistShopTemplate(IKernel* pKernel, ShopBase *pShopBase);

	// ��ʼ���̵�ģ��
	bool InitShopTemplate(IKernel* pKernel);


	/************************************************************************/
	/*												��������				*/
	/************************************************************************/

public:
	// �̵꿪��
	int ExecOpenShop(IKernel* pKernel, const PERSISTID& self);

	// �����ѯ�̵�
	int ExecQueryShop(IKernel *pKernel, const PERSISTID &self, 
		const int shop_type);

	// �����̵�ˢ��
	int ExecRefreshShop(IKernel *pKernel, const PERSISTID &self, 
		const int shop_type);

	// �����̵긶��ˢ��
	int ExecPay2RefreshShop(IKernel *pKernel, const PERSISTID &self, 
		const int vip_buy_type);

	// ����ͬ���͵��̵�����
	int ExecShopping(IKernel *pKernel, const PERSISTID &self, 
		const int shop_type, const int buy_index, const int buy_amount);

	// ע���̵���ض�ʱ��
	int RegistShopResetTimer(IKernel *pKernel, const PERSISTID &self);

private:
	/************************************************************************/
	/*												�ص�����					*/
	/************************************************************************/

	// �ȼ����� 
	static int OnCommandLevelUp(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID &sender, const IVarList &args);

	// �������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// �ͻ�����Ϣ�ص�
	static int OnCustomShop(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// �ͻ��˹�����������Ϣ�ص�
	static int OnCustomBuyTimes(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	static void ReloadConfig(IKernel *pKernel);

private:
	ShopBase* m_pArrayShop[SHOP_TYPE_TOTAL];

public:
	static ShopManagerModule *m_pShopManagerModule;
};

#endif // _SHOP_MANAGER_MODULE_H_