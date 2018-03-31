//--------------------------------------------------------------------
// �ļ���:		GuildCustomHook.cpp
// ��  ��:		����ϵͳ-����ͻ�����Ϣ
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#include <time.h>
#include "GuildModule.h"
#include "../Define/GuildMsgDefine.h"
#include "../Define/PubDefine.h"
#include "../Define/ServerCustomDefine.h"
#include "../Define/GameDefine.h"
#include "../Define/CommandDefine.h"
#include "utils/util_func.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"
#include "FsGame/SystemFunctionModule/CapitalModule.h"
#include "FsGame/CommonModule/VirtualRecModule.h"
#include "FsGame/CommonModule/ContainerModule.h"
#include "FsGame/CommonModule/SwitchManagerModule.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
#include "FsGame/Define/StaticDataDefine.h"
#include <algorithm>
#include "Server/LoopCheck.h"

#include "FsGame/CommonModule/EnvirValueModule.h"
#include "utils/EnvirDefine.h"
#include "CommonModule/LogModule.h"
#include "FriendModule.h"
#include "SkillModule/SkillUpgradeModule.h"
#include "SkillModule/SkillToSprite.h"
#include "Define/Skilldefine.h"
#include "Define/Fields.h"
#include "CommonModule/FunctionEventModule.h"
#include "ItemModule/ToolItem/ToolItemModule.h"
#include "SystemFunctionModule/RewardModule.h"
#include "Define/SnsDefine.h"
#include "CommonModule/ActionMutex.h"
#include "Define/SystemInfo_Item.h"
#include "SystemFunctionModule/ActivateFunctionModule.h"

// ���Ͱ���Ա�б�һ�η��ͳ�Ա����
const int SEND_MEMBER_COUNT = 10;

// �ͻ�����Ϣ����
int GuildModule::OnCustomMessage(IKernel* pKernel, const PERSISTID& self,
	const PERSISTID& sender, const IVarList& args)
{
	if (m_pGuildModule == NULL)
	{
		return 0;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	int subMsg = args.IntVal(1);
	switch (subMsg)
	{
		case CS_GUILD_CREATE:// ��������
		{
			m_pGuildModule->OnCustomCreateGuild(pKernel, self, args);
		}
		break;
		case CS_GUILD_GUILD_LIST:// ���󹫻��б�
		{
			m_pGuildModule->OnCustomGetGuildList(pKernel, self, args);
		}
		break;
		case CS_GUILD_SEARCH:// ��������
		{
			m_pGuildModule->OnCustomFindGuild(pKernel, self, args);
		}
		break;
		case CS_GUILD_APPLY_JOIN:// ������빫��
		{
			if (m_pGuildModule->OnCustomApplyJoinGuild(pKernel, self, args) == 0)
			{
				// ֪ͨ����ʧ��
				CVarList msg;
				msg << 0
					<< 0
					<< args.WideStrVal(2)
					<< GUILD_MSG_REQ_FAIL;
				m_pGuildModule->OnCommandApplyJoinGuildResult(pKernel, self, msg);
			}
		}
		break;
		case CS_GUILD_ONE_KEY_APPLY_JOIN://һ��������빫��
		{
			m_pGuildModule->OnCustomOneKeyApplyJoinGuild(pKernel, self);


		}break;
		case CS_GUILD_CANCEL_APPLY_JOIN:// ����ȡ�����빫������
		{
			m_pGuildModule->OnCustomCancelApplyJoinGuild(pKernel, self, args);
		}
		break;
		case CS_GUILD_ACCEPT_APPLY:// ���ܼ��빫�������
		{
			// У�鹫���ڼ��Ƿ��������
			/*if (!m_pGuildModule->CheckGuildActivity(pKernel, self))
			{
				return 0;
			}
*/
			if (m_pGuildModule->OnCustomAcceptJoinGuild(pKernel, self, args) == 0)
			{
				CVarList msg;
				msg << 0
					<< 0
					<< GUILD_MSG_REQ_FAIL
					<< pSelfObj->QueryWideStr("GuildName")
					<< 0
					<< args.WideStrVal(2);

				m_pGuildModule->OnCommandAcceptResult(pKernel, self, msg);
			}
		}
		break;
		case CS_GUILD_REFUSE_APPLY:// �ܾ����빫�������
		{
			if (m_pGuildModule->OnCustomRefuseJoinGuild(pKernel, self, args) == 0)
			{
				CVarList msg;
				msg << 0
					<< 0
					<< GUILD_MSG_REQ_FAIL
					<< args.WideStrVal(2);

				m_pGuildModule->OnCommandRefuseJoin(pKernel, self, msg);
			}
		}
		break;
		case CS_GUILD_GUILD_INFO:// ���󹫻���Ϣ
		{
			m_pGuildModule->OnCustomGetGuildInfo(pKernel, self);
		}
		break;
		case CS_GUILD_MEMBER_LIST:// �����Ա�б�
		{
			m_pGuildModule->OnCustomGetGuildMemberList(pKernel, self, args);
		}
		break;
		case CS_GUILD_APPLY_LIST:// �����������б�
		{
			m_pGuildModule->OnCustomGetGuildApplyList(pKernel, self, args);
		}
		break;
		case CS_GUILD_NOTICE:// �����޸Ĺ���
		{
			m_pGuildModule->OnCustomChangeNotice(pKernel, self, args);
		}
		break;
		case CS_GUILD_DECLARATION:// �����޸�����
		{
			m_pGuildModule->OnCustomChangeDeclaration(pKernel, self, args);
		}
		break;
		case CS_GUILD_QUIT:// �����˳�����
		{
			// У�鹫���ڼ��Ƿ��������
			if (!m_pGuildModule->CheckGuildActivity(pKernel, self))
			{
				return 0;
			}

			//�����в���ִ�иò���
// 			if (GuildImpeach::IsImpeachIng(pKernel, self) && pSelfObj->QueryInt("GuildPosition") == GUILD_POSITION_CAPTAIN)
// 			{
// 				CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_48, CVarList());
// 				return 0;
// 			}

			m_pGuildModule->OnCustomQuit(pKernel, self, args);
		}
		break;
		case CS_GUILD_FIRE:// �����߳�����
		{
			//// У�鹫���ڼ��Ƿ��������
			if (!m_pGuildModule->CheckGuildActivity(pKernel, self))
			{
				return 0;
			}

			//�����в���ִ�иò���
// 			if (GuildImpeach::IsImpeachIng(pKernel, self))
// 			{
// 				CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_48, CVarList());
// 				return 0;
// 			}

			m_pGuildModule->OnCustomFire(pKernel, self, args);
		}
		break;
		case CS_GUILD_DISMISS:// �����ɢ����
		{
			// У�鹫���ڼ��Ƿ��������
			if (!m_pGuildModule->CheckGuildActivity(pKernel, self))
			{
				return 0;
			}

			//�����в���ִ�иò���
// 			if (GuildImpeach::IsImpeachIng(pKernel, self))
// 			{
// 				CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_48, CVarList());
// 				return 0;
// 			}

			m_pGuildModule->OnCustomDismiss(pKernel, self, args);
		}
		break;
		case CS_GUILD_DONATE: //����
		{
			m_pGuildModule->OnCustomDonate(pKernel, self, args);
		}
		break;
		case CS_GUILD_PROMOTION: //��ְ
		{
			// У�鹫���ڼ��Ƿ��������
			if (!m_pGuildModule->CheckGuildActivity(pKernel, self))
			{
				return 0;
			}

			m_pGuildModule->OnCustomPromotion(pKernel, self, args, CS_GUILD_PROMOTION);
		}
		break;
		case CS_GUILD_DEMOTION: //��ְ
		{
			// У�鹫���ڼ��Ƿ��������
			if (!m_pGuildModule->CheckGuildActivity(pKernel, self))
			{
				return 0;
			}

			m_pGuildModule->OnCustomPromotion(pKernel, self, args, CS_GUILD_DEMOTION);
		}
		break;
		case CS_GUILD_SHOP_LIST:// ���󹫻��̵��б�
		{
			m_pGuildModule->OnCustomGetShopList(pKernel, self, args);
		}
		break;
		case CS_GUILD_BUY_ITEM:// �������̵���Ʒ
		{
			m_pGuildModule->OnCustomBuyItem(pKernel, self, args);
		}
		break;
		case CS_GUILD_CAPTAIN_TRANSFER: //ת�ư���
		{
			//�����в���ִ�иò���
// 			if (GuildImpeach::IsImpeachIng(pKernel, self))
// 			{
// 				CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_48, CVarList());
// 				return 0;
// 			}
			if (!m_pGuildModule->CheckGuildActivity(pKernel, self))
			{
				return 0;
			}

			m_pGuildModule->OnCustomCaptainTransfer(pKernel, self, args);
		}
		break;
		case CS_GUILD_JOIN_CD:// ������빫��CDʱ��
		{
			m_pGuildModule->OnCustomGetJoinCD(pKernel, self, args);
		}
		break;
		case CS_GUILD_LOG: // ��������־
		{
			m_pGuildModule->OnCustomGetLog(pKernel, self, args);
		}
		break;
		case CS_GUILD_LOG_DONATE: // �����������־
		{
			m_pGuildModule->OnCustomGetDonateLog(pKernel, self, args);
		}
		break;

		case CS_GUILD_AUTO_AGREE://�Զ�ͬ�������֯
		{
			m_pGuildModule->OnCustomSetAutoAgree(pKernel, self, args);
		}break;
		case CS_GUILD_UNLOCK_SKILL://�������Ἴ��
		{
			m_pGuildModule->OnCustomUnLockSkill(pKernel, self, args);
		}break;
		case CS_GUILD_GET_SKILL_LIST://��ȡ���Ἴ���б�
		{
			IGameObj *pSelfObj = pKernel->GetGameObj(self);
			if (pSelfObj == NULL){
				break;
			}

			const wchar_t * guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
			if (StringUtil::CharIsNull(guildName)){
				break;
			}

			CVarList msg;
			msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_SKILL_LIST;
			m_pGuildModule->OnCustomGetSkillList(pKernel, guildName, msg);
			pKernel->Custom(self, msg);
		}break;
		case CS_GUILD_SKILL_LEVEL_UP://��������
		{
			m_pGuildModule->OnCustomSkillLevelUp(pKernel, self, args);
		}break;
		
		case CS_GUILD_BUILDING_OPERATE_LV:
		{
			m_pGuildModule->OnGuildBuildingOperateLv(pKernel, self, args);
		}break;
		case CS_GUILD_NUM_BUY:
		{
			m_pGuildModule->OnGuildNumBuy(pKernel, self, args);
		}break;
		case CS_GUILD_CHANGE_SHORT_NAME:
		{
			m_pGuildModule->OnCustomChangeGuildShortName(pKernel, self, args);
		}break;
		case CS_GUILD_START_OPERATE_SKILL:
		{
			m_pGuildModule->OnCustomGuildSkillLvOperate(pKernel, self, args);
		}break;
		case CS_GUILD_GET_JIAN_KANG_DU_GIF:
		{
			m_pGuildModule->OnCustomGetJianKangDuGif(pKernel, self);

		}break;
		case CS_GUILD_GET_GUILD_NUM:
		{
			const wchar_t*guildName = args.WideStrVal(2);
			m_pGuildModule->OnCustomGuildNumInfo(pKernel, self, guildName);
		}break;
		case CS_GUILD_GET_GUILD_SYSMBOL:
		{
			const wchar_t*guildName = args.WideStrVal(2);
			m_pGuildModule->OnCustomGuildSysmbol(pKernel, self, guildName);
		}break;
		case CS_GUILD_GET_GUILD_BUILDING:
		{
			m_pGuildModule->OnCustomGuildBuilding(pKernel, self);
		}break;
		case CS_GUILD_GET_GUILD_SET_INFO:
		{
			m_pGuildModule->OnCustomGuildSetInfo(pKernel,self);
		}break;
		case CS_GUILD_USE_RARE_TREASURE:{
			int rareTreasureType = args.IntVal(2);
			int useNum = args.IntVal(3);
			m_pGuildModule->OnCustomUseRareTreasure(pKernel, self, rareTreasureType, useNum);
		}break;
		case CS_GUILD_GET_RARE_TREASURE_LIST:{
			m_pGuildModule->OnCustomGetRareTreasureList(pKernel, self);
		}break;
		case CS_GUILD_GET_NUM_BUY_RECORD:{
			m_pGuildModule->OnCustomGetNumBuyRecord(pKernel, self);
		}break;
		case CS_GUILD_GET_BONUS_INFO:{

			m_pGuildModule->OnCustomGetBonusInfo(pKernel,self);
		}break;
		case CS_GUILD_GET_WELFARE_INFO:{
			m_pGuildModule->OnCustomGetGuildWelfare(pKernel, self);
		}break;
		case CS_GUILD_GIVE_OUT_BONUS:{

			m_pGuildModule->OnCustomGiveOutBonus(pKernel, self,args);
		}break;
		case CS_GUILD_INVITE_JOIN_GUILD:{
			m_pGuildModule->OnCustomInviteJoinGuild(pKernel, self, args);
		}break;
		case CS_GUILD_INVITE_JOIN_GUILD_RSP:{
			m_pGuildModule->OnCustomAgreeInviteJoinGuild(pKernel, self, args);
		}break;
		case CS_GUILD_ADD_PETITION:{
			m_pGuildModule->OnCustomAddPetition(pKernel, self, args);
		}break;
		case CS_GUILD_GET_PETITION_LIST:
		{
			m_pGuildModule->OnCustomGetPetition(pKernel, self, args);
		}break;
		case CS_GUILD_GET_OTHER_GUILD_INFO:
		{
			m_pGuildModule->OnCustomGetOtherGuildInfo(pKernel,self,args);
		}break;
// 		case CS_GUILD_TEACH_LIST:
// 		{
// 			// ��ȡ���߿ɴ����б�
// 			m_pGuildModule->OnCustomTeachList(pKernel, self, args, 2);
// 		}break;
// 		case CS_GUILD_TEACH_REQ:
// 		{			
// 			// ����Է����� args: nick[widestr] type[1:req 2:push]
// 			m_pGuildModule->OnCustomTeachReq(pKernel, self, args, 2);
// 		}
// 		break;
// 		case CS_GUILD_TEACH_ACK:
// 		{
// 			// ��Ӧ���Է����� args: nick[widestr] type[1:req 2:push] agree[1: ͬ�� 0: �ܾ�]
// 			m_pGuildModule->OnCustomTeachAck(pKernel, self, args, 2);
// 		}break;
// 		case CS_GUILD_TEACH_READY:
// 		{
// 			// �ͻ��˸��߷�����׼������ args: none
// 			m_pGuildModule->OnCustomTeachReady(pKernel, self, args, 2);
// 		}
	default:
		break;
	}

	return 0;
}

// ���������������
int GuildModule::OnCustomCreateGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	// obtain parameters
	const wchar_t* guildName = args.WideStrVal(2);
    int guildIndentifying = args.IntVal(3);
	const wchar_t* guildShortName = args.WideStrVal(4);
	

	// check condifion
	IGameObj* pPlayer = pKernel->GetGameObj(self);
	if (pPlayer == NULL)
	{
		return 0;
	}
	if (StringUtil::CharIsNull(guildShortName)){
		return 0;
	}


	//���ᴴ�����ܿ������
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_GUILD_CREATE, self))
	{
		return 0;
	}

	int64_t quitGuildDate = pPlayer->QueryInt64("QuitGuildDate");
	if (quitGuildDate > 0)
	{
		int passMinutes = GetPassMinutes(quitGuildDate);
		if (passMinutes < EnvirValueModule::EnvirQueryInt(ENV_VALUE_ADD_GUILD_CD))
		{
			// ������ȴʱ�䣬�޷���������,�����˳���������
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17623, CVarList());
			return 0;
		}
	}

	if (!CheckCanCreateGuild(pKernel, self, guildName))
	{
		return 0;
	}
	if (wcslen(guildShortName)>1){
		return 0;
	}

	if (GuildShortNameExist(pKernel, guildShortName))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17618, CVarList());
		return 0;
	}
	


	const wchar_t* playerName = pPlayer->QueryWideStr("Name");
	const char* pPlayerUid = pKernel->SeekRoleUid(playerName);
	int sex = pPlayer->QueryInt("Sex");
	int career = pPlayer->QueryInt("Job");
	int ability = pPlayer->QueryInt("BattleAbility");
	int nation = pPlayer->QueryInt("Nation");
	int vipLv = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);

	// ��ͭ�ң��������ʧ�ܵĻ��ڽ�����������ټ�Ǯ
	int nCostGold = EnvirValueModule::EnvirQueryInt(ENV_VALUE_CREATE_GUILD_GOLD);
	int rc = m_pCapitalModule->DecCapital(pKernel, self, CAPITAL_GOLD, (__int64)nCostGold, FUNCTION_EVENT_ID_GUILD_CREATE);
	if (rc != DC_SUCCESS)
	{
		return 0;
	}

	CVarList msg;
	msg << PUBSPACE_GUILD << guildName << SP_GUILD_MSG_CREATE
		<< pPlayerUid << playerName << sex << pPlayer->QueryInt("Level") 
		<< career << ability << nation 
		<< guildIndentifying << guildShortName << vipLv;
	pKernel->SendPublicMessage(msg);

	return 0;
}

// �����ȡ�����б�����, ÿ��ȡREQUEST_GUILD_LIST_PRE_COUNT�������¼
int GuildModule::OnCustomGetGuildList(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	//��ҳ��
	int index = args.IntVal(2);
	if (index < 0)
	{
		return 0;
	}

	if (index == 0)
	{
		pSelfObj->SetInt("GuildListBeginRow", 0);
	}

	IGameObj* pPlayer = pKernel->GetGameObj(self);
	if (pPlayer == NULL)
	{
		return 0;
	}

	// �����ҵȼ��Ƿ��������
	if (ActivateFunctionModule::CheckActivateFunction(pKernel, self, AFM_GUILD_FUNCTION))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17604, CVarList());
		return false;
	}

	IRecord* pGuildListRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
	IRecord* pGuildSortRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_SORT_REC);
	if (pGuildListRecord == NULL || pGuildSortRecord == NULL)
	{
		return 0;
	}

	int rowTotal = pGuildSortRecord->GetRows();
	int rowStart = pSelfObj->QueryInt("GuildListBeginRow");
	if (rowStart >= rowTotal)
	{
		m_pVirtualRecModule->SendVirtualRecord(pKernel, self, GUILD_LIST_VIRTUALREC_NAME, CVarList());
		return 0;
	}

	CVarList msg;
	int getRowCount = 1;    // ��¼ȡ�˼��й����¼
	int rankStart = index * REQUEST_GUILD_LIST_PRE_COUNT;
	int nation = pSelfObj->QueryInt("Nation");
	// ѭ������
	LoopBeginCheck(dh);
	for (int i = rowStart; i < rowTotal; i++)
	{
		LoopDoCheck(dh);
		if (getRowCount > REQUEST_GUILD_LIST_PRE_COUNT)
		{
			pSelfObj->SetInt("GuildListBeginRow", i);
			break;
		}

		const wchar_t *guildName = pGuildSortRecord->QueryWideStr(i, GUILD_SORT_REC_COL_NAME);
		if (StringUtil::CharIsNull(guildName))
		{
			continue;
		}

		SerialGuildInfo(pKernel, self, msg, rankStart+1, guildName);

		getRowCount++;
		rankStart++;
	}
	m_pVirtualRecModule->SendVirtualRecord(pKernel, self, GUILD_LIST_VIRTUALREC_NAME, msg);

	return 0;
}

// ����������������
int GuildModule::OnCustomFindGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	const wchar_t* searchName = args.WideStrVal(2);
	if (!IsWidStrValid(searchName))
	{
		return 0;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	IGameObj* pPlayer = pKernel->GetGameObj(self);
	if (pPlayer == NULL)
	{
		return 0;
	}

	// �����ҵȼ��Ƿ��������
	if (ActivateFunctionModule::CheckActivateFunction(pKernel, self, AFM_GUILD_FUNCTION))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17604, CVarList());
		return false;
	}

	IRecord* pGuildListRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_SORT_REC);
	if (pGuildListRecord == NULL)
	{
		return 0;
	}

	int iGuildListRow = pGuildListRecord->GetRows();
	if (iGuildListRow <= 0)
	{
		return 0;
	}

	typedef std::vector<std::pair<int,std::wstring> > GuildNameVec;
	typedef GuildNameVec::iterator GuildNameVecIter;

	int rankVal = 0;
	int guildCount = 0;
	GuildNameVec guildNameVector;

	// ѭ������
	LoopBeginCheck(di);
	for (int i = 0; i < iGuildListRow; i++)
	{
		LoopDoCheck(di);
		rankVal++;

		const wchar_t* pGuildName = pGuildListRecord->QueryWideStr(i, GUILD_SORT_REC_COL_NAME);
		if(!::wcsstr(pGuildName, searchName))
		{
			continue;
		}

		guildNameVector.push_back(std::make_pair(rankVal, std::wstring(pGuildName)));
		guildCount++;
		if (guildCount >= REQUEST_GUILD_LIST_PRE_COUNT)
		{
			break;
		}
	}

	int number = 0;
	CVarList varGuild;
	// ѭ������
	LoopBeginCheck(dj);
	for(GuildNameVecIter iter = guildNameVector.begin(); iter != guildNameVector.end() ; ++iter)
	{
		LoopDoCheck(dj);
		if (SerialGuildInfo(pKernel, self, varGuild, iter->first, iter->second.c_str()))
		{
			number++;
		}
	}

	CVarList rmsg;
	rmsg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_SEARCH_RESULT << number << varGuild;
	pKernel->Custom(self, rmsg);

	return 0;
}

// ��������������
int GuildModule::OnCustomGetTableData(IKernel* pKernel, const PERSISTID& self, int msgType)
{
	CVarList msg;
	msg << SERVER_CUSTOMMSG_GUILD;

	std::string tableName;
	switch(msgType)
	{
	case CS_GUILD_MEMBER_LIST:
		{
			tableName = GUILD_MEMBER_REC;
			msg << SC_GUILD_MEMBER_LIST;
		}
		break;
	case CS_GUILD_APPLY_LIST:
		{
			tableName = GUILD_JOIN_REC;
			msg << SC_GUILD_APPLY_LIST;
		}
		break;
	default:
		return 0;
	}

	IPubData* pGuildPubData = GetPubData(pKernel, GUILD_PUB_DATA);
	if (pGuildPubData == NULL)
	{
		return 0;
	}

	IRecord* pRecord = pGuildPubData->GetRecord(tableName.c_str());
	if (pRecord == NULL)
	{
		// ����������δ�鵽���ڹ�����������ѯ
		const wchar_t* guildName = GetPlayerGuildName(pKernel, self);
		IPubData* pGuildData = GetPubData(pKernel, guildName);
		if (pGuildData == NULL)
		{
			return 0;
		}

		pRecord = pGuildData->GetRecord(tableName.c_str());
		if (pRecord == NULL)
		{
			return 0;
		}
	}

	int rows = pRecord->GetRows();
	// ѭ������
	LoopBeginCheck(dk);
	for (int i = 0; i < rows; ++i)
	{
		LoopDoCheck(dk);
		CVarList rowValue;
		pRecord->QueryRowValue(i, rowValue);      
		msg << rowValue;
	}

	pKernel->Custom(self, msg);

	return 0;
}

// ����������빫�������
int GuildModule::OnCustomApplyJoinGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	const wchar_t* guildName = args.WideStrVal(2);
	if (!IsWidStrValid(guildName))
	{
		return 0;
	}

	IGameObj * pPlayer = pKernel->GetGameObj(self); 
	if(pPlayer == NULL)
	{
		return 0;
	}

	//��������Ὺ���Ƿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_GUILD_JOIN, self))
	{
		return 0;
	}

	const wchar_t* playerName = pPlayer->QueryWideStr("Name");
	const wchar_t* selfGuildName = pPlayer->QueryWideStr("GuildName");
	if (IsWidStrValid(selfGuildName))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17605, CVarList());
		return 0;
	}

	// ����Ƿ���CD��
	int64_t quitGuildDate = pPlayer->QueryInt64("QuitGuildDate");
	if (quitGuildDate > 0)
	{
		int passMinutes = GetPassMinutes(quitGuildDate);
		if (passMinutes < EnvirValueModule::EnvirQueryInt(ENV_VALUE_ADD_GUILD_CD))
		{
			// ������ȴʱ�䣬�޷�������빫��
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17623, CVarList());
			return 0;
		}
	}

	// �����ҵȼ��Ƿ��������
	if (ActivateFunctionModule::CheckActivateFunction(pKernel, self, AFM_GUILD_FUNCTION))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17604, CVarList());
		return 0;
	}

	// �����������б��Ƿ��м�¼
	IRecord* applyRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_APPLY_REC);
	if (applyRecord == NULL)
	{
		return 0;
	}
	int row = applyRecord->FindWideStr(GUILD_APPLY_REC_COL_NAME, playerName);
	if (row >= 0)
	{
		// ����Ҵ��������¼
		std::wstring applyGuildString = applyRecord->QueryWideStr(row, GUILD_APPLY_REC_COL_GUILD);
		CVarList applyGuildList;
		::util_split_wstring(applyGuildList, applyGuildString, L";");

		// �ﵽ����������
		int guildCount = (int)applyGuildList.GetCount();
		if (guildCount >= GUILD_APPLY_MAX_NUM)
		{
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17619, CVarList());
			return 0;
		}

		// ѭ������
		LoopBeginCheck(dl);
		for (int i = 0; i < guildCount; ++i)
		{
			LoopDoCheck(dl);
			// �Ѿ�������ù���
			if (wcscmp(guildName, applyGuildList.WideStrVal(i)) == 0)
			{
				CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17620, CVarList());
				return 0;
			}
		}
	}

	// ��鹫��������б��Ƿ�����
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}
	IRecord* joinRecord = pGuildData->GetRecord(GUILD_JOIN_REC);
	if (joinRecord == NULL)
	{
		return 0;
	}
	if (joinRecord->GetRows() >= GUILD_JOIN_REC_ROW_MAX)
	{
		//�ù��������б�����
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17621, CVarList());
		return 0;
	}

	// ȡ����ȼ�
	IRecord* guildListRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC); 
	if (guildListRecord == NULL)
	{
		return 0;
	}
	row = guildListRecord->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (row < 0)
	{
		return 0;
	}
	int xiangFangLevel = GuildBuildingLevel(pKernel, guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);

	// ���ݹ���ȼ���ȡ��������Ա����
	GuildUpLevelConfig_t *upLevelConfig = GetGuildUpLevelConfig(xiangFangLevel);
	if (upLevelConfig == NULL)
	{
		return 0;
	}
	int maxMember = upLevelConfig->m_MaxMember;

	// ȡ���ᵱǰ��Ա����
	IRecord* memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (memberRecord == NULL)
	{
		return 0;
	}
	int curMember  = memberRecord->GetRows();

	// �жϹ����Ƿ���Ա
	if (curMember >= maxMember)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17622, CVarList());
		return 0;
	}

	IRecord* pGuildListRec = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
	if (pGuildListRec == NULL)
	{
		return 0;
	}
	int guildListRow = pGuildListRec->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildListRow < 0)
	{
		return 0;
	}

	const char* pPlayerUid = pKernel->SeekRoleUid(playerName);
	int playerSex   = pPlayer->QueryInt("Sex");
	int playerLevel = pPlayer->QueryInt("Level");
	int ability = pPlayer->QueryInt("BattleAbility");
	int career = pPlayer->QueryInt("Job");
	int vipLevel = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
	CVarList msg;
	msg << PUBSPACE_GUILD << guildName << SP_GUILD_MSG_APPLY_JOIN << pPlayerUid << playerName 
		<< playerSex << playerLevel << ability << career << vipLevel;
	pKernel->SendPublicMessage(msg);

	return 1;
}

int GuildModule::OnCustomOneKeyApplyJoinGuild(IKernel* pKernel, const PERSISTID& self)
{

	IGameObj * pPlayer = pKernel->GetGameObj(self);
	if (pPlayer == NULL)
	{
		return 0;
	}
	//��������Ὺ���Ƿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_GUILD_JOIN, self))
	{
		return 0;
	}

	// ����Ƿ���CD��
	int64_t quitGuildDate = pPlayer->QueryInt64("QuitGuildDate");
	if (quitGuildDate > 0)
	{
		int passMinutes = GetPassMinutes(quitGuildDate);
		if (passMinutes < EnvirValueModule::EnvirQueryInt(ENV_VALUE_ADD_GUILD_CD))
		{
			// ������ȴʱ�䣬�޷�������빫��
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17623, CVarList());
			return 0;
		}
	}

	int64_t OneKeyJoinTime = pPlayer->QueryInt64("OneKeyJoinTime");
	if (OneKeyJoinTime >0)
	{
		int passMinutes = GetPassMinutes(OneKeyJoinTime);
		if (passMinutes < GUILD_ONE_KEY_JOIN_CD)
		{
			// ������ȴʱ�䣬�޷�������빫��
			return 0;
		}
		
	}
	pPlayer->SetInt64("OneKeyJoinTime", (int64_t)::time(NULL));
	const wchar_t* playerName = pPlayer->QueryWideStr("Name");
	const wchar_t* selfGuildName = pPlayer->QueryWideStr("GuildName");
	if (IsWidStrValid(selfGuildName))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17605, CVarList());
		return 0;
	}
	// �����ҵȼ��Ƿ��������
	if (ActivateFunctionModule::CheckActivateFunction(pKernel, self, AFM_GUILD_FUNCTION))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17604, CVarList());
		return false;
	}
	
	// �����������б��Ƿ��м�¼
	IRecord* applyRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_APPLY_REC);
	if (applyRecord == NULL)
	{
		return 0;
	}

	int sendApplyCountMax = 10;
	int applayRow = applyRecord->FindWideStr(GUILD_APPLY_REC_COL_NAME, playerName);
	std::wstring applyGuildString;
	if (applayRow >= 0)
	{
		// ����Ҵ��������¼
		applyGuildString = applyRecord->QueryWideStr(applayRow, GUILD_APPLY_REC_COL_GUILD);
		CVarList applyGuildList;
		::util_split_wstring(applyGuildList, applyGuildString, L";");

		// �ﵽ����������
		int guildCount = (int)applyGuildList.GetCount();
		if (guildCount >= GUILD_APPLY_MAX_NUM)
		{
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17619, CVarList());
			return 0;
		}
		else
		{
			sendApplyCountMax = GUILD_APPLY_MAX_NUM - guildCount;
		}
	}

	IRecord* pGuildSortRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_SORT_REC);
	if ( pGuildSortRecord == NULL)
	{
		return 0;
	}

	int rowTotal = pGuildSortRecord->GetRows();

	if (rowTotal <= 0)
	{
		return 0;
	}
	
	const char* pPlayerUid = pKernel->SeekRoleUid(playerName);
	int playerSex = pPlayer->QueryInt("Sex");
	int playerLevel = pPlayer->QueryInt("Level");
	int ability = pPlayer->QueryInt("BattleAbility");
	int career = pPlayer->QueryInt("Job");
	int vipLv = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);

	IRecord* pGuildListRec = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
	if (pGuildListRec == NULL)
	{
		return 0;
	}

	// ȡ����ȼ�
	IRecord* guildListRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
	if (guildListRecord == NULL)
	{
		return 0;
	}
	//�������������
	int sendApplyCount = 0;

	CVarList guildList;

	// ѭ������
	LoopBeginCheck(dh);
	for (int i = 0; i < rowTotal; i++)
	{
		LoopDoCheck(dh);
		const wchar_t *guildName = pGuildSortRecord->QueryWideStr(i, GUILD_SORT_REC_COL_NAME);
		if (StringUtil::CharIsNull(guildName))
		{
			continue;
		}

		if (applyGuildString.find(guildName) != std::string::npos)
		{
			continue;
		}
		

		// ��鹫��������б��Ƿ�����
		IPubData* pGuildData = GetPubData(pKernel, guildName);
		if (pGuildData == NULL)
		{
			continue;
		}
		IRecord* joinRecord = pGuildData->GetRecord(GUILD_JOIN_REC);
		if (joinRecord == NULL)
		{
			continue;
		}
		if (joinRecord->GetRows() >= GUILD_JOIN_REC_ROW_MAX)
		{
			continue;
		}

		int row = guildListRecord->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
		if (row < 0)
		{
			continue;
		}
		
		int xiangFangLevel = GuildBuildingLevel(pKernel, guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);
		// ���ݹ���ȼ���ȡ��������Ա����
		GuildUpLevelConfig_t *upLevelConfig = GetGuildUpLevelConfig(xiangFangLevel);
		if (upLevelConfig == NULL)
		{
			continue;
		}
		int maxMember = upLevelConfig->m_MaxMember;

		// ȡ���ᵱǰ��Ա����
		IRecord* memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
		if (memberRecord == NULL)
		{
			continue;
		}
		int curMember = memberRecord->GetRows();

		// �жϹ����Ƿ���Ա
		if (curMember >= maxMember)
		{
			continue;
		}

		int guildListRow = pGuildListRec->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
		if (guildListRow < 0)
		{
			continue;
		}

		if (sendApplyCount >= sendApplyCountMax)
		{
			break;
		}

		guildList << guildName;

		sendApplyCount++;
	}

	CVarList msg;
	msg << PUBSPACE_GUILD <<"" << SP_GUILD_START_ONE_KEY_APPLY_JOIN << pPlayerUid << playerName
		<< playerSex << playerLevel << ability << career << vipLv<< guildList.GetCount() << guildList;
	pKernel->SendPublicMessage(msg);
	return 0;
}




// ����ȡ��������빫�������
int GuildModule::OnCustomCancelApplyJoinGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	const wchar_t* guildName = args.WideStrVal(2);
	if (!IsWidStrValid(guildName))
	{
		return 0;
	}

	IRecord* applyRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_APPLY_REC);
	if (applyRecord == NULL)
	{
		return 0;
	}

	const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
	const char* playerUid = pKernel->SeekRoleUid(playerName);
	int row = applyRecord->FindString(GUILD_APPLY_REC_COL_NAME_UID, playerUid);
	if (row < 0)
	{
		// �Ѿ������������¼ ֱ��֪ͨ�ͻ���ȡ���ɹ�
		m_pGuildModule->OnCommandCancelApplyJoinGuildResult(pKernel, self, CVarList() << 0 << 0 << guildName << GUILD_MSG_REQ_SUCC);
		return 0;
	}

	std::wstring applyGuildString = applyRecord->QueryWideStr(row, GUILD_APPLY_REC_COL_GUILD);
	CVarList applyGuildList;
	::util_split_wstring(applyGuildList, applyGuildString, L";");
	int guildCount = (int)applyGuildList.GetCount();

	// ѭ������
	LoopBeginCheck(dm);
	for (int i = 0; i < guildCount; ++i)
	{
		LoopDoCheck(dm);
		if (wcscmp(guildName, applyGuildList.WideStrVal(i)) == 0)
		{
			CVarList msg;
			msg << PUBSPACE_GUILD << guildName << SP_GUILD_MSG_CANCEL_APPLY_JOIN << playerUid << playerName;
			pKernel->SendPublicMessage(msg);

			break;
		}
	}

	return 0;
}

// �����ȡ������Ϣ������
int GuildModule::OnCustomGetGuildInfo(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// ֻ�ܻ�ȡ�Լ��Ĺ�����Ϣ
	const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
	if (!IsWidStrValid(guildName))
	{
		return 0;
	}

	// �����ҵȼ��Ƿ��������
	if (ActivateFunctionModule::CheckActivateFunction(pKernel, self, AFM_GUILD_FUNCTION))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17604, CVarList());
		return false;
	}

	// ��ȡ�����ռ�
	IPubData* pPubData = GetPubData(pKernel, GUILD_PUB_DATA);
	if (pPubData == NULL)
	{
		return 0;
	}

	// ��ȡ�����б�
	IRecord* guildListRecord = pPubData->GetRecord(GUILD_LIST_REC);
	if (guildListRecord == NULL)
	{
		return 0;
	}

	// ����������ڹ���
	int row = guildListRecord->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (row < 0)
	{
		// ������ϵĹ����¼�Ѿ�������
		pSelfObj->SetWideStr("GuildName", L"");
		pSelfObj->SetInt("GuildPosition", GUILD_POSITION_NONE);
		//LogModule::m_pLogModule->OnGuildQuit(pKernel, self);
		return 0;
	}

	// ��ȡ������
	IRecord* pSortRecord = pPubData->GetRecord(GUILD_SORT_REC);
	if (pSortRecord == NULL)
	{
		return 0;
	}

	// ���ҹ�������
	int rank = 0;
	int tempNationId = 0;
	std::wstring tempName = L"";
	int sortCount = pSortRecord->GetRows();
	// ѭ������
	LoopBeginCheck(dn);
	for (int i = 0; i < sortCount; ++i)
	{
		LoopDoCheck(dn);
		++rank;
		tempName = pSortRecord->QueryWideStr(i, GUILD_SORT_REC_COL_NAME);
		if (tempName == guildName)
		{
			break;
		}
	}

	// ��ȡ��ṫ���ռ�
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}

	// ��ȡ��Ա�б�
	IRecord* memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (memberRecord == NULL)
	{
		return 0;
	}

	const wchar_t *playerName = pSelfObj->QueryWideStr("Name");
	int memberRow = memberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, playerName);
	if (memberRow < 0)
	{
		return 0;
	}

	IRecord* joinRecord = pGuildData->GetRecord(GUILD_JOIN_REC);
	if (joinRecord == NULL)
	{
		return 0;
	}

	const wchar_t *captainName = guildListRecord->QueryWideStr(row, GUILD_LIST_REC_COL_CAPTAIN);
	int guildJuYiTingLevel = GetGuildLevel(pKernel, guildName);
	int curMember  = memberRecord->GetRows();
	int xiangFangLevel = GuildBuildingLevel(pKernel, guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);
	GuildUpLevelConfig_t *upLevelConfig = GetGuildUpLevelConfig(xiangFangLevel);
	if (upLevelConfig == NULL)
	{
		return 0;
	}
	int maxMember   = upLevelConfig->m_MaxMember;
	const wchar_t* notice = guildListRecord->QueryWideStr(row, GUILD_LIST_REC_COL_ANNOUNCEMENT);
	// �������
	const wchar_t* decal = guildListRecord->QueryWideStr(row, GUILD_LIST_REC_COL_DECLARATION);
	int maxContrib  = memberRecord->QueryInt(memberRow, GUILD_MEMBER_REC_COL_DEVOTE);
	int applyNumber = joinRecord->GetRows();
	int selfPosition= memberRecord->QueryInt(memberRow, GUILD_MEMBER_REC_COL_POSITION);
	int ability = guildListRecord->QueryInt(row, GUILD_LIST_REC_COL_FIGHT_ABILITY);
	int guildCapital = GetGuildNumValue(pKernel,guildName,GUILD_NUM_TYPE::GUILD_NUM_CAPITAL);
	int devoteSilver = memberRecord->QueryInt(memberRow, GUILD_MEMBER_REC_COL_DEVOTE_SILVER);
	int64_t xiBingFuTime = pGuildData->QueryAttrInt64(STOPPING_WAR_TIME);

	// ��ȡ����������
	IPubData* pGuildPubdata_ = GetPubData(pKernel, guildName);
	int nFireNum_ = 0;
	if (NULL != pGuildPubdata_)
	{
		nFireNum_ = pGuildPubdata_->QueryAttrInt("FireNum");
	}

	CVarList msg;
	msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_GUILD_INFO << guildName << captainName << guildJuYiTingLevel
		<< curMember << maxMember << notice << maxContrib
		<< applyNumber << selfPosition << rank << decal << ability << devoteSilver << xiBingFuTime << nFireNum_;
	pKernel->Custom(self, msg);


	OnCustomGuildNumInfo(pKernel,self,guildName);
	OnCustomGuildSysmbol(pKernel,self,guildName);
	OnCustomGuildBuilding(pKernel, self);
	return 0;
}


int GuildModule::OnCustomGuildNumInfo(IKernel*pKernel, const PERSISTID& self, const wchar_t * guildName)
{


	IRecord *pGuildNumRec = GetPubRecord(pKernel, guildName,GUILD_NUM_REC);
	if (pGuildNumRec == NULL){
		return 0;
	}

	CVarList info;

	
	int rowMax = pGuildNumRec->GetRows();

	for (int i = 0; i < rowMax; i++)
	{

		CVarList var;
		pGuildNumRec->QueryRowValue(i, var);
		info << var;
	}

	CVarList msg;
	msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_GUILD_NUM_INFO << info;
	pKernel->Custom(self, msg);
	return 0;
}




int GuildModule::OnCustomGuildSysmbol(IKernel*pKernel,const PERSISTID& self, const wchar_t * guildName)
{
	

	IRecord *pGuildSysmbolRec = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_SYSMBOL_REC) ;
	if (pGuildSysmbolRec == NULL){
		return 0;
	}
	int row = pGuildSysmbolRec->FindWideStr(GUILD_SYSMBOL_REC_COL_GUILD_NAME, guildName);
	if (row < 0){
		return 0;
	}

	int identifying = pGuildSysmbolRec->QueryInt(row, GUILD_SYSMBOL_REC_COL_IDENTIFYING);
	const wchar_t* shortName = pGuildSysmbolRec->QueryWideStr(row, GUILD_SYSMBOL_REC_COL_SHORT_NAME);
	CVarList msg;
	msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_GUILD_SYSMBOL << identifying << shortName;
	pKernel->Custom(self, msg);

	return 0;
}

int GuildModule::OnCustomGuildBuilding(IKernel*pKernel, const PERSISTID& self)
{
		const wchar_t* guildName = pKernel->QueryWideStr(self,FIELD_PROP_GUILD_NAME);
		IRecord *pGuildBuildingRec = GetPubRecord(pKernel, guildName, GUILD_BUILDING_REC); 
		if (pGuildBuildingRec == NULL){
			return 0;
		}
		

		int rowMax = pGuildBuildingRec->GetRows();
		CVarList info;
		for (int i = 0; i < rowMax; i++)
		{

			CVarList var;
			pGuildBuildingRec->QueryRowValue(i, var);
			info << var;
		}


		CVarList msg;
		msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_BUILDING_INFO << info;
		pKernel->Custom(self, msg);
		return 0;
}

int GuildModule::OnCustomGuildSetInfo(IKernel*pKernel, const PERSISTID& self)
{
	const wchar_t* guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);

	IRecord *pGuildSetRec = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_SET_REC);
	if (pGuildSetRec == NULL){
		return 0;
	}


	int row = pGuildSetRec->FindWideStr(GUILD_SET_REC_COL_GUILD_NAME,guildName);
	if (row < 0){
		return 0;
	}
	

	CVarList var;
	pGuildSetRec->QueryRowValue(row, var);


	CVarList msg;
	msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_SET_INFO << var;
	pKernel->Custom(self, msg);

	return 0;
}




// ������ܼ��빫�������
int GuildModule::OnCustomAcceptJoinGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	const wchar_t* applyName = args.WideStrVal(2);
	if (!IsWidStrValid(applyName))
	{
		return 0;
	}

	const wchar_t *guildName = pSelfObj->QueryWideStr("GuildName");
	if (StringUtil::CharIsNull(guildName))
	{
		return 0;
	}

	if (!CheckCanOptJoinGuild(pKernel, self, applyName))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17624, CVarList());
		return 0;
	}

	const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
	const char* pPlayerNameUid = pKernel->SeekRoleUid(playerName);
	const char* pApplyNameUid = pKernel->SeekRoleUid(applyName);

	// ��ù���ȼ�
	IRecord* pGuildList = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		return 0;
	}
	int guildRow = pGuildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		return 0;
	}
	int guildJuYiTingLevel = GetGuildLevel(pKernel, guildName);
	int xiangFangLevel = GuildBuildingLevel(pKernel, guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);
	// ��ù�������Ա����
	GuildUpLevelConfig_t* upLevelConfig = GetGuildUpLevelConfig(xiangFangLevel);
	if (upLevelConfig == NULL)
	{
		return 0;
	}

	// �������Ƿ����� 
	int applyOnline = (pKernel->GetPlayerScene(applyName) > 0) ? ONLINE : OFFLINE;

	CVarList msg;
	msg << PUBSPACE_GUILD << guildName 
		<< SP_GUILD_MSG_ACCEPT_JOIN << pApplyNameUid << applyName
		<< playerName << guildJuYiTingLevel << upLevelConfig->m_MaxMember << applyOnline;

	pKernel->SendPublicMessage(msg);


	// �������������Ϣ֪ͨ
// 	if( applyOnline == PLAYER_ONLINE && GuildDefendModule::m_pGuildDefendModule->IsGuildDefendRunning(pKernel, guildName) )
// 	{
// 		pKernel->CustomByName(applyName, CVarList() << SERVER_CUSTOMMSG_GUILD_ACTIVITY 
// 			                                         << SC_GUILD_DEFEND_RUNNING
// 		                                	         << 1);
// 	}


	return 1;
}

// ����ܾ����빫�������
int GuildModule::OnCustomRefuseJoinGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	const wchar_t* applyName = args.WideStrVal(2);
	if (!IsWidStrValid(applyName))
	{
		return 0;
	}

	const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
	if (!IsWidStrValid(guildName))
	{
		return 0;
	}

	if (!CheckCanOptJoinGuild(pKernel, self, applyName))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17624, CVarList());
		return 0;
	}

	const wchar_t* playerName = pSelfObj->QueryWideStr("Name");

	CVarList msg;
	msg << PUBSPACE_GUILD << guildName << SP_GUILD_MSG_REFUSE_JOIN << applyName << playerName;
	pKernel->SendPublicMessage(msg);

	return 0;
}

// ���������Ա�б�
int GuildModule::OnCustomGetGuildMemberList(IKernel *pKernel, const PERSISTID &self, const IVarList &args)
{
	const wchar_t* guildName = GetPlayerGuildName(pKernel, self);
	if (!IsWidStrValid(guildName))
	{
		return 0;
	}
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}

	IRecord *memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (memberRecord == NULL)
	{
		return 0;
	}

	CVarList msg;
	CVarList memberArray;
	//int64_t curTime = ::time(NULL);
	int rows = memberRecord->GetRows();
	int curBath = 0; // ��ǰ����
	// ��0����
	if (SEND_MEMBER_COUNT == 0)
	{
		return 0;
	}

	IRecord * pOnlinePlayerRec = FriendModule::m_pFriendModule->GetPubOnlineRec(pKernel);
	if (NULL == pOnlinePlayerRec)
	{
		return 0;
	}


	int totalBath = rows % SEND_MEMBER_COUNT != 0 ?
		((rows / SEND_MEMBER_COUNT) + 1) : rows / SEND_MEMBER_COUNT;// ������

	// ѭ������
	LoopBeginCheck(do);
	for (int i = 0, j = 0; i < rows;)
	{
		LoopDoCheck(do);
		memberArray.Clear();
		// ѭ������
		LoopBeginCheck(dp);
		for(j = 0; j < SEND_MEMBER_COUNT && i < rows; ++i, ++j)
		{
			LoopDoCheck(dp);



			int guildPosition = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_POSITION);
			const wchar_t *name = memberRecord->QueryWideStr(i, GUILD_MEMBER_REC_COL_NAME);
			int sex     = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_SEX);
			int level   = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_LEVEL);
			int career  = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_CAREER);
			int maxContrib = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_DEVOTE);
			int fight   = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_FIGHT_ABILITY);	// ս����
			int online  = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_ONLINE);	// 0�����ߣ�1����
			int64_t lastOfflineTime = memberRecord->QueryInt64(i, GUILD_MEMBER_REC_COL_OUTLINE_DATE);
			int offlinehours = GetPassMinutes(lastOfflineTime)/60; // ��������ߣ���ʾ����ʱ�䣬��СʱΪ��λ
			int vipLv = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_VIP_LEVEL);
			int teamID = 0;

			const char * pPlayerUID = memberRecord->QueryString(i, GUILD_MEMBER_REC_COL_NAME_UID);
			int nRowsIndex = pOnlinePlayerRec->FindString(PUB_COL_PLAYER_UID, pPlayerUID);
			if (nRowsIndex >= 0)
			{
				teamID = pOnlinePlayerRec->QueryInt(nRowsIndex, PUB_COL_PLAYER_TEAM_ID);
			}



			memberArray << guildPosition << name << sex << level << career 
				<< maxContrib << fight << online << offlinehours << teamID << vipLv;
		}

		msg.Clear();
		msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_MEMBER_LIST << totalBath << curBath++ << j;
		pKernel->Custom2(self, msg, memberArray);
	}

	return 0;
}

// ����������������б�
int GuildModule::OnCustomGetGuildApplyList(IKernel *pKernel, const PERSISTID &self, const IVarList &args)
{
	// ����Ƿ���Ȩ��
	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_RATIFY))
	{
		return 0;
	}
	const wchar_t* guildName = GetPlayerGuildName(pKernel, self);
	if (!IsWidStrValid(guildName))
	{
		return 0;
	}
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}

	IRecord *joinRecord = pGuildData->GetRecord(GUILD_JOIN_REC);
	if (joinRecord == NULL)
	{
		return 0;
	}

	CVarList applyArray;
	int rows = joinRecord->GetRows();
	// ѭ������
	LoopBeginCheck(dq);
	for (int i = 0; i < rows; ++i)
	{
		LoopDoCheck(dq);
		const wchar_t *name = joinRecord->QueryWideStr(i, GUILD_JOIN_REC_COL_PLAYER_NAME);	// ��Ա����
		int sex     = joinRecord->QueryInt(i, GUILD_JOIN_REC_COL_SEX);	// 0Ů��1��
		int level   = joinRecord->QueryInt(i, GUILD_JOIN_REC_COL_LEVEL);
		int career  = joinRecord->QueryInt(i, GUILD_JOIN_REC_COL_CAREER);
		int fight   = joinRecord->QueryInt(i, GUILD_JOIN_REC_COL_FIGHT_ABILITY);	// ս����
		int online_ = joinRecord->QueryInt(i, GUILD_JOIN_REC_COL_ONLINE);
		applyArray << name << sex << level << career << fight << online_;
	}

	CVarList msg;
	msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_APPLY_LIST << rows;
	pKernel->Custom2(self, msg, applyArray);

	return 0;
}

// �����޸Ĺ�������
int GuildModule::OnCustomChangeNotice(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	const wchar_t* text = args.WideStrVal(2);

	if (!IsWidStrValid(text) || wcslen(text) > GUILD_NOTICE_MAX_LENGTH)
	{
		return 0;
	}

	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_ANNOUNCEMENT))
	{
		// ��Ȩ��������
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17625, CVarList());
		return 0;
	}

	// ����Ƿ�����Ƿ��ַ�
	if (!pKernel->CheckName(text))
	{
		// ��ʾ�����Ƿ��ַ�
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17626, CVarList());
		return 0;
	}




	const wchar_t* playerName  = pSelfObj->QueryWideStr("Name");
	const char* pPlayerNameUid = pKernel->SeekRoleUid(playerName);
	const wchar_t* guildName   = pSelfObj->QueryWideStr("GuildName");
	if (GetGuildNumValue(pKernel,guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL) < EnvirValueModule::EnvirQueryInt(ENV_VALUE_CHANGE_ANNOUNCEMENT))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17627, CVarList());
		return 0;
	}

	CVarList msg;
	msg << PUBSPACE_GUILD 
		<< guildName 
		<< SP_GUILD_MSG_CHANGE_NOTICE 
		<< pPlayerNameUid
		<< playerName
		<< text;
	pKernel->SendPublicMessage(msg);

	return 0;
}

// �����޸�����
int GuildModule::OnCustomChangeDeclaration(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	// ��ȡ��Ҷ���
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// ��ȡ����
	const wchar_t* text = args.WideStrVal(2);

	// ���ԺϷ����
	if (!IsWidStrValid(text) || wcslen(text) > GUILD_DECLARATION_MAX_LENGTH)
	{
		return 0;
	}

	// ����Ȩ�޼��
	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_ANNOUNCEMENT))
	{
		// ��Ȩ��������
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17625, CVarList());
		return 0;
	}

	// ����Ƿ�����Ƿ��ַ�
	if (!pKernel->CheckName(text))
	{
		// ��ʾ�����Ƿ��ַ�
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17628, CVarList());
		return 0;
	}

	// �۳�Ԫ��
// 	int nGold_ = EnvirValueModule::EnvirQueryInt(ENV_VALUE_GUILD_DECLAREATION_MONDIY_COST);
// 	if (nGold_ > 0)
// 	{
// 		if (DC_SUCCESS != 
// 			CapitalModule::m_pCapitalModule->DecCapital(pKernel, self, CAPITAL_SILVER, nGold_, FUNCTION_EVENT_ID_GUILD_DECLAR_MONDIFY))
// 		{
// 			// �ʽ���
// 			CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_7, CVarList());
// 			return 0;
// 		}
// 	}
// 	else
// 	{
// 		// δ����ֱ�ӷ����ʽ���
// 		CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_7, CVarList());
// 		return 0;
// 	}

	const wchar_t* playerName  = pSelfObj->QueryWideStr("Name");
	const char* pPlayerNameUid = pKernel->SeekRoleUid(playerName);
	const wchar_t* guildName   = pSelfObj->QueryWideStr("GuildName");

	CVarList msg;
	msg << PUBSPACE_GUILD 
		<< guildName 
		<< SP_GUILD_CHANGE_DECLARATION 
		<< pPlayerNameUid
		<< playerName
		<< text;
	pKernel->SendPublicMessage(msg);

	return 0;
}


// �����˳�����
int GuildModule::OnCustomQuit(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
	const char* pPlayerNameUid = pKernel->SeekRoleUid(playerName);
	const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
	if (StringUtil::CharIsNull(guildName))
	{
		return 0;
	}

	// �����˳����Ṧ�ܿ����Ƿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_GUILD_LEAVE, self))
	{
		return 0;
	}

	CVarList msg;
	msg << PUBSPACE_GUILD 
		<< guildName
		<< SP_GUILD_MSG_QUIT
		<< pPlayerNameUid
		<< playerName;
	pKernel->SendPublicMessage(msg);

	return 0;
}

// �����߳�����
int GuildModule::OnCustomFire(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	const wchar_t* memberName = args.WideStrVal(2);
	if (!CheckCanFire(pKernel, self, memberName))
	{
		return 0;
	}

	const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
	const wchar_t* guildName  = pSelfObj->QueryWideStr("GuildName");
	const char* pPlayerNameUid = pKernel->SeekRoleUid(playerName);
	const char* pMemberNameUid = pKernel->SeekRoleUid(memberName);

	CVarList msg;
	msg << PUBSPACE_GUILD 
		<< guildName
		<< SP_GUILD_MSG_FIRE
		<< pPlayerNameUid
		<< playerName
		<< pMemberNameUid
		<< memberName;
	pKernel->SendPublicMessage(msg);

	return 0;
}

// �����ɢ����
int GuildModule::OnCustomDismiss(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	if (!CheckCanDismiss(pKernel, self))
	{
		return false;
	}

	const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
	const char* pPlayerNameUid = pKernel->SeekRoleUid(playerName);

	CVarList msg;
	msg << PUBSPACE_GUILD 
		<< pSelfObj->QueryWideStr("GuildName") 
		<< SP_GUILD_MSG_DISMISS
		<< pPlayerNameUid
		<< playerName;
	pKernel->SendPublicMessage(msg);

	return 0;
}

// ���󹫻����
int GuildModule::OnCustomDonate(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	int donateValue = args.IntVal(2);

	const wchar_t *guildName = pSelfObj->QueryWideStr("GuildName");
	if (!IsWidStrValid(guildName))
	{
		return 0;
	}


	int donateTime = pSelfObj->QueryInt("GuildDonateTimes");
	if (donateTime > 0){ return 0; }
	const wchar_t *playerName = pSelfObj->QueryWideStr("Name");
	const char* playerUid = pKernel->SeekRoleUid(playerName);
	//���󹫻���׹��ܿ����Ƿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_GUILD_DONATE, self))
	{
		return 0;
	}

	// ��ð�ṱ����Ϣ

	GuildDonateConfigVector::iterator iter(m_GuildDonateConfig.begin());
	// ѭ������

	GuildDonateConfig_t *config = &(*iter);

	if (config == NULL)
	{
		return 0;
	}
	if (config->m_CapitalNumber > donateValue){
		return 0;
	}

	


	//������֯�ʽ�	
	int addSelfGuildCurrency = (int)(donateValue*config->m_RewardPlayerDonate / (float)config->m_CapitalNumber);
	//��֯�ʽ�
	int addGuildCapital = (int)(donateValue*config->m_GuildDonate / (float)config->m_CapitalNumber);


// 	if (!m_pCapitalModule->IsDailyCanAddCapital(pKernel, self, CAPITAL_GUILD_CURRENCY))
// 	{
// 		CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE,
// 			GUILD_CUSTOM_STRING_106, CVarList());
// 		return 0;
// 	}



// 	__int64 capitalSilver = m_pCapitalModule->GetCapital(pKernel, self, CAPITAL_TRADE_SILVER);
// 	if (capitalSilver < donateValue)
// 	{
// 		CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE,
// 			GUILD_CUSTOM_STRING_96, CVarList());
// 		return 0;
// 	}
// 
// 
// 	if (!m_pCapitalModule->CanIncCapital(pKernel, self, CAPITAL_GUILD_CURRENCY, addSelfGuildCurrency))
// 	{
// 		CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE,
// 			GUILD_CUSTOM_STRING_106, CVarList());
// 		return 0;
// 	}
// 
// 
// 
// 	int rc = m_pCapitalModule->DecCapital(pKernel, self, CAPITAL_TRADE_SILVER,
// 		(__int64)donateValue, FUNCTION_EVENT_ID_GUILD_DONATE);
// 	if (rc != DC_SUCCESS)
// 	{
// 		return 0;
// 	}
// 
// 	m_pCapitalModule->IncCapital(pKernel, self, CAPITAL_GUILD_CURRENCY, addSelfGuildCurrency, FUNCTION_EVENT_ID_GUILD_DONATE);

	// ������Ϣ������������
	CVarList msg;
	msg << PUBSPACE_GUILD
		<< guildName
		<< SP_GUILD_MSG_DONATE
		<< playerName
		<< playerUid
		<< addGuildCapital
		<< donateValue
		<< addSelfGuildCurrency;
	pKernel->SendPublicMessage(msg);

	return 0;
}

// ���󹫻�ְλ���
int GuildModule::OnCustomPromotion(IKernel* pKernel, const PERSISTID& self, const IVarList& args, int type)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	const wchar_t *memberName = args.WideStrVal(2);
	int newPosition = args.IntVal(3);
	const wchar_t *guildName = pSelfObj->QueryWideStr("GuildName");
	if (!IsWidStrValid(guildName))
	{
		return 0;
	}

	// �����Ƿ����
	IRecord *guildListRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
	if (guildListRecord == NULL)
	{
		return 0;
	}
	int guildRow = guildListRecord->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		return 0;
	}

	// �Ƿ񹫻��Ա
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}
	IRecord *memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (memberRecord == NULL)
	{
		return 0;
	}
	const wchar_t *selfName = pSelfObj->QueryWideStr("Name");
	int selfRow = memberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, selfName);
	if (selfRow < 0)
	{
		return 0;
	}
	int memberRow = memberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, memberName);
	if (memberRow < 0)
	{
		return 0;
	}

	int selfPosition = memberRecord->QueryInt(selfRow, GUILD_MEMBER_REC_COL_POSITION);
	int oldPosition = memberRecord->QueryInt(memberRow, GUILD_MEMBER_REC_COL_POSITION);
	if (0 > oldPosition || oldPosition >= GUILD_POSITION_COUNT)
	{
		return 0;
	}

	
	int pubMsgId = 0;
	
	switch (type)
	{
	case CS_GUILD_PROMOTION:
		//newPosition = oldPosition-1;
		pubMsgId = SP_GUILD_MSG_PROMOTION;
		
		break;
	case CS_GUILD_DEMOTION:
		//newPosition = oldPosition+1;
		pubMsgId = SP_GUILD_MSG_DEMOTION;
		
		break;
	default:
		return 0;
	}

	if (0 > newPosition || newPosition >= GUILD_POSITION_COUNT)
	{
		return 0;
	}

	// ����Ƿ��а��������Ȩ��
	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_SET_POSITION))
	{
		// ��Ȩ����
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17629, CVarList());
		return 0;
	}

	// �����������Լ�ְλ�ߵĳ�Ա
	if (selfPosition >= oldPosition)
	{
		return 0;
	}
	// �����������Լ�ְλ�ߵ�ְλ
	if (selfPosition >= newPosition)
	{
		return 0;
	}

	// ���͹������������д���
	CVarList msg;
	msg << PUBSPACE_GUILD
		<< guildName
		<< pubMsgId
		<< selfName
		<< memberName
		<< selfPosition
		<< oldPosition
		<< newPosition;
	pKernel->SendPublicMessage(msg);

	return 0;
}

// ���󹫻��̵��б�
int GuildModule::OnCustomGetShopList(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}


	// ���������̵깦�ܿ����Ƿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_GUILD_SHOP, self))
	{
		return 0;
	}

	// �ҵ���ᵱǰ�ȼ�
	const wchar_t *guildName = pSelfObj->QueryWideStr("GuildName");
	const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
	const char* playerUid = pKernel->SeekRoleUid(playerName);
	if (StringUtil::CharIsNull(guildName) 
		|| StringUtil::CharIsNull(playerName)
		|| StringUtil::CharIsNull(playerUid))
	{
		return 0;
	}
	IRecord *guildList = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_LIST_REC);
	if (guildList == NULL)
	{
		return 0;
	}
	int guildRow = guildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		return 0;
	}
	int shangdianLevel = GuildBuildingLevel(pKernel, guildName,GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_SHANG_DIAN);

	int xiangFangLevel = GuildBuildingLevel(pKernel, guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);
	GuildUpLevelConfig_t *upLevelCfg = &m_GuildUpLevelConfig[xiangFangLevel];
    if(upLevelCfg == NULL)
    {
        return 0;
    }

	IPubData *pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}
	IRecord *shopBuyRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_SHOP_BUY_REC);
	if (shopBuyRecord == NULL)
	{
		return 0;
	}
	IRecord *memberBuyRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_MEMBER_BUY_REC);
	if (memberBuyRecord == NULL)
	{
		return 0;
	}
	IRecord *memberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (memberList == NULL)
	{
		return 0;
	}
	int memberRow = memberList->FindWideStr(GUILD_MEMBER_REC_COL_NAME, playerName);
	if (memberRow < 0)
	{
		return 0;
	}
	int position = memberList->QueryInt(memberRow, GUILD_MEMBER_REC_COL_POSITION);


	// ���ȷ����Ѿ������ĵ���
	int shopItemCount = 0;
	CVarList shopItemList;
	GuildShopConfigVector::iterator iter(m_GuildShopConfig.begin());
	// ѭ������
	LoopBeginCheck(ds);
	for ( ; iter != m_GuildShopConfig.end(); ++iter)
	{
		LoopDoCheck(ds);
		if (iter->m_UnlockLevel > shangdianLevel)
		{
			break;
		}

		int guildTotal = (int)iter->m_GuildTotal;
		std::wstring itemName = guildName + ::util_int64_as_widestr(pKernel->GetServerId()) 
			+ StringUtil::StringAsWideStr(iter->m_ItemId.c_str()) + L"_" + StringUtil::IntAsWideStr(iter->m_Index);
		int shopBuyRow = shopBuyRecord->FindWideStr(GUILD_SHOP_BUY_REC_COL_ITEMID, itemName.c_str());
		if (shopBuyRow >= 0)
		{
			int count = shopBuyRecord->QueryInt(shopBuyRow, GUILD_SHOP_BUY_REC_COL_BUY_COUNT);
			guildTotal -= count;
		}

		int memberLimit = GetGuildShopMemberLimit(position, iter->m_MemberLimit);
		std::string memberBuyName = std::string(playerUid) + iter->m_ItemId + "_" + StringUtil::IntAsString(iter->m_Index);
		int memberBuyRow = memberBuyRecord->FindString(GUILD_MEMBER_BUY_REC_COL_UID_ITEMID, memberBuyName.c_str());
		if (memberBuyRow >= 0)
		{
			int count = memberBuyRecord->QueryInt(memberBuyRow, GUILD_MEMBER_BUY_REC_COL_BUY_COUNT);
			memberLimit -= count;
		}

		shopItemList << iter->m_Index << iter->m_ItemId<< (std::max)(guildTotal, 0) << (std::max)(memberLimit, 0) ;
		shopItemCount++;
	}


	CVarList rmsg;
	rmsg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_SHOP_LIST << shopItemCount;
	pKernel->Custom2(self, rmsg, shopItemList);
	return 0;
}

// ������빫���CD
int GuildModule::OnCustomGetJoinCD(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	int leftMinutes = 0;
	int64_t quitGuildDate = pSelfObj->QueryInt64("QuitGuildDate");
	if (quitGuildDate > 0)
	{
		int passMinutes = GetPassMinutes(quitGuildDate);
		leftMinutes = EnvirValueModule::EnvirQueryInt(ENV_VALUE_ADD_GUILD_CD) - passMinutes;
		// Ϊ���ֲ����ÿͻ��˵���ʱ��ʱ���ȽϿ�����⣬�����CDʱ�䲢����60�����ڣ��ٶ��10����
		//leftMinutes = (leftMinutes > 0 && leftMinutes < 60) ? (leftMinutes+10) : leftMinutes;
	}

	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_JOIN_CD << leftMinutes);

	return 0;
}

// �������̵���Ʒ
int GuildModule::OnCustomBuyItem(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	int index = args.IntVal(2);
	int itemNum = args.IntVal(3);
	if (itemNum <= 0) 
	{
		return 0;
	}

	

	// ���������̵깦�ܿ����Ƿ���
	if (!SwitchManagerModule::CheckFunctionEnable(pKernel, SWITCH_FUNCTION_GUILD_SHOP, self))
	{
		return 0;
	}

	const wchar_t *guildName = pSelfObj->QueryWideStr("GuildName");
	const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
	const char* playerUid = pKernel->SeekRoleUid(playerName);
	if (StringUtil::CharIsNull(guildName)
		|| StringUtil::CharIsNull(playerName)
		|| StringUtil::CharIsNull(playerUid))
	{
		return 0;
	}

	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}
	// ����Ƿ���һԱ
	IRecord *memberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (memberList == NULL)
	{
		return 0;
	}
	int memberRow = memberList->FindWideStr(GUILD_MEMBER_REC_COL_NAME, playerName);
	if (memberRow < 0)
	{
		return 0;
	}
	int position = memberList->QueryInt(memberRow, GUILD_MEMBER_REC_COL_POSITION);
	

	const GuildShopConfigItem_t *shopItem = GetShopItemInfo(index);
	if (shopItem == NULL)
	{
		return 0;
	}
	const char* itemID = shopItem->m_ItemId.c_str();

	int shangDianLv = GuildBuildingLevel(pKernel,guildName,GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_SHANG_DIAN);

	// �Ƿ����
	if (shopItem->m_UnlockLevel > shangDianLv)
	{
		return 0;
	}


	////�̵깺����Ʒ��û�п�Ǯ
	int memberBuyTotal = GetGuildShopMemberLimit(position, shopItem->m_MemberLimit);
	int shopBuyedTotal = shopItem->m_GuildTotal;

	//// ���ʣ����������Ƿ��㹻
	IRecord *shopBuyRecord = GetPubRecord(pKernel, GUILD_PUB_DATA, GUILD_SHOP_BUY_REC);
	if (shopBuyRecord == NULL)
	{
		return 0;
	}
	int shopBuyCount = 0;
	std::wstring guildDataName = guildName + ::util_int64_as_widestr(pKernel->GetServerId());
	std::wstring shopBuyId = guildDataName + StringUtil::StringAsWideStr(itemID) + L"_" + StringUtil::IntAsWideStr(index);
	int shopBuyRow = shopBuyRecord->FindWideStr(GUILD_SHOP_BUY_REC_COL_ITEMID, shopBuyId.c_str());
	if (shopBuyRow >= 0)
	{
		shopBuyCount = shopBuyRecord->QueryInt(shopBuyRow, GUILD_SHOP_BUY_REC_COL_BUY_COUNT);
	}
	if ((shopBuyedTotal - shopBuyCount) < itemNum)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17630, CVarList());
		return 0;
	}

	////���ѹ����� 
	for (auto it : shopItem->m_decCapital)
	{
		if (!CapitalModule::m_pCapitalModule->CanDecCapital(pKernel, self, it.first, it.second*itemNum))
		{
			//CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_21, CVarList());
			return 0;
		}
	}


	//// �������Ƿ��㹻
	const PERSISTID toolbox = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
	if (!pKernel->Exists(toolbox))
	{
		return 0;
	}
	int nBindState = FunctionEventModule::GetItemBindState(FUNCTION_EVENT_ID_GUILD_SHOP_BUY);
	int putCnt = ContainerModule::TryAddItem(pKernel, toolbox, itemID, itemNum, nBindState);
	if (putCnt < itemNum)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_7004, CVarList());
		return 0;
	}
	//�۳�

	for (auto it : shopItem->m_decCapital)
	{
		if (!CapitalModule::m_pCapitalModule->DecCapital(pKernel, self, it.first, it.second*itemNum,FUNCTION_EVENT_ID_GUILD_SHOP_BUY ))
		{
			return 0;
		}
	}
	


	//// ��������Ʒ
	CVarList msg;
	msg << PUBSPACE_GUILD 
		<< guildName
		<< SP_GUILD_MSG_BUY_ITEM 
		<< playerUid
		<< playerName
		<< index
		<< itemID
		<< itemNum
		<< shopItem->m_addGuildCapital * itemNum
		<< shopBuyedTotal
		<< memberBuyTotal;

	pKernel->SendPublicMessage(msg);

	return 0;
}


// �����ƽ�����֮λ
int GuildModule::OnCustomCaptainTransfer(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	const wchar_t *newCaptainName = args.WideStrVal(2);
	if (StringUtil::CharIsNull(newCaptainName))
	{
		return 0;
	}

	const wchar_t *selfName = pSelfObj->QueryWideStr("Name");
	if (!CheckIsCaptain(pKernel, self)) 
	{
		return 0;
	}
	const char* playerUid = pKernel->SeekRoleUid(selfName);

	// ���newCaptainName�Ƿ񱾰���Ա
	const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
	if (!IsWidStrValid(guildName))
	{
		return 0;
	}
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}
	IRecord* pMemberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pMemberRecord == NULL)
	{
		return 0;
	}
	int memberRow = pMemberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, newCaptainName);
	if (memberRow < 0)
	{
		return 0;
	}
	int position = pMemberRecord->QueryInt(memberRow, GUILD_MEMBER_REC_COL_POSITION);
	if (position == GUILD_POSITION_CAPTAIN)
	{
		return 0;
	}

	CVarList msg;
	msg << PUBSPACE_GUILD 
		<< guildName
		<< SP_GUILD_MSG_CAPTAIN_TRANSFER 
		<< playerUid
		<< selfName
		<< newCaptainName;
	pKernel->SendPublicMessage(msg);

	return 0;
}

// ��������־
int GuildModule::OnCustomGetLog(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	// ��ȡ��Ҷ���
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// �������
	const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
	if (StringUtil::CharIsNull(guildName))
	{
		return 0;
	}

	// ��ȡ��ṫ���ռ�
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}

	// ��ȡ�����־��
	IRecord* pLogRecord = pGuildData->GetRecord(GUILD_LOG_REC);
	if (pLogRecord == NULL)
	{
		return 0;
	}
	
	// ��ʼ����
	
	int type = args.IntVal(2);
	int count = pLogRecord->GetRows();
	// ����
	CVarList list;
	// ѭ������
	LoopBeginCheck(a);
	// ȡ����
	for (int i = 0; i < count; ++i)
	{
		LoopDoCheck(a);
		int typeRow = pLogRecord->QueryInt(i, GUILD_LOG_COL_type);
		if (type != -1 && typeRow != type){
			continue;
		}

		int64_t time = pLogRecord->QueryInt64(i, GUILD_LOG_COL_time);
		const wchar_t* args = pLogRecord->QueryWideStr(i, GUILD_LOG_COL_args);
		if (args == NULL)
		{
			args = L"";
		}
		list << typeRow << time << args;
	}

	// �����ݸ��ͻ���
	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_LOG << count << list);

	return 0;
}

// ���������־
int GuildModule::OnCustomGetDonateLog(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	// ��ȡ��Ҷ���
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// �������
	const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
	if (StringUtil::CharIsNull(guildName))
	{
		return 0;
	}

	// ��ȡ��ṫ���ռ�
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return 0;
	}

	// ��ȡ�����־��
	IRecord* pLogRecord = pGuildData->GetRecord(GUILD_LOG_REC);
	if (pLogRecord == NULL)
	{
		return 0;
	}

	// ������
	int count = pLogRecord->GetRows();
	// ������������
	int donateCount = 0;
	// ����
	CVarList list;
	// ѭ������
	LoopBeginCheck(b);
	for (int i = count - 1; i >= 0; --i)
	{
		LoopDoCheck(b);

		// �����������
		int type = pLogRecord->QueryInt(i, GUILD_LOG_COL_type);
		int64_t time = pLogRecord->QueryInt64(i, GUILD_LOG_COL_time);
		const wchar_t* args = pLogRecord->QueryWideStr(i, GUILD_LOG_COL_args);
		if (args == NULL)
		{
			args = L"";
		}

		// �������
		list << type << time << args;

		// �ж��Ƿ�ȡ��
		if (++donateCount > GUILD_LOG_DONATE_COUNT)
		{
			break;
		}
	}

	// �����ݸ��ͻ���
	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_LOG_DONATE << donateCount << list);

	return 0;
}



//У���Ƿ񹫻��ڼ�
bool GuildModule::CheckGuildActivity(IKernel* pKernel, const PERSISTID& self)
{
	// ����ػ��
	//if (GuildDefendModule::m_pGuildDefendModule->IsInGuildDefendTime() 
	//	|| GuildDineModule::IsDineTime(pKernel))		// ���۲ͻ
	//{
	//	CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_34, CVarList());
	//	return false;
	//}

	//�����Ա�˳�����Ƿ�Ϸ�,�����˳�������,��ɢ����
// 	if (!MiddleModule::Instance()->CheckGuildMemberQuit(pKernel,self))
// 	{
// 		return false;
// 	}
	return true;
}


void GuildModule::OnCustomSetAutoAgree(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return ;
	}
	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_AUTO_AGREE))
	{
		// ��Ȩ����
		return ;
	}
	//����Լ��Ƿ��ǰ���Ա
	const wchar_t *selfName = pSelfObj->QueryWideStr("Name");
	const char* playerUid = pKernel->SeekRoleUid(selfName);
	const wchar_t* guildName = pSelfObj->QueryWideStr("GuildName");
	if (!IsWidStrValid(guildName))
	{
		return ;
	}
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return ;
	}
	IRecord* pMemberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pMemberRecord == NULL)
	{
		return ;
	}
	int memberRow = pMemberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, selfName);
	if (memberRow < 0)
	{
		return;
	}
	

	CVarList msg;
	msg << PUBSPACE_GUILD
		<< guildName
		<< SP_GUILD_SET_AUTO_AGREE_STATE
		<<args.IntVal(2)
		<<args.IntVal(3)
		<<args.IntVal(4)
		<<args.IntVal(5);
	pKernel->SendPublicMessage(msg);

}
void GuildModule::OnCustomUnLockSkill(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return;
	}

	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_UNLOCK_SKILL))
	{

		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17631, CVarList());
		return;
	}

	const wchar_t * guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(guildName))
	{
		return;
	}
	const wchar_t* playerName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);

	int skillID = args.IntVal(2);


	auto it = m_guildSkill.find(skillID);
	if (it == m_guildSkill.end())
	{
		return;
	}

	int row;
	IRecord* pGuildRecord = m_pGuildModule->GetGuildRecord(pKernel, guildName, row);
	if (pGuildRecord == NULL){
		return;
	}
	if (it->second.m_autoUnlock){
		return;
	}

	int shuYuanLv = GuildBuildingLevel(pKernel,guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_SHU_YUAN);
	if (it->second.m_unlockShuYuanLevel > shuYuanLv){
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17632, CVarList());
		return;
	}

	int guildCapital = GetGuildNumValue(pKernel,guildName,GUILD_NUM_TYPE::GUILD_NUM_CAPITAL);
	if (guildCapital < it->second.m_unlockCost)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17633, CVarList());
		return;
	}

	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return;
	}

	IRecord* pGuildPubSkillRecord = pGuildData->GetRecord(GUILD_PUB_SKILL_REC);
	if (pGuildPubSkillRecord == NULL)
	{
		return;
	}

	int skillRow = pGuildPubSkillRecord->FindInt(GUILD_PUB_SKILL_REC_COL_SKILLID, skillID);
	if (skillRow >= 0)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17634, CVarList());
		return;
	}

	CVarList msg;
	msg << PUBSPACE_GUILD
		<< guildName
		<< SP_GUILD_UNLOCK_SKILL
		<< skillID
		<< playerName;
	pKernel->SendPublicMessage(msg);

}


void GuildModule::OnCustomGetSkillList(IKernel*pKernel, const wchar_t*guildName, CVarList&var)
{
	
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return;
	}

	IRecord* pSkillRecord = pGuildData->GetRecord(GUILD_PUB_SKILL_REC);
	if (pSkillRecord == NULL)
	{
		return;
	}

	for (int row = 0; row < pSkillRecord->GetRows(); row++)
	{
		CVarList info;
		pSkillRecord->QueryRowValue(row, info);
		var << info;
	}
		

}

void GuildModule::OnCustomSkillLevelUp(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return;
	}
	const wchar_t * guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(guildName))
	{
		return;
	}
	const wchar_t* playerName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
	const int skillID = args.IntVal(2);
	auto  it = m_guildSkill.find(skillID);
	if (it == m_guildSkill.end())
	{
		return;
	}


	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL)
	{
		return;
	}

	IRecord* pGuildPubSkillRecord = pGuildData->GetRecord(GUILD_PUB_SKILL_REC);
	if (pGuildPubSkillRecord == NULL)
	{
		return;
	}

	int skillRow = pGuildPubSkillRecord->FindInt(GUILD_PUB_SKILL_REC_COL_SKILLID, skillID);
	if (skillRow < 0)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17635, CVarList());
		return;
	}
	int skillLimitLv = pGuildPubSkillRecord->QueryInt(skillRow, GUILD_PUB_SKILL_REC_COL_LEVEL);


	// �ҵ��������ܱ�
	IRecord* pPassiveSkillRec = pSelfObj->GetRecord(PASSIVE_SKILL_REC_NAME);
	if (NULL == pPassiveSkillRec)
	{
		return;
	}
	// û�ҵ�����
	int nRows = pPassiveSkillRec->FindInt(PS_COL_SKILL_ID, skillID);
	if (-1 == nRows)
	{
		return;
	}


	// Ĭ��Ϊֻ��һ��
	int nOldSkillLevel = pPassiveSkillRec->QueryInt(nRows, PS_COL_SKILL_LEVEL);

	if (nOldSkillLevel >= skillLimitLv){
		return;
	}

	int nNewSkillLevel = nOldSkillLevel + 1;
	auto its = m_guildSkillLevelUpCost.find(it->second.m_skillType);

	if (its == m_guildSkillLevelUpCost.end()){
		return;
	}

	auto itt = its->second.find(nOldSkillLevel);
	if (itt == its->second.end()){
		return;
	}
	// ���Ҳ���
	for (auto itf : itt->second.m_vCurrency)
	{
		if (!CapitalModule::m_pCapitalModule->CanDecCapital(pKernel, self, itf.first, itf.second) )
		{
			//::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_82, CVarList());
			return;
		}
	}

	// ���ұ���
	PERSISTID itemBox = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
	if (!pKernel->Exists(itemBox))
	{
		return;
	}
	// ��Ʒ�Ƿ��㹻
	if (!ContainerModule::m_pContainerModule->TryRemoveItems(pKernel, itemBox, itt->second.m_vItems))
	{
		return;
	}


	// ��ҵȼ�
	int player_level = pSelfObj->QueryInt("Level");
	
	// �ķ��ȼ����ܸ�������
	if (nNewSkillLevel > player_level)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17636, CVarList());
		return;
	}

	// �۳�����
	for (auto itf : itt->second.m_vCurrency)
	{
		CapitalModule::m_pCapitalModule->DecCapital(pKernel, self, itf.first, itf.second, FUNCTION_EVENT_ID_GUILD_SKILL);
	}
	// �۳���Ʒ
	ContainerModule::m_pContainerModule->RemoveItems(pKernel, itemBox, itt->second.m_vItems, NULL, FUNCTION_EVENT_ID_GUILD_SKILL);

	m_pGuildModule->GuildSkillLevelUp(pKernel, self, skillID, nNewSkillLevel);

	/*CVarList msg;
	msg << PUBSPACE_GUILD
		<< guildName
		<< SP_GUILD_SKILL_LEVEL_UP
		<< skillID
		<< nCostDonate
		<< playerName
		<< nNewSkillLevel;
	pKernel->SendPublicMessage(msg);*/
}





void GuildModule::OnGuildBuildingOperateLv(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{
	//Ȩ�޼��
	if (!m_pGuildModule->CheckPositionPriv(pKernel, self, GUILD_PRIV_BUILDING_LEVEL_UP)){
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17637, CVarList());
		return ;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}

	const wchar_t * guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(guildName)){
		return;
	}

	const wchar_t *playerName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
	int buildingType = args.IntVal(2);
	int operate = args.IntVal(3);
	int useMiBao = args.IntVal(4);
	CVarList pubMsg;
	pubMsg << PUBSPACE_GUILD << guildName << SP_GUILD_BUILDING_LVUP_OPERATE << operate << playerName << buildingType << useMiBao;
	pKernel->SendPublicMessage(pubMsg);



}

void GuildModule::AddGuildBuildingExp(IKernel *pKernel, const wchar_t*guildName, int addValue)
{

	if (StringUtil::CharIsNull(guildName)){
		return;
	}


	CVarList pubMsg;
	pubMsg << PUBSPACE_GUILD << guildName << SP_GUILD_BUILDING_LEVEL_ADD_EXP << addValue;
	pKernel->SendPublicMessage(pubMsg);

}

void GuildModule::OnGuildNumBuy(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{
	//Ȩ�޼��
	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_GUILD_NUM_BUY)){
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17637, CVarList());
		return;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}

	const wchar_t * guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(guildName)){
		return;
	}

	const wchar_t *playerName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
	int guildNumType = args.IntVal(2);


	CVarList pubMsg;
	pubMsg << PUBSPACE_GUILD << guildName << SP_GUILD_NUM_BUY << playerName << guildNumType;
	pKernel->SendPublicMessage(pubMsg);

}

void GuildModule::OnCustomChangeGuildShortName(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{

	const wchar_t *shortName = args.WideStrVal(2);
	const wchar_t *guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(guildName))
	{
		return;
	}
	const wchar_t* playerName = pKernel->QueryWideStr(self, FIELD_PROP_NAME);

	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_CHANGE_SHORT_NAME))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17637, CVarList());
		return;
	}

	if (GuildShortNameExist(pKernel, shortName))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17637, CVarList());
		return ;
	}

// 	if (GetGuildNumValue(pKernel, guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL) < EnvirValueModule::EnvirQueryInt(ENV_VALUE_CHANGE_SHORT_NAME))
// 	{
// 		CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_103, CVarList());
// 		return;
// 	}
// 
// 
// 	CVarList pubMsg;
// 	pubMsg << PUBSPACE_GUILD << guildName << SP_GUILD_CHANGE_SHORT_NAME << shortName << playerName;
// 	pKernel->SendPublicMessage(pubMsg);
}

void GuildModule::OnCustomGetJianKangDuGif(IKernel*pKernel, const PERSISTID& self)
{
	IGameObj*pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}


// 	const wchar_t *playerName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
// 	const wchar_t *guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
// 	int guildDailyGif = pSelfObj->QueryInt(FIELD_PROP_GUILD_DAILY_GIF);
// 	if (guildDailyGif > 0){
// 		CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_105, CVarList());
// 		return;
// 	}
// 	
// 
// 	CVarList pubMsg;
// 	pubMsg << PUBSPACE_GUILD << guildName << SP_GUILD_GET_JIAN_KANG_DU_GIF << playerName;
// 	pKernel->SendPublicMessage(pubMsg);
}

void GuildModule::OnCustomGuildSkillLvOperate(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{
	int skillID = args.IntVal(2);
	int operate = args.IntVal(3);
	const wchar_t *playerName = pKernel->QueryWideStr(self, FIELD_PROP_NAME);
	const wchar_t *guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_START_STUDY_SKILL))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17637, CVarList());
		return;
	}

	IRecord* pGuildPubSkillData = GetPubRecord(pKernel, guildName, GUILD_PUB_SKILL_REC);
	if (pGuildPubSkillData == NULL)
	{
		return;
	}
	int row = -1;
	if (operate == GUILD_OPERATE_START_LVUP){
		 row = pGuildPubSkillData->FindInt(GUILD_PUB_SKILL_REC_COL_STUDY_STATE, GUILD_PUB_SKILL_STUDY_STATE_STUDYING);
		if (row >= 0)
		{
			CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17638, CVarList());
			return;
		}
	}

	row = pGuildPubSkillData->FindInt(GUILD_PUB_SKILL_REC_COL_SKILLID,skillID);
	if (row < 0)
	{
		return;
	}



	CVarList pubMsg;
	pubMsg << PUBSPACE_GUILD << guildName << SP_GUILD_SKILL_LVUP_OPERATE << operate << skillID << playerName;
	pKernel->SendPublicMessage(pubMsg);



}
int GuildModule::OnCustomUseRareTreasure(IKernel*pKernel, const PERSISTID& self, int rareTreasureType, int useNum)
{
	if (useNum == 0){
		return 0;
	}
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return 0;
	}
	
	

	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_USE_RARE_TREASURE)){
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17637, CVarList());
		return 0 ;
	}
	const wchar_t*guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	

// 	CVarList pubMsg;
// 	pubMsg << PUBSPACE_GUILD << guildName << SP_GUILD_USE_RARE_TREASURE << pSelfObj->QueryWideStr(FIELD_PROP_NAME) << rareTreasureType << useNum;
// 	pKernel->SendPublicMessage(pubMsg);


	return 0;
}

int GuildModule::OnCustomGetRareTreasureList(IKernel*pKernel, const PERSISTID& self)
{
	const wchar_t *guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);

	if (StringUtil::CharIsNull(guildName)){
		return 0;
	}


	IRecord *pGuildRareTreasure = GetPubRecord(pKernel, guildName, GUILD_RARE_TREASURE_REC);
	if (pGuildRareTreasure == NULL)
	{
		return 0;
	}
	int count = pGuildRareTreasure->GetRows();
	CVarList var;
	for (int i = 0; i < count; i++)
	{
		CVarList info;
		pGuildRareTreasure->QueryRowValue(i, info);
		var << info;
	}

	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_RARE_TREASURE_LIST << count << var);

	return 0;
}


int GuildModule::OnCustomGetNumBuyRecord(IKernel*pKernel, const PERSISTID& self)
{
	const wchar_t* guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(guildName)){
		return 0;
	}

	IRecord * pGuildNumBuyRec = GetPubRecord(pKernel, guildName, GUILD_NUM_BUY_REC);
	if (pGuildNumBuyRec == NULL){
		return 0;
	}
	CVarList var;
	int buyTimes = 0;
	int count = pGuildNumBuyRec->GetRows();
	for (int i = 0; i < count; i++)
	{
		CVarList info;
		pGuildNumBuyRec->QueryRowValue(i,info);
		var << info;
	}

	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_NUM_BUY_RECORD << count << var);


	return 0;
}
int GuildModule::OnCustomGetBonusInfo(IKernel*pKernel, const PERSISTID& self)
{
	const wchar_t*guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(guildName)){ 
		return 0; 
	}

	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL){
		return 0;
	}


	int bonusValue = pGuildData->QueryAttrInt(GUILD_BONUS_VALUE);
	int bonusLockValue = pGuildData->QueryAttrInt(GUILD_BONUS_LOCK_VALUE);

	auto bonusTime = pGuildData->GetRecord(GUILD_BONUS_REC);
	if (NULL == bonusTime)
	{
		return 0;
	}

	CVarList bonusInfo;
	for (int i = 0; i < bonusTime->GetRows(); i++)
	{
		CVarList info;
		bonusTime->QueryRowValue(i, info);
		bonusInfo << info;
	}


	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_BONUS_INFO << bonusValue << bonusLockValue
		<< bonusTime->GetRows() << bonusInfo);


	return 0;
}



int GuildModule::OnCustomGetGuildWelfare(IKernel*pKernel, const PERSISTID& self)
{
	const wchar_t*guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(guildName)){
		return 0;
	}
	const wchar_t*playerName = pKernel->QueryWideStr(self, FIELD_PROP_NAME);
	IPubData* pGuildPubData = GetPubData(pKernel, GUILD_PUB_DATA);
	if (pGuildPubData == NULL){
		return 0;
	}

	

	IPubData* pGuilData = GetPubData(pKernel, guildName);
	if (pGuilData == NULL){
		return 0;
	}


	int dailyGetCapital = pGuilData->QueryAttrInt(DAILY_GET_GUILD_CAPITAL);
	int dailyGainCapital = 0;
	IRecord* pGainCapital = pGuilData->GetRecord(GUILD_GAIN_CAPITAL_REC);
	if (pGainCapital != NULL)
	{
		int row = pGainCapital->FindWideStr(GUILD_GAIN_CAPITAL_REC_COL_NAME, playerName);
		if (row >= 0)
		{
			dailyGainCapital = pGainCapital->QueryInt(row, GUILD_GAIN_CAPITAL_REC_COL_VALUE);
		}
	}

	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_WELFARE_INFO << dailyGetCapital << dailyGainCapital);
	return 0;
}


int GuildModule::OnCustomGiveOutBonus(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{
	
	IGameObj*pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return 0;
	}

		
// 	int giveOutValue = args.IntVal(3);
// 	int giveOutValueMax = EnvirValueModule::EnvirQueryInt(ENV_VALUE_GIVE_OUT_BONUS_MAX);
// 	if (giveOutValue > giveOutValueMax)
// 	{
// 		CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_112, CVarList());
// 		return 0;
// 	}
// 
// 
// 
// 
// 	const wchar_t*guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
// 	const wchar_t*playerName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
// 	if (!CheckPositionPriv(pKernel, self, GUILD_PRIV_GIVE_OUT_BONUS)){
// 		CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_109, CVarList());
// 		return 0;
// 	}
// 
// 	CVarList info;
// 	info.Append(args, 2, args.GetCount() - 2);
// 	CVarList pubMsg;
// 	pubMsg << PUBSPACE_GUILD << guildName << SP_GUILD_GIVE_OUT_BONUS << playerName << info;
// 	pKernel->SendPublicMessage(pubMsg);
	return 0;
}

int GuildModule::OnCustomInviteJoinGuild(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	//ȷ����Ұ��
	const wchar_t * guildName = pSelfObj->QueryWideStr("GuildName");
	if (StringUtil::CharIsNull(guildName))
	{
		return 0;
	}

	int position = pSelfObj->QueryInt("GuildPosition");
	if (position != GUILD_POSITION_CAPTAIN){
		return 0;
	}

	int guildJuYiTingLevel = GetGuildLevel(pKernel, guildName);
	int xiangFangLevel = GuildBuildingLevel(pKernel, guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);
	// ��ù�������Ա����
	GuildUpLevelConfig_t* upLevelConfig = GetGuildUpLevelConfig(xiangFangLevel);
	if (upLevelConfig == NULL)
	{
		return 0;
	}

	IRecord* pGuildMemberList = GetPubRecord(pKernel,guildName, GUILD_MEMBER_REC);
	if (pGuildMemberList == NULL)
	{
		return 0;
	}

	int curMember = pGuildMemberList->GetRows();
	

	if (curMember >= upLevelConfig->m_MaxMember)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17639, CVarList());
		return 0;
	}

	const wchar_t* inviteName = args.WideStrVal(2);
	if (StringUtil::CharIsNull(inviteName))
	{
		return 0;
	}

	if (pKernel->GetPlayerScene(inviteName) <= 0)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17640, CVarList());
		return 0;
	}

	pKernel->CustomByName(inviteName, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_INVITE_JOIN_GUILD<<pSelfObj->QueryWideStr(FIELD_PROP_NAME)<<guildName);
	pKernel->CommandByName(inviteName, CVarList() << COMMAND_GUILD_BY_NAME << SS_GUILD_INVITE_JOIN_GUILD << guildName);
	return 0;
}

int GuildModule::OnCustomAgreeInviteJoinGuild(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return 0;
	}
	
	const wchar_t*guildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	if (!StringUtil::CharIsNull(guildName)){
		return 0;
	}

	if (!pSelfObj->FindData(GUILD_INVITE_GUILD_NAME))
	{
		return 0 ;
	}

	const wchar_t* inviteGuildName	=  pSelfObj->QueryDataWideStr(GUILD_INVITE_GUILD_NAME);
	
	int guildJuYiTingLevel = GetGuildLevel(pKernel, inviteGuildName);
	int xiangFangLevel = GuildBuildingLevel(pKernel, inviteGuildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);
	// ��ù�������Ա����
	GuildUpLevelConfig_t* upLevelConfig = GetGuildUpLevelConfig(xiangFangLevel);
	if (upLevelConfig == NULL)
	{
		return 0;
	}

	IRecord* pGuildMemberList = GetPubRecord(pKernel, inviteGuildName, GUILD_MEMBER_REC);
	if (pGuildMemberList == NULL)
	{
		return 0;
	}

	int curMember = pGuildMemberList->GetRows();


	if (curMember >= upLevelConfig->m_MaxMember)
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17622, CVarList());
	}

	int findRow = pGuildMemberList->FindWideStr(GUILD_MEMBER_REC_COL_NAME, inviteGuildName);
	if (findRow >= 0)
	{
		return 0;
	}


	const wchar_t* playerName = pSelfObj->QueryWideStr("Name");
	const char* pPlayerUid = pKernel->SeekRoleUid(playerName);
	int playerSex = pSelfObj->QueryInt("Sex");
	int playerLevel = pSelfObj->QueryInt("Level");
	int ability = pSelfObj->QueryInt("BattleAbility");
	int career = pSelfObj->QueryInt("Job");
	int vipLevel = pSelfObj->QueryInt(FIELD_PROP_VIP_LEVEL);
	CVarList info;
	info << pPlayerUid << playerName
		<< playerSex << playerLevel << ability << career << vipLevel << upLevelConfig->m_MaxMember;
	CVarList pubMsg;
	pubMsg << PUBSPACE_GUILD << inviteGuildName << SP_GUILD_AGREE_INVITE_JOIN_GUILD << info;
	pKernel->SendPublicMessage(pubMsg);


	return 0;
}


void GuildModule::OnCustomAddPetition(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{
// 	int type = args.IntVal(2);
// 	IGameObj*pSelfObj = pKernel->GetGameObj(self);
// 	if (pSelfObj == NULL){
// 		return;
// 	}
// 
// 	const wchar_t *pGuildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
// 	if (StringUtil::CharIsNull(pGuildName)){
// 		return;
// 	}
// 
// 	const wchar_t* pSelfName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
// 	CVarList pubMsg;
// 	pubMsg << PUBSPACE_GUILD << pGuildName << SP_GUILD_ADD_PETITION << type << pSelfName;
// 	pKernel->SendPublicMessage(pubMsg);

}

void GuildModule::OnCustomGetPetition(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{
	int type = args.IntVal(2);

	IGameObj*pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}

	const wchar_t *pGuildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
	if (StringUtil::CharIsNull(pGuildName)){
		return;
	}

	IRecord* pGuildMemberList = GetPubRecord(pKernel, pGuildName, GUILD_PETITION_REC);
	if (pGuildMemberList == NULL)
	{
		return;
	}
	int count = 0;
	CVarList info;
	for (int row = 0; row < pGuildMemberList->GetRows(); row++)
	{
		
		int typeRow = pGuildMemberList->QueryInt(row, GUILD_PETITION_REC_COL_TYPE);
		if (type !=-1&&type != typeRow){ 
			continue; 
		}
		++count;
		const wchar_t* playerNameRow = pGuildMemberList->QueryWideStr(row, GUILD_PETITION_REC_COL_NAME);
		info << type << playerNameRow;
	}
	
	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_GET_PETTION_LIST << count << info);
}
void GuildModule::OnCustomGetOtherGuildInfo(IKernel*pKernel, const PERSISTID& self, const IVarList& args)
{
	const wchar_t* guildName = args.WideStrVal(2);
	if (StringUtil::CharIsNull(guildName)){
		return;
	}

	int fanrongdu = GetGuildNumValue(pKernel, guildName, GUILD_NUM_FANRONGDU);
	int guildCapital = GetGuildNumValue(pKernel, guildName, GUILD_NUM_CAPITAL);
	int guildXingdongli = GetGuildNumValue(pKernel, guildName, GUILD_NUM_XINGDONGLI);
	int guildJiankandu = GetGuildNumValue(pKernel, guildName, GUILD_NUM_JIANKANDU);

	// ��ȡ��ṫ���ռ�
	IPubData* pGuildData = GetPubData(pKernel, guildName);
	if (pGuildData == NULL){
		return;
	}

	// ��ȡ��Ա�б�
	IRecord* memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (memberRecord == NULL){
		return;
	}


	int playerNum = memberRecord->GetRows();
	
	const wchar_t* captainName = L"";
	const wchar_t* assistantName = L"";

	for (int i = 0; i < playerNum; i++)
	{
		int position = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_POSITION);
		if (position == GUILD_POSITION_CAPTAIN)
		{
			captainName = memberRecord->QueryWideStr(i, GUILD_MEMBER_REC_COL_NAME);
		}
		else if (position == GUILD_POSITION_ASSISTANT_CAPTAIN)
		{
			assistantName = memberRecord->QueryWideStr(i, GUILD_MEMBER_REC_COL_NAME);
		}

	}



	IRecord *pGuildBuildingRec = GetPubRecord(pKernel, guildName, GUILD_BUILDING_REC);
	if (pGuildBuildingRec == NULL){
		return ;
	}


	int buildingRowMax = pGuildBuildingRec->GetRows();
	CVarList buildingInfo;
	for (int i = 0; i < buildingRowMax; i++)
	{

		int buildingType = pGuildBuildingRec->QueryInt(i, GUILD_BUILDING_REC_COL_TYPE);
		int buildingLv = pGuildBuildingRec->QueryInt(i, GUILD_BUILDING_REC_COL_LEVEL);
		CVarList var;
		var << buildingType << buildingLv;
		buildingInfo << var;
	}

	
// 	IRecord *pbattle = m_pGuildModule->GetPubRecord(pKernel, GUILD_PUB_DATA, C_REC_GUILD_BATTLE_TOWN);
// 	if (pbattle == NULL)
// 	{
// 		return;
// 	}
// 	int ownerID = 0;
// 	int battleRow = pbattle->FindWideStr(E_FIELD_BATTLE_TOWN_WCHAR_OCCUPIER,guildName);
// 	if (battleRow >= 0)
// 	{
// 		ownerID = pbattle->QueryInt(battleRow, E_FIELD_BATTLE_TOWN_INT32_TOWN);
// 	}
// 
// 	IRecord* pGuildFlagRec = pGuildData->GetRecord(GUILD_DOMAIN_FLAG_REC);
// 	if (NULL == pGuildFlagRec)
// 	{
// 		return;
// 	}
// 
// 	int guildFlagRowMax = pGuildFlagRec->GetRows() ;
// 	CVarList flagInfo;
// 	for (int i = 0; i < guildFlagRowMax; i++)
// 	{
// 		int nFlagType = pGuildFlagRec->QueryInt(i, GUILD_FLAG_COL_FLAG_TYPE);
// 		int nFlagId = pGuildFlagRec->QueryInt(i, GUILD_FLAG_COL_FLAG_ID);
// 		flagInfo<<nFlagType
// 			<< nFlagId;
// 	}
// 
// 	IRecord *pGuildList = m_pGuildModule->GetPubRecord(pKernel, GUILD_PUB_DATA,GUILD_LIST_REC);
// 	if (pGuildList == NULL)
// 	{
// 		return;
// 	}
// 
// 	int guildListRow = pGuildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
// 	if (guildListRow < 0){
// 		return;
// 	}
// 
// 	auto declaration =  pGuildList->QueryWideStr(guildListRow, GUILD_LIST_REC_COL_DECLARATION);
// 	
// 	int nation = pGuildList->QueryInt(guildListRow, GUILD_LIST_REC_COL_NATION);
// 
// 	int xiangFangLevel = GuildBuildingLevel(pKernel, guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);
// 	GuildUpLevelConfig_t *upLevelConfig = GetGuildUpLevelConfig(xiangFangLevel);
// 	if (upLevelConfig == NULL)
// 	{
// 		return ;
// 	}
// 
// 	int maxMember = upLevelConfig->m_MaxMember;
// 	int power = pGuildList->QueryInt(guildListRow, GUILD_LIST_REC_COL_FIGHT_ABILITY);
// 
// 	// ��ȡ�����ռ�
// 	IPubData* pPubData = GetPubData(pKernel, GUILD_PUB_DATA);
// 	if (NULL == pPubData)
// 	{
// 		return;
// 	}
// 
// 	IRecord* pSortRecord = pPubData->GetRecord(GUILD_SORT_REC);
// 	if (NULL == pSortRecord)
// 	{
// 		return;
// 	}
// 
// 	const int rank_row = pSortRecord->FindWideStr(GUILD_SORT_REC_COL_NAME, guildName);	
// 
// 	CVarList msg;
// 	msg << SERVER_CUSTOMMSG_GUILD
// 		<< SC_GUILD_OTHER_GUILD_INFO
// 		<< guildName
// 		<< nation
// 		<< fanrongdu
// 		<< guildCapital
// 		<< playerNum
// 		<< captainName
// 		<< assistantName
// 		<< declaration
// 		<< maxMember
// 		<< ownerID
// 		<< guildFlagRowMax
// 		<< flagInfo
// 		<< buildingRowMax
// 		<< buildingInfo
// 		<< power
// 		<< rank_row
// 		<< guildXingdongli
// 		<< guildJiankandu;
// 
// 	pKernel->Custom(self, msg);

}