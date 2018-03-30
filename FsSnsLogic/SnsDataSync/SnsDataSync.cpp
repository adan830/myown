//---------------------------------------------------------
//�ļ���:       SnsDataModule.cpp
//��  ��:       ������ҵ�����
//˵  ��:       
//          
//��������:      2014��12��24��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------
#include "SnsDataSync.h"

#include "../../utils/XmlFile.h"

#include "server/ISnsCallee.h"
#include "server/SnsLogicDll.h"
#include "server/ISnsData.h"
#include "FsGame/Define/SnsDataDefine.h"

#include "public/VarList.h"
#include "public/Inlines.h"
#include "public/Converts.h"

#include "../../utils/util_func.h"
#include "../../utils/XmlFile.h"

#include "../../FsGame/Define/SnsDefine.h"
#include "../../FsGame/Define/SnsDataDefine.h"
#include "../../FsGame/Define/CommandDefine.h"

#define SECTION_TYPE_ATTRIBUTE  1 // �����Ա����section
#define SECTION_TYPE_RECORD     2 // �Ա����section

#define CONFIG_FILE_VAR_DEF        "ini/SystemFunction/SNS/var_def.xml"
#define CONFIG_FILE_SECTION_TYPE   "ini/SystemFunction/SNS/section_type.xml"
#define CONFIG_FILE_SECTION_DEF    "ini/SystemFunction/SNS/section_def.xml"
#define CONFIG_FILE_ELEMENT_DEF    "ini/SystemFunction/SNS/element_def.xml"

SnsDataSync* SnsDataSync::m_pSnsDataSync = NULL;

SnsDataSync::SnsDataSync(): ISnsLogic(L"SnsDataSync", 0)
{
   // TODO 
}

SnsDataSync::~SnsDataSync()
{
    // TODO 
}

// �߼����󱻴���
int SnsDataSync::OnModuleCreate(ISnsKernel* pKernel, const IVarList& args)
{	
    return 1;
}

// SNS���������Ե�SNS�����Ѿ��������׼������(������OnSnsLoad֮��)
int SnsDataSync::OnReady(ISnsKernel* pKernel, const IVarList& args)
{
    LoadResource(pKernel);

	m_pSnsDataSync = this;
    return 1;
}

// SNS�����Ѵ������������ڴ���
int SnsDataSync::OnCreate(ISnsKernel* pKernel, const char * uid, const IVarList& args)
{
    if (args.GetCount() == 0)
    {
        return 1;
    }
    
    int load_flag = args.IntVal(0);
    if (load_flag == CREATE_SNS_DATA_DEFAULT)
    {
        CVarList real_args;
        real_args.Append(args, 1, args.GetCount() - 1);
        InnerSavePlayerData(pKernel, real_args, SNS_DATA_RETURN_IF_NOT_IN_MEM);
    }
    else if (load_flag == SNS_DATA_QUERY_PLAYER)
    {
        CVarList real_args;
        real_args.Append(args, 1, args.GetCount() - 1);
        InnerQueryPlayerData(pKernel, real_args, SNS_DATA_RETURN_IF_NOT_IN_MEM);
    }

    return 1;
}

// SNS �����Ѽ��ؽ��ڴ�
int SnsDataSync::OnLoad(ISnsKernel* pKernel, const char* uid, const IVarList& args)
{
    if (args.GetCount() == 0)
    {
        return 1;
    }

    int load_flag = args.IntVal(0);
    if (load_flag == CREATE_SNS_DATA_DEFAULT)
    {
        CVarList real_args;
        real_args.Append(args, 1, args.GetCount() - 1);
        InnerSavePlayerData(pKernel, real_args, SNS_DATA_RETURN_IF_NOT_IN_MEM);
    }
    else if (load_flag == SNS_DATA_QUERY_PLAYER)
    {
        CVarList real_args;
        real_args.Append(args, 1, args.GetCount() - 1);
        InnerQueryPlayerData(pKernel, real_args, SNS_DATA_RETURN_IF_NOT_IN_MEM);
    }

    return 1;
}

//�յ�����member����Ϣ
int SnsDataSync::OnMessage(ISnsKernel* pKernel, int distribute_id,int server_id, 
                             int member_id,int scene_id, const char* uid, const IVarList& args)
{
    // ��Ϣ��ʽ
    //
    //    ["sns"]["SnsDataSync"][msg_id][sub_msg_id]...
    //
    // ��Ӧ��Ϣ 
    //
    //    ����л�Ӧ��Ϣ, ��ͨ�� ISnsKernel::SendToScenePlayer() �������

    if (pKernel == NULL)
    {
        return 1;
    }

    if (args.GetCount() == 0)
    {
        return 0;
    }

    int args_index = 3; // �ֶ� [msg_id] ������
    int submsg_id = args.IntVal(args_index);

    switch (submsg_id)
    {
    case SNS_DATA_SAVE_PLAYER_DATA:
        {
            // �����������, ����Ҳ�����ʱ�ᴴ��
            SavePlayerData(pKernel, distribute_id, server_id, member_id, scene_id, args);
        }
        break;
    case SNS_DATA_QUERY_PLAYER_DATA:
        {
            // ��ѯ�������
            QueryPlayerData(pKernel, distribute_id, server_id, member_id, scene_id, args);
        }
        break;
    case SNS_DATA_CREATE_PLAYER_ENTRY:
        {
            // �����µ������Ŀ
            CreatePlayerEntry(pKernel, distribute_id, server_id, member_id, scene_id, args);
        }
        break;

    default:
        break;
    }
    return 1;
}


///////////////////////////////////////////////////////////////////////////
// ������
///////////////////////////////////////////////////////////////////////////

// �����������
bool SnsDataSync::SavePlayerData(ISnsKernel* pKernel, int distribute_id, int server_id,
                                int member_id, int scene_id, const IVarList& args)
{
    return InnerSavePlayerData(pKernel, args, SNS_DATA_CREATE_IF_NOT_FOUND);
}

// �����������
bool SnsDataSync::InnerSavePlayerData(ISnsKernel* pKernel, const IVarList& args, int sns_flag)
{
    // ��Ϣ��ʽ
    //
    //    ["sns"]["SnsDataSync"][msg_id][sub_msg_id]
    //    [uid]
    //    [section_count]
    //    ["PlayerAttribute"][row][col][...]
    //    ["PlayerEquipment"][row][col][...]
    //
    // ��Ӧ��Ϣ 
    //
    //    N/A

    int args_index = 4; // ��uid��ʼ��ȡ
    const char *uid = args.StringVal(args_index);
    int section_count = args.IntVal(++args_index);

    ISnsData *sns_data = InnerGetSnsDataHandle(pKernel, uid, args, CREATE_SNS_DATA_DEFAULT, sns_flag);
    if (sns_data == NULL)
    {
        // ����NULL ��ʾSNS���ݲ����ڻ����ڼ���, ��SNS���ݼ��سɹ���
        // ������Ӧ�Ļص�����(OnLoad(), OnCreate())��ɴ˲���
        return false;
    }

    LoopBeginCheck(a)
    for (int i=0; i<section_count; ++i)
    {
        LoopDoCheck(a)
        
        std::string section_name = args.StringVal(++args_index);

        int section_index = args_index;

        int row_count = args.IntVal(++args_index);
        int col_count = args.IntVal(++args_index);

        SaveSectionData(sns_data, args, section_index);

        args_index += (row_count * col_count);
    }
    
    // �������浽���ݿ�
    pKernel->SaveSns(uid);

    return false;
}

// ��ȡ�������
bool SnsDataSync::QueryPlayerData(ISnsKernel* pKernel, int distribute_id, int server_id,
                                 int member_id, int scene_id, const IVarList& args)
{
    // ���Ҫ��ѯ����������ݿ��о����룬��֮�ͷ���
    
    CVarList new_args;
    new_args << distribute_id << server_id << member_id << scene_id;
    new_args << args;
    InnerQueryPlayerData(pKernel, new_args, SNS_DATA_RETURN_IF_NOT_IN_DB);

    return true;
}

// ��ѯ�������    
bool SnsDataSync::InnerQueryPlayerData(ISnsKernel* pKernel, const IVarList& args, int sns_flag)
{

    // ��Ϣ��ʽ
    //    [distribute_id][server_id][member_id][scene_id]
    //    ["sns"]["SnsDataSync"][msg_id][sub_msg_id][logic_id]
    //    [requester_uid]
    //    [target_uid]
    //    [section_count]
    //    ["PlayerAttribute"]["PlayerEquipment"]
    //
    // ��Ӧ��Ϣ 
    //
    //    [msg_id][sub_msg_id][logic_id][request_uid][target_uid]
    //    [section_count]
    //    ["PlayerAttribute"][row][col][...]
    //    ["PlayerEquipment"][row][col][...]

    int args_index = 0;
    ++args_index; // distribute_id
    int server_id = args.IntVal(args_index);
    int member_id = args.IntVal(++args_index);
    int scene_id = args.IntVal(++args_index);
    
    ++args_index; // "sns"
    ++args_index; // "SnsDataSync"
    
    int msg_id = args.IntVal(++args_index);
    int sum_msg_id = args.IntVal(++args_index);
    int logic_id = args.IntVal(++args_index);
    const char *requester_uid = args.StringVal(++args_index);
    const char *target_uid = args.StringVal(++args_index);
    int section_count = args.IntVal(++args_index);

    ISnsData *sns_data = InnerGetSnsDataHandle(pKernel, target_uid, args, SNS_DATA_QUERY_PLAYER, sns_flag);
    if (sns_data == NULL)
    {
        return false;
    }

    // ��Ӧ��Ϣ
    CVarList query_result;
    query_result << msg_id;
    query_result << sum_msg_id;
    query_result << logic_id;
    query_result << requester_uid;
    query_result << target_uid;
    query_result << section_count;

    LoopBeginCheck(a)
    for (int i=0; i<section_count; ++i)
    {
        LoopDoCheck(a)

        std::string section_name = args.StringVal(++args_index);
        QuerySectionData(pKernel, sns_data, section_name, query_result);
    }

    int ret = pKernel->SendToScenePlayer(server_id, member_id, scene_id, requester_uid, query_result);

    return true;
}

// ���������Ŀ
bool SnsDataSync::CreatePlayerEntry(ISnsKernel* pKernel, int distribute_id, int server_id,
                                    int member_id, int scene_id, const IVarList& args)
{
    // args��ʽ
    //
    //    ["sns"]["SnsDataSync"][msg_id][sub_msg_id][uid]
    //
    
    const char *uid = args.StringVal(4);
    
    // �鿴�����Ƿ���Լ���@uid������
    if (!pKernel->CanLoadSns(uid))
    {
        return false;
    }

    // �鿴�����Ƿ��Ѿ����ڴ���
    if (!pKernel->FindSns(uid))
    {
        // ���ݲ����ڴ���, ����Ƿ������ݿ���
        if (!pKernel->IsExistSns(uid))
        {
            // ���ݿ���Ҳû��, ����Ҫ����
            // �˲������첽��, ��������ͨ�� OnCreate();

            CVarList create_args;
            create_args << -1;
            pKernel->CreateSns(uid, create_args);
        }
        else
        {
            // ���ݴ�����������, ���Ű��������ڴ���
            // �˲������첽��, ��������ͨ�� OnLoad();
            CVarList load_args;
            load_args << -1;
            pKernel->LoadSns(uid, load_args);            
        }
    }
    
    return true;
}

// ��ȡSNS����
ISnsData * SnsDataSync::InnerGetSnsDataHandle(ISnsKernel* pKernel, const char *uid,
                                        const IVarList& args, int op_flag, int sns_flag)
{
    // �鿴�����Ƿ���Լ���@uid������
    if (!pKernel->CanLoadSns(uid))
    {
        return NULL;
    }

    // �鿴�����Ƿ��Ѿ����ڴ���
    if (!pKernel->FindSns(uid))
    {
        // ���ݲ����ڴ���
        
        if (sns_flag == SNS_DATA_RETURN_IF_NOT_IN_MEM)
        {
            return NULL;
        }
        
        // ����Ƿ������ݿ���
        if (!pKernel->IsExistSns(uid))
        {
            // ���ݿ���Ҳû��
            
            if (sns_flag == SNS_DATA_RETURN_IF_NOT_IN_DB)
            {
                return NULL;
            }
            
            // ��Ҫ����
            // �˲������첽��, ��������ͨ�� OnCreate();
            if (sns_flag == SNS_DATA_CREATE_IF_NOT_FOUND)
            {
                CVarList create_args;
                create_args << op_flag << args;
                pKernel->CreateSns(uid, create_args);
            }          
        }
        else
        {
            // ���ݴ�����������, ���Ű��������ڴ���
            // �˲������첽��, ��������ͨ�� OnLoad();
            CVarList load_args;
            load_args << op_flag << args;
            pKernel->LoadSns(uid, load_args);
        }

        return NULL;
    }
    else
    {
        return pKernel->GetSnsData(uid);
    }   
}

// ����ÿһ�ε�����
bool SnsDataSync::SaveSectionData(ISnsData *sns_data, const IVarList &args,
                                  int args_index)
{
    bool ret = false;
    std::string section_name = args.StringVal(args_index);
    const SectionDef *sec_def = GetSectionDef(section_name);
    if (sec_def == NULL)
    {
        return false;
    }

    switch (sec_def->sec_type)
    {
    case SECTION_TYPE_ATTRIBUTE:
        ret = SaveAttrSectionData(sns_data, args, args_index);
        break;
    case SECTION_TYPE_RECORD:
        ret = SaveRecordSectionData(sns_data, args, args_index);
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}

// ������������
bool SnsDataSync::SaveAttrSectionData(ISnsData *sns_data, const IVarList &args,
                                      int args_index)
{
    // args ��ʽ�� [SectionName][row_count][col_count] [name][value] [name][value]...

    int read_index = args_index;
    std::string section_name = args.StringVal(read_index);
    int row_count = args.IntVal(++read_index);
    int col_count = args.IntVal(++read_index);

    // ����, �洢��ʽ: [name][value]...
    
    LoopBeginCheck(a)
    for (int i=0; i<row_count; ++i)
    {
        LoopDoCheck(a)
        
        std::string attr_name = args.StringVal(++read_index);

        const ElementDef * element_def = GetElementDef(attr_name, section_name);
        if (element_def == NULL)
        {
            continue;
        }

        if (!sns_data->FindAttr(attr_name.c_str()))
        {
            bool add_ret = sns_data->AddAttr(element_def->name.c_str(), element_def->type, true);
            if (!add_ret)
            {
                continue;
            }
        }
		else
		{
			int saved_type = sns_data->GetAttrType(attr_name.c_str());
			if (saved_type != element_def->type)
			{
				sns_data->RemoveAttr(attr_name.c_str());
				bool add_ret = sns_data->AddAttr(element_def->name.c_str(), element_def->type, true);
				if (!add_ret)
				{
					continue;
				}
			}
		}

        switch (element_def->type)
        {
        case VTYPE_INT:
            {
                int attr_val = args.IntVal(++read_index);
                sns_data->SetAttrInt(attr_name.c_str(), attr_val);
            }
            break;
        case VTYPE_INT64:
            {
                int64_t attr_val = args.Int64Val(++read_index);
                sns_data->SetAttrInt64(attr_name.c_str(), attr_val);
            }
            break;
        case VTYPE_FLOAT:
            {
                float attr_val = args.FloatVal(++read_index);
                sns_data->SetAttrFloat(attr_name.c_str(), attr_val);
            }
            break;
        case VTYPE_DOUBLE:
            {
                double attr_val = args.DoubleVal(++read_index);
                sns_data->SetAttrDouble(attr_name.c_str(), attr_val);
            }
            break;
        case VTYPE_STRING:
            {
                const char *attr_val = args.StringVal(++read_index);
                sns_data->SetAttrString(attr_name.c_str(), attr_val);
            }
            break;
        case VTYPE_WIDESTR:
            {
                const wchar_t *attr_val = args.WideStrVal(++read_index);
                sns_data->SetAttrWideStr(attr_name.c_str(), attr_val);
            }
            break;
        default:
            break; 
        }
    }

    return true;
}

// ����������
bool SnsDataSync::SaveRecordSectionData(ISnsData *sns_data, const IVarList &args,
                                        int args_index)
{
    // args ��ʽ�� 
    //      [SectionName][row_count][col_count]
    //      [col1][col2][col3]...
    //      [col1][col2][col3]...

    int read_index = args_index;
    std::string section_name = args.StringVal(read_index);
    int row_count = args.IntVal(++read_index);
    int col_count = args.IntVal(++read_index);

    const SectionDef *sec_def = GetSectionDef(section_name);
    if (sec_def == NULL)
    {
        return false;
    }

    // ���ñ��
    IRecord * record = ResetRecord(sns_data, section_name);
    if (record == NULL)
    {
        return false;
    }

    // д��������
    LoopBeginCheck(a)
    for (int i=0; i<row_count; ++i)
    {
        LoopDoCheck(a)
        
        CVarList row_data;
        LoopBeginCheck(b)
        for (int col=0; col<sec_def->record_col_count; ++col)
        {
            LoopDoCheck(b)
            
            switch (sec_def->record_col[col])
            {
            case VTYPE_INT:
                {
                    int attr_val = args.IntVal(++read_index);
                    row_data << attr_val;
                }
                break;
            case VTYPE_INT64:
                {
                    int64_t attr_val = args.Int64Val(++read_index);
                    row_data << attr_val;
                }
                break;
            case VTYPE_FLOAT:
                {
                    float attr_val = args.FloatVal(++read_index);
                    row_data << attr_val;
                }
                break;
            case VTYPE_DOUBLE:
                {
                    double attr_val = args.DoubleVal(++read_index);
                    row_data << attr_val;
                }
                break;
            case VTYPE_STRING:
                {
                    const char *attr_val = args.StringVal(++read_index);
                    row_data << attr_val;
                }
                break;
            case VTYPE_WIDESTR:
                {
                    const wchar_t *attr_val = args.WideStrVal(++read_index);
                    row_data << attr_val;
                }
                break;
            default:
                break; 
            }
        }

        record->AddRowValue(-1, row_data);
    }
    return true;
}

// ��ѯsection����
bool SnsDataSync::QuerySectionData(ISnsKernel *pKernel, ISnsData *sns_data,
                                   const std::string &section_name, IVarList &result)
{
    bool ret = false;

    const SectionDef *sec_def = GetSectionDef(section_name);
    if (sec_def == NULL)
    {
        result << section_name.c_str();
        result << 0 << 0; // ��������

        return true;
    }

    switch (sec_def->sec_type)
    {
    case SECTION_TYPE_ATTRIBUTE:
        {
            ret = QueryAttrSectionData(pKernel, sns_data, section_name, result);
        }
        break;
    case SECTION_TYPE_RECORD:
        {
            ret = QueryRecordSectionData(pKernel, sns_data, section_name, result);
        }
        break;
    default:
        {
            result << section_name.c_str();
            result << 0 << 0; // ��������
            ret = false;
        }
        break;
    }

    return true;
}

// ��ѯ��������
bool SnsDataSync::QueryAttrSectionData(ISnsKernel *pKernel, ISnsData *sns_data,
                                       const std::string &section_name, IVarList &result)
{
    // temp_result ��ʽ�� [name][value] [name][value]...

    CVarList temp_result;

    const std::vector<ElementDef> *elements_list = GetSectionElements(section_name);
    if (elements_list == NULL)
    {
        result << section_name.c_str();
        result << 0 << 0; // 0��0��
        result << temp_result;

        return false;
    }

    std::vector<ElementDef>::const_iterator it = elements_list->begin();
    std::vector<ElementDef>::const_iterator it_end = elements_list->end();

    LoopBeginCheck(a)
    for (; it!=it_end; ++it)
    {
        LoopDoCheck(a)
        
        const ElementDef &element_def = *it;

        if (!sns_data->FindAttr(element_def.name.c_str()))
        {
            continue;
        }

        switch (element_def.type)
        {
        case VTYPE_INT:
            {
                int attr_val = sns_data->QueryAttrInt(element_def.name.c_str());
                temp_result << element_def.name.c_str();
                temp_result << attr_val;
            }
            break;
        case VTYPE_INT64:
            {
                int64_t attr_val = sns_data->QueryAttrInt64(element_def.name.c_str());
                temp_result << element_def.name.c_str();
                temp_result << attr_val;
            }
            break;
        case VTYPE_FLOAT:
            {
                float attr_val = sns_data->QueryAttrFloat(element_def.name.c_str());
                temp_result << element_def.name.c_str();
                temp_result << attr_val;
            }
            break;
        case VTYPE_DOUBLE:
            {
                double attr_val = sns_data->QueryAttrDouble(element_def.name.c_str());
                temp_result << element_def.name.c_str();
                temp_result << attr_val;
            }
            break;
        case VTYPE_STRING:
            {
                const char *attr_val = sns_data->QueryAttrString(element_def.name.c_str());
                temp_result << element_def.name.c_str();
                temp_result << attr_val;
            }
            break;
        case VTYPE_WIDESTR:
            {
                const wchar_t *attr_val = sns_data->QueryAttrWideStr(element_def.name.c_str());
                temp_result << element_def.name.c_str();
                temp_result << attr_val;
            }
            break;
        default:
            break; 
        }
    }//for loop

    // �����
    int col_count = 2; // name, value ����
    int row_count = ((int)temp_result.GetCount())/col_count;

    result << section_name.c_str();
    result << row_count << col_count;
    result << temp_result;

    return true;
}

// ��ѯ�������
bool SnsDataSync::QueryRecordSectionData(ISnsKernel *pKernel, ISnsData *sns_data,
                                         const std::string &section_name, IVarList &result)
{
    // temp_result ��ʽ�� [col1][col2][col3] [col1][col2][col3]...

    CVarList temp_result;

    const SectionDef *sec_def = GetSectionDef(section_name);
    if (sec_def == NULL)
    {
        result << section_name.c_str();
        result << 0 << 0; // 0��0��
        result << temp_result; 

        return false;
    }

    // ���ұ��
    if (!sns_data->FindRecord(section_name.c_str()))
    {
        result << section_name.c_str();
        result << 0 << 0;// 0��0��
        result << temp_result; 

        return false;
    }

    IRecord * record = sns_data->GetRecord(section_name.c_str());
    if (record == NULL)
    {
        result << section_name.c_str();
        result << 0 << 0;// 0��0��
        result << temp_result; 

        return false;
    }


    int row_in_record = record->GetRows();
    
    LoopBeginCheck(a)
    for (int i=0; i<row_in_record; ++i)
    {
        LoopDoCheck(a)
        
        CVarList row_data;
        record->QueryRowValue(i, row_data);

        temp_result << row_data;
    }

    // �����
    int row_count = row_in_record;
    int col_count = record->GetCols();

    result << section_name.c_str();
    result << row_count << col_count;
    result << temp_result;

    return true;
}

// ���ñ��, 
IRecord * SnsDataSync::ResetRecord(ISnsData *sns_data, const std::string &section_name)
{
    const SectionDef *sec_def = GetSectionDef(section_name);

    if (sec_def == NULL)
    {
        return NULL;
    }

    bool create_new = false;

    if (!sns_data->FindRecord(sec_def->name.c_str()))
    {
        // ��񲻴���, ��Ҫ����
        create_new = true;
    }
    else
    {
        // �Ѿ�����, ��鶨���Ƿ���ͬ
        IRecord * record = sns_data->GetRecord(sec_def->name.c_str());
        if (record == NULL)
        {
            return NULL;
        }

        // �������, �����Ƿ���ͬ
        if (record->GetRowMax() != sec_def->record_row_count
            || record->GetCols() != sec_def->record_col_count)
        {
            sns_data->RemoveRecord(sec_def->name.c_str());
            create_new = true;
        }
        else
        {
            // ����ж����Ƿ���ͬ
            
            LoopBeginCheck(a)
            for (int i=0;i<sec_def->record_col_count; ++i)
            {
                LoopDoCheck(a)
                
                if (record->GetColType(i) != sec_def->record_col[i])
                {
                    create_new = true;
                    sns_data->RemoveRecord(sec_def->name.c_str());
                    break;
                }
            }

            if (!create_new)
            {
                // �����������
                record->ClearRow();
            }
        }
    }

    if (create_new)
    {
        CVarList col_def;

        LoopBeginCheck(a)
        for (int i=0; i<sec_def->record_col_count; ++i)
        {
            LoopDoCheck(a)
            
            col_def << sec_def->record_col[i];
        }

        sns_data->AddRecord(sec_def->name.c_str(), sec_def->record_row_count,
            sec_def->record_col_count, col_def);
    }

    return sns_data->GetRecord(sec_def->name.c_str());
}

//////////////////////////////////////////////////////////////////////////
// ������صĲ���
//////////////////////////////////////////////////////////////////////////

// ��ȡsection�Ķ���
const SnsDataSync::SectionDef *SnsDataSync::GetSectionDef(const std::string &section_name)
{
    SectionDefMap::const_iterator it = m_SectionDef.find(section_name);
    if (it == m_SectionDef.end())
    {
        return NULL;
    }
    else
    {
        return &(it->second);
    }
}

// ��ȡsection�ĳ�Ա
const std::vector<SnsDataSync::ElementDef> *SnsDataSync::GetSectionElements(
    const std::string &section_name)
{
    SectionElementsMap::const_iterator it = m_SectionElements.find(section_name);
    if (it == m_SectionElements.end())
    {
        return NULL;
    }
    else
    {
        return &(it->second);
    }
}

const SnsDataSync::ElementDef * SnsDataSync::GetElementDef(const std::string &attr_name,
                                 const std::string &section_name)
{
    SectionElementsMap::const_iterator it = m_SectionElements.find(section_name);
    if (it == m_SectionElements.end())
    {
        return NULL;
    }
    else
    {
        const std::vector<ElementDef> &element_list = it->second;
        std::vector<ElementDef>::const_iterator it2 = element_list.begin();
        std::vector<ElementDef>::const_iterator it_end = element_list.end();
        
        LoopBeginCheck(a)
        for (; it2!=it_end; ++it2)
        {
            LoopDoCheck(a)
            
            const ElementDef &element = *it2;
            if (element.name == attr_name)
            {
                return &element;
            }
        }

        return NULL;
    }
}


// ��ȡSection������
int SnsDataSync::GetSectionType(const std::string &type_name)
{
    SectionTypeMap::const_iterator it = m_SectionType.find(type_name);
    if (it == m_SectionType.end())
    {
        return -1;
    }
    else
    {
       return it->second;
    }
}

// ��ȡ������id, ����-1��ʾ�Ҳ���
int SnsDataSync::GetVarID(const std::string &section_name)
{
    VarDefMap::const_iterator it = m_VarDef.find(section_name);
    if (it == m_VarDef.end())
    {
        return -1;
    }
    else
    {
        return it->second;
    }
}

bool SnsDataSync::LoadResource(ISnsKernel *pKernel)
{
    if (!LoadVarDef(pKernel))
    {
        return false;
    }

    if (!LoadSectionType(pKernel))
    {
        return false;
    }

    if (!LoadSectionDef(pKernel))
    {
        return false;
    }

    if (!LoadElementsDef(pKernel))
    {
        return false;
    }

    return true;
}

bool SnsDataSync::LoadVarDef(ISnsKernel *pKernel)
{
    //
    //    <Property ID="2" TypeName="int"/>
    //

    std::string res_path = pKernel->GetResourcePath();

    std::string config_file = res_path;
    config_file.append(CONFIG_FILE_VAR_DEF);

    CXmlFile xml(config_file.c_str());
    if (!xml.LoadFromFile())
    {
        ::extend_warning(LOG_ERROR, "%s does not exists.", config_file.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a)
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a)
        
        const char *xml_section = sec_list.StringVal(i);
        std::string type_id = xml_section;
        std::string type_name = xml.ReadString(xml_section, "TypeName", "");

        if (type_id.length() == 0 || type_name.length() == 0)
        {                
            ::extend_warning(LOG_ERROR, "%s: invalid var define.", config_file.c_str());

            continue;
        }

        int int_id = atoi(type_id.c_str());

        m_VarDef.insert(VarDefMap::value_type(type_name, int_id));
    }

    return true;
}


bool SnsDataSync::LoadSectionType(ISnsKernel *pKernel)
{

    //
    //    <Property ID="1" TypeName="attribute"/>
    //

    std::string res_path = pKernel->GetResourcePath();

    std::string config_file = res_path;
    config_file.append(CONFIG_FILE_SECTION_TYPE);

    CXmlFile xml(config_file.c_str());
    if (!xml.LoadFromFile())
    {
        ::extend_warning(LOG_ERROR, "%s does not exists.", config_file.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a)
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a)
        
        const char *xml_section = sec_list.StringVal(i);
        std::string type_id = xml_section;
        std::string type_name = xml.ReadString(xml_section, "TypeName", "");

        if (type_id.length() == 0 || type_name.length() == 0)
        {                
            ::extend_warning(LOG_ERROR, "%s: invalid section type define.", config_file.c_str());

            continue;
        }

        int int_id = atoi(type_id.c_str());

        m_SectionType.insert(SectionTypeMap::value_type(type_name, int_id));
    }

    return true;
}

bool SnsDataSync::LoadSectionDef(ISnsKernel *pKernel)
{
    //
    //    <Property ID="PlayerEquipment" SectionType="record" ExtParam01="str,str,int,int" ExtParam02="100"/>
    //

    std::string res_path = pKernel->GetResourcePath();

    std::string config_file = res_path;
    config_file.append(CONFIG_FILE_SECTION_DEF);

    CXmlFile xml(config_file.c_str());
    if (!xml.LoadFromFile())
    {
        ::extend_warning(LOG_ERROR, "%s does not exists.", config_file.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a)
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a)
        
        const char *xml_section = sec_list.StringVal(i);
        std::string sec_name = xml_section;
        std::string sec_type = xml.ReadString(xml_section, "SectionType", "");
        std::string ext_param01 = xml.ReadString(xml_section, "ExtParam01", "");
        std::string ext_param02 = xml.ReadString(xml_section, "ExtParam02", "");

        int type_id = GetSectionType(sec_type);

        if (type_id ==-1 || sec_type.length() == 0)
        {
            ::extend_warning(LOG_ERROR, "%s: invalid section define.", config_file.c_str());
            continue;
        }

        SectionDef sec_def;
        sec_def.name = sec_name;
        sec_def.sec_type = type_id;

        // "record" ���͵�section, ExtParam01 ��ű����ж���, ExtParam02 ����������
        if (sec_def.sec_type == SECTION_TYPE_RECORD)
        {
            std::string delimiter = ",";
            CVarList cols;
            util_split_string(cols, ext_param01, delimiter);

            int col_count = (int) cols.GetCount();
            if (col_count > SNS_SECTION_RECORD_MAX_COL)
            {
                ::extend_warning(LOG_ERROR, "%s: invalid section define.", config_file.c_str());
                continue;
            }

            sec_def.record_col_count = col_count;
            sec_def.record_row_count = atoi(ext_param02.c_str());
            
            LoopBeginCheck(b)
            for (int j=0; j<col_count; ++j)
            {
                LoopDoCheck(b)
                
                std::string type_name = cols.StringVal(j);
                int type_value = GetVarID(type_name);
                if (type_value == -1)
                {
                    ::extend_warning(LOG_ERROR, "%s: invalid section define.", config_file.c_str());
                    continue;
                }

                sec_def.record_col[j] = type_value;
            }
        }

        m_SectionDef.insert(SectionDefMap::value_type(sec_def.name, sec_def));
    }

    return true;
}

bool SnsDataSync::LoadElementsDef(ISnsKernel *pKernel)
{
    //
    //    <Property ID="Name" Type="wstr" Section="PlayerAttribute"/>
    //

    std::string res_path = pKernel->GetResourcePath();

    std::string config_file = res_path;
    config_file.append(CONFIG_FILE_ELEMENT_DEF);

    CXmlFile xml(config_file.c_str());
    if (!xml.LoadFromFile())
    {
        ::extend_warning(LOG_ERROR, "%s does not exists.", config_file.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a)
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a)
         
        const char *xml_section = sec_list.StringVal(i);
        std::string elem_name = xml_section;
        std::string elem_type = xml.ReadString(xml_section, "Type", "");
        std::string elem_section = xml.ReadString(xml_section, "Section", "");

        if (elem_type.length() == 0 || elem_section.length() == 0)
        {                
            ::extend_warning(LOG_ERROR, "%s: invalid element define.", config_file.c_str());

            continue;
        }

        int type_id = GetVarID(elem_type);
        if (type_id == -1)
        {
            ::extend_warning(LOG_ERROR, "%s: invalid element define.", config_file.c_str());
            continue;
        }

        ElementDef element;
        element.name = elem_name;
        element.type = type_id;
        element.section = elem_section;

        SectionElementsMap::iterator it = m_SectionElements.find(elem_section);
        if (it == m_SectionElements.end())
        {
            std::vector<ElementDef> element_list;
            element_list.push_back(element);

            m_SectionElements.insert(SectionElementsMap::value_type(element.section, element_list));
        }
        else
        {
            std::vector<ElementDef> &element_list = it->second;
            element_list.push_back(element);
        }
    }

    return true;
}