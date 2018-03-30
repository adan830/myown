//--------------------------------------------------------------------
// �ļ���:      RankListModule
// ��  ��:      �������а�Ĺ�����
// ˵  ��:      ���а������,���а��������Ϣͨ�š����Իص����ŵ�������(��ʱ�ȿ���������а�)
// ��������:    2014-10-15
//
// ������:      liumf
//    :        
//--------------------------------------------------------------------

#ifndef FSGAME_GENERAL_RANK_LIST_MODULE_H
#define FSGAME_GENERAL_RANK_LIST_MODULE_H

#include "Fsgame/Define/header.h"

#include <map>
#include <vector>

#define RANK_LIST_MAX_COLUMN_COUNT 50 //һ�����а���������
#define REQUEST_FROM_SERVER 0 // ��Ϣ���Է�����
#define REQUEST_FROM_CLIENT 1 // ��Ϣ���Կͻ���

class RewardModule;
class RankListModule : public ILogicModule
{
public:

	virtual bool Init(IKernel * pKernel);

	virtual bool Shut(IKernel * pKernel);

    // ������Դ����
    bool LoadResource(IKernel * pKernel);
    
    // ���а�Ķ���
    bool LoadRankListDef(IKernel * pKernel, const char *file);
    bool LoadRankListColumnDef(IKernel * pKernel, const char *file);
    bool LoadTypeNameDef(IKernel * pKernel, const char *file);
	// ���а�������
	bool LoadRankListAwardDef(IKernel * pKernel, const char *file);

public:

    // ��ȡdomain������
    static const std::wstring& GetDomainName(IKernel * pKernel);

	// ������а񹫹�����
	static IPubData* GetPubData(IKernel* pKernel);

    // ����@args������ѯ, ͨ���ص����ؽ��. @requester�����ڻ�Ӧ����Ϣ��ԭ������
    // �������� -1 ��ʾ����ʧ�� 
    // ��ѯ�������COMMAND_RANK_LIST ��Ϣ���� @requester, ����� RankListModule::OnPublicMessage();
    //
    // @args �ĸ�ʽ
    //    [�������Զ���ID]
    //    [���а������][��ʼ�к�][�������ص�����]
    //    [��1��][��1ֵ][��2��][��2ֵ]...
    //
    // ��Ч�� SQL ���:
    //  SELELCT TOP [�������ص�����] FROM [���а������]
    //  WHERE 
    //   [��1��]=[��1ֵ] AND [��2��]=[��2ֵ] ....
    //
    static int Query(IKernel * pKernel, const PERSISTID &requester,
        const IVarList & args, int request_from = REQUEST_FROM_SERVER);

    // �������а��һ����¼, request�����ڻ�Ӧ����Ϣ��ԭ������
    // �������� -1 ��ʾ����ʧ��
    // [����������Ƿ����]: 0 ��ʾ�����룬 1��ʾ����
    // @args �ĸ�ʽ��
    //    
    //    [���а������][����������Ƿ����][�����е�����][�����е�ֵ]
    //    [Ҫ���µ�����][Ҫ������ֵ]...
    //
    // ���磺["level_rank_list"][0]["player_uid"]["1234"]["player_level"][3]["player_experience"][6]
    //  ��Ч�� SQL ���:
    //     UPDATE level_rank_list
    //     SET player_level=3, player_experience=6
    //     WHERE player_uid="1234"
    //
    static int UpdateEntry(IKernel * pKernel, const PERSISTID &requester,
        const IVarList & args, int request_from = REQUEST_FROM_SERVER);

    // ɾ�����а��һ����¼, request�����ڻ�Ӧ����Ϣ��ԭ������
    // �������� -1 ��ʾ����ʧ��
    // @args �ĸ�ʽ��
    //     [�������Զ���ID]
    //     [���а������][�����е�����][�����е�ֵ]
    // 
    //  ��Ч�� SQL ���:
    //     DELETE FROM [���а������]
    //     WHERE [�����е�����]=[�����е�ֵ]
    //
    static int DeleteEntry(IKernel * pKernel, const PERSISTID &requester,
        const IVarList & args, int request_from = REQUEST_FROM_SERVER);

    // ������а��ȫ����¼
    // �������� -1 ��ʾ����ʧ��, request�����ڻ�Ӧ����Ϣ��ԭ������
    // @args �ĸ�ʽ��
    //     [�������Զ���ID]
    //     [���а������]
    //  
    //  ��Ч�� SQL ���:
    //     TRUNCATE TABLE [���а������]
    static int ClearRankList(IKernel * pKernel, const PERSISTID &requester,
        const IVarList & args, int request_from = REQUEST_FROM_SERVER);

    // ��ѯ���а����ݵ�����(���ݵ�ˢ������)
    // args�ĸ�ʽ: [���а�ĸ���][���а�1]...[���а�n]
    static int QueryRankListDate(IKernel * pKernel, const PERSISTID &requester,
        const IVarList & args);

	// ������ҵȼ�ȡ�����а�ȼ����ֱ���
	static std::string GetDifLvlRankListName(int player_lvl);

    // ��ѯ @column_name �����а�@rank_list_name�е��к�.(�кŴ�0��ʼ)
    // ���� -1 ��ʾû�д��л���û�д����а�
    static int GetColumnIndexByName(const char *column_name, const char *rank_list_name);

    // Ϊ���а�׼������
    static bool PrepareDataForRankList(IKernel* pKernel, const PERSISTID& player, 
        const char *rank_list_name, IVarList &the_data);

    // ���԰���Ҳ������а�, �˽ӿھ����ܵ�����Ӧ���е��ֶ�
    static int InsertPlayerToRankList(IKernel* pKernel, const PERSISTID& player, 
        const char *rank_list_name);
        
    // ɾ�����а�����Ҽ�¼
    static int DeleteFromRankList(IKernel* pKernel, const PERSISTID& player, 
        const char *rank_list_name);

	// ɾ�����а�����Ҽ�¼
	static int DeleteFromRankList(IKernel* pKernel, const char* playerUid, 
		const char *rank_list_name);

    // �������а�����������
    // @args �ĸ�ʽ��
    //     [�������Զ���ID]
    //     [���а������][A�����е�����][A�����е�ֵ][B�����е�����][B�����е�ֵ]
    //      
    //  ���磺["level_rank_list"]["player_uid"]["1234"]["player_uid"]["456"]
    //      ��ʾ�� uid=1234 ���� �� uid=456���л���
    //
    static int SwapEntry(IKernel * pKernel, const PERSISTID &requester,
        const IVarList & args, int request_from = REQUEST_FROM_SERVER);
private:

    //������ݼ������
    static int OnPlayerRecover(IKernel *pKernel, const PERSISTID &player,
        const PERSISTID &sender, const IVarList &args);

    // ������ݱ���
    static int OnStore(IKernel * pKernel, const PERSISTID &player,
        const PERSISTID & sender, const IVarList &args);

    // �Զ���Ļص�����

	// ���Թ�������Ϣ
	static int OnPublicMessage(IKernel * pKernel, const PERSISTID & self,
	    const PERSISTID & sender, const IVarList & args);

    // ����������command��Ϣ
    static int OnSceneCommandMessage(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ������ҵ�command��Ϣ
    static int OnPlayerCommandMessage(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // �ͻ�����Ϣ
    static int OnCustomMessage(IKernel * pKernel, const PERSISTID & player,
        const PERSISTID & sender, const IVarList & args);

    // ����ָ��������ѯ���а�
    // [���а������][��ѯ����������][��ѯ������ֵ]...
    static int OnQuery(IKernel * pKernel, const PERSISTID & player,
        const PERSISTID & sender, const IVarList & args);

    // ���VIP�ȼ��ı�
    static int OnPlayerLevel(IKernel* pKernel, const PERSISTID& self,
        const char* property, const IVar& old);
        
    // ��ҵȼ��ı�
    static int OnPlayerVipLevel(IKernel* pKernel, const PERSISTID& self,
        const char* property, const IVar& old);

    // ���ս�����ı�
    static int OnPlayerFightPower(IKernel* pKernel, const PERSISTID& self,
        const char* property, const IVar& old);

	// ���������а�
// 	static int OnPlayerPVPExp(IKernel* pKernel, const PERSISTID& self,
// 		const char* property, const IVar& old);

    // ������һ�λ�ɱ
    static int OnKillObject(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // �������а�����ҵ�����״̬
    static int SetPlayerOnlineState(IKernel* pKernel, const PERSISTID& player, int on_line);

    // ���԰�������ݼ��뵽��Ҫ�����а���
    static int InsertToMultiRankList(IKernel* pKernel, const PERSISTID& player);

	// ��������ص���ɾ��ԭ���а�ȼ����ֱ���������
	static int RemovePlayerOfOldDifLvlRankList(IKernel* pKernel, const PERSISTID& player,int oldLevel = 0);


    // ������ҵ����а�����
    static int UpdateRankList(IKernel* pKernel, const PERSISTID& player);
        
private:

    // ���Թ�������������Ϣ

    // �ַ����Թ���������Ϣ
    static int DispatchPublicMessage(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

    // ���Թ�������������Ŀ��ѯ���
    static int OnPublicQueryResult(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

    // ���Թ�������������Ŀ���½��
    static int OnPublicUpdateEntryResult(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

    // ���Թ�������������Ŀɾ�����
    static int OnPublicDeleteEntyResult(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

    // ���Թ�������������ս��
    static int OnPublicClearResult(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

    // ���Թ����������������Ϣ, ���а����ݼƻ����
    static int OnPublicClearPlanned(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

    // ���Թ�������������Ϣ, ���а��ˢ������
    static int OnPublicRankListDate(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

	// ���Թ����������������Ϣ, ���а����ݼƻ����-���ͽ���
	static int OnPublicClearAward(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args);

    //////////////////////////////////////////////////////////////////////////
    // ˽�еĸ�������
    //////////////////////////////////////////////////////////////////////////

    struct RankListDef
    {
		RankListDef(): name(""), columns_count(0)
		{
		}
        std::string name;
        int columns_count;
        std::string columns[RANK_LIST_MAX_COLUMN_COUNT];
    };
    
    struct RankListColumnDef
    {
		RankListColumnDef(): name(""), prop(""), column_type(""){}
        std::string name; // ���а���ֶ���
        std::string prop; // �ֶζ�Ӧ���������, ���ַ�����ʾû�����������֮��Ӧ
        std::string column_type; // �ֶ������� (int, float....)
    };

    // ���а���
    static const RankListDef* GetRankListDef(const char *rank_list_name);
    
    static bool HasColumn(const char *column_name, const RankListDef * rank_list_def);

    
    // ��ȡ�ֶζ���
    static const RankListColumnDef* GetRankListColumnDef(const char *field_name);
    
    // ����ָ���ֶε�ֵ, ���Ѵ��ֶμ���@fields
    static bool SetColumnValue(IKernel *pKernel, const PERSISTID &player,
        const RankListColumnDef *field_def, IVarList & fields);

    // ����ָ���ֶε�ֵ, �������ֶ���
    static bool SetColumnValueWithoutName(IKernel *pKernel, const PERSISTID &player,
        const RankListColumnDef *field_def, IVarList & fields);

    // �Ѷ��ŷָ����ַ������浽����
    // ����: "a,b,c" ���浽 {"a", "b", "c"}
    static bool SaveStrToArray(const char *str_src, std::string *str_array, int array_length,
        int *actual_size);
        
    // ������������ȡ����ID
    static int GetColumnTypeByName(const std::string &column_name);

	// �������а���
	static int RewardRankList(IKernel *pKernel, const IVarList & args);

	// ���а�������
	struct RewardConfig_s
	{
		RewardConfig_s(): m_ID(0), m_Rank(""), m_Capital(""), m_Items(""){}
		int m_ID;               // ID
		std::string m_Rank;     // ��������
		std::string m_Capital;  // ���ҽ���
		std::string m_Items;    // ��Ʒ����
	};
	struct RankListRewardConfig_s
	{
		RankListRewardConfig_s(): m_RankListName("")
		{
			m_RewardConfigVec.clear();
		}
		std::string m_RankListName;                       // ���а�����
		std::vector<RewardConfig_s> m_RewardConfigVec;    // <��������>
	};

	// ���а�������
	RankListRewardConfig_s* RankListRewardConfig( const char* rankListName );

	static void ReloadConfig(IKernel *pKernel);
public:

    static RankListModule*   m_pRankListModule;

	static RewardModule*     m_pRewardModule;      // �콱ģ��

private:

    typedef std::map<std::string, RankListDef> RankListMap;
    
    typedef std::map<std::string, RankListColumnDef> ColumnMap;
    
    typedef std::map<std::string, int> ColumnTypeMap;

	typedef std::map<std::string, RankListRewardConfig_s> RankListRewardConfigMap;   // <���а�����, ��������>

	typedef RankListRewardConfigMap::iterator RankListRewardConfigMap_it;

	static std::wstring m_domainName;  // ������������Domain��������

    static RankListMap m_RankListMap;
    
    static ColumnMap m_ColumnMap;
    
    static ColumnTypeMap m_ColumnType;

	static RankListRewardConfigMap m_RankListRewardConfigMap;    // ���а�������
    
};


#endif