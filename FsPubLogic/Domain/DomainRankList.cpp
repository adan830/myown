//---------------------------------------------------------
//�ļ���:       DomainRankList.cpp
//��  ��:       ���а�����
//˵  ��:       �����洢������������б�, ���а�ͨ������������
//          
//��������:      2014��11��04��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#include "DomainRankList.h"

#include <string>
#include <time.h>
#include <algorithm>

#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/RankingDefine.h"
#include "utils/extend_func.h"
#include "utils/util_func.h"
#include "utils/XmlFile.h"
#include "FsGame/Define/SnsDefine.h"
//#include "FsGame/Define/OffLineDefine.h"
#include "utils/string_util.h"
//#include "FsGame/Define/NationDefine.h"
//#include "FsGame/Define/FrontierSceneDefine.h"

#define SUCCESS_RET_CODE 0
#define FAILED_RET_CODE  -1

#define RANK_LIST_DEFINE_FILE       "ini/SystemFunction/RankList/rank_list_record.xml"
#define RANK_LIST_COLUMN_FILE       "ini/SystemFunction/RankList/rank_list_column.xml"
#define RANK_LIST_COLUMN_TYPE_FILE  "ini/SystemFunction/RankList/rank_list_column_type.xml"
#define LEVEL_LOCK_CONFIG          "ini/SystemFunction/RankList/level_lock_config.xml"

// ���а����Ա�
#define INFO_TABLE              "RankListTable"
#define INFO_TABLE_COLUMN_COUNT 3
#define INFO_TABLE_MAX_ROW      100

#define SECONDS_OF_ONE_DAY      86400 // һ��(24hr)������

#define VALUE_TYPE_IMMEDIATE         0 // ������
#define VALUE_TYPE_EXP               1 // ���ʽ

#define UPDATE_STRATEGY_DEFAULT      0 // Ĭ�ϲ���
#define UPDATE_STRATEGY_GREAT_THAN   1 // ����ʱ���� 
#define UPDATE_STRATEGY_LESS_THAN    2 // С��ʱ����

#define RANK_PLAYER_COUNT  10		         // ÿ�δ���������

DomainRankList* DomainRankList::m_pDomainRankList = NULL;

DomainRankList::RecordDefineMap DomainRankList::m_RecordDefine;
DomainRankList::ColumnDefineMap DomainRankList::m_ColumnDefine;  
DomainRankList::ColumnTypeMap DomainRankList::m_ColumnType;

DomainRankList::DomainRankList() : IDomainData(L"")
{
}

DomainRankList::~DomainRankList()
{
    m_RecordDefine.clear();
}

// ������а�Ķ�ʱ��
int DomainRankList::ClearRankListTimer(IPubKernel *pPubKernel, const char *space_name,
                              const wchar_t *data_name, int time)
{
    IPubSpace *pPubSpace = pPubKernel->GetPubSpace(space_name);
    if (pPubSpace == NULL)
    {
        return 0;
    }
    IPubData *pDomainData = pPubSpace->GetPubData(data_name);
    if (pDomainData == NULL)
    {
        return 0;
    }

    if (m_pDomainRankList == NULL)
    {
        return 0;
    }

    std::string current_date = GetDateOfToday();

    RecordDefineMap::iterator it = m_RecordDefine.begin();
    RecordDefineMap::iterator it_end = m_RecordDefine.end();
    
    LoopBeginCheck(a);
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(a);
        
        bool do_clear = false;
        RecordDefine &rec_def = it->second;

        // ȡ���ϴ�������� �� �´��������
        std::string last_clear_date;
        std::string next_clear_date;
        m_pDomainRankList->GetClearDate(pDomainData, rec_def.rec_name,
            last_clear_date, next_clear_date);
        
        CVarList clear_msg;
        clear_msg << rec_def.rec_name.c_str();

        switch (rec_def.clear_strategy)
        {
        case RANKING_CLEAR_ON_DAY:
            {
                if (current_date != last_clear_date)
                {
                    std::string next_clear_date_new = GetDateOfNextDay();
                    clear_msg << current_date;
                    clear_msg << next_clear_date_new.c_str();
                    do_clear = true;
                }
            }
            break; 
        case RANKING_CLEAR_ON_WEEK:
            {
                if (current_date != last_clear_date)
                {
                    // ����´���������
                    if (CompareDate(next_clear_date, current_date) <=0)
                    {
                        std::string next_clear_date_new = GetDateOfNextMonday();
                        clear_msg << current_date;
                        clear_msg << next_clear_date_new.c_str();
                        do_clear = true;
                    }
                }
            }
            break;
        case RANKING_CLEAR_ON_MONTH:
            {
                if (current_date != last_clear_date)
                {
                    // ����´���������
                    if (CompareDate(next_clear_date, current_date) <=0)
                    {
                        std::string next_clear_date_new = GetDateOfNextMonth();
                        clear_msg << current_date;
                        clear_msg << next_clear_date_new.c_str();
                        do_clear = true;
                    }
                }
            }
            break;
        default:
            break;
        }
        if (do_clear)
        {
            m_pDomainRankList->ClearRankList(pPubKernel, pPubSpace, clear_msg);
        }
    }

    return 0;
}

IPubData* DomainRankList::GetRankListData(IPubKernel* pPubKernel)
{
	IPubData* pData = NULL;

	wchar_t wstr[256];
	int server_id = pPubKernel->GetServerId();
	SWPRINTF_S(wstr, L"Domain_RankingList_%d", server_id);
	IPubSpace* pSpace = pPubKernel->GetPubSpace(PUBSPACE_DOMAIN);
	Assert(NULL != pSpace);
	if (NULL != pSpace)
	{
		pData = pSpace->GetPubData(wstr);
	}

	return pData;
}

const std::wstring & DomainRankList::GetDomainName(IPubKernel * pPubKernel)
{
    if (m_domainName.empty())
    {
        wchar_t wstr[256];
        int server_id = pPubKernel->GetServerId();
		 SWPRINTF_S(wstr, L"Domain_RankingList_%d", server_id);
        m_domainName = wstr;
    }

    return m_domainName;
}

int DomainRankList::OnCreate(IPubKernel* pPubKernel, IPubSpace * pPubSpace)
{
    m_pDomainRankList = this;
    
    return OnLoad(pPubKernel, pPubSpace);
}

int DomainRankList::OnLoad(IPubKernel* pPubKernel, IPubSpace * pPubSpace)
{
    m_pDomainRankList = this;

    // �����Ķ�������
    LoadResource(pPubKernel, pPubSpace);

    // ��ʼ�����а�
    InitAllRankList(pPubKernel, pPubSpace);

	const wchar_t* name = GetDomainName(pPubKernel).c_str();
	if (StringUtil::CharIsNull(name))
	{
		return 0;
	}

	// ��Ӽ������
	if (!pPubKernel->FindTimer("RankListClear", pPubSpace->GetSpaceName(), name))
	{
		// ����������а�Ķ�ʱ��
		int timer_interval = 1000 * 60 * 1; // 1min �ļ��
		int timer_count = 0; // ���޴���

		pPubKernel->AddTimer("RankListClear", DomainRankList::ClearRankListTimer,
			pPubSpace->GetSpaceName(), name,
			timer_interval, timer_count);
	}

    return 0;
}

/// �������Գ�������������Ϣ
int DomainRankList::OnMessage(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
                             int source_id, int scene_id, const IVarList& msg)
{
    /*
       ��Ϣ��ʽ:
           [PUBSPACE][DOMAIN_NAME]
           [msg_id][sub_msg_id]
     */
    IPubData * pDomainData = pPubSpace->GetPubData(GetDomainName(pPubKernel).c_str());
    if (pDomainData == NULL)
    {
        return 1;
    }

    int msgid = msg.IntVal(3);
    switch (msgid)
    {
    case RANKING_SUB_MSG_QUERY:
        {
            OnQuery(pPubKernel, pPubSpace, source_id, scene_id, msg);
        }
        break;
    case RANKING_SUB_MSG_QUERY_TOP_N:
        {
            // ����Ϣ������
        }
        break;
    case RANKING_SUB_MSG_DELETE_ENTRY:
        {
            OnDeleteEntry(pPubKernel, pPubSpace, source_id, scene_id, msg);
        }
        break;
    case RANKING_SUB_MSG_UPDATE_ENTRY:
        {
            OnUpdateEntry(pPubKernel, pPubSpace, source_id, scene_id, msg);
        }
        break;
    case RANKING_SUB_MSG_CLEAR:
        {
            OnClear(pPubKernel, pPubSpace, source_id, scene_id, msg); 
        }
        break;
    case RANKING_SUB_MSG_DATE:
        {
             OnQueryRecordDate(pPubKernel, pPubSpace, source_id, scene_id, msg); 
        }
        break;
    case RANKING_SUB_MSG_SWAP:
        {
            OnSwapEntry(pPubKernel, pPubSpace, source_id, scene_id, msg); 
        }
        break;
    }

    return 1;
}

// ��ѯ����, ���ݵ�һ������ѯ�������Ȼ���ѯ�ڶ������ڴ˽���е�����
int DomainRankList::Query(IPubKernel* pPubKernel, IPubSpace* pPubSpace, IVarList &out_result, 
              const IVarList& msg, int msg_start_index)
{
    /*
        msg ��ʽ��

        [rank_list_name]
        [start_row][row_count_wanted]
        [sub_column_count]            // �ڶ�����������
        [column1_name][column1_value] // �ڶ�����
        [column2_name][column2_value] 
        ...
        [columnX_name][columnx_value] // ��һ����
        ...

        out_result ��ʽ

        [rank_list_name]
        [start_row][row_count_wanted]
        [result_row_count][column_count_per_row]
        [seq_no_all + seq_no_query + row_data...]
        ...
    */
    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return -1;
    }

    int msg_index = msg_start_index;
    const char *record_name = msg.StringVal(msg_index);
    int start_row = msg.IntVal(++msg_index);
    int row_count_wanted = msg.IntVal(++msg_index);

    if (start_row < 1 || row_count_wanted < 0)
    {
        return -1;
    }

    RecordDefine *rec_def = GetRecordDefine(record_name);
    if (rec_def == NULL)
    {
        return -1;
    }

    if (rec_def->output_columns_count == 0)
    {
        return -1;
    }

    IRecord * record = pDomainData->GetRecord(record_name);
    if (record == NULL)
    {
        return -1;
    }


    ++msg_index; // �ƶ��� [sub_column_count]
    int sub_column_count = msg.IntVal(msg_index); // �ڶ�����������
    if (sub_column_count < 0)
    {
        return -1;
    }

    // ��ȡ��ѯ��������
    ++msg_index; // �ƶ��� [column1_name]

    // ��ȡ�ڶ�����
    std::vector<ColumnVar> sub_colunms_to_check;
    if (!GetColumnsFromArgs(&sub_colunms_to_check, msg, msg_index, sub_column_count, rec_def))
    {
        return -1;
    }
    msg_index += 2*sub_column_count; // �ƶ�����һ���� [columnX_name]

    // ��һ����
    std::vector<ColumnVar> colunms_to_check;
    int column_count_to_check = (int(msg.GetCount()) - msg_index)/2;
    if (!GetColumnsFromArgs(&colunms_to_check, msg, msg_index, column_count_to_check, rec_def))
    {
        return -1;
    }

    CVarList matched_rows;
    int result_row_count = 0;
    int result_column_count = 0;
    int search_ret = ReadRowOfOutputCols(matched_rows, &result_row_count, &result_column_count, record,
        &colunms_to_check, &sub_colunms_to_check, start_row, row_count_wanted, rec_def);
    if (search_ret == 0)
    {
        out_result << record_name;
        out_result << start_row;
        out_result << row_count_wanted;
        out_result << result_row_count;
        out_result << result_column_count;
        out_result << matched_rows;
    }

    return 0;
}

// ����(����)��Ŀ
int DomainRankList::UpdateEntry(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
                                const IVarList& msg, int msg_start_index)
{
    /*
        msg ��ʽ��

            [record_name][insert_if_not_exist]
            [search_column][search_column_value]
            [column1_name][column1_value]
            [column2_name][column2_value]
            ...

        out_result ��ʽ

            N/A
    */

    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return -1;
    }

    int msg_index = msg_start_index;
    const char *record_name = msg.StringVal(msg_index);
    int do_insert = msg.IntVal(++msg_index);

    RecordDefine *rec_def = GetRecordDefine(record_name);
    if (rec_def == NULL)
    {
        // û�������Ľṹ����
        return -1;
    }

    IRecord *record = pDomainData->GetRecord(record_name);
    if (record == NULL)
    {
        // ������
        return -1;
    }

    // ��ȡ����
    ++msg_index; // �ƶ��� [search_column]
    ColumnVar search_column;
    if (!GetColumnFromArgs(&search_column, msg, msg_index, rec_def))
    {
        return -1;
    }
    msg_index += 2; // �ƶ��� [column1_name]

    // ��ȡҪ���µ���

    int column_count_to_check = (int(msg.GetCount()) - msg_index)/2;
    std::vector<ColumnVar> colunms_to_update;
    colunms_to_update.push_back(search_column);

    if (!GetColumnsFromArgs(&colunms_to_update, msg, msg_index, column_count_to_check, rec_def))
    {
        return -1;
    }

    // ���ҷ�����������
    std::vector<int> found_rows;
    int count_updated = 0;
    
    FindRowsByColumn(record, &search_column, rec_def, &found_rows);

    if (found_rows.size() > 0)
    {
        // �����ҵ�����
        
        std::vector<int>::iterator it_row = found_rows.begin();
        std::vector<int>::iterator it_row_end = found_rows.end();
        
        LoopBeginCheck(a);
        for (; it_row != it_row_end; ++it_row)
        {
            LoopDoCheck(a);
            
            int temp_row_id = *it_row;
            UpdateRow(record, temp_row_id, &colunms_to_update, rec_def, rec_def->associated_update_strategy);
            ++count_updated;
        }
    }
    else
    {
        // û����ƥ�����
        
        if (do_insert)
        {
            InsertRow(record, &colunms_to_update, rec_def);
            ++count_updated;
        }
    }

    // ���¹�����
    LoopBeginCheck(b);
    for (int i=0; i<rec_def->associated_rank_list_count; ++i)
    {
        LoopDoCheck(b);
        
        IRecord *associated_record = pDomainData->GetRecord((rec_def->associated_rank_list[i]).c_str());
        if (associated_record == NULL)
        {
            continue;
        }

        std::vector<int> aso_found_rows;
        FindRowsByColumn(associated_record, &search_column, rec_def, &aso_found_rows);

        if (aso_found_rows.size() > 0)
        {
            // �����ҵ�����

            std::vector<int>::iterator it_aso_row = aso_found_rows.begin();
            std::vector<int>::iterator it_aso_row_end = aso_found_rows.end();
            
            LoopBeginCheck(c);
            for (; it_aso_row != it_aso_row_end; ++it_aso_row)
            {
                LoopDoCheck(c);
                
                int temp_row_id = *it_aso_row;
                UpdateRow(associated_record, temp_row_id, &colunms_to_update, rec_def, rec_def->associated_update_strategy);
                ++count_updated;
            }
        }
        else
        {
            // û����ƥ�����

            if (do_insert)
            {
                InsertRow(associated_record, &colunms_to_update, rec_def);
                ++count_updated;
            }
        }
    }

    return count_updated;
}

// ɾ����Ŀ
int DomainRankList::DeleteEntry(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
                                const IVarList& msg, int msg_start_index)
{
    /*
        msg ��ʽ��

            [record_name]
            [primary_key_column][primary_key_value]

        out_result ��ʽ

            N/A
    */
    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return -1;
    }

    int msg_index = msg_start_index;
    const char *record_name = msg.StringVal(msg_index);

    RecordDefine *rec_def = GetRecordDefine(record_name);
    if (rec_def == NULL)
    {
        // û�������Ľṹ����
        return -1;
    }

    IRecord *record = pDomainData->GetRecord(record_name);
    if (record == NULL)
    {
        // ������
        return -1;
    }

    ++msg_index; // �ƶ���[primary_key_column]
    ColumnVar pk_column;
    if (!GetColumnFromArgs(&pk_column, msg, msg_index, rec_def))
    {
        return -1;
    }
    msg_index += 2;

    int count_deleted = DeleteRow(record, &pk_column, rec_def);

    return count_deleted;
}

// ������������
// @msg �ĸ�ʽ
//
//      [�������Զ���ID]
//      [���а������][A�����е�����][A��ֵ][B�����е�����][B��ֵ]
//      
// �˲������ύ�� A �� B ��λ��
////  ���磺["level_rank_list"]["player_uid"]["1234"]["player_uid"]["456"]
//      ��ʾ�� uid=1234 ���� �� uid=456���л���
//
bool DomainRankList::SwapEntry(IPubKernel* pPubKernel, IPubSpace* pPubSpace, const IVarList& msg,
                 int msg_start_index)
{
    /*
        msg ��ʽ��

            [record_name]
            [primary_key_column of A][primary_key_value of A]
            [primary_key_column of B][primary_key_value of B]

        out_result ��ʽ

            N/A
    */
    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return false;
    }

    int msg_index = msg_start_index;
    const char *record_name = msg.StringVal(msg_index);

    RecordDefine *rec_def = GetRecordDefine(record_name);
    if (rec_def == NULL)
    {
        // û�������Ľṹ����
        return false;
    }

    IRecord *record = pDomainData->GetRecord(record_name);
    if (record == NULL)
    {
        // ������
        return false;
    }

    ++msg_index; // �ƶ���[primary_key_column of A]
    ColumnVar pk_column_of_A;
    if (!GetColumnFromArgs(&pk_column_of_A, msg, msg_index, rec_def))
    {
        return false;
    }
    ++msg_index; // ����[column_value]
    
    ++msg_index; // �ƶ���[primary_key_column of B]
    ColumnVar pk_column_of_B;
    if (!GetColumnFromArgs(&pk_column_of_B, msg, msg_index, rec_def))
    {
        return false;
    }
    msg_index += 2;

    // �ҵ�A������
    std::vector<int> found_A;
    FindRowsByColumn(record, &pk_column_of_A, rec_def, &found_A);
    if (found_A.size() == 0)
    {
        return false;
    }
    
    int row_id_of_A = found_A.at(0);
    CVarList entry_of_A;
    record->QueryRowValue(row_id_of_A, entry_of_A);
    
    // �ҵ�B������
    std::vector<int> found_B;
    FindRowsByColumn(record, &pk_column_of_B, rec_def, &found_B);
    if (found_B.size() == 0)
    {
        return false;
    }
    
    int row_id_of_B = found_B.at(0);
    CVarList entry_of_B;
    record->QueryRowValue(row_id_of_B, entry_of_B);
    
    // ����
    record->SetRowValue(row_id_of_A, entry_of_B);
    record->SetRowValue(row_id_of_B, entry_of_A);

    return true;
}

// ������а�
int DomainRankList::ClearRankList(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
                                  const IVarList& msg)
{
    /*
        msg ��ʽ��

            [rank_list_name]
            ��
            [rank_list_name][last_clear_date][next_clear_date]
    */
    int msg_size = (int)msg.GetCount();

    std::string record_name = msg.StringVal(0);
    std::string last_clear_date = "";
    std::string next_clear_date = "";
    if (msg_size == 3)
    {
        last_clear_date = msg.StringVal(1);
        next_clear_date = msg.StringVal(2);
    }

    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return -1;
    }

    RecordDefine *rec_def = GetRecordDefine(record_name.c_str());
    if (rec_def == NULL)
    {
        // û�������Ľṹ����
        return -1;
    }

    IRecord *record = pDomainData->GetRecord(rec_def->rec_name.c_str());
    if (record == NULL)
    {
        // ������
        return -1;
    }

    record->ClearRow();

	if(rec_def->clear_strategy != RANKING_CLEAR_ON_REQUEST)
	{
		// ���´����а�ĸ�������
		ResetClearDate(pDomainData, rec_def->rec_name, last_clear_date, next_clear_date);
	}

    CVarList clear_msg;
    clear_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    clear_msg << RANKING_SUB_MSG_CLEAR_PLANNED;
    clear_msg << rec_def->rec_name.c_str();
    clear_msg << last_clear_date.c_str();
    clear_msg << next_clear_date.c_str();

    // ֪ͨ����������, ���а��Ѱ��ƻ����
    // 0 ��ʾ������һ������
    pPubKernel->SendPublicMessage(0, 0, clear_msg);

    return 0;
}

// ��ѯ@column_name �����а�@rank_list_name �е�����
// �Ҳ���ʱ����-1
int DomainRankList::GetColumnIndex(const std::string &column_name,
                                   const std::string &rank_list_name)
{
    const RecordDefine * rec_def = GetRecordDefine(rank_list_name.c_str());
    if (rec_def == NULL)
    {
        return -1;
    }

    return GetColumnIndex(column_name, rec_def);
}

// ��ѯ@column_name �����а�@rank_list_name �е�����
// �Ҳ���ʱ����-1
int DomainRankList::GetColumnIndexInOutput(const std::string &column_name,
                                           const std::string &rank_list_name)
{
    const RecordDefine * rec_def = GetRecordDefine(rank_list_name.c_str());
    if (rec_def == NULL)
    {
        return -1;
    }

    int index = GetColumnIndex(column_name, rec_def);

    LoopBeginCheck(a);
    for (int i=0; i<rec_def->output_columns_count; i++)
    {
        LoopDoCheck(a);
        
        if (index == rec_def->output_columns[i])
        {
            return i;
        }
    }

    return -1;
}

// ���ݵȼ��͹����ҵ�����������һ�����
bool DomainRankList::FindPlayerUidByLevelNation(IPubKernel* pPubKernel, IPubSpace * pPubSpace, int nLevel
												, int nNation, IVarList& outData, const std::vector<std::string>& vRecUid)
{
	outData.Clear();

	// ��ʱ�ȼ�����
	int tempLevel = nLevel;

	// ��ŷ������������uid
	std::vector<const char*> vFitPlayerUid;

	// �����ռ�
	const std::wstring &domain_name = GetDomainName(pPubKernel);
	IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());
	if (pDomainData == NULL)
	{
		return false;
	}

	LoopBeginCheck(a)
	while( vFitPlayerUid.empty() && tempLevel > 0 )
	{
		LoopDoCheck(a)
		std::string strRec = GetDifLvlRankListName(tempLevel);

		// �ҵ����а�
		IRecord* pRec = pDomainData->GetRecord(strRec.c_str());
		if (NULL == pRec)
		{
			return false;
		}

		int nCount = pRec->GetCols();
		// �ҵ�������ͬ��һ���������
		int nRows = pRec->GetRows();

		int nNationColumn = GetColumnIndex("player_nation", strRec);
		int nOfflineTypeColum = GetColumnIndex("player_online_type", strRec);

		LoopBeginCheck(b);
		for (int i = 0;i < nRows;++i)
		{
			LoopDoCheck(b);

			int nCurNation = pRec->QueryInt(i, nNationColumn);
			//int nCurOnline = pRec->QueryInt(i, nOfflineTypeColum);
			if (nCurNation == nNation)
			{
				// ��1��Ϊ���uid
				const char* strUid = pRec->QueryString(i, 0);
				// �ж����Ƿ�Ϊ�������������
				std::vector<std::string>::const_iterator iter = find(vRecUid.begin(), vRecUid.end(), std::string(strUid));
				if (iter == vRecUid.end())
				{
					vFitPlayerUid.push_back(strUid);
				}
			}
		}

		// ���ݰ񵥹��򽵵͵ȼ�
		tempLevel = CompatiblePlayerLevel(tempLevel);
	}

	if (vFitPlayerUid.empty())
	{
		return false;
	}

	// ���һ�����������������
	int nRandomInex = util_random_int((int)vFitPlayerUid.size());
	const char* strFind = vFitPlayerUid[nRandomInex];
	outData << strFind;

	return true;
}

// ������������ѯ���а�
int DomainRankList::OnQuery(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
                            int source_id, int scene_id, const IVarList& msg)
{
    /*
        ������Ϣ��

            [PUBSPACE][DOMAIN_NAME]
            [msg_id][sub_msg_id]

            [request_from]
            [requester_obj]

            [user_defined_id]
            [rank_list_name]
            [start_row][row_count_wanted]
            [column1_name][column1_value]
            [column2_name][column2_value]
            ...

        ��Ӧ��Ϣ

            [msg_id][sub_msg_id]

            [request_from]
            [requester_obj]

            [user_defined_id]
            [rank_list_name]
            [start_row][row_count_wanted]
            [result_row_count][column_count_per_row]
            [seq_no_all + seq_no_query + row_data...]
            ...

    */

    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return 0;
    }

    int msg_index = 4; // ��[request_from] ��ʼ
    int request_from = msg.IntVal(msg_index);
    PERSISTID requester = msg.ObjectVal(++msg_index);
    int user_defined_id = msg.IntVal(++msg_index);
    ++msg_index; // �ƶ���[rank_list_name]

    CVarList query_result;
    int query_ret = Query(pPubKernel, pPubSpace, query_result, msg, msg_index);
    if (query_ret == -1)
    {
        return 0;
    }

    // ��Ӧ��Ϣ
    CVarList response_msg;
    response_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    response_msg << RANKING_SUB_MSG_QUERY;
    response_msg << request_from;
    response_msg << requester;      // ��������ݵĶ���
    response_msg << user_defined_id;
    response_msg << query_result;

    // �ظ���Ϣ
    pPubKernel->SendPublicMessage(source_id, scene_id, response_msg);

    return 0;
}

// ��Ŀ�и���, ˢ�����а�
int DomainRankList::OnUpdateEntry(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
                                 int source_id, int scene_id, const IVarList& msg)
{
    /*
        ������Ϣ��

            [PUBSPACE][DOMAIN_NAME]
            [msg_id][sub_msg_id]

            [request_from]
            [requester_obj]

            [user_defined_id]
            [record_name][insert_if_not_exist]
            [primary_column][primary_column_value]
            [column1_name][column1_value]
            [column2_name][column2_value]
            ...

        ��Ӧ��Ϣ

            [msg_id][sub_msg_id]

            [request_from]
            [requester_obj]

            [user_defined_id]
            [record_name]
            [primary_column][primary_column_value]
            [updated_columns_count]
            
    */

    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return 0;
    }

    int msg_index = 4; // ��[request_from] ��ʼ
    int request_from = msg.IntVal(msg_index);
    PERSISTID requester = msg.ObjectVal(++msg_index);
    int user_defined_id =msg.IntVal(++msg_index);
    const char *record_name = msg.StringVal(++msg_index);
    
    int updated_count = UpdateEntry(pPubKernel, pPubSpace, msg, msg_index);
    if (updated_count == -1)
    {
        return 0;
    }
   
    CVarList response_msg;
    response_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    response_msg << RANKING_SUB_MSG_UPDATE_ENTRY;
    response_msg << request_from;
    response_msg << requester;
    response_msg << user_defined_id;
    response_msg << record_name;
    response_msg.Append(msg, 9, 2); //�������������[column_name][column_value]
    response_msg << updated_count;     // ���µ�����

    pPubKernel->SendPublicMessage(source_id, scene_id, response_msg);

    return 0;
}

// ɾ�����а����Ŀ
int DomainRankList::OnDeleteEntry(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
                                 int source_id, int scene_id, const IVarList& msg)
{
    /*
        ������Ϣ��

            [PUBSPACE][DOMAIN_NAME]
            [msg_id][sub_msg_id]

            [request_from]
            [requester_obj]

            [user_defined_id]
            [record_name]
            [primary_key_column][primary_key_value]

        ��Ӧ��Ϣ

            [msg_id][sub_msg_id]

            [request_from]
            [requester_obj]

            [user_defined_id]
            [record_name]
            [primary_column][primary_column_value]
            [updated_columns_count]
    */

    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return 0;
    }

    int msg_index = 4; // ��[request_from] ��ʼ
    int request_from = msg.IntVal(msg_index);
    PERSISTID requester = msg.ObjectVal(++msg_index);
    int user_defined_id = msg.IntVal(++msg_index);
    const char *record_name = msg.StringVal(++msg_index);

    int deleted_count = DeleteEntry(pPubKernel, pPubSpace, msg, msg_index);
    if (deleted_count == -1)
    {
        return 0;
    }

    CVarList response_msg;
    response_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    response_msg << RANKING_SUB_MSG_DELETE_ENTRY;
    response_msg << request_from;
    response_msg << requester;
    response_msg << user_defined_id;
    response_msg << record_name;
    response_msg.Append(msg, 8, 2); //�������������[column_name][column_value]
    response_msg << deleted_count; // ɾ��������

    pPubKernel->SendPublicMessage(source_id, scene_id, response_msg);

    return 0;
}


// ����������а�
int DomainRankList::OnClear(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
            int source_id, int scene_id, const IVarList& msg)
{
    /*
        ������Ϣ��

            [PUBSPACE][DOMAIN_NAME]
            [msg_id][sub_msg_id]
            
            [request_from]
            [request_obj]

            [user_defined_id]
            [record_name]

        ��Ӧ��Ϣ

            [msg_id][sub_msg_id]
            
            [request_from]
            [request_obj]

            [user_defined_id]
            [record_name]
            [clear_ret] 0 ��ʾ�ɹ� 1 ��ʾʧ��

    */

    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return 0;
    }

    int msg_index = 4; // ��[request_from] ��ʼ
    int request_from = msg.IntVal(msg_index);
    PERSISTID requester = msg.ObjectVal(++msg_index);
    int user_defined_id = msg.IntVal(++msg_index);
    const char *record_name = msg.StringVal(++msg_index);

    CVarList clear_msg;
    clear_msg << record_name;
    int clear_ret = ClearRankList(pPubKernel, pPubSpace, clear_msg);
    if (clear_ret == -1)
    {
        return 0;
    }

    CVarList response_msg;
    response_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    response_msg << RANKING_SUB_MSG_CLEAR;
    response_msg << request_from;
    response_msg << requester;
    response_msg << user_defined_id;
    response_msg << record_name;
    response_msg << SUCCESS_RET_CODE;

    pPubKernel->SendPublicMessage(source_id, scene_id, response_msg);

    return 0;
}

// ��ѯ���а����������
int DomainRankList::OnQueryRecordDate(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
                int source_id, int scene_id, const IVarList& msg)
{
    /*
        ������Ϣ��

            [PUBSPACE][DOMAIN_NAME]
            [msg_id][sub_msg_id]

            [request_obj]

            [record_name 1]
            ...
            [record_name n]

        ��Ӧ��Ϣ

            [msg_id][sub_msg_id]

            [request_obj]

            [record_name 1][last_clear_date][next_clear_date]
            ...
            [record_name n][last_clear_date][next_clear_date]
    */

    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return 0;
    }

    // �ҵ�������
    IRecord *info_record = pDomainData->GetRecord(INFO_TABLE);
    if (info_record == NULL)
    {
        return 0;
    }

    int msg_size = (int)msg.GetCount();
    PERSISTID requester = msg.ObjectVal(4);
    int record_count = msg_size - 5;
    int record_index = 5;

    CVarList record_date;

    LoopBeginCheck(a);
    for (int i=0; i<record_count; ++i)
    {
        LoopDoCheck(a);
        
        const char *record_name = msg.StringVal(record_index + i);

        int row_id = info_record->FindString(0, record_name);
        if (row_id != -1)
        {
            CVarList temp_row;
            if (info_record->QueryRowValue(row_id, temp_row))
            {
                record_date << temp_row;
            }
        }
    }

    CVarList response_msg;
    response_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    response_msg << RANKING_SUB_MSG_DATE;
    response_msg << requester;
    response_msg << record_date;

    pPubKernel->SendPublicMessage(source_id, scene_id, response_msg);

    return 0;
}

// �������а��е�����
int DomainRankList::OnSwapEntry(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
                int source_id, int scene_id, const IVarList& msg)
{
    /*
        ������Ϣ��

            [PUBSPACE][DOMAIN_NAME]
            [msg_id][sub_msg_id]

            [request_from]
            [requester_obj]

            [user_defined_id]
            [record_name]
            [primary_column_of_A][primary_column_value]
            [primary_column_of_B][primary_column_value]


            
    */

    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());

    if (pDomainData == NULL)
    {
        return 0;
    }

    int msg_index = 4; // ��[request_from] ��ʼ
    int request_from = msg.IntVal(msg_index);
    PERSISTID requester = msg.ObjectVal(++msg_index);
    int user_defined_id =msg.IntVal(++msg_index);
    const char *record_name = msg.StringVal(++msg_index);
    
    SwapEntry(pPubKernel, pPubSpace, msg, msg_index);
    //if (updated_count == -1)
    //{
    //    return 0;
    //}
   
    //CVarList response_msg;
    //response_msg << SP_DOMAIN_MSG_GENERAL_RANK_LIST;
    //response_msg << RANKING_SUB_MSG_SWAP;
    //response_msg << request_from;
    //response_msg << requester;
    //response_msg << user_defined_id;
    //response_msg << record_name;
    //response_msg.Append(msg, 9, 2); //�������������[column_name][column_value]
    //response_msg << updated_count;     // ���µ�����

    //pPubKernel->SendPublicMessage(source_id, scene_id, response_msg);

    return 0;
}

// ��ʼ�����а�
void DomainRankList::InitAllRankList(IPubKernel* pPubKernel, IPubSpace* pPubSpace)
{
    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());
    if (pDomainData == NULL)
    {
        return ;
    }

    // ����������
    CreateInfoTable(pDomainData);

    RecordDefineMap::iterator it = m_RecordDefine.begin();
    RecordDefineMap::iterator it_end = m_RecordDefine.end();
    
    LoopBeginCheck(a);
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(a);
        
        RecordDefine &rec_def = it->second;

        bool create_new = false;
        OldDataStrategy old_data_strategy = KEEP_OLD_DATA;

        if (!pDomainData->FindRecord(rec_def.rec_name.c_str()))
        {
            // ���а񲻴���
            create_new = true;
        }
        else
        {
            IRecord *rec = pDomainData->GetRecord(rec_def.rec_name.c_str());
            if (rec != NULL)
            {
                // �鿴�����Ƿ���ͬ
                if (!create_new)
                {
                    if (rec_def.columns_count != rec->GetCols() || rec_def.max_row > rec->GetRowMax())
                    { 
                        create_new = true;
                    }
                    
                    if (rec_def.columns_count < rec->GetCols())
                    {
                        // �°�����������ˣ����ܱ���������
                        old_data_strategy = DISCARD_OLD_DATA;
                    }
                }
            
                // �鿴ÿһ�еĶ����Ƿ���ͬ
                if (!create_new)
                {
                    // ������ͬ, �鿴�����Ͷ����Ƿ���ͬ
                    
                    LoopBeginCheck(b);
                    for (int i=0; i<rec_def.columns_count; i++)
                    {
                        LoopDoCheck(b);
                        
                        if (rec->GetColType(i) != rec_def.columns_type[i])
                        {
                            create_new = true;
                            old_data_strategy = DISCARD_OLD_DATA;
                            break;
                        }
                    }
                }

            }// record != NULL

        } // End of ����

        if (create_new)
        {
            bool created = RecreateRankList(pDomainData, rec_def, old_data_strategy);

            if (created)
            {
                // ����������(����)
                pDomainData->SetRecordKey(rec_def.rec_name.c_str(), rec_def.pk_column);
                pDomainData->SetRecordSave(rec_def.rec_name.c_str(), true);
            }
            else
            {
                std::string err_msg = rec_def.rec_name;
                err_msg.append(" Recreate failed.");
				::extend_warning_pub(LOG_ERROR, err_msg.c_str());
            }
        }
        else
        {
            // ���а�ı��Ѿ����ڣ����ñ�Ҫ���еĳ�ʼֵ
            if (rec_def.columns_to_reset_count > 0)
            {
                IRecord *record = pDomainData->GetRecord(rec_def.rec_name.c_str());
                if (record != NULL)
                {
                    ResetRankListColumn(record, &rec_def);
                }
            }
        }
    }// End of ���а�ѭ��

    return;
}

// �����в���һ������, ������ID, û�ҵ�����-1
int DomainRankList::FindRowByColumn(IRecord *the_record,
                                   const ColumnVar *the_column,
                                   const RecordDefine *rec_def)
{
    int row_index = -1;
    int column_type = rec_def->columns_type[the_column->column_index];
    
    switch (column_type)
    {
    case VTYPE_INT:
        row_index = the_record->FindInt(the_column->column_index, the_column->int_value);
        break;
    case VTYPE_INT64:
        row_index = the_record->FindInt64(the_column->column_index, the_column->int64_value);
        break;
    case VTYPE_FLOAT:
        row_index = the_record->FindFloat(the_column->column_index, the_column->float_value);
        break;
    case VTYPE_DOUBLE:
        row_index = the_record->FindDouble(the_column->column_index, the_column->double_value);
        break;
    case VTYPE_STRING:
        row_index = the_record->FindString(the_column->column_index, the_column->str_value);
        break;
    case VTYPE_WIDESTR:
        row_index = the_record->FindWideStr(the_column->column_index, the_column->wstr_value);
        break;
    default:
        break;
    }

    return row_index;
}


// ��@the_record�в��ҷ�������@the_column������������
void DomainRankList::FindRowsByColumn(IRecord *the_record, const ColumnVar *the_column,
                     const RecordDefine *rec_def, std::vector<int> *row_id_list)
{
    int row_index = -1;
    int column_type = rec_def->columns_type[the_column->column_index];

    // ����к�
    int max_row_id = the_record->GetRows() - 1;

    switch (column_type)
    {
    case VTYPE_INT:
        {
            row_index = the_record->FindInt(the_column->column_index, the_column->int_value);
            
            LoopBeginCheck(a)
            while (row_index != -1)
            {
                LoopDoCheck(a)
                
                row_id_list->push_back(row_index);

                if (row_index != max_row_id)
                {
                    row_index = the_record->FindInt(the_column->column_index, the_column->int_value, row_index+1);
                }
                else
                {
                    break;
                }
            }
        }
        break;
    case VTYPE_INT64:
        {
            row_index = the_record->FindInt64(the_column->column_index, the_column->int64_value);
            
            LoopBeginCheck(a)
            while (row_index != -1)
            {
                LoopDoCheck(a)
                
                row_id_list->push_back(row_index);

                if (row_index != max_row_id)
                {
                    row_index = the_record->FindInt64(the_column->column_index, the_column->int64_value, row_index+1);
                }
                else
                {
                    break;
                }
            }
        }
        break;
    case VTYPE_FLOAT:
        {
            row_index = the_record->FindFloat(the_column->column_index, the_column->float_value);
            
            LoopBeginCheck(a)
            while (row_index != -1)
            {
                LoopDoCheck(a)
                
                row_id_list->push_back(row_index);

                if (row_index != max_row_id)
                {
                    row_index = the_record->FindFloat(the_column->column_index, the_column->float_value, row_index+1);
                }
                else
                {
                    break;
                }
            }
        }
        break;
    case VTYPE_DOUBLE:
        {
            row_index = the_record->FindDouble(the_column->column_index, the_column->double_value);
            
            LoopBeginCheck(a)
            while (row_index != -1)
            {
                LoopDoCheck(a)
                
                row_id_list->push_back(row_index);

                if (row_index != max_row_id)
                {
                    row_index = the_record->FindDouble(the_column->column_index, the_column->double_value, row_index+1);
                }
                else
                {
                    break;
                }
            }
        }
        break;
    case VTYPE_STRING:
        {
            row_index = the_record->FindString(the_column->column_index, the_column->str_value);
            
            LoopBeginCheck(a)
            while (row_index != -1)
            {
                LoopDoCheck(a)
                
                row_id_list->push_back(row_index);

                if (row_index != max_row_id)
                {
                    row_index = the_record->FindString(the_column->column_index, the_column->str_value, row_index+1);
                }
                else
                {
                    break;
                }
            }
        }
        break;
    case VTYPE_WIDESTR:
        {
            row_index = the_record->FindWideStr(the_column->column_index, the_column->wstr_value);
            
            LoopBeginCheck(a)
            while (row_index != -1)
            {
                LoopDoCheck(a)
                
                row_id_list->push_back(row_index);

                if (row_index != max_row_id)
                {
                    row_index = the_record->FindWideStr(the_column->column_index, the_column->wstr_value, row_index+1);
                }
                else
                {
                    break;
                }
            }
        }
        break;
    default:
        break;
    }
}

// ��@record��ȡ����@row_id�е����ݣ�����@dest��, 
// ��@rec_def.output_columns ������������Щ�� 
int DomainRankList::ReadRowOfOutputCols(IVarList &dest, IRecord *record, int row_id,
                           const RecordDefine *rec_def)
{
    LoopBeginCheck(a);
    for (int i=0; i<rec_def->output_columns_count; i++)
    {
        LoopDoCheck(a);
        
        int column_index = rec_def->output_columns[i];

        switch (rec_def->columns_type[column_index])
        {
        case VTYPE_INT:
            dest << record->QueryInt(row_id, column_index);
            break;
        case VTYPE_INT64:
            dest << record->QueryInt64(row_id, column_index);
            break;
        case VTYPE_FLOAT:
            dest << record->QueryFloat(row_id, column_index);
            break;
        case VTYPE_DOUBLE:
            dest << record->QueryDouble(row_id, column_index);
            break;
        case VTYPE_STRING:
            dest << record->QueryString(row_id, column_index);
            break;
        case VTYPE_WIDESTR:
            dest << record->QueryWideStr(row_id, column_index);
            break;
        default:
            return -1;
        }
    }

    return 0;
}

// ���� @columns��ѯ����� �ӽ���ĵ�@start_row��ʼ��ȡ@row_count_wanted������, ����@out_list
// @row_count ��¼���ص�����, @coulumn_count ��¼���ص�����
int DomainRankList::ReadRowOfOutputCols(IVarList &out_list, int *row_count, int *coulumn_count,
                        IRecord *record, const std::vector<ColumnVar> *columns,
                        int start_row, int row_count_wanted, const RecordDefine *rec_def)
{
    int read_ret = 0;
    int current_row_count = record->GetRows();
    if (row_count_wanted == 0)
    {
        row_count_wanted = current_row_count;
    }

    std::vector<int> matched_row;
    int matched_row_index = 0;
    int saved_count = 0;

    // ���ҷ���������row������
    
    LoopBeginCheck(a);
    for (int i=0; i<current_row_count; i++)
    {
        LoopDoCheck(a);
        
        if (!IsRowMatch(i, record, columns, rec_def))
        {
            continue;
        }
        
        // ƥ����������
        matched_row_index ++;

        if (matched_row_index < start_row)
        {
            // ƥ������С��Ҫ�����
            continue;
        }

        // �ﵽҪ������@start_row, ��ȡһ������
        out_list << i+1; // i ���㿪ʼ����, ��Ҫ+1ת��
        out_list << matched_row_index;
        read_ret = ReadRowOfOutputCols(out_list, record, i, rec_def);
        if (read_ret == -1)
        {
            return -1;
        }

        saved_count++;

        if (saved_count == row_count_wanted)
        {
            break;
        }
    }

    *row_count = saved_count;
    *coulumn_count = rec_def->output_columns_count + 2; // ��Ϊ������ seq_no_all �� seq_no_query ����

    return 0;
}

// ���� @columns��ѯ����� �ӽ���ĵ�@start_row��ʼ��ȡ@row_count_wanted������, Ȼ����
// ����м�����������@sub_column ����Ŀ, ����@out_list
// @row_count ��¼���ص�����, @coulumn_count ��¼���ص�����
// �������ݸ�ʽ��: [seq_no_all][seq_no_query][column1]...
int DomainRankList::ReadRowOfOutputCols(IVarList &out_list, int *row_count, int *coulumn_count,
                        IRecord *record, const std::vector<ColumnVar> *columns, 
                        const std::vector<ColumnVar> *sub_columns,
                        int start_row, int row_count_wanted, const RecordDefine *rec_def)
{
    int read_ret = 0;
    int current_row_count = record->GetRows();
    if (row_count_wanted == 0)
    {
        row_count_wanted = current_row_count;
    }

    std::vector<int> matched_row;
    int matched_row_index = 0;
    int saved_count = 0;

    // ���ҷ���������row������
    
    LoopBeginCheck(a);
    for (int i=0; i<current_row_count; i++)
    {
        LoopDoCheck(a);
        
        if (!IsRowMatch(i, record, columns, rec_def))
        {
            continue;
        }

        // ƥ����������
        matched_row_index ++;

        if (matched_row_index < start_row)
        {
            // ƥ������С��Ҫ�����
            continue;
        }

        // ����Ƿ���ϵڶ�����
        if (!IsRowMatch(i, record, sub_columns, rec_def))
        {
            continue;
        }

        // �ﵽҪ������@start_row, ��ȡһ������
        out_list << i+1; // i ���㿪ʼ����, ��Ҫ+1ת��
        out_list << matched_row_index;
        read_ret = ReadRowOfOutputCols(out_list, record, i, rec_def);
        if (read_ret == -1)
        {
            return -1;
        }

        saved_count++;

        if (saved_count == row_count_wanted)
        {
            break;
        }
    }

    *row_count = saved_count;
    *coulumn_count = rec_def->output_columns_count + 2; // ��Ϊ������ seq_no_all �� seq_no_query ����

    return 0;
}

// ����@column �鿴���Ƿ�ƥ��
bool DomainRankList::IsRowMatch(int row_id, IRecord *record,
                                const std::vector<ColumnVar> *columns,
                                const RecordDefine *rec_def)
{
    std::vector<ColumnVar>::const_iterator it = columns->begin();
    std::vector<ColumnVar>::const_iterator it_end = columns->end();

    LoopBeginCheck(a);
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(a);
        
        const ColumnVar &column_var = *it;
        int column_index = column_var.column_index;
        int column_type = rec_def->columns_type[column_index];

        switch (column_type)
        {
        case VTYPE_INT:
            {
                int val = record->QueryInt(row_id, column_index);

                if (column_var.int_value != val)
                {
                    return false;
                }
            }
            break;
        case VTYPE_INT64:
            {
                int64_t val = record->QueryInt64(row_id, column_index);

                if (column_var.int64_value != val)
                {
                    return false;
                }
            }
            break;
        case VTYPE_FLOAT:
            {
                float val = record->QueryFloat(row_id, column_index);

                if (column_var.float_value != val)
                {
                    return false;
                }
            }
            break;
        case VTYPE_DOUBLE:
            {
                double val = record->QueryDouble(row_id, column_index);

                if (column_var.double_value != val)
                {
                    return false;
                }
            }
            break;
        case VTYPE_STRING:
            {
                const char *val = record->QueryString(row_id, column_index);

                if (strcmp(val, column_var.str_value) != 0)
                {
                    return false;
                }
            }
            break;
        case VTYPE_WIDESTR:
            {
                const wchar_t *val = record->QueryWideStr(row_id, column_index);

                if (wcscmp(val, column_var.wstr_value) != 0)
                {
                    return false;
                }
            }
            break;
        default:
            return false;
        }
    }

    return true;
}

// ��@record��ȡ����@row_id�е����ݣ�����@dest��
int DomainRankList::ReadRow(IVarList &dest, IRecord *record, int row_id,
                            const RecordDefine *rec_def)
{
    LoopBeginCheck(a);
    for (int i=0; i<rec_def->columns_count; i++)
    {
        LoopDoCheck(a);
        
        switch (rec_def->columns_type[i])
        {
        case VTYPE_INT:
            dest << record->QueryInt(row_id, i);
            break;
        case VTYPE_INT64:
            dest << record->QueryInt64(row_id, i);
            break;
        case VTYPE_FLOAT:
            dest << record->QueryFloat(row_id, i);
            break;
        case VTYPE_DOUBLE:
            dest << record->QueryDouble(row_id, i);
            break;
        case VTYPE_STRING:
            dest << record->QueryString(row_id, i);
            break;
        case VTYPE_WIDESTR:
            dest << record->QueryWideStr(row_id, i);
            break;
        default:
            dest << "";
        }
    }

    return 0;
}

// ��@record��ȡ����@row_id�е����ݣ�����@row_data��, 
int DomainRankList::ReadRow(std::vector<ColumnVar> *row_data, IRecord *record, int row_id,
            const RecordDefine *rec_def)
{
    LoopBeginCheck(a);
    for (int i=0; i<rec_def->columns_count; i++)
    {
        LoopDoCheck(a);
        
        ColumnVar column;        
        column.column_index = i;

        int x = sizeof(column);
        ColumnVar * p = &column;

        switch (rec_def->columns_type[i])
        {
        case VTYPE_INT:
            column.int_value = record->QueryInt(row_id, i);
            break;
        case VTYPE_INT64:
            column.int64_value = record->QueryInt64(row_id, i);
            break;
        case VTYPE_FLOAT:
            column.float_value = record->QueryFloat(row_id, i);
            break;
        case VTYPE_DOUBLE:
            column.double_value = record->QueryDouble(row_id, i);
            break;
        case VTYPE_STRING:
            {
                const char *src = record->QueryString(row_id, i);
                StrSafeCopy(column.str_value, sizeof(column.str_value), src);
            }
            break;
        case VTYPE_WIDESTR:
            {
                const wchar_t *src = record->QueryWideStr(row_id, i);
                WstrSafeCopy(column.wstr_value, sizeof(column.wstr_value), src);
            }
            break;
        default:
            break;
        }

        row_data->push_back(column);
    }

    return 0;
}

// ����һ������
int DomainRankList::InsertRow(IRecord *the_record,
                             const std::vector<ColumnVar> *columns,
                             const RecordDefine *rec_def)
{
    // ��ʼ��һ������
    CVarList row_data;
    InitRowData(row_data, rec_def);

    // ���ô�������
    std::vector<ColumnVar>::const_iterator it = columns->begin();
    std::vector<ColumnVar>::const_iterator it_end = columns->end();
    
    LoopBeginCheck(a);
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(a);
        
        const ColumnVar &column = *it;
        SetColunmOfRow(row_data, &column, rec_def);
    }
    
    // ����Ƿ���Ҫ����
    if (rec_def->sort_columns_count == 0)
    {
        // ����Ҫ��������а�, ֱ�Ӳ��뵽���
        return InsertToRecord(row_data, -1, the_record, rec_def);
    }

    // TODO �Ľ������㷨
    // Ѱ�Һ��ʵĲ���λ��
    int row_count = the_record->GetRows();
    
    LoopBeginCheck(b);
    for (int i=0; i<row_count; i++)
    {
        LoopDoCheck(b);
        
        CVarList temp_row;
        ReadRow(temp_row, the_record, i, rec_def);

        int compare_ret = CompareRowData(row_data, temp_row, rec_def);

        if (rec_def->use_asc)
        {
            // ��������
            // �ҵ�һ�����ȼ��ߵ���,Ȼ���������ǰ��
            if (compare_ret < 0)
            {
                int row_id = (i-1) ? -1 : 0;
                return InsertToRecord(row_data, row_id, the_record, rec_def);
            }
        }
        else
        {
            // �������� 
            // �ҵ�һ�����ȼ��͵���,Ȼ���������ǰ��
            if (compare_ret > 0)
            {
                int row_id = (i-1)==-1 ? 0 : i;
                return InsertToRecord(row_data, row_id, the_record, rec_def);
            }
        }
    }

    // �ڱ�β����
    return InsertToRecord(row_data, -1, the_record, rec_def);
}

// ����һ������
int DomainRankList::UpdateRow(IRecord *the_record,
                             int row_id,
                             const std::vector<ColumnVar> *columns,
                             const RecordDefine *rec_def,
                             int update_strategy)
{
    // ��¼�Ƿ���Ҫ��������
    bool need_resort = false;

    std::vector<ColumnVar>::const_iterator it = columns->begin();
    std::vector<ColumnVar>::const_iterator it_end = columns->end();
    
	std::vector<ColumnVar> old_row_data;
	ReadRow(&old_row_data, the_record, row_id, rec_def);

    LoopBeginCheck(a);
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(a);
        
        const ColumnVar &column = *it;
        SetColunmOfRow(the_record, row_id, &column, rec_def, update_strategy);

        if (!need_resort)
        {
            // �鿴Ҫ���µ����Ƿ������������
            
            LoopBeginCheck(b);
            for (int i=0; i<rec_def->sort_columns_count; i++)
            {
                LoopDoCheck(b);
                
                if (column.column_index == rec_def->sort_columns[i])
                {
					// SetColunmOfRow����ִ�к�column��ֵӦ���б䶯,�������¶�ȡһ�±�����¼
					std::vector<ColumnVar> temp_column;
					ReadRow(&temp_column, the_record, row_id, rec_def);
					
					// ����������б䶯,����������
					if (static_cast<int>(temp_column.size()) > column.column_index 
						&& static_cast<int>(old_row_data.size()) > column.column_index)
					{
						int cmp_ret = CompareColumnData(&temp_column[column.column_index], &old_row_data[column.column_index], rec_def);
						if (cmp_ret != 0)
						{
							need_resort = true;

							break;
						}
					}
                }
            }
        }
    }

    if (need_resort)	
    {   
        // ��ɾ����Ȼ�����
        std::vector<ColumnVar> row_data;
        ReadRow(&row_data, the_record, row_id, rec_def);
        the_record->RemoveRow(row_id);
        InsertRow(the_record, &row_data, rec_def);
    }

    return 1;
}

// ɾ��һ������
int DomainRankList::DeleteRow(IRecord *the_record,
                             const ColumnVar *pk_column,
                             const RecordDefine *rec_def)
{
    int row_id = FindRowByColumn(the_record, pk_column, rec_def);

    if (row_id == -1)
    {
        return 0;
    }

    if (the_record->RemoveRow(row_id))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// ��@args��ȡ��һ����
bool DomainRankList::GetColumnFromArgs(ColumnVar *the_column,
                                     const IVarList &args,
                                     int args_index,
                                     const RecordDefine *rec_def)
{
    std::string column_name = args.StringVal(args_index);
    args_index++;

    int column_index = GetColumnIndex(column_name, rec_def);
    if (column_index == -1)
    {
        return false;
    }

    the_column->column_index = column_index;

    switch (rec_def->columns_type[column_index])
    {
    case VTYPE_INT:
        {
            if (args.GetType(args_index) == VTYPE_STRING)
            {
                const char *src = args.StringVal(args_index);
                StrSafeCopy(the_column->expression, sizeof(the_column->expression), src);
                the_column->value_type = VALUE_TYPE_EXP;
            }
            else
            {
                the_column->int_value = args.IntVal(args_index);
                the_column->value_type = VALUE_TYPE_IMMEDIATE;
            }
        }
        break;
    case VTYPE_INT64:
        {
            if (args.GetType(args_index) == VTYPE_STRING)
            {
                const char *src = args.StringVal(args_index);
                StrSafeCopy(the_column->expression, sizeof(the_column->expression), src);
                the_column->value_type = VALUE_TYPE_EXP;
            }
            else
            {
                the_column->int64_value = args.Int64Val(args_index);
                the_column->value_type = VALUE_TYPE_IMMEDIATE;
            }
        }
        break;
    case VTYPE_FLOAT:
        {

            if (args.GetType(args_index) == VTYPE_STRING)
            {
                const char *src = args.StringVal(args_index);
                StrSafeCopy(the_column->expression, sizeof(the_column->expression), src);
                the_column->value_type = VALUE_TYPE_EXP;
            }
            else
            {
                the_column->float_value = args.FloatVal(args_index);
                the_column->value_type = VALUE_TYPE_IMMEDIATE;
            }
        }
        break;
    case VTYPE_DOUBLE:
        {

            if (args.GetType(args_index) == VTYPE_STRING)
            {
                const char *src = args.StringVal(args_index);
                StrSafeCopy(the_column->expression, sizeof(the_column->expression), src);
                the_column->value_type = VALUE_TYPE_EXP;
            }
            else
            {
                the_column->double_value = args.DoubleVal(args_index);
                the_column->value_type = VALUE_TYPE_IMMEDIATE;
            }
        }
        break;
    case VTYPE_STRING:
        {
            const char *src = args.StringVal(args_index);
            StrSafeCopy(the_column->str_value, sizeof(the_column->str_value), src);
            the_column->value_type = VALUE_TYPE_IMMEDIATE;
        }
        break;
    case VTYPE_WIDESTR:
        {
            const wchar_t *src = args.WideStrVal(args_index);
            WstrSafeCopy(the_column->wstr_value, sizeof(the_column->wstr_value), src);
            the_column->value_type = VALUE_TYPE_IMMEDIATE;
        }
        break;
    default:
        return false;
        break;
    }

    return true;
}

// ��@args��ȡ�������
bool DomainRankList::GetColumnsFromArgs(std::vector<ColumnVar> *columns,
                                     const IVarList &args,
                                     int args_index,
                                     int column_count,
                                     const RecordDefine *rec_def)
{
    LoopBeginCheck(a);
    for (int i=0; i<column_count; i++)
    {
        LoopDoCheck(a);
        
        ColumnVar the_column;
        if (!GetColumnFromArgs(&the_column, args, args_index, rec_def))
        {
            return false;
        }
        args_index += 2; // ���� [column_name][column_value]

        columns->push_back(the_column);
    }
    
    return true;
}

// ��ȡ����
bool DomainRankList::LoadResource(IPubKernel* pPubKernel, IPubSpace* pPubSpace)
{

    if (!LoadColunmTypeConfig(pPubKernel, pPubSpace))
    {
        return false;
    }
    

    if (!LoadColunmNameConfig(pPubKernel, pPubSpace))
    {
        return false;
    }

    if (!LoadRankListConfig(pPubKernel, pPubSpace))
    {
        return false;
    }

    return true;
}

// ����������
bool DomainRankList::LoadColunmTypeConfig(IPubKernel* pPubKernel, IPubSpace* pPubSpace)
{
    /*
        <Property ID="int" Type="2"/>
    */

    std::string res_path = pPubKernel->GetResourcePath();

    std::string config_file = res_path;
    config_file.append(RANK_LIST_COLUMN_TYPE_FILE);

    CXmlFile xml(config_file.c_str());
    if (!xml.LoadFromFile())
    {
		::extend_warning_pub(LOG_ERROR, "%s does not exists.", config_file.c_str());
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
			::extend_warning_pub(LOG_ERROR, "%s: empty column type found.", config_file.c_str());

            return false;
        }

        if (type_value == -1)
        {
			::extend_warning_pub(LOG_ERROR, "%s:invalid column type value found. column is %s",
                config_file.c_str(), type_name.c_str());

            return false;
        }

        m_ColumnType.insert(ColumnTypeMap::value_type(type_name, type_value));
    }

    return true;
}

// ��������
bool DomainRankList::LoadColunmNameConfig(IPubKernel* pPubKernel, IPubSpace* pPubSpace)
{
    /*
        <Property ID="player_uid" ValueType="str" InitValue="" /> 
    */
    std::string res_path = pPubKernel->GetResourcePath();

    std::string config_file = res_path;
    config_file.append(RANK_LIST_COLUMN_FILE);

    CXmlFile xml(config_file.c_str());
    if (!xml.LoadFromFile())
    {
		::extend_warning_pub(LOG_ERROR, "%s does not exists.", config_file.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a);
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a);
        
        std::string column_name = sec_list.StringVal(i);
        std::string column_str_type = xml.ReadString(column_name.c_str(), "ValueType", "");
        std::string column_init_value = xml.ReadString(column_name.c_str(), "InitValue", "");
		bool IsNeedInit = xml.ReadInteger(column_name.c_str(), "IsNeedInit", 0) == 1;

        if (column_name.length() == 0)
        {                
			::extend_warning_pub(LOG_ERROR, "%s: empty column name found.", config_file.c_str());

            return false;
        }

        if (column_name.length() > RANK_LIST_MAX_COLUMN_NAME_LENGTH)
        {
			::extend_warning_pub(LOG_ERROR, "%s: Column name[%s] too long. max length is  %d",
                config_file.c_str(), column_name.c_str(), RANK_LIST_MAX_COLUMN_NAME_LENGTH);

            return false;
        }

        int the_type = GetColumnIntType(column_str_type);
        if (the_type == -1)
        {
			::extend_warning_pub(LOG_ERROR, "%s: invalid column type(%s).",
                config_file.c_str(), column_str_type.c_str());

            return false;
        }

        ColumnDefine col_def;
        col_def.column_name = column_name;
        col_def.column_type = the_type;
        col_def.init_value_in_str = column_init_value;
		col_def.bIsNeedInit = IsNeedInit;

        m_ColumnDefine.insert(ColumnDefineMap::value_type(col_def.column_name, col_def));
        //m_ColumnName.insert(ColumnNameMap::value_type(column_name, the_type));
    }

    return true;
}

// ���а�����
bool DomainRankList::LoadRankListConfig(IPubKernel* pPubKernel, IPubSpace* pPubSpace)
{
    /*
    <Property ID="player_level_rank_list" 
                MaxRows="1000" 
                ColumnsName="player_uid,player_name,player_career,player_nation,player_level,player_experience" 
                OutputColumns="player_name,player_career,player_nation,player_level,player_experience" 
                PrimaryKey="player_uid" 
                SortColumns="player_level,player_experience" 
                SortOrder="desc" /> 
    */
    std::string res_path = pPubKernel->GetResourcePath();

    std::string rank_list_config_file = res_path;
    rank_list_config_file.append(RANK_LIST_DEFINE_FILE);

    CXmlFile xml(rank_list_config_file.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = rank_list_config_file;
        err_msg.append(" does not exists.");
		::extend_warning_pub(LOG_ERROR, err_msg.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a);
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a);
        
		bool ignore = false;

        const char *rank_list_name = sec_list.StringVal(i);
        int associated_update_stragegy = xml.ReadInteger(rank_list_name, "AssociatedRankListStrategy", 0);
        const char *associated_rank_list = xml.ReadString(rank_list_name, "AssociatedRankList", "");
        int max_row_count = xml.ReadInteger(rank_list_name, "MaxRows", 1000);
        const char *columns_name = xml.ReadString(rank_list_name, "ColumnsName", "");
        const char *output_columns = xml.ReadString(rank_list_name, "OutputColumns", "");
        const char *primary_key = xml.ReadString(rank_list_name, "PrimaryKey", "");
        const char *sort_columns = xml.ReadString(rank_list_name, "SortColumns", "");
        const char *sort_order = xml.ReadString(rank_list_name, "SortOrder", "");
        const char *columns_to_reset = xml.ReadString(rank_list_name, "ColumnsToResetOnStart", "");
        int clear_strategy = xml.ReadInteger(rank_list_name, "ClearStrategy", 0);
		int clear_award = xml.ReadInteger(rank_list_name, "ClearAward", 0);
		int awardByNation = xml.ReadInteger(rank_list_name, "AwardByNation", 1);

        if (rank_list_name == NULL || columns_name == NULL
            || output_columns == NULL || primary_key== NULL
            || sort_columns == NULL || sort_order == NULL
            || columns_to_reset == NULL)
        {
            continue;
        }

        if (strlen(columns_name) == 0)
        {
			::extend_warning_pub(LOG_ERROR, "%s has no columns.", rank_list_name);

            continue;
        }

        RecordDefine rec_def;
        rec_def.rec_name = rank_list_name;
        rec_def.max_row = max_row_count;

        rec_def.associated_rank_list_count = 0;
        rec_def.columns_count = 0;
        rec_def.output_columns_count = 0;
        rec_def.sort_columns_count = 0;
        rec_def.columns_to_reset_count = 0;
        
        rec_def.associated_update_strategy = associated_update_stragegy;

        // ��ȡ������
        SaveStrToArray(associated_rank_list, rec_def.associated_rank_list, RANK_LIST_MAX_ASSOCIATED,
            &(rec_def.associated_rank_list_count));

        // ��ȡȫ����
        SaveStrToArray(columns_name, rec_def.columns_name, RANK_LIST_MAX_COLUMN_COUNT,
            &(rec_def.columns_count));

        ignore = false;
        
        LoopBeginCheck(b);
        for (int j=0; j<rec_def.columns_count; j++)
        {
            LoopDoCheck(b);
            
            rec_def.columns_type[j] = GetColumnType(rec_def.columns_name[j]);

            if (rec_def.columns_type[j] == -1)
            {
				::extend_warning_pub(LOG_ERROR, "%s: invalid column name %s.",
                    rank_list_name, (rec_def.columns_name[j]).c_str());

                ignore = true;
                break;
            }
        }
        
        if (ignore)
        {
            continue;
        }

        //��ȡ������
        std::string str_pk = primary_key;
        rec_def.pk_column = GetColumnIndex(str_pk, &rec_def);
        if (rec_def.pk_column == -1)
        {
			::extend_warning_pub(LOG_ERROR, "%s: invalid primary column name %s.",
                rank_list_name, primary_key);

            continue;
        }

        // ��ȡ�������
        std::string output_columns_array[RANK_LIST_MAX_COLUMN_COUNT];
        SaveStrToArray(output_columns, output_columns_array, RANK_LIST_MAX_COLUMN_COUNT,
            &(rec_def.output_columns_count));
            
        ignore = false;
        
        LoopBeginCheck(c);
        for (int j=0; j<rec_def.output_columns_count; j++)
        {
            LoopDoCheck(c);
            
            rec_def.output_columns[j] = GetColumnIndex(output_columns_array[j], &rec_def);

            if (rec_def.output_columns[j] == -1)
            {
				::extend_warning_pub(LOG_ERROR, "%s: invalid output column name %s.",
                    rank_list_name, (output_columns_array[j]).c_str());
                
                ignore = true;
                break;
            }
        }
        
        if (ignore)
        {
            continue;
        }

        // ��ȡ�������
        std::string sort_column_array[RANK_LIST_MAX_COLUMN_COUNT];
        SaveStrToArray(sort_columns, sort_column_array, RANK_LIST_MAX_COLUMN_COUNT,
            &(rec_def.sort_columns_count));

        ignore = false;
        
        LoopBeginCheck(d);
        for (int j=0; j<rec_def.sort_columns_count; j++)
        {
            LoopDoCheck(d);
            
            rec_def.sort_columns[j] = GetColumnIndex(sort_column_array[j], &rec_def);

            if (rec_def.sort_columns[j] == -1)
            {
				::extend_warning_pub(LOG_ERROR, "%s: invalid sort column name %s.",
                    rank_list_name, (sort_column_array[j]).c_str());

                ignore = true;
                continue;
            }
        }
        
        if (ignore)
        {
            continue;
        }

        // ����/����
        if (strcmp(sort_order, "desc") == 0)
        {
            rec_def.use_asc = false;
        }
        else
        {
            rec_def.use_asc = true;
        }

        // ����ʱ�������
        std::string columns_to_reset_array[RANK_LIST_MAX_COLUMN_COUNT];        
        SaveStrToArray(columns_to_reset, columns_to_reset_array, RANK_LIST_MAX_COLUMN_COUNT,
            &(rec_def.columns_to_reset_count));

        ignore = false;
        
        LoopBeginCheck(e);
        for (int j=0; j<rec_def.columns_to_reset_count; j++)
        {
            LoopDoCheck(e);
            
            rec_def.columns_to_reset[j] = GetColumnIndex(columns_to_reset_array[j], &rec_def);

            if (rec_def.columns_to_reset[j] == -1)
            {
				::extend_warning_pub(LOG_ERROR, "%s: invalid reset column name %s.",
                    rank_list_name, (columns_to_reset_array[j]).c_str());

                ignore = true;
                break;
            }
        }
        
        if (ignore)
        {
            continue;
        }

        // ��ղ���
        rec_def.clear_strategy = clear_strategy;
		rec_def.clear_award = clear_award;
		rec_def.bAwardByNation = awardByNation != 0;

        m_RecordDefine.insert(RecordDefineMap::value_type(rec_def.rec_name, rec_def));

    }

    return true;
}

// ���ݱ�����ֻ�ȡ��Ķ���
DomainRankList::RecordDefine * DomainRankList::GetRecordDefine(const char *rec_name)
{
    std::string search_key = rec_name;
    RecordDefineMap::iterator it = m_RecordDefine.find(search_key);
    if (it == m_RecordDefine.end())
    {
        return NULL;
    }
    else
    {
        return &(it->second);
    }
}

// ����������ȡ��������
const DomainRankList::ColumnDefine * DomainRankList::GetColumnDefine(const std::string &column_name)
{
    ColumnDefineMap::const_iterator it = m_ColumnDefine.find(column_name);
    if (it == m_ColumnDefine.end())
    {
        return NULL;
    }
    else
    {
        return &(it->second);
    }
}

// �����ַ���ʾ������@type_name, ��ȡ������ʾ������
// �Ҳ���ʱ����-1
int DomainRankList::GetColumnIntType(std::string &type_name)
{
    ColumnTypeMap::iterator it = m_ColumnType.find(type_name);
    if (it == m_ColumnType.end())
    {
        return -1;
    }
    else
    {
        return it->second;
    }
}

// ����������ȡ������
// �Ҳ���ʱ����-1
int DomainRankList::GetColumnType(std::string &column_name)
{
    ColumnDefineMap::const_iterator it = m_ColumnDefine.find(column_name);
    if (it == m_ColumnDefine.end())
    {
        return -1;
    }
    else
    {
        const ColumnDefine &col_def = it->second;
        return col_def.column_type;
    }

    //ColumnNameMap::iterator it = m_ColumnName.find(column_name);
    //if (it == m_ColumnName.end())
    //{
    //    return -1;
    //}
    //else
    //{
    //    return it->second;
    //}
}


// ��ȡ��@column_name�ڱ�@rec_def�е�����
// �Ҳ���ʱ����-1
int DomainRankList::GetColumnIndex(const std::string &column_name, const RecordDefine *rec_def)
{
    LoopBeginCheck(a);
    for (int i=0; i<rec_def->columns_count; i++)
    {
        LoopDoCheck(a);
        
        if (column_name == rec_def->columns_name[i])
        {
            return i;
        }
    }

    return -1;
}

// ��ȡ@the_column��ֵ, ����@dest
int DomainRankList::ReadColumnVar(CVarList &dest, const ColumnVar *the_column, const RecordDefine *rec_def)
{
    int column_type = rec_def->columns_type[the_column->column_index];

    switch (column_type)
    {
    case VTYPE_INT:
        dest << the_column->int_value;
        break;
    case VTYPE_INT64:
        dest << the_column->int64_value;
        break;
    case VTYPE_FLOAT:
        dest << the_column->float_value;
        break;
    case VTYPE_DOUBLE:
        dest << the_column->double_value;
        break;
    case VTYPE_STRING:
        dest << the_column->str_value;
        break;
    case VTYPE_WIDESTR:
        dest << the_column->wstr_value;
        break;
    default:
        dest << 0;
        break;
    }

    return 0;
}

// ����@row_dataָ����@the_column��ֵ
int DomainRankList::SetColunmOfRow(CVarList &row_data, const ColumnVar *the_column,
                                  const RecordDefine *rec_def)
{
    int column_type = rec_def->columns_type[the_column->column_index];

    switch (column_type)
    {
    case VTYPE_INT:
		{
			// �������ʽ
			if (the_column->value_type == VALUE_TYPE_EXP)
			{
				// �������ʽ
				ColumnVar temp_column;
				temp_column.int_value = 0;

				ParseRightExp(&temp_column, VTYPE_INT, the_column->expression);

				row_data.SetInt(the_column->column_index, temp_column.int_value);
			}
			else
			{
				row_data.SetInt(the_column->column_index, the_column->int_value);
			}
		}
        break;
    case VTYPE_INT64:
		{
			// �������ʽ
			if (the_column->value_type == VALUE_TYPE_EXP)
			{
				// �������ʽ
				ColumnVar temp_column;
				temp_column.int64_value = static_cast<int64_t>(0);

				ParseRightExp(&temp_column, VTYPE_INT, the_column->expression);

				row_data.SetInt64(the_column->column_index, temp_column.int64_value);
			}
			else
			{
				row_data.SetInt64(the_column->column_index, the_column->int64_value);
			}
		}
        
        break;
    case VTYPE_FLOAT:
		{
			// �������ʽ
			if (the_column->value_type == VALUE_TYPE_EXP)
			{
				// �������ʽ
				ColumnVar temp_column;
				temp_column.float_value = 0.0f;

				ParseRightExp(&temp_column, VTYPE_INT, the_column->expression);

				row_data.SetFloat(the_column->column_index, temp_column.float_value);
			}
			else
			{
				row_data.SetFloat(the_column->column_index, the_column->float_value);
			}
		}
        break;
    case VTYPE_DOUBLE:
		{
			// �������ʽ
			if (the_column->value_type == VALUE_TYPE_EXP)
			{
				// �������ʽ
				ColumnVar temp_column;
				temp_column.double_value = 0.0f;

				ParseRightExp(&temp_column, VTYPE_INT, the_column->expression);

				row_data.SetDouble(the_column->column_index, temp_column.double_value);
			}
			else
			{
				row_data.SetDouble(the_column->column_index, the_column->double_value);
			}
		}
        break;
    case VTYPE_STRING:
        row_data.SetString(the_column->column_index,  the_column->str_value);
        break;
    case VTYPE_WIDESTR:
        row_data.SetWideStr(the_column->column_index, the_column->wstr_value);
        break;
    default:
        break;
    }

    return 0;
}

// ����@the_record����@row_id�е�ָ����@the_column��ֵ
int DomainRankList::SetColunmOfRow(IRecord *the_record, int row_id, const ColumnVar *the_column,
                   const RecordDefine *rec_def, int update_strategy)
{
    int column_type = rec_def->columns_type[the_column->column_index];

    switch (column_type)
    {
    case VTYPE_INT:
        {
            int cur_val = the_record->QueryInt(row_id, the_column->column_index);
            
            if (the_column->value_type == VALUE_TYPE_EXP)
            {
                // �������ʽ
                ColumnVar temp_column;
                temp_column.int_value = cur_val;
                
                ParseRightExp(&temp_column, VTYPE_INT, the_column->expression);
                
                the_record->SetInt(row_id, the_column->column_index, temp_column.int_value);
            }
            else
            {
                // ���ݸ��²����������Ƿ���и���
                switch (update_strategy)
                {
                case UPDATE_STRATEGY_GREAT_THAN:
                {
                    if (the_column->int_value > cur_val)
                    {
                        the_record->SetInt(row_id, the_column->column_index, the_column->int_value); 
                    }
                }
                break;
                case UPDATE_STRATEGY_LESS_THAN:
                    {
                        if (the_column->int_value < cur_val)
                        {
                            the_record->SetInt(row_id, the_column->column_index, the_column->int_value); 
                        }
                    }
                    break;
                default:
                    the_record->SetInt(row_id, the_column->column_index, the_column->int_value); 
                }
            }
            
        }
        break;
    case VTYPE_INT64:
        {
            int64_t cur_val = the_record->QueryInt64(row_id, the_column->column_index);
            if (the_column->value_type == VALUE_TYPE_EXP)
            {
                // �������ʽ
                ColumnVar temp_column;
                temp_column.int64_value = cur_val;

                ParseRightExp(&temp_column, VTYPE_INT64, the_column->expression);

                the_record->SetInt64(row_id, the_column->column_index, temp_column.int64_value);
            }
            else
            {  
                // ���ݸ��²����������Ƿ���и���
                switch (update_strategy)
                {
                case UPDATE_STRATEGY_GREAT_THAN:
                    {
                        if (the_column->int64_value > cur_val)
                        {
                            the_record->SetInt64(row_id, the_column->column_index, the_column->int64_value); 
                        }
                    }
                    break;
                case UPDATE_STRATEGY_LESS_THAN:
                    {
                        if (the_column->int64_value < cur_val)
                        {
                            the_record->SetInt64(row_id, the_column->column_index, the_column->int64_value); 
                        }
                    }
                    break;
                default:
                    the_record->SetInt64(row_id, the_column->column_index, the_column->int64_value); 
                }
            }
        }
        break;
    case VTYPE_FLOAT:
        {
            float cur_val = the_record->QueryFloat(row_id, the_column->column_index);

            if (the_column->value_type == VALUE_TYPE_EXP)
            {
                // �������ʽ
                ColumnVar temp_column;
                temp_column.float_value = cur_val;

                ParseRightExp(&temp_column, VTYPE_FLOAT, the_column->expression);
                
                the_record->SetFloat(row_id, the_column->column_index, temp_column.float_value);
            }
            else
            {
                // ���ݸ��²����������Ƿ���и���
                switch (update_strategy)
                {
                case UPDATE_STRATEGY_GREAT_THAN:
                    {
                        if (the_column->float_value > cur_val)
                        {
                            the_record->SetFloat(row_id, the_column->column_index, the_column->float_value);
                        }
                    }
                    break;
                case UPDATE_STRATEGY_LESS_THAN:
                    {
                        if (the_column->float_value < cur_val)
                        {
                            the_record->SetFloat(row_id, the_column->column_index, the_column->float_value);
                        }
                    }
                    break;
                default:
                    the_record->SetFloat(row_id, the_column->column_index, the_column->float_value);
                }
            }
        }
        break;
    case VTYPE_DOUBLE:
        {
            double cur_val = the_record->QueryDouble(row_id, the_column->column_index);

            if (the_column->value_type == VALUE_TYPE_EXP)
            {
                // �������ʽ
                ColumnVar temp_column;
                temp_column.double_value = cur_val;

                ParseRightExp(&temp_column, VTYPE_DOUBLE, the_column->expression);

                the_record->SetDouble(row_id, the_column->column_index, temp_column.double_value);
            }
            else
            {
                // ���ݸ��²����������Ƿ���и���
                switch (update_strategy)
                {
                case UPDATE_STRATEGY_GREAT_THAN:
                    {
                        if (the_column->double_value > cur_val)
                        {
                            the_record->SetDouble(row_id, the_column->column_index, the_column->double_value);
                        }
                    }
                    break;
                case UPDATE_STRATEGY_LESS_THAN:
                    {
                        if (the_column->double_value < cur_val)
                        {
                            the_record->SetDouble(row_id, the_column->column_index, the_column->double_value);
                        }
                    }
                    break;
                default:
                    the_record->SetDouble(row_id, the_column->column_index, the_column->double_value);
                }
            }            
        }
        break;
    case VTYPE_STRING:
        the_record->SetString(row_id, the_column->column_index, the_column->str_value);
        break;
    case VTYPE_WIDESTR:
        the_record->SetWideStr(row_id, the_column->column_index, the_column->wstr_value);
        break;
    default:
        break;
    }

    return 0;
}

// ��ʼ����@rec_defһ�е�ֵ, ����@row_data
int DomainRankList::InitRowData(CVarList &row_data, const RecordDefine *rec_def)
{
    LoopBeginCheck(a);
    for (int i=0; i<rec_def->columns_count; i++)
    {
        LoopDoCheck(a);
        
        switch (rec_def->columns_type[i])
        {
        case VTYPE_INT:
            {
                int v = 0;
                row_data << v;
            }
            break;
        case VTYPE_INT64:
            {
                int64_t v = 0;
                row_data << v;
            }
            break;
        case VTYPE_FLOAT:
            row_data << 0.0F;
            break;
        case VTYPE_DOUBLE:
            {
                double v = 0.0;
                row_data << v;
            }
            break;
        case VTYPE_STRING:
            row_data << "";
            break;
        case VTYPE_WIDESTR:
            row_data << L"";
            break;
        default:
            break; 
        }
    }

    return 0;
}

// �Ƚ���������@row1 �� @row2�����ȼ�
int DomainRankList::CompareRowData(const CVarList &row1, const CVarList &row2,
                                  const RecordDefine *rec_def)
{
    LoopBeginCheck(a);
    for (int i=0; i<rec_def->sort_columns_count; i++)
    {
        LoopDoCheck(a);
        
        int column_index = rec_def->sort_columns[i];
        
        ColumnVar col1;
        ColumnVar col2;
        GetColumnValue(&col1, row1, column_index, rec_def);
        GetColumnValue(&col2, row2, column_index, rec_def);

        int cmp_ret = CompareColumnData(&col1, &col2, rec_def);

        if (cmp_ret != 0)
        {
            return cmp_ret;
        }
    }

    return 0;
}

// ��@row��ȡ��@column_index�е�ֵ, ����@the_column
int DomainRankList::GetColumnValue(ColumnVar *the_column, const CVarList &row,
                                  int column_index, const RecordDefine *rec_def)
{
    the_column->column_index = column_index;
    
    switch (rec_def->columns_type[column_index])
    {
    case VTYPE_INT:
        the_column->int_value = row.IntVal(column_index);
        break;
    case VTYPE_INT64:
        the_column->int64_value = row.Int64Val(column_index);
        break;
    case VTYPE_FLOAT:
        the_column->float_value = row.FloatVal(column_index);
        break;
    case VTYPE_DOUBLE:
        the_column->double_value = row.DoubleVal(column_index);
        break;
    case VTYPE_STRING:
        {
            const char *src = row.StringVal(column_index);
            StrSafeCopy(the_column->str_value, sizeof(the_column->str_value), src);
        }
        break;
    case VTYPE_WIDESTR:
        {
            const wchar_t *src = row.WideStrVal(column_index);
            WstrSafeCopy(the_column->wstr_value, sizeof(the_column->wstr_value), src);
        }
        break;
    default:
        
        break; 
    }

    return 0;
}

// �Ƚ�@column1��@column2�����ȼ�
int DomainRankList::CompareColumnData(const ColumnVar *column1,
                                     const ColumnVar *column2,
                                     const RecordDefine *rec_def)
{
    if (column1->column_index != column2->column_index)
    {
        return 0;
    }

    int ret = 0;

    switch (rec_def->columns_type[column1->column_index])
    {
    case VTYPE_INT:
        ret = column1->int_value - column2->int_value;
        break;
    case VTYPE_INT64:
        ret = (int)(column1->int64_value - column2->int64_value);
        break;
    case VTYPE_FLOAT:
        ret = ((column1->float_value - column2->float_value) > 0 ? 1 : -1);
        break;
    case VTYPE_DOUBLE:
        ret = ((column1->double_value - column2->double_value) > 0 ? 1 : -1);
        break;
    case VTYPE_STRING:
        ret = (int)(strlen(column1->str_value) - strlen(column2->str_value));
        break;
    case VTYPE_WIDESTR:
        ret = (int)(wcslen(column1->wstr_value) - wcslen(column2->wstr_value));
        break;
    default:
        ret = 0;
        break; 
    }

    return ret;
}

// �Ѷ��ŷָ����ַ������浽����
// ����: "a,b,c" ���浽 {"a", "b", "c"}
bool DomainRankList::SaveStrToArray(const char *str_src,
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


int DomainRankList::StrSafeCopy(char *dest, size_t dest_size, const char *src)
{
    memset(dest, 0, dest_size);
    size_t src_size = strlen(src);

    if (src_size >= dest_size)
    {
        memcpy(dest, src, dest_size-sizeof(char));
    }
    else
    {
        memcpy(dest, src, src_size);
    }

    return 0;
}

int DomainRankList::WstrSafeCopy(wchar_t *dest, size_t dest_size, const wchar_t *src)
{
    memset(dest, 0, dest_size);

    size_t src_size = wcslen(src) * sizeof(wchar_t);

    if (src_size >= dest_size)
    {
        memcpy(dest, src, dest_size-sizeof(wchar_t));
    }
    else
    {
        memcpy(dest, src, src_size);
    }

    return 0;
}

// ����һ������
int DomainRankList::InsertToRecord(const CVarList& row_data, int row_id, 
                              IRecord *record, const RecordDefine *rec_def)
{
    int row_count = record->GetRows();
    if (row_count == rec_def->max_row)
    {
         // ��ǰ�����Ѿ��ﵽ�������

        if (row_id == -1)
        {
            //��������������
            return 0;
        }
        else
        {
            //����ǰҪɾ��һ��
            record->RemoveRow(-1);
        }

    }

    if (row_id >= row_count)
    {
        if (record->AddRowValue(-1, row_data) > 0)
        {
            return 1;
        }
    }
    else
    {
        int new_row_id = record->AddRowValue(row_id, row_data);
        if (new_row_id >= 0)
        {
            return 1;
        }
    }

    return 0;
}

// ����RankList��ĳЩ��
int DomainRankList::ResetRankListColumn(IRecord *record, const RecordDefine *rec_def)
{
    int row_count = record->GetRows();

    LoopBeginCheck(a);
    for (int row=0; row<row_count; row++)
    {
        LoopDoCheck(a);
        
        LoopBeginCheck(b);
        for (int j=0; j<rec_def->columns_to_reset_count; j++)
        {
            LoopDoCheck(b);
            
            int column_index = rec_def->columns_to_reset[j];
            const ColumnDefine * col_def = GetColumnDefine(rec_def->columns_name[column_index]);
            if (col_def == NULL)
            {
                continue;
            }

			if (!col_def->bIsNeedInit)
			{
				continue;
			}

            switch (col_def->column_type)
            {
            case VTYPE_INT:
                {
                    int val = atoi(col_def->init_value_in_str.c_str());
                    record->SetInt(row, column_index, val);
                }
                break;
            case VTYPE_INT64:
                {
                    __int64 val = util_string_as_int64(col_def->init_value_in_str);
                    record->SetInt64(row, column_index, val);
                }
                break;
            case VTYPE_FLOAT:
                {
                    float val = (float)atof(col_def->init_value_in_str.c_str());
                    record->SetFloat(row, column_index, val);
                }
                break;
            case VTYPE_DOUBLE:
                {
                    double val = atof(col_def->init_value_in_str.c_str());
                    record->SetDouble(row, column_index, val);
                }
                break;
            case VTYPE_STRING:
                record->SetString(row, column_index, col_def->init_value_in_str.c_str());
                break;
            case VTYPE_WIDESTR:
                {
                    std::wstring val = util_string_as_widestr(col_def->init_value_in_str.c_str());
                    record->SetWideStr(row, column_index, val.c_str());
                }
                break;
            default:
                break; 
            }
        }
    }

    return 0;
}

// �����а�ĸ�����Ĳ���
// ���а�ĸ�����, ������¼���а���ϴ����ʱ����´����ʱ��
bool DomainRankList::CreateInfoTable(IPubData * pPubData)
{
    bool do_create = false;
    if (pPubData->FindRecord(INFO_TABLE))
    {
        IRecord *record = pPubData->GetRecord(INFO_TABLE);
        if (record == NULL)
        {
            return false;
        }

        // ������ͬ
        if (record->GetCols() != INFO_TABLE_COLUMN_COUNT)
        {
            pPubData->RemoveRecord(INFO_TABLE);
            do_create = true;
        }
    }
    else
    {
        do_create = true;
    }

    if (do_create)
    {
        CVarList column;
        column << VTYPE_STRING << VTYPE_STRING << VTYPE_STRING;

        return pPubData->AddRecord(INFO_TABLE, INFO_TABLE_MAX_ROW,
            INFO_TABLE_COLUMN_COUNT, column);
    }

    return true;
}

bool DomainRankList::AddToInfoTable(IPubData * pPubData, const std::string &entry_name)
{
    IRecord *rec = pPubData->GetRecord(INFO_TABLE);
    if (rec == NULL)
    {
        return false;
    }

    int row_id = rec->FindString(0, entry_name.c_str());
    if (row_id == -1)
    {
        CVarList entry;
        entry << entry_name.c_str() << "" << "";
        rec->AddRowValue(row_id, entry);
    }

    return true;
}

bool DomainRankList::ResetClearDate(IPubData * pPubData, const std::string &entry_name, 
                                    const std::string &last_clear_date,
                                    const std::string &next_clear_date)
{
    IRecord *rec = pPubData->GetRecord(INFO_TABLE);
    if (rec == NULL)
    {
        return false;
    }

    int row_id = rec->FindString(0, entry_name.c_str());
    if (row_id == -1)
    {
        CVarList entry;
        entry << entry_name.c_str();
        entry << last_clear_date.c_str();
        entry << next_clear_date.c_str();

        rec->AddRowValue(row_id, entry);
    }
    else
    {
        rec->SetString(row_id, 1, last_clear_date.c_str());
        rec->SetString(row_id, 2, next_clear_date.c_str());
    }

    return true;
}

bool DomainRankList::GetClearDate(IPubData * pPubData, const std::string &entry_name,
                                  std::string &last_clear_date, std::string &next_clear_date)
{
    IRecord *rec = pPubData->GetRecord(INFO_TABLE);
    if (rec == NULL)
    {
        return false;
    }

    int row_id = rec->FindString(0, entry_name.c_str());
    if (row_id == -1)
    {
        return false;
    }
    else
    {
        last_clear_date.append(rec->QueryString(row_id, 1));
        next_clear_date.append(rec->QueryString(row_id, 2));
    }

    return true;
}

// �Ƚ���������, ���
// date1 ���� date2 ���ش���0����, date1 ���� date2 ����С��0����, date1 ���� date2 ����0
// date �ĸ�ʽ��: yyyy-dd-mm
int DomainRankList::CompareDate(const std::string &date1, const std::string &date2)
{
    if (date1.length() == 0)
    {
        return -1;
    }

    if (date2.length() == 0)
    {
        return 1;
    }

    CVarList date1_list;
    util_split_string(date1_list, date1, "-");
    
    if (date1_list.GetCount() != 3)
    {
        return -1;
    }

    CVarList date2_list;
    util_split_string(date2_list, date2, "-");

    if (date1_list.GetCount() != 3)
    {
        return 1;
    }

    int date1_year = atoi(date1_list.StringVal(0));
    int date2_year = atoi(date2_list.StringVal(0));

    if (date1_year != date2_year)
    {
        return date1_year - date2_year;
    }

    int date1_month = atoi(date1_list.StringVal(1));
    int date2_month = atoi(date2_list.StringVal(1));
    
    if (date1_month != date2_month)
    {
        return date1_month - date2_month;
    }

    int date1_day = atoi(date1_list.StringVal(2));
    int date2_day = atoi(date2_list.StringVal(2));

    if (date1_day != date2_day)
    {
        return date1_day - date2_day;
    }

    return 0;
}

// ��ȡ���������
std::string DomainRankList::GetDateOfToday()
{
    __time64_t current = _time64(NULL);

    tm * local = _localtime64(&current);

    char str_date[256] = {0};
    SPRINTF_S(str_date, "%d-%02d-%02d", local->tm_year+1900,
        local->tm_mon+1, local->tm_mday);

    std::string temp_ret = str_date;
    return temp_ret;
}

// ��ȡ���������
std::string DomainRankList::GetDateOfNextDay()
{
    __time64_t current = _time64(NULL);
    __time64_t tomorow = current + SECONDS_OF_ONE_DAY; // ��һ��

    tm * local_tomorrow = _localtime64(&tomorow);

    char str_date[256] = {0};
    SPRINTF_S(str_date, "%d-%02d-%02d", local_tomorrow->tm_year+1900,
        local_tomorrow->tm_mon+1, local_tomorrow->tm_mday);

    std::string temp_ret = str_date;
    return temp_ret;
}

// ��ȡ����һ������
std::string DomainRankList::GetDateOfNextMonday()
{
    __time64_t current = _time64(NULL);
    tm * local = _localtime64(&current);

    int week_day = local->tm_wday; // 0-6
    if (week_day == 0)
    {
        week_day = 7; //����
    }
    int day_offset = 7 - week_day + 1;

    __time64_t next_monday = current + day_offset * SECONDS_OF_ONE_DAY;
    tm * local_next_monday = _localtime64(&next_monday);

    char str_date[256] = {0};
    SPRINTF_S(str_date, "%d-%02d-%02d", local_next_monday->tm_year+1900,
        local_next_monday->tm_mon+1, local_next_monday->tm_mday);

    std::string temp_ret = str_date;
    return temp_ret;
}

// ��ȡ�¸���һ�ŵ�����
std::string DomainRankList::GetDateOfNextMonth()
{
    __time64_t current = _time64(NULL);
    tm * local = _localtime64(&current);

    int year = local->tm_year + 1900;
    int month = local->tm_mon + 1; //tm_mon �ķ�Χ�� 0-11
    int day_of_month = 1;

    if (month == 12)
    {
        // ���������һ����, �¸��¾�������� 1.1
        year += 1;
        month = 1;
    }
    else
    {
        month += 1;
    }

    char str_date[256] = {0};
    SPRINTF_S(str_date, "%d-%02d-%02d", year, month, day_of_month);

    std::string temp_ret = str_date;
    return temp_ret;
}

// �����ұ��ʽ, �ұ��ʽ�Ƕ����ֽ��е���ְ����
// �ұ��ʽ�ĸ�ʽΪ�� (+)n ���� (-)n
bool DomainRankList::ParseRightExp(ColumnVar *the_column, int column_type, const char *exp)
{
    int exp_len = (int) strlen(exp);
    if (exp_len < 4)
    {
        return false;
    }

    // ��ȡ������
    const char the_operator = *(exp+1);

    // ��ȡ������
    const char *the_operand = exp + 3;

    bool ret = false;
    switch (column_type)
    {
    case VTYPE_INT:
        {
            int val = atoi(the_operand);
            switch(the_operator)
            {
            case '+':
                {
                    the_column->int_value += val;
                    ret = true;
                }
                break;
            case '-':
                {
                    the_column->int_value -= val;
                    ret = true;
                }
                break;
            default:
                break;
            }
        }
        break;
    case VTYPE_INT64:
        {
            __int64 val = _atoi64(the_operand);
            switch(the_operator)
            {
            case '+':
                {
                    the_column->int64_value += val;
                    ret = true;
                }
                break;
            case '-':
                {
                    the_column->int64_value -= val;
                    ret = true;
                }
                break;
            default:
                break;
            }
        }
        break;
    case VTYPE_FLOAT:
        {
            float val = (float)atof(the_operand);
            switch(the_operator)
            {
            case '+':
                {
                    the_column->float_value += val;
                    ret = true;
                }
                break;
            case '-':
                {
                    the_column->float_value -= val;
                    ret = true;
                }
                break;
            default:
                break;
            }
        }
        break;
    case VTYPE_DOUBLE:
        {
            double val = atof(the_operand);
            switch(the_operator)
            {
            case '+':
                {
                    the_column->double_value += val;
                    ret = true;
                }
                break;
            case '-':
                {
                    the_column->double_value -= val;
                    ret = true;
                }
                break;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }

    return ret;
}

// ������ҵȼ�ȡ�����а�ȼ����ֱ���
std::string DomainRankList::GetDifLvlRankListName(int player_lvl)
{
	int left = 0;
	int right = 0;
	if (player_lvl < 30)
	{
		left = (player_lvl/10)*10 == 0 ? 1 : (player_lvl/10)*10;
		right = ((player_lvl + 10)/10)*10;
	}
	else
	{
		if (player_lvl %10 < 5)
		{
			left = (player_lvl/10)*10;
			right = (player_lvl/10)*10 + 5;
		}
		else
		{
			left = (player_lvl/10)*10 + 5;
			right = ((player_lvl + 5)/10)*10;
		}
	}

	//ƴ�ӱ���
	char rank_list_name_lvl[256] = {0};
	SPRINTF_S(rank_list_name_lvl, "%s_%d_%d", PLAYER_LEVEL_RANK_LIST,left,right);
	//rank_list_name = std::string(rank_list_name_lvl);
	return rank_list_name_lvl;
}

// ������ҵȼ�(����GetDifLvlRankListName����,��͵ȼ�����,��ȡ���ݵȼ�)
int DomainRankList::CompatiblePlayerLevel(int player_lvl)
{
	// С��30��,ÿ10��һ���ȼ���
	if( player_lvl < 30 )
	{
		player_lvl -= 10;
	}
	// ����30��,ÿ5��һ���ȼ���
	else
	{
		player_lvl -= 5;
	}

	return player_lvl;
};

// �ؽ����а�, @old_data �������а����ݵĴ������
bool DomainRankList::RecreateRankList(IPubData * pDomainData, const RecordDefine &rec_def,
    OldDataStrategy old_data)
{   
    // �°���ж���
    CVarList rank_list_columns;

    LoopBeginCheck(a);
    for (int i=0; i<rec_def.columns_count; i++)
    {
        LoopDoCheck(a);

        rank_list_columns << rec_def.columns_type[i];
    }
    
    if (old_data == DISCARD_OLD_DATA)
    {
        // ֱ�Ӵ��������а�, ����������
        
        pDomainData->RemoveRecord(rec_def.rec_name.c_str());
        
        bool created = pDomainData->AddRecord(rec_def.rec_name.c_str(), rec_def.max_row,
            int(rank_list_columns.GetCount()), rank_list_columns);

        if (created)
        {
            // ����������(����)
            pDomainData->SetRecordKey(rec_def.rec_name.c_str(), rec_def.pk_column);
            pDomainData->SetRecordSave(rec_def.rec_name.c_str(), true);
        }
        
        return true;
    }
    
    //
    // ��Ҫ���������а�������ƶ����µ����а�
    //
    
    bool has_old_record = true;
    
    // ���еı�
    IRecord *old_record = pDomainData->GetRecord(rec_def.rec_name.c_str());
    if (old_record == NULL)
    {
        has_old_record = false;
        
        bool created = pDomainData->AddRecord(rec_def.rec_name.c_str(), rec_def.max_row,
            int(rank_list_columns.GetCount()), rank_list_columns);

        if (created)
        {
            // ����������(����)
            pDomainData->SetRecordKey(rec_def.rec_name.c_str(), rec_def.pk_column);
            pDomainData->SetRecordSave(rec_def.rec_name.c_str(), true);
        }
        
        return created;
    }
    
    int old_record_column_count = old_record->GetCols();
    
    CVarList new_fields;
    
    LoopBeginCheck(b);
    for (int i=old_record_column_count; i<rec_def.columns_count; ++i)
    {
        LoopDoCheck(b);

        switch(rec_def.columns_type[i])
        {
        case VTYPE_INT:
            new_fields << 0;
            break;
        case VTYPE_INT64:
            new_fields << (int64_t)0;
            break;
        case VTYPE_FLOAT:
            new_fields << 0.0f;
            break;
        case VTYPE_DOUBLE:
            new_fields << 0.0;
            break;
        case VTYPE_STRING:
            new_fields << "";
            break;
        case VTYPE_WIDESTR:
            new_fields << L"";
            break;
        default:
            return false;
        }   
    }
       
    // ������ʱ��, ��������������
    std::string temp_rank_list_name = rec_def.rec_name;
    temp_rank_list_name.append("temp");

    bool created = pDomainData->AddRecord(temp_rank_list_name.c_str(), rec_def.max_row,
        int(rank_list_columns.GetCount()), rank_list_columns);
    if (!created)
    {
        return false;
    }
    
    IRecord *temp_record = pDomainData->GetRecord(temp_rank_list_name.c_str());
    if (temp_record == NULL)
    {
        return false;
    }
    
    CVarList temp_rows;
    int old_data_count = old_record->GetRows();
    
    LoopBeginCheck(c);
    for (int i=0; i<old_data_count; ++i)
    {
        LoopDoCheck(c);
        
        temp_rows.Clear();
        
        old_record->QueryRowValue(i, temp_rows);
        temp_rows << new_fields;
        
        temp_record->AddRowValue(-1, temp_rows);
    }
    
    // ɾ���ɱ�
    pDomainData->RemoveRecord(rec_def.rec_name.c_str());
    
    // �����±�
    created = pDomainData->AddRecord(rec_def.rec_name.c_str(), rec_def.max_row,
        int(rank_list_columns.GetCount()), rank_list_columns);
    if (!created)
    {
        pDomainData->RemoveRecord(temp_rank_list_name.c_str());
        return false;
    }
    
    IRecord *new_record = pDomainData->GetRecord(rec_def.rec_name.c_str());
    if (new_record == NULL)
    {
        pDomainData->RemoveRecord(temp_rank_list_name.c_str());
        return false; 
    }
    
    // ����ʱ���и������ݵ��±�
    int temp_data_count = temp_record->GetRows();
    
    LoopBeginCheck(d);
    for (int i=0; i<temp_data_count; ++i)
    {
        LoopDoCheck(d);
        
        temp_rows.Clear();
        
        temp_record->QueryRowValue(i, temp_rows);
        new_record->AddRowValue(-1, temp_rows);
        
    }
    
    // ɾ����ʱ��
    pDomainData->RemoveRecord(temp_rank_list_name.c_str());
    
    return true;
}
