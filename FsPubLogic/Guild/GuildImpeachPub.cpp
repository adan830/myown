#include "FsPubLogic\Guild\GuildManager.h"
#include "FsGame\Define\GuildImpeachDefine.h"
#include "FsGame\Define\GuildDefine.h"
#include "time.h"
#include "utils\string_util.h"
#include "FsGame\Define\PubDefine.h"
#include "FsGame\Define\CommandDefine.h"

//��ʼ����
int PubGuildManager::OnStartImpeach(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
{
    if(guildName == NULL)
    {
        return 0;
    }

    IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
    if (pPubSpace == NULL)
    {
        return 0;
    }

    std::wstring guildDataName = guildName + m_ServerId;
    IPubData *pData = pPubSpace->GetPubData(guildDataName.c_str());
    if (pData == NULL)
    {
        return 0;
    }

    //������Ա������¼��
    IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
    if(impeachRec == NULL)
    {
        return 0;
    }

    //�������0�ʹ���ǰ���н����еĵ���
    if(impeachRec->GetRows() > 0)
    {
        //֪ͨ������
        pPubKernel->CommandByName(args.WideStrVal(4), CVarList() << COMMAND_GUILD_IMPEACH << PS_GUILD_IMPEACH_START_FAIL);
        return 0;
    }

    CVarList rowData;
    rowData << args.StringVal(3)
            << args.WideStrVal(4)
            << args.IntVal(5)
            << args.IntVal(6)
            << args.Int64Val(7);
    impeachRec->AddRowValue(-1, rowData);

    //�����ɹ�������Ӧ�������ٷֱ�С��
    pData->SetAttrInt(GUILD_IMPEACH_ANSWER_NUM, args.IntVal(8));
    //�ܵ����İ�������ʱ��
    pData->SetAttrInt64(GUILD_IMPEACH_OFFLINE_TIME, args.Int64Val(9));
    //ͶƱ�ռ��ʱ��
    pData->SetAttrInt64(GUILD_IMPEACH_VOTE_TIME, args.Int64Val(10));

    RecordGuildLog(guildName, GUILD_LOG_TYPE_IMPEACH_START, args.WideStrVal(4));

    StartImpeachTimer(pPubKernel, guildName);

    //֪ͨ���а���Ա
    IRecord *memberRecord = pData->GetRecord(GUILD_MEMBER_REC);
    if(memberRecord == NULL)
    {
        return 0;
    }
    int memberRecordLength = memberRecord->GetRows();
    LoopBeginCheck(a);
    for(int i = 0; i < memberRecordLength; ++i)
    {
        LoopDoCheck(a);
        
        pPubKernel->CommandByName(memberRecord->QueryWideStr(i, GUILD_MEMBER_REC_COL_NAME), CVarList() << COMMAND_GUILD_IMPEACH << PS_GUILD_IMPEACH_START_SUCCEED);
    }

    return 0;
}

//ע�ᵯ����ɼ�鶨ʱ��
void PubGuildManager::StartImpeachTimer(IPubKernel* pPubKernel, const wchar_t* guildName)
{
    IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
    if (pPubSpace == NULL)
    {
        return;
    }

    std::wstring guildDataName = guildName + m_ServerId;
    IPubData *pData = pPubSpace->GetPubData(guildDataName.c_str());
    if (pData == NULL)
    {
        return;
    }

    //������Ա������¼��
    IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
    if(impeachRec == NULL)
    {
        return;
    }
    if(impeachRec->GetRows() <= 0)
    {
        return;
    }

    if(!pPubKernel->FindTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), guildDataName.c_str()))
    {
        pPubKernel->RemoveTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), guildDataName.c_str());
    }
    pPubKernel->AddTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, OnImpeachExamine, GetName(), guildDataName.c_str(), GUILD_IMPEACH_EXAMINE_TIME, 10000);
}

//������ɼ�鶨ʱ���ص�
int PubGuildManager::OnImpeachExamine(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name, int time)
{
    IPubSpace * pPubSpace = pPubKernel->GetPubSpace(m_pInstance->GetName());
    if (pPubSpace == NULL)
    {
        return 0;
    }

    IPubData *pData = pPubSpace->GetPubData(data_name);
    if (pData == NULL)
    {
        return 0;
    }

    //������Ա������¼��
    IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
    if(impeachRec == NULL)
    {
        return 0;
    }
    if(impeachRec->GetRows() <= 0)
    {
        return 0;
    }

    //����ʣ��ʱ��
    int64_t startTime = 0;
    int initiatorRow = impeachRec->FindInt(GUILD_IMPEACH_MEMBER_INITIATOR, GUILD_IMPEACH_INITIATOR);
    if(initiatorRow != -1)
    {
        startTime = impeachRec->QueryInt64(initiatorRow, GUILD_IMPEACH_MEMBER_TIME);
    }
    //ʣ��ʱ��ת�ɺ���
    int64_t remainTime = (pData->QueryAttrInt64(GUILD_IMPEACH_VOTE_TIME) - (::time(NULL) - startTime)) * 1000;
    if(remainTime <= 0)
    {
        //�Ƴ���ʱ��
        if(pPubKernel->FindTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, m_pInstance->GetName(), data_name))
        {
            pPubKernel->RemoveTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, m_pInstance->GetName(), data_name);
        }
        //���㵯��
        m_pInstance->SettleImpeach(pPubKernel, data_name);
    }
    else if(remainTime <= GUILD_IMPEACH_EXAMINE_TIME)
    {
        //���ļ�ʱ�����ʱ��Ϊʣ��ʱ��
        if(pPubKernel->FindTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, m_pInstance->GetName(), data_name))
        {
            pPubKernel->RemoveTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, m_pInstance->GetName(), data_name);
        }
        pPubKernel->AddTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, OnImpeachExamine, m_pInstance->GetName(), data_name, int(remainTime), 2);
    }
 
    return 0;
}


//���㵯��
bool PubGuildManager::SettleImpeach(IPubKernel* pPubKernel, const wchar_t* data_name)
{
    if(data_name == NULL)
    {
        return false;
    }

    //�Ƴ���ʱ��
    if(pPubKernel->FindTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), data_name))
    {
        pPubKernel->RemoveTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), data_name);
    }

    //�����
    std::wstring guildName = data_name;
    guildName = guildName.substr(0, guildName.length() - m_ServerId.length());

    IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
    if (pPubSpace == NULL)
    {
        return false;
    }
    IPubData *pData = pPubSpace->GetPubData(data_name);
    if (pData == NULL)
    {
        return false;
    }

    //������Ա������¼��
    IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
    if(impeachRec == NULL)
    {
        return false;
    }

    //������
    int initiatorRow = impeachRec->FindInt(GUILD_IMPEACH_MEMBER_INITIATOR, GUILD_IMPEACH_INITIATOR);
    if(initiatorRow == -1)
    {
        //������,�Ҳ�����������ε���û����
        impeachRec->ClearRow();
        return false;
    }
    std::wstring impeachName = impeachRec->QueryWideStr(initiatorRow, GUILD_IMPEACH_MEMBER_NAME);

    //����Ա��
    IRecord *memberRecord = pData->GetRecord(GUILD_MEMBER_REC);
    if(memberRecord == NULL)
    {
        return false;
    }

    //ͳ����ЧƱ��
    int poll = 0;
    int impeachRecLength = impeachRec->GetRows();
    int memberRecordLength = memberRecord->GetRows();
    LoopBeginCheck(d);
    for(int i = 0; i < impeachRecLength; ++i)
    {
        LoopDoCheck(d);
        const char *uid = impeachRec->QueryString(i, GUILD_IMPEACH_MEMBER_UID);
        if(StringUtil::CharIsNull(uid))
        {
            continue;
        }
        if(memberRecord->FindString(GUILD_MEMBER_REC_COL_NAME_UID, uid) != -1)
        {
            poll += impeachRec->QueryInt(i, GUILD_IMPEACH_MEMBER_NUM);
        }
    }

    //�������������¼����
    impeachRec->ClearRow();

    //�ж��Ƿ�����Ʊ��,��1����Ϊ�ѷ��������ѵ�ͶƱ��ȥ
    int needPoll = pData->QueryAttrInt(GUILD_IMPEACH_ANSWER_NUM) ;
    if(poll < needPoll)
    {
        //��¼��������������־
        std::wstring wstrlogArgs = impeachName + L"," + StringUtil::IntAsWideStr(GUILD_IMPEACH_RESULT_VOTE_FAIL);
        RecordGuildLog(guildName.c_str(), GUILD_LOG_TYPE_IMPEACH_FAIL, impeachName.c_str());
        //֪ͨ����������
        pPubKernel->SendPublicMessage(0, 0, 
            CVarList() << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_IMPEACH_RESULT << guildName.c_str() << impeachName.c_str() << GUILD_IMPEACH_RESULT_VOTE_FAIL);
        return false;
    }
    
    //������Ա���㹻
    if(memberRecord->GetRows() < START_IMPEACH_MEMBER_MIN)
    {
        //��¼��������������־
        std::wstring wstrlogArgs = impeachName + L"," + StringUtil::IntAsWideStr(GUILD_IMPEACH_RESULT_MEMBER_FAIL);
        RecordGuildLog(guildName.c_str(), GUILD_LOG_TYPE_IMPEACH_FAIL, impeachName.c_str());
        //֪ͨ����������
        pPubKernel->SendPublicMessage(0, 0, 
            CVarList() << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_IMPEACH_RESULT << guildName.c_str() << impeachName.c_str() << GUILD_IMPEACH_RESULT_MEMBER_FAIL);
        return false;
    }

    //����������
    int row = memberRecord->FindInt(GUILD_MEMBER_REC_COL_POSITION, GUILD_POSITION_CAPTAIN);
    if(row == -1)
    {
        return false;
    }
    const wchar_t *captainName = memberRecord->QueryWideStr(row, GUILD_MEMBER_REC_COL_NAME);
    if(captainName == NULL)
    {
        return false;
    }

    //�ƽ�����
    if(TransferCaptain(guildName.c_str(), captainName, impeachName.c_str()) != GUILD_MSG_REQ_SUCC)
    {
        //��¼��������������־
        std::wstring wstrlogArgs = impeachName + L"," + StringUtil::IntAsWideStr(GUILD_IMPEACH_RESULT_FAIL);
        RecordGuildLog(guildName.c_str(), GUILD_LOG_TYPE_IMPEACH_FAIL, impeachName.c_str());
        //֪ͨ����������
        pPubKernel->SendPublicMessage(0, 0, 
            CVarList() << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_IMPEACH_RESULT << guildName.c_str() << impeachName.c_str() << GUILD_IMPEACH_RESULT_FAIL);
        return false;
    }

    //��¼��������������־
    std::wstring wstrlogArgs = impeachName + L"," + StringUtil::IntAsWideStr(GUILD_IMPEACH_RESULT_SUCCEED);
    RecordGuildLog(guildName.c_str(), GUILD_LOG_TYPE_IMPEACH_SUCCEED, impeachName.c_str());

    //����ת��֪ͨ
    SendCaptainResult(pPubKernel, 0, 0, captainName, GUILD_MSG_REQ_SUCC, impeachName.c_str(), GUILD_POSITION_MEMBER, guildName.c_str());

    //֪ͨ�������������ʼ�
    pPubKernel->SendPublicMessage(0, 0, 
        CVarList() << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_IMPEACH_RESULT << guildName.c_str() << impeachName.c_str() << GUILD_IMPEACH_RESULT_SUCCEED);
   
    return true;
}

//��������
int PubGuildManager::OnUndoImpeach(IPubKernel* pPubKernel, const wchar_t* guildName,  const char *pSelfUid)
{
    if(guildName == NULL)
    {
        return 0;
    }

    IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
    if (pPubSpace == NULL)
    {
        return 0;
    }

    std::wstring guildDataName = guildName + m_ServerId;
    IPubData *pData = pPubSpace->GetPubData(guildDataName.c_str());
    if (pData == NULL)
    {
        return 0;
    }

    //������Ա������¼��
    IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
    if(impeachRec == NULL)
    {
        return 0;
    }

    //������
    if(StringUtil::CharIsNull(pSelfUid))
    {
        return 0;
    }

    //��ǰ������
    int initiatorRow = impeachRec->FindInt(GUILD_IMPEACH_MEMBER_INITIATOR, GUILD_IMPEACH_INITIATOR);
    if(initiatorRow == -1)
    {
        //������,�Ҳ�����������ε���û����
        return 0;
    }
    const char *impeachPlayerUid = impeachRec->QueryString(initiatorRow, GUILD_IMPEACH_MEMBER_UID);

    //�ж��Ƿ��Ƿ�����
    if (strcmp(pSelfUid, impeachPlayerUid) != 0)
    {
        return 0;
    }

	//�Ƴ���ʱ��
	if (!pPubKernel->FindTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), guildName))
	{
		pPubKernel->RemoveTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), guildName);
	}

    impeachRec->ClearRow();

    return 0;
}

//��Ӧ����
int PubGuildManager::OnRespondImpeach(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
{
    if(guildName == NULL)
    {
        return 0;
    }

    const char* pSelfUid = args.StringVal(3);
    if(StringUtil::CharIsNull(pSelfUid))
    {
        return 0;
    }

    const wchar_t *pSelfName = args.WideStrVal(4);
    if(StringUtil::CharIsNull(pSelfName))
    {
        return 0;
    }

    IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
    if (pPubSpace == NULL)
    {
        return 0;
    }

    std::wstring guildDataName = guildName + m_ServerId;
    IPubData *pData = pPubSpace->GetPubData(guildDataName.c_str());
    if (pData == NULL)
    {
        return 0;
    }

    //������Ա������¼��
    IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
    if(impeachRec == NULL)
    {
        return 0;
    }

    if(impeachRec->GetRows() < 1)
    {
        return 0;
    }

    int row = impeachRec->FindString(GUILD_IMPEACH_MEMBER_UID, pSelfUid);
    if(row == -1)
    {
        CVarList rowData;
        rowData << pSelfUid << pSelfName << GUILD_IMPEACH_RESPOND << 1 << ::time(NULL);
        impeachRec->AddRowValue(-1, rowData);
    }
    else
    {
        int curPollNum = impeachRec->QueryInt(row, GUILD_IMPEACH_MEMBER_NUM);
        impeachRec->SetInt(row, GUILD_IMPEACH_MEMBER_NUM, curPollNum + 1);
    }

    //����Ա��
    IRecord *memberRecord = pData->GetRecord(GUILD_MEMBER_REC);
    if(memberRecord == NULL)
    {
        return false;
    }

    //ͳ����ЧƱ��
    int poll = 0;
    int impeachRecLength = impeachRec->GetRows();
    LoopBeginCheck(d);
    for(int i = 0; i < impeachRecLength; ++i)
    {
        LoopDoCheck(d);
        const char *uid = impeachRec->QueryString(i, GUILD_IMPEACH_MEMBER_UID);
        if(StringUtil::CharIsNull(uid))
        {
            continue;
        }
        if(memberRecord->FindString(GUILD_MEMBER_REC_COL_NAME_UID, uid) != -1)
        {
            poll += impeachRec->QueryInt(i, GUILD_IMPEACH_MEMBER_NUM);
        }
    }

    //�ж��Ƿ�����Ʊ��
    int needPoll = pData->QueryAttrInt(GUILD_IMPEACH_ANSWER_NUM) ;
    if(poll >= needPoll)
    {
        //���㵯��
        m_pInstance->SettleImpeach(pPubKernel, guildDataName.c_str());
    }

    return 0;
}