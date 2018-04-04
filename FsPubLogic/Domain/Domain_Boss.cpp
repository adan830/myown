//--------------------------------------------------------------------
// �ļ���:      Domain_Boss.h
// ��  ��:      Boss��¼��
// ˵  ��:		
// ��������:    2018��03��30��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "Domain_Boss.h"
#include "FsGame\Define\FightNpcDefine.h"
#include "utils\record.h"
#include "FsGame\Define\PubDefine.h"	
#include "utils\util_func.h"

Domain_Boss::Domain_Boss() : IDomainData(L"", 0)
{
	
}

const std::wstring & Domain_Boss::GetDomainName(IPubKernel * pPubKernel)
{
	//������
	if (m_domainName.empty())
	{
		wchar_t wstr[256];
		const int server_id = pPubKernel->GetServerId();
		SWPRINTF_S(wstr, L"Domain_Boss_%d", server_id);

		m_domainName = wstr;
	}

	return m_domainName;
}


bool Domain_Boss::GetSave()
{
	return true;
}

int Domain_Boss::OnCreate(IPubKernel * pPubKernel, IPubSpace * pPubSpace)
{
	//������
	const std::wstring & name = GetDomainName(pPubKernel);
	IPubData * pPubData = pPubSpace->GetPubData(name.c_str());

	if (NULL == pPubData)
	{
		return 1;
	}

	//����BOSS��Ϣ��
	CVarList col_types;
	col_types << VTYPE_STRING //boss���
		<< VTYPE_INT64//BOSS����ʱ��
		<< VTYPE_INT	// boss���ڳ���
		<< VTYPE_WIDESTR;// ɱ��boss�����
	_create_record(pPubKernel, pPubData, SCENE_BOSS_TOTAL_REC, 0, col_types, false);

	return 1;
}

int Domain_Boss::OnLoad(IPubKernel * pPubKernel, IPubSpace * pPubSpace)
{
	return OnCreate(pPubKernel, pPubSpace);
}

///  \brief ���յ���������������Ϣ
///  \param source_id ����Ϣ�ķ�������ʶ
///  \param msg ��Ϣ����
int Domain_Boss::OnMessage(IPubKernel * pPubKernel, IPubSpace * pPubSpace, int source_id, int scene_id, const IVarList & msg)
{
	//������
	const std::wstring & name = GetDomainName(pPubKernel);
	IPubData * pPubData = pPubSpace->GetPubData(name.c_str());
	if (NULL == pPubData)
	{
		return 1;
	}

	//������
	const int msg_type = msg.IntVal(2);
	if (msg_type != SP_DOMAIN_MSG_SCENE_BOSS_STATE)
	{
		return 1;
	}

	const char* strBossConfig = msg.StringVal(3);
	const int nState = msg.IntVal(4);
	const int nSceneId = msg.IntVal(5);
	const wchar_t* wsKiller = msg.WideStrVal(6);

	IRecord* pRec = pPubData->GetRecord(SCENE_BOSS_TOTAL_REC);
	if (NULL == pRec)
	{
		return false;
	}
	int nRowIndex = pRec->FindString(SCENE_BOSS_INFO_CONFIG, strBossConfig);
	// ��һ��ˢ��boss
	if (-1 == nRowIndex)
	{
		pRec->AddRowValue(-1, CVarList() << strBossConfig << (int64_t)nState << nSceneId << wsKiller);
	}
	else
	{
		if (BS_BOSS_DEAD == nState)
		{
			pRec->SetInt64(nRowIndex, SCENE_BOSS_DEAD_TIME, ::util_get_utc_time());
			pRec->SetWideStr(nRowIndex, SCENE_BOSS_INFO_KILLER, wsKiller);
		}
		else
		{
			pRec->SetInt64(nRowIndex, SCENE_BOSS_DEAD_TIME, (int64_t)nState);
			pRec->SetWideStr(nRowIndex, SCENE_BOSS_INFO_KILLER, L"");
		}
	}
	return true;
}