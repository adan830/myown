//----------------------------------------------------------
// 文件名:      GmccModule.h
// 内  容:      gmcc消息处理模块
// 说  明:
// 创建日期:    2014年10月17日
// 创建人:        
// 修改人:    
//    :        
//----------------------------------------------------------

#ifndef __GmccModule_H__
#define __GmccModule_H__

#include "Fsgame/Define/header.h"
#include <map>
#include <vector>


class GMModule;
class GmccModule : public ILogicModule_t
{
	struct PayedOrderData
	{
		std::string OrderId;
		std::string ProductId;
		std::string Items;
		std::string Rewards;
		std::string GiftRewords;
		std::string Extras;
		int64_t nOrderTime;
		int64_t nPayedTime;
		float Amount;
		int ProductRule;
		int MaxPurchase;
		bool IsRealPay;
	};
public:
	virtual bool Init(IKernel* pKernel);
	
	virtual bool Shut(IKernel* pKernel);
public:
	// command
	virtual void OnModuleCommand(IKernel* pKernel, int cmd_id, const IVarList& args);
private:
	static int OnPlayerReady(IKernel* pKernel, const PERSISTID& player,
		const PERSISTID& sender, const IVarList& msg);

	static int OnPlayerContinue(IKernel* pKernel, const PERSISTID& player,
		const PERSISTID& sender, const IVarList& msg);
	// 来自公服的消息
	static int OnPublicMessage(IKernel * pKernel, const PERSISTID & self,
							const PERSISTID & sender, const IVarList & args);
    // 接收到客户端消息
    static int OnCustom(IKernel* pKernel, const PERSISTID& player, 
							const PERSISTID& sender, const IVarList& msg);

    // gmcc服务器->游戏服务器: 发送的控制消息
    static int OnGmccMessage(IKernel* pKernel, const PERSISTID& scene,
							const PERSISTID& sender, const IVarList& args);

    //GMCC服务器->游戏服务器: 给指定名字的场景发送消息
	static int OnGmccCommand(IKernel* pKernel, const PERSISTID& player,
									const PERSISTID& sender, const IVarList& msg);
	//GMCC服务器->游戏服务器: 发给场景
	static int OnGmccSceneCommand(IKernel* pKernel, const PERSISTID& player,
									const PERSISTID& sender, const IVarList& msg);

	// 响应玩家禁言
	bool OnForbidSpeak(IKernel* pKernel, const PERSISTID& player, const IVarList &msg, int nOffset);
	// 响应玩家解除禁言
	bool OnAllowSpeak(IKernel* pKernel, const PERSISTID& player, const IVarList &msg, int nOffset);
	// gm特权指令
	bool OnGrantAuth(IKernel* pKernel, const PERSISTID& player, const IVarList &msg, int nOffset);

	// 检查G参数
    bool CheckCustom(IKernel* pKernel, const PERSISTID& player, 
						const PERSISTID& sender, const IVarList& msg);

	// 支付sdk插入结果(公共服务器返回)
	void OnPayedPushResult(IKernel* pKernel, const char* pszRoleId, const char* pszOrder, int nResult);
	// 收到支付完成通知(OnPayedPushResult成功后命令回调)
	void OnPayedCompleted(IKernel* pKernel, const PERSISTID& player, const char* pszOrder);
	// 收取支付成功奖励(收到通知或上线查询一次) pszOrder:不为NULL只检查指定的订单, 返回领取条目
	int OnPayedRecvItems(IKernel* pKernel, const PERSISTID& player, const char* pszOrder = NULL);
	// 查询充值返回返回
	void OnCommandRebateInfo( IKernel* pKernel, const PERSISTID& player, const IVarList& args, int nOffset );
	// 领取返利结果
	void OnCommandRebateResult(IKernel* pKernel, const PERSISTID& player, const IVarList& args, int nOffset);
	// 查询充值返利
	void QueryPayedRebate( IKernel* pKernel, const PERSISTID& player );
	// 支付等待心跳
	static int HB_PayedWait(IKernel* pKernel, const PERSISTID& self, int slice);
	// 发放支付奖励（实际添加物品的函数)
	bool SendPayedItem(IKernel* pKernel, const PERSISTID& player, IRecord* pPubPayedRec, int nRowId, IRecord* pPayedRec, IRecord* pPayedProducts);
	// 处理订单完成推送
	bool HandlePayment(IKernel* pKernel, const char* pszRoleId,
										const char* pszOrderId,
										const char* pszProductId,
										float fAmount,
										int nPayType,
										int64_t nCreatedTime,
										int64_t nPayedTime);
	// 处理订单成功
	bool HandleOrderSucceed( IKernel* pKernel, const PERSISTID& player, 
									IRecord* pPayedRec,					// 购买记录表
									IRecord* pPayedProducts,			// 购买产品记录表
									const PayedOrderData& data);
	bool IsInPurchaseDuration(int nRule, int64_t tTime);
	// 充值成功处理
	void OnRechargeSucceed(IKernel* pKernel, const PERSISTID& player, const char* pszOrderId, float fAmout, bool bIsReal);

	// gmcc查询玩家属性
	void OnPropQuery(IKernel* pKernel, IGameObj* pPlayer, const IVarList& keys, size_t nOffset, int64_t nQueryId );
	// gmcc查询表属性
	void OnRecordQuery(IKernel* pKernel, IGameObj* pPlayer, const char* pRecName, int nOffset, int nCount, int64_t nQueryId);
	// 同步服务器信息(命令转发)
	void OnSyncGmccSrvInfo(IKernel* pKernel, const IVarList& msg, int nOffset);

	// 上报服务器状态心跳
	static int HB_ReportMemberInfo(IKernel* pKernel, const PERSISTID& self, int slice);
	// 请求服务器状态
	static int HB_RequestSrvInfo(IKernel* pKernel, const PERSISTID& self, int slice);
	
private:
	void OnDoGmccCommand( IKernel* pKernel, const IVarList& args );
	void OnDoGmccMail(IKernel* pKernel, const IVarList& args);
	void OnDoGmccNotice(IKernel* pKernel, const IVarList& args);
	void OnDoGmccNoticeDel(IKernel* pKernel, const IVarList& args);
	void OnDoGmccExchResult(IKernel* pKernel, const IVarList& args);
	void OnDoGmccSilencePlayer(IKernel* pKernel, const IVarList& args);
	__declspec(deprecated("使用OnDoGamccPaymentPush代替"))
	void OnDoGmccPayed(IKernel* pKernel, const IVarList& args);
	void OnDoGmccProductsPush(IKernel* pKernel, const IVarList& args);
	void OnDoGamccPaymentPush(IKernel* pKernel, const IVarList& args);
	void OnDoGamccOrderResult(IKernel* pKernel, const IVarList& args);
	void OnDoGmccPropQuery(IKernel* pKernel, const IVarList& args);
	void OnDoGmccRecordQuery(IKernel* pKernel, const IVarList& args);
	void OnDoGmccInitSrvInfo(IKernel* pKernel, const IVarList& args);
	void OnDoGmccRebateInfo(IKernel* pKernel, const IVarList& args);
	void OnDoGmccRebateResult(IKernel* pKernel, const IVarList& args);

	// 推送礼包商城物品
	void OnDoGmccPushShopItems(IKernel* pKernel, const IVarList& args);
	// 删除商城物品
	void OnDoGmccDelShopItems(IKernel* pKernel, const IVarList& args);
	// 查询商品物品
	void OnDoGmccQueryShopItems(IKernel* pKernel, const IVarList& args);
	// 设置活动通用数据
	void OnDoGmccSetGenActivityConfig(IKernel* pKernel, const IVarList& args);
	// 加载配置
	void OnDoGmccLoadConfig(IKernel* pKernel, const IVarList& args);
private:
	// 客户端请求兑换
	void OnClientExchGift(IKernel* pKernel, const PERSISTID& player, const char* exchKey);
	// 领取充值返利
	void OnClientPickPayedRebate(IKernel* pKernel, const PERSISTID& player);
	// 同步GMCC
	void OnGameReady(IKernel* pKernel );
	// 回应gmcc消息
	void ConfirmGmccAck(IKernel* pKernel, int64_t nSeq);
	// 执行gm命令处理
	int HandleCommand(IKernel* pKernel, const PERSISTID& player, const char* cmd, const IVarList& args, int offset, std::string& outMsg);
	// 检查gm命令规则
	bool IsCommandRule(IKernel* pKernel, const char* cmd, const char* rule);
public:
	// 获取domainName
	static const std::wstring& GetDomainName(IKernel * pKernel);
	static IPubData* GetPubData(IKernel* pKernel);
	static IRecord* GetPubRecord(IKernel* pKernel, const char* pszRecName );
public:
	static void LoadPropsValue(IGameObj* pObject, IVarList& values, const IVarList& keys, size_t nOffset);
	static void LoadRecordData(IGameObj* pObject, IRecord* pRec, int nOffset, int nCount, IVarList& retData );

	// 广播member消息 返回广播数
	static int BroadcastMemberMessage(IKernel* pKernel, const IVarList& args);
	// 下单处理[只发消息， 不处理逻辑]
	static bool OnClientOrder(IKernel * pKernel, const char* pszRoleId, const wchar_t* pwszName, const char* pszProductId, const char* pszToken);
	// 通知状态改变[只发消息， 不处理逻辑]
	static bool OnOrderStateChanged(IKernel* pKernel, const char* pszRoleId, const char* pszOrder, int nState);
private:

	static GMModule*	m_pGmModule;
	static std::wstring m_domainName;
	static bool			ms_bGameReady;
	static bool			ms_bRecivedSrvInfo;
	static int64_t		ms_nLastSyncTimestamp;

public:
	static GmccModule * m_pGmccModule;
	friend int nx_recharge(void *state);
	friend int nx_pay_item(void *state);
};


#endif // __GmccModule_H__