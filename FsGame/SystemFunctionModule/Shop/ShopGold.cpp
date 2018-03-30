// -------------------------------------------
// �ļ����ƣ� ShopGold.cpp
// �ļ�˵���� Ԫ���̵�
// �������ڣ� 
// �� �� �ˣ�  tongzt
// �޸��ˣ�
// -------------------------------------------

#include "ShopGold.h"
#include "ShopUtils.h"
#include "..\ResetTimerModule.h"
#include "Define\SystemInfo_SysFunction.h"

ShopGold::ShopGold()
{
	// �̵����͡�����
	m_shop_type = SHOP_TYPE_GOLD;
	m_shop_rec_name = FIELD_RECORD_SHOP_GOLD_REC;

	// ˢ����Ϣ
	//m_refresh_info.vip_buy_type = 0/*VIP_BUY_CHIVALRY_REFRESH*/;
	m_refresh_info.lose_capital_log = FUNCTION_EVENT_ID_SHOP_GOLD_REFRESH;

	// ������־
	m_buy_log.lack_capital = SYSTEM_INFO_ID_19005;
	//m_buy_log.pay_failed = STR_SHOP_SYSINFO_DEC_CHIVALRY_FAILED;
	m_buy_log.logSrcFunction = FUNCTION_EVENT_ID_SHOP_GOLD_BUY;
	/*m_buy_log.game_action = LOG_GAME_ACTION_SHOP_CHIVALRY_BUY;*/
}

// ��ʼ��
bool ShopGold::Init(IKernel* pKernel)
{
	// ��ʱ������
	DECL_RESET_TIMER(RESET_SHOP_GOLD, ShopGold::ItemResetTimer);
	DECL_RESET_TIMER(RESET_SHOP_GOLD_TIMES, ShopGold::ShopResetTimer);

	return true;
}

// ע�ᶨʱ��
void ShopGold::RegistResetTimer(IKernel *pKernel, const PERSISTID &self)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	// ��ʱ��ע��
	REGIST_RESET_TIMER(pKernel, self, RESET_SHOP_GOLD);
	REGIST_RESET_TIMER(pKernel, self, RESET_SHOP_GOLD_TIMES);
}

// ��Ʒˢ�¶�ʱ��
int ShopGold::ItemResetTimer(IKernel *pKernel, const PERSISTID &self, int slice)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// �Զ�ˢ����Ʒ
	ShopUtilClass::Instance()->RefreshShopItem(pKernel, self, SHOP_TYPE_GOLD);

	return 0;
}

// �̵����ö�ʱ��
int ShopGold::ShopResetTimer(IKernel *pKernel, const PERSISTID &self, int slice)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// �Զ������̵����
	ShopUtilClass::Instance()->RefreshShopTimes(pKernel, self, SHOP_TYPE_GOLD);

	return 0;
}