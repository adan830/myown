//---------------------------------------------------------
//�ļ���:       RewardModule.h
//��  ��:       ���Ž��������
//˵  ��:       
//          
//��������:      2015��03��30��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#include "RewardModule.h"

#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/PubDefine.h"


#include "FsGame/CommonModule/ContainerModule.h"
#include "FsGame/SystemFunctionModule/CapitalModule.h"
#ifndef FSROOMLOGIC_EXPORTS
#include "FsGame/SocialSystemModule/SystemMailModule.h"
#endif
#include "FsGame/CommonModule/LevelModule.h"
#include "FsGame/CommonModule/LogModule.h"
//#include "FsGame/ItemModule/EquipmentModule.h"
#include "FsGame/ItemModule/ToolItem/ToolItemModule.h"

#include "utils/util_func.h"
#include "public/VarList.h"
#include "utils/XmlFile.h"
#include "utils/string_util.h"
#include "FsGame/CommonModule/LuaExtModule.h"


#include <vector>
#include "utils/custom_func.h"
#include "../Define/ToolBoxSysInfoDefine.h"
#include "CommonModule/FunctionEventModule.h"
//#include "Define/GuildDefine.h"

// ���ͽ���:
int nx_reward(void* state)
{
//     IKernel* pKernel = LuaExtModule::GetKernel(state);
// 
//     if (NULL == RewardModule::m_pRewardInstance)
//     {
//         return 0;
//     }
// 
//     int param_cnt = pKernel->LuaGetArgCount(state);
//     
//     // ��������
// 	RewardModule::AwardEx award;
// 	award.srcFunctionId = FUNCTION_EVENT_ID_GM_COMMAND;
//     award.name = "player_greeting";
//     award.exp = 10;
// 	award.capitals["CapitalCopper"] = 99;
// 	award.capitals["CapitalGold"] = 99;
// 	award.capitals["CapitalSmelt"] = 99;
// 	award.capitals["CapitalPublicOffice"] = 99;
// 
// 	RewardModule::ParseAddOneItem(award.items, "Equip10710032", 3);
// 	RewardModule::ParseAddOneItem(award.items, "BianStone2082014", 2);
//     
//     if (param_cnt == 1)
//     {
//         // ����������
//         CHECK_ARG_OBJECT(state, nx_reward, 1);
// 
//         PERSISTID player = pKernel->LuaToObject(state, 1);    
//         RewardModule::m_pRewardInstance->RewardPlayer(pKernel, player, &award);
//     }
//     else if (param_cnt == 2)
//     {
//         // ����������
//         CHECK_ARG_OBJECT(state, nx_reward, 1);
//         CHECK_ARG_WIDESTR(state, nx_reward, 2);
// 
//         const wchar_t * role_name = pKernel->LuaToWideStr(state, 2);
//         RewardModule::m_pRewardInstance->RewardByRoleName(pKernel, role_name, &award);
//     }
    
    return 1;
}

RewardModule* RewardModule::m_pRewardInstance = NULL;
ContainerModule* RewardModule::m_pContainerModule = NULL;
CapitalModule* RewardModule::m_pCapitalModule = NULL;
#ifndef FSROOMLOGIC_EXPORTS
SystemMailModule* RewardModule::m_pMailModule = NULL;
#endif
LevelModule* RewardModule::m_pLevelModule = NULL;

bool RewardModule::Init(IKernel* pKernel)
{
	m_pRewardInstance = this;
	
    m_pContainerModule = (ContainerModule*)pKernel->GetLogicModule("ContainerModule");
    m_pCapitalModule   = (CapitalModule*)pKernel->GetLogicModule("CapitalModule");
	m_pLevelModule      = (LevelModule*)pKernel->GetLogicModule("LevelModule");
#ifndef FSROOMLOGIC_EXPORTS
    m_pMailModule      = (SystemMailModule*)pKernel->GetLogicModule("SystemMailModule");
	Assert(m_pContainerModule != NULL && m_pCapitalModule != NULL
		&& m_pMailModule != NULL && m_pLevelModule != NULL);
#else
	Assert(m_pContainerModule != NULL && m_pCapitalModule != NULL
		&& m_pLevelModule != NULL);
#endif
    

	LoadAllRewardConfig(pKernel);

    DECL_LUA_EXT(nx_reward);
	return true;
}

bool RewardModule::Shut(IKernel* pKernel)
{
	return true;
}

bool RewardModule::TryRewardPlayer(IKernel *pKernel, const PERSISTID &player, int nAwardId, int operation_on_full)
{
	// ֱ�ӷ��ʼ�
	if (REWARD_MAIL == operation_on_full)
	{
		return true;
	}

	const AwardEx *award = m_pRewardInstance->QueryAwardById(nAwardId);
	if (NULL == award)
	{
		return false;
	}
	// ���������ܷ����
	int nBindState = FunctionEventModule::GetItemBindState(award->srcFunctionId);
	// �����Ų���,ֱ��ʧ��
	if (!m_pRewardInstance->CanPutInToolBox(pKernel, player, award->items, nBindState) && operation_on_full == REWARD_FAILED_ON_FULL)
	{
		return false;
	}

	return true;
}

// �������������
// �� @operation_on_full = REWARD_MAIL_ON_FULL ��ʱ��, ��ͨ��ʹ��Ԥ������ʼ��������.
// ����, @award.name ���ʼ�ģ�������, ����ʼ�ģ�����в�������ͨ��@mail_param ����
bool RewardModule::RewardPlayerById(IKernel *pKernel, const PERSISTID &player, 
	int nRewardId, int operation_on_full, const IVarList &mail_param)
{
	// ֱ�ӷ��ʼ�
	IGameObj* pDcObj = pKernel->GetGameObj(player);
	const AwardEx* award = m_pRewardInstance->QueryAwardById(nRewardId);
	if (NULL == pDcObj || NULL == award)
	{
		return false;
	}

	return RewardPlayer(pKernel, player, award, mail_param, operation_on_full);
}


// ֱ�ӽ�������ɫ
// �� @operation_on_full = REWARD_MAIL_ON_FULL ��ʱ��, ��ͨ��ʹ��Ԥ������ʼ��������.
// ����, @award.name ���ʼ�ģ�������, ����ʼ�ģ�����в�������ͨ��@mail_param ����
// bool RewardModule::RewardByRoleName(IKernel *pKernel, const wchar_t *role_name, int nAwardId, const IVarList &mail_param)
// {   
// 	// ֱ���ʼ�����
// 	if (operation_on_full == REWARD_MAIL)
// 	{
// 		// �ʼ�����
// 		return InnerRewardViaMail(pKernel, role_name, award, mail_param);
// 	}
// 
// 	// �������
// 	PERSISTID player = pKernel->FindPlayer(role_name);
// 	if (pKernel->Exists(player))
// 	{
// 		// ����ڵ�ǰ����, ֱ�ӽ��������
// 		RewardPlayer(pKernel, player, award, mail_param, operation_on_full);
// 	}
// 	else
// 	{
// 		// ��Ҳ��ٵ�ǰ����, ͨ���ʼ����ͽ���
// 		InnerRewardViaMail(pKernel, role_name, award, mail_param);
// 	}
// 	
// 	return true;
// }

// ͨ���ʼ������������
bool RewardModule::RewardViaMail(IKernel *pKernel, const wchar_t *role_name, int nAwardId, const IVarList &mail_param, const wchar_t *sender_name /*= L""*/)
{
	const AwardEx* pAwardData = m_pRewardInstance->QueryAwardById(nAwardId);
	if (NULL == pAwardData)
	{
		return false;
	}

	return m_pRewardInstance->InnerRewardViaMail(pKernel, role_name, pAwardData, mail_param, sender_name);
}

// ��������Ʒ������Ϊ��������
// ��ʽ��  "CapitalCopper:10,CapitalGold:10,CapitalSmelt:10,CapitalPublicOffice:10,CapitalBattleSoul:10,CapitalBlackShop:10,CapitalGuild:10"
bool RewardModule::GenerateAwardData(const std::string& strAward, RewardModule::AwardEx& award)
{
	// ��ȫ���
	if (strAward.empty())
	{
		return false;
	}

	// �ָ��ַ���
	CVarList result;
	util_split_string(result, strAward, ",");

	// �ָ�����ַ�������
	int iCount = (int)result.GetCount();
	if (iCount == 0)
	{
		return false;
	}

	// ѭ������
	LoopBeginCheck(fh);
	for (int i = 0; i < iCount; ++i)
	{
		LoopDoCheck(fh);

		// ȡ�ָ�����Ӵ�
		std::string strTmp = result.StringVal(i);

		// �ٴν��зָ� ��Ʒ���� ����
		CVarList info;
		util_split_string(info, strTmp, ":");
		if (info.GetCount() < 2)
		{
			continue;
		}

		// ����
		std::string name = info.StringVal(0);

		int nCapitalType = m_pCapitalModule->GetCapitalType(name);
		// ˵���ǻ���
		if (nCapitalType >= CAPITAL_MIN_TYPE)
		{
			ParseAddOneAwardData(strTmp.c_str(), CAPITAL_AWARD, award);
		}
		else
		{
			if (StringUtil::CharIsEqual(name.c_str(), STR_LEVEL_EXP))
			{
				// ����
				ParseAddOneAwardData(strTmp.c_str(), EXP_AWARD, award);
			}
			else
			{
				// ��Ʒ
				ParseAddOneAwardData(strTmp.c_str(), ITEM_AWARD, award);
			}
		}
	}

	return true;
}

bool RewardModule::RewardPlayer(IKernel *pKernel, const PERSISTID &player, const AwardEx *award, const IVarList &mail_param, int operation_on_full /*= REWARD_MAIL_ON_FULL*/)
{
	// ֱ�ӷ��ʼ�
	IGameObj* pDcObj = pKernel->GetGameObj(player);
	Assert(pDcObj != NULL);

	if (REWARD_MAIL == operation_on_full)
	{
		// ͨ���ʼ��������
		return m_pRewardInstance->InnerRewardViaMail(pKernel, pDcObj->QueryWideStr("Name"), award, mail_param);
	}

	// ���������ܷ����
	int nBindState = FunctionEventModule::GetItemBindState(award->srcFunctionId);
	if (!m_pRewardInstance->CanPutInToolBox(pKernel, player, award->items, nBindState))
	{
		// �����Ų���,ֱ��ʧ��
		if (operation_on_full == REWARD_FAILED_ON_FULL)
		{
			// �����ռ䲻��
			::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_7004, CVarList());
			return false;
		}
		// lihl: �޷����뱳�� ����ϵͳ��ʾ
		// �����ռ䲻��
		::CustomSysInfo(pKernel, player, SYSTEM_INFO_ID_7010, CVarList());

		// �����޷�����, ֻ��ͨ���ʼ��������
		m_pRewardInstance->InnerRewardViaMail(pKernel, pDcObj->QueryWideStr("Name"), award, mail_param);
		// �Ѿ���Ӹ����
		if (!award->exps.empty())
		{
			m_pRewardInstance->AddExp(pKernel, player, award->exps, award->srcFunctionId);
		}
	}
	else
	{
		// ֱ�ӽ��������
		m_pRewardInstance->InnerRewardDirectly(pKernel, player, award);
	}

	return true;
}

// ͨ���ʼ����ͽ���
bool RewardModule::InnerRewardViaMail(IKernel *pKernel, const wchar_t *role_name, const AwardEx* award, const IVarList &mail_param, const wchar_t *sender_name)
{
#ifndef FSROOMLOGIC_EXPORTS
    // ���ݽ��������֣���ȡ�ʼ�ģ��
    const ConfigMailItem& mail_template = m_pMailModule->GetConfigMail(pKernel, award->name);

    // �ʼ��еĸ���
    bool has_attachment = false;
    std::string attachment; // ���������ʽ�͵���

    // �ʽ�
    if (award->capitals.size() > 0)
    {
        has_attachment = true;
		attachment.append(award->strCapitals);
		attachment.append(";");
    }
	else
	{
		attachment.append(";");
	}
    
    if (award->items.size() > 0)
    {
        has_attachment = true;
		attachment.append(award->strItems);
		attachment.append(";");
    }
    
    if (!has_attachment)
    {
        attachment = mail_template.items;
    }

    // �ʼ�������
    std::wstring mail_content = mail_template.mail_content;
    if (mail_param.GetCount() > 0)
    {
        mail_content = SystemMailModule::ComposeMailContent(pKernel, mail_template.mail_content.c_str(), 
            mail_param);
    }
    if (StringUtil::CharIsNull(sender_name))
    {
        sender_name = mail_template.sender.c_str();
    }

    m_pMailModule->SendMailToPlayer(pKernel, sender_name,
									role_name,
									mail_template.mail_title.c_str(),
									mail_content.c_str(),
									attachment, (EmFunctionEventId)award->srcFunctionId);

	// �ʼ���־��¼
	MailLog mailLotLog;
	mailLotLog.name = role_name;
	mailLotLog.templateType = award->name;
	mailLotLog.itemParams = award->strItems;
	mailLotLog.capitalParams = award->strCapitals;

	LogModule::m_pLogModule->SaveMailAwardLog(pKernel, mailLotLog);
#endif
   return true;
}

// ֱ�ӽ��������
bool RewardModule::InnerRewardDirectly(IKernel *pKernel, const PERSISTID &player, const AwardEx *award)
{
    // �ѵ��߷��뱳����
	int nBindState = FunctionEventModule::GetItemBindState(award->srcFunctionId);
	PutInToolBox( pKernel, player, award->items, award->srcFunctionId, nBindState);

    // ���ʽ�Ӹ����
    AddCapital(pKernel, player, award->capitals, award->srcFunctionId);
    
    // �Ѿ���Ӹ����
	AddExp(pKernel, player, award->exps, award->srcFunctionId);
    return true;
}

// ����һ��� nAwardType (��ӦAwardType)
void RewardModule::ParseAddOneAwardData(const char* strAward, int nAwardType, RewardModule::AwardEx& award)
{
	// ��ʽ: type:num
	CVarList single_item;
	util_split_string(single_item, strAward, ":");
	if (single_item.GetCount() != 2)
	{
		return;
	}

	AwardData data;
	data.strType = single_item.StringVal(0);
	data.nAddVal = single_item.IntVal(1);

	if (nAwardType == CAPITAL_AWARD)
	{
		award.capitals.push_back(data);
	}
	else if (nAwardType == ITEM_AWARD)
	{
		award.items.push_back(data);
	}
	else
	{
		award.exps.push_back(data);
	}
}

bool RewardModule::ItemsToString(const AwardVec& items_list, std::string& strItems, const char* pszSp1/* = ","*/, const char* pszSp2/* = ":"*/)
{
	for (AwardVec::const_iterator itr = items_list.begin(); itr != items_list.end(); ++itr )
	{
		if (strItems.size() > 0)
		{
			strItems.append(pszSp1);
		}
		strItems.append(itr->strType);
		strItems.append(pszSp2);
		strItems.append( StringUtil::IntAsString(itr->nAddVal) );
	}
	return true;
}

const RewardModule::AwardEx* RewardModule::QueryAwardById(int nAwardId)
{
	AwardMap::iterator iter = m_mapAllAwardData.find(nAwardId);
	if (iter == m_mapAllAwardData.end())
	{
		return NULL;
	}

	return iter->second;
}

// �������ֱ�ӼӸ����
bool RewardModule::AddExp(IKernel *pKernel, const PERSISTID &player, const AwardVec& exp_list, int exp_from)
{
	int nSize = (int)exp_list.size();
	LoopBeginCheck(u);
	for (int i = 0; i < nSize;++i)
	{
		LoopDoCheck(u);
		const AwardData& exp = exp_list[i];
		if (StringUtil::CharIsEqual(exp.strType.c_str(), STR_LEVEL_EXP))
		{
			m_pLevelModule->AddExp(pKernel, player, exp_from, exp.nAddVal);
		}
	}
    return true;
}

// �ʽ����ֱ�ӼӸ����, @capital �ĸ�ʽ��capital_name:����,capital_name:����,
bool RewardModule::AddCapital(IKernel *pKernel, const PERSISTID &player,
					const AwardVec &capital_list, EmFunctionEventId capitalEventID)
{    
	if (capital_list.empty())
    {
        return true;
    }
        
    LoopBeginCheck(a)
	for (AwardVec::const_iterator itr = capital_list.begin(); itr != capital_list.end(); ++itr)
    {
        LoopDoCheck(a)

		int capital_type = m_pCapitalModule->GetCapitalType( itr->strType.c_str() );
		m_pCapitalModule->IncCapital(pKernel, player, capital_type, itr->nAddVal, capitalEventID);
    }
    
    return true;
}

// ���Ӱ�ṱ�����ֵֻ������¼
// bool RewardModule::AddGuildDevote(IKernel *pKernel, const PERSISTID &player, int64_t devoteValue)
// {
// 	IGameObj* pSelfObj = pKernel->GetGameObj(player);
// 	if (pSelfObj == NULL)
// 	{
// 		return false;
// 	}
// 
// 	// �������
// 	const wchar_t* strGuildName = pSelfObj->QueryWideStr("GuildName");
// 	if (StringUtil::CharIsNull(strGuildName))
// 	{
// 		return false;
// 	}
// #ifndef FSROOMLOGIC_EXPORTS
// 	CVarList pubMsg;
// 	pubMsg << PUBSPACE_GUILD 
// 		<< strGuildName 
// 		<< SP_GUILD_ADD_GUID_DEVOTE_MAX
// 		<< pSelfObj->QueryWideStr("Name") 
// 		<< (int)devoteValue;
// 	pKernel->SendPublicMessage(pubMsg);
// #endif
// 	return true;
// }

void RewardModule::ShowItem(IKernel*pKernel, const PERSISTID &player, const std::string& awardStr)
{

	if (StringUtil::CharIsNull(awardStr.c_str()))
	{
		return;
	}
	
	std::map<std::string, int> itemList;
	// ���ͻ��˷��͵Ĳ���
	CVarList argsItems;
	CVarList res1;
	StringUtil::SplitString(res1, awardStr, ",");
	for (size_t i = 0; i < res1.GetCount();i++)
	{
		CVarList res2;
		StringUtil::SplitString(res2, res1.StringVal(i), ":");

		const char* itemID = res2.StringVal(0);
		int itemNum = res2.IntVal(1);
		if (StringUtil::CharIsNull(itemID) ||itemNum == 0){
			continue;
		}
		if (!ToolItemModule::IsCanWearItem(pKernel, itemID))
		{
			itemList[itemID] += itemNum;
		}
		else
		{
			
			argsItems << itemID << itemNum;
		}
	}

	for (auto it : itemList){
		
		argsItems << it.first << it.second;
	};

	if (argsItems.GetCount() == 0)
	{
		return;
	}

	CVarList args;
	args << SERVER_CUSTOMMSG_GET_ITEM
		<< S2C_SHOW_ITEM_LIST
		<<(int) argsItems.GetCount()
		<< argsItems;
	pKernel->Custom(player, args);
}

// ��ȡȫ����������
bool RewardModule::LoadAllRewardConfig(IKernel *pKernel)
{
	std::string strAwardPath = pKernel->GetResourcePath();
	strAwardPath += "ini/SystemFunction/Reward/";

	std::string strAwardFilepath = strAwardPath + "PathConfig.xml";
	CXmlFile xml(strAwardFilepath.c_str());

	if (!xml.LoadFromFile())
	{
		extend_warning(LOG_WARNING, "[RewardModule::LoadRewardConfig PathConfig.xml] failed");
		return false;
	}

	// ��ȡ·������
	int nSecCount = (int)xml.GetSectionCount();
	LoopBeginCheck(c);
	for (int i = 0; i < nSecCount;++i)
	{
		LoopDoCheck(c);
		const char *section = xml.GetSectionByIndex(i);

		std::string strXmlName = xml.ReadString(section, "Path", "");
		std::string strPath = strAwardPath + strXmlName;

		LoadRewardConfig(pKernel, strPath.c_str());
	}

	return true;
}

// ��ȡ����
bool RewardModule::LoadRewardConfig(IKernel *pKernel, const char* strPath)
{
	CXmlFile xml(strPath);

	if (!xml.LoadFromFile())
	{
		extend_warning(LOG_ERROR, "[RewardModule::LoadRewardConfig %s] failed", strPath);
		return false;
	}

	// xml��ȡ
	LoopBeginCheck(b);
	for (int i = 0; i < (int)xml.GetSectionCount(); ++i)
	{
		LoopDoCheck(b);
		const char *section = xml.GetSectionByIndex(i);
		if (StringUtil::CharIsNull(section))
		{
			continue;
		}

		int id = StringUtil::StringAsInt(section);

		// һ��������Ϣ
		AwardEx* pData = NEW AwardEx; 
		if (NULL == pData)
		{
			return false;
		}

		pData->name = xml.ReadString(section, "MailName", "");

		pData->strCapitals = xml.ReadString(section, "Capitals", "");
		ParseAddAwardData(pData->strCapitals.c_str(), pData->capitals);

		pData->strItems = xml.ReadString(section, "Items", "");
		ParseAddAwardData(pData->strItems.c_str(), pData->items);

		const char* strExp = xml.ReadString(section, "AddExp", "");
		ParseAddAwardData(strExp, pData->exps);

		pData->srcFunctionId = (EmFunctionEventId)xml.ReadInteger(section, "FunctionEventId", -1);

		AwardMap::iterator iter = m_mapAllAwardData.find(id);
		if (iter != m_mapAllAwardData.end())
		{
			extend_warning(LOG_WARNING, "LoadRewardConfig %s, exist repeat id %d", strPath, id);
			continue;
		}

		m_mapAllAwardData.insert(std::make_pair(id, pData));
	}

	return true;
}

// �Ƿ���Ա��浽������
bool RewardModule::CanPutInToolBox(IKernel *pKernel, const PERSISTID &player, const AwardVec &items_list, int nBindState)
{      
	if (items_list.size() == 0)
    {
        return true;
    }
        
    // ��ȡ����
    PERSISTID tool_box = pKernel->GetChild(player, ITEM_BOX_NAME_WSTR);

    if (!pKernel->Exists(tool_box))
    {
        return false;
    }
    
    VectorBoxTestItem try_items;
    
    int index = -1;
    
    LoopBeginCheck(a)
	for (AwardVec::const_iterator itr = items_list.begin(); itr != items_list.end(); ++itr)
    {
        LoopDoCheck(a)
        
        SBoxTestItem temp_item;
		temp_item.config_id = itr->strType;
		temp_item.amount = itr->nAddVal;
		temp_item.bind_status = nBindState;
        try_items.push_back(temp_item);
    }
    
	if (m_pContainerModule->TryAddItems(pKernel, tool_box, &try_items))
	{
		return true;
	}

	// ������ʱ����
// 	if (bUseTempBag)
// 	{
// 		// ��ȡ����
// 		PERSISTID temp_box = pKernel->GetChild(player, TEMPORARY_BAG_BOX_NAME_WSTR);
// 		if (!pKernel->Exists(temp_box))
// 		{
// 			return false;
// 		}
// 
// 		if (m_pContainerModule->TryAddItems(pKernel, temp_box, &try_items))
// 		{
// 			return true;
// 		}
// 	}

	return false;
}

// ���뱳����
bool RewardModule::PutInToolBox(IKernel *pKernel, const PERSISTID &player, const AwardVec& item_list, EmFunctionEventId itemEventID, int nBindState)
{
	if (item_list.size() == 0)
    {
        return true;
    }

    VectorBoxTestItem try_items; 
    LoopBeginCheck(a)
	for (AwardVec::const_iterator itr = item_list.begin(); itr != item_list.end(); ++itr)
    {
        LoopDoCheck(a)
        
        SBoxTestItem temp_item;
		temp_item.config_id = itr->strType;
		temp_item.amount = itr->nAddVal;
		temp_item.bind_status = nBindState;

        try_items.push_back(temp_item);
    }

// 	if (bUseTempBag)
// 	{
// 		return ContainerModule::PlaceItemsToBagOrTemp(pKernel, player, &try_items, itemEventID, true);
// 	}

	// ��ȡ����
	PERSISTID tool_box = pKernel->GetChild(player, ITEM_BOX_NAME_WSTR);

	if (!pKernel->Exists(tool_box))
	{
		return false;
	}
	return m_pContainerModule->PlaceItems(pKernel, tool_box, &try_items, itemEventID, true); 
}

// ��������Ʒ������Ϊ��������
// ��ʽ��  "CapitalCopper:10,CapitalGold:10,CapitalSmelt:10,CapitalPublicOffice:10,CapitalBattleSoul:10,CapitalBlackShop:10,CapitalGuild:10"
void RewardModule::ParseAddAwardData(const char* strAwardData, AwardVec& vecAward)
{
	// name:amount,...name:amount
	CVarList temp_award_list;
	util_split_string(temp_award_list, strAwardData, ",");

	int temp_count = (int)(temp_award_list.GetCount());

	vecAward.reserve(temp_count);
	LoopBeginCheck(a)
	for (int i = 0; i < temp_count; ++i)
	{
		LoopDoCheck(a)

		const char *temp_capital = temp_award_list.StringVal(i);
		CVarList single_award;
		util_split_string(single_award, temp_capital, ":");
		if (single_award.GetCount() != 2)
		{
			continue;
		}

		AwardData data;
		data.strType = single_award.StringVal(0);
		data.nAddVal = single_award.IntVal(1);

		vecAward.push_back(data);
	}
}

// ����Ʒ��������ְҵ����Ʒ�ų�
// ��ʽ��  "ConfigID:����,ConfigID:����"
// bool RewardModule::FilterAwardByJob(IKernel* pKernel, const PERSISTID& self, AwardVec& items)
// {
// 	
// 	// ��Ҷ���
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (pSelfObj == NULL)
// 	{
// 		return false;
// 	}
// 
// 	//���ְҵ
// 	int job = pSelfObj->QueryInt("Job");
// 	return FilterAwardByJob(pKernel, job, items);
// }
// 
// bool RewardModule::FilterAwardByJob(IKernel* pKernel, int job, AwardVec& items)
// {
// 	// ��ȫ���
// 	if (EquipmentModule::m_pEquipmentModule == NULL)
// 	{
// 		return false;
// 	}
// 
// 	// ѭ������
// 	LoopBeginCheck(fh);
// 	for (AwardVec::iterator itr = items.begin(), next = itr; itr != items.end(); itr = next)
// 	{
// 		++next;
// 		LoopDoCheck(fh);
// 		// ����
// 		const char* pszItemId = itr->strType.c_str();
// 		std::string itemScript = pKernel->GetConfigProperty(pszItemId, "Script");
// 
// 		//��װ��
// 		if (itemScript == "Equipment")
// 		{
// 			// ����Ƿ��Ǵ�ְҵ��װ��
// 			int jobLimit = StringUtil::StringAsInt(pKernel->GetConfigProperty(pszItemId, "ClassLimit"));
// 			if (!EquipmentModule::m_pEquipmentModule->EquipIsSameJob(jobLimit, job))
// 			{
// 				next = items.erase(itr);
// 			}
// 		}
// 	}
// 
// 	return true;
// }