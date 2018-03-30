//--------------------------------------------------------------------
// �ļ���:      RankListModule
// ��  ��:      �������а�Ĺ�����
// ˵  ��:      ���а������,���а��������Ϣͨ�š����Իص����ŵ�������(��ʱ�ȿ���������а�)
// ��������:    2014-10-15
//
// ������:      liumf
//    :        
//--------------------------------------------------------------------
#include <string>
#include "RankListModule.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/RankingDefine.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/Define/PlayerBaseDefine.h"
#include "utils/util_func.h"
#include "public/VarList.h"
#include "utils/XmlFile.h"
#include "utils/string_util.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/SystemFunctionModule/RewardModule.h"
#include "FsGame/Define/LogDefine.h"
#include "../CommonModule/PubModule.h"
#include "CommonModule/ReLoadConfigModule.h"
#include "SystemFunctionModule/PlayerBaseModule.h"
#include "CommonModule/LogModule.h"
//#include "Define/NationDefine.h"

#define RANK_LIST_DEFINE_FILE "ini/SystemFunction/RankList/rank_list_record.xml"
#define RANK_LIST_COLUMN_FILE "ini/SystemFunction/RankList/rank_list_column.xml"
#define RANK_LIST_COLUMN_TYPE_FILE "ini/SystemFunction/RankList/rank_list_column_type.xml"
#define RANK_LIST_REWARD "ini/SystemFunction/RankList/rank_list_reward.xml"


#define PLAYER_ONLINE 1
#define PLAYER_OFFLINE 0

#define RANK_NATION_DEFEND_AWARD "rank_nation_defend_award"		//�������а���
#define RANK_PVP_EXP_AWARD "rank_pvp_exp_award_mail"			// pvp�������ʼ�ģ��

RankListModule* RankListModule::m_pRankListModule = NULL;
RewardModule* RankListModule::m_pRewardModule = NULL;

std::wstring RankListModule::m_domainName = L"";  // ������������
RankListModule::RankListMap RankListModule::m_RankListMap;
RankListModule::ColumnMap RankListModule::m_ColumnMap;
RankListModule::ColumnTypeMap RankListModule::m_ColumnType;
RankListModule::RankListRewardConfigMap RankListModule::m_RankListRewardConfigMap;

inline int nx_reload_rank_list_config(void* state)
{
    IKernel* pKernel = LuaExtModule::GetKernel(state);

    if (NULL != RankListModule::m_pRankListModule)
    {
        RankListModule::m_pRankListModule->LoadResource(pKernel);
    }

    return 0;
}

// ��ʼ��
bool RankListModule::Init(IKernel * pKernel)
{
	m_pRankListModule = this;
	m_pRewardModule = dynamic_cast<RewardModule*>(pKernel->GetLogicModule("RewardModule"));

	Assert(m_pRewardModule != NULL);

    // ������ҵ������а���ص���Ϣ 
    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_RANKING, RankListModule::OnCustomMessage);
   
    // ���Թ���������Ϣ
    pKernel->AddEventCallback("scene", "OnPublicMessage", RankListModule::OnPublicMessage);

    // ���Գ�����������а���ص���Ϣ
    pKernel->AddIntCommandHook("scene", COMMAND_RANK_LIST, RankListModule::OnSceneCommandMessage);

    // ������ҵ�������ص���Ϣ
    pKernel->AddIntCommandHook("player", COMMAND_RANK_LIST, RankListModule::OnPlayerCommandMessage);

    // նɱ��Ϣ
    pKernel->AddIntCommandHook("player", COMMAND_KILLTARGET, RankListModule::OnKillObject);

    pKernel->AddEventCallback("player", "OnRecover", RankListModule::OnPlayerRecover);
    pKernel->AddEventCallback("player", "OnStore", RankListModule::OnStore);

	// ��ȡ����
	LoadResource(pKernel);

    // �������Ա仯�ص�
    DECL_CRITICAL(RankListModule::OnPlayerLevel);
    DECL_CRITICAL(RankListModule::OnPlayerVipLevel);
    DECL_CRITICAL(RankListModule::OnPlayerFightPower);
	//DECL_CRITICAL(RankListModule::OnPlayerPVPExp);
	
    // lua �ӿ�
    DECL_LUA_EXT(nx_reload_rank_list_config);

	RELOAD_CONFIG_REG("RankListConfig", RankListModule::ReloadConfig);
	return true;
}

// �ر�
bool RankListModule::Shut(IKernel * pKernel)
{
	return true;
}

// ��ȡDomain������
const std::wstring& RankListModule::GetDomainName(IKernel * pKernel)
{
    if (m_domainName.empty())
    {
        wchar_t wstr[256];
        const int server_id = pKernel->GetServerId();
		SWPRINTF_S(wstr, L"Domain_RankingList_%d", server_id);

        m_domainName = wstr;
    }

    return m_domainName;
}

IPubData* RankListModule::GetPubData(IKernel * pKernel)
{
	return PubModule::GetPubData( pKernel, GetDomainName(pKernel).c_str() );
}

// ��ѯ���а�
int RankListModule::Query(IKernel * pKernel, const PERSISTID &requester,
                          const IVarList & args, int request_from)
{
    /*
        @args�ĸ�ʽ

            [user_defined_id]
            [rank_list_name]
            [start_row][row_count_wanted]
            [sub_query_count]
            [column1_name][column1_value]
            [column2_name][column2_value]
            ...

        ���͸���������������Ϣ��ʽ

            [PUBSPACE][DOMAIN_NAME]
            [msg_id][sub_msg_id]
            [request_from][player]

            [user_defined_id]
            [record_name]
            [start_row][row_count_wanted]
            [sub_query_count]
            [column1_name][column1_value]
            [column2_name][column2_value]
            ...
    */
    int msg_index = 0;
    int user_defined_id = args.IntVal(msg_index);
    const char *rank_list_name = args.StringVal(++msg_index);
    int start_row = args.IntVal(++msg_index);
    int row_count_wanted = args.IntVal(++msg_index);
    int sub_column_count = args.IntVal(++msg_index);
    ++msg_index; //�ƶ���[column1_name]
    int column_count_to_check = (int(args.GetCount()) - msg_index)/2;

    // start_row �ӵ�һ�п�ʼ����
    // row_count_wanted = 0 ��ʾ����������
    // column_count_to_check = 0 ��ʾû�в�ѯ����
    if (start_row < 1 || row_count_wanted < 0 || column_count_to_check < 0)
    {
        return 0;
    }

    if (sub_column_count < 0 || sub_column_count > column_count_to_check)
    {
        return 0;
    }

    // ����Ƿ���ڴ����а�
    const RankListDef * rank_list_def = GetRankListDef(rank_list_name);
    if (rank_list_def == NULL)
    {
        return 0;
    }

    // ������Ƿ����
    LoopBeginCheck(a);
    for (int i=0; i<column_count_to_check; i++)
    {
        LoopDoCheck(a);
        
        if (!HasColumn(args.StringVal(msg_index), rank_list_def))
        {
            return 0;
        }

        msg_index += 2; // ���� [column_name][column_value]
    }

    // ȥ�򹫹���������ȡ���а�
    CVarList pub_msg;
    pub_msg << PUBSPACE_DOMAIN;
    pub_msg << RankListModule::GetDomainName(pKernel).c_str();
    pub_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    pub_msg << RANKING_SUB_MSG_QUERY;
    pub_msg << request_from;
    pub_msg << requester;
    pub_msg << args;

    pKernel->SendPublicMessage(pub_msg);
    return 0;
}

// �������а��һ����¼
int RankListModule::UpdateEntry(IKernel * pKernel, const PERSISTID &requester,
                                const IVarList & args, int request_from)
{
    /*
        �յ���args�ĸ�ʽ
            [user_defined_id]
            [rank_list_name][insert_if_not_exist]
            [search_column_name][search_column_value]
            [column1_name][column1_value]
            [column2_name][column2_value]
            ...

        ������Ϣ�ĸ�ʽ
            [PUBSPACE_DOMAIN][DOMAIN_NAME]
            [msg_id][sub_msg_id]
            [request_from][requester]

            [user_defined_id]
            [rank_list_name]
            [search_column_name][search_column_value]
            [column1_name][column1_value]
            [column2_name][column2_value]
            ...
    */

    // ����Ƿ���ڴ����а�
    const RankListDef * rank_list_def = GetRankListDef(args.StringVal(1));
    if (rank_list_def == NULL)
    {
        return -1;
    }

    int msg_index = 3; // �ƶ��� [search_column_name]

    // ����������Ƿ����
    if (!HasColumn(args.StringVal(msg_index), rank_list_def))
    {
        return -1;
    }
    ++msg_index; // ����[search_column_value]

    ++msg_index; // �ƶ���[column1_name]
    
    int column_count = (int(args.GetCount()) - msg_index)/2;
    if (column_count <= 0)
    {
        return -1;
    }

    LoopBeginCheck(a);
    for (int i=0; i<column_count; i++)
    {
        LoopDoCheck(a);
        
        // ����������Ƿ����
        if (!HasColumn(args.StringVal(msg_index), rank_list_def))
        {
            return -1;
        }

        msg_index += 2; // �ƶ�����һ��[column_name]�ֶ�
    }

    CVarList pub_msg;
    pub_msg << PUBSPACE_DOMAIN;
    pub_msg << RankListModule::GetDomainName(pKernel).c_str();
    pub_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    pub_msg << RANKING_SUB_MSG_UPDATE_ENTRY;
    pub_msg << request_from;
    pub_msg << requester;
    pub_msg << args;

    pKernel->SendPublicMessage(pub_msg);

    return 0;
}

// ɾ�����а��һ����¼
int RankListModule::DeleteEntry(IKernel * pKernel, const PERSISTID &requester,
                                const IVarList & args, int request_from)
{
    /*
        �յ���args�ĸ�ʽ
            [user_defined_id]
            [rank_list_name]
            [primary_key_name][primary_key_value]

        ������Ϣ�ĸ�ʽ
            [PUBSPACE_DOMAIN][DOMAIN_NAME]
            [msg_id][sub_msg_id]
            [request_from][requester]

            [user_defined_id]
            [rank_list_name]
            [primary_key_name][primary_key_value]
    */

    // ����Ƿ���ڴ����а�
    int msg_index = 0;
    const RankListDef * rank_list_def = GetRankListDef(args.StringVal(++msg_index));
    if (rank_list_def == NULL)
    {
        return -1;
    }

    // ��������Ƿ����
    if (!HasColumn(args.StringVal(++msg_index), rank_list_def))
    {
        return -1;
    }

    CVarList pub_msg;
    pub_msg << PUBSPACE_DOMAIN;
    pub_msg << RankListModule::GetDomainName(pKernel).c_str();
    pub_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    pub_msg << RANKING_SUB_MSG_DELETE_ENTRY;
    pub_msg << request_from;
    pub_msg << requester;
    pub_msg << args;

    pKernel->SendPublicMessage(pub_msg);

    return 0;
}

// ������а��ȫ����¼
int RankListModule::ClearRankList(IKernel * pKernel, const PERSISTID &requester,
                                  const IVarList & args, int request_from)
{
    /*
        �յ���args�ĸ�ʽ
            [user_defined_id]
            [rank_list_name]


        ������Ϣ�ĸ�ʽ
            [PUBSPACE_DOMAIN][DOMAIN_NAME]
            [msg_id][sub_msg_id]
            [request_from][requester]

            [user_defined_id]
            [rank_list_name]
    */

    // ����Ƿ���ڴ����а�
    const RankListDef * rank_list_def = GetRankListDef(args.StringVal(1));
    if (rank_list_def == NULL)
    {
        return -1;
    }

    CVarList pub_msg;
    pub_msg << PUBSPACE_DOMAIN;
    pub_msg << RankListModule::GetDomainName(pKernel).c_str();
    pub_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    pub_msg << RANKING_SUB_MSG_CLEAR;
    pub_msg << request_from;
    pub_msg << requester;
    pub_msg << args;

    pKernel->SendPublicMessage(pub_msg);

    return 0;
}

// ��ѯ���а����ݵ�����(���ݵ�ˢ������)
// args�ĸ�ʽ: [���а�1]...[���а�n]
int RankListModule::QueryRankListDate(IKernel * pKernel, const PERSISTID &requester,
                             const IVarList & args)
{
    CVarList pub_msg;
    pub_msg << PUBSPACE_DOMAIN;
    pub_msg << RankListModule::GetDomainName(pKernel).c_str();
    pub_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    pub_msg << RANKING_SUB_MSG_DATE;
    pub_msg << requester;
    pub_msg << args;

    pKernel->SendPublicMessage(pub_msg);

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// ϵͳ�ص�����
//////////////////////////////////////////////////////////////////////////

//������ݼ������
int RankListModule::OnPlayerRecover(IKernel *pKernel, const PERSISTID &player,
                    const PERSISTID &sender, const IVarList &args)
{
    bool is_robot = PlayerBaseModule::IsRobot(pKernel, player);
    if (!is_robot)
    {
        // ���Իص�
        pKernel->AddCritical(player, "Level", "RankListModule::OnPlayerLevel");
        pKernel->AddCritical(player, "VipLevel", "RankListModule::OnPlayerVipLevel");
        pKernel->AddCritical(player, "BattleAbility", "RankListModule::OnPlayerFightPower");
		//pKernel->AddCritical(player, "PVPExp", "RankListModule::OnPlayerPVPExp");

        // �������а�����ҵ�����״̬
        SetPlayerOnlineState(pKernel, player, PLAYER_ONLINE);
        
        // ��ѯָ�����а��ͳ������, �Ա�����Ƿ����������          
    }

    // �Ѵ���ҵ�����д�����а�
    // ����˵���˲����ı�Ҫ�ԣ����������������¼�������Ҽ������а�Ļ�, ��ô�ںϷ���,
    // һ�����������Զ���ᱻ���뵽�ȼ����а�.
    InsertToMultiRankList(pKernel, player);
    
    //������Ҿɱ��Ƿ���Ҫɾ�����������
    RemovePlayerOfOldDifLvlRankList(pKernel,player);
    
    return 0;
}

// ������ݱ���
int RankListModule::OnStore(IKernel * pKernel, const PERSISTID & player,
                   const PERSISTID & sender, const IVarList &args)
{
    if (PlayerBaseModule::IsRobot(pKernel, player))
    {
        return 0;
    }
    
    STORE_TYPES_ENUM save_type = (STORE_TYPES_ENUM)args.IntVal(0);

    if (save_type == STORE_EXIT)
    {
        // ������ߺ��������а�����ҵ�����״̬
        SetPlayerOnlineState(pKernel, player, PLAYER_OFFLINE);
    }
    
    return 0;
}

//������Ϣ
int RankListModule::OnPublicMessage(IKernel * pKernel, const PERSISTID & self,
					   const PERSISTID & sender, const IVarList & args)
{
    /*
        ��Ϣ��ʽ
            [sub_msg_id][...]
    */
    int main_msg_id = args.IntVal(0);
    if (main_msg_id != SP_DOMAIN_MSG_GENERAL_RANK_LIST)
    {
        return 0;
    }

    int sub_msg_id = args.IntVal(1);

    switch (sub_msg_id)
    {
    case RANKING_SUB_MSG_QUERY_TOP_N:
    case RANKING_SUB_MSG_QUERY:
        OnPublicQueryResult(pKernel, self, sender, args);
        break;
    case RANKING_SUB_MSG_UPDATE_ENTRY:
        OnPublicUpdateEntryResult(pKernel, self, sender, args);
        break;
    case RANKING_SUB_MSG_DELETE_ENTRY:
        OnPublicDeleteEntyResult(pKernel, self, sender, args);
        break;
    case RANKING_SUB_MSG_CLEAR:
        OnPublicClearResult(pKernel, self, sender, args);
        break;
    case RANKING_SUB_MSG_CLEAR_PLANNED:
        OnPublicClearPlanned(pKernel, self, sender, args);
        break;
    case RANKING_SUB_MSG_DATE:
        OnPublicRankListDate(pKernel, self, sender, args);
        break;
	case RANKING_SUB_MSG_CLEAR_AWARD:
		OnPublicClearAward(pKernel, self, sender, args);
		break;
    default:
        break;
    }

	return 0;
}

/// ����������command��Ϣ
int RankListModule::OnSceneCommandMessage(IKernel* pKernel, const PERSISTID& self,
                            const PERSISTID& sender, const IVarList& args)
{
    int sub_msg_id = args.IntVal(1);
    int core_msg_index = 2;

    switch (sub_msg_id)
    {
    case RANKING_SUB_MSG_CLEAR_PLANNED:
        {
            // ���а����ռƻ������, ֪ͨ���������ն�Ӧ������ݻ���

            pKernel->CommandByWorld(args);
        }
        break;
	case RANKING_SUB_MSG_UPDATE_ENTRY:
		{
			//TitleModule::RefreshRankListTitle(pKernel);
		}
		break;
    }

    return 0;
}

// ������ҵ�command��Ϣ
int RankListModule::OnPlayerCommandMessage(IKernel* pKernel, const PERSISTID& self,
                                     const PERSISTID& sender, const IVarList& args)
{
    // args �ĸ�ʽ:
    // [msg_id][sub_msg_id]...
    int sub_msg_id = args.IntVal(1);

    switch (sub_msg_id)
    {
    case RANKING_SUB_MSG_CLEAR_PLANNED:
        {
            // ���а��Ѱ��ƻ���յ���Ϣ
            //����Ϣ��ʽ��[msg_id][sub_msg_id][rank_list_name][last_clear_date][next_clear_date]
            int core_msg_index = 2;
            const char *rank_list_name = args.StringVal(core_msg_index);
            const char *last_clear_date = args.StringVal(++core_msg_index);

        }
        break;
    default:
        break;
    }

    return 0;
}

// �ͻ�����Ϣ
int RankListModule::OnCustomMessage(IKernel * pKernel, const PERSISTID & player,
				const PERSISTID & sender, const IVarList & args)
{
    /*
        ��Ϣ��ʽ
            [msg_id][sub_msg_id][...]

        ���Կͻ��˵���Ϣֻ�в�ѯ
     */
    int sub_msg_id = args.IntVal(1);

    switch (sub_msg_id)
    {
    case RANKING_SUB_MSG_QUERY:
        OnQuery(pKernel, player, sender, args);
        break;
    default:
        break;
    }

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// ���е����а��������
//////////////////////////////////////////////////////////////////////////

// ���Թ���������Ϣ
// �ַ����Թ���������Ϣ
int RankListModule::DispatchPublicMessage(IKernel * pKernel, const PERSISTID & self,
                                          const PERSISTID & sender, const IVarList & args)
{
    /*
        �յ�����Ϣ��ʽ
            [msg_id][sub_msg_id]
            [request_from][requester]
            [core_msg...]
    */

    int msg_index = 1;
    int sub_msg_id = args.IntVal(msg_index);
    int request_from = args.IntVal(++msg_index);
    PERSISTID requester = args.ObjectVal(++msg_index);
    int user_defined_id = args.IntVal(++msg_index);
    const char *rank_list_name = args.StringVal(++msg_index);

    // ����ָ�����򣬷���������
    int copy_from = 4;
    int copy_size = (int)args.GetCount() - 4;
    if (copy_size <= 0)
    {
        return 0;
    }

    if (!pKernel->Exists(requester))
    {
        return 0;
    }

    if (request_from == REQUEST_FROM_CLIENT)
    {
        if (pKernel->Type(requester) == TYPE_PLAYER)
        {
            CVarList response_msg;
            response_msg << SERVER_CUSTOMMSG_RANKING;
            response_msg << sub_msg_id;
            response_msg.Append(args, copy_from, copy_size);

            pKernel->Custom(requester, response_msg);
        }
    }
    else if (request_from == REQUEST_FROM_SERVER)
    {
        CVarList response_msg;
        response_msg << COMMAND_RANK_LIST;
        response_msg << sub_msg_id;
        response_msg.Append(args, copy_from, copy_size);

        pKernel->Command(self, requester, response_msg);
    }

    return 0;
}

// ���Թ����������Ĳ�ѯ���
int RankListModule::OnPublicQueryResult(IKernel * pKernel, const PERSISTID & self,
                         const PERSISTID & sender, const IVarList & args)
{
    return DispatchPublicMessage(pKernel, self, sender, args);
}

// ���Թ�������������Ŀ���½��
int RankListModule::OnPublicUpdateEntryResult(IKernel * pKernel, const PERSISTID & self,
                                     const PERSISTID & sender, const IVarList & args)
{
    return DispatchPublicMessage(pKernel, self, sender, args);
}

// ���Թ�������������Ŀɾ�����
int RankListModule::OnPublicDeleteEntyResult(IKernel * pKernel, const PERSISTID & self,
                                             const PERSISTID & sender, const IVarList & args)
{
    return DispatchPublicMessage(pKernel, self, sender, args);
}

// ���Թ�������������ս��
int RankListModule::OnPublicClearResult(IKernel * pKernel, const PERSISTID & self,
                                        const PERSISTID & sender, const IVarList & args)
{
    return DispatchPublicMessage(pKernel, self, sender, args);
}

// ���Թ����������������Ϣ, ���а����ݼƻ����
int RankListModule::OnPublicClearPlanned(IKernel * pKernel, const PERSISTID & self,
                                const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ��
    // [msg_id] [sub_msg_id][ranklist_name][last_clear_date][next_clear_date]

    int core_msg_index = 2;

    CVarList clear_msg;
    clear_msg << COMMAND_RANK_LIST;
    clear_msg << RANKING_SUB_MSG_CLEAR_PLANNED;
    clear_msg << args.StringVal(core_msg_index);
    clear_msg << args.StringVal(++core_msg_index);
    clear_msg << args.StringVal(++core_msg_index);

    // ���а����ռƻ������, ֪ͨ���������ն�Ӧ������ݻ���
    pKernel->CommandByWorld(clear_msg);

    return 0;
}

// ���Թ����������������Ϣ, ���а����ݼƻ����-���ͽ���
int RankListModule::OnPublicClearAward(IKernel * pKernel, const PERSISTID & self,
									   const PERSISTID & sender, const IVarList & args)
{
	// args �ĸ�ʽ��
	// [msg_id] [sub_msg_id][ranklist_name][player_name][rank_num]

    RewardRankList(pKernel, args);
	return 0;
}

// ���Թ�������������Ϣ, ���а��ˢ������
int RankListModule::OnPublicRankListDate(IKernel * pKernel, const PERSISTID & self,
                                const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ��
    // [msg_id][sub_msg_id]
    // [requester]
    // [ranklist_name 1][last_clear_date][next_clear_date]
    // ...
    // [ranklist_name n][last_clear_date][next_clear_date]

    int msg_size = (int)args.GetCount();
    PERSISTID requester = args.ObjectVal(2);
    int core_msg_index = 3; // �ӵ�һ�����а�ʼ
    int rank_list_count = (msg_size - 3)/3; // ���а�ĸ���

    if (!pKernel->Exists(requester))
    {
        return 0;
    }

    return 0;
}

// ���Կͻ��˵���Ϣ

// ����ָ��������ѯ���а�
int RankListModule::OnQuery(IKernel * pKernel, const PERSISTID & player,
                           const PERSISTID & sender, const IVarList & args)
{
    /*
        �յ�����Ϣ��ʽ

            [msg_id][sub_msg_id]

            [user_defined_id]
            [rank_list_name]
            [start_row][row_count_wanted]
            [sub_query_count]
            [column1_name][column1_value]
            [column2_name][column2_value]
            ...
    */
    CVarList request_msg;
    request_msg.Append(args, 2, int(args.GetCount())-2);

    return Query(pKernel, player, request_msg, REQUEST_FROM_CLIENT);
}

// ��ҵȼ��ı�
int RankListModule::OnPlayerLevel(IKernel* pKernel, const PERSISTID& self,
                                const char* property, const IVar& old)
{
	if (!PlayerBaseModule::IsRobot(pKernel, self))
    {
        // ֻ��¼��ͨ��ɫ
        InsertPlayerToRankList(pKernel, self, PLAYER_LEVEL_RANK_LIST);
    }

//	int nNation = pKernel->QueryInt(self, FIELD_PROP_NATION);
// 	if (!IsValidNation(nNation)) // ûѡ��Ӫ����Ҳ��ӵ��ֶεȼ����а���
// 	{
// 		return 0;
// 	}

	//������Ҿɱ��Ƿ���Ҫɾ�����������
	RemovePlayerOfOldDifLvlRankList(pKernel,self,old.IntVal());
	//������ҵ��ȼ��ּ���
	InsertPlayerToRankList(pKernel, self, GetDifLvlRankListName(pKernel->QueryInt(self,"Level")).c_str());

    return 0;
}


// ��ҵȼ��ı�
int RankListModule::OnPlayerVipLevel(IKernel* pKernel, const PERSISTID& self,
                            const char* property, const IVar& old)
{
	if (!PlayerBaseModule::IsRobot(pKernel, self))
    {
        // ֻ��¼��ͨ��ɫ
        
        //������ҵ��ȼ��ּ���
        InsertPlayerToRankList(pKernel, self, GetDifLvlRankListName(pKernel->QueryInt(self,"Level")).c_str());
    }

    return 0;
}

// ���ս�����ı�
int RankListModule::OnPlayerFightPower(IKernel* pKernel, const PERSISTID& self,
                              const char* property, const IVar& old)
{ 
    if (!pKernel->Exists(self))
    {
        return 0;
    }

    IGameObj *player_obj = pKernel->GetGameObj(self);
    if (player_obj == NULL)
    {
        return 0;
    }
    
	if (PlayerBaseModule::IsRobot(pKernel, self))
    {
        return 0;
    }    
    
    // ��ǰս����
    int current_ba = player_obj->QueryInt("BattleAbility");
    
    // ��ʷ���ս����
    int highest_ba = player_obj->QueryInt("HighestBattleAbility");
    
    if (current_ba > highest_ba)
    {
        player_obj->SetInt("HighestBattleAbility", current_ba);
        InsertPlayerToRankList(pKernel, self, "player_fight_power_rank_list");

		// ��¼���ս��
//		LogModule::m_pLogModule->OnPowerChanged(pKernel, self, current_ba);
    }
    
    return 0;
}

// ���������а�
// int RankListModule::OnPlayerPVPExp(IKernel* pKernel, const PERSISTID& self,
// 									  const char* property, const IVar& old)
// {
// 	// ֻ��¼��ͨ��ɫ
// 	InsertPlayerToRankList(pKernel, self, RANK_LIST_PVP_EXP);
// 
// 	return 0;
// }

int RankListModule::OnKillObject(IKernel* pKernel, const PERSISTID& self,
                        const PERSISTID& sender, const IVarList& args)
{

	if (PlayerBaseModule::IsRobot(pKernel, self))
    {
        return 0;
    }

    int args_type = args.IntVal(0);

    if (args_type != COMMAND_KILLTARGET)
    {
        return 0;
    }

    PERSISTID object_killed =args.ObjectVal(1);
    if (!pKernel->Exists(object_killed))
    {
        return 0;
    }

    // ��ɱ�����ж�, ֻ������һ�ɱ��ҵ����
    if (pKernel->Type(self) != TYPE_PLAYER || pKernel->Type(object_killed) != TYPE_PLAYER)
    {
        return 0;
    }
 
    PERSISTID scene = pKernel->GetScene();
    if (scene.IsNull())
    {
        return 0;
    }

	//PKģʽ�²�����ɱ����
	IGameObj* selfobj = pKernel->GetGameObj(self);
	IGameObj* killerobj = pKernel->GetGameObj(object_killed);
	if (NULL == selfobj || NULL == killerobj)
	{
		return 0;
	}

    return 0;
}


// �������а�����ҵ�����״̬
int RankListModule::SetPlayerOnlineState(IKernel* pKernel, const PERSISTID& player, int on_line)
{

	if (PlayerBaseModule::IsRobot(pKernel, player))
    {
        return 0;
    }
    
    const char *player_uid = pKernel->SeekRoleUid(pKernel->QueryWideStr(player, "Name"));

    RankListMap::const_iterator it = m_RankListMap.begin();
    RankListMap::const_iterator it_end = m_RankListMap.end();
    
    LoopBeginCheck(a);
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(a);
        
        const RankListDef& rank_list_def = it->second;

        // ������ҵ�����״̬
        if (!HasColumn("player_online", &rank_list_def))
        {
            continue;
        }

        CVarList update_msg;
        update_msg << RESERVED_NUMBER;
        update_msg << rank_list_def.name.c_str();
        update_msg << DO_NOT_INSERT;
        update_msg << "player_uid" << player_uid;
        update_msg << "player_online" << on_line;

        UpdateEntry(pKernel, player, update_msg);
    }
    
    return 0;
}

// ���԰�������ݼ��뵽��Ҫ�����а���
int RankListModule::InsertToMultiRankList(IKernel* pKernel, const PERSISTID& player)
{
    int player_level = pKernel->QueryInt(player, "Level");
    
    // �ֶεȼ�����������������ң�����Ԥ������
    std::string extral_level_rank_list = GetDifLvlRankListName(pKernel->QueryInt(player,"Level"));
    InsertPlayerToRankList(pKernel, player, extral_level_rank_list.c_str());
    
	// �ų�Ԥ�����Ľ�ɫ
	if (PlayerBaseModule::IsRobot(pKernel, player))
	{
	    return 0;
	}
	
    // ���а�ֻ��¼��ͨ���
	if (player_level > 0)
	{
		InsertPlayerToRankList(pKernel, player, PLAYER_LEVEL_RANK_LIST);
	}

	InsertPlayerToRankList(pKernel, player, "player_fight_power_rank_list");
    
    return 0;
}

// ������ҵȼ�ȡ�����а�ȼ����ֱ���
std::string RankListModule::GetDifLvlRankListName(int player_lvl)
{
	int left = 0;
	int right = 0;
	if (player_lvl < 30)
	{
		left = (player_lvl / 10) * 10 == 0 ? 1 : (player_lvl / 10) * 10;
		right = ((player_lvl + 10) / 10) * 10;
	}
	else
	{
		if (player_lvl %10 < 5)
		{
			left = (player_lvl / 10) * 10;
			right = (player_lvl / 10) * 10 + 5;
		}
		else
		{
			left = (player_lvl / 10) * 10 + 5;
			right = ((player_lvl + 5) / 10) * 10;
		}
	}

	//ƴ�ӱ���
	char rank_list_name_lvl[256] = {0};
	SPRINTF_S(rank_list_name_lvl, "%s_%d_%d", PLAYER_LEVEL_RANK_LIST, left, right);
	return rank_list_name_lvl;

}


// ��ѯ @column_name �����а�@rank_list_name�е��к�.(�кŴ�0��ʼ)
// ���� -1 ��ʾû�д��л���û�д����а�
int RankListModule::GetColumnIndexByName(const char *column_name, const char *rank_list_name)
{
    // ���а���
    const RankListDef* rank_list_def = GetRankListDef(rank_list_name);
    if (rank_list_def == NULL)
    {
        return -1;
    }
    
    std::string str_col_name = column_name;
    
    LoopBeginCheck(a);
    for (int i=0; i<rank_list_def->columns_count; ++i)
    {
        LoopDoCheck(a);
        
        if (rank_list_def->columns[i] == str_col_name)
        {
            return i;
        }
    }
    
    return -1;
}

// ��������ص���ɾ��ԭ���а�ȼ����ֱ���������
int RankListModule::RemovePlayerOfOldDifLvlRankList(IKernel* pKernel, const PERSISTID& player,int oldLevel)
{
	int player_lvl = pKernel->QueryInt(player,"Level");
	std::string rank_list_name = GetDifLvlRankListName(player_lvl);
	
	oldLevel = oldLevel > 0 ? oldLevel : (player_lvl - 1);
	// �ɵȼ�
	if (oldLevel <= 0)
	{
		return 0;
	}
	std::string old_rank_list_name = GetDifLvlRankListName(oldLevel);
	
	if (rank_list_name.empty() || old_rank_list_name.empty())
	{
		return 0;
	}
	if (strcmp(rank_list_name.c_str(),old_rank_list_name.c_str()) == 0)
	{
		return 0;
	}
	// TODO
	// ɾ������������ݽӿ�
	const wchar_t *player_name = pKernel->QueryWideStr(player,"Name");
	const char *uid = pKernel->SeekRoleUid(player_name);
	if (StringUtil::CharIsNull(uid))
	{
		return 0;
	}
	CVarList args;
	args<< 0
		<< old_rank_list_name.c_str()
		<< "player_uid"
		<< uid;
	m_pRankListModule->DeleteEntry(pKernel,PERSISTID(),args);

	return 0;
}


// Ϊ���а�׼������
bool RankListModule::PrepareDataForRankList(IKernel* pKernel, const PERSISTID& player, 
                                            const char *rank_list_name, IVarList &the_data)
{
    const RankListDef * rank_list_def = GetRankListDef(rank_list_name);
    if (rank_list_def == NULL)
    {
        return false;
    }
    
    LoopBeginCheck(a);
    for (int i=0; i<rank_list_def->columns_count; i++)
    {
        LoopDoCheck(a);

        const RankListColumnDef * field_def = 
            GetRankListColumnDef(rank_list_def->columns[i].c_str());

        if (field_def == NULL)
        {
            return false;
        }

        SetColumnValueWithoutName(pKernel, player, field_def, the_data);    
    }

    int result_column_count = (int) the_data.GetCount();

    if (result_column_count != rank_list_def->columns_count)
    {
        return false;
    }

    return true;
}

// ���԰���Ҳ������а�
int RankListModule::InsertPlayerToRankList(IKernel* pKernel, const PERSISTID& player,
    const char *rank_list_name)

{    
    // ����Ƿ���ڴ����а�
    const RankListDef * rank_list_def = GetRankListDef(rank_list_name);
    if (rank_list_def == NULL)
    {
        return 0;
    }

    CVarList insert_msg;
    insert_msg << RESERVED_NUMBER;
    insert_msg << rank_list_name;
    insert_msg << INSERT_IF_NOT_EXIST;
    
    // �������е���, ȥ��������
    
    LoopBeginCheck(a);
    for (int i=0; i<rank_list_def->columns_count; i++)
    {
        LoopDoCheck(a);
        
        const RankListColumnDef * field_def = 
                GetRankListColumnDef(rank_list_def->columns[i].c_str());

        if (field_def == NULL)
        {
            continue;
        }
        
        SetColumnValue(pKernel, player, field_def, insert_msg);    
    }

    UpdateEntry(pKernel, player, insert_msg);
    
    return 0;
}

// ɾ�����а�����Ҽ�¼
int RankListModule::DeleteFromRankList(IKernel* pKernel, const PERSISTID& player, 
                              const char *rank_list_name)
{
	// ��ȫ���
	if (!pKernel->Exists(player))
	{
		return 0;
	}

    CVarList delete_args;
    delete_args << 0;
    delete_args << rank_list_name;
    delete_args << "player_uid";
    delete_args << pKernel->QueryString(player, "Uid");
    
    return DeleteEntry(pKernel, PERSISTID(), delete_args);
}

// ɾ�����а�����Ҽ�¼
int RankListModule::DeleteFromRankList(IKernel* pKernel, const char* playerUid, 
									   const char *rank_list_name)
{
	// ��ȫ���
	if (StringUtil::CharIsNull(playerUid))
	{
		return 0;
	}

	CVarList delete_args;
	delete_args << 0;
	delete_args << rank_list_name;
	delete_args << "player_uid";
	delete_args << playerUid;

	return DeleteEntry(pKernel, PERSISTID(), delete_args);
}

// �������а�����������
// @args �ĸ�ʽ��
//     [�������Զ���ID]
//     [���а������][A�����е�����][A�����е�ֵ][B�����е�����][B�����е�ֵ]
//      
//  ���磺["level_rank_list"]["player_uid"]["1234"]["player_uid"]["456"]
//      ��ʾ�� uid=1234 ���� �� uid=456���л���
//
int RankListModule::SwapEntry(IKernel * pKernel, const PERSISTID &requester,
                             const IVarList & args, int request_from)
{
                                 /*
        �յ���args�ĸ�ʽ
            [user_defined_id]
            [rank_list_name]
            [search_column_name_of_A][search_column_value]
            [search_column_name_of_B][search_column_value]

        ������Ϣ�ĸ�ʽ
            [PUBSPACE_DOMAIN][DOMAIN_NAME]
            [msg_id][sub_msg_id]
            [request_from][requester]

            [user_defined_id]
            [rank_list_name]
            [search_column_name_of_A][search_column_value]
            [search_column_name_of_B][search_column_value]
    */

    // ����Ƿ���ڴ����а�
    const RankListDef * rank_list_def = GetRankListDef(args.StringVal(1));
    if (rank_list_def == NULL)
    {
        return -1;
    }

    int msg_index = 2; // �ƶ��� [search_column_name_of_A]

    // ����������Ƿ����
    if (!HasColumn(args.StringVal(msg_index), rank_list_def))
    {
        return -1;
    }
    ++msg_index; // ����[search_column_value]

    ++msg_index; // �ƶ���[search_column_name_of_B]
    // ����������Ƿ����
    if (!HasColumn(args.StringVal(msg_index), rank_list_def))
    {
        return -1;
    }

    CVarList pub_msg;
    pub_msg << PUBSPACE_DOMAIN;
    pub_msg << RankListModule::GetDomainName(pKernel).c_str();
    pub_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    pub_msg << RANKING_SUB_MSG_SWAP;
    pub_msg << request_from;
    pub_msg << requester;
    pub_msg << args;

    pKernel->SendPublicMessage(pub_msg);

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// ˽�еĸ�������
//////////////////////////////////////////////////////////////////////////

// ������Դ����
bool RankListModule::LoadResource(IKernel * pKernel)
{
    LoadRankListDef(pKernel, RANK_LIST_DEFINE_FILE);
    LoadRankListColumnDef(pKernel, RANK_LIST_COLUMN_FILE);
    LoadTypeNameDef(pKernel, RANK_LIST_COLUMN_TYPE_FILE);
    LoadRankListAwardDef(pKernel, RANK_LIST_REWARD);

    return true;
}

// ���а�Ķ���
bool RankListModule::LoadRankListDef(IKernel * pKernel, const char *file)
{
    m_RankListMap.clear();

    std::string res_path = pKernel->GetResourcePath();

    std::string rank_list_config_file = res_path;
    rank_list_config_file.append(file);

    // ��ȡ���а���
    /*
    <Property   ID="player_level_rank_list" 
    MaxRows="1000" 
    ColumnsName="player_uid,player_name,player_career,player_nation,player_level,player_experience" 
    DisplayColumns="player_name,player_career,player_nation,player_level,player_experience" 
    PrimaryKey="player_uid" 
    SortColumns="player_level,player_experience" 
    SortOrder="desc" /> 
    */

    CXmlFile xml(rank_list_config_file.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = rank_list_config_file;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a);
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a);
        
        RankListDef rank_list_def;
        const char *sec = sec_list.StringVal(i);
        rank_list_def.name = sec;

        const char *rank_list_columns = xml.ReadString(sec, "ColumnsName", "");

        SaveStrToArray(rank_list_columns, rank_list_def.columns, 
            RANK_LIST_MAX_COLUMN_COUNT, &(rank_list_def.columns_count));

        m_RankListMap.insert(RankListMap::value_type(rank_list_def.name,
            rank_list_def));

    }

    return true;
}

bool RankListModule::LoadRankListColumnDef(IKernel * pKernel, const char *file)
{
    m_ColumnMap.clear();

    std::string res_path = pKernel->GetResourcePath();

    std::string rank_list_config_file = res_path;
    rank_list_config_file.append(file);

    // ��ȡ���а���
    /*
    <Property ID="player_uid" PlayerProp="Uid" ValueType="str"/>
    */

    CXmlFile xml(rank_list_config_file.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = rank_list_config_file;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a);
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a);
        
        RankListColumnDef column_def;
        const char *sec = sec_list.StringVal(i);
        
        column_def.name = sec;
        column_def.prop = xml.ReadString(sec, "PlayerProp", "");
        column_def.column_type = xml.ReadString(sec, "ValueType", "int");

        m_ColumnMap.insert(ColumnMap::value_type(column_def.name, column_def));

    }

    return true;
}

// ����������
bool RankListModule::LoadTypeNameDef(IKernel * pKernel, const char *file)
{
    /*
        <Property ID="int" Type="2"/>
    */
        
    m_ColumnType.clear();

    std::string res_path = pKernel->GetResourcePath();

    std::string config_file = res_path;
    config_file.append(file);

    CXmlFile xml(config_file.c_str());
    if (!xml.LoadFromFile())
    {
        ::extend_warning(LOG_ERROR, "%s does not exists.", config_file.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a);
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a);
        
        std::string type_name = sec_list.StringVal(i);
        int type_value = xml.ReadInteger(type_name.c_str(), "Type", -1);

        if (type_name.length() == 0)
        {                
            ::extend_warning(LOG_ERROR, "%s: empty column type found.", config_file.c_str());

            return false;
        }

        if (type_value == -1)
        {
            ::extend_warning(LOG_ERROR, "%s:invalid column type value found. column is %s",
                config_file.c_str(), type_name.c_str());

            return false;
        }

        m_ColumnType.insert(ColumnTypeMap::value_type(type_name, type_value));
    }

    return true;
}

// ���а�������
bool RankListModule::LoadRankListAwardDef(IKernel * pKernel, const char *file)
{
	m_RankListRewardConfigMap.clear();

	std::string res_path = pKernel->GetResourcePath();
	std::string config_file = res_path;
	config_file.append(file);

	try
	{
		CXmlFile xml(config_file.c_str());
		if (!xml.LoadFromFile())
		{
			extend_warning(LOG_ERROR, "[RankListModule::LoadRankListAwardDef] failed");
			return false;
		}

		char* pfilebuff = GetFileBuff(config_file.c_str());
		if (pfilebuff == NULL)
		{
			return false;
		}

		xml_document<> doc;
		doc.parse<rapidxml::parse_default>(pfilebuff);
		xml_node<>* pNodeRoot = doc.first_node("Object");
		if (pNodeRoot == NULL)
		{
			return false;
		}
		xml_node<>* pNode = pNodeRoot->first_node("Property");
		if (pNode == NULL)
		{
			return false;
		}

		LoopBeginCheck(a);
		while (pNode != NULL)
		{
			LoopDoCheck(a);
			RankListRewardConfig_s rankAwardcConfig;
			rankAwardcConfig.m_RankListName = QueryXmlAttr(pNode, "Rank_List_Name");
			
			xml_node<>* p_SubNode = pNode->first_node("Item");
			if (p_SubNode == NULL)
			{
				return false;
			}
			LoopBeginCheck(b);
			while (p_SubNode != NULL )
			{
				LoopDoCheck(b);
				RewardConfig_s config;
				config.m_ID = convert_int(QueryXmlAttr(p_SubNode, "ID"));
				config.m_Rank = convert_string(QueryXmlAttr(p_SubNode, "Rank"), "");
				config.m_Capital = convert_string(QueryXmlAttr(p_SubNode, "Capital"), "");
				config.m_Items = convert_string(QueryXmlAttr(p_SubNode, "Items"), "");

				rankAwardcConfig.m_RewardConfigVec.push_back(config);
				p_SubNode = p_SubNode->next_sibling("Item");
			}

			m_RankListRewardConfigMap.insert(std::make_pair(rankAwardcConfig.m_RankListName, rankAwardcConfig));
			pNode = pNode->next_sibling("Property");

		}

		delete[] pfilebuff;
	}
	catch(parse_error& e)
	{
		::extend_warning(pKernel, "[Error]RankListModule::LoadRankListAwardDef: load xml error:");
		::extend_warning(pKernel, config_file.c_str());
		::extend_warning(pKernel, e.what());
		return false;
	}

	return true;
}

bool RankListModule::HasColumn(const char *column_name, const RankListDef * rank_list_def)
{
    std::string search_key = column_name;
 
    LoopBeginCheck(a);
    for (int i=0; i<rank_list_def->columns_count; i++)
    {
        LoopDoCheck(a);
        
        if (search_key == rank_list_def->columns[i])
        {
            return true;
        }
    }

    return false;
}

const RankListModule::RankListDef* RankListModule::GetRankListDef(const char *rank_list_name)
{
    std::string search_key = rank_list_name;
    RankListMap::iterator it = m_RankListMap.find(search_key);
    if (it != m_RankListMap.end())
    {
        return &(it->second);
    }
    else
    {
        return NULL;
    }
}

const RankListModule::RankListColumnDef* RankListModule::GetRankListColumnDef(const char *field_name)
{
    std::string search_key = field_name;
    ColumnMap::iterator it = m_ColumnMap.find(search_key);
    if (it != m_ColumnMap.end())
    {
        return &(it->second);
    }
    else
    {
        return NULL;
    }
}

// ����ָ���ֶε�ֵ, ���Ѵ��ֶμ���@fields
bool RankListModule::SetColumnValue(IKernel *pKernel, const PERSISTID &player,
                                   const RankListColumnDef *field_def, IVarList & fields)
{
    if (field_def->prop.length() == 0)
    {
        // ���ֶβ����������
        return true;
    }
    
    if (!pKernel->Find(player, field_def->prop.c_str()))
    {
        // ���û�д�����
        return false;
    }
    
    int prop_type = pKernel->GetType(player, field_def->prop.c_str());
    
    switch (prop_type)
    {
    case VTYPE_INT:
        {
            fields << field_def->name.c_str();
            fields << pKernel->QueryInt(player, field_def->prop.c_str());
        }
        break;
    case VTYPE_INT64:
        {
            fields << field_def->name.c_str();
            fields << pKernel->QueryInt64(player, field_def->prop.c_str());
        }
        break;
    case VTYPE_FLOAT:
        {
            fields << field_def->name.c_str();
            fields << pKernel->QueryFloat(player, field_def->prop.c_str());
        }
        break;
    case VTYPE_DOUBLE:
        {
            fields << field_def->name.c_str();
            fields << pKernel->QueryDouble(player, field_def->prop.c_str());
        }
        break;
    case VTYPE_STRING:
        {
            fields << field_def->name.c_str();
            fields << pKernel->QueryString(player, field_def->prop.c_str());
        }
        break;
    case VTYPE_WIDESTR:
        {
            fields << field_def->name.c_str();
            fields << pKernel->QueryWideStr(player, field_def->prop.c_str());
        }
        break;
    }
    
    return true;
}

// ����ָ���ֶε�ֵ, �������ֶ���
bool RankListModule::SetColumnValueWithoutName(IKernel *pKernel, const PERSISTID &player,
                                      const RankListColumnDef *field_def, IVarList & fields)
{
    if (field_def->name.length() == 0)
    {
        // ��Ч��
        return true;
    }

    bool is_prop = true;

    if (!pKernel->Find(player, field_def->prop.c_str()))
    {
        // �����������
        is_prop = false;
    }

    int prop_type = GetColumnTypeByName(field_def->column_type);//pKernel->GetType(player, field_def->prop.c_str());

    if (prop_type == -1)
    {
        return false;
    }

    switch (prop_type)
    {
    case VTYPE_INT:
        {
            if (is_prop)
            {
                fields << pKernel->QueryInt(player, field_def->prop.c_str());
            }
            else
            {
                fields << 0;
            }
        }
        break;
    case VTYPE_INT64:
        {
            if (is_prop)
            {
                fields << pKernel->QueryInt64(player, field_def->prop.c_str());
            }
            else
            {
                fields << (__int64) 0;
            }
        }
        break;
    case VTYPE_FLOAT:
        {
            if (is_prop)
            {
                fields << pKernel->QueryFloat(player, field_def->prop.c_str());
            }
            else
            {
                fields << 0.0F;
            }
        }
        break;
    case VTYPE_DOUBLE:
        {
            if (is_prop)
            {
                fields << pKernel->QueryDouble(player, field_def->prop.c_str());
            }
            else
            {
                fields << 0.0;
            }
        }
        break;
    case VTYPE_STRING:
        {
            if (is_prop)
            {
                fields << pKernel->QueryString(player, field_def->prop.c_str());
            }
            else
            {
                fields << "";
            }
        }
        break;
    case VTYPE_WIDESTR:
        {
            if (is_prop)
            {
                fields << pKernel->QueryWideStr(player, field_def->prop.c_str());
            }
            else
            {
                fields << L"";
            }
        }
        break;
    }
    
    return true;
}

// �Ѷ��ŷָ����ַ������浽����
// ����: "a,b,c" ���浽 {"a", "b", "c"}
bool RankListModule::SaveStrToArray(const char *str_src,
                                    std::string *str_array,
                                    int array_length,
                                    int *actual_size)
{
    CVarList parse_result;
    util_split_string(parse_result, str_src, ",");

    int result_count = (int)parse_result.GetCount();
    if (result_count > array_length)
    {
        return false;
    }

    LoopBeginCheck(a);
    for (int i = 0; i<result_count; ++i)
    {
        LoopDoCheck(a);
        
        str_array[i] = parse_result.StringVal(i);
    }

    *actual_size = result_count;

    return true;
}

// ������������ȡ����ID
int RankListModule::GetColumnTypeByName(const std::string &column_name)
{
    ColumnTypeMap::const_iterator it_end = m_ColumnType.end();
    
    ColumnTypeMap::const_iterator it = m_ColumnType.find(column_name);
    
    if (it == it_end)
    {
        return -1;
    }
    else
    {
        return it->second;
    }
}

// �������а���
int RankListModule::RewardRankList(IKernel *pKernel, const IVarList & args)
{
	// args �ĸ�ʽ��
	// [msg_id] [sub_msg_id][ranklist_name][player_name][rank_num]

	int core_msg_index = 2;

	const char* rankListName = args.StringVal(core_msg_index);       // ���а�����
	const wchar_t* playerNames = args.WideStrVal(++core_msg_index);  // ����ǳƼ���(������ŷָ�)
	const char* rankNums = args.StringVal(++core_msg_index);         // �������(���ǳ�������Ӧ)
 

	//1������������֤
	if( StringUtil::CharIsNull( rankListName ) || StringUtil::CharIsNull( playerNames ) || StringUtil::CharIsNull( rankNums ) )
	{
		return -1;
	}

	//2�����а���
	RankListRewardConfigMap_it it = m_pRankListModule->m_RankListRewardConfigMap.find(rankListName);
	if ( it == m_pRankListModule->m_RankListRewardConfigMap.end() )
	{
		return -1;
	}

	//3���������ó���
	int awardLen = static_cast<int>(it->second.m_RewardConfigVec.size());

	CVarList nameVars;
	CVarList rankVars;
	StringUtil::SplitStringW(nameVars, playerNames, L",");
	StringUtil::SplitString(rankVars, rankNums, ",");
	if( nameVars.GetCount() != rankVars.GetCount() )
	{
		return -1;
	}

	int nameLen = static_cast<int>(nameVars.GetCount());
	LoopBeginCheck(a);
	for( int i = 0; i < nameLen; ++i )
	{
		LoopDoCheck(a);
		if( StringUtil::CharIsNull(nameVars.WideStrVal(i)) )
		{
			continue;
		}

		bool isReward = false;
		RewardModule::AwardEx award;
		int realRank = rankVars.IntVal(i);  //ÿ����ҵ�����

		//4�����Ҷ�Ӧ�Ľ���
		LoopBeginCheck(b);
		for( int j = 0 ; j < awardLen ; ++j )
		{
			LoopDoCheck(b);
			std::string strRankNum = it->second.m_RewardConfigVec[j].m_Rank;

			CVarList rankConfigVars;
			StringUtil::SplitString(rankConfigVars, strRankNum, ",");
			if( rankConfigVars.GetCount() > 1 )
			{
				if( realRank >= rankConfigVars.IntVal(0) && realRank <= rankConfigVars.IntVal(1) )
				{
					isReward = true;
				}

			}
			else if( rankConfigVars.GetCount() == 1 )
			{
				if( realRank == rankConfigVars.IntVal(0) )
				{
					isReward = true;
				}
			}

// 			if( isReward )
// 			{
// 				RewardModule::ParseAddCapitals(it->second.m_RewardConfigVec[j].m_Capital.c_str(), award.capitals);
// 				RewardModule::ParseAddItems(it->second.m_RewardConfigVec[j].m_Items.c_str(), award.items);
// 
// 				if (strcmp(rankListName, PVP_EXP_RANK_LIST) == 0)
// 				{
// 					award.srcFunctionId = FUNCTION_EVENT_ID_PVP_RANK_AWARD;
// 					award.name = RANK_PVP_EXP_AWARD;
// 				}
// 				
// 				break;
// 			}
		}

		//5���ʼ����ͽ���
		if( isReward )
		{
			//�ʼ�ģ�����
// 			CVarList params;
// 			params << realRank;
// 			m_pRewardModule->RewardViaMail(pKernel, nameVars.WideStrVal(i), &award, params);
		}
	}

	return 0;
}

void RankListModule::ReloadConfig(IKernel *pKernel)
{
	if (NULL != RankListModule::m_pRankListModule)
	{
		RankListModule::m_pRankListModule->LoadResource(pKernel);
	}
}