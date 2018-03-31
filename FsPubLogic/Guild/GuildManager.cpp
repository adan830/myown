//--------------------------------------------------------------------
// �ļ���:		GuildManager.cpp
// ��  ��:		����ϵͳ������
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#include "GuildManager.h"
#include <time.h>
#include <algorithm>
#include "../../FsGame/SocialSystemModule/GuildLoadResource.h"
#include "../../FsGame/Define/PubDefine.h"
#include "../../utils/util_func.h"
#include "../../utils/string_util.h"
#include "../../utils/util.h"
#include "../../utils/record.h"
#include "../../FsGame/Define/GameDefine.h"
#include "utils/custom_func.h"
#include "server/LoopCheck.h"
#include "FsGame/Define/CommandDefine.h"
 
#include "../Domain/EnvirValue.h"
#include <list>
#include "GuildBaseManager.h"
#include "FsGame/Define/PubCmdDefine.h"
#include "GuildNumManage.h"
 

// һ���Сʱ��
int DAY_HOURS = 24;
// һСʱ������
int HOUR_MINUTES = 60;
// һ��������
int MINUTE_SECOND = 60;
// ʮ��������
int TEN_MINUTE_SECOND = 600;
// һСʱ������
int HOUR_SECONDS = 3600;

PubGuildManager* PubGuildManager::m_pInstance = NULL;

PubGuildManager::PubGuildManager()
:m_pPubSpace(NULL),m_pGuildPubData(NULL)
{
}

PubGuildManager::~PubGuildManager()
{
}

IPubLogic* PubGuildManager::Instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = NEW PubGuildManager;
	}

	return m_pInstance;
}

const char* PubGuildManager::GetName()
{
	return PUBSPACE_GUILD;
}

// �������ݷ���������
int PubGuildManager::OnPublicStart(IPubKernel* pPubKernel)
{
	m_ServerId = ::util_int64_as_widestr(pPubKernel->GetServerId());
	return 1;
}

// �������ݷ������ر�
int PubGuildManager::OnServerClose(IPubKernel* pPubKernel)
{
	// ��ȡ��ǰʱ��
	int year, month, day, hour, minute, second;
	util_get_year_month_day(year, month, day);
	util_get_hour_minute_second(hour, minute, second);

	// ��¼�رշ�����ʱ��
	m_pGuildPubData->SetAttrInt("CloseYear", year);
	m_pGuildPubData->SetAttrInt("CloseMonth", month);
	m_pGuildPubData->SetAttrInt("CloseDay", day);
	m_pGuildPubData->SetAttrInt("CloseHour", hour);
	m_pGuildPubData->SetAttrInt("CloseMinute", minute);
	m_pGuildPubData->SetAttrInt("CloseSecond", second);

	pPubKernel->SavePubSpace(GetName());

	return 1;
}

// �������ݿռ��е�������������
int PubGuildManager::OnPubSpaceLoaded(IPubKernel* pPubKernel)
{
	IPubSpace* pPubSpace = pPubKernel->GetPubSpace(GetName());
	if (pPubSpace == NULL)
	{
		return 0;
	}
	m_pPubSpace = pPubSpace;

	std::wstring guildPubDataName = std::wstring(GUILD_PUB_DATA) + m_ServerId;
	// ����Ƿ񴴽��˹���Ĺ�����������û���򴴽�
	if (!pPubSpace->FindPubData(guildPubDataName.c_str()))
	{
		if (!pPubSpace->AddPubData(guildPubDataName.c_str(), true))
		{
			return 0;
		}
	}

	// ��ȡ����Ĺ���������
	IPubData* pGuildPubData = pPubSpace->GetPubData(guildPubDataName.c_str());
	if (pGuildPubData == NULL)
	{
		return 0;
	}
	m_pGuildPubData = pGuildPubData;

	AddGuildSub(new GuildBaseManager());
	//AddGuildSub(new GuildBuildingManage());
	AddGuildSub(new GuildNumManage());
	//AddGuildSub(new GuildSkillManage());
	//AddGuildSub(new PrepareGuildManage());
	//AddGuildSub(new GuildRareTreasure());
	//AddGuildSub(new GuildBonus());
	//AddGuildSub(new GuildOccupyFlag());
	//AddGuildSub(new GuildWeeklyScore());
	CallBackAllSub([pPubKernel](GuildBase *pBase)->bool{
		if (pBase != NULL)
		{
			if (!pBase->Init(pPubKernel)){
				return false;
			}
		}
		return true;
	});


	CallBackAllGuild(
		[](IPubData* pGuildData)->bool{
		return m_pInstance->CallBackAllSub([pGuildData](GuildBase *pBase)->bool{
			if (pBase != NULL)
			{
				if (!pBase->CreateGuildInit(pGuildData)){
					return false;
				}
			}
			return true;
		});
	});

	CallBackAllGuild(
		[](IPubData* pGuildData)->bool{
		return m_pInstance->CallBackAllSub([pGuildData](GuildBase *pBase)->bool{
			if (pBase != NULL)
			{
				if (!pBase->Patch(pGuildData)){
					return false;
				}
			}
			return true;
		});
	});
	
	pPubKernel->SavePubSpace(GetName());


	// ��ȡ����
	
	InitTimer(pPubKernel);
	UpdateGuildTimer(pPubKernel);

	port_date_time_t now;
	Port_GetLocalTime(&now);
	ResetGuildTimer(pPubKernel, now);

	return 1;
}

// ����������������
int PubGuildManager::OnPubDataLoaded(IPubKernel* pPubKernel, const wchar_t* data_name)
{
	return 1;
}


// ��ᶨʱ�ص�ˢ��
void PubGuildManager::UpdateGuildTimer(IPubKernel* pPubKernel)    
{
	// ��ȡ��ǰʱ��
	int year, month, day, hour, minute, second;   
	util_get_year_month_day(year, month, day);
	util_get_hour_minute_second(hour, minute, second);

	// ��ȡ����Ĺ���������
	int closeYear = m_pGuildPubData->QueryAttrInt("CloseYear");
	
	

	// û�йرշ�����ʱ��
	if (closeYear <= 0)
	{
		// ��ȡ��ǰʱ��
		int year, month, day, hour, minute, second;
		util_get_year_month_day(year, month, day);
		util_get_hour_minute_second(hour, minute, second);
		// ��¼�رշ�����ʱ��
		m_pGuildPubData->SetAttrInt("CloseYear", year);
		m_pGuildPubData->SetAttrInt("CloseMonth", month);
		m_pGuildPubData->SetAttrInt("CloseDay", day);
		m_pGuildPubData->SetAttrInt("CloseHour", hour);
		m_pGuildPubData->SetAttrInt("CloseMinute", minute);
		m_pGuildPubData->SetAttrInt("CloseSecond", second);
	}
	 closeYear = m_pGuildPubData->QueryAttrInt("CloseYear");
	int closeMonth = m_pGuildPubData->QueryAttrInt("CloseMonth");
	int closeDay = m_pGuildPubData->QueryAttrInt("CloseDay");
	int closeHour = m_pGuildPubData->QueryAttrInt("CloseHour");
	int closeMinute = m_pGuildPubData->QueryAttrInt("CloseMinute");

	double closeTime = 0.0f;
	util_encode_date(closeYear, closeMonth, closeDay, closeTime);
	double nowTime = 0.0f;
	util_encode_date(year, month, day, nowTime);
	double diffTime = util_get_time_diff(nowTime, closeTime);
	if (diffTime < 0.0f)
	{
		return;
	}

	// ������Χ���Ͷ���
	enum RangeType
	{
		RT_ALL,
		RT_INCLUDE,
		RT_EXCEPT,
	};

	RangeType rangeType = RT_ALL;

	GuildTimerCallback_t minTime;
	GuildTimerCallback_t maxTime;

	LoopBeginCheck(a);
	do
	{
		LoopDoCheck(a);
		// ��������+
		if (diffTime > DBL_EPSILON + 1)
		{
			break;
		}

		// ����
		if (diffTime > DBL_EPSILON)
		{
			// ����һ��+
			if (hour > closeHour)
			{
				break;
			}

			// ����һ��+
			if (hour == closeHour && minute >= closeMinute)
			{
				break;
			}

			// ����һ��-
			if (hour < closeHour || hour == closeHour && minute < closeMinute)
			{
				rangeType			= RT_EXCEPT;
				minTime.m_Hour		= hour;
				minTime.m_Minute	= minute;
				maxTime.m_Hour		= closeHour;
				maxTime.m_Minute	= closeMinute;

				break;
			}

			break;
		}

		// ����
		rangeType			= RT_INCLUDE;
		minTime.m_Hour		= closeHour;
		minTime.m_Minute	= closeMinute;
		maxTime.m_Hour		= hour;
		maxTime.m_Minute	= minute;

	} while (0);

	// ����ˢ�����ͣ����ûص�����
	std::wstring guildPubDataName = (GUILD_PUB_DATA + m_ServerId).c_str();
	GuildTimerCallbackVector::iterator iter(m_GuildTimer.begin());

	switch (rangeType)
	{
	case RT_INCLUDE: // ʱ��������
		{
			// ѭ������
			LoopBeginCheck(at);

			for ( ; iter != m_GuildTimer.end(); ++iter)
			{
				LoopDoCheck(at);
				GuildTimerCallback_t& guildTime = *iter;
				if (minTime < guildTime && guildTime < maxTime)
				{
					guildTime.m_CallbackFunc(pPubKernel, GetName(), guildPubDataName.c_str());
				}
			}
		}
		break;
	case RT_EXCEPT: // ʱ��������
		{
			// ѭ������
			LoopBeginCheck(au);

			for ( ; iter != m_GuildTimer.end(); ++iter)
			{
				LoopDoCheck(au);
				GuildTimerCallback_t& guildTime = *iter;
				if (guildTime < minTime || maxTime < guildTime)
				{
					guildTime.m_CallbackFunc(pPubKernel, GetName(), guildPubDataName.c_str());
				}
			}
		}
		break;
	case RT_ALL: // �������лص�   
	default:
		{
			// ѭ������
			LoopBeginCheck(av);

			for ( ; iter != m_GuildTimer.end(); ++iter)
			{
				LoopDoCheck(av);
				GuildTimerCallback_t& guildTime = *iter;
				guildTime.m_CallbackFunc(pPubKernel, GetName(), guildPubDataName.c_str());
			}
		}
		break;   
	}
}

// ��ȡ�������
IPubData* PubGuildManager::GetGuildPubData(IPubKernel* pPubKernel, const wchar_t* guildname)
{
	// ��ȡ�����pubspace
	IPubSpace* pPubSpace = pPubKernel->GetPubSpace(PUBSPACE_GUILD);
	if (pPubSpace == NULL)
	{
		return NULL;
	}

	// ��ȡ�������
	int nServerId = pPubKernel->GetServerId();
	std::wstring wsGuildDataName = std::wstring(guildname) + StringUtil::IntAsWideStr(nServerId);
	return pPubSpace->GetPubData(wsGuildDataName.c_str());
}

// ��ᶨʱ������
void PubGuildManager::ResetGuildTimer(IPubKernel* pPubKernel, const port_date_time_t& now)
{
	if (!m_GuildTimer.empty())
	{
		std::wstring guildPubDataName = (std::wstring(GUILD_PUB_DATA) + m_ServerId).c_str();
		if (pPubKernel->FindTimer(GUILD_TIMER_NAME, GetName(), guildPubDataName.c_str()))
		{
			pPubKernel->RemoveTimer(GUILD_TIMER_NAME, GetName(), guildPubDataName.c_str());
		}

		GuildTimerCallback_t currentTimer(now.nHour, now.nMinute);
		GuildTimerCallback_t *guildTimer = NULL;
		int deltaSeconds = 0;
		GuildTimerCallbackVector::iterator iter(m_GuildTimer.begin());
		// ѭ������
		LoopBeginCheck(cy);
		for ( ; iter != m_GuildTimer.end(); ++iter)
		{
			LoopDoCheck(cy);
			if (currentTimer < *iter)
			{
				guildTimer = &(*iter);
				break;
			}
		}
		if (guildTimer == NULL)
		{
			guildTimer = &m_GuildTimer[0];
			deltaSeconds += DAY_HOURS * HOUR_SECONDS;
		}
		deltaSeconds += ((*guildTimer - currentTimer) - now.nSecond);
		int deltaTimer = 0;
		if (deltaSeconds > HOUR_SECONDS)
		{
			deltaTimer = HOUR_SECONDS;
		}
		else if (deltaSeconds > TEN_MINUTE_SECOND)
		{
			deltaTimer = TEN_MINUTE_SECOND;
		}
		else if (deltaSeconds > MINUTE_SECOND)
		{
			deltaTimer = MINUTE_SECOND;
		}
		else
		{
			deltaTimer = deltaSeconds + 1;
		}

		pPubKernel->AddTimer(GUILD_TIMER_NAME, PubGuildManager::GuildTimerCallback, 
			GetName(), guildPubDataName.c_str(), deltaTimer*1000, 1);
	}
}


bool PubGuildManager::CallBackAllSub(std::function<bool(GuildBase*)> func)
{
	for (auto it : m_guildSubManage)
	{
		if (!func(it)){
			return false;
		}
	}
	return true;
}

bool PubGuildManager::CallBackAllGuild(std::function<bool(IPubData*)> func)
{
	
	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL){
		return false;
	}
	for (int row = 0; row < pGuildList->GetRows(); row++)
	{
		const wchar_t*guildName = pGuildList->QueryWideStr(row, GUILD_LIST_REC_COL_NAME);

		std::wstring guildDataName = guildName + m_ServerId;
		IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
		if (pGuildData != NULL)
		{
			if (!func(pGuildData))
			{
				return false;
			}
		}
	}

	return true;

}

//�̵�����
void PubGuildManager::ShopResetTimerCb(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name)
{

	IPubSpace* pPubSpace = pPubKernel->GetPubSpace(space_name);
	if (pPubSpace == NULL){
		return;
	}

	IPubData* pPubData = pPubSpace->GetPubData(data_name);
	if (pPubData == NULL){
		return;
	}

	IRecord* pGuildList = pPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL){
		return;
	}
	IRecord* shopBuyRec = pPubData->GetRecord(GUILD_SHOP_BUY_REC);
	if (shopBuyRec != NULL)
	{
		shopBuyRec->ClearRow();
	}
	IRecord* memberBuyRec = pPubData->GetRecord(GUILD_MEMBER_BUY_REC);
	if (memberBuyRec != NULL)
	{
		memberBuyRec->ClearRow();
	}
	
}




#define GUILD_DNLEVEL_TIMER 3600*1000
#define GUILD_DNLEVEL_TERM  3600*24*7


// ����������������Ϣ
int PubGuildManager::OnPublicMessage(IPubKernel* pPubKernel, int source_id, 
									 int scene_id, const IVarList& msg)
{
	if (m_pPubSpace == NULL || m_pGuildPubData == NULL)
	{
		return 0;
	}

	const wchar_t* guildName = msg.WideStrVal(1);
	int msgId = msg.IntVal(2);

	
	CallBackAllSub([pPubKernel, source_id, scene_id, &msg](GuildBase*pBase)->bool{
		if (pBase != NULL)
		{
			 pBase->OnPublicMessage(pPubKernel, source_id, scene_id, msg);
		}
		return true;
	});


	switch (msgId)
	{
		case SP_GUILD_MSG_BUY_ITEM:// �����򹫻��̵���Ʒ���۳����˹���ֵ
		{
			OnBuyShopItem(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		default:
			OnFilterMessage(pPubKernel, guildName, msgId, source_id, scene_id, msg);
			break;
	}

	return 0;
}
bool PubGuildManager::OnPublicCommand(IPubKernel* pPubKernel, int cmd_id, const IVarList& args)
{
	return true;
}
///////////////////////////////////////////////////////////////////////////
// ��������������Ϣ����
///////////////////////////////////////////////////////////////////////////
bool PubGuildManager::OnFilterMessage(IPubKernel* pPubKernel, const wchar_t* guildname, int msgid, int source_id, int scene_id, const IVarList& args)
{
	return false;
}



void PubGuildManager::AddGuildSub(GuildBase* pBase)
{
	if (pBase == NULL){
		return;
	}

	m_guildSubManage.push_back(pBase);
}

bool PubGuildManager::InitTimer(IPubKernel* pPubKernel)
{
	const char *path = pPubKernel->GetResourcePath();
	if (!LoadGuildTimer(path, m_GuildTimer))
	{
		return false;
	}
	// ����ʱ����������
	std::sort(m_GuildTimer.begin(), m_GuildTimer.end());
	// ���ö�ʱ���ص�����
	GuildTimerCallbackVector::iterator iter(m_GuildTimer.begin());
	// ѭ������
	LoopBeginCheck(aq);
	for (; iter != m_GuildTimer.end(); ++iter)
	{
		LoopDoCheck(aq);
		if (iter->m_CallbackName == GUILD_PUB_RESET)
		{
			iter->m_CallbackFunc = GuildPubReset;
		}
		else if (iter->m_CallbackName == GUILD_BUILDING_MAINTAIN)
		{
		//	iter->m_CallbackFunc = GuildBuildingManage::TimingMaintain;

		}

	}

	return true;
}

// �����򹫻���Ʒ���۳����˹��׶�
int PubGuildManager::OnBuyShopItem(IPubKernel* pPubKernel, const wchar_t* guildName, 
								   int sourceId, int sceneId, const IVarList& args)
{
	const char* playerNameUid = args.StringVal(3);
	const wchar_t* playerName = args.WideStrVal(4);
	int index = args.IntVal(5);
	const char* itemId = args.StringVal(6);
	int buyNum = args.IntVal(7);
	int addGuildCapital= args.IntVal(8);
	int shopBuyTotal = args.IntVal(9);
	int memberBuyTotal = args.IntVal(10);
	int consume = 0;
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
		return 0;
	}
	IRecord* pGuildMemberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pGuildMemberList == NULL)
	{
		SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
		return 0;
	}
	int memberRow = pGuildMemberList->FindWideStr(GUILD_MEMBER_REC_COL_NAME, playerName);
	if (memberRow < 0)
	{
		SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
		return 0;
	}
	int curDonate = GuildNumManage::m_pInstance->GetGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL);
	if (curDonate < consume)
	{
		SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
		return 0;
	}

	// ���ʣ����������Ƿ��㹻
	IRecord *shopBuyRecord = m_pGuildPubData->GetRecord(GUILD_SHOP_BUY_REC);
	if (shopBuyRecord == NULL)
	{
		SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
		return 0;
	}
	int shopBuyCount = 0;
	std::wstring shopBuyId = guildDataName + StringUtil::StringAsWideStr(itemId) + L"_" + StringUtil::IntAsWideStr(index);
	int shopBuyRow = shopBuyRecord->FindWideStr(GUILD_SHOP_BUY_REC_COL_ITEMID, shopBuyId.c_str());
	if (shopBuyRow >= 0)
	{
		shopBuyCount = shopBuyRecord->QueryInt(shopBuyRow, GUILD_SHOP_BUY_REC_COL_BUY_COUNT);
	}
	if ((shopBuyTotal - shopBuyCount) < buyNum)
	{
		SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
		return 0;
	}

	// ����Ա�������Ƿ��㹻
	IRecord *memberBuyRecord = m_pGuildPubData->GetRecord(GUILD_MEMBER_BUY_REC);
	if (memberBuyRecord == NULL)
	{
		SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
		return 0;
	}
	int memberBuyCount = 0;
	std::string memberBuyId = std::string(playerNameUid) + std::string(itemId) + "_" + StringUtil::IntAsString(index);
	int memberBuyRow = memberBuyRecord->FindString(GUILD_MEMBER_BUY_REC_COL_UID_ITEMID, memberBuyId.c_str());
	if (memberBuyRow >= 0)
	{
		memberBuyCount = memberBuyRecord->QueryInt(memberBuyRow, GUILD_MEMBER_BUY_REC_COL_BUY_COUNT);
	}
	if ((memberBuyTotal - memberBuyCount) < buyNum)
	{
		SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
		return 0;
	}

	// ���ù�����
	if (shopBuyRow >= 0)
	{
		if (!shopBuyRecord->SetInt(shopBuyRow, GUILD_SHOP_BUY_REC_COL_BUY_COUNT, shopBuyCount+buyNum))
		{
			SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
			return 0;
		}
	}
	else 
	{
		if (shopBuyRecord->AddRowValue(-1, CVarList() << shopBuyId << buyNum) < 0)
		{
			SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
			return 0;
		}
	}

	if (memberBuyRow >= 0)
	{
		if (!memberBuyRecord->SetInt(memberBuyRow, GUILD_MEMBER_BUY_REC_COL_BUY_COUNT, memberBuyCount+buyNum))
		{
			shopBuyRecord->SetInt(shopBuyRow, GUILD_SHOP_BUY_REC_COL_BUY_COUNT, shopBuyCount);
			SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
			return 0;
		}
	}
	else 
	{
		if (memberBuyRecord->AddRowValue(-1, CVarList() << memberBuyId << buyNum) < 0)
		{
			shopBuyRecord->SetInt(shopBuyRow, GUILD_SHOP_BUY_REC_COL_BUY_COUNT, shopBuyCount);
			SendBuyItemResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, index, itemId, buyNum, consume);
			return 0;
		}
	}

	if (addGuildCapital != 0)
	{
		GuildNumManage::m_pInstance->AddGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL, addGuildCapital, GUILD_NUM_CHANGE::GUILD_NUM_CHANGE_SHOP);
	}

	// ���ͽ��
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_BUY_ITEM << GUILD_MSG_REQ_SUCC 
		<< playerName << index << itemId << buyNum << consume << (shopBuyTotal-shopBuyCount-buyNum) 
		<< (memberBuyTotal-memberBuyCount-buyNum) << (curDonate-consume);
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);

	return 0;
}
// ֪ͨ������Ʒ�۳����˹���ֵ���
void PubGuildManager::SendBuyItemResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
											const wchar_t* guildName, const wchar_t* playerName,
											int result, int index, const char* itemId, int itemNum, int consume)
{
// 	CVarList msg;
// 	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_BUY_ITEM << result
// 		<< playerName << index << itemId << itemNum << consume << 0 << 0;
// 	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
}
//��ʱ�������ص�
int PubGuildManager::GuildTimerCallback(IPubKernel* pPubKernel, const char* space_name,
	const wchar_t* data_name, int time)
{

	port_date_time_t now;
	Port_GetLocalTime(&now);
	GuildTimerCallback_t currentTimer(now.nHour, now.nMinute);
	//int deltaSeconds = 0;
	GuildTimerCallbackVector::const_iterator iter(PubGuildManager::m_pInstance->m_GuildTimer.begin());
	// ѭ������
	LoopBeginCheck(aw);
	for (; iter != PubGuildManager::m_pInstance->m_GuildTimer.end(); ++iter)
	{
		LoopDoCheck(aw);
		if (currentTimer == *iter && iter->m_CallbackFunc)
		{
			iter->m_CallbackFunc(pPubKernel, space_name, data_name);
		}
	}

	PubGuildManager::m_pInstance->ResetGuildTimer(pPubKernel, now);
	return 0;
}

void PubGuildManager::GuildPubReset(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name)
{
	PubGuildManager::ShopResetTimerCb(pPubKernel, space_name,data_name);
	GuildBaseManager::FireNumResetTimerCb(pPubKernel, space_name, data_name);
	GuildBaseManager::m_pInstance->OnZeroClock(pPubKernel);
	//GuildBaseManager::DancingGirlReset(pPubKernel, space_name, data_name);
	//GuildBaseManager::AutoImpeachTimer(pPubKernel, space_name, data_name);
	GuildNumManage::m_pInstance->GuildNumDailyReset(pPubKernel);
}





