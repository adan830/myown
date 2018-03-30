//---------------------------------------------------------
//�ļ���:       Domain_Look.h
//��  ��:       �鿴��Ϣ
//˵  ��:      
//��������:
//������:         
       
//---------------------------------------------------------

#include "Domain_Look.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/SystemFunctionModule/LookDefine.h"

Domain_Look* Domain_Look::m_pShowoffInstance = NULL;

Domain_Look::Domain_Look() : IDomainData(L"")
{
}

Domain_Look::~Domain_Look()
{
}

const std::wstring & Domain_Look::GetDomainName(IPubKernel * pPubKernel)
{
    if (m_domainName.empty())
    {
        wchar_t wstr[256];
        int server_id = pPubKernel->GetServerId();
		SWPRINTF_S(wstr, DOMAIN_LOOK, server_id);
        m_domainName = wstr;
    }

    return m_domainName;
}

int Domain_Look::OnCreate(IPubKernel* pPubKernel, IPubSpace * pPubSpace)
{
    m_pShowoffInstance = this;
    
    return OnLoad(pPubKernel, pPubSpace);
}

int Domain_Look::OnLoad(IPubKernel* pPubKernel, IPubSpace * pPubSpace)
{
    m_pShowoffInstance = this;

    CreateShowoffRecord(pPubKernel, pPubSpace);

    return 0;
}

/// �������Գ�������������Ϣ
int Domain_Look::OnMessage(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
                             int source_id, int scene_id, const IVarList& msg)
{
    /*
       ��Ϣ��ʽ:
           [PUBSPACE][DOMAIN_NAME]
           [msg_id][sub_msg_id]...
     */
    IPubData * pDomainData = pPubSpace->GetPubData(GetDomainName(pPubKernel).c_str());
    if (pDomainData == NULL)
    {
        return 1;
    }

    int sub_msg_id = msg.IntVal(3);
    switch (sub_msg_id)
    {
    case SHOWOFF_PUB_SAVE_ITEM:
        {
            // ��¼�´˴�չʾ����
            SaveShowoffItem(pPubKernel, pPubSpace, source_id, scene_id, msg);
        }
        break;
    }

    return 1;
}

IRecord * Domain_Look::GetShowoffRecord(IPubKernel* pPubKernel, IPubSpace* pPubSpace)
{
    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());
    if (pDomainData == NULL)
    {
        return NULL;
    }

    bool create_new = false;

    if (!pDomainData->FindRecord(SHOWOFF_ITEM_PUB_RECORD_NAME))
    {
        return NULL;
    }
    else
    {       
        return pDomainData->GetRecord(SHOWOFF_ITEM_PUB_RECORD_NAME);;
    }
}

// ������Ʒ��Ϣ������
bool Domain_Look::SaveShowoffItem(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
                                        int source_id, int scene_id, const IVarList& msg)
{
    //msg��ʽ:
    //    [PUBSPACE][DOMAIN_NAME]
    //    [msg_id][sub_msg_id]
    //    [role_uid][role_name]
    //    [item1_uid][item1_prop]...[[itemN_uid]][itemN_prop]
       
    int fixed_header = 6;
    int msg_size = (int)(msg.GetCount());

    if (msg_size < fixed_header)
    {
        return false;
    }

    // ��ȡ��Ʒ��
    IRecord * showoff_rec = GetShowoffRecord(pPubKernel, pPubSpace);
    if (showoff_rec == NULL)
    {
        return false;
    }

    const char *role_uid = msg.StringVal(4);
    const wchar_t *role_name = msg.WideStrVal(5);

    int item_count = (msg_size - fixed_header) / 2;

    int item_index = 5; // ��5���Ժ�����Ʒ��Ϣ
    
    LoopBeginCheck(a)
    for (int i=0; i<item_count; ++i)
    {
        LoopDoCheck(a)
        
        const char* item_uid = msg.StringVal(++item_index);
        const char* item_prop = msg.StringVal(++item_index);
        
        // ����Ƿ��Ѿ��д���Ʒ����Ϣ
        int row_no = showoff_rec->FindString(SHOWOFF_ITEM_PUB_RECORD_UID_COL, item_uid);
        if (row_no == -1)
        {  
            // ����Ʒ������, �������
            
            // ����ǰ�ȿ�����������Ƿ񵽴�����
            if (showoff_rec->GetRows() >= showoff_rec->GetRowMax())
            {
                // ���Ｋ�ޣ�ɾ����һ��
                showoff_rec->RemoveRow(0);
            }
            
            CVarList new_row;
            new_row << role_uid << role_name << item_uid << item_prop;
            showoff_rec->AddRowValue(-1, new_row);
        }
        else
        {
            // ��Ʒ����, ��������
            showoff_rec->SetString(row_no, SHOWOFF_ITEM_PUB_RECORD_PROP_COL, item_prop);
        }
    }
    
    return true;
}

// �������չʾ����Ʒ��
bool Domain_Look::CreateShowoffRecord(IPubKernel* pPubKernel, IPubSpace* pPubSpace)
{
    const std::wstring &domain_name = GetDomainName(pPubKernel);
    IPubData * pDomainData = pPubSpace->GetPubData(domain_name.c_str());
    if (pDomainData == NULL)
    {
        return false;
    }
    
    bool create_new = false;
    
    if (!pDomainData->FindRecord(SHOWOFF_ITEM_PUB_RECORD_NAME))
    {
        // ���а񲻴���
        create_new = true;
    }
    else
    {
        IRecord *rec = pDomainData->GetRecord(SHOWOFF_ITEM_PUB_RECORD_NAME);
        if (rec != NULL)
        {
            // �鿴�������������Ƿ���ͬ
            if (SHOWOFF_ITEM_PUB_RECORD_MAX_ROW != rec->GetRowMax())
            {
                pDomainData->RemoveRecord(SHOWOFF_ITEM_PUB_RECORD_NAME);
                create_new = true;
            }

            // �鿴�����Ƿ���ͬ
            if (!create_new)
            {
                if (SHOWOFF_ITEM_PUB_RECORD_COL_COUNT != rec->GetCols())
                {
                    pDomainData->RemoveRecord(SHOWOFF_ITEM_PUB_RECORD_NAME);
                    create_new = true;
                }
            }

        }// record != NULL

    } // End of ����

    if (create_new)
    {        
        CVarList columns;
        columns << VTYPE_STRING << VTYPE_WIDESTR;
        columns << VTYPE_STRING << VTYPE_STRING;

        bool created = pDomainData->AddRecord(SHOWOFF_ITEM_PUB_RECORD_NAME,
                                              SHOWOFF_ITEM_PUB_RECORD_MAX_ROW,
                                              SHOWOFF_ITEM_PUB_RECORD_COL_COUNT,
                                              columns);

        if (created)
        {
            // ����������(����)
            pDomainData->SetRecordKey(SHOWOFF_ITEM_PUB_RECORD_NAME,
                                      SHOWOFF_ITEM_PUB_RECORD_UID_COL);
            pDomainData->SetRecordSave(SHOWOFF_ITEM_PUB_RECORD_NAME, true);
        }
    }
    
    
    return true;
}
