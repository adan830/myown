//--------------------------------------------------------------------
// �ļ���:		GuildCommandHook.cpp
// ��  ��:		����ϵͳ-�����ڲ���Ϣ
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#include "GuildModule.h"
#include <time.h>
#include "../Define/GuildMsgDefine.h"
#include "../Define/PubDefine.h"
#include "../Define/LogDefine.h"
#include "../Define/ServerCustomDefine.h"
#include "FsGame/SystemFunctionModule/CapitalModule.h"
#include "FsGame/CommonModule/LogModule.h"
#include "utils/extend_func.h"
#include "utils/util_func.h"
#include "FsGame/CommonModule/ContainerModule.h"
#include "utils/string_util.h"
#include "FsGame/SocialSystemModule/ChatModule.h"
#include "FsGame/SocialSystemModule/SystemMailModule.h"
#include "FsGame/Define/CommandDefine.h"
//#include "FsGame/Define/GuildStationDefine.h"
#include "utils/custom_func.h"
#include "SkillModule/SkillUpgradeModule.h"
#include "SkillModule/SkillToSprite.h"
#include "Define/Skilldefine.h"
#include "SkillModule/PassiveSkillSystem.h"
#include "SystemFunctionModule/RewardModule.h"
#include "CommonModule/FunctionEventModule.h"
#include "ItemModule/ToolItem/ToolItemModule.h"
#include "Define/BattleAbilityDefine.h"
#include "CommonModule/EnvirValueModule.h"


// �ڲ�����ص�
int GuildModule::OnCommandGuild(IKernel* pKernel, const PERSISTID& self,
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

    int subCommand = args.IntVal(1);
	switch (subCommand)
	{
		case SS_GUILD_CREATE_RESULT:// ���ᴴ�����
		{
			m_pGuildModule->OnCommandCreateGuildResult(pKernel, self, args);
		}
		break;
		case SS_GUILD_APPLY_JOIN_RESULT:// ������빫��Ľ��
		{
			m_pGuildModule->OnCommandApplyJoinGuildResult(pKernel, self, args);
		}
		break;
		case SS_GUILD_CANCEL_APPLY_RESULT:// ȡ��������빫��Ľ��
		{
			m_pGuildModule->OnCommandCancelApplyJoinGuildResult(pKernel, self, args);
		}
		break;
		case SS_GUILD_ACCEPT_RESULT:// ֪ͨ�����߽�������Ľ��
		{
			m_pGuildModule->OnCommandAcceptResult(pKernel, self, args);
		}
		break;
		case SS_GUILD_ACCEPT_JOIN_RESULT:// ֪ͨ�����߼��빫����
		{
			m_pGuildModule->OnCommandApplyJoinResult(pKernel, self, args);
		}
		break;
		case SS_GUILD_REFUSE_JOIN:// ֪ͨ�����߾ܾ����빫�������
		{
			m_pGuildModule->OnCommandRefuseJoin(pKernel, self, args);
		}
		break;
		case SS_GUILD_NOTICE_RESULT:// ֪ͨ�޸Ĺ�����
		{
			pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD
				<< SC_GUILD_NOTICE_RESULT << args.IntVal(2) << args.WideStrVal(3));

			// ֪ͨ�ͻ����޸Ľ��
			CustomSysInfo(pKernel, self, args.IntVal(2) == GUILD_MSG_REQ_SUCC ? SYSTEM_INFO_ID_17613 : SYSTEM_INFO_ID_17614, CVarList());

			// �㲥��֯��Ϣ [lihl 2017/12/2]
			if (args.IntVal(2) == GUILD_MSG_REQ_SUCC)
			{
				CVarList broadMsg_;
				broadMsg_ << SERVER_CUSTOMMSG_GUILD
					<< SC_GUILD_DECLARATION_BROADCAST << args.WideStrVal(3);
				GuildModule::BroadCastGuild(pKernel, self, broadMsg_);
			}
		}
		break;
		case SS_GUILD_DECLARATION_RESULT:// ֪ͨ�޸����Խ��
		{
			pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD
				<< SC_GUILD_DECLARATION_RESULT << args.IntVal(2) << args.WideStrVal(3));

			// ֪ͨ�ͻ����޸Ľ��
			CustomSysInfo(pKernel, self, args.IntVal(2) == GUILD_MSG_REQ_SUCC ? SYSTEM_INFO_ID_17615 : SYSTEM_INFO_ID_17616, CVarList());
		}
		break;
		case SS_GUILD_QUIT_RESULT:// �˳�������
		{
			m_pGuildModule->OnCommandQuitResult(pKernel, self, args);
		}
		break;
		case SS_GUILD_FIRE_RESULT:// �߳�������
		{
			int result = args.IntVal(2);
			const wchar_t* memberName = args.WideStrVal(3);
			const wchar_t* guildName = args.WideStrVal(4);
			pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD
				<< SC_GUILD_FIRE_RESULT << result << memberName);

			//֪ͨ����뿪���פ�س���
			//pKernel->CommandByName(memberName, CVarList() << COMMAND_GUILD_STATION << SS_GUILD_STATION_LEAVE << guildName);

			// ��¼������־
			//GuildLog log;
			//log.byRoleName         = memberName;
			//log.operateType        = LOG_GUILD_FIRE;
			//log.guildName          = guildName;
			//log.grade              = m_pGuildModule->GetGuildLevel(pKernel, guildName);
			//LogModule::m_pLogModule->SaveGuildLog(pKernel, self, log);
			if (0 != result)
			{
// 				int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);
// 				LogModule::m_pLogModule->OnGuildEvent(pKernel, self, guildName, grade, LOG_GUILD_FIRE, memberName);
			}
		}
		break;
		case SS_GUILD_DISMISS_RESULT:// ��ɢ������
		{
			m_pGuildModule->OnCommandDismissResult(pKernel, self, args);
		}
		break;
		case SS_GUILD_LEAVE:// ֪ͨ����뿪�˹���
		{
			m_pGuildModule->OnCommandLeaveGuild(pKernel, self, args);
		}
		break;
		case SS_GUILD_DONATE_RESULT: //���׽��
		{
			m_pGuildModule->OnCommandDonateResult(pKernel, self, args);
		}
		break;
		case SS_GUILD_PROMOTION_RESULT: //��ְ���
		{
			int result = args.IntVal(2);
			const wchar_t* target = args.WideStrVal(3);
			int position = args.IntVal(4);
			pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD
				<< SC_GUILD_PROMOTION << result << target << position);

			const wchar_t* guildName = args.WideStrVal(5);
			int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);
			// ��¼������־modify by   (2015-04-23)
			//LogModule::m_pLogModule->OnGuildMemberPositionEvent(pKernel, self, guildName, grade, target, position, true);
		}
		break;
		case SS_GUILD_BE_PROMOTION: //����ְ
		{
			// ���ݵ�ǰְλ ˢ���Ƿ�������ı�־
			m_pGuildModule->UpdateNewApplyByPosition(pKernel, self);
			// ����ְλ
			pSelfObj->SetInt("GuildPosition", args.IntVal(2));
		}
		break;
		case SS_GUILD_DEMOTION_RESULT: //��ְ���
		{
			int result = args.IntVal(2);
			const wchar_t* target = args.WideStrVal(3);
			int position = args.IntVal(4);
			pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD
				<< SC_GUILD_DEMOTION << result << target << position);

			// ��¼������־add by   (2015-04-23)
// 			const wchar_t* guildName = args.WideStrVal(5);
// 			int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);
// 			LogModule::m_pLogModule->OnGuildMemberPositionEvent(pKernel, self, guildName, grade, target, position, false);

		}
		break;
		case SS_GUILD_BE_DEMOTION: //����ְ
		{
			// ���ݵ�ǰְλ ˢ���Ƿ�������ı�־
			m_pGuildModule->UpdateNewApplyByPosition(pKernel, self);
			// ����ְλ
			pSelfObj->SetInt("GuildPosition", args.IntVal(2));
		}
		break;
		case SS_GUILD_BUY_ITEM_RESULT:// ֪ͨ��ҹ�����Ʒ�۳�����ֵ���
		{
			m_pGuildModule->OnCommandBuyItemResult(pKernel, self, args);
		}
		break;
		case SS_GUILD_CAPTAIN_TRANSFER: //����ת��
		{
			pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD
				<< SC_GUILD_CAPTAIN_TRANSFER << args.IntVal(2) << args.WideStrVal(3) << args.IntVal(4));

			// ˢ���Ƿ�������ı�־
			if (args.IntVal(2) == GUILD_MSG_REQ_SUCC)
			{
				m_pGuildModule->UpdateNewApplyByPosition(pKernel, self);

				// ����ְλ
				pSelfObj->SetInt("GuildPosition", GUILD_POSITION_MEMBER);
			}

			// ��¼��־
			const wchar_t *guildName = args.WideStrVal(6);
			int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);

			// ��¼������־modify by   (2015-04-23)
			//LogModule::m_pLogModule->OnGuildEvent(pKernel, self, guildName, grade, LOG_GUILD_TRANSFER, args.WideStrVal(3));
		}
		break;
		case SS_GUILD_CAPTAIN_BE_TRANSFER:// ��ת��Ϊ����
		{
			// ˢ���Ƿ�������ı�־
			m_pGuildModule->UpdateNewApplyByPosition(pKernel, self);
			// ����ְλ
			pSelfObj->SetInt("GuildPosition", GUILD_POSITION_CAPTAIN);
			
		}
		break;
		case SS_GUILD_REFRESH_GUILD_SYSMBOL:
		{
			m_pGuildModule->RefreshGuildSysmbol(pKernel, self);
		
		}break;
		case SS_GUILD_NEW_APPLAY://֪ͨ���µ�����
		{
			if (!m_pGuildModule->CheckPositionPriv(pKernel, self, GUILD_PRIV_RATIFY))
			{
				pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD
					<< SC_GUILD_NEW_APPLY << NEW_NOTIFY_SHOW);
			}

		}break;

		
		case SS_GUILD_SKILL_UNLOCK:
		{
			m_pGuildModule->RefreshGuildSkill(pKernel, self);
		}break;
		
		case SS_GUILD_ADD_GUILD_BUILDING_EXP:
		{
			int addValue = args.IntVal(2);
			const wchar_t*guildName = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
			m_pGuildModule->AddGuildBuildingExp(pKernel, guildName, addValue);
		}break;
		case SS_GUILD_INVITE_JOIN_GUILD:
		{
			const wchar_t* inviteGuildName = args.WideStrVal(2);
			if (!StringUtil::CharIsNull(inviteGuildName))
			{
				IGameObj*pSelfObj = pKernel->GetGameObj(self);
				if (pSelfObj != NULL)
				{
					if (!pSelfObj->FindData(GUILD_INVITE_GUILD_NAME))
					{
						pSelfObj->AddDataWideStr(GUILD_INVITE_GUILD_NAME, L"");
					}
					pSelfObj->SetDataWideStr(GUILD_INVITE_GUILD_NAME, inviteGuildName);
				}
			}

		}break;
// 		case SS_GUILD_TEACH_REQUEST:
// 		{
// 			m_pGuildModule->OnCommandTeachRequest(pKernel, self, args, 2);
// 		}break;
// 		case SS_GUILD_TEACH_ACK:
// 		{
// 			m_pGuildModule->OnCommandTeachAck(pKernel, self, args, 2);
// 		}break;
// 		case SS_GUILD_TEACH_FIN:
// 		{
// 			m_pGuildModule->OnCommandTeachFin(pKernel, self, args, 2);
// 		}break;
// 		case SS_GUILD_TEACH_ENT_FAILED:
// 		{
// 			m_pGuildModule->OnCommandTeachFailed(pKernel, self, args, 2);
// 		}break;
    default:
        break;
    }

    return 0;
}

// �����������Ϣ
int GuildModule::OnCommandCreateGuildResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

    const wchar_t* guildName = args.WideStrVal(2);
    int result = args.IntVal(3);

    if (result == GUILD_MSG_REQ_SUCC)
    {
        pSelfObj->SetWideStr("GuildName", guildName);
		pSelfObj->SetInt("GuildPosition", GUILD_POSITION_CAPTAIN);
//		LogModule::m_pLogModule->OnGuildJoin(pKernel, self, guildName);
		InformGuildApply(pKernel,self);
		RefreshGuildSysmbol(pKernel, self);
		m_pGuildModule->RefreshGuildSkill(pKernel, self);
    }
    else 
    {
		int nCostGold = EnvirValueModule::EnvirQueryInt(ENV_VALUE_CREATE_GUILD_GOLD);
		m_pCapitalModule->IncCapital(pKernel, self, CAPITAL_GOLD, (__int64)nCostGold, FUNCTION_EVENT_ID_GUILD_CREATE_FAILED_RET, L"Create Guild Failed, Return Capital");
    }
    
	// ֪ͨ��Ҵ������
	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_CREATE_RESULT << result << guildName);

	// �㲥��֯������Ϣ lihl-2017-09-11
	if (result == GUILD_MSG_REQ_SUCC)
	{
		const wchar_t* pwszName_ = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
		int nVip_ = pSelfObj->QueryInt(FIELD_PROP_VIP_LEVEL);
		pKernel->CustomByWorld(CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_CREATED_BROADCAST << pwszName_ << nVip_ << guildName);
	}
	// ��¼������־add by   (2015-04-23)
	//GuildLog log;
	//log.operateType        = LOG_GUILD_CREATE;
	//log.guildName          = guildName;
	//log.grade              = m_pGuildModule->GetGuildLevel(pKernel, guildName);
	//LogModule::m_pLogModule->SaveGuildLog(pKernel, self, log);
// 	int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);
// 	LogModule::m_pLogModule->OnGuildEvent(pKernel, self, guildName, grade, LOG_GUILD_CREATE, NULL);
//	pKernel->Command(self, self, CVarList() << COMMAND_GUILD_STATION << SS_GUILD_STATION_LEAVE);
/*
	// ��ʼ��ȫ�����а�
	GlobalRankModule::InitGuildGlobalProp(pKernel, self, guildName);
*/
    return 0;
}

// ����������빫������Ϣ
int GuildModule::OnCommandApplyJoinGuildResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
    const wchar_t* guildName = args.WideStrVal(2);
    int result = args.IntVal(3);

    // ֪ͨ���������
    pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_APPLY_RESULT << result << guildName);

	// ����ɹ���֪ͨ��Ȩ�޴�������
	if (result == GUILD_MSG_REQ_SUCC)
	{
		UpdateNewApplyToAllAuditor(pKernel, guildName);
	}

	// ��¼������־
// 	int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);
// 	LogModule::m_pLogModule->OnGuildEvent(pKernel, self, guildName, grade, LOG_GUILD_APPLY, NULL);
    return 0;
}

// ����ȡ��������빫������Ϣ
int GuildModule::OnCommandCancelApplyJoinGuildResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
    const wchar_t* guildName = args.WideStrVal(2);
    int result = args.IntVal(3);
    pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_CANCEL_APPLY << result << guildName);

	// ȡ������ɹ���֪ͨ��Ȩ�޴�������
	if (result == GUILD_MSG_REQ_SUCC)
	{
		UpdateNewApplyToAllAuditor(pKernel, guildName);
	}

	// ��¼������־add by   (2015-04-23)
// 	int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);
// 	LogModule::m_pLogModule->OnGuildEvent(pKernel, self, guildName, grade, LOG_GUILD_UNAPPLY, NULL);
    return 0;
}

// ���������߽�������������Ϣ
int GuildModule::OnCommandAcceptResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
    int result = args.IntVal(2);
    const wchar_t *guildName = args.WideStrVal(3);
    const wchar_t *applyName = args.WideStrVal(5);
    
    pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD 
        << SC_GUILD_ACCEPT_RESULT << result << args.IntVal(4) << applyName
        << args.IntVal(6) << args.IntVal(7) << args.IntVal(8) << args.IntVal(9) 
        << args.IntVal(10) << args.IntVal(11) << args.IntVal(12) << args.IntVal(13));

	// ����ɹ���֪ͨ��Ȩ�޴�������
	if (result == GUILD_MSG_REQ_SUCC)
	{
		UpdateNewApplyToAllAuditor(pKernel, guildName);

		// �������Ƶ����ʾ���ͨ����Ϣ
		if (StringUtil::CharIsNull(applyName))
		{
			return 0;
		}

// 		int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);
// 		LogModule::m_pLogModule->OnGuildEvent(pKernel, self, guildName, grade, LOG_GUILD_ACCEPT, applyName);
		m_pGuildModule->RefreshGuildSkill(pKernel, self);
	}
            
    return 0;
}

// ���������߼��빫��������Ϣ
int GuildModule::OnCommandApplyJoinResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

    int result = args.IntVal(2);
    const wchar_t *guildName = args.WideStrVal(3);
	if (StringUtil::CharIsNull(guildName))
	{
		return 0;
	}

    if (result == GUILD_MSG_REQ_SUCC)
    {
        pSelfObj->SetWideStr("GuildName", guildName);
		pSelfObj->SetInt("GuildPosition", GUILD_POSITION_MEMBER);
	//	LogModule::m_pLogModule->OnGuildJoin(pKernel, self, guildName);
		RefreshGuildSysmbol(pKernel, self);
		m_pGuildModule->RefreshGuildSkill(pKernel, self);

// 		pKernel->Command(self,self,CVarList() << COMMAND_GUILD_STATION << SS_GUILD_STATION_LEAVE );
    }
	

    return 0;
}

// ���������߳ɹ��ܾ����������Ϣ
int GuildModule::OnCommandRefuseJoin(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

    int result = args.IntVal(2);
    const wchar_t *applyName = args.WideStrVal(3);
    const wchar_t *guildName = args.WideStrVal(4);
    pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD 
        << SC_GUILD_REFUSE_RESULT << result << applyName);

	// ����ɹ���֪ͨ��Ȩ�޴�������
	if (result == GUILD_MSG_REQ_SUCC)
	{
		guildName = pSelfObj->QueryWideStr("GuildName");
		UpdateNewApplyToAllAuditor(pKernel, guildName);
		if (pKernel->GetPlayerScene(applyName) > 0)
		{
			::CustomSysInfoByName(pKernel, applyName, SYSTEM_INFO_ID_17617, CVarList() << guildName);
		}
		// ��¼������־add by   (2015-04-23)
// 		int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);
// 		LogModule::m_pLogModule->OnGuildEvent(pKernel, self, guildName, grade, LOG_GUILD_REFUSE, applyName);
	}

    return 0;
}

// �����˳�������
int GuildModule::OnCommandQuitResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
    int result = args.IntVal(2);
    const wchar_t *guildName = args.WideStrVal(3);

    if (result == GUILD_MSG_REQ_SUCC)
    {
        SetLeaveGuild(pKernel, self, (int64_t)::time(NULL),true);
    }
    pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_QUIT_RESULT << result);


    //֪ͨ�뿪���פ�س���
//	pKernel->Command(self, self , CVarList() << COMMAND_GUILD_STATION << SS_GUILD_STATION_LEAVE << guildName);

// 	int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);
// 	LogModule::m_pLogModule->OnGuildEvent(pKernel, self, guildName, grade, LOG_GUILD_QUIT, NULL);

    return 0;
}

// �����ɢ������
int GuildModule::OnCommandDismissResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
    int result = args.IntVal(2);
	const wchar_t *guildName = args.WideStrVal(3);
    if (result == GUILD_MSG_REQ_SUCC)
    {
        SetLeaveGuild(pKernel, self, (int64_t)::time(NULL));
    }
    pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_DISMISS_RESULT << result);

	//��¼��ɢ������־
// 	int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName);
// 	LogModule::m_pLogModule->OnGuildEvent(pKernel, self, guildName, grade, LOG_GUILD_DISMISS, NULL);
    return 0;
}

// �����뿪����
int GuildModule::OnCommandLeaveGuild(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	const wchar_t* guildName = args.WideStrVal(2);
    SetLeaveGuild(pKernel, self, ::time(NULL));
    pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_LEAVE_GUILD << guildName);
    return 0;
}

// ������׽��
int GuildModule::OnCommandDonateResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
    int result = args.IntVal(2);
    const wchar_t* guildName = args.WideStrVal(3);

    CVarList msg;
    msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_DONATE_RESULT << result;

	
	int capitalNumber = args.IntVal(4);
    if (result != GUILD_MSG_REQ_SUCC)
    {
		int selfGuildCurrency = args.IntVal(5);
// 		m_pCapitalModule->IncCapital(pKernel, self, CAPITAL_TSILVER,
// 			(__int64)capitalNumber, FUNCTION_EVENT_ID_GUILD_DONATE_FAILED_RET);
// 
// 		m_pCapitalModule->DecCapital(pKernel, self, CAPITAL_GUILD_CURRENCY,
// 			(__int64)selfGuildCurrency, FUNCTION_EVENT_ID_GUILD_DONATE_FAILED_RET);

    }
    else 
    {
		pKernel->SetInt(self,"GuildDonateTimes",1);

		//pKernel->Command(self, self, CVarList() << COMMAND_SEVEN_DAY_OBJ << COMMAND_SEVEN_DAY_OBJ_DONATE);
    }
    
    pKernel->Custom(self, msg);
	// ��¼������־add by   (2015-04-23)
// 	if (result == GUILD_MSG_REQ_SUCC)
// 	{
// 		LogModule::m_pLogModule->OnGuildDonate(pKernel, self, guildName, args.IntVal(5), CAPITAL_TRADE_SILVER, capitalNumber);
// 	}
    return 0;
}

// ��������Ʒ�۳�����ֵ�����Ϣ
int GuildModule::OnCommandBuyItemResult(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

    //int buyResult = 0;
    //std::wstring strItem;
    int result = args.IntVal(2);
	int index = args.IntVal(3);
	const char *itemId = args.StringVal(4);
	int buyNum = args.IntVal(5);
	int consume = args.IntVal(6);
    int shopBuyCount = args.IntVal(7);
    int memberBuyRest = args.IntVal(8);
    int contribValue = args.IntVal(9);
    if (result != GUILD_MSG_REQ_SUCC)
    {
		auto itemConfig = GetShopItemInfo(index);
		if (itemConfig != NULL)
		{
			for (auto it : itemConfig->m_decCapital)
			{
				CapitalModule::m_pCapitalModule->IncCapital(pKernel, self, it.first, it.second*buyNum, FUNCTION_EVENT_ID_GUILD_SHOP_BUY);
			}
		}

        pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_BUY_ITEM_RESULT << 0 << index << itemId);
        return 0;
    }

    // ���ӵ���
    // ��¼��־
    //std::wstring logComment = L"consume:" + ::util_int_as_widestr(consume);
    //std::wstring name = pSelfObj->QueryWideStr("Name");
    std::wstring guildName = pSelfObj->QueryWideStr("GuildName");

    const PERSISTID toolbox = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
	LoopBeginCheck(a);
	do
	{
		LoopDoCheck(a);
		if (!pKernel->Exists(toolbox))
		{
			pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_BUY_ITEM_RESULT << 0 << index << itemId);
			break;
		}
		
		int nBindState = FunctionEventModule::GetItemBindState(FUNCTION_EVENT_ID_GUILD_SHOP_BUY);
		int putCnt = ContainerModule::PlaceItem(pKernel, toolbox, itemId, buyNum, FUNCTION_EVENT_ID_GUILD_SHOP_BUY, true, nBindState);

		if (putCnt != buyNum)
		{
			pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_BUY_ITEM_RESULT << 0 << index << itemId);
			break;
		}

		pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_BUY_ITEM_RESULT << 1 << index << itemId
			<< shopBuyCount << memberBuyRest << contribValue);

	}while(false);

	// ��¼������־add by   (2015-04-23)
// 	int grade = m_pGuildModule->GetGuildLevel(pKernel, guildName.c_str());
// 	LogModule::m_pLogModule->OnGuildDonateCost(pKernel, self, guildName.c_str(), grade, consume);

    return 0;
}


void GuildModule::GuildSkillLevelUp(IKernel*pKernel, const PERSISTID & player, int skillID, int newLevel)
{
	
	
	PassiveSkillSystemSingleton::Instance()->UpdatePassiveSkill(pKernel, player, skillID, newLevel,PSS_BY_GUILD);
	pKernel->Command(player, player, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PROPERTY_TYPE);
	pKernel->Command(player, player, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PASSIVE_SKILL_TYPE);
					
	
}

int GuildModule::OnCommandDailyActivity(IKernel* pKernel, const PERSISTID& self,
	const PERSISTID& sender, const IVarList& args)
{
	if (m_pGuildModule == NULL)
	{
		return 0;
	}

	int subCommand = args.IntVal(1);
// 	switch (subCommand)
// 	{
// 		case DAILY_ACTIVITY_TYPE_GUILD_TASK:	// �����֯���� ��ӽ���exp
// 		{
// 			const wchar_t* guildName_ = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
// 			if (!StringUtil::CharIsNull(guildName_))
// 			{
// 				int nExp_ = EnvirValueModule::EnvirQueryInt(ENV_VALUE_GUILD_TASK_ADD_BUILDING_EXP);
// 				m_pGuildModule->AddGuildBuildingExp(pKernel, guildName_, nExp_);
// 			}
// 		}
// 		break;
// 		case DAILY_ACTIVITY_TYPE_KIDNAP:
// 		{
// 			int nNum = args.IntVal(2);
// 			if (nNum < 1) nNum = 1;
// 			const wchar_t* guildName_ = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
// 			if (!StringUtil::CharIsNull(guildName_))
// 			{
// 				int nExp_ = EnvirValueModule::EnvirQueryInt(ENV_VALUE_GUILD_KIDNAP_ADD_BUILDING_EXP) * nNum;
// 				m_pGuildModule->AddGuildBuildingExp(pKernel, guildName_, nExp_);
// 
// 				// ��Ӽ��ܾ����		
// 				//int nSkillExp_ = EnvirValueModule::EnvirQueryInt(ENV_VALUE_GUILD_KIDNAP_ADD_SKILL_EXP) * nNum;
// 				//m_pGuildModule->AddGuildSkillExp(pKernel, guildName_, nSkillExp_);
// 			}
// 		}
// 		break;
// 	}
	return 0;
}