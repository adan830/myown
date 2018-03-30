//--------------------------------------------------------------------
// �ļ���:      GmccModule.cpp
// ��  ��:      gmcc��Ϣ����ģ��
// ˵  ��:
// ��������:    2014��10��17��
// ������:        
//    :        
//--------------------------------------------------------------------

#include "FsGame/CommonModule/GmccModule.h"
#include "FsGame/Define/ModuleCmdDefine.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/TipsDefine.h"
#include "FsGame/Define/GmccDefine.h"
#include "public/Converts.h"
#include "utils/util_func.h"
#include "utils/extend_func.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"
#include "GMModule.h"
#include "Define/ExtraServerDefine.h"
#include "extension/FsGmCC/protocols/ProtocolsID.h"
#include "extension/FsGmCC/model/ServerInfo.h"
#include "InterActiveModule/SystemMailModule.h"
#include "InterActiveModule/ChatModule.h"
#include "system\winportable.h"
#include <time.h>
#include "GeneralModule/RewardModule.h"
#include "Define/PubDefine.h"
#include "server/RoomKnlConst.h"
#include "Define/SceneListDefine.h"
#include "LoginExtModule.h"
#include "TradeModule/ShopModule.h"
#include "GeneralModule/CapitalModule.h"
#include "LuaExtModule.h"
#include "Define/DynamicActDefine.h"
#include "InterActiveModule/RedPacketModule.h"
#include "EnvirValueModule.h"
#include "PlayerBaseModule/PlayerBaseModule.h"
#include "ReLoadConfigModule.h"
#include "LuaScriptModule.h"
#include "FsGame/Middle/MiddleModule.h"

std::wstring GmccModule::m_domainName = L"";
GmccModule * GmccModule::m_pGmccModule = NULL;
GMModule*	GmccModule::m_pGmModule = NULL;
bool		GmccModule::ms_bGameReady = false;
bool		GmccModule::ms_bRecivedSrvInfo = false;
int64_t		GmccModule::ms_nLastSyncTimestamp = 0;

// ��ֵ
inline int nx_recharge(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	// ����������
	CHECK_ARG_NUM(state, nx_recharge, 3);

	// ���ÿ����������
	CHECK_ARG_OBJECT(state, nx_recharge, 1);
	CHECK_ARG_FLOAT(state, nx_recharge, 2);
	CHECK_ARG_STRING(state, nx_recharge, 3);
	PERSISTID player = pKernel->LuaToObject(state, 1);
	float fAmout = pKernel->LuaToFloat(state, 2);
	const char* pszOrderId = pKernel->LuaToString(state, 3);

	GmccModule::m_pGmccModule->OnRechargeSucceed(pKernel, player, pszOrderId, fAmout, true);
	pKernel->LuaPushBool(state, true);

	return  1;
}

// ֧������
inline int nx_pay_item(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	// ����������
	CHECK_ARG_NUM(state, nx_pay_item, 6);


	// ���ÿ����������
	CHECK_ARG_OBJECT(state, nx_pay_item, 1);
	CHECK_ARG_FLOAT(state, nx_pay_item, 2);
	CHECK_ARG_STRING(state, nx_pay_item, 3);
	CHECK_ARG_STRING(state, nx_pay_item, 4);
	CHECK_ARG_STRING(state, nx_pay_item, 5);
	CHECK_ARG_STRING(state, nx_pay_item, 6);

	PERSISTID player = pKernel->LuaToObject(state, 1);
	float fAmout = pKernel->LuaToFloat(state, 2);
	const char* pszOrderId = pKernel->LuaToString(state, 3);			// ����id
	const char* pszProductId = pKernel->LuaToString(state, 4);			// ��Ʒ
	const char* pszItems = pKernel->LuaToString(state, 5);				// �����Ʒ
	const char* pszRewards = pKernel->LuaToString(state, 6);			// �׳佱��

	if (NULL == pszOrderId || NULL == pszProductId || NULL == pszItems || NULL == pszRewards)
	{
		pKernel->LuaErrorHandler(state, "nx_pay_item argument has null value");
		return 1; 
	}

	IRecord* pPayedRec = pKernel->GetRecord(player, FIELD_RECORD_PAYED_ORDER);
	if (NULL == pPayedRec)
	{
		pKernel->LuaErrorHandler(state, "nx_pay_item not found record 'FIELD_RECORD_PAYED_INFO'");
		return 1;
	}

	IRecord* pPayedProducts = pKernel->GetRecord(player, FIELD_RECORD_PAYED_PRODUCTS);
	time_t tNow = util_get_utc_time();
	GmccModule::PayedOrderData data;
	data.OrderId = pszOrderId;
	data.ProductId = pszProductId;
	data.Items = pszItems;
	data.Rewards = pszRewards;
	data.Extras = pszItems;
	data.Amount = fAmout;
	data.ProductRule = EM_PRODUCT_RULE_NORMAL;
	data.MaxPurchase = 0;
	data.IsRealPay = false;
	data.nOrderTime = tNow;
	data.nPayedTime = tNow;
	bool bRet = GmccModule::m_pGmccModule->HandleOrderSucceed(pKernel, player, pPayedRec, pPayedProducts, data);
	pKernel->LuaPushBool(state, bRet);

	return  1;
}


bool GmccModule::Init(IKernel * pKernel)
{
	m_pGmccModule = this;
	m_pGmModule = dynamic_cast<GMModule*>(pKernel->GetLogicModule("GMModule"));

	Assert(m_pGmModule && m_pGmccModule);

	pKernel->AddEventCallback("player", "OnReady", GmccModule::OnPlayerReady);
	pKernel->AddEventCallback("player", "OnContinue", GmccModule::OnPlayerContinue);

	//�ͻ�����Ϣ
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_GMCC_MSG, OnCustom);
	pKernel->AddEventCallback("scene", "OnExtraMessage", OnGmccMessage);
	pKernel->AddIntCommandHook("player", COMMAND_GMCC, OnGmccCommand);
	pKernel->AddIntCommandHook("scene", COMMAND_GMCC, OnGmccSceneCommand);

	// ��������Ϣ
	pKernel->AddEventCallback("scene", "OnPublicMessage", OnPublicMessage);

	DECL_HEARTBEAT(GmccModule::HB_PayedWait);
	DECL_HEARTBEAT(GmccModule::HB_ReportMemberInfo);
	DECL_HEARTBEAT(GmccModule::HB_RequestSrvInfo);

	// gm
	DECL_LUA_EXT(nx_recharge);
	DECL_LUA_EXT(nx_pay_item);

	return true;
}

bool GmccModule::Shut(IKernel * pKernel)
{
	return true;
}
void GmccModule::OnModuleCommand(IKernel* pKernel, int cmd_id, const IVarList& args)
{
	switch (cmd_id)
	{
	case E_MM_CMD_READY_PUB:
		{
			ms_bGameReady = true;
			OnGameReady(pKernel);
			// SystemMailModule::SendGlobalMail(pKernel, L"", L"�㲥�ʼ�����", L"���ǹ㲥�����˲����ն��� ��", NULL, 0);
		}
		break;
	default:
		break;
	}
}
int GmccModule::OnPlayerReady(IKernel* pKernel, const PERSISTID& player, const PERSISTID& sender, const IVarList& msg)
{
	bool bFirst = msg.IntVal(0) == 1;
	if (bFirst)
	{
		// �����ᴦ���ֵ����
		OnPlayerContinue(pKernel, player, sender, msg);

		// �״����߲�ѯ��ֵ֧��
		m_pGmccModule->OnPayedRecvItems(pKernel, player);

		// ��ȡȫ���ʼ�
		SystemMailModule::RecvGlobalMail(pKernel, player);
	}
	else
	{
		// ��ֵ��������
		m_pGmccModule->QueryPayedRebate(pKernel, player);
	}

	return 0;
}

int GmccModule::OnPlayerContinue(IKernel* pKernel, const PERSISTID& player, const PERSISTID& sender, const IVarList& msg)
{
	// ��ֵ��������
	m_pGmccModule->QueryPayedRebate(pKernel, player);

	const std::wstring wsName = GetDomainName(pKernel);

	IPubSpace* pPubSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (NULL == pPubSpace)
	{
		return 0;
	}

	IPubData * pPubData = pPubSpace->GetPubData(wsName.c_str());
	if (pPubData == NULL)
	{
		return 0;
	}

	IRecord* pNoticeRec = pPubData->GetRecord(PUB_GMCC_NOTICE_REC);
	if (NULL == pNoticeRec)
	{
		return 0;
	}

	int nRows = pNoticeRec->GetRows();

	CVarList senddata;
	senddata << SERVER_CUSTOMMSG_GMCC << GMCC_S2C_ADD_NOTICE << nRows;
	LoopBeginCheck(t);

	static int64_t s_LastCheckNoticeTime = 0;
	const int64_t NOTICE_CHECK_INTERVAL = 3;
	int64_t nNow = util_get_utc_time();
	if (nNow - s_LastCheckNoticeTime >= NOTICE_CHECK_INTERVAL)
	{
		int nCount = 0;
		for (int i = 0; i < nRows; ++i)
		{
			LoopDoCheck(t);
			int64_t nNoticeId = pNoticeRec->QueryInt64(i, PUB_COL_GNR_NOTICE_ID);
			// ɾ������֪ͨ
			int64_t nStart = pNoticeRec->QueryInt64(i, PUB_COL_GNR_NOTICE_CREATE_TIME);
			int64_t nEnd = pNoticeRec->QueryInt64(i, PUB_COL_GNR_NOTICE_END_TIME);
			if (nEnd <= nNow)
			{
				CVarList msg;
				msg << PUBSPACE_DOMAIN << wsName.c_str() << SP_DOMAIN_MSG_GMCC_DELETE_NOTICE << nNoticeId;
				pKernel->SendPublicMessage(msg);
			}
			else
			{
				const wchar_t* strContent = pNoticeRec->QueryWideStr(i, PUB_COL_GNR_NOTICE_CONTENT);
				int nLoop = pNoticeRec->QueryInt(i, PUB_COL_GNR_NOTICE_LOOP);
				int nInterval = pNoticeRec->QueryInt(i, PUB_COL_GNR_NOTICE_INTERVAL);
				int nType = pNoticeRec->QueryInt(i, PUB_COL_GNR_NOTICE_TYPE);

				// ���㹫�濪ʼʣ��ʱ��
				int32_t nLeft = 0;
				if (nStart > nNow)
				{
					nLeft = (int32_t)(nStart - nNow);
				}
				++nCount;
				senddata << nNoticeId << strContent << nLoop << nInterval << nType << nLeft << nEnd;
			}
		}
		// ��������
		senddata.SetInt(2, nCount);
		s_LastCheckNoticeTime = nNow;
	}
	else
	{
		for (int i = 0; i < nRows; ++i)
		{
			LoopDoCheck(t);
			int64_t nNoticeId = pNoticeRec->QueryInt64(i, PUB_COL_GNR_NOTICE_ID);
			const wchar_t* strContent = pNoticeRec->QueryWideStr(i, PUB_COL_GNR_NOTICE_CONTENT);
			int nLoop = pNoticeRec->QueryInt(i, PUB_COL_GNR_NOTICE_LOOP);
			int nInterval = pNoticeRec->QueryInt(i, PUB_COL_GNR_NOTICE_INTERVAL);
			int64_t nStart = pNoticeRec->QueryInt64(i, PUB_COL_GNR_NOTICE_CREATE_TIME);
			int64_t nEnd = pNoticeRec->QueryInt64(i, PUB_COL_GNR_NOTICE_END_TIME);
			int nType = pNoticeRec->QueryInt(i, PUB_COL_GNR_NOTICE_TYPE);

			// ���㹫�濪ʼʣ��ʱ��
			int32_t nLeft = 0;
			if (nStart > nNow)
			{
				nLeft = (int32_t)(nStart - nNow);
			}
			senddata << nNoticeId << strContent << nLoop << nInterval << nType << nLeft << nEnd;
		}
	}
	pKernel->Custom(player, senddata);

	return 0;
}

int GmccModule::OnPublicMessage(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args)
{
	if (args.GetCount()<2)
	{
		return 0;
	}
	// ��Ϣ��ʽ
	// string "domain", wstring L"DomainGmcc_serverid", int msgid, string RoleId, string Account, string Order, int nResult

	const char* spaceName = args.StringVal(0);
	const wchar_t* logicName = args.WideStrVal(1);

	// ����ָ������Ϣ������Ҫ����
	if (0 != ::strcmp(spaceName, PUBSPACE_DOMAIN)
		|| 0 != ::wcscmp(logicName, GetDomainName(pKernel).c_str()))
	{
		return 0;
	}

	int msgId = args.IntVal(2);
	if (msgId == PS_DOMAIN_MSG_GMCC_PAYED_PUSH_RESULT)
	{
		Assert(args.GetCount() == 6);
		if (args.GetCount() < 6)
		{
			::extend_warning(LOG_ERROR, "PS_DOMAIN_MSG_PAYED_PUSH_RESULT args count error.");
			return 0;
		}

		int nResult = args.IntVal(3);
		const char* pszRoleId = args.StringVal(4);
		const char* pszOrder = args.StringVal(5);
		m_pGmccModule->OnPayedPushResult(pKernel, pszRoleId, pszOrder, nResult);
	}
	else if (msgId == PS_DOMAIN_MSG_ADD_GLOBAL_MAIL_RESULT)
	{
		// ���ȫ���ʼ���� ��ʽ��string "domain", wstring L"DomainGmcc_serverid", int msgid, int64 nIdent, int nResult
		
		int nResult = args.IntVal(3);
		if (nResult == EM_GLOBAL_MAIL_ADD_SUCCESS)
		{
			// �㲥������ʾ���������ȡ�ʼ�
			CVarList var;
			var << COMMAND_GMCC << GMCC_CMD_RECV_GLOBAL_MAIL;
			pKernel->CommandByWorld( var );
		}
		else
		{
			int64_t nIdent = args.Int64Val(4);
			// ���������־
			::extend_warning(LOG_ERROR, "add global mail failed(ident:%lld result:%d)", nIdent, nResult);
		}
	}

	return 0;
}

// ���GMCC�ͷ��Ĳ���
bool GmccModule::CheckCustom(IKernel * pKernel, const PERSISTID & player, const PERSISTID & sender, const IVarList & msg)
{
	// �ж���ҵĺϷ���
	if (!pKernel || !pKernel->Exists(player))
	{
		::extend_warning(LOG_ERROR, "[GmccModule::CheckCustom] %s is NULL", "pKernel");
		return false;
	}

	int count = (int)msg.GetCount();
	if (count < 2 || msg.GetType(1) != VTYPE_INT)
	{
		return false;
	}

	// ���GMCC��cmd����
	const int cmd = msg.IntVal(1);
	if ( cmd == GMCC_C2S_ACCUSE )
	{ 
		if (count != 6 
			|| msg.GetType(2) != VTYPE_WIDESTR		// const std::wstring & name
			|| msg.GetType(3) != VTYPE_INT			// const int why
			|| msg.GetType(4) != VTYPE_WIDESTR		// const std::wstring & Reason
			|| msg.GetType(5) != VTYPE_WIDESTR )	// const std::wstring & BaseInfo
		{
			return false; 
		}
	}
	else if( cmd == GMCC_C2S_BUG || cmd == GMCC_C2S_EXP )
	{	
		if (count != 6 
			|| msg.GetType(2) != VTYPE_WIDESTR		// const wchar_t* index_type
			|| msg.GetType(3) != VTYPE_WIDESTR		// const wchar_t* ipt_1
			|| msg.GetType(4) != VTYPE_WIDESTR		// const wchar_t* ipt_2
			|| msg.GetType(5) != VTYPE_WIDESTR )	// const wchar_t* desc
		{
			return false; 
		}	
	}
	else if (cmd == GMCC_C2S_EXCH_GIFT)
	{
		if (count != 3
			|| msg.GetType(2) != VTYPE_STRING)		// const string* exch_key
		{
			return false;
		}
	}
	else if (cmd == GMCC_C2S_PICK_PAYED_REBATE)
	{
		return count >= 2;
	}
	else
	{
		if (count != 3 || msg.GetType(2) != VTYPE_WIDESTR)
		{
			return false; 
		}	
	}
	return true;
}

void GmccModule::OnPayedPushResult(IKernel* pKernel, const char* pszRoleId, const char* pszOrder, int nResult)
{
	Assert(NULL != pszRoleId);
	Assert(NULL != pszOrder);
	if (StringUtil::CharIsNull(pszRoleId) || StringUtil::CharIsNull(pszOrder))
	{
		::extend_warning(LOG_ERROR, "OnPayedPushResult args invalid.");
		return;
	}

	// ��Ӧgmcc������
	OnOrderStateChanged(pKernel, pszRoleId, pszOrder, EM_ORDER_STATE_ARRIVED);
	
	// ����ɹ� ֪ͨ�����ȡ
	if (nResult == PAYED_RESULT_SUCCED)
	{
		const wchar_t* pwszRoleName = pKernel->SeekRoleName(pszRoleId);
		if (StringUtil::CharIsNull(pwszRoleName))
		{
			::extend_warning(LOG_ERROR, "payed failed, role '%s' not exists(order:%s).", pszRoleId, pszOrder);
		}
		else
		{
			CVarList var;
			var << COMMAND_GMCC << GMCC_CMD_EXEC_PAYED << pszOrder;
			pKernel->CommandByName(pwszRoleName, var);
		}
	}
}

void GmccModule::OnPayedCompleted(IKernel* pKernel, const PERSISTID& player, const char* pszOrder)
{
	if ( OnPayedRecvItems(pKernel, player, pszOrder) <= 0)
	{
		// δ�ɹ�  ��һ���ѯ��  �������������ݻ�û��ͬ������
		if (pKernel->FindHeartBeat(player, "GmccModule::HB_PayedWait") == 0)
		{
			// 15�뻹δ�յ� �ղ��ٴ������ߺ��ٴ���
			pKernel->AddCountBeat(player, "GmccModule::HB_PayedWait", 3000, 5);
		}
	}
}

int GmccModule::HB_PayedWait(IKernel* pKernel, const PERSISTID& self, int slice)
{
	// ˢ�������б�
	if (m_pGmccModule->OnPayedRecvItems(pKernel, self) > 0)
	{
		pKernel->RemoveHeartBeat(self, "GmccModule::HB_PayedWait");
	}
	return 0;
}

bool GmccModule::SendPayedItem(IKernel* pKernel, const PERSISTID& player, IRecord* pPubPayedRec, int nRowId, IRecord* pPayedRec, IRecord* pPayedProducts)
{
	Assert(pPubPayedRec);
	Assert(pPayedRec);
	Assert(nRowId > -1);

	// �����ݲ��뵽��ұ���
	const char* pszRoleId = pKernel->QueryString(player, FIELD_PROP_UID);
	Assert(pszRoleId);

	PayedOrderData data;
	data.OrderId = pPubPayedRec->QueryString(nRowId, PUB_COL_GNR_PAYED_ORDER);
	Assert(!StringUtil::CharIsNull(data.OrderId.c_str()));
	data.ProductId = pPubPayedRec->QueryString(nRowId, PUB_COL_GNR_PAYED_PRODUCT_ID);
	data.Items = pPubPayedRec->QueryString(nRowId, PUB_COL_GNR_PAYED_ITEMS);
	data.Rewards = pPubPayedRec->QueryString(nRowId, PUB_COL_GNR_PAYED_REWARDS);
	data.GiftRewords = pPubPayedRec->QueryString(nRowId, PUB_COL_GNR_PAYED_GIFTS);
	data.Extras = pPubPayedRec->QueryString(nRowId, PUB_COL_GNR_PAYED_EXTRAS);
	data.Amount = pPubPayedRec->QueryFloat(nRowId, PUB_COL_GNR_PAYED_AMOUNT);
	data.IsRealPay = pPubPayedRec->QueryInt(nRowId, PUB_COL_GNR_PAYED_BILL_TYPE) == EM_BILL_TYPE_REAL;
	data.ProductRule = pPubPayedRec->QueryInt(nRowId, PUB_COL_GNR_PAYED_PRODUCT_RULE);
	data.MaxPurchase = pPubPayedRec->QueryInt(nRowId, PUB_COL_GNR_PAYED_PRODUCT_NUMBER);
	data.nOrderTime = pPubPayedRec->QueryInt64(nRowId, PUB_COL_GNR_PAYED_ORDER_TIME);
	data.nPayedTime = pPubPayedRec->QueryInt64(nRowId, PUB_COL_GNR_PAYED_PAYED_TIME);
	Assert(data.Amount > 0);


	int nRowIndex = pPayedRec->FindString( COLUMN_PAYED_ORDER_ORDER_ID, data.OrderId.c_str() );
	if (nRowIndex >= 0)
	{
		::extend_warning(LOG_WARNING, "order had received, don't get again[roleid:%s order:%s item:%s rewards:%s amount:%f]",
									pszRoleId, data.OrderId.c_str(), data.Items.c_str(), data.Rewards.c_str(), data.Amount);
		return true;
	}

	// ���һ��������־
	::extend_warning(LOG_INFO, "payed[roleid:%s order:%s product:%s item:%s rewards:%s amount:%f]", 
							pszRoleId, data.OrderId.c_str(), data.ProductId.c_str(), data.Items.c_str(), data.Rewards.c_str(), data.Amount);
	
	return HandleOrderSucceed( pKernel, player, pPayedRec, pPayedProducts, data );
}

bool GmccModule::HandlePayment(IKernel* pKernel, const char* pszRoleId, 
												 const char* pszOrderId, 
												 const char* pszProductId, 
												 float fAmount, int nPayType,
												 int64_t nCreatedTime, 
												 int64_t nPayedTime)
{
	Assert(!StringUtil::CharIsNull(pszRoleId));
	Assert(!StringUtil::CharIsNull(pszOrderId));
	Assert(!StringUtil::CharIsNull(pszProductId));
	Assert(fAmount > 0);

	extend_warning(LOG_INFO, "[GmccModule::HandlePayment]payed recv payed data from gmcc[roleid:%s order:%s product_id:%s fAmount:%f pay_type:%d]",
									pszRoleId, pszOrderId, pszProductId, fAmount, nPayType);
	Assert(!StringUtil::CharIsNull(pszRoleId));
	Assert(!StringUtil::CharIsNull(pszOrderId));
	Assert(!StringUtil::CharIsNull(pszProductId));
	Assert(fAmount > 0);

	// ��֤����
	if (StringUtil::CharIsNull(pszRoleId) || StringUtil::CharIsNull(pszOrderId) || StringUtil::CharIsNull(pszProductId))
	{
		extend_warning(LOG_INFO, "[GmccModule::HandlePayment]push payment data error");
		return false;
	}

	// �ȼ�¼������������
	CVarList msg;
	std::wstring wstr = GetDomainName(pKernel);
	msg << PUBSPACE_DOMAIN << wstr.c_str() << SP_DOMAIN_MSG_GMCC_PAYED_PUSH
		<< pszRoleId << pszOrderId << pszProductId << fAmount << nPayType << nCreatedTime << nPayedTime;
	return pKernel->SendPublicMessage(msg);
}

bool GmccModule::HandleOrderSucceed(	IKernel* pKernel, const PERSISTID& player, 
										IRecord* pPayedRec,
										IRecord* pPayedProducts,
										const PayedOrderData& data	)
{
	int nRows = pPayedRec->GetRows();
	int nMax = pPayedRec->GetRowMax();
	if (nRows > nMax)
	{
		// ɾ����ǰһ����¼
		pPayedRec->RemoveRow(0);
	}

	// ��Ӽ�¼
	int nFindIndex_ = -1;
	int nPurchased_ = 0;
	if (NULL != pPayedProducts)
	{
		nFindIndex_ = pPayedProducts->FindString(COLUMN_PAYED_PRODUCTS_PRODUCT_ID, data.ProductId.c_str());
		if (nFindIndex_ == -1)
		{
			CVarList values;
			values << data.ProductId.c_str() << 0 << 0 << (int64_t)0;
			nFindIndex_ = pPayedProducts->AddRowValue(-1, values);
		}
		else
		{
			nPurchased_ = pPayedProducts->QueryInt(nFindIndex_, COLUMN_PAYED_PRODUCTS_PURCHASE_TIMES);
		}
	}

	const char* pszItems_ = data.Items.c_str();
	bool bInDurationMode_ = false;
	bool bOverload_ = false;
	// ���ݹ����������յ���Ʒ
	if (data.ProductRule != EM_PRODUCT_RULE_NORMAL && data.MaxPurchase > 0)
	{
		bInDurationMode_ = IsInPurchaseDuration(data.ProductRule, data.nOrderTime);
		if (bInDurationMode_ && nPurchased_ >= data.MaxPurchase)
		{
			pszItems_ = data.Extras.c_str();
			bOverload_ = true;
		}
	}

	// �����ʼ�֪ͨ
	const wchar_t* pRoleName = pKernel->QueryWideStr(player, FIELD_PROP_NAME);
	std::string strProductNo = "{#";
	strProductNo.append(data.ProductId);
	strProductNo.append("}");
	std::string strDate;
	CVarList params;
	try
	{
		char szbuf[64] = { 0 };
		time_t time_ = (time_t)data.nPayedTime;
		util_convert_time_to_string(&time_, szbuf, 64);
		strDate = szbuf;
	}
	catch (...)
	{
		::extend_warning(LOG_ERROR, "convert payed time error(time:%lld)", data.nPayedTime);
	}
	params << strDate << strProductNo;
	SystemMailModule::SendMailToPlayer(pKernel, MAIL_NAME_PAYED, pRoleName, params, std::string(), FUNCTION_EVENT_ID_PAYED);

	// ��ʱ�䷶Χ����δ�����������
	if ( !bOverload_ )
	{
		// �¿�/��������
		if (stricmp(EnvirValueModule::EnvirQueryString(ENV_VALUE_MONTHOD_CARD_NO), data.ProductId.c_str()) == 0)
		{
			// ������Ʒ��
			ShopModule::AddWelfareCard(pKernel, player, EM_CARD_MONTH);
			pszItems_ = "";
		}
		else if (stricmp(EnvirValueModule::EnvirQueryString(ENV_VALUE_FORERVER_CARD_NO), data.ProductId.c_str()) == 0)
		{
			// �ڶ��ι����ö�����Ʒ����
			bool bOK = ShopModule::AddWelfareCard(pKernel, player, EM_CARD_FORERVER);
			if (bOK)
			{
				pszItems_ = "";
			}
		}
	}

	if ( !StringUtil::CharIsNull(pszItems_) )
	{
		// ���Ž���
		RewardModule::AwardEx award;
		award.name = MAIL_NAME_PAYED_ITEM;
		award.srcFunctionId = FUNCTION_EVENT_ID_PAYED;

		CVarList mail_param;
		RewardModule::ParseAwardData(pszItems_, award);
		bool bOK = RewardModule::m_pRewardInstance->RewardPlayer(pKernel, player, &award, mail_param);
		Assert(bOK);
	}

	// ���֧����¼
	CVarList row;
	row << data.OrderId.c_str() << data.ProductId.c_str() << pszItems_ << data.Rewards.c_str() << data.GiftRewords.c_str()
				<< data.Amount << data.nOrderTime << data.nPayedTime ;
	pPayedRec->AddRowValue(-1, row);

	// ������Ʒ
	if (!StringUtil::CharIsNull(data.Rewards.c_str()))
	{
		RewardModule::AwardEx award;
		award.name = MAIL_NAME_PAYED_REWORD;
		award.srcFunctionId = FUNCTION_EVENT_ID_PAYED_PRODUCTION_REWORD;

		CVarList mail_param;
		RewardModule::ParseAwardData(data.Rewards.c_str(), award);
		RewardModule::m_pRewardInstance->RewardPlayer(pKernel, player, &award, mail_param);
	}

	// ������Ʒ�׳�
	if (!StringUtil::CharIsNull(data.GiftRewords.c_str()) && nFindIndex_ >= 0)
	{
		int nTotalPurchased_ = pPayedProducts->QueryInt(nFindIndex_, COLUMN_PAYED_PRODUCTS_TATAL_TIMES);
		if (nTotalPurchased_ == 0)
		{
			RewardModule::AwardEx award;
			award.name = MAIL_NAME_PAYED_FIRST_PRODUCT;
			award.srcFunctionId = FUNCTION_EVENT_ID_PAYED_PRODUCTION_FIRST_REWORD;

			CVarList mail_param;
			RewardModule::ParseAwardData(data.GiftRewords.c_str(), award);
			RewardModule::m_pRewardInstance->RewardPlayer(pKernel, player, &award, mail_param);
		}
	}

	OnRechargeSucceed(pKernel, player, data.OrderId.c_str(), data.Amount, data.IsRealPay);

	// ���Ӽ�¼
	if (nFindIndex_ > -1)
	{
		int nTotalPurchased_ = pPayedProducts->QueryInt(nFindIndex_, COLUMN_PAYED_PRODUCTS_TATAL_TIMES);
		pPayedProducts->SetInt(nFindIndex_, COLUMN_PAYED_PRODUCTS_TATAL_TIMES, nTotalPurchased_ + 1);
		if (bInDurationMode_)
		{
			pPayedProducts->SetInt(nFindIndex_, COLUMN_PAYED_PRODUCTS_PURCHASE_TIMES, nPurchased_ + 1);
			pPayedProducts->SetInt64(nFindIndex_, COLUMN_PAYED_PRODUCTS_ORDER_TIME, data.nOrderTime);
		}
	}

	// ֪ͨ�ͻ�����ȡ�ɹ�
	ShopModule::OnReciveProduct( pKernel, player, data.OrderId.c_str(), data.ProductId.c_str() );
	return true;
}

bool GmccModule::IsInPurchaseDuration(int nRule, int64_t tTime)
{
	switch ( nRule )
	{
	case EM_PRODUCT_RULE_DAILY:
		return util_is_sameday(tTime, util_get_utc_time());
		break;
	case EM_PRODUCT_RULE_ONCE:
		return true;
	case EM_PRODUCT_RULE_WEEK:
		return util_is_sameweek(tTime, util_get_utc_time());
		break;
	case EM_PRODUCT_RULE_MONTH:
		return util_is_samemonth(tTime, util_get_utc_time());
		break;
	case EM_PRODUCT_RULE_YEAR:
		return util_is_sameyear(tTime, util_get_utc_time());
		break;
	default:
		break;
	}

	return false;
}

void GmccModule::OnRechargeSucceed(IKernel* pKernel, const PERSISTID& player, const char* pszOrderId, float fAmout, bool bIsReal)
{
	float fPayed = pKernel->QueryFloat(player, FIELD_PROP_TOTAL_PAY);
	bool bFirst = fPayed < 0.01; // �׳�
	fPayed += fAmout;
	pKernel->SetFloat(player, FIELD_PROP_TOTAL_PAY, fPayed);
	pKernel->IncInt(player, FIELD_PROP_PAY_TIMES, 1);
	// ����VIP����
	pKernel->IncInt(player, FIELD_PROP_VIP_EXP, (int)fAmout);

	// ������ʵ��ֵ���
	if (bIsReal)
	{
		pKernel->IncFloat(player, FIELD_PROP_TOTAL_PAY_REAL, fAmout);
	}

	LogModule::m_pLogModule->OnPayed(pKernel, player, pszOrderId, fAmout, 0, bFirst, bIsReal, NULL, NULL);

	// �׳䴦��
	if (bFirst)
	{
		ShopModule::OnFirstPayed(pKernel, player, fAmout);
	}
	// �ƺţ��׳壬�ۼƽ��ۼƴ���
	MiddleModule::Instance()->OnTitleCondition(pKernel, player, ETitleCond_PayedFirst, (int)fAmout);
	MiddleModule::Instance()->OnTitleCondition(pKernel, player, ETitleCond_PayedSum, (int)fPayed);
	MiddleModule::Instance()->OnTitleCondition(pKernel, player, ETitleCond_PayedCount, pKernel->QueryInt(player, FIELD_PROP_PAY_TIMES));

	// ͳ�Ƴ�ֵ��
	pKernel->Command(player, player, CVarList() << COMMAND_DYNAMIC_ACT << DYNAMIC_ACT_SUB_MSG_FILL_SILVER << (int)fPayed << (int)fAmout);
	// ��Ӧ��ֵ�����
	if (RedPacketModule::m_pRedPacketModule != NULL)
	{
		RedPacketModule::m_pRedPacketModule->OnSendChargeSystemPacket(pKernel, player, (int)fAmout);
	}
}

void GmccModule::OnPropQuery(IKernel* pKernel, IGameObj* pPlayer, const IVarList& keys, size_t nOffset, int64_t nQueryId)
{
	if (NULL == pPlayer) return;
	CVarList ret;
	ret << PROTOCOL_ID_ROLE_PROP_DATA << nQueryId;
	const char* pszRoleId = pPlayer->QueryString(FIELD_PROP_UID);
	if (StringUtil::CharIsNull(pszRoleId))
	{
		ret << DATA_QUERY_ROLEID_ERR << "";
	}
	else
	{
		size_t nCount = keys.GetCount();
		if (nCount <= nOffset)
		{
			ret << DATA_QUERY_KEY_ERROR << pszRoleId;
		}
		else
		{
			LoadPropsValue(pPlayer, ret, keys, nOffset);
		}
	}

	pKernel->SendExtraMessage(ESI_GMCC_SERVER, ret);
}

void GmccModule::OnRecordQuery(IKernel* pKernel, IGameObj* pPlayer, const char* pRecName, int nOffset, int nCount, int64_t nQueryId)
{
	if (NULL == pPlayer) return;
	if (NULL == pRecName) return;

	CVarList ret;
	ret << PROTOCOL_ID_ROLE_RECORD_DATA << nQueryId;
	const char* pszRoleId = pPlayer->QueryString(FIELD_PROP_UID);
	if (StringUtil::CharIsNull(pszRoleId))
	{
		ret << "" << pRecName << DATA_QUERY_ROLEID_ERR << nOffset << nCount << 0;
	}
	else
	{
		ret << pszRoleId << pRecName;
		IRecord* pRecord = pPlayer->GetRecord(pRecName);
		if ( NULL == pRecord )
		{
			ret << DATA_QUERY_KEY_ERROR << nOffset << nCount << 0;
		}
		else
		{
			ret << DATA_QUERY_SUCCESS << nOffset << nCount << pRecord->GetCols();
			LoadRecordData(pPlayer, pRecord, nOffset, nCount, ret );
		}
	}

	pKernel->SendExtraMessage(ESI_GMCC_SERVER, ret);
}

void GmccModule::OnSyncGmccSrvInfo(IKernel* pKernel, const IVarList& msg, int nOffset)
{
	assert((int)msg.GetCount() >= nOffset + 6);
	if ((int)msg.GetCount() < nOffset + 6)
	{
		::extend_warning(LOG_ERROR, "cmd sync gmcc srv info param error.");
		return;
	}

	SrvInfo _info;
	_info.Identity = msg.Int64Val(nOffset++);				// nOffset + 0
	_info.GameId = msg.Int64Val(nOffset++);					// nOffset + 1
	_info.ChannelId = msg.Int64Val(nOffset++);				// nOffset + 2
	_info.ProductionId = msg.Int64Val(nOffset++);			// nOffset + 3
	_info.ServerId = msg.Int64Val(nOffset++);				// nOffset + 4
	_info.AreaId = msg.Int64Val(nOffset++);					// nOffset + 5
	_info.DeployId = msg.Int64Val(nOffset++);				// nOffset + 6

	LogModule::m_pLogModule->SetSrvInfo(pKernel, _info);
}

int GmccModule::HB_ReportMemberInfo(IKernel* pKernel, const PERSISTID& self, int slice)
{
	// �ϱ�������״̬
	int nCurOnlineCount = pKernel->GetOnlineCount();
	int nMemberId = pKernel->GetMemberId();
	int nWorkState = 0;

	CVarList ret;
	ret << PROTOCOL_ID_REPORT_MEMBER_INFO << nCurOnlineCount << nMemberId << nWorkState;
	pKernel->SendExtraMessage(ESI_GMCC_SERVER, ret);
	return 0;
}

int GmccModule::HB_RequestSrvInfo(IKernel* pKernel, const PERSISTID& self, int slice)
{
	if (ms_bRecivedSrvInfo)
	{
		// �������������Ϣ����
		if (pKernel->FindHeartBeat(pKernel->GetScene(), "GmccModule::HB_RequestSrvInfo"))
		{
			// 1����
			pKernel->RemoveHeartBeat(pKernel->GetScene(), "GmccModule::HB_RequestSrvInfo");
		}
	}
	else
	{
		// �ȷ���׼��������Ϣ
		CVarList args;
		args << PROTOCOL_ID_GAME_READY;
		pKernel->SendExtraMessage(ESI_GMCC_SERVER, args);

		// �������������Ϣ
		CVarList ret;
		ret << PROTOCOL_ID_GAME_REQ_SRV_INFO;
		pKernel->SendExtraMessage(ESI_GMCC_SERVER, ret);
	}
	return 0;
}

int GmccModule::OnPayedRecvItems(IKernel* pKernel, const PERSISTID& player, const char* pszOrder /*= NULL*/)
{
	IPubSpace* pPubSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (NULL == pPubSpace)
	{
		return 0;
	}
	IPubData * pPubData = pPubSpace->GetPubData(GetDomainName(pKernel).c_str());
	if (pPubData == NULL)
	{
		return 0;
	}

	IRecord* pPubPayedRec = GetPubRecord(pKernel, PUB_GMCC_PAYED_REC);
	if (NULL == pPubPayedRec)
	{
		return 0;
	}

	IRecord* pPayedRec = pKernel->GetRecord(player, FIELD_RECORD_PAYED_ORDER);
	if (NULL == pPayedRec)
	{
		return 0;
	}

	const char* pszRoleId = pKernel->QueryString(player, FIELD_PROP_UID);
	if (StringUtil::CharIsNull(pszRoleId))
	{
		return 0;
	}	

	IRecord* pPayedProducts_ = pKernel->GetRecord(player, FIELD_RECORD_PAYED_PRODUCTS);
	int nCount = 0;
	const std::wstring& pubName = GetDomainName(pKernel);
	int nRows = pPubPayedRec->GetRows();
	if (StringUtil::CharIsNull(pszOrder))
	{
		// �������еĶ���
		LoopBeginCheck(a);
		for (int i = 0; i < nRows; ++i)
		{
			LoopDoCheck(a);
			const char* pszRecRoleId = pPubPayedRec->QueryString(i, PUB_COL_GNR_PAYED_ROLE_ID);
			if (strcmp(pszRecRoleId, pszRoleId) == 0)
			{
				if (SendPayedItem(pKernel, player, pPubPayedRec, i, pPayedRec, pPayedProducts_))
				{
					++nCount;

					const char* pszRecOrderId = pPubPayedRec->QueryString(i, PUB_COL_GNR_PAYED_ORDER);
					// ɾ������
					CVarList var;
					var << PUBSPACE_DOMAIN << pubName.c_str() << SP_DOMAIN_MSG_GMCC_PAYED_PICKED << pszRoleId << pszRecOrderId;
					pKernel->SendPublicMessage(var);

					// ֪ͨGmcc״̬�ı�
					OnOrderStateChanged(pKernel, pszRoleId, pszRecOrderId, EM_ORDER_STATE_TAKE);
				}
			}
		}
	}
	else
	{
		// ����ָ������
		LoopBeginCheck(a);
		for (int i = 0; i < nRows; ++i)
		{
			LoopDoCheck(a);
			const char* pszRecRoleId = pPubPayedRec->QueryString(i, PUB_COL_GNR_PAYED_ROLE_ID);
			const char* pszRecOrderId = pPubPayedRec->QueryString(i, PUB_COL_GNR_PAYED_ORDER);
			if (strcmp(pszRecRoleId, pszRoleId) == 0 && strcmp(pszOrder, pszRecOrderId) == 0 )
			{
				if (SendPayedItem(pKernel, player, pPubPayedRec, i, pPayedRec, pPayedProducts_))
				{
					++nCount;
					// ɾ������
					CVarList var;
					var << PUBSPACE_DOMAIN << pubName.c_str() << SP_DOMAIN_MSG_GMCC_PAYED_PICKED << pszRoleId << pszRecOrderId;
					pKernel->SendPublicMessage(var);

					// ֪ͨGmcc״̬�ı�
					OnOrderStateChanged(pKernel, pszRoleId, pszRecOrderId, EM_ORDER_STATE_TAKE);
				}
				break;
			}
		}
	}

	return nCount;
}

void GmccModule::OnCommandRebateInfo(IKernel* pKernel, const PERSISTID& player, const IVarList& args, int nOffset)
{
	int nType_ = args.IntVal(nOffset++);
	int nRebate_ = args.IntVal(nOffset++);
	if (nRebate_ > 0)
	{
		pKernel->SetInt(player, FIELD_PROP_PAYED_REBATE, nRebate_);
		pKernel->SetInt(player, FIELD_PROP_PAYED_REBATE_TYPE, nType_);

		// ����֪ͨ
		CVarList retMsg;
		retMsg << SERVER_CUSTOMMSG_GMCC << GMCC_S2C_PAYED_REBATE_NOTIFY << nRebate_ << nType_;
		pKernel->Custom(player, retMsg);
	}
	else
	{
		PlayerBaseModule::AddPlayerGlobalFlag(pKernel, player, EM_PLAYER_GLOBAL_PAYED_REBATE_PICKED);
	}
}

void GmccModule::OnCommandRebateResult(IKernel* pKernel, const PERSISTID& player, const IVarList& args, int nOffset)
{
	int nResult = args.IntVal(nOffset++);
	int nRebate = args.IntVal(nOffset++);

	if (nResult == EM_REBATE_RESULT_SUCCEED)
	{
		Assert(nRebate > 0);
		bool bOk = CapitalModule::m_pCapitalModule->IncCapital(pKernel, player, CAPITAL_SILVER, nRebate, FUNCTION_EVENT_ID_TEST_PAYED_REBATE);
		if (bOk)
		{
			// �޸ı�־ �����سɹ���Ϣ
			pKernel->SetInt(player, FIELD_PROP_PAYED_REBATE, nRebate);
			PlayerBaseModule::AddPlayerGlobalFlag(pKernel, player, EM_PLAYER_GLOBAL_PAYED_REBATE_PICKED);
		}
		else
		{
			// ���һ�����󣬽���޷����أ��Ʒ�״̬�Ѹı���
			::extend_warning(LOG_ERROR, "[GmccModule::OnCommandRebateResult] inc silver failed.");
		}
	}
	else if (nResult == EM_REBATE_RESULT_PICKED)
	{
		pKernel->SetInt(player, FIELD_PROP_PAYED_REBATE, 0);
		PlayerBaseModule::AddPlayerGlobalFlag(pKernel, player, EM_PLAYER_GLOBAL_PAYED_REBATE_PICKED);
	}

	CVarList retMsg;
	retMsg << SERVER_CUSTOMMSG_GMCC << GMCC_S2C_PICK_PAYED_REBATE << nResult;
	pKernel->Custom(player, retMsg);
}

void GmccModule::QueryPayedRebate( IKernel* pKernel, const PERSISTID& player )
{
	// �Ƿ���ȡ����
	bool bPicked_ = PlayerBaseModule::TestPlayerGlobalFlag( pKernel, player, EM_PLAYER_GLOBAL_PAYED_REBATE_PICKED);
	if (bPicked_)
	{
		// ��ȡ����ʲôҲ������
		return;
	}

	IGameObj* pPlayer_ = pKernel->GetGameObj(player);
	if (NULL == pPlayer_)
	{
		return;
	}

	// �Ƿ��ѯ���� ��ѯ����, ����ֵ�϶���Ϊ0
	int nRebate_ = pPlayer_->QueryInt( FIELD_PROP_PAYED_REBATE );
	if (nRebate_ > 0)
	{
		int nType_ = pPlayer_->QueryInt(FIELD_PROP_PAYED_REBATE_TYPE);
		CVarList retMsg;
		retMsg << SERVER_CUSTOMMSG_GMCC << GMCC_S2C_PAYED_REBATE_NOTIFY << nRebate_ << nType_;
		pKernel->Custom(player, retMsg);
	}
	else
	{
		const char* pszRoleId_ = pPlayer_->QueryString(FIELD_PROP_UID);
		const char* pszUid_ = pPlayer_->QueryString(FIELD_PROP_ACCOUNT);

		// ����Ϣ��web��ѯ����
		CVarList args;
		args << PROTOCOL_ID_REBATE_QUERY
			<< pszUid_
			<< pszRoleId_;

		pKernel->SendExtraMessage(ESI_GMCC_SERVER, args);
	}
}

// ���յ��ͻ�����Ϣ ���������ŵĶ���ͻ�����Ϣ������
int GmccModule::OnCustom(IKernel* pKernel, const PERSISTID& player, const PERSISTID& sender, const IVarList& msg)
{
	// �ж���ҵĺϷ���
	if (!pKernel || !pKernel->Exists(player))
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnCustom] %s is NULL", "pKernel");
		return 0;
	}
	// �������ж�
	if (!m_pGmccModule->CheckCustom(pKernel, player, sender, msg))
	{
		return 0;
	}

	// ���GMCC��cmd����
	const int cmd = msg.IntVal(1);
	if ( cmd == GMCC_C2S_ACCUSE )
	{ 
		// �ͻ��˾ٱ����
		const wchar_t* accusedname = msg.WideStrVal(2);					// ���ٱ�������
		if (StringUtil::CharIsNull(accusedname))
		{
			::extend_warning(LOG_ERROR, "[GmccModule::OnCustom] %s is NULL or Empty !", "name");
			return 0;
		}
		int why = msg.IntVal(3);
		const wchar_t* reas = msg.WideStrVal(4);
		const wchar_t* baseInfo = msg.WideStrVal(5);
		if ( !reas || !baseInfo)
		{
			::extend_warning(LOG_ERROR, "[GmccModule::OnCustom] %s is NULL!", "reason|baseInfo");
			return 0;
		}

		//GmccModule::SendAccuseInfoToPlayer(pKernel, player, accusedname, why, reas, baseInfo);
	}
	else if( cmd == GMCC_C2S_BUG || cmd == GMCC_C2S_EXP )
	{	
		// �ͻ����ύBUG����Ϸ�ύ
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)
		{
			::extend_warning(LOG_ERROR, "[GmccModule::OnCustom] %s is NULL or Empty!", "Account|Name");
			return 0;
		}
		const char* account= pPlayer->QueryString("Account");	//����˺�
		const wchar_t* name = pPlayer->QueryWideStr("Name");	//�������
		if (StringUtil::CharIsNull(account) || StringUtil::CharIsNull(name))
		{
			::extend_warning(LOG_ERROR, "[GmccModule::OnCustom] %s is NULL or Empty!", "Account|Name");
			return 0;
		}

		int level = pPlayer->QueryInt("Level");					//�ȼ�
		int jobid = pPlayer->QueryInt("Job");					//ְҵ

		const wchar_t* index_type = msg.WideStrVal(2);   
		const wchar_t* ipt_1 = msg.WideStrVal(3); 	
		const wchar_t* ipt_2 = msg.WideStrVal(4);
		const wchar_t* desc  = msg.WideStrVal(5);
		if (!index_type || !ipt_1 || !ipt_2 || !desc)
		{
			::extend_warning(LOG_ERROR, "[GmccModule::OnCustom] %s is NULL !", " index_type|ipt_1|ipt_2|desc");
			return 0;
		}

		char strtime[256] = {0};
		time_t ttime = time(NULL);
		util_convert_time_to_string(&ttime, strtime, sizeof(strtime));

		int nation = pPlayer->QueryInt("Nation");
		std::wstring roleCamp = util_int_as_widestr(nation);	// ��Ӫ -> ����
		std::wstring wstrJobID = util_int64_as_widestr(jobid);	// ְҵ

		int reort_type = cmd - 2;								// ���� 0 bug 1 advise
		const char* default_string = "";						// �����ֶ�

		// ��Ϸ��������GMCC������ͨ��bug�ͽ����ռ���Ϣ
		CVarList var;
		var << 0x4033 << account << name << level << roleCamp 
			<< wstrJobID << reort_type << index_type << ipt_1 
			<< ipt_2 << desc << strtime << default_string; 

		//pKernel->SendGmccCustom(var);
	}
	else if (cmd == GMCC_C2S_EXCH_GIFT)
	{
		const char* pszKey = msg.StringVal(2);
		if (!pszKey)
		{
			::extend_warning(LOG_ERROR, "[GmccModule::OnCustom] %s is NULL !", "key");
			return 0;
		}
		m_pGmccModule->OnClientExchGift(pKernel, player, pszKey);
	}
	else if (cmd == GMCC_C2S_PICK_PAYED_REBATE)
	{
		m_pGmccModule->OnClientPickPayedRebate(pKernel, player);
	}
	else
	{
		const wchar_t* info = msg.WideStrVal(2);
		if (!info)
		{
			::extend_warning(LOG_ERROR, "[GmccModule::OnCustom] %s is NULL !", "info");
			return 0;
		}
		if (pKernel->Type(player) == TYPE_PLAYER)
		{
			//pKernel->SendToGmcc(player, info);
		}
	}

	return 0;
}

// gmcc������->��Ϸ������: ���͵Ŀ�����Ϣ
int GmccModule::OnGmccMessage(IKernel* pKernel, const PERSISTID& scene, const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel)
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnGmccMessage] %s is NULL", "pKernel");
		return 0;
	}

	// ������id
	int index = args.IntVal(0);
	if ( index != ESI_GMCC_SERVER )
	{
		return 0;
	}

	//�鿴2����Ϣͷ
	int msgid = args.IntVal(1);
	::extend_warning(LOG_INFO, "[GmccModule::OnGmccMessage] recv gmcc msg id:%d", msgid);
	switch (msgid)
	{
	case PROTOCOL_ID_GM_EXEC:
		m_pGmccModule->OnDoGmccCommand(pKernel, args);
		break;
	case PROTOCOL_ID_MAIL_POST:
		m_pGmccModule->OnDoGmccMail(pKernel, args);
		break;
	case PROTOCOL_ID_NOTICE_POST:
		m_pGmccModule->OnDoGmccNotice(pKernel, args);
		break;
	case PROTOCOL_ID_NOTICE_DELETE:
		m_pGmccModule->OnDoGmccNoticeDel(pKernel, args);
		break;
	case PROTOCOL_ID_GIFT_RESULT:
		m_pGmccModule->OnDoGmccExchResult(pKernel, args);
		break;
	case PROTOCOL_ID_PRODUCTS_PSH:
		m_pGmccModule->OnDoGmccProductsPush(pKernel, args);
		break;
	case PROTOCOL_ID_PAYMENT_PSH:
		m_pGmccModule->OnDoGamccPaymentPush(pKernel, args);
		break;
	case PROTOCOL_ID_ORDER_INFO:
		m_pGmccModule->OnDoGamccOrderResult(pKernel, args);
		break;
	case PROTOCOL_ID_ROLE_PROP_QUERY:
		m_pGmccModule->OnDoGmccPropQuery(pKernel, args);
		break;
	case PROTOCOL_ID_ROLE_RECORD_QUERY:
		m_pGmccModule->OnDoGmccRecordQuery(pKernel, args);
		break;
	case RPOTOCOL_ID_GAME_INIT_INFO:
		m_pGmccModule->OnDoGmccInitSrvInfo(pKernel, args);
		break;
	case PROTOCOL_ID_GAME_CHECK_READY:
		if (ms_bGameReady)
		{
			m_pGmccModule->OnGameReady(pKernel);
		}
		break;
	case PROTOCOL_ID_PUSH_SHOP_ITEM:
		m_pGmccModule->OnDoGmccPushShopItems(pKernel, args);
		break;
	case PROTOCOL_ID_DEL_SHOP_ITEM:
		m_pGmccModule->OnDoGmccDelShopItems(pKernel, args);
		break;
	case PROTOCOL_ID_QUERY_SHOP_ITEM:
		m_pGmccModule->OnDoGmccQueryShopItems(pKernel, args);
		break;
	case PROTOCOL_ID_SET_ACTIVITY_CONFIG:
		m_pGmccModule->OnDoGmccSetGenActivityConfig(pKernel, args);
		break;
	case PROTOCOL_ID_LOAD_CONFIG:
		m_pGmccModule->OnDoGmccLoadConfig(pKernel, args);
		break;
	case PROTOCOL_ID_REBATE_INFO:
		m_pGmccModule->OnDoGmccRebateInfo(pKernel, args);
		break;
	case PROTOCOL_ID_REBATE_RESULT:
		m_pGmccModule->OnDoGmccRebateResult(pKernel, args);
		break;
	default:
		break;
	}

	return 0;
}

// GMCC������->��Ϸ������: ��ָ�����ֵĳ���������Ϣ
int GmccModule::OnGmccCommand(IKernel* pKernel, const PERSISTID& player, const PERSISTID& sender, const IVarList& msg)
{
	// �ж���ҵĺϷ���
	IGameObj* pPlayer = pKernel->GetGameObj(player);
	if (NULL == pPlayer)
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnGmccCommand] %s is NULL", "pKernel");
		return 0;
	}

	int cmd = msg.IntVal(1);
	switch (cmd)
	{
	case GMCC_CMD_EXEC_GM_COMMAND:
		{
			// ����ִ��Ŀ��
			if (!pKernel->FindData(player, "GMPropTarget"))
			{
				pKernel->AddData(player, "GMPropTarget", VTYPE_OBJECT);
			}
			pKernel->SetDataObject(player, "GMPropTarget", player);

			int64_t nCmdId_ = msg.Int64Val(2);
			const char* pszCommand = msg.StringVal(3);
			const char* pszRoleId_ = pKernel->QueryString(player, FIELD_PROP_UID);
			std::string strMsg_;
			int nResult = m_pGmccModule->HandleCommand( pKernel, player, pszCommand, msg, 4, strMsg_ );
			CVarList retMsg;
			retMsg << PROTOCOL_ID_GM_REPLY << nCmdId_ << pszRoleId_ << nResult << strMsg_.c_str();
			pKernel->SendExtraMessage(ESI_GMCC_SERVER, retMsg);
		}
		break;
	case GMCC_CMD_EXEC_PAYED:
		{
			const char* pszOrder = msg.StringVal(2);
			m_pGmccModule->OnPayedCompleted(pKernel, player, pszOrder);
		}
		break;
	case GMCC_CMD_EXEC_PROP_QUERY:
	{
		IGameObj* pGameObj = pKernel->GetGameObj(player);
		int64_t nQueryId = msg.Int64Val(2);
		m_pGmccModule->OnPropQuery(pKernel, pGameObj, msg, 3, nQueryId);
	}
	break;
	case GMCC_CMD_EXEC_RECORD_QUERY:
		{
			IGameObj* pGameObj = pKernel->GetGameObj(player);
			int64_t nQueryId = msg.Int64Val(2);
			const char* pszRecName = msg.StringVal(3);
			int nOffset = msg.IntVal(4);
			int nCount = msg.IntVal(5);
			m_pGmccModule->OnRecordQuery(pKernel, pGameObj, pszRecName, nOffset, nCount, nQueryId);
		}
		break;
		//case GMCC_CMD_SYNC_SEV_INFO:
		//{
		//	m_pGmccModule->OnSyncGmccSrvInfo(pKernel, msg, 2);
		//}
		//break;
	case GMCC_CMD_RECV_GLOBAL_MAIL:
		SystemMailModule::RecvGlobalMail( pKernel, player );
		break;
	case GMCC_CMD_REBATE_INFO:
		m_pGmccModule->OnCommandRebateInfo(pKernel, player, msg, 2);
		break;
	case GMCC_CMD_REBATE_RESULT:
		m_pGmccModule->OnCommandRebateResult(pKernel, player, msg, 2);
		break;
	default:
		break;
	}
	return 0;
}

int GmccModule::OnGmccSceneCommand(IKernel* pKernel, const PERSISTID& player, const PERSISTID& sender, const IVarList& msg)
{
	int cmd = msg.IntVal(1);
	if (cmd == GMCC_CMD_SYNC_SEV_INFO)
	{
		int64_t tNow = util_get_utc_time();
		if (!ms_bRecivedSrvInfo || tNow > ms_nLastSyncTimestamp + 5)
		{
			m_pGmccModule->OnSyncGmccSrvInfo(pKernel, msg, 2);

			// ֪ͨ�Ʒѵ�½������
			{
				LoginExtModule::OnLoginReady(pKernel);
			}

			// �����ϱ�״̬����(1����)
			if (!pKernel->FindHeartBeat(player, "GmccModule::HB_ReportMemberInfo"))
			{
				pKernel->AddHeartBeat(player, "GmccModule::HB_ReportMemberInfo", 60000);
			}
			ms_bRecivedSrvInfo = true;
			ms_nLastSyncTimestamp = tNow;
		}
	}
	return 0;
}

bool GmccModule::OnForbidSpeak(IKernel* pKernel, const PERSISTID& player, const IVarList &msg, int nOffset)
{
	IGameObj* pPlayer = pKernel->GetGameObj(player);
	if (NULL == pPlayer)
	{
		return false;
	}

	int nSilenceTime = StringUtil::StringAsInt( msg.StringVal(nOffset) );
	const wchar_t* name = pPlayer->QueryWideStr(FIELD_PROP_NAME);
	const char* account = pPlayer->QueryString(FIELD_PROP_ACCOUNT);

	//���ԵĽ��
	bool nRes = ChatModule::m_pChatModule->SetSilence(pKernel, player, nSilenceTime);

	//���Գɹ�  ��ʾ���
	if (nRes)
	{
		CVarList msgList;
		msgList << SERVER_CUSTOMMSG_SYSINFO << TIPSTYPE_NORMAL_CHAT_MESSAGE << "sys_item_0083" << account << nSilenceTime;
		pKernel->CustomByName(name, msgList);

		// ��ӳͷ���־��¼:������GMCC��ָ���������˽���
		PunishLog log;
		log.gmAccount = account;
		log.type = LOG_PUNISH_GMCC_SLIENCE;
		log.minutes = nSilenceTime;
		LogModule::m_pLogModule->SavePunishLog(pKernel, player, log);

		int64_t nInterval = nSilenceTime * 60;
		int64_t nExpired = util_get_utc_time() + nInterval;
		LogModule::m_pLogModule->OnForbidSpeak(pKernel, player, nInterval, nExpired);
	}

	return nRes;
}

// ��Ӧ��ҽ������
bool GmccModule::OnAllowSpeak(IKernel* pKernel, const PERSISTID& player, const IVarList &msg, int nOffset)
{
	IGameObj* pPlayer = pKernel->GetGameObj(player);
	if (NULL == pPlayer)
	{
		return false;
	}

	const wchar_t * name = pPlayer->QueryWideStr(FIELD_PROP_NAME);
	const char * account = pPlayer->QueryString(FIELD_PROP_ACCOUNT);

	// ������,ɾ����������
	if (pKernel->FindHeartBeat(player, "ChatModule::H_Silence"))
	{
		pKernel->RemoveHeartBeat(player, "ChatModule::H_Silence");
	}

	pPlayer->SetInt("SilenceTick", 0);
	LogModule::m_pLogModule->OnAllowSpeak(pKernel, player);

	return true;
}

bool GmccModule::OnGrantAuth(IKernel* pKernel, const PERSISTID& player, const IVarList &msg, int nOffset)
{
	int nLevel = StringUtil::StringAsInt(msg.StringVal(nOffset));
	pKernel->SetInt(player, FIELD_PROP_GAMEMASTER, nLevel);

	::extend_warning(LOG_INFO, "[GmccModule::OnGrantAuth]grant %s game master level %d",
							pKernel->QueryString(player, FIELD_PROP_UID), nLevel);

	return true;
}

void GmccModule::OnDoGmccCommand(IKernel* pKernel, const IVarList& args)
{
// 	assert(args.GetCount() >= 5);
// 	if (args.GetCount() < 5)
// 	{
// 		// param error
// 		return;
// 	}

	size_t nIndex = 2;
	const char* pszOpRoleId = args.StringVal(nIndex++);
	const char* pszRoleId = args.StringVal(nIndex++);

	// ����������������
	const char* pszCommand = args.StringVal(nIndex++);
	int64_t nCmdId = args.Int64Val(nIndex++);

	CVarList retMsg;
	retMsg << PROTOCOL_ID_GM_REPLY << nCmdId << pszRoleId;
	int nResult = EM_GMCC_RESULT_SUCCEED;
	std::string strMsg_;
	do 
	{
		if (StringUtil::CharIsNull(pszRoleId))
		{
			// ִ�в���Ҫ��ɫ������
			if (m_pGmccModule->IsCommandRule(pKernel, pszCommand, "run_no_player"))
			{
				PERSISTID player;
				nResult = m_pGmccModule->HandleCommand(pKernel, player, pszCommand, args, nIndex, strMsg_);
			}
			else
			{
				// ֪ͨ��Ӫƽ̨��Ҳ�����
				::extend_warning(LOG_ERROR, "[GmccModule::OnDoGmccCommand]lack role id, don't operator by cmd '%s'", pszCommand);
				nResult = EM_GMCC_RESULT_NO_PLAYER;
				strMsg_ = "{\"error\":\"command need role id\"}";
			}
			break;
		}

		extend_warning(LOG_INFO, "[GmccModule::OnDoGmccCommand]execute cmd '%s'(id:%lld)", pszCommand, nCmdId);
		const wchar_t* pwszName = pKernel->SeekRoleName(pszRoleId);
		if ( StringUtil::CharIsNull(pwszName) )
		{
			::extend_warning(LOG_ERROR, "[GmccModule::OnDoGmccCommand]not found roleid('%s')", pszRoleId);
			nResult = EM_GMCC_RESULT_NO_PLAYER;
			strMsg_ = "{\"error\":\"role id not found\"}";
			break;
		}

		// ���˷��
		if (strcmp(pszCommand, GMCC_EXEC_KICK_OUT_IN_TIMER) == 0 || strcmp(pszCommand, GMCC_EXEC_LOCK_PLAYER) == 0)
		{
			int64_t nBlockTime = StringUtil::StringAsInt64(args.StringVal(nIndex));
			int64_t nExpired = util_get_utc_time() + nBlockTime;
			if (PlayerBaseModule::BlockPlayer(pKernel, pwszName, nExpired))
			{
				nResult = EM_GMCC_RESULT_SUCCEED;
				strMsg_ = "{\"content\":\"success\"}";
			}
			else
			{
				nResult = EM_GMCC_RESULT_ERROR;
				strMsg_ = "{\"error\":\"execute block player function failed\"}";
			}
			break;
		}
		// ���
		else if (strcmp(pszCommand, GMCC_EXEC_UNLOCK_PLAYER) == 0)
		{
			if (PlayerBaseModule::BlockPlayer(pKernel, pwszName, 0))
			{
				nResult = EM_GMCC_RESULT_SUCCEED;
				strMsg_ = "{\"content\":\"success\"}";
			}
			else
			{
				nResult = EM_GMCC_RESULT_ERROR;
				strMsg_ = "{\"error\":\"execute block player function failed\"}";
			}
			break;
		}

		// ��ұ������ߵ������
		int nSceneId = pKernel->GetPlayerScene(pwszName);
		if (nSceneId <= 0)
		{
			// ִ�в���Ҫ���ߵ�����
			if (m_pGmccModule->IsCommandRule(pKernel, pszCommand, "run_offline"))
			{
				PERSISTID player;
				nResult = m_pGmccModule->HandleCommand(pKernel, player, pszCommand, args, nIndex, strMsg_);
			}
			else
			{
				// ֪ͨ��Ӫƽ̨��Ҳ�����
				::extend_warning(LOG_ERROR, "[GmccModule::OnDoGmccCommand]'%s' offline, don't operator by cmd '%s'",
					StringUtil::WideStrAsString(pwszName).c_str(), pszCommand);
				nResult = EM_GMCC_RESULT_PLAYER_OFFLINE;
				strMsg_ = "{\"error\":\"player not on line\"}";
			}
		}
		else
		{
			CVarList var;
			var << COMMAND_GMCC << GMCC_CMD_EXEC_GM_COMMAND << nCmdId << pszCommand;
			var.Append(args, nIndex, args.GetCount() - nIndex);
			pKernel->CommandByName(pwszName, var);

			// ����ִ���� �Ȳ�����
			// retMsg << EM_GMCC_RESULT_SUCCEED;
			return;
		}
	} while ( false );

	retMsg << nResult << strMsg_.c_str();
	pKernel->SendExtraMessage(ESI_GMCC_SERVER, retMsg);
}

void GmccModule::OnDoGmccMail(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 13);
	if (args.GetCount() < 13)
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnDoGmccMail]args count error");
		// param error
		return;
	}

	size_t nIndex = 2;
	bool bGlobal = args.BoolVal(nIndex++);				// nIndex + 0
	int64_t nMailId = args.Int64Val(nIndex++);			// nIndex + 1
	int nMailType = args.IntVal(nIndex++);				// nIndex + 2
	int64_t nCreateTime = args.Int64Val(nIndex++);		// nIndex + 3
	int nStatus = args.IntVal(nIndex++);				// nIndex + 4
	int nPeriod = args.IntVal(nIndex++);				// nIndex + 5
	const char* pszTitle = args.StringVal(nIndex++);		// nIndex + 6
	const char* pszContent = args.StringVal(nIndex++);		// nIndex + 7
	const char* pszsenderId = args.StringVal(nIndex++);		// nIndex + 8

	int nRecvCount = args.IntVal(nIndex++);					// nIndex + 9
	CVarList recvers;
	LoopBeginCheck(a);
	for (int i = 0; i < nRecvCount; ++i)
	{
		LoopDoCheck(a);
		const char* pszReciverId = args.StringVal(nIndex++);	// nIndex + 10 + i
		if (!StringUtil::CharIsNull(pszReciverId))
		{
			recvers << pszReciverId;
		}
	}

	::extend_warning(LOG_INFO, "[GmccModule::OnDoGmccMail] recv mail:%lld is global:%s", nMailId, bGlobal ? "true" : "false");
	bool bHasAttach = args.BoolVal(nIndex++);				// nIndex + 10 + nRecvCount
	const char* pszAttachInfo = args.StringVal(nIndex++);	// nIndex + 11 + nRecvCount

	const wchar_t* pwszSender = L"";
	if (!StringUtil::CharIsNull(pszsenderId))
	{
		pwszSender = pKernel->SeekRoleName(pszsenderId);
	}
	if (NULL == pwszSender) pwszSender = L"";

	Assert(NULL != pszTitle);
	Assert(NULL != pszContent);

	std::wstring strTitle;
	std::wstring strContent;
	if (NULL != pszTitle)
	{
		strTitle = StringUtil::UTF8StringAsWideStr(pszTitle);
	}
	if (NULL != pszContent)
	{
		strContent = StringUtil::UTF8StringAsWideStr(pszContent);
	}

	std::string strAttach;
	if (bHasAttach && NULL != pszAttachInfo )
	{
		strAttach = pszAttachInfo;
	}

	CVarList retMsg;
	retMsg << PROTOCOL_ID_MAIL_RESULT << nMailId;
	if (bGlobal)
	{
		if (SystemMailModule::SendGlobalMail(pKernel, pwszSender, strTitle.c_str(), strContent.c_str(), strAttach.c_str(), FUNCTION_EVENT_ID_BS_SEND_GLOBAL_MAIL, nMailId))
		{
			retMsg << EM_GMCC_RESULT_SUCCEED;
		}
		else
		{
			retMsg << EM_GMCC_RESULT_ERROR;
		}
		// ȫ���ʼ���־
		retMsg << 1;
	}
	else
	{
		// ȫ���ʼ���־
		retMsg << EM_GMCC_RESULT_SUCCEED << 0;
		Assert(recvers.GetCount() > 0);
		size_t nCount = recvers.GetCount();
		if (nCount == 0)
		{
			::extend_warning(LOG_ERROR, "gmcc mail recv error.");
		}

		LoopBeginCheck(a);
		int nCountPos = (int)retMsg.GetCount();
		retMsg << nCount;
		for ( size_t i = 0; i < nCount; ++i)
		{
			LoopDoCheck(a);
			const char* pszRecv = recvers.StringVal(i);
			const wchar_t* pwszRecv = pKernel->SeekRoleName(pszRecv);
			retMsg << pszRecv;
			if ( StringUtil::CharIsNull(pwszRecv))
			{
				::extend_warning(LOG_ERROR, "gmcc mail receiver not found(role id:%s).", pszRecv);
				retMsg << EM_GMCC_RESULT_NO_PLAYER;
			}
			else
			{
				if (SystemMailModule::SendMailToPlayer(pKernel, pwszSender, pwszRecv, strTitle.c_str(), strContent.c_str(), strAttach, FUNCTION_EVENT_ID_GMCC_WEB))
				{
					retMsg << EM_GMCC_RESULT_SUCCEED;
				}
				else
				{
					retMsg << EM_GMCC_RESULT_NO_PLAYER;
				}
			}
		}
	}

	pKernel->SendExtraMessage(ESI_GMCC_SERVER, retMsg);
}

void GmccModule::OnDoGmccNotice(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 9);
	if (args.GetCount() < 9)
	{
		// param error
		return;
	}

	size_t nIndex = 2;
	std::wstring& strTitle = StringUtil::StringAsWideStr( args.StringVal(nIndex++) );		// nIndex + 0
	std::wstring& strContent = StringUtil::StringAsWideStr(args.StringVal(nIndex++));		// nIndex + 1
	int64_t nNoticeId = args.Int64Val(nIndex++);				// nIndex + 2
	int64_t nCreateTime = args.Int64Val(nIndex++);				// nIndex + 3
	int64_t nEndTime = args.Int64Val(nIndex++);					// nIndex + 4
	int32_t nInterval = (int)args.Int64Val(nIndex++);			// nIndex + 5
	int32_t nLoop = args.IntVal(nIndex++);						// nIndex + 6
	int32_t nType = args.IntVal(nIndex++);						// nIndex + 7
 	// 0��-1����ѭ�� -1���㴦��
	if (nLoop == 0)
	{
		nLoop = -1;
	}

	/*!
	* @brief	����gmcc����
	* @param	int			ͨ�����n
	����ѭ��n��
	* @param	int64		ͨ��id
	* @param	string		ͨ������
	* @param	int			�Ƿ�ѭ������
	* @param	int			���ż��ʱ��
	*/

	// ���͹���
	CVarList msg;
	// ���㹫�濪ʼʣ��ʱ��
	int64_t tNow = util_get_utc_time();
	int32_t nLeft = 0;
	if (nCreateTime > tNow)
	{
		nLeft = (int32_t)(nCreateTime - tNow);
	}
	msg << SERVER_CUSTOMMSG_GMCC << GMCC_S2C_ADD_NOTICE << 1 << nNoticeId << strContent << nLoop << nInterval << nType << nLeft << nEndTime;
	pKernel->CustomByWorld(msg);

	// ��¼�������� 
	msg.Clear();
	std::wstring wstr = GetDomainName(pKernel);
	msg << PUBSPACE_DOMAIN << wstr.c_str() << SP_DOMAIN_MSG_GMCC_ADD_NOTICE << nNoticeId << strTitle << strContent << nCreateTime << nEndTime << (int)nInterval << nLoop << nType;
	pKernel->SendPublicMessage(msg);
}

void GmccModule::OnDoGmccNoticeDel(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 3);
	if (args.GetCount() < 3)
	{
		// param error
		return;
	}

	size_t nIndex = 2;
	int64_t nNoticeId = args.Int64Val(nIndex++);				// nIndex + 1

	CVarList msg;
	msg << SERVER_CUSTOMMSG_GMCC << GMCC_S2C_DELETE_NOTICE << nNoticeId;
	pKernel->CustomByWorld(msg);

	// ��¼�������� 
	msg.Clear();
	std::wstring wstr = GetDomainName(pKernel);
	msg << PUBSPACE_DOMAIN << wstr.c_str() << SP_DOMAIN_MSG_GMCC_DELETE_NOTICE << nNoticeId;
	pKernel->SendPublicMessage(msg);
}

void GmccModule::OnDoGmccExchResult(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 7);
	if (args.GetCount() < 7)
	{
		// param error
		return;
	}

	size_t nIndex = 2;
	const char* pszRoleId = args.StringVal(nIndex++);		// nIndex + 0
	const char* pszKey = args.StringVal(nIndex++);			// nIndex + 1
	int nResult = args.IntVal(nIndex++);					// nIndex + 2
	const char* pszCapitals = args.StringVal(nIndex++);		// nIndex + 3
	const char* pszItems = args.StringVal(nIndex++);		// nIndex + 4

	Assert(NULL != pszRoleId);
	if (NULL == pszRoleId)
	{
		::extend_warning(LOG_ERROR, "exchange gift result role id is null."); 
		return;
	}
	const wchar_t* pwszRoleName = pKernel->SeekRoleName(pszRoleId);
	if (StringUtil::CharIsNull(pwszRoleName))
	{
		::extend_warning(LOG_ERROR, "exchange gift result role id not found(role id:%s).", pszRoleId);
		return;
	}

	if (nResult == EM_GIFT_CODE_SUCCESS)
	{
		std::string attachData;
		if (NULL != pszCapitals)
		{
			attachData.append(pszCapitals);
		}
		attachData.append(";");
		if (NULL != pszItems)
		{
			attachData.append(pszItems);
		}

		// ���ݽ��������֣���ȡ�ʼ�ģ��
		const ConfigMailItem& mail_template = SystemMailModule::GetConfigMail(pKernel, "mail_exch_gift_result");
		SystemMailModule::SendMailToPlayer(pKernel, mail_template.sender.c_str(),
			pwszRoleName,
			mail_template.mail_title.c_str(),
			mail_template.mail_content.c_str(),
			attachData, FUNCTION_EVENT_ID_SYSTEM_GIF);

		// �ɹ���ʾ
		CVarList msgList;
		msgList << SERVER_CUSTOMMSG_SYSINFO << TIPSTYPE_SHOW_WINDOW_MESSAGE << TIP_EXCH_GIFT_SUCCEED;
		pKernel->CustomByName(pwszRoleName, msgList);
	}
	else
	{
		CVarList msgList;
		msgList << SERVER_CUSTOMMSG_SYSINFO << TIPSTYPE_SHOW_WINDOW_MESSAGE;
		// ʧ����ʾ
		switch (nResult)
		{
		case EM_GIFT_CODE_INVLID:		// δ�ҵ�
			msgList << TIP_EXCH_GIFT_INVALID;
			break;
		case EM_GIFT_CODE_TAKED:		// ����ȡ
			msgList << TIP_EXCH_GIFT_TAKED;
			break;
		case EM_GIFT_CODE_EXPIRED:		// �ѹ���
			msgList << TIP_EXCH_GIFT_EXPIRED;
			break;
		case EM_GIFT_CODE_LIMIT:			// �����ȡ����������
			msgList << TIP_EXCH_GIFT_LIMIT;
			break;
		default:
			msgList << TIP_EXCH_GIFT_UNKOWN;
			break;
		}
		pKernel->CustomByName(pwszRoleName, msgList);
	}
}

std::string ConcatPayedRewards(const IVarList& args, size_t& nIndex, int nCount)
{
	// ƴ����Ʒ�ͽ���
	//std::stringstream items;
	//std::stringstream captials;
	//LoopBeginCheck(a);
	//for (int i = 0; i < nCount; ++i)
	//{
	//	LoopDoCheck(a);
	//	const char* pszItem = args.StringVal(nIndex++);
	//	int nNumber = args.IntVal(nIndex++);
	//	assert(!StringUtil::CharIsNull(pszItem));
	//	assert(nNumber > 0);
	//	// �Ƿ��ǻ���
	//	if (CapitalModule::m_pCapitalModule->IsCapitalDefined(pszItem))
	//	{
	//		captials << pszItem << ":" << nNumber << ",";
	//	}
	//	else
	//	{
	//		items << pszItem << ":" << nNumber << ",";
	//	}
	//}

	//std::string rewards = captials.str();
	//if (rewards.size() > 0)
	//{
	//	// �ض����һ���ַ���
	//	rewards[rewards.size() - 1] = 0;
	//}
	//rewards.append(";");
	//rewards.append(items.str());

	//size_t nSize = rewards.size();
	//if (rewards[nSize - 1] == ',')
	//{
	//	rewards[nSize - 1] = '\0';
	//}

	std::stringstream rewards;
	for (int i = 0; i < nCount; ++i)
	{
		const char* pszItem = args.StringVal(nIndex++);
		int nNumber = args.IntVal(nIndex++);
		assert(!StringUtil::CharIsNull(pszItem));
		assert(nNumber > 0);

		if ( i > 0 )
		{
			rewards << ",";
		}

		rewards << pszItem << ":" << nNumber;
	}

	return rewards.str();
}

void GmccModule::OnDoGmccProductsPush(IKernel* pKernel, const IVarList& args)
{
	// ��Ϣת�������������� ��ʽ: domain domain_name modify_msg mode data(gmcc����������ȫת���� ע��gmcc��public��������ȫһ��)
	CVarList msg;
	std::wstring wstr = GetDomainName(pKernel);
	msg << PUBSPACE_DOMAIN << wstr.c_str() << SP_DOMAIN_MSG_GMCC_PRODUCT_MONDIFY << EM_GMCC_MONDIFY_PUSH;
	int nCount_ = args.GetCount();
	if ((nCount_ - 3) % 9 != 0)
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnDoGmccProductsPush]gmcc push goods data error.");
		return;
	}

	// *����λ�ǲ�Ʒ��������Ϊ: ���� (id �۸� ���� ���� ״̬ ��Ʒ�б� ���ͽ��� �׹� ���������������滻��Ʒ)...
	msg.Append(args, 2, nCount_ - 2);
	pKernel->SendPublicMessage(msg);
}

void GmccModule::OnDoGamccPaymentPush(IKernel* pKernel, const IVarList& args)
{
	// ��Ϣת�������������� ��֪ͨ  �����ȡ
	assert(args.GetCount() >= 10 );
	if (args.GetCount() < 10)
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnDoGamccPaymentPush]gmcc push payment data param count error.");
		return;
	}

	size_t nIndex_ = 2;
	const char* pszRoleId_ = args.StringVal(nIndex_++);				// nIndex + 0
	const char* pszOrder_ = args.StringVal(nIndex_++);				// nIndex + 1
	const char* pszProductId_ = args.StringVal(nIndex_++);			// nIndex + 2
	float fAmount_ = args.FloatVal(nIndex_++);						// nIndex + 3
	int nState_ = args.IntVal(nIndex_++);							// nIndex + 4
	int nBillType_ = args.IntVal(nIndex_++);						// nIndex + 5
	int64_t nOrderTime_ = args.Int64Val(nIndex_++);					// nIndex + 6
	int64_t nPayedTime_ = args.Int64Val(nIndex_++);					// nIndex + 7

	if (!HandlePayment(pKernel, pszRoleId_, pszOrder_, pszProductId_, fAmount_, nBillType_, nOrderTime_, nPayedTime_))
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnDoGamccPaymentPush]gmcc push payment data error.");
	}
}

void GmccModule::OnDoGamccOrderResult(IKernel* pKernel, const IVarList& args)
{
	// �ж��Ƿ���֧���� ��֧��ת�߳ɹ���Ϣ ����ͨ�̿ͻ���֧��
	// ��Ϣת�������������� ��֪ͨ  �����ȡ
	assert(args.GetCount() >= 9);
	if (args.GetCount() < 9)
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnDoGamccOrderResult]gmcc order result data error.");
		return;
	}

	size_t nIndex_ = 2;
	const char* pszRoleId_ = args.StringVal(nIndex_++);				// nIndex + 0
	const char* pszProductId_ = args.StringVal(nIndex_++);			// nIndex + 1
	const char* pszOrder_ = args.StringVal(nIndex_++);				// nIndex + 2
	int nState_ = args.IntVal(nIndex_++);							// nIndex + 3
	int nBillType_ = args.IntVal(nIndex_++);						// nIndex + 4
	const char* pszExtra_ = args.StringVal(nIndex_++);				// nIndex + 5
	float fPrice_ = args.FloatVal(nIndex_++);						// nIndex + 6

	Assert(!StringUtil::CharIsNull(pszRoleId_));
	//Assert(!StringUtil::CharIsNull(pszOrder_));
	Assert(!StringUtil::CharIsNull(pszProductId_));

	extend_warning(LOG_INFO, "[GmccModule::OnDoGamccOrderResult]push order info[roleid:%s order:%s product_id:%s state:%d type:%d extra:%s]",
									pszRoleId_, pszOrder_, pszProductId_, nState_, nBillType_, pszExtra_);

	EmPayOrderResult eResult = EM_PAY_ORDER_RESULT_SUCEED;
	switch (nState_)
	{
	case EM_ORDER_STATE_PAYED:
		{
			// ֧����ɴ���
			int64_t tNow_ = util_get_utc_time();
			if (!HandlePayment(pKernel, pszRoleId_, pszOrder_, pszProductId_, fPrice_, nBillType_, tNow_, tNow_))
			{
				::extend_warning(LOG_ERROR, "[GmccModule::OnDoGamccOrderResult] payment error.");
			}
			eResult = EM_PAY_ORDER_RESULT_PAYED;
		}
		break;
	case EM_ORDER_STATE_UNKOWN:
		eResult = EM_PAY_ORDER_RESULT_ORDING;
		break;
	case EM_ORDER_STATE_ARRIVED:
	case EM_ORDER_STATE_TAKE:
	case EM_ORDER_STATE_FINISH:
		eResult = EM_PAY_ORDER_RESULT_PAYED;
		break;
	}
	
	// �µ����� ���ظ��ͻ���
	ShopModule::OnOrderInfo(pKernel, pszRoleId_, pszOrder_, pszProductId_, pszExtra_, eResult, fPrice_);
}

void GmccModule::OnDoGmccPropQuery(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 4);
	if (args.GetCount() < 4)
	{
		// param error
		return;
	}

	size_t nIndex = 2;
	int64_t nQueryId = args.Int64Val(nIndex++);				// nIndex + 0
	const char* pszRoleId = args.StringVal(nIndex++);		// nIndex + 1
	Assert(!StringUtil::CharIsNull(pszRoleId));
	const wchar_t* pwszRoleName = pKernel->SeekRoleName(pszRoleId);
	if (StringUtil::CharIsNull(pwszRoleName))
	{
		CVarList ret;
		ret << PROTOCOL_ID_ROLE_PROP_DATA << nQueryId << DATA_QUERY_ROLEID_ERR << pszRoleId;
		pKernel->SendExtraMessage(ESI_GMCC_SERVER, ret);
		return;
	}

	int nSceneId = pKernel->GetPlayerScene(pwszRoleName);
	if (nSceneId <= 0)
	{
		CVarList ret;
		ret << PROTOCOL_ID_ROLE_PROP_DATA << nQueryId << DATA_QUERY_ROLEIN_OFFLINE << pszRoleId;
		pKernel->SendExtraMessage(ESI_GMCC_SERVER, ret);
		return;
	}

	const PERSISTID player = pKernel->FindPlayer(pwszRoleName);
	IGameObj* pPlayer = pKernel->GetGameObj(player);
	if (NULL != pPlayer)
	{
		OnPropQuery(pKernel, pPlayer, args, nIndex, nQueryId);
	}
	else
	{
		CVarList cmd;
		cmd << COMMAND_GMCC << GMCC_CMD_EXEC_PROP_QUERY << nQueryId;
		cmd.Append(args, nIndex, args.GetCount() - nIndex);
		pKernel->CommandByName(pwszRoleName, cmd);
	}
}

void GmccModule::OnDoGmccRecordQuery(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 7);
	if (args.GetCount() < 7)
	{
		// param error
		return;
	}

	size_t nIndex = 2;
	int64_t nQueryId = args.Int64Val(nIndex++);				// nIndex + 0
	const char* pszRoleId = args.StringVal(nIndex++);		// nIndex + 1
	const char* pszRcordName = args.StringVal(nIndex++);	// nIndex + 2
	int nOffset = args.IntVal(nIndex++);					// nIndex + 3
	int nCount = args.IntVal(nIndex++);						// nIndex + 4
	Assert(!StringUtil::CharIsNull(pszRoleId));
	const wchar_t* pwszRoleName = pKernel->SeekRoleName(pszRoleId);
	if (StringUtil::CharIsNull(pwszRoleName))
	{
		CVarList ret;
		ret << PROTOCOL_ID_ROLE_RECORD_DATA << nQueryId << pszRoleId << pszRcordName << DATA_QUERY_ROLEID_ERR
			<< nOffset << nCount << 0;
		pKernel->SendExtraMessage(ESI_GMCC_SERVER, ret);
		return;
	}

	int nSceneId = pKernel->GetPlayerScene(pwszRoleName);
	if (nSceneId <= 0)
	{
		CVarList ret;
		ret << PROTOCOL_ID_ROLE_RECORD_DATA << nQueryId << pszRoleId << pszRcordName << DATA_QUERY_ROLEIN_OFFLINE
			<< nOffset << nCount << 0;
		pKernel->SendExtraMessage(ESI_GMCC_SERVER, ret);
		return;
	}

	const PERSISTID player = pKernel->FindPlayer(pwszRoleName);
	IGameObj* pPlayer = pKernel->GetGameObj(player);
	if (NULL != pPlayer)
	{
		OnRecordQuery(pKernel, pPlayer,  pszRcordName, nOffset, nCount, nQueryId);
	}
	else
	{
		CVarList cmd;
		cmd << COMMAND_GMCC << GMCC_CMD_EXEC_RECORD_QUERY << nQueryId << pszRcordName << nOffset << nCount;
		pKernel->CommandByName(pwszRoleName, cmd);
	}
}

//#define TEST_NOTIFY_PUSH
#ifdef TEST_NOTIFY_PUSH
#include "PushDataModule.h"
// ����֪ͨ
#endif

void GmccModule::OnDoGmccInitSrvInfo(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 8);
	if (args.GetCount() < 8)
	{
		// param error
		return;
	}

#ifdef _DEBUG
	SrvInfo _info;
	size_t nIndex = 2;
	_info.Identity = args.Int64Val(nIndex++);				// nIndex + 0
	_info.GameId = args.Int64Val(nIndex++);					// nIndex + 1
	_info.ChannelId = args.Int64Val(nIndex++);				// nIndex + 2
	_info.ProductionId = args.Int64Val(nIndex++);			// nIndex + 3
	_info.ServerId = args.Int64Val(nIndex++);				// nIndex + 4
	_info.AreaId = args.Int64Val(nIndex++);					// nIndex + 5
	_info.DeployId = args.Int64Val(nIndex++);
#endif
	// �޸���־������Ϣ
	{
		CVarList cmd;
		cmd << COMMAND_GMCC << GMCC_CMD_SYNC_SEV_INFO;
		cmd.Append(args, 2, args.GetCount() - 2);
		BroadcastMemberMessage(pKernel, cmd);
	}

#ifdef TEST_NOTIFY_PUSH
	// ����֪ͨ
	PushDataModule::PushNotification(pKernel, "test title", "this is content");
#endif
}

void GmccModule::OnDoGmccRebateInfo(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 4);
	if (args.GetCount() < 4)
	{
		// param error
		return;
	}

	size_t nIndex = 2;
	const char* rid_ = args.StringVal(nIndex++);
	int nRebate_ = args.IntVal(nIndex++);
	int nType_ = args.IntVal(nIndex++);

	const wchar_t* pszName_ = pKernel->SeekRoleName(rid_);
	if (StringUtil::CharIsNull(pszName_))
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnDoGmccRebateInfo] role id not found[rid:%s]", rid_);
		return;
	}

	// ֪ͨ��Ҵ���
	CVarList cmd;
	cmd << COMMAND_GMCC << GMCC_CMD_REBATE_INFO << nType_ << nRebate_;
	pKernel->CommandByName(pszName_, cmd);
}

void GmccModule::OnDoGmccRebateResult(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 5);
	if (args.GetCount() < 5)
	{
		// param error
		return;
	}

	size_t nIndex = 2;
	const char* rid_ = args.StringVal(nIndex++);
	int nResult_ = args.IntVal(nIndex++);
	int nRebate_ = args.IntVal(nIndex++);

	const wchar_t* pszName_ = pKernel->SeekRoleName(rid_);
	if (StringUtil::CharIsNull(pszName_))
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnDoGmccRebateResult] role id not found[rid:%s]", rid_);
		return;
	}

	// ֪ͨ��Ҵ���
	CVarList cmd;
	cmd << COMMAND_GMCC << GMCC_CMD_REBATE_RESULT << nResult_ << nRebate_;
	pKernel->CommandByName(pszName_, cmd);
}

void GmccModule::OnDoGmccPushShopItems(IKernel* pKernel, const IVarList& args)
{
	extend_warning(LOG_INFO, "[GmccModule::OnDoGmccPushShopItems] args:%d", args.GetCount() - 1);
	ShopModule::AddPlusShopItem(pKernel, args, 2);
}

void GmccModule::OnDoGmccDelShopItems(IKernel* pKernel, const IVarList& args)
{
	extend_warning(LOG_INFO, "[GmccModule::OnDoGmccDelShopItems] args:%d", args.GetCount() - 1);
	ShopModule::RemovePlusShopItem(pKernel, args, 2);
}

void GmccModule::OnDoGmccQueryShopItems(IKernel* pKernel, const IVarList& args)
{
	CVarList msg;
	msg << PROTOCOL_ID_SHOP_ITEM_LIST << 0; // ��2λΪ��Ʒ��
	
	int nCount = ShopModule::GetPlusShopList(pKernel, msg);
	// ����Ϊʵ�ʵ�����
	msg.SetInt(1, nCount);
	if (!pKernel->SendExtraMessage(ESI_GMCC_SERVER, msg))
	{
		extend_warning(LOG_ERROR, "[GmccModule::OnDoGmccQueryShopItems] return plus shop list failed.");
	}
}

void GmccModule::OnDoGmccLoadConfig(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 4);
	if (args.GetCount() < 4)
	{
		extend_warning(LOG_ERROR, "[GmccModule::OnDoGmccSetGenActivityConfig]args count error");
		// param error
		return;
	}

	size_t _nIndex = 2;
	int64_t nSeq = args.Int64Val(_nIndex++);
	int32_t nCount = args.IntVal(_nIndex++);

	for (int32_t i = 0; i < nCount; ++i )
	{
		const char* pszKey = args.StringVal(_nIndex++);
		// ����path,catalog,cfgid
		_nIndex += 3;
		extend_warning(LOG_INFO, "[GmccModule::OnDoGmccSetGenActivityConfig]reload config '%s'", pszKey);
		ReLoadConfigModule::m_pReLoadConfigModule->SendReloadConfigCommand(pKernel, pszKey);
	}
	
	ConfirmGmccAck(pKernel, nSeq);
}

void GmccModule::OnDoGmccSetGenActivityConfig(IKernel* pKernel, const IVarList& args)
{
	assert(args.GetCount() >= 3);
	if (args.GetCount() < 3)
	{
		// param error
		return;
	}

	size_t _nIndex = 2;

	const wchar_t* _pszName = args.WideStrVal(_nIndex++);
	const wchar_t* _pszDesc = args.WideStrVal(_nIndex++);
	int _nType = args.IntVal(_nIndex++);
	int64_t _startTime = args.Int64Val(_nIndex++);
	int64_t _endTime = args.Int64Val(_nIndex++);

	// ��֤���ݳ��� ActivityItem(gmcc\ShopItem.h)
	Assert((args.GetCount() - _nIndex - 1) % 4 == 0);
	if (_nType == EM_ACT_TYPE_PLAYER_LEVEL_MATCH)
	{
		// ���ó弶����
		ShopModule::SetPlushActivityConfig(pKernel, (EmPlusActivityType)_nType, _pszName, _pszDesc, _startTime, _endTime, args, _nIndex);
	}
	else
	{
		Assert(false);
		extend_warning(LOG_ERROR, "[GmccModule::OnDoGmccSetGenActivityConfig]don't support config type:%d.", _nType);
	}
}

void GmccModule::OnClientExchGift(IKernel* pKernel, const PERSISTID& player, const char* exchKey)
{
	Assert(NULL != exchKey);
	const char* pAccId = pKernel->QueryString(player, FIELD_PROP_ACCOUNT);
	const char* pRoleId = pKernel->QueryString(player, FIELD_PROP_UID);
	if (StringUtil::CharIsNull(pAccId) || StringUtil::CharIsNull(pRoleId))
	{
		extend_warning(LOG_ERROR, "[GmccModule::OnClientExchGift]accid or roleid is null.");
		return;
	}

	do 
	{
		// �ж�key�Ƿ�Ϸ�(��ү����: ��ĸ������� ���ִ�Сд ~~ � 32 )
		if ( strlen(exchKey) > 32 )
		{
			break;
		}

		if ( !StringUtil::StringIsAlnum(exchKey))
		{
			break;
		}

		CVarList args;
		args << PROTOCOL_ID_GIFT_EXCH << exchKey << pAccId << pRoleId;
		if (!pKernel->SendExtraMessage(ESI_GMCC_SERVER, args))
		{
			CVarList param;
			::CustomSysInfo(pKernel, player, TIPSTYPE_NORMAL_CHAT_MESSAGE, TIP_EXCH_GIFT_UNKOWN, param);
		}

		return;
	} while (false);
	
	extend_warning(LOG_TRACE, "[GmccModule::OnClientExchGift]key invalid:%s.", exchKey);
	CVarList msgList;
	msgList << SERVER_CUSTOMMSG_SYSINFO << TIPSTYPE_SHOW_WINDOW_MESSAGE << TIP_EXCH_GIFT_INVALID;
	pKernel->Custom(player, msgList);
}

void GmccModule::OnClientPickPayedRebate(IKernel* pKernel, const PERSISTID& player)
{
	// �Ƿ���ȡ����
	bool bPicked_ = PlayerBaseModule::TestPlayerGlobalFlag( pKernel, player, EM_PLAYER_GLOBAL_PAYED_REBATE_PICKED);
	if (bPicked_)
	{
		// ������Ϣ ��ȡ����ʲôҲ������
		return;
	}

	IGameObj* pPlayer_ = pKernel->GetGameObj(player);
	if (NULL == pPlayer_)
	{
		return;
	}	
	
	// ���ݷ����ֶη�����Ԫ��
	int nSliver_ = pPlayer_->QueryInt(FIELD_PROP_PAYED_REBATE);
	if (nSliver_ <= 0)
	{
		// ������Ϣ
		::extend_warning(LOG_WARNING, "[GmccModule::OnClientPickPayedRebate] rebate lower zero[uid:%s]", 
							pPlayer_->QueryString(FIELD_PROP_UID));
		return;
	}

	// ֪ͨ�Ʒ���ȡ
	const char* pszRoleId_ = pPlayer_->QueryString(FIELD_PROP_UID);
	const char* pszUid_ = pPlayer_->QueryString(FIELD_PROP_ACCOUNT);
	const wchar_t* pwszName_ = pPlayer_->QueryWideStr(FIELD_PROP_NAME);
	// ����Ϣ��web��ѯ����
	CVarList args;
	args << PROTOCOL_ID_REBATE_PICK
		<< pszUid_
		<< pszRoleId_
		<< pwszName_;

	pKernel->SendExtraMessage(ESI_GMCC_SERVER, args);
}

void GmccModule::OnGameReady(IKernel* pKernel)
{
	int member_id = pKernel->GetMemberId();
	if (member_id != 0)
	{
		//by kevin ����ԭ�⣬ֻ֪ͨmaster���Ƿ�Ҫ���ţ����������Ա���ͣ������š� 2017-08-07 20:54
		return;
	}
	CVarList args;
	args << PROTOCOL_ID_GAME_READY;
	pKernel->SendExtraMessage(ESI_GMCC_SERVER, args);

	// �������������Ϣ����
	if (!ms_bRecivedSrvInfo)
	{
		if (!pKernel->FindHeartBeat(pKernel->GetScene(), "GmccModule::HB_RequestSrvInfo"))
		{
			// 1����
			pKernel->AddHeartBeat(pKernel->GetScene(), "GmccModule::HB_RequestSrvInfo", 60000);
		}
	}
}

void GmccModule::ConfirmGmccAck(IKernel* pKernel, int64_t nSeq)
{
	CVarList args;
	args << PROTOCOL_ID_CONFIRM_ACK << nSeq;
	pKernel->SendExtraMessage(ESI_GMCC_SERVER, args);
}

int GmccModule::HandleCommand(IKernel* pKernel, const PERSISTID& player, 
					const char* cmd, const IVarList& args, int offset, std::string& outMsg)
{
	int nResult = EM_GMCC_RESULT_SUCCEED;
	outMsg = "{\"content\":\"success\"}";
	if (strncmp(cmd, GMCC_GM_COMMAND_PRI, strlen(GMCC_GM_COMMAND_PRI)) == 0)
	{
		// ȥ��gmǰ׺
		if (!m_pGmModule->RunGmCommand(pKernel, player, cmd + strlen(GMCC_GM_COMMAND_PRI), args, offset, args.GetCount() - offset, outMsg))
		{
			nResult = EM_GMCC_RESULT_ERROR;
		}
	}
	else
	{
		if (strcmp(cmd, GMCC_EXEC_FORBID_SPEAK) == 0)
		{
			if (!m_pGmccModule->OnForbidSpeak(pKernel, player, args, offset))
			{
				outMsg = "{\"error\":\"execute failed\"}";
				nResult = EM_GMCC_RESULT_ERROR;
			}
		}
		else if (strcmp(cmd, GMCC_EXEC_ALLOW_SPEAK) == 0)
		{
			if (!m_pGmccModule->OnAllowSpeak(pKernel, player, args, offset))
			{
				outMsg = "{\"error\":\"execute failed\"}";
				nResult = EM_GMCC_RESULT_ERROR;
			}
		}
		else if (strcmp(cmd, GMCC_EXEC_GRANT_AUTH) == 0)
		{
			if (!m_pGmccModule->OnGrantAuth(pKernel, player, args, offset))
			{
				outMsg = "{\"error\":\"execute failed\"}";
				nResult = EM_GMCC_RESULT_ERROR;
			}
		}
		else if (strcmp(cmd, GMCC_EXEC_RELOAD_LUA) == 0)
		{
			if ( !pKernel->LuaLoadScript(args.StringVal(offset)) )
			{
				outMsg = "{\"error\":\"execute failed\"}";
				nResult = EM_GMCC_RESULT_ERROR;
			}
		}
		else
		{
			// �������
			outMsg = "{\"error\":\"command not found\"}";
			nResult = EM_GMCC_RESULT_ERROR;
		}
	}

	return nResult;
}

bool GmccModule::IsCommandRule(IKernel* pKernel, const char* cmd, const char* rule)
{
	if (LuaScriptModule::m_pLuaScriptModule)
	{
		if (strncmp(cmd, GMCC_GM_COMMAND_PRI, strlen(GMCC_GM_COMMAND_PRI)) == 0)
		{
			// ȥ��gmǰ׺
			cmd = cmd + strlen(GMCC_GM_COMMAND_PRI);
		}

		CVarList results;
		CVarList luaargs;
		luaargs << cmd;
		LuaScriptModule::m_pLuaScriptModule->RunLuaScript(pKernel, "gm_x_rule.lua", rule, luaargs, 1, &results);
		return strcmp(results.StringVal(0), "ok") == 0;
	}

	return false;
}

const std::wstring & GmccModule::GetDomainName(IKernel * pKernel)
{
	if (m_domainName.empty())
	{
		wchar_t wstr[256];

		const int server_id = pKernel->GetServerId();
		SWPRINTF_S(wstr, L"DomainGmcc_%d", server_id);
		m_domainName = wstr;
	}

	return m_domainName;
}

IPubData* GmccModule::GetPubData(IKernel* pKernel)
{
	IPubSpace* pPubSpace_ = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (NULL == pPubSpace_)
	{
		return NULL;
	}
	IPubData* pPubData_ = pPubSpace_->GetPubData(GetDomainName(pKernel).c_str());
	return pPubData_;
}

IRecord* GmccModule::GetPubRecord(IKernel* pKernel, const char* pszRecName)
{
	IPubData * pPubData_ = GetPubData(pKernel);
	if (pPubData_ == NULL)
	{
		return NULL;
	}

	IRecord* pRecord_ = pPubData_->GetRecord(pszRecName);
	return pRecord_;
}

void GmccModule::LoadPropsValue(IGameObj* pObject, IVarList& values, const IVarList& keys, size_t nOffset)
{
	Assert(NULL != pObject);
	size_t nCount = keys.GetCount();
	while (nOffset < nCount)
	{
		const char* pszKey = keys.StringVal(nOffset++);
		if (StringUtil::CharIsNull(pszKey))
		{
			continue;
		}

		int nType = pObject->GetAttrType(pszKey);
		switch ( nType )
		{
		case VTYPE_BOOL:
			values.AddInt(pObject->QueryInt(pszKey));
			break;
		case VTYPE_INT:
			values.AddInt(pObject->QueryInt(pszKey));
			break;
		case VTYPE_INT64:
			values.AddInt64(pObject->QueryInt64(pszKey));
			break;
		case VTYPE_FLOAT:
			values.AddFloat(pObject->QueryFloat(pszKey));
			break;
		case VTYPE_DOUBLE:
			values.AddDouble(pObject->QueryDouble(pszKey));
			break;
		case VTYPE_STRING:
			values.AddString(pObject->QueryString(pszKey));
			break;
		case VTYPE_WIDESTR:
			values.AddWideStr(pObject->QueryWideStr(pszKey));
			break;
		case VTYPE_OBJECT:
			values.AddInt64(pObject->QueryObject(pszKey).nData64);
			break;
		default:
			break;
		}
	}
}

void GmccModule::LoadRecordData(IGameObj* pObject, IRecord* pRec, int nOffset, int nCount, IVarList& retData)
{
	Assert(NULL != pObject);
	Assert(NULL != pRec);
	int nEnd = nOffset + nCount;
	if (nEnd > pRec->GetRows())
	{
		nEnd = pRec->GetRows();
	}
	int nCols = pRec->GetCols();
	LoopBeginCheck(a);
	for ( int i = nOffset; i < nEnd; ++i )
	{
		LoopDoCheck(a);

		LoopBeginCheck(b);
		for (int j = 0; j < nCols; ++j)
		{
			LoopDoCheck(b);
			int nType = pRec->GetColType(j);
			switch (nType)
			{
			case VTYPE_BOOL:
				retData.AddInt(pRec->QueryInt(i, j));
				break;
			case VTYPE_INT:
				retData.AddInt(pRec->QueryInt(i, j));
				break;
			case VTYPE_INT64:
				retData.AddInt64(pRec->QueryInt64(i, j));
				break;
			case VTYPE_FLOAT:
				retData.AddFloat(pRec->QueryFloat(i, j));
				break;
			case VTYPE_DOUBLE:
				retData.AddDouble(pRec->QueryDouble(i, j));
				break;
			case VTYPE_STRING:
				retData.AddString(pRec->QueryString(i, j));
				break;
			case VTYPE_WIDESTR:
				retData.AddWideStr(pRec->QueryWideStr(i, j));
				break;
			case VTYPE_OBJECT:
				retData.AddInt64(pRec->QueryObject(i, j).nData64);
				break;
			default:
				Assert(false);
				retData.AddInt(pRec->QueryInt(i, j));
				break;
			}
		}
	}
}

int GmccModule::BroadcastMemberMessage(IKernel* pKernel, const IVarList& args)
{
	// �����ռ�
	IPubSpace* pPubSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (pPubSpace == NULL)
	{
		return 0;
	}

	// ��������
	wchar_t pubDataName[256];
	SWPRINTF_S(pubDataName, L"Domain_SceneList%d", pKernel->GetServerId());
	IPubData* pPubData = pPubSpace->GetPubData(pubDataName);
	if (pPubData == NULL)
	{
		return 0;
	}

	// �ϼܱ�
	IRecord* pRecord = pPubData->GetRecord(SERVER_MEMBER_FIRST_LOAD_SCENE_REC);
	if (pRecord == NULL)
	{
		return 0;
	}
	int rows = pRecord->GetRows();
	LoopBeginCheck(a);
	for (int i = 0; i < rows; ++i)
	{
		LoopDoCheck(a);
		pKernel->CommandToScene(pRecord->QueryInt(i, SERVER_MEMBER_FIRST_LOAD_SCENE_COL_scene_id), args);
	}

	return rows;
}

bool GmccModule::OnClientOrder(IKernel * pKernel, const char* pszRoleId, const wchar_t* pwszName, 
								const char* pszProductId, const char* pszToken)
{
	Assert(!StringUtil::CharIsNull(pszRoleId));
	Assert(!StringUtil::CharIsNull(pszProductId));
	Assert(!StringUtil::CharIsNull(pszToken));

	if (StringUtil::CharIsNull(pszRoleId) || StringUtil::CharIsNull(pszProductId) || StringUtil::CharIsNull(pszToken))
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnClientOrder] param is emty");
		return false;
	}

	// �ͻ����µ�����
	CVarList args;
	args << PROTOCOL_ID_ORDER_REQ
		<< pszRoleId
		<< pwszName
		<< pszToken
		<< pszProductId;

	return pKernel->SendExtraMessage(ESI_GMCC_SERVER, args);
}

bool GmccModule::OnOrderStateChanged(IKernel* pKernel, const char* pszRoleId, const char* pszOrder, int nState)
{
	Assert(!StringUtil::CharIsNull(pszRoleId));
	Assert(!StringUtil::CharIsNull(pszOrder));

	if (StringUtil::CharIsNull(pszRoleId) || StringUtil::CharIsNull(pszOrder))
	{
		::extend_warning(LOG_ERROR, "[GmccModule::OnOrderStateChanged] param is emty");
		return false;
	}

	// ֪ͨ��չ����������״̬�ı�
	CVarList args;
	args << PROTOCOL_ID_ORDER_NOTIFY
		<< pszRoleId
		<< pszOrder
		<< nState;

	return pKernel->SendExtraMessage(ESI_GMCC_SERVER, args);
}
