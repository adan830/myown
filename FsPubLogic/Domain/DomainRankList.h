//---------------------------------------------------------
//�ļ���:       DomainRankList.h
//��  ��:       ���а�����
//˵  ��:       �����洢�������б�
//              - ÿ������붨��һ��������
//              - �������Լ�����˽������ RecordDefine.
//              
//          
//��������:      2014��11��04��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#ifndef PUB_DOMAIN_DOMAIN_RANK_LIST_H
#define PUB_DOMAIN_DOMAIN_RANK_LIST_H

#include "IDomainData.h"
#include "FsGame/Define/PubDefine.h"

#include <string>
#include <map>
#include <vector>

#define RANK_LIST_MAX_COLUMN_COUNT         50   // һ�����а���������
#define RANK_LIST_MAX_COLUMN_NAME_LENGTH   128  // ����������������
#define RANK_LIST_MAX_ASSOCIATED           5    // ÿ�����а������Թ��������а���
#define RANK_LIST_MAX_COLUMN_SIZE          256  // ���а�����������󳤶�

class DomainRankList : public IDomainData
{
public: 
    DomainRankList();

    virtual ~DomainRankList();

public:

    enum OldDataStrategy
    {
        DISCARD_OLD_DATA    = 0, // ����������
        KEEP_OLD_DATA       = 1, // ����������
    };

    // ������а�Ķ�ʱ��
    static int ClearRankListTimer(IPubKernel *pPubKernel, const char *space_name,
        const wchar_t *data_name, int time);
	// �ⲿ��ȡ���а�����
	static IPubData* GetRankListData(IPubKernel* pPubKernel);

    // ������������
    virtual const std::wstring & GetDomainName(IPubKernel* pPubKernel);

    //�Ƿ񱣴浽���ݿ�
    virtual bool GetSave(){return true;}

    virtual int OnCreate(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

    virtual int OnLoad(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

    /// �������Գ�������������Ϣ
    virtual int OnMessage(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
        int source_id, int scene_id, const IVarList& msg);

    // ����@msg������ѯ, ������ص�@out_result   
    // �������� -1 ��ʾ����ʧ��
    //
    // @args �ĸ�ʽ��
    //     [�������Զ���ID]
    //     [���а������][��ʼ�к�][�������ص�����]
    //     [��1��][��1ֵ][��2��][��2ֵ]...
    //
    // ��Ч�� SQL ���:
    //     SELELCT TOP [�������ص�����] FROM [���а������]
    //     WHERE 
    //       [��1��]=[��1ֵ] AND [��2��]=[��2ֵ] ....
    //   
    // ���ظ�ʽ��
    // 
    //     [�������Զ���ID][���а������][��ʼ����][�������ص�����]
    //     [��ѯ���������][��ѯ����ĵ�����]
    //     [ȫ������][�ڱ��β�ѯ�����е�����][����][��ֵ]...
    //      ...
    int Query(IPubKernel* pPubKernel, IPubSpace* pPubSpace, IVarList &out_result, 
        const IVarList& msg, int msg_start_index=0);

    // ����(����)��Ŀ, ���ظ��µ�����, ����-1��ʾ���ִ���
    // @msg �ĸ�ʽ
    //
    //      [�������Զ���ID]
    //      [���а������][�����Ŀ�����ڵĴ���ʽ][�����е�����][�����е�ֵ][��1������][��1��ֵ]...
    //      [�����Ŀ�����ڵĴ���ʽ]: ����Ϊ 0��ʾ�����κδ���1��ʾ�����ھͲ���
    //
    // ���磺["level_rank_list"][0]["player_uid"]["1234"]["player_level"][3]["player_experience"][6]
    //  ��Ч�� SQL ���:
    //     UPDATE level_rank_list
    //     SET player_level=3, player_experience=6
    //     WHERE player_uid="1234"
    //
    int UpdateEntry(IPubKernel* pPubKernel, IPubSpace* pPubSpace, const IVarList& msg,
        int msg_start_index=0);

    // ɾ����Ŀ, ����ɾ��������, ����-1��ʾ���ִ���
    // @msg �ĸ�ʽ
    //
    //      [���а������][������][����ֵ]
    // 
    // ��Ч��SQL ���: 
    //    DELETE FROM [���а������] WHERE [������]=[����ֵ]
    int DeleteEntry(IPubKernel* pPubKernel, IPubSpace* pPubSpace, const IVarList& msg,
        int msg_start_index=0);

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
    bool SwapEntry(IPubKernel* pPubKernel, IPubSpace* pPubSpace, const IVarList& msg,
        int msg_start_index=0);

	// ������а�-ǰ����, 0��ʾ�ɹ�, ����-1��ʾ���ִ���
	// @msg �ĸ�ʽ
	//
	//      [�������Զ���ID]
	//      [���а������]
	// ��Ч�� SQL ��䣺 TRUNCATE TABLE [���а������]
	//int BeforeClearRankList(IPubKernel* pPubKernel, IPubSpace* pPubSpace, const IVarList& msg);

    // ������а�, 0��ʾ�ɹ�, ����-1��ʾ���ִ���
    // @msg �ĸ�ʽ
    //
    //      [�������Զ���ID]
    //      [���а������]
    // ��Ч�� SQL ��䣺 TRUNCATE TABLE [���а������]
    int ClearRankList(IPubKernel* pPubKernel, IPubSpace* pPubSpace, const IVarList& msg);

    // ��ѯ@column_name �����а�@rank_list_name �е�����
    // �Ҳ���ʱ����-1
    int GetColumnIndex(const std::string &column_name, const std::string &rank_list_name);

    // ��ѯ@column_name �����а�@rank_list_name �е�����
    // �Ҳ���ʱ����-1
    int GetColumnIndexInOutput(const std::string &column_name, const std::string &rank_list_name);

	// ���ݵȼ��͹����ҵ�����������һ�����
	bool FindPlayerUidByLevelNation(IPubKernel* pPubKernel, IPubSpace * pPubSpace, int nLevel, int nNation,
										 IVarList& outData, const std::vector<std::string>& vRecUid);
										 
private:

    // ��Ľṹ����
    struct RecordDefine
    {
		RecordDefine() : rec_name(""), max_row(0), columns_count(0), pk_column(0), output_columns_count(0),
			sort_columns_count(0), use_asc(0), columns_to_reset_count(0), clear_strategy(0), clear_award(0),
			associated_update_strategy(0), associated_rank_list_count(0), bAwardByNation(true)
		{
			memset(columns_type, 0, sizeof(columns_type));
			memset(output_columns, 0, sizeof(output_columns));
			memset(sort_columns, 0, sizeof(sort_columns));
			memset(columns_to_reset, 0, sizeof(columns_to_reset));
		}
        std::string rec_name;
        int max_row;
        int columns_count; // ������

        int pk_column;     // ������

        int columns_type[RANK_LIST_MAX_COLUMN_COUNT];         // �е���������
        std::string columns_name[RANK_LIST_MAX_COLUMN_COUNT]; // ��������

        int output_columns_count;                            // ���������
        int output_columns[RANK_LIST_MAX_COLUMN_COUNT];      // �������

        int sort_columns_count;                               // �������������
        int sort_columns[RANK_LIST_MAX_COLUMN_COUNT];         // �����������
        
        int use_asc; // �Ƿ���������

        int columns_to_reset_count;                                // Ҫ���������
        int columns_to_reset[RANK_LIST_MAX_COLUMN_COUNT]; // ����������ʱֵ�������

        int clear_strategy;   // ��ղ���
		int clear_award;      // ����Ƿ��н���
		bool bAwardByNation;  // �Ƿ���ݹ�������������

        int associated_update_strategy;                  // �������а�ĸ��²���
        int associated_rank_list_count;
        std::string associated_rank_list[RANK_LIST_MAX_ASSOCIATED];

    };

    struct ColumnDefine 
    {
		ColumnDefine() : column_name(""), column_type(0), init_value_in_str(""), bIsNeedInit(false){}
        std::string column_name;         // ����
        int column_type;                 // ������
        std::string init_value_in_str;   // �ַ�����ʾ�ĳ�ʼֵ
		bool bIsNeedInit;				 // �Ƿ���Ҫ��ʼֵ
    };

    // ����б���
    struct ColumnVar
    {
        int column_index;
        
        int value_type; // ֵ����: ���������Ǳ��ʽ

        // ������
        union
        {
            int int_value;
            int64_t int64_value;
            float float_value;
            double double_value;
            char str_value[RANK_LIST_MAX_COLUMN_SIZE];
            wchar_t wstr_value[RANK_LIST_MAX_COLUMN_SIZE];
        };

        // ���ʽ
        char expression[RANK_LIST_MAX_COLUMN_SIZE]; // ֵ�ı��ʽ
        ColumnVar() : value_type(0),int_value(1)
		{
			memset(expression, 0, RANK_LIST_MAX_COLUMN_SIZE * sizeof(char));
		}
    };



private:

    // ����������ѯ���а�
    int OnQuery(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
        int source_id, int scene_id, const IVarList& msg);

    // ��Ŀ�и���, ˢ�����а�
    int OnUpdateEntry(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
        int source_id, int scene_id, const IVarList& msg);

    // ɾ�����а����Ŀ
    int OnDeleteEntry(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
        int source_id, int scene_id, const IVarList& msg);
    
    // ����������а�
    int OnClear(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
        int source_id, int scene_id, const IVarList& msg);

    // ��ѯ���а����������
    int OnQueryRecordDate(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
        int source_id, int scene_id, const IVarList& msg);

    // �������а��е�����
    int OnSwapEntry(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
        int source_id, int scene_id, const IVarList& msg);

    // ��@the_record�в��ҷ�������@the_column��һ������, ����������, û�ҵ�����-1
    int FindRowByColumn(IRecord *the_record, const ColumnVar *the_column,
        const RecordDefine *rec_def);
        
    // ��@the_record�в��ҷ�������@the_column������������
    void FindRowsByColumn(IRecord *the_record, const ColumnVar *the_column,
        const RecordDefine *rec_def, std::vector<int> *row_id_list);

    // ��@record��ȡ����@row_id�е����ݣ�����@dest��, 
    // ��@rec_def.output_columns ������������Щ��
    int ReadRowOfOutputCols(IVarList &dest, IRecord *record, int row_id,
        const RecordDefine *rec_def);

    // ���� @columns��ѯ����� �ӽ���ĵ�@start_row��ʼ��ȡ@row_count_wanted������, ����@out_list
    // @row_count ��¼���ص�����, @coulumn_count ��¼���ص�����
    // �������ݸ�ʽ��: [seq_no_all][seq_no_query][column1]...
    int ReadRowOfOutputCols(IVarList &out_list, int *row_count, int *coulumn_count,
        IRecord *record, const std::vector<ColumnVar> *columns,
        int start_row, int row_count_wanted, const RecordDefine *rec_def);

    // ���� @columns��ѯ����� �ӽ���ĵ�@start_row��ʼ��ȡ@row_count_wanted������, Ȼ����
    // ����м�����������@sub_column ����Ŀ����@out_list
    // @row_count ��¼���ص�����, @coulumn_count ��¼���ص�����
    // �������ݸ�ʽ��: [seq_no_all][seq_no_query][column1]...
    int ReadRowOfOutputCols(IVarList &out_list, int *row_count, int *coulumn_count,
        IRecord *record, const std::vector<ColumnVar> *columns, const std::vector<ColumnVar> *sub_column,
        int start_row, int row_count_wanted, const RecordDefine *rec_def);

    // ����@column �鿴���Ƿ�ƥ��
    bool IsRowMatch(int row_id, IRecord *record, const std::vector<ColumnVar> *columns,
        const RecordDefine *rec_def);

    // ��@record��ȡ����@row_id�е����ݣ�����@dest��
    int ReadRow(IVarList &dest, IRecord *record, int row_id, const RecordDefine *rec_def);

    // ��@record��ȡ����@row_id�е����ݣ�����@row_data��, 
    int ReadRow(std::vector<ColumnVar> *row_data, IRecord *record, int row_id,
        const RecordDefine *rec_def);

    // ����һ������
    int InsertRow(IRecord *the_record, const std::vector<ColumnVar> *columns,
        const RecordDefine *rec_def);

    // ����һ������
    int UpdateRow(IRecord *the_record, int row_id, const std::vector<ColumnVar> *columns,
        const RecordDefine *rec_def, int update_strategy = 0);

    // ɾ��һ������
    int DeleteRow(IRecord *the_record, const ColumnVar *pk_column, const RecordDefine *rec_def);

    // ��@args��ȡ��һ����
    bool GetColumnFromArgs(ColumnVar *the_column, const IVarList &args,
        int args_index, const RecordDefine *rec_def);

    // ��@args��ȡ�������
    bool GetColumnsFromArgs(std::vector<ColumnVar> *columns, const IVarList &args,
        int args_index, int column_count, const RecordDefine *rec_def);

    // ��ȡ����
    bool LoadResource(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

    // ����������
    bool LoadColunmTypeConfig(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

    // ��������
    bool LoadColunmNameConfig(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

    // ���а�����
    bool LoadRankListConfig(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

    // ��ʼ�����а�
    void InitAllRankList(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

    // ���ݱ�����ֻ�ȡ��Ķ���
    RecordDefine * GetRecordDefine(const char *rec_name);

    // ����������ȡ��������
    const ColumnDefine * GetColumnDefine(const std::string &column_name);

    // �����ַ���ʾ������@type_name, ��ȡ������ʾ������
    // �Ҳ���ʱ����-1
    int GetColumnIntType(std::string &type_name);

public: 

    // ����������ȡ������
    // �Ҳ���ʱ����-1
    int GetColumnType(std::string &column_name);

private:

    // ��ȡ��@column_name�ڱ�@rec_def�е�����
    // �Ҳ���ʱ����-1
    int GetColumnIndex(const std::string &column_name, const RecordDefine *rec_def);

    // ��ȡ@the_column��ֵ, ����@dest
    int ReadColumnVar(CVarList &dest, const ColumnVar *the_column,
        const RecordDefine *rec_def);

    // ��@row��ȡ��@column_index�е�ֵ, ����@the_column
    int GetColumnValue(ColumnVar *the_column, const CVarList &row,
        int column_index, const RecordDefine *rec_def);

    // ����@the_record����@row_id�е�ָ����@the_column��ֵ
    int SetColunmOfRow(IRecord *the_record, int row_id, const ColumnVar *the_column,
        const RecordDefine *rec_def, int update_strategy = 0);

    // ����@row_dataָ����@the_column��ֵ
    int SetColunmOfRow(CVarList &row_data, const ColumnVar *the_column,
        const RecordDefine *rec_def);

    // ��ʼ����@rec_defһ�е�ֵ, ����@row_data
    int InitRowData(CVarList &row_data, const RecordDefine *rec_def);

    // �Ƚ���������@row1 �� @row2�����ȼ�
    int CompareRowData(const CVarList &row1, const CVarList &row2,
        const RecordDefine *rec_def);

    // �Ƚ�@column1��@column2�����ȼ�
    int CompareColumnData(const ColumnVar *column1, const ColumnVar *column2,
        const RecordDefine *rec_def);

    //  ��������

    // �Ѷ��ŷָ����ַ������浽����
    // ����: "a,b,c" ���浽 {"a", "b", "c"}
    bool SaveStrToArray(const char *str_src, std::string *str_array, int array_length,
        int *actual_size);

    // ��ȫ���ַ�����������
    int StrSafeCopy(char *dest, size_t dest_size, const char *src);

    // ��ȫ���ַ�����������
    int WstrSafeCopy(wchar_t *dest, size_t dest_size, const wchar_t *src);

    // ��@record�ĵ�@row_id�в���һ������@row_data
    int InsertToRecord(const CVarList& row_data, int row_id, IRecord *record, const RecordDefine *rec_def);

    // ����RankList��ĳЩ��
    int ResetRankListColumn(IRecord *record, const RecordDefine *rec_def);

    // �����а�ĸ�����Ĳ���
    // ���а�ĸ�����, ������¼���а���ϴ����ʱ����´����ʱ��
    bool CreateInfoTable(IPubData * pPubData);
    bool AddToInfoTable(IPubData * pPubData, const std::string &entry_name);
    bool ResetClearDate(IPubData * pPubData, const std::string &entry_name, 
        const std::string &last_clear_date, const std::string &next_clear_date);
    bool GetClearDate(IPubData * pPubData, const std::string &entry_name,
        std::string &last_clear_date, std::string &next_clear_date);

    // �Ƚ���������, ���
    // date1 ���� date2 ���ش���0����, date1 ���� date2 ����С��0����, date1 ���� date2 ����0
    // date �ĸ�ʽ��: yyyy-dd-mm
    static int CompareDate(const std::string &date1, const std::string &date2);

    // ��ȡ���������
    static std::string GetDateOfToday();
    // ��ȡ���������
    static std::string GetDateOfNextDay();
    // ��ȡ����һ������
    static std::string  GetDateOfNextMonday();
    // ��ȡ�¸���һ�ŵ�����
    static std::string  GetDateOfNextMonth();

    // �����ұ��ʽ, �ұ��ʽ�Ƕ����ֽ��е���ְ����
    // �ұ��ʽ�ĸ�ʽΪ�� (+)n ���� (-)n
    bool ParseRightExp(ColumnVar *the_column, int column_type, const char *exp);

	// ������ҵȼ�ȡ�����а�ȼ����ֱ���
	static std::string GetDifLvlRankListName(int player_lvl);

	// ������ҵȼ�(����GetDifLvlRankListName����,��͵ȼ�����,��ȡ���ݵȼ�)
	static int CompatiblePlayerLevel(int player_lvl);

	// �ؽ����а�, @old_data �������а����ݵĴ������
    bool RecreateRankList(IPubData * pDomainData, const RecordDefine &rec_def, OldDataStrategy old_data = DISCARD_OLD_DATA);


public:
    static DomainRankList* m_pDomainRankList;

private:
    typedef std::map<std::string, RecordDefine> RecordDefineMap;
    typedef std::map<std::string, ColumnDefine> ColumnDefineMap;
    //typedef std::map<std::string, int> ColumnNameMap; // ���磺<player_uid, 6>
    typedef std::map<std::string, int> ColumnTypeMap; // ���磺<"int", 2>

    static RecordDefineMap m_RecordDefine;
    static ColumnDefineMap m_ColumnDefine;
    //static ColumnNameMap m_ColumnName;
    static ColumnTypeMap m_ColumnType;

	friend class Domain_Offline;
};

#endif