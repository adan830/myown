//--------------------------------------------------------------------
// �ļ���:		GuildPubMessageHook.cpp
// ��  ��:		����ϵͳ-����������������Ϣ
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#include "GuildModule.h"
#include "../Define/PubDefine.h"
#include "../Define/GuildMsgDefine.h"
#include "../Define/CommandDefine.h"
#include "../Define/GameDefine.h"
#include "utils/util_func.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/SocialSystemModule/SystemMailModule.h"
#include "Define/ServerCustomDefine.h"
#include "ChatModule.h"
#include "SystemFunctionModule/RewardModule.h"
#include "SystemFunctionModule/CapitalModule.h"
#include "Define/SnsDefine.h"

// ���������ݷ������´�����Ϣ
int GuildModule::OnPublicMessage(IKernel* pKernel, const PERSISTID& self,
                           const PERSISTID& sender, const IVarList& args)
{
    const char* spaceName = args.StringVal(0);
    const wchar_t* logicName = args.WideStrVal(1);
    // ���ǹ������Ϣ������Ҫ����
    if (::strcmp(spaceName, PUBSPACE_GUILD) != 0
        || ::wcscmp(logicName, GUILD_LOGIC) != 0)
    {
        return 0;
    }

    int msgid = args.IntVal(2);
    switch (msgid)
    {
    case PS_GUILD_MSG_CREATE:// �����������ظ�
        {
            const wchar_t* guildName = args.WideStrVal(3);
            const wchar_t* playerName = args.WideStrVal(4);
            int result = args.IntVal(5);

            pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME 
                << SS_GUILD_CREATE_RESULT << guildName << result);

			
        }
        break;
    case PS_GUILD_MSG_APPLY_JOIN:// ���빫��������
        {
            const wchar_t* guildName = args.WideStrVal(3);
            const wchar_t* playerName = args.WideStrVal(4);
            int result = args.IntVal(5);

            pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME 
                << SS_GUILD_APPLY_JOIN_RESULT << guildName << result);
        }
        break;
    case PS_GUILD_MSG_CANCEL_APPLY:// ȡ��������빫��
        {
            const wchar_t* guildName = args.WideStrVal(3);
            const wchar_t* playerName = args.WideStrVal(4);
            int result = args.IntVal(5);

            pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME 
                << SS_GUILD_CANCEL_APPLY_RESULT << guildName << result);
        }
        break;
	case PS_GUILD_MSG_ACCEPT_JOIN:// ��������Ľ��
		{
			const wchar_t* guildName = args.WideStrVal(3);
			const wchar_t* playerName = args.WideStrVal(4);
			const wchar_t* applyName = args.WideStrVal(5);
			int onLine = args.IntVal(6);
			int result = args.IntVal(7);
            int position = args.IntVal(8);
            int sex = args.IntVal(9);
            int level = args.IntVal(10);
            int career = args.IntVal(11);
            int curContrib = args.IntVal(12);
            int maxContrib = args.IntVal(13);
            int fight = args.IntVal(14);
			int offlinehours = args.IntVal(15);
			int vip = args.IntVal(16);

            // ֪ͨ��������빫��Ľ��
            // ��������߲����ߣ�����������ʱ��֪ͨ����¼���������ݿ��У�
			if (onLine == ONLINE)
			{
                pKernel->CommandByName(applyName, CVarList() << COMMAND_GUILD_BY_NAME 
                    << SS_GUILD_ACCEPT_JOIN_RESULT << result << guildName);
			}
			if (result = GUILD_MSG_REQ_SUCC){

				const ConfigMailItem& mail = m_pMailModule->GetConfigMail(pKernel, "guild_join");

				// �ʼ�������
				std::wstring mail_content = SystemMailModule::ComposeMailContent(pKernel,
					mail.mail_content.c_str(), CVarList() << guildName);

				// �ʼ�֪ͨ���ɹ����빫��
				m_pMailModule->SendMailToPlayer(pKernel, mail.sender.c_str(), applyName,
					mail.mail_title.c_str(), mail_content.c_str(), mail.items);
			}



			// ֪ͨ�����߽��
			pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME 
				<< SS_GUILD_ACCEPT_RESULT << result << guildName << position 
                << applyName << sex << level << career << curContrib << maxContrib 
                << fight << onLine << offlinehours);

			// ���������ʾ
			if (result == GUILD_MSG_REQ_SUCC)
			{
				// �޸���֯������ʾ(lihl 2017-09-11)
				CVarList msg;
				msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_MEMBER_JOIN_BROADCAST << applyName << vip;
				GuildModule::BroadCastGuild(pKernel, guildName, msg);
			}
		}
		break;
    case PS_GUILD_MSG_REFUSE_JOIN:// �ܾ��˼��빫�������
        {
            // ֪ͨ�����߾ܾ��˼��빫�������
            const wchar_t *playerName = args.WideStrVal(3);
            const wchar_t *applyName = args.WideStrVal(4);
            const wchar_t *guildName = args.WideStrVal(5);
            int result = args.IntVal(6);

            pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME 
                << SS_GUILD_REFUSE_JOIN << result << applyName << guildName);
        }
        break;
    case PS_GUILD_MSG_CHANGE_NOTICE:// �޸Ĺ�����
        {
            pKernel->CommandByName(args.WideStrVal(4), CVarList() << COMMAND_GUILD_BY_NAME 
                << SS_GUILD_NOTICE_RESULT << args.IntVal(5) << args.WideStrVal(6));
        }
        break;
	case PS_GUILD_CHANGE_DECLARATION:// �޸����Խ��
		{
			pKernel->CommandByName(args.WideStrVal(4), CVarList() << COMMAND_GUILD_BY_NAME 
				<< SS_GUILD_DECLARATION_RESULT << args.IntVal(5) << args.WideStrVal(6));
		}
		break;
    case PS_GUILD_MSG_QUIT:// �˳�����ظ�
        {
            const wchar_t *guildName = args.WideStrVal(3);
            const wchar_t *playerName = args.WideStrVal(4);
			const wchar_t *captainName =  args.WideStrVal(6);
            int result = args.IntVal(5);

            pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME 
                << SS_GUILD_QUIT_RESULT << result << guildName);
			if(captainName !=playerName )
			{
				pKernel->CommandByName(captainName, CVarList() << COMMAND_GUILD_BY_NAME
					<< SS_GUILD_BE_PROMOTION << GUILD_POSITION_CAPTAIN );
			}
        }
        break;
    case PS_GUILD_MSG_FIRE:// �߳�����ظ�
        {
            // ����߳��ɹ�����Ҫ��������ҷ��ʼ�
            // �������������ߣ�ֱ���������˳�����
            // ���������Ҳ����ߣ����������ʱ�������˳�����
            const wchar_t* guildName = args.WideStrVal(3);
            const wchar_t* playerName = args.WideStrVal(4);
            const wchar_t* memberName = args.WideStrVal(7);
            int result = args.IntVal(5); 

            if (result == GUILD_MSG_REQ_SUCC)
            {
                int onLine = args.IntVal(6);
                if (onLine == ONLINE)
                {
                    // ֪ͨ����˳��˹���
                    pKernel->CommandByName(memberName, CVarList() << COMMAND_GUILD_BY_NAME 
                        << SS_GUILD_LEAVE<<guildName);
                }

                // ���ʼ�
				const ConfigMailItem& mail = SystemMailModule::GetConfigMail(pKernel, MAIL_GUILD_FIRE);
                std::wstring mailContent = SystemMailModule::ComposeMailContent(pKernel, mail.mail_content.c_str(), 
                    CVarList() << playerName << guildName);
                SystemMailModule::SendMailToPlayer(pKernel, mail.sender.c_str(), memberName, 
                    mail.mail_title.c_str(), mailContent.c_str(), "");
            }

            pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME 
                << SS_GUILD_FIRE_RESULT << result << memberName << guildName);
        }
        break;
    case PS_GUILD_MSG_DISMISS:// ��ɢ����ظ�
        {
            const wchar_t* guildName = args.WideStrVal(3);
			int nIMRoomId	= args.IntVal(4);
            
			// ��¼������־add by   (2015-04-23)
			//ע:ϵͳ�Զ���ɢ����
			//GuildLog log;
			//log.account            = "0";
			//log.playerName         = L"System";
			//log.operateType        = LOG_GUILD_DISMISS;
			//log.guildName          = guildName;
			//LogModule::m_pLogModule->SaveGuildLog(pKernel, PERSISTID(), log);
			::extend_warning(LOG_TRACE, "guild '%s' dismissed by system", StringUtil::WideStrAsString(guildName).c_str());
        }
        break;
    case PS_GUILD_MSG_DONATE:
        {
            const wchar_t *guildName = args.WideStrVal(3);
            const wchar_t *playerName = args.WideStrVal(4);
            int result = args.IntVal(5);
			CVarList msg;
			msg << COMMAND_GUILD_BY_NAME
				<< SS_GUILD_DONATE_RESULT << result << guildName << args.IntVal(6) << args.IntVal(7);
			

            pKernel->CommandByName(playerName, msg );
        }
        break;
    case PS_GUILD_MSG_PROMOTION:
        {
            const wchar_t *guildName = args.WideStrVal(3);
            const wchar_t *playerName = args.WideStrVal(4);
            int result = args.IntVal(5);
            const wchar_t *applyName = args.WideStrVal(6);
            int position = args.IntVal(7);

            pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME
                << SS_GUILD_PROMOTION_RESULT << result << applyName << position << guildName);

			// ֪ͨ��������
			if (result == GUILD_MSG_REQ_SUCC)
			{
				pKernel->CommandByName(applyName, CVarList() << COMMAND_GUILD_BY_NAME
					<< SS_GUILD_BE_PROMOTION << position);
			}
        }
        break;
    case PS_GUILD_MSG_DEMOTION:
        {
            const wchar_t *guildName = args.WideStrVal(3);
            const wchar_t *playerName = args.WideStrVal(4);
            int result = args.IntVal(5);
            const wchar_t *applyName = args.WideStrVal(6);
            int position = args.IntVal(7);

            pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME
                << SS_GUILD_DEMOTION_RESULT << result << applyName << position << guildName);

			// ֪ͨ����ְ��
			if (result == GUILD_MSG_REQ_SUCC)
			{
				pKernel->CommandByName(applyName, CVarList() << COMMAND_GUILD_BY_NAME
					<< SS_GUILD_BE_DEMOTION << position);
			}
        }
        break;
    case PS_GUILD_MSG_GUILD_GONE:// ֪ͨ��Ա�����ɢ
        {
            const wchar_t* guildName = args.WideStrVal(3);
            //const wchar_t* playerName = args.WideStrVal(4);
            int onLine = args.IntVal(5);
            const wchar_t* memberName = args.WideStrVal(6);
            if (onLine == ONLINE)
            {
                pKernel->CommandByName(memberName, CVarList() << COMMAND_GUILD_BY_NAME 
                    << SS_GUILD_LEAVE << guildName);
            }

            //// ���ʼ�֪ͨ��ҹ����ɢ��
            //const ConfigMailItem& mail = SystemMailModule::GetConfigMail(pKernel, "guild_dismiss");
            //std::wstring mailContent = SystemMailModule::ComposeMailContent(pKernel, mail.mail_content.c_str(), 
            //    CVarList() << guildName << playerName);
            //SystemMailModule::SendMailToPlayer(pKernel, mail.sender.c_str(), memberName, 
            //    mail.mail_title.c_str(), mailContent.c_str(), "");
        }
        break;
    case PS_GUILD_MSG_MODIFY_DEVOTE:// ֪ͨ�޸��˸���ʣ�๱��ֵ
        {
            pKernel->CommandByName(args.WideStrVal(4), CVarList() << COMMAND_GUILD_BY_NAME 
                << SS_GUILD_MODIFY_DEVOTE << args.IntVal(5));
        }
        break;
    case PS_GUILD_MSG_BUY_ITEM:// ֪ͨ������Ʒ�۳�����ֵ���
        {
            int result = args.IntVal(3);
            const wchar_t *playerName = args.WideStrVal(4);
			int index = args.IntVal(5);
            const char *itemId = args.StringVal(6);
            int buyNum = args.IntVal(7);
            int consume = args.IntVal(8);
            int shopBuyCount = args.IntVal(9);
            int memberBuyRest = args.IntVal(10);

            // ��¼��־
            //std::wstring strItemId = ::util_string_as_widestr(itemId);

            pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME 
                << SS_GUILD_BUY_ITEM_RESULT << result << index << itemId 
                << buyNum << consume << shopBuyCount << memberBuyRest);
        }
        break;
    case PS_GUILD_MSG_CAPTAIN_TRANSFER:
        {
            int result = args.IntVal(3);
            const wchar_t *oldCaptain = args.WideStrVal(4);
            const wchar_t *newCaptain = args.WideStrVal(5);
            int oldCaptainPosition = args.IntVal(6);
			const wchar_t* guildName = args.WideStrVal(7);

            pKernel->CommandByName(oldCaptain, CVarList() << COMMAND_GUILD_BY_NAME
                << SS_GUILD_CAPTAIN_TRANSFER << result << newCaptain << oldCaptainPosition << oldCaptain << guildName);

			if (result == GUILD_MSG_REQ_SUCC)
			{
				pKernel->CommandByName(newCaptain, CVarList() << COMMAND_GUILD_BY_NAME
					<< SS_GUILD_CAPTAIN_BE_TRANSFER);
			}
        }
        break;
	case PS_GUILD_CHANGE_GUILD_SHORT_NAME:
	{
		int ret = args.IntVal(3);
		if (ret == GUILD_MSG_REQ_SUCC)
		{
			const wchar_t *guildName = args.WideStrVal(4);
			CVarList var;
			var << COMMAND_GUILD_BY_NAME
				<< SS_GUILD_REFRESH_GUILD_SYSMBOL;
			m_pGuildModule->CommandToGuildMember(pKernel, guildName, var);
		}
		const wchar_t* playerName = args.WideStrVal(5);

		pKernel->CustomByName(playerName, CVarList() << SERVER_CUSTOMMSG_GUILD << SS_GUILD_CHANGE_GUILD_SYSMBOL << ret);
	
	}break;

// 	case PS_GUILD_DEFEND_IS_APPLY:
// 		{
// 			const wchar_t* playName = args.WideStrVal(3);
// 			int result = args.IntVal(4);
// 			int gmType = args.IntVal(5);
// 			pKernel->CommandByName(playName, CVarList() << COMMAND_GUILD_DEFEND_ACTIVITY << SS_GUILD_DEFEND_IS_APPLY << result << gmType);
// 		}
// 		break;
// 	case PS_GUILD_DEFEND_APPLY:
// 		{
// 			const wchar_t* playName = args.WideStrVal(3);
// 			int result = args.IntVal(4);
// 			pKernel->CommandByName(playName, CVarList() << COMMAND_GUILD_DEFEND_ACTIVITY << SS_GUILD_DEFEND_APPLY << result);
// 		}
// 		break;
//         //������ɽ��
//     case PS_GUILD_IMPEACH_RESULT:
//         {
//             GuildImpeach::InformMemberResult(pKernel, args.WideStrVal(3), args.WideStrVal(4), GUILD_IMPEACH_RESULT(args.IntVal(5)));
//         }
//         break;
	case PS_GUILD_MSG_NEW_APPLY:
	{
		const wchar_t* playName = args.WideStrVal(3);
		
		pKernel->CommandByName(playName, CVarList() << COMMAND_GUILD_BY_NAME << SS_GUILD_NEW_APPLAY);

	}break;
	case PS_GUILD_NUM_CHANGE:				   // ����ʽ�䶯
	{
		CVarList msg;
		msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_NUM_VALUE<<args.IntVal(4)<<args.IntVal(5);
		pKernel->CustomByName(args.WideStrVal(3), msg);

	}break;
// 	case PS_GUILD_DONATE_UNLOCK_SKILL:			//�������ܽ��
// 	{
// 		const wchar_t*guildName = args.WideStrVal(3);
// 		
// 		// ��鹫��������б��Ƿ�����
// 		IPubData* pGuildData = m_pGuildModule->GetPubData(pKernel, guildName);
// 		if (pGuildData == NULL)
// 		{
// 			return 0;
// 		}
// 		// ��Ա���
// 		IRecord* pRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
// 		if (pRecord == NULL)
// 		{
// 			return 0;
// 		}
// 		// ��Ա����
// 		int memberCount = pRecord->GetRows();
// 		// ��Ϣ
// 		CVarList msg;
// 		 msg << COMMAND_GUILD_BY_NAME << SS_GUILD_SKILL_UNLOCK;
// 		// ֪ͨ��Ա���ȼ��ı�
// 		LoopBeginCheck(a);
// 		for (int i = 0; i < memberCount; ++i)
// 		{
// 			LoopDoCheck(a);
// 
// 			const wchar_t* playerName = pRecord->QueryWideStr(i, GUILD_MEMBER_REC_COL_NAME);
// 			if (StringUtil::CharIsNull(playerName))
// 			{
// 				continue;
// 			}
// 			if (pKernel->GetPlayerScene(playerName) > 0)
// 			{
// 					// ֪ͨ���
// 					pKernel->CommandByName(playerName, msg);
// 			}
// 		}
// 
// 	}break;
// 	case PS_GUILD_SKILL_OPERATE_RET:
// 	{
// 		const wchar_t* 	playerName = args.WideStrVal(3);
// 		int nResult_ = args.IntVal(4);
// 		int nSkillId_ = args.IntVal(5);
// 		int nLevel_ = args.IntVal(6);
// 		pKernel->CustomByName(playerName, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_SKILL_OPERATE_RET 
// 			<< nResult_ << nSkillId_ << nLevel_);
// 
// 	}break;
// 	
// 	
// 	case PS_GUILD_BUILDING_LEVEL_UP:
// 	{
// 		const wchar_t* playerName = args.WideStrVal(3);
// 		int ret = args.IntVal(4);
// 			
// 		CVarList msg;
// 		pKernel->CustomByName(playerName, CVarList() << SERVER_CUSTOMMSG_GUILD << SS_GUILD_BUILDING_LEVEL_UP<<ret);
// 
// 	}break;
// 	case PS_GUILD_BUILDING_LEVEL_UP_BROADCAST:
// 	{
// 		const wchar_t* guildName = args.WideStrVal(3);
// 		int building_ = args.IntVal(4);
// 		int level_ = args.IntVal(5);
// 
// 		CVarList msg;
// 		msg << SERVER_CUSTOMMSG_GUILD << SC_GUILD_BUILDING_LVUP_BROADCAST << building_ << level_;
// 		GuildModule::BroadCastGuild(pKernel, guildName, msg);
// 	}
// 	break;
// 	case PS_GUILD_NUM_BUY:
// 	{
// 		const wchar_t*playerName = args.WideStrVal(3);
// 		int guildNumType = args.IntVal(4);
// 		int ret = args.IntVal(5);
// 		int value = args.IntVal(6);
// 		CVarList msg;
// 		pKernel->CustomByName(playerName, CVarList() << SERVER_CUSTOMMSG_GUILD << SS_GUILD_NUM_BUY << guildNumType << ret<<value);
// 	}break;
// 	case PS_GUILD_JIAN_KANG_DU_GIF:
// 	{
// 		const wchar_t * playerName = args.WideStrVal(3);
// 		const char *award = args.StringVal(4);
// 		
// 		pKernel->CommandByName(playerName, CVarList() << COMMAND_GUILD_BY_NAME << SS_GUILD_GET_JIAN_KANG_DU_GIF << award );
// 	}break;
// 	case PS_GUILD_ADD_PREPARE_GUILD:
// 	{
// 		const wchar_t*guildName = args.WideStrVal(3);
// 		const wchar_t*applyName = args.WideStrVal(4);
// 
// 		pKernel->CommandByName(applyName, CVarList() << COMMAND_GUILD_BY_NAME
// 			<< SS_GUILD_ACCEPT_JOIN_RESULT << GUILD_MSG_REQ_SUCC << guildName);
// 
// 
// 		const ConfigMailItem& mail = m_pMailModule->GetConfigMail(pKernel, "guild_join");
// 
// 		// �ʼ�������
// 		std::wstring mail_content = SystemMailModule::ComposeMailContent(pKernel,
// 			mail.mail_content.c_str(), CVarList() << guildName);
// 
// 		// �ʼ�֪ͨ���ɹ����빫��
// 		m_pMailModule->SendMailToPlayer(pKernel, mail.sender.c_str(), applyName,
// 			mail.mail_title.c_str(), mail_content.c_str(), mail.items);
// 
// 
// 	
// 	}break;
// 	case PS_GUILD_USE_GUILD_RARE_TREASURE:
// 	{
// 		const wchar_t* guildName = args.WideStrVal(3);
// 		const wchar_t * playerName = args.WideStrVal(4);
// 		int ret = args.IntVal(5);
// 		pKernel->CustomByName(playerName, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_USE_GUILD_RARE_TREASURE << ret);
// 	
// 	};
// 	case PS_GUILD_GIVE_OUT_BONUS_RESUTL:
// 	{
// 		const wchar_t * playerName = args.WideStrVal(3);
// 		const wchar_t *reason = args.WideStrVal(4);
// 		int giveValue = args.IntVal(5); //����ÿ����Ҷ���
// 		
// 		RewardModule::AwardEx award;
// 		award.srcFunctionId = FUNCTION_EVENT_ID_GUILD_BONUS;
// 		std::string capName = CapitalModule::m_pCapitalModule->GetCapitalName(CAPITAL_SILVER);
// 		award.capitals[capName.c_str()] = giveValue;
// 		award.name = GUILD_BONUS_MAIL;
// 
// 
// 		for (size_t i = 6; i < args.GetCount(); i++)
// 		{
// 			const wchar_t*name = args.WideStrVal(i);
// 			CVarList mail_param;
// 			mail_param<< giveValue;
// 			RewardModule::m_pRewardInstance->RewardByRoleName(pKernel, name, &award, mail_param);
// 		}
// 
// 		pKernel->CustomByName(playerName, CVarList() << SERVER_CUSTOMMSG_GUILD << SC_GUILD_GIVE_OUT_BONUS);
// 	
// 	}break;
    default:
        break;
    }

    return 0;
}
