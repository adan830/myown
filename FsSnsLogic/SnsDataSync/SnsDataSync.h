//---------------------------------------------------------
//�ļ���:       SnsDataSync.cpp
//��  ��:       ������ҵ�����
//˵  ��:       
//          
//��������:      2014��12��24��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------
#ifndef FSSNS_SNS_DATA_SYNC_H_
#define FSSNS_SNS_DATA_SYNC_H_

#include "server/ISnsKernel.h"
#include "server/ISnsCallee.h"
#include "server/IRecord.h"
#include "../ISnsLogic.h"
#include <map>
#include <vector>

#define SNS_SECTION_RECORD_MAX_COL      10 // �����10��

#define SNS_DATA_RETURN_IF_NOT_IN_MEM   0  // �����ڴ���ֱ�ӷ���
#define SNS_DATA_RETURN_IF_NOT_IN_DB    1  // ����������ֱ�ӷ���
#define SNS_DATA_CREATE_IF_NOT_FOUND    2  // �Ҳ���ʱ���Դ���

class SnsDataSync : public ISnsLogic 
{
public:
    SnsDataSync();
    ~SnsDataSync();

    // �߼����󱻴���
    int OnModuleCreate(ISnsKernel* pKernel, const IVarList& args);

    // SNS���������е�SNS�����Ѿ��������׼������(�����ɴ�OnSnsLoad֮��)
    int OnReady(ISnsKernel* pKernel, const IVarList& args);

    // SNS���������ڴ��д���
    int OnCreate(ISnsKernel* pKernel, const char* uid, const IVarList& args);

    // SNS�����Ѿ����ص��ڴ���
    int OnLoad(ISnsKernel* pKernel, const char* uid, const IVarList& args);

    //�յ�����member����Ϣ
    int OnMessage(ISnsKernel* pKernel, int distribute_id, int server_id,
        int member_id, int scene_id, const char* uid, const IVarList& args);

    struct ElementDef
    {
		ElementDef() : name(""), type(0), section(""){}
        std::string name;
        int type;
        std::string section;
    };

    struct SectionDef
    {
		SectionDef() : name(""), sec_type(0), record_col_count(0), record_row_count(0)
		{
			memset(record_col, 0, sizeof(record_col));
		}
        std::string name;
        int sec_type;
        int record_col_count;
        int record_col[SNS_SECTION_RECORD_MAX_COL];
        int record_row_count;
    };

	// ��ȡElement�Ķ���
	const ElementDef * GetElementDef(const std::string &attr_name,
		const std::string &section_name);
private:
    // �����������
    bool SavePlayerData(ISnsKernel* pKernel, int distribute_id, int server_id,
        int member_id, int scene_id, const IVarList& args);

    // �����������
    bool InnerSavePlayerData(ISnsKernel* pKernel, const IVarList& args,
        int sns_flag);
    
    // ��ѯ�������    
    bool InnerQueryPlayerData(ISnsKernel* pKernel, const IVarList& args,
        int sns_flag);

	// ��ѯ�������
    bool QueryPlayerData(ISnsKernel* pKernel, int distribute_id, int server_id,
        int member_id, int scene_id, const IVarList& args);

    // ���������Ŀ
    bool CreatePlayerEntry(ISnsKernel* pKernel, int distribute_id, int server_id,
        int member_id, int scene_id, const IVarList& args);

    // ��ȡSNS���ݾ��
    ISnsData * InnerGetSnsDataHandle(ISnsKernel* pKernel, const char *uid,
        const IVarList& args, int op_flag, int sns_flag);

    // ����ε�����
    bool SaveSectionData(ISnsData *sns_data, const IVarList &args, int args_index);
    
    // ������������
    bool SaveAttrSectionData(ISnsData *sns_data, const IVarList &args, int args_index);

    // ����������
    bool SaveRecordSectionData(ISnsData *sns_data, const IVarList &args, int args_index);

    // ��ѯsection����
    bool QuerySectionData(ISnsKernel *pKernel, ISnsData *sns_data,
        const std::string &section_name, IVarList &result);

    // ��ѯ��������
    bool QueryAttrSectionData(ISnsKernel *pKernel, ISnsData *sns_data,
        const std::string &section_name, IVarList &result);

    // ��ѯ�������
    bool QueryRecordSectionData(ISnsKernel *pKernel, ISnsData *sns_data,
        const std::string &section_name, IVarList &result);

    // ���ñ��, 
    IRecord * ResetRecord(ISnsData *sns_data, const std::string &section_name);

	// ��ȡSection������, ����-1��ʾ�Ҳ���
	int GetSectionType(const std::string &type_name);

	// ��ȡsection�Ķ���
	const SectionDef *GetSectionDef(const std::string &section_name);

    //////////////////////////////////////////////////////////////////////////
    // ������صĲ���
    //////////////////////////////////////////////////////////////////////////
    // ��ȡsection�ĳ�Ա
    const std::vector<ElementDef> *GetSectionElements(const std::string &section_name);
    
    // ��ȡ������id, ����-1��ʾ�Ҳ���
    int GetVarID(const std::string &section_name);

    bool LoadResource(ISnsKernel *pKernel);
    bool LoadVarDef(ISnsKernel *pKernel);
    bool LoadSectionType(ISnsKernel *pKernel);
    bool LoadSectionDef(ISnsKernel *pKernel);
    bool LoadElementsDef(ISnsKernel *pKernel);
private:
    typedef std::map<std::string, int> VarDefMap;
    typedef std::map<std::string, int> SectionTypeMap;
    typedef std::map<std::string, SectionDef> SectionDefMap;
    typedef std::map<std::string, std::vector<ElementDef> > SectionElementsMap;

    VarDefMap m_VarDef; 
    SectionTypeMap m_SectionType;
    SectionDefMap m_SectionDef;
    SectionElementsMap m_SectionElements;

public:
	static SnsDataSync *m_pSnsDataSync;
};

#endif