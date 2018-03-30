//--------------------------------------------------------------------
// �ļ���:		FriendCommandHook.cpp
// ��  ��:		����ϵͳ-�����ڲ���Ϣ
// ˵  ��:		
// ��������:	2014��10��28��
// ��������:	2014��10��28��
// ������:		  
// �޸���:     
//    :	    
//--------------------------------------------------------------------
#include "FriendModule.h"
#include "FsGame/Define/FriendMsgDefine.h"
#include "FsGame/Define/FriendDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/LogDefine.h"
#include "FsGame/Define/SnsDataDefine.h"
#include "FsGame/Define/SnsDefine.h"
#include "FsGame/Define/FriendDefine.h"

#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/CommonModule/SnsDataModule.h"
//#include "FsGame/InterActiveModule/PartnerModule.h"
// #include "FsGame/InterActiveModule/AchievementModule.h"
// #include "FsGame/Define/QuestDefine.h"

#include "utils/string_util.h"
#include "utils/custom_func.h"
#include "utils/util_func.h"
#include "CommonModule/EnvirValueModule.h"
#include "SystemFunctionModule/RankListModule.h"
#include "Define/RankingDefine.h"
// #include "Define/VipDefine.h"
// #include "TradeModule/VipModule.h"
#include "Define/PlayerBaseDefine.h"
#include "CommonModule/ContainerModule.h"
#include "CommonModule/AsynCtrlModule.h"
//#include "Define/SevenDayObjectDefine.h"
// #include "Define/TeamDefine.h"
// #include "Define/DailyActivityDefine.h"
// #include "Define/WeeklyScoreDefine.h"

// ȡ��sns��ѯ����
void FriendModule::OnCommandGotSnsData(IKernel * pKernel, const PERSISTID & self, const IVarList & args,IVarList &core_msg)
{

	const char *target_uid = args.StringVal(3);
	const wchar_t *target_name = pKernel->SeekRoleName(target_uid);
	int index = 6;
	int row = args.IntVal(index++); 
	int col = args.IntVal(index++);


	const int propLen = 10;
	//��������ע��˳��
	std::string strPropName[propLen] = 
	{
		"Level", "Job", "Sex", "BattleAbility", "GuildName", "RoleLogoutTime", "SumFlower", "WeekFlower", "VipLevel"
	};

	core_msg.Clear();
	core_msg << target_uid<< target_name;
	// ��˳��ȡ
    LoopBeginCheck(a);
	for (unsigned int i = 0; i < propLen; ++i)
	{
        LoopDoCheck(a);
        LoopBeginCheck(b);
		for (unsigned int counter = 0; counter < args.GetCount();++counter)
		{
            LoopDoCheck(b);
			if (strcmp(strPropName[i].c_str(),args.StringVal(counter)) == 0)
			{
				int data_type = pKernel->GetType(self,args.StringVal(counter));
				switch (data_type)
				{
				case VTYPE_INT:
					{
						core_msg << args.IntVal(++counter);
					}
					break;
				case VTYPE_INT64:
					{
						core_msg << args.Int64Val(++counter);
					}
					break;
				case VTYPE_FLOAT:
					{
						core_msg << args.FloatVal(++counter);
					}
					break;
				case VTYPE_DOUBLE:
					{
						core_msg << args.DoubleVal(++counter);
					}
					break;
				case VTYPE_STRING:
					{
						core_msg << args.StringVal(++counter);
					}
					break;
				case VTYPE_WIDESTR:
					{
						core_msg << args.WideStrVal(++counter);
					}
					break;
				default:
					break; 
				}
			}
		}
	}

	

}

// Player ��Command��Ϣ������
int FriendModule::OnPlayerSnsCommand(IKernel* pKernel, const PERSISTID& self, 
									  const PERSISTID& sender, const IVarList& args)
{
	// ��Ϣ��ʽ
	//
	//    [command_id][sub_msg_id]....
	//
	// ��Ӧ��Ϣ 
	//
	//    N/A

	int args_index = 1;
	int sub_msg_id = args.IntVal(args_index);

	switch (sub_msg_id)
	{
	case SNS_DATA_HERE_IS_DATA_YOU_WANT:
		{
			// ����SNS�������Ĳ�ѯ���

			// ��Ϣ��ʽ: [command_id][sub_msg_id][logic_id][targer_uid]
			//          [section_count][section1_name][row][col]...[sectionX_name][row][col]...
			 int logic_id = args.IntVal(2);
			 if (logic_id == SNS_DATA_REQUEST_FORM_FRIEND_UPDATE)
			 {
				 // �����ѯ���
				CVarList core_msg;
				m_pFriendModule->OnCommandGotSnsData(pKernel,self,args,core_msg);
				
				//���߸��º�������
				m_pFriendModule->UpdateOneFriend(pKernel, self, core_msg);
				 
			 }
			 else if (logic_id == SNS_DATA_REQUEST_FORM_FRIEND_ENEMY)
			 {
				 // �����ѯ���
				 CVarList core_msg;
				 m_pFriendModule->OnCommandGotSnsData(pKernel,self,args,core_msg);
				 // ���߸��³������
				 m_pFriendModule->UpdateOneEnmey(pKernel, self, core_msg);

			 }
			 else if (logic_id == SNS_DATA_REQUEST_FORM_FRIEND_FIND)
			 {
				 // �����ѯ���
				 CVarList core_msg;
				 m_pFriendModule->OnCommandGotSnsData(pKernel,self,args,core_msg);

				 // �������
				 m_pFriendModule->FindOnePlayer(pKernel, self, core_msg);

			 }
             else if (logic_id == SNS_DATA_REQUEST_FORM_ADD_FRIEND_APPLY)
             {
                 IGameObj* pSelfObj = pKernel->GetGameObj(self);
                 if (NULL == pSelfObj)
                 {
                     return 0;
                 }
                 // �����ѯ���
                 CVarList core_msg;
                 m_pFriendModule->OnCommandGotSnsData(pKernel,self,args,core_msg);
                 const char* pTargetUid = core_msg.StringVal(0);
                 const wchar_t* pTargetName = core_msg.WideStrVal(1);
                 
                 // �Ѵ���Ҽ���������ı���
                 m_pFriendModule->SetPlayerInvited(pKernel, self, pTargetUid);

                 //��Ҳ����ߣ���������������
                 pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
                     pKernel->GetSceneId(), pTargetUid,
                     CVarList() << SPACE_SNS
                     << SNS_FRIEND_SPACE
                     << SNS_MSG_FRIEND_APPLY 
                     << pKernel->SeekRoleUid(pSelfObj->QueryWideStr("Name"))
                     << pSelfObj->QueryWideStr("Name")
                     << pTargetUid
                     << pTargetName
                     << pSelfObj->QueryInt("Level")
                     << pSelfObj->QueryInt("Job")
                     << pSelfObj->QueryInt("Sex")
                     << pSelfObj->QueryInt("BattleAbility")
                     << pSelfObj->QueryWideStr("GuildName")
                     << APPLY_TYPE_FRIEND);

				 // ֪ͨ�ͻ��ˣ������ѷ�������ȴ�
				 ::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17101, CVarList());
             }
			 else if (logic_id == SNS_DATA_REQUEST_FORM_FRIEND_BLACK)
			 {
				 // �����ѯ���
				 CVarList core_msg;
				 m_pFriendModule->OnCommandGotSnsData(pKernel, self, args, core_msg);

				 // ���º��������
				 m_pFriendModule->UpdateBlackPlayer(pKernel, self, core_msg);

			 }
		}
		break;

	default:
		break;
	}

	return 0;
}

// ���º��ѵ�����״̬
int FriendModule::OnCommandMessage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	int subCommand = args.IntVal(1);
	switch (subCommand)
	{
		case SS_FRIEND_ONLINE_STATUS_UPDATE: //�������������֪ͨ
		{
			m_pFriendModule->OnCommandOnlineUpdate(pKernel, self, args);
		}
		break;
		case SS_FRIEND_REMOVE_PLAYER://֪ͨ�Է�ɾ������
		{
			m_pFriendModule->OnCommandRemoveFriend(pKernel, self, args);
		}
		break;
		case SS_FRIEND_APPLY://���Ӻ��������б�
		{
			m_pFriendModule->OnCommandAddFriendApply(pKernel, self, args);
		}
		break;
		case SS_FRIEND_LEVEL_UPDATE://���ѵȼ�����
		{
			m_pFriendModule->OnCommandFriendLevelUpdate(pKernel, self, args);
		}
		break;
		case SS_FRIEND_BATTLEABILITY_UPDATE://����ս��������
		{
			m_pFriendModule->OnCommandFriendBattleAbility(pKernel, self, args);
		}
		break;
		case SS_FRIEND_GUILDNAME_UPDATE://���Ѱ�����
		{
			m_pFriendModule->OnCommandFriendGuildName(pKernel, self, args);
		}
		break;
		case SS_FRIEND_ADD_RESULT://��Ӻ��ѷ���
		{
			m_pFriendModule->OnCommandAddFriendResult(pKernel, self, args);
		}
		break;
		case SS_FRIEND_SEND_GIFT:// �����ͻ�
		{
			const char * pSenderUid = args.StringVal(2);
			int nFlowers = args.IntVal(3);
			IGameObj* pSelf = pKernel->GetGameObj(self);
			if (NULL == pSelf) break;
			m_pFriendModule->OnRecvFlower(pKernel, pSelf, pSenderUid, nFlowers);
		}
		break;
		case SS_FRIEND_FLOWER_UPDATE:  // ���ѻ���������
		{
			const char * pSenderUid = args.StringVal(2);
			int nSumFlower = args.IntVal(3);
			int nWeekFlower = args.IntVal(4);
			m_pFriendModule->OnCommandFriendFlowerChanged(pKernel, self, pSenderUid, nSumFlower, nWeekFlower);
		}
		break;
		case SS_FRIEND_ADD_MUTUAL_INTIMACY: //���ӱ˴����ܶ�
		{
			const char* pFriendUid = args.StringVal(2);
			int addIntimacy = args.IntVal(3);
			m_pFriendModule->OnCommandAddFriendMutualIntiMacy(pKernel, self, pFriendUid, addIntimacy);

		}break;
		case SS_FRIEND_ADD_INTIMACY: //�������ܶ�
		{
			const char* pSenderUid = args.StringVal(2);
			int addIntimacy = args.IntVal(3);
			bool addDaily = args.IntVal(4) != 0 ?true:false; //�Ƿ�����ÿ�ջ�ȡ���ܶ�ֵ
			m_pFriendModule->OnCommandAddFriendIntiMacy(pKernel, self, pSenderUid, addIntimacy, addDaily);
		}break;

		case SS_FRIEND_UPDATE_TEAM_ID: //���º��Ѷ���id
		{
			m_pFriendModule->OnCommandUpdateFriendTeamID(pKernel, self, args);

		}break;
		case SS_FRIEND_UPDATE_VIP:
		{
			m_pFriendModule->OnCommandUpdatetVip(pKernel, self, args);
		}break;
		case SS_FRIEND_APPLAY_SWORN: //�������
		{
			m_pFriendModule->OnCommandApplaySworn(pKernel, self, args);
		}break;
		case SS_FRIEND_APPLAY_SWORN_RSP:
		{
			m_pFriendModule->OnCommandApplaySwornRsp(pKernel, self, args);

		}break;
		case SS_FRIEND_SWORN_SUCCESS:
		{
			m_pFriendModule->OnCommandSwornSuccess(pKernel, self, args);
		}break;
		case SS_FRIEND_DEL_SWORN:
		{
			m_pFriendModule->OnCommandDelSworn(pKernel, self, args);
		}break;
		case SS_FRIEND_AID_TARGET_POS_REQ:
		{
			m_pFriendModule->OnCommandAidGetPosReq(pKernel, self, args);
		}break;
		case SS_FRIEND_AID_TARGET_POS_RSP:
		{
			m_pFriendModule->OnCommandAidGetPosRsp(pKernel, self, args);
		}break;
		case SS_FRIEND_ADD_ONECE_ENEMY:
		{
			CVarList info;
			info.Append(args, 2, 11);
			m_pFriendModule->AddOnceEnemy(pKernel, self, info);

		}break;
// 		case SS_FRIEND_TEAM_ADD_INTIMACY:{
// 			m_pFriendModule->OnCommandTeamAddIntimacy (pKernel, self, args);
// 		}break;
		default:
			break;
	}
	return 0;
}

// ���º��ѵ�����״̬
void FriendModule::OnCommandOnlineUpdate(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	const char * pUid = args.StringVal(2);
	if (StringUtil::CharIsNull(pUid))
	{
		return;
	}
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }

	int OLState = args.IntVal(3);

	IRecord* pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return;
	}

	int nRowIndex = pRec->FindString(COLUMN_FRIEND_REC_UID, pUid);
	if (nRowIndex != -1)
	{

		pRec->SetInt( nRowIndex, COLUMN_FRIEND_REC_ONLINE, OLState);
		// [lihl 2016/08/08] ��������� ��������ʱ��
		pRec->SetInt64(nRowIndex, COLUMN_FRIEND_REC_LAST_TIME, util_get_utc_time());
	}
}

// ɾ������
void FriendModule::OnCommandRemoveFriend(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	const wchar_t* pTargetName = args.WideStrVal(2);
	if (StringUtil::CharIsNull(pTargetName))
	{
		return;
	}
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }

	IRecord* pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return;
	}

	int nRowIndex = pRec->FindWideStr(COLUMN_FRIEND_REC_NAME, pTargetName);
	if (nRowIndex != -1)
	{
		pRec->RemoveRow(nRowIndex);
		LogModule::m_pLogModule->OnFriendEvent(pKernel, self, pTargetName, LOG_FRIEND_DELETE);
		// ֪ͨ�ɾ�ģ����º��ѳɾ�
// 		CVarList achieve_msg;
// 		achieve_msg << COMMAND_ACHIEVE_MSG 
// 			<< SUBTYPE_BASIC_FRIEND_NUMS;
// 		pKernel->CommandByName(pTargetName, achieve_msg);	// ֪ͨ�Է�
// 		pKernel->Command(self, self, achieve_msg);	// ֪ͨ�Լ�
	}
	{
		CVarList sysTip;
		sysTip << pTargetName;
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17102, sysTip);
	}
}

// �����������
void FriendModule::OnCommandAddFriendApply(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }
	const wchar_t * pMyName = pSelfObj->QueryWideStr("Name");
	const wchar_t * pTargetName = args.WideStrVal(3);

	if (StringUtil::CharIsNull(pMyName) ||
		StringUtil::CharIsNull(pTargetName))
	{
		return;
	}

	if ( !m_pFriendModule->IsCanAddFriend( pKernel, self, pTargetName, false, true) )
	{
		return;
	}

	CVarList var;
	var.Append(args, 2, 10);

	m_pFriendModule->AddFriendApply(pKernel, self, var);
}

// ���ѵȼ�����
void FriendModule::OnCommandFriendLevelUpdate(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	//����
	const char * pTargetUid = args.StringVal(2);
	if (StringUtil::CharIsNull(pTargetUid))
	{
		return;
	}
	//�ȼ�
	const int nTargetLevel = args.IntVal(3);
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }

	//���ѱ��б�
	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return;
	}

	//ˢ�º��ѱ�
	int nRowIndex = pRec->FindString(COLUMN_FRIEND_REC_UID, pTargetUid);
	if (nRowIndex != -1)
	{
		pRec->SetInt( nRowIndex, COLUMN_FRIEND_REC_LEVEL, nTargetLevel);
	}
}

// ����ս��������
void FriendModule::OnCommandFriendBattleAbility(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	//����
	const char * pTargetUid = args.StringVal(2);
	if (StringUtil::CharIsNull(pTargetUid))
	{
		return;
	}
	//ս����
	const int nTargetAbility= args.IntVal(3);

    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }
	//���ѱ��б�
	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return;
	}

	//ˢ�º��ѱ�
	int nRowIndex = pRec->FindString(COLUMN_FRIEND_REC_UID, pTargetUid);
	if (nRowIndex != -1)
	{
		pRec->SetInt( nRowIndex, COLUMN_FRIEND_REC_POWER, nTargetAbility);
	}
}

// ���Ѱ�����
void FriendModule::OnCommandFriendGuildName(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	//����
	const char * pTargetUid = args.StringVal(2);
	if (StringUtil::CharIsNull(pTargetUid))
	{
		return;
	}
	//���
	const wchar_t *pGuildName = args.WideStrVal(3);

    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }
	//���ѱ��б�
	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return;
	}

	//ˢ�º��ѱ�
	int nRowIndex = pRec->FindString(COLUMN_FRIEND_REC_UID, pTargetUid);
	if (nRowIndex != -1)
	{
		pRec->SetWideStr( nRowIndex, COLUMN_FRIEND_REC_GUILD_NAME, pGuildName);
	}
}

// ����һ������
void FriendModule::UpdateOneFriend(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	if (args.GetCount() < 11)
	{
		return;
	}
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }
	const char *	pTargetUid = args.StringVal(0);
	const wchar_t*	pTargetName = args.WideStrVal(1);

	 
	if (StringUtil::CharIsNull(pTargetName) ||
		StringUtil::CharIsNull(pTargetUid) )
	{
		return;
	}

	IRecord* rec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (rec == NULL)
	{
		return;
	}

	int row = rec->FindString(COLUMN_FRIEND_REC_UID, pTargetUid);
	if (row < 0)
	{
		return;
	}
	//rec->SetString(row,COLUMN_FRIEND_REC_UID,pTargetUid);
	rec->SetWideStr(row,COLUMN_FRIEND_REC_NAME,pTargetName);
	rec->SetInt(row,COLUMN_FRIEND_REC_LEVEL,args.IntVal(2));
	rec->SetInt(row,COLUMN_FRIEND_REC_JOB,args.IntVal(3));
	rec->SetInt(row,COLUMN_FRIEND_REC_SEX,args.IntVal(4));
	rec->SetInt(row,COLUMN_FRIEND_REC_POWER,args.IntVal(5));
	rec->SetWideStr(row, COLUMN_FRIEND_REC_GUILD_NAME, args.WideStrVal(6));
	rec->SetInt64(row, COLUMN_FRIEND_REC_LAST_TIME, args.Int64Val(7));
	rec->SetInt(row, COLUMN_FRIEND_REC_SUM_FLOWER, args.IntVal(8));
	rec->SetInt(row, COLUMN_FRIEND_REC_WEEK_FLOWER, args.IntVal(9));
	rec->SetInt(row, COLUMN_FRIEND_REC_VIP_LEVEL, args.IntVal(10));
	//rec->SetRowValue(row, rowvalue);
}

// ����һ������
void FriendModule::UpdateOneEnmey(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	if (args.GetCount() < 8)
	{
		return;
	}
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }
	const char *	pTargetUid = args.StringVal(0);
	const wchar_t*	pTargetName = args.WideStrVal(1);

	if (StringUtil::CharIsNull(pTargetName) ||
		StringUtil::CharIsNull(pTargetUid))
	{
		return;
	}

	IRecord* rec = pSelfObj->GetRecord(FIELD_RECORD_ENEMY_REC);
	if (rec == NULL)
	{
		return;
	}

	int row = rec->FindString(COLUMN_ENEMY_REC_UID, pTargetUid);
	if (row < 0)
	{
		return;
	}

	rec->SetWideStr(row, COLUMN_ENEMY_REC_NAME, pTargetName);
	rec->SetInt(row, COLUMN_ENEMY_REC_LEVEL, args.IntVal(2));
	rec->SetInt(row, COLUMN_ENEMY_REC_JOB, args.IntVal(3));
	rec->SetInt(row, COLUMN_ENEMY_REC_SEX, args.IntVal(4));
	rec->SetInt(row, COLUMN_ENEMY_REC_POWER, args.IntVal(5));
	rec->SetWideStr(row, COLUMN_ENEMY_REC_GUILD_NAME, args.WideStrVal(6));
	rec->SetInt(row, COLUMN_ENEMY_REC_VIP_LEVEL, args.IntVal(10));
	//rec->SetRowValue(row, rowvalue);
}

// ����һ����������Ϣ
void FriendModule::UpdateBlackPlayer(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	if (args.GetCount() < 8)
	{
		return;
	}
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }
	const char *	pTargetUid = args.StringVal(0);
	const wchar_t*	pTargetName = args.WideStrVal(1);

	 
	if (StringUtil::CharIsNull(pTargetName) ||
		StringUtil::CharIsNull(pTargetUid))
	{
		return;
	}

	IRecord* rec = pSelfObj->GetRecord(FIELD_RECORD_BLACKLIST_REC);
	if (rec == NULL)
	{
		return;
	}

	int row = rec->FindString(COLUMN_BLACKLIST_REC_UID, pTargetUid);
	if (row < 0)
	{
		return;
	}

	rec->SetWideStr(row, COLUMN_BLACKLIST_REC_NAME, pTargetName);	// name
	rec->SetInt(row, COLUMN_BLACKLIST_REC_JOB, args.IntVal(3));	// job
	rec->SetInt(row, COLUMN_BLACKLIST_REC_SEX, args.IntVal(4));	// sex
}

// ����һ�����
void FriendModule::FindOnePlayer(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	if (args.GetCount() < 8)
	{
		return;
	}
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }
	const wchar_t*	pMyName = pSelfObj->QueryWideStr("Name");
	const wchar_t*	pTargetName = args.WideStrVal(1);
	const char *	pTargetUid = args.StringVal(0);

	if (StringUtil::CharIsNull(pMyName) ||
		StringUtil::CharIsNull(pTargetName) ||
		StringUtil::CharIsNull(pTargetUid) ||
		wcscmp(pMyName, pTargetName) == 0)
	{
		return;
	}

	IRecord * pRecommendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_RECOMMEND_REC);
	if (NULL == pRecommendRec)
	{
		return;
	}

	pRecommendRec->ClearRow();
	CVarList rowValue;
	rowValue << pTargetUid
		<< pTargetName
		<< args.IntVal(2)		// level
		<< args.IntVal(3)		// job
		<< args.IntVal(4)		// sex
		<< args.IntVal(5);		// power
	const wchar_t* pGuildName = args.WideStrVal(6);	// guild name
	if (NULL != pGuildName)
	{
		rowValue << pGuildName;
	}
	else
	{
		rowValue << "";
	}
	// vip
	rowValue << args.IntVal(10);
	pRecommendRec->AddRowValue(-1, rowValue);
}

//��Ӻ��ѷ���
void FriendModule::OnCommandAddFriendResult(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
    int result = args.IntVal(2);//0-�ܾ� 1-����
    const char * pTargetUid = args.StringVal(3);
    const wchar_t * pTargetName = args.WideStrVal(4);
    int applyType = args.IntVal(12);
    if (StringUtil::CharIsNull(pTargetName) ||
        StringUtil::CharIsNull(pTargetUid))
    {
        return;
    }
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return;
    }

    IRecord* pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
    if (NULL == pRec )
    {
        return;
    }

	bool bOK = true;
	bool bResult = result != 0;
	if ( !bResult )
    {
		CVarList params;
		params << pTargetName;
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17103, params);
		bOK = false;
    }
	else if (m_pFriendModule->IsInBlackList(pKernel, self, pTargetName))
	{
		// �������е�ֱ��ɾ����֪ͨ
		//::CustomSysInfoByName(pKernel, pTargetName, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_TARGET_FRIEND_FULL, CVarList());
		bOK = false;
	}
	else
	{
		int nRowIndex = pRec->FindString(COLUMN_FRIEND_REC_UID, pTargetUid);
		if (nRowIndex == -1)
		{
			if (pRec->GetRows() >= pRec->GetRowMax())
			{
				bOK = false;
				::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17104, CVarList());
			}
			else
			{

				int teamID = 0;
				int vipLv = 0;
				IRecord * applayRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_APPLY_REC);
				if (applayRec != NULL){
					int row = applayRec->FindString(COLUMN_FRIEND_APPLY_REC_UID, pTargetUid);
					if (row >= 0){
						vipLv = applayRec->QueryInt(row, COLUMN_FRIEND_APPLY_REC_VIP_LEVEL);
					}
				}
				//�����Ƽ������
				IRecord * pOnlinePlayerRec = m_pFriendModule->GetPubOnlineRec(pKernel);
				if (NULL != pOnlinePlayerRec)
				{
					int row = pOnlinePlayerRec->FindString(PUB_COL_PLAYER_UID, pTargetUid);
					if (row>=0)
					{
						teamID = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_TEAM_ID);
						vipLv = pOnlinePlayerRec->QueryInt(row, PUB_COL_PLAYER_VIP);
					}
				}

				

				CVarList row_value;
				row_value.Append(args, 3, 9);
				row_value << util_get_utc_time()	// ����ʱ��
					<< 1			// ���ܶȵȼ�
					<< 0			// ���ܶȾ���
					<< 0			// �ܻ���
					<< 0			// ���ܻ���
					<< 0			// ���ͻ���
					<< 0			// �Է�����
					<< teamID			//����teamID
					<< FRIEND_RELATION_NOMAL		//��ϵ
					<< L""							//��ν
					<< int64_t(0)  // Ѱ��Ԯ��ʱ��
					<< vipLv			//vip
					<< 0;            //ÿ���������ܶ�ֵ
				pRec->AddRowValue(-1, row_value);
				//pKernel->Command(self, self, CVarList() << COMMAND_SEVEN_DAY_OBJ << COMMAND_SEVEN_DAY_OBJ_ADD_FRIEND);
				// ֪ͨ�Է��Լ�����
				CVarList var;
				var << COMMAND_FRIEND_MSG
					<< SS_FRIEND_ONLINE_STATUS_UPDATE
					<< pSelfObj->QueryString(FIELD_PROP_UID)
					<< ONLINE;
				pKernel->CommandByName(pTargetName, var);

				//��Ӻ�����־
				LogModule::m_pLogModule->OnFriendEvent(pKernel, self, pTargetName, LOG_FRIEND_ADD);
				// ֪ͨ�ɾ�ģ����º��ѳɾ�
// 				CVarList achieve_msg;
// 				achieve_msg << COMMAND_ACHIEVE_MSG
// 					<< SUBTYPE_BASIC_FRIEND_NUMS;
// 				pKernel->Command(self, self, achieve_msg);	// ֪ͨ�Լ�
// 
// 				// ����������Ӻ�������
// 				CVarList s2s_msg;
// 				s2s_msg << COMMAND_QUEST_COMMAND
// 						<< COMMAND_QUEST_SUB_UPDATE
// 						<< CRITERIA_TYPE_FRIEDN_ACTIVE_ADD
// 						<< 1;
// 				pKernel->Command(self, self, s2s_msg);

				// �ɹ���Ӻ���  ���º�����Ϣ
				if (SnsDataModule::m_pSnsDataModule)
				{
					CVarList query_args;
					query_args << SNS_DATA_REQUEST_FORM_FRIEND_UPDATE;
					query_args << pTargetName;
					query_args << 1;
					query_args << "PlayerAttribute";
					SnsDataModule::OnQueryPlayerData(pKernel, self, self, query_args);
				}

				// ��ʾ֪ͨ
				CVarList params;
				params << pTargetName;
				::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17105, params);
			}
		}
	}

	//�Ƴ�������Ϣ
	m_pFriendModule->DeleteFriendApply(pKernel, self, CVarList() << pTargetUid << applyType);
	//�Ƴ������
	m_pFriendModule->TestRemovePlayerInvited(pKernel, self, pTargetUid, true);

	// ���ʧ��֪ͨ�Է�Ҳɾ��
	if (bResult && !bOK)
	{
		//�Ƿ�����
		if (CheckPlayerOnline(pKernel, pTargetName))
		{
			pKernel->CommandByName(pTargetName, 
				CVarList() << COMMAND_FRIEND_MSG << SS_FRIEND_REMOVE_PLAYER << pSelfObj->QueryWideStr(FIELD_PROP_NAME));
			//::CustomSysInfoByName(pKernel, pTargetName, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_TARGET_FRIEND_FULL, CVarList());
		}
		else
		{
			// Ŀ�겻���ߣ�������Ϣ��SNS����������¼ɾ��
			pKernel->SendSnsMessage(pKernel->GetDistrictId(),
				pKernel->GetServerId(),
				pKernel->GetMemberId(),
				pKernel->GetSceneId(),
				pSelfObj->QueryString(FIELD_PROP_UID),
				CVarList() << SPACE_SNS
				<< SNS_FRIEND_SPACE
				<< SNS_MSG_FRIEND_DELETE
				<< pSelfObj->QueryString( FIELD_PROP_UID )
				<< pSelfObj->QueryString(FIELD_PROP_NAME)
				<< pTargetUid
				<< pTargetName);
		}
	}
}


// �ɹ����ͻ�
void FriendModule::OnSendFlower(IKernel* pKernel, IGameObj* pSelf,
				const char* pszTargetUid, int nFlowers)
{
	if (NULL == pSelf || NULL == pszTargetUid )
	{
		return;
	}

	IRecord* pFriendRec = pSelf->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec)
	{
		return;
	}

	// ���ѹ�ϵ���
	int nFriendIndex = pFriendRec->FindString(COLUMN_FRIEND_REC_UID, pszTargetUid);
	if (nFriendIndex == -1)
	{
		return;
	}

	// �������ܶ�
	int nFlowerIntimacyUnit = EnvirValueModule::EnvirQueryInt(ENV_VALUE_FLOWER_TO_INTIMACY_UNIT);
	if (nFlowerIntimacyUnit < 1)
	{
		nFlowerIntimacyUnit = 1;
	}
	int nExp = nFlowers * nFlowerIntimacyUnit;
	int nIntLevel = IncFriendIntimacy(pKernel, pSelf, pFriendRec, nFriendIndex, nExp);

	// ��д�ͻ���¼
	int nSumFlower = pFriendRec->QueryInt(nFriendIndex, COLUMN_FRIEND_REC_SUM_FLOWER) + nFlowers;
	int nWeekFlower = pFriendRec->QueryInt(nFriendIndex, COLUMN_FRIEND_REC_WEEK_FLOWER) + nFlowers;
	int nMySendFlower = pFriendRec->QueryInt(nFriendIndex, COLUMN_FRIEND_REC_SEND_FLOWER) + nFlowers;
	pFriendRec->SetInt(nFriendIndex, COLUMN_FRIEND_REC_SUM_FLOWER, nSumFlower);
	pFriendRec->SetInt(nFriendIndex, COLUMN_FRIEND_REC_WEEK_FLOWER, nWeekFlower);
	pFriendRec->SetInt(nFriendIndex, COLUMN_FRIEND_REC_SEND_FLOWER, nMySendFlower);

	const wchar_t* pszTargetName = pFriendRec->QueryWideStr(nFriendIndex, COLUMN_FRIEND_REC_NAME);
	IRecord* pSendRecord = pSelf->GetRecord(FIELD_RECORD_FRIEND_SEND_FLOWER_REC);
	if (NULL != pSendRecord)
	{
		// ������� ɾ������һ����¼
		if ( pSendRecord->GetRows() == pSendRecord->GetRowMax() )
		{
			pSendRecord->RemoveRow(0);
		}

		CVarList args;
		args << pszTargetUid << pszTargetName << nFlowers 
			<< pFriendRec->QueryInt(nFriendIndex, COLUMN_FRIEND_REC_JOB)
			<< pFriendRec->QueryInt(nFriendIndex, COLUMN_FRIEND_REC_SEX);
		pSendRecord->AddRowValue(-1, args);
	}

	LogModule::m_pLogModule->OnFriendSendGift(pKernel, pSelf->GetObjectId(), pszTargetName, nFlowers, nIntLevel, nExp );
	// ������ʾ
	::CustomSysInfo(pKernel, pSelf->GetObjectId(), SYSTEM_INFO_ID_17106, CVarList());
	//pKernel->Command(pSelf->GetObjectId(), pSelf->GetObjectId(), CVarList() << COMMAND_SEVEN_DAY_OBJ << COMMAND_SEVEN_DAY_OBJ_FRIEND_SEND_GIFT);
	// ����������������
// 	CVarList s2s_msg;
// 	s2s_msg << COMMAND_QUEST_COMMAND
// 			<< COMMAND_QUEST_SUB_UPDATE
// 			<< CRITERIA_TYPE_FRIEND_GIVE_GIFT
// 			<< 1;
// 	pKernel->Command(pSelf->GetObjectId(), pSelf->GetObjectId(), s2s_msg);
// 
// 	// �������������Ծ��
// 	s2s_msg.Clear();
// 	s2s_msg << COMMAND_DAILY_ACTIVITY
// 			<< DAILY_ACTIVITY_TYPE_FRIEND_GIFT
// 			<< 1;
// 	pKernel->Command(pSelf->GetObjectId(), pSelf->GetObjectId(), s2s_msg);
}
// �ɹ��յ���
void FriendModule::OnRecvFlower(IKernel* pKernel, IGameObj* pSelf, 
					const char* pszSenderUid, int nFlowers)
{
	if (NULL == pSelf || StringUtil::CharIsNull(pszSenderUid) )
	{
		return;
	}

	IRecord* pFriendRec = pSelf->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec)
	{
		return;
	}

	// ���ѹ�ϵ���
	int nFriendIndex = pFriendRec->FindString(COLUMN_FRIEND_REC_UID, pszSenderUid);
	if (nFriendIndex == -1)
	{
		return;
	}

	// �������ܶ�
	int nFlowerIntimacyUnit = EnvirValueModule::EnvirQueryInt(ENV_VALUE_FLOWER_TO_INTIMACY_UNIT);
	if (nFlowerIntimacyUnit < 1)
	{
		nFlowerIntimacyUnit = 1;
	}
	int nExp = nFlowers * nFlowerIntimacyUnit;
	int nIntLevel = IncFriendIntimacy(pKernel, pSelf, pFriendRec, nFriendIndex, nExp);

	// �޸����������ֵ
	int nMySumFlower = pSelf->QueryInt(FIELD_PROP_SUM_FLOWER) + nFlowers;
	int nMyWeekFlower = pSelf->QueryInt(FIELD_PROP_WEEK_FLOWER) + nFlowers;
	pSelf->SetInt(FIELD_PROP_SUM_FLOWER, nMySumFlower);
	pSelf->SetInt(FIELD_PROP_WEEK_FLOWER, nMyWeekFlower);

	try
	{
		// ���а�����ֵ�޸�
		int nFlowerCharmUnit = EnvirValueModule::EnvirQueryInt(ENV_VALUE_FLOWER_TO_CHARM_UNIT);
		if (nFlowerCharmUnit < 1)
		{
			nFlowerCharmUnit = 1;
		}
		int nCharms = nFlowerCharmUnit * nMySumFlower;
		CVarList rank;
		rank << 0
			<< PUB_PLAYER_CHARM_RANK_NAME
			<< INSERT_IF_NOT_EXIST
			<< "player_uid" << pSelf->QueryString(FIELD_PROP_UID)
			<< "player_name" << pSelf->QueryWideStr(FIELD_PROP_NAME)
			<< "player_career" << pSelf->QueryInt(FIELD_PROP_JOB)
			<< "player_sex" << pSelf->QueryInt(FIELD_PROP_SEX)
			<< "player_level" << pSelf->QueryInt(FIELD_PROP_LEVEL)
			<< "player_charm" << nCharms
			<< "player_vip_level" << pSelf->QueryInt(FIELD_PROP_VIP_LEVEL);
		RankListModule::UpdateEntry(pKernel, pSelf->GetObjectId(), rank);

		// ��¼�µ�����ֵ
		int nAddCharms = nFlowers * nFlowerCharmUnit;
		LogModule::m_pLogModule->OnFriendCharmChanged(pKernel, pSelf->GetObjectId(), nAddCharms, nCharms, LOG_FRIEND_GIVE);
	}
	catch (...)
	{
		::extend_warning(LOG_ERROR, "[FriendModule::OnRecvFlower] update charm rank exception");
	}

	// ��д�ջ���¼
	int nMyRecvFlower = pFriendRec->QueryInt(nFriendIndex, COLUMN_FRIEND_REC_RECEIVE_FLOWER) + nFlowers;
	pFriendRec->SetInt(nFriendIndex, COLUMN_FRIEND_REC_RECEIVE_FLOWER, nMyRecvFlower);

	const wchar_t* pszSenderName = pFriendRec->QueryWideStr(nFriendIndex, COLUMN_FRIEND_REC_NAME);
	if (!StringUtil::CharIsNull(pszSenderName))
	{
		IRecord* pRecvRecord = pSelf->GetRecord(FIELD_RECORD_FRIEND_RECEIVE_FLOWER_REC);
		if (NULL != pRecvRecord)
		{
			// ������� ɾ������һ����¼
			if (pRecvRecord->GetRows() == pRecvRecord->GetRowMax())
			{
				pRecvRecord->RemoveRow(0);
			}

			CVarList args;
			args << pszSenderUid << pszSenderName << nFlowers
				<< pFriendRec->QueryInt(nFriendIndex, COLUMN_FRIEND_REC_JOB)
				<< pFriendRec->QueryInt(nFriendIndex, COLUMN_FRIEND_REC_SEX);
			pRecvRecord->AddRowValue(-1, args);

			// ������ʾ
			::CustomSysInfo(pKernel, pSelf->GetObjectId(), SYSTEM_INFO_ID_17107, CVarList() << pszSenderName);
		}
	}
	
	// ��¼��־
	LogModule::m_pLogModule->OnFriendRecvGift(pKernel, pSelf->GetObjectId(), pszSenderName, nFlowers, nIntLevel, nExp);
	// ֪ͨ����ˢ��
	OnPlayerGiftChanged(pKernel, pSelf, nMySumFlower, nMyWeekFlower);
}

// �������ܶȴ���
int FriendModule::IncFriendIntimacy(IKernel* pKernel, IGameObj* pSelf, IRecord* pRecord, int nRowIndex, int nExp, bool isAddDaily)
{
	if (NULL == pKernel || NULL == pRecord)
	{
		return 0;
	}

	int nIntLevel = pRecord->QueryInt(nRowIndex, COLUMN_FRIEND_REC_INTIMACY_LEVEL);
	int nIntExp = pRecord->QueryInt(nRowIndex, COLUMN_FRIEND_REC_INTIMACY_EXP);
	if (nIntLevel < 1)
	{
		nIntLevel = 1;
	}
	
	int relation = pRecord->QueryInt(nRowIndex, COLUMN_FRIEND_REC_RELATION);
	int intimacyMaxLevel = 0;
	if (relation == FRIEND_RELATION_NOMAL)
	{
		intimacyMaxLevel = EnvirValueModule::EnvirQueryInt(ENV_VALUE_NOMAL_FRIEND_INTIMACY_MAX);
	}
	else if (relation == FRIEND_RELATION_SWORN)
	{
		intimacyMaxLevel = EnvirValueModule::EnvirQueryInt(ENV_VALUE_SWORN_FRIEND_INTIMACY_MAX);

	}
	if (isAddDaily)
	{
		int dailyIntimacyRow = pRecord->QueryInt(nRowIndex, COLUMN_FRIEND_REC_DAILY_INTIMACY);
		int limit = GetDailyAddIntimacyLimit();

		if (dailyIntimacyRow >= limit)
		{
			return nIntLevel;
		}

		if (dailyIntimacyRow + nExp > limit){
			nExp = limit - dailyIntimacyRow;
		}


	}

	ConfigFriendIntimacyUpgradeItem* pUpgradeItem = Configure<ConfigFriendIntimacyUpgradeItem>::Find(nIntLevel);
	if (NULL == pUpgradeItem)
	{
		// ���������Ϣ
		::extend_warning(LOG_ERROR, "[FriendModule::IncFriendIntimacy]intimacy level(%d) not found", nIntLevel);
	}
	else
	{
		int nUpExp = pUpgradeItem->GetExp();
		int nNewExp = nIntExp + nExp;
		while (nNewExp >= nUpExp)
		{

			int nNewLevel = nIntLevel + 1;
			if (!IsCanAddIntimacy(nIntLevel, intimacyMaxLevel, nNewExp))
			{
				nNewExp = pUpgradeItem->GetExp();
				break;
			}
			ConfigFriendIntimacyUpgradeItem* pUpgradeNext = Configure<ConfigFriendIntimacyUpgradeItem>::Find(nNewLevel);
			if (NULL == pUpgradeNext)
			{
				nNewExp = pUpgradeItem->GetExp();
				break;
			}
			else
			{
				nNewExp -= nUpExp;
				nUpExp = pUpgradeNext->GetExp();
				nIntLevel = nNewLevel;
				pRecord->SetInt(nRowIndex, COLUMN_FRIEND_REC_INTIMACY_LEVEL, nNewLevel);
// 				CVarList s2s_seven_obj_msg;
// 				s2s_seven_obj_msg << COMMAND_SEVEN_DAY_OBJ
// 					<< COMMAND_SEVEN_DAY_OBJ_NTIMACY_LEVEL << nNewLevel;
// 				auto self = pSelf->GetObjectId() ;
// 				pKernel->Command(self, self, s2s_seven_obj_msg);
				
			}
		}
		pRecord->SetInt(nRowIndex, COLUMN_FRIEND_REC_INTIMACY_EXP, nNewExp);

		if (isAddDaily)
		{
			int dailyIntimacyRow = pRecord->QueryInt(nRowIndex, COLUMN_FRIEND_REC_DAILY_INTIMACY);
			dailyIntimacyRow += nExp;
			pRecord->SetInt(nRowIndex, COLUMN_FRIEND_REC_DAILY_INTIMACY, dailyIntimacyRow);
		}

		// �ܳ����ּ���
// 		CVarList s2s_msg;
// 		s2s_msg << COMMAND_WEEKLY_SCORE
// 				<< S2S_WEEKLY_SCORE_SUBMSG_INCC_SCORE
// 				<< WEEKLY_SCORE_EVENT_TYPE_FRIENDLY
// 				<< nExp;
// 		pKernel->Command(pSelf->GetObjectId(), pSelf->GetObjectId(), s2s_msg);
	}

	return nIntLevel;
}

void  FriendModule::IncTeamFriendIntimacy(IKernel* pKernel, IGameObj* pSelf, const CVarList& friendList, int nExp)
{
	const wchar_t* pMyName = pSelf->QueryWideStr(FIELD_PROP_NAME);
	if ( StringUtil::CharIsNull(pMyName))
		return;

	const char* playerUid = pKernel->SeekRoleUid(pMyName);
	IRecord * pFriendRec = pSelf->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec)
	{
		return;
	}
	
	int nCount = friendList.GetCount();
	LoopBeginCheck(a);
	for (int i = 0; i < nCount; ++i)
	{
		LoopDoCheck(a);
		const wchar_t* pFriendName = friendList.WideStrVal(i);
		int nRow = pFriendRec->FindWideStr(COLUMN_FRIEND_REC_NAME, pFriendName);
		if (nRow != -1)
		{
			const char* friendUid = pFriendRec->QueryString(nRow, COLUMN_FRIEND_REC_UID);
			{
				CVarList args;
				args << COMMAND_FRIEND_MSG << SS_FRIEND_ADD_INTIMACY << friendUid << nExp << 1;
				pKernel->CommandByName(pMyName, args);
			}
			{
				CVarList args;
				args << COMMAND_FRIEND_MSG << SS_FRIEND_ADD_INTIMACY << playerUid << nExp << 1;
				pKernel->CommandByName(pFriendName, args);

			}

			/*int nLevel = IncFriendIntimacy(pKernel, pSelf, pFriendRec, nRow, nExp,true);*/
			// ��¼��־
			//LogModule::m_pLogModule->OnFriendTeamFight(pKernel, pSelf->GetObjectId(), pFriendName, nLevel, nExp);
		}
	}
}



int FriendModule::ResetWeekFlowerNum(IKernel* pKernel, const PERSISTID& self, int slice)
{
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL != pSelf)
	{
		m_pFriendModule->ResetFriedFlower(pKernel, pSelf);
	}
	return 0;
}



void FriendModule::ResetFriedFlower( IKernel* pKernel, IGameObj* pSelf )
{
	pSelf->SetInt(FIELD_PROP_WEEK_FLOWER, 0);
}

void FriendModule::OnCommandApplaySworn(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	const wchar_t*applayName = args.WideStrVal(2);
	int applaySex = args.IntVal(3);
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}

	if (!IsFriend(pKernel, self, applayName)){
		return;
	}


	IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec){
		return;
	}

	int swornSexNum = GetSwornNum(pKernel, self, applaySex);
// 	if (swornSexNum >= VipModule::m_pVipModule->GetNumber(pKernel, self, VIP_FRIEND_SWORN_NUM))
// 	{
// 		if (applaySex == pSelfObj->QueryInt(FIELD_PROP_SEX))
// 		{
// 			::CustomSysInfoByName(pKernel, applayName, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_FRIEND_SWORN_FULL1, CVarList());
// 		}
// 		else
// 		{
// 			::CustomSysInfoByName(pKernel, applayName, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_FRIEND_SWORN_FULL2, CVarList());
// 		}
// 
// 		return;
// 	}
// 	else
// 	{
// 		if (applaySex == pSelfObj->QueryInt(FIELD_PROP_SEX))
// 		{
// 			::CustomSysInfoByName(pKernel, applayName, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRIEND_APPLAY_SWORN_SUCCESS1, CVarList());
// 		}
// 		else
// 		{
// 			::CustomSysInfoByName(pKernel, applayName, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRIEND_APPLAY_SWORN_SUCCESS2, CVarList());
// 		}
// 
// 	}

	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_FRIEND << S2C_APPLAY_SWORN << applayName);

}

void FriendModule::OnCommandApplaySwornRsp(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	int result = args.IntVal(2);
	const wchar_t* targetName = args.WideStrVal(3);
	int targetSex = args.IntVal(4);
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}

	bool sameSex = targetSex == pSelfObj->QueryInt(FIELD_PROP_SEX)?true:false;
	

	do {

		IRecord *pFriendApplaySworn = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_APPLAY_SWORN_REC);
		if (pFriendApplaySworn == NULL){
			return;
		}
		int row = pFriendApplaySworn->FindWideStr(COLUMN_FRIEND_APPLAY_SWORN_REC_NAME, targetName);
		if (row < 0){
			break;
		}
		pFriendApplaySworn->RemoveRow(row);
		if (result == 1)
		{
			std::string spendItemID;

			 if (sameSex)
			 {
				 spendItemID = EnvirValueModule::EnvirQueryString(ENV_VALUE_SWORN_NEED_ITEM1);
			 }
			 else
			 {
				 spendItemID = EnvirValueModule::EnvirQueryString(ENV_VALUE_SWORN_NEED_ITEM2);
			 }
			
			IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
			if (NULL == pFriendRec){
				return;
			}

			int nRow = pFriendRec->FindWideStr(COLUMN_FRIEND_REC_NAME, targetName);
			if (nRow < 0){
				::CustomSysInfoByName(pKernel, targetName, SYSTEM_INFO_ID_17108, CVarList() << pSelfObj->QueryWideStr(FIELD_PROP_NAME));
				break;
			}

			// ȡ�ñ���
			PERSISTID item_box = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
			if (!pKernel->Exists(item_box)){
				break;
			}

			if (!ContainerModule::m_pContainerModule->TryRemoveItems(pKernel, item_box, spendItemID.c_str(), 1)){
				break;
			}

// 			int intimacyLevel = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_INTIMACY_LEVEL);
// 			if (intimacyLevel < EnvirValueModule::EnvirQueryInt(ENV_VALUE_SWORN_INTIMACY_LEVEL)){
// 				break;
// 			}
			if (!ContainerModule::m_pContainerModule->RemoveItems(pKernel, item_box, spendItemID.c_str(), 1, FUNCTION_EVENT_ID_FRIEND)){
				break;
			}

			pFriendRec->SetInt(nRow, COLUMN_FRIEND_REC_RELATION, FRIEND_RELATION_SWORN);

			if (targetSex == pSelfObj->QueryInt(FIELD_PROP_SEX))
			{
				::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17109, CVarList() << targetName);
			}
			else
			{
				::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17110, CVarList() << targetName);
			}
			pKernel->CommandByName(targetName, CVarList() << COMMAND_FRIEND_MSG << SS_FRIEND_SWORN_SUCCESS << pSelfObj->QueryWideStr(FIELD_PROP_NAME));
			return;
		}
	} while (false);

	if (result == 1)
	{
		if (sameSex)
		{
			::CustomSysInfoByName(pKernel, targetName, SYSTEM_INFO_ID_17113, CVarList());
		}
		else
		{
			::CustomSysInfoByName(pKernel, targetName, SYSTEM_INFO_ID_17114, CVarList());
		}

		LogModule::m_pLogModule->OnSworn(pKernel, self, targetName, sameSex, FRIEND_SWORN_ADD);
	}
	else
	{
		if (sameSex)
		{
			::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17111, CVarList() << targetName);
		}
		else
		{
			::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17112, CVarList() << targetName);
		}
	}
	return;
}
void FriendModule::OnCommandSwornSuccess(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	const wchar_t * targetName = args.WideStrVal(2);
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}
	IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec){
		return;
	}

	int nRow = pFriendRec->FindWideStr(COLUMN_FRIEND_REC_NAME, targetName);
	if (nRow < 0){
		return;
	}
	pFriendRec->SetInt(nRow, COLUMN_FRIEND_REC_RELATION, FRIEND_RELATION_SWORN);
	int targetSex = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_SEX);
	int sameSex = targetSex == pSelfObj->QueryInt(FIELD_PROP_SEX)?true:false;
	if (sameSex)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17109, CVarList() << targetName);
	}
	else
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17110, CVarList() << targetName);
	}

	LogModule::m_pLogModule->OnSworn(pKernel, self, targetName, sameSex, FRIEND_SWORN_ADD);

}
void FriendModule::OnCommandDelSworn(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	const wchar_t* swornName = args.WideStrVal(2);
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return;
	}
	IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec){
		return;
	}
	int nRow = pFriendRec->FindWideStr(COLUMN_FRIEND_REC_NAME, swornName);
	if (nRow < 0){
		return;
	}
	int intimacyLevel = EnvirValueModule::EnvirQueryInt(ENV_VALUE_NOMAL_FRIEND_INTIMACY_MAX);
	pFriendRec->SetInt(nRow, COLUMN_FRIEND_REC_RELATION, FRIEND_RELATION_NOMAL);
	pFriendRec->SetInt(nRow, COLUMN_FRIEND_REC_INTIMACY_LEVEL, intimacyLevel);
	int friendSex = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_SEX);
	pFriendRec->SetInt(nRow, COLUMN_FRIEND_REC_INTIMACY_EXP, 0);
	bool isSameSex = friendSex == pSelfObj->QueryInt(FIELD_PROP_SEX) ? true : false;
	if (isSameSex)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17115, CVarList() << swornName);
	}
	else
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17116, CVarList() << swornName);
	}
	LogModule::m_pLogModule->OnSworn(pKernel, self, swornName, isSameSex, FRIEND_SOWRN_DEL);
}

bool FriendModule::IsCanSworn(IKernel*pKernel, const PERSISTID& self, const wchar_t* targetName)
{

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL){
		return false;
	}
	if (pKernel->GetPlayerScene(targetName) <= 0)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17007, CVarList() << targetName);
		return false;
	}


	IRecord * pFriendRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec)
	{
		return false;
	}

	int nRow = pFriendRec->FindWideStr(COLUMN_FRIEND_REC_NAME, targetName);
	if (nRow < 0)
	{
		return false;
	}

	int relation = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_RELATION);
	if (relation != FRIEND_RELATION_NOMAL){
		return false;
	}

// 	int friendSex = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_SEX);
// 	int selfSex = pSelfObj->QueryInt(FIELD_PROP_SEX);
// 	int swornSexNum = GetSwornNum(pKernel, self, friendSex);
// 	if (swornSexNum >= VipModule::m_pVipModule->GetNumber(pKernel, self, VIP_FRIEND_SWORN_NUM))
// 	{
// 		if (selfSex == friendSex)
// 		{
// 			::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_FRIEND_SWORN_FULL1, CVarList());
// 		}
// 		else
// 		{
// 			::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_FRIEND_SWORN_FULL2, CVarList());
// 		}
// 
// 		return false;
// 	}
// 
// 
// 	int intimacyLevel = pFriendRec->QueryInt(nRow, COLUMN_FRIEND_REC_INTIMACY_LEVEL);
// 	int needintimacyLevel = EnvirValueModule::EnvirQueryInt(ENV_VALUE_SWORN_INTIMACY_LEVEL);
// 	if (intimacyLevel < needintimacyLevel)
// 	{
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, FRI_FRIEND_INTIMACY_LEVEL_LIMIT, CVarList());
// 		return false;
// 	}


	return true;
}

void FriendModule::OnCommandAidGetPosReq(IKernel* pKernel, const PERSISTID & self, const IVarList & args)
{
	IGameObj* pSceneObj = pKernel->GetGameObj(self);
	if (NULL == pSceneObj)
	{
		return;
	}

	const wchar_t* targetName = args.WideStrVal(2);
	IGameObj* pTargetObj = pKernel->GetGameObj(self);
	if (NULL == pTargetObj){
		return;
	}


	float fPosX = pTargetObj->GetPosiX();
	float y = pTargetObj->GetPosiY();
	float fPosZ = pTargetObj->GetPosiZ();
	float fPosY = GetCurFloorHeight(pKernel, fPosX, y, fPosZ);


	CVarList msg;
	msg << COMMAND_FRIEND_MSG << SS_FRIEND_AID_TARGET_POS_RSP << pKernel->GetSceneId() << fPosX << fPosY << fPosZ;

	pKernel->CommandByName(targetName, msg);


}


void FriendModule::OnCommandAidGetPosRsp(IKernel* pKernel, const PERSISTID & self, const IVarList & args)
{
	int objSceneID = args.IntVal(2);
	float x = args.FloatVal(3);
	float y = args.FloatVal(4);
	float z = args.FloatVal(5);

	int selfSceneID = pKernel->GetSceneId();
	if (selfSceneID == objSceneID)
	{
		pKernel->MoveTo(self, x, y, z, 0);
	}
	else
	{
		AsynCtrlModule::m_pAsynCtrlModule->SwitchLocate(pKernel, self, objSceneID, x, y, z, 0);
	}

}

// void FriendModule::OnCommandSetNation(IKernel* pKernel, const PERSISTID & self, const IVarList & args)
// {
// 	//����
// 	const char * pTargetUid = args.StringVal(2);
// 	if (StringUtil::CharIsNull(pTargetUid))
// 	{
// 		return;
// 	}
// 
// 	int nation = args.IntVal(3);
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (NULL == pSelfObj)
// 	{
// 		return;
// 	}
// 	
// 	//���ѱ��б�
// 	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
// 	if (NULL == pRec)
// 	{
// 		return;
// 	}
// 
// 	//ˢ�º��ѱ�
// 	int nRowIndex = pRec->FindString(COLUMN_FRIEND_REC_UID, pTargetUid);
// 	if (nRowIndex != -1)
// 	{
// 		pRec->SetInt(nRowIndex, COLUMN_FRIEND_REC_NATION, nation);
// 	}
// }


void FriendModule::OnCommandFriendFlowerChanged(IKernel * pKernel, const PERSISTID & self, const char* friendUid, int nSumFlower, int nWeekFlower)
{
	//����
	if (StringUtil::CharIsNull(friendUid))
	{
		return;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	//���ѱ��б�
	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return;
	}

	//ˢ�º��ѱ�
	int nRowIndex = pRec->FindString(COLUMN_FRIEND_REC_UID, friendUid);
	if (nRowIndex != -1)
	{
		pRec->SetInt(nRowIndex, COLUMN_FRIEND_REC_SUM_FLOWER, nSumFlower);
		pRec->SetInt(nRowIndex, COLUMN_FRIEND_REC_WEEK_FLOWER, nWeekFlower);
	}
}

void FriendModule::OnCommandAddFriendMutualIntiMacy(IKernel*pKernel, const PERSISTID&self, const char* firendUid, int addIntimacy)
{
	if (StringUtil::CharIsNull(firendUid))
	{
		return;
	}

	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return;
	}

	const wchar_t* pwstrTargetName = pKernel->SeekRoleName(firendUid);
	if (StringUtil::CharIsNull(pwstrTargetName))
	{

		return;
	}
	const wchar_t* pwstrMyName = pSelf->QueryWideStr(FIELD_PROP_NAME);
	if (StringUtil::CharIsNull(pwstrMyName))
	{
		return;
	}
	if (wcscmp(pwstrMyName, pwstrTargetName) == 0)
	{
		return;
	}

	const char* pstrMyUid = pSelf->QueryString(FIELD_PROP_UID);
	if (StringUtil::CharIsNull(pstrMyUid))
	{
		return;
	}

	// 1. �Ƿ��Ǻ���
	IRecord* pFriendRec = pKernel->GetRecord(self, FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec)
	{
		return;
	}
	int nFriendIndex = pFriendRec->FindString(COLUMN_FRIEND_REC_UID, firendUid);
	if (nFriendIndex == -1)
	{

		return;
	}

	if (!IsCanAddIntimacy(pKernel, pSelf, firendUid))
	{
		return;
	}

	// 4. ��ʼ�������ܶ�
	int nScene = pKernel->GetPlayerScene(pwstrTargetName);
	if (nScene > 0)
	{
		// 4.1 �����ͻ�
		CVarList args;
		args << COMMAND_FRIEND_MSG << SS_FRIEND_ADD_INTIMACY << pstrMyUid << addIntimacy<<0;
		pKernel->CommandByName(pwstrTargetName, args);
	}
	else
	{
		// 4.1 �����߷���sns��������������
		pKernel->SendSnsMessage(pKernel->GetDistrictId(), pKernel->GetServerId(), pKernel->GetMemberId(),
			pKernel->GetSceneId(), firendUid,
			CVarList() << SPACE_SNS
			<< SNS_FRIEND_SPACE
			<< SNS_MSG_FRIEND_PUSH_ADD_INTIMACY
			<< pstrMyUid
			<< addIntimacy);
	}

	IncFriendIntimacy(pKernel, pSelf, pFriendRec, nFriendIndex, addIntimacy);

	::CustomSysInfo(pKernel, pSelf->GetObjectId(), SYSTEM_INFO_ID_17117, CVarList() << pwstrTargetName);


}


void FriendModule::OnCommandAddFriendIntiMacy(IKernel*pKernel, const PERSISTID&self, const char* firendUid, int addIntimacy, bool isAddDaily)
{

	if (StringUtil::CharIsNull(firendUid))
	{
		return;
	}

	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return;
	}

	const wchar_t* pwstrTargetName = pKernel->SeekRoleName(firendUid);
	if (StringUtil::CharIsNull(pwstrTargetName))
	{

		return;
	}
	const wchar_t* pwstrMyName = pSelf->QueryWideStr(FIELD_PROP_NAME);
	if (StringUtil::CharIsNull(pwstrMyName))
	{
		return;
	}
	const char* pstrMyUid = pSelf->QueryString(FIELD_PROP_UID);
	if (StringUtil::CharIsNull(pstrMyUid))
	{
		return;
	}

	// 1. �Ƿ��Ǻ���
	IRecord* pFriendRec = pKernel->GetRecord(self, FIELD_RECORD_FRIEND_REC);
	if (NULL == pFriendRec)
	{
		return;
	}
	int nFriendIndex = pFriendRec->FindString(COLUMN_FRIEND_REC_UID, firendUid);
	if (nFriendIndex == -1)
	{
		return;
	}
	IncFriendIntimacy(pKernel, pSelf, pFriendRec, nFriendIndex, addIntimacy, isAddDaily);
	if (!isAddDaily)
	{
		::CustomSysInfo(pKernel, pSelf->GetObjectId(), SYSTEM_INFO_ID_17117, CVarList() << pwstrTargetName);
	}


}


void FriendModule::OnCommandUpdateFriendTeamID(IKernel * pKernel, const PERSISTID & self, const IVarList & args)
{
	//����
	const char * pTargetUid = args.StringVal(2);
	if (StringUtil::CharIsNull(pTargetUid))
	{
		return;
	}
	//����
	const int teamID = args.IntVal(3);

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}
	//���ѱ��б�
	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return;
	}

	//ˢ�º��ѱ�
	int nRowIndex = pRec->FindString(COLUMN_FRIEND_REC_UID, pTargetUid);
	if (nRowIndex != -1)
	{
		pRec->SetInt(nRowIndex, COLUMN_FRIEND_REC_TEAM_ID, teamID);
	}



}


void FriendModule::OnCommandUpdatetVip(IKernel* pKernel, const PERSISTID & self, const IVarList & args)
{
	//����
	const char * pTargetUid = args.StringVal(2);
	if (StringUtil::CharIsNull(pTargetUid))
	{
		return;
	}
	//����
	const int vipLv = args.IntVal(3);

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}
	//���ѱ��б�
	IRecord * pRec = pSelfObj->GetRecord(FIELD_RECORD_FRIEND_REC);
	if (NULL == pRec)
	{
		return;
	}

	//ˢ�º��ѱ�
	int nRowIndex = pRec->FindString(COLUMN_FRIEND_REC_UID, pTargetUid);
	if (nRowIndex != -1)
	{
		pRec->SetInt(nRowIndex, COLUMN_FRIEND_REC_VIP_LEVEL, vipLv);
	}
}



// int FriendModule::OnCommandTeamAddIntimacy(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
// {
// 	int  type = args.IntVal(2);
// 
// 	int addIntimacy = m_pFriendModule->GetTeamAddIntimacyValue(type);
// 
// 	if (addIntimacy == 0){
// 		return 0;
// 	}
// 
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	if (pSelfObj == NULL){
// 		return 0;
// 	}
// 
// 
// 	int teamID = pSelfObj->QueryInt(FIELD_PROP_TEAM_ID);
// 	if (teamID <= 0){
// 		return 0;
// 	}
// 
// 	
// 	IRecord *pTeamRecord = pSelfObj->GetRecord(TEAM_REC_NAME);
// 	if (pTeamRecord == NULL){
// 		return 0;
// 	}
// 
// 
// 	int count = pTeamRecord->GetRows();
// 	for (int row = 0; row < count; row++)
// 	{
// 		const wchar_t* rowName = pTeamRecord->QueryWideStr(row, TEAM_REC_COL_NAME);
// 		if (pKernel->GetPlayerScene(rowName) <= 0){
// 			continue;
// 		}
// 		const char *rowUid = pTeamRecord->QueryString(row, TEAM_REC_COL_UID);
// 
// 		for (int i = row + 1; i < count; i++)
// 		{
// 			const char* teamMemUid = pTeamRecord->QueryString(i, TEAM_REC_COL_UID);
// 			const wchar_t* teamName = pTeamRecord->QueryWideStr(i, TEAM_REC_COL_NAME);
// 			if (pKernel->GetPlayerScene(rowName) <= 0){
// 				continue;
// 			}
// 
// 			{
// 				CVarList args;
// 				args << COMMAND_FRIEND_MSG << SS_FRIEND_ADD_INTIMACY << teamMemUid << addIntimacy << 1;
// 				pKernel->CommandByName(rowName, args);
// 			}
// 			{
// 				CVarList args;
// 				args << COMMAND_FRIEND_MSG << SS_FRIEND_ADD_INTIMACY << rowUid << addIntimacy << 1;
// 				pKernel->CommandByName(teamName, args);
// 	
// 			}
// 		}
// 
// 	}
// 	return 0;
// 
// }
