//--------------------------------------------------------------------
// �ļ���:      DomainCampaign.h
// ��  ��:      �����
// ˵  ��:		
// ��������:    2017��2��13��
// ������:      ������
// �޸���:        
// ��Ȩ����:      
//--------------------------------------------------------------------
#include "DomainCampaign.h"
#include "FsGame/Define/CampaignDefine.h"
#include "utils/record.h"
#include "utils/extend_func.h"
#include "utils/XmlFile.h"
#include "utils/string_util.h"
#include "utils/util_func.h"
#include <algorithm>
#include "FsGame/Define/PubDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "DomainCampaignBase.h"

#define CAMPAIGN_CONFIG "ini/Campaign/CampaignConfig.xml"

// �״̬���
static const char* CHECK_CAMPAIGN_STATE_TIMER = "check_campaign_state_timer";
static const int CHECK_CAMPAIGN_STATE_INTERVAL = 5000; // ���Ƶ��

DomainCampaign * DomainCampaign::m_pDomainCampaign = NULL;

const std::wstring& DomainCampaign::GetDomainName(IPubKernel * pPubKernel)
{
	if (m_domainName.empty())
	{
		wchar_t wstr[256];

		const int server_id = pPubKernel->GetServerId();
		SWPRINTF_S(wstr, L"Domain_Campaign_%d", server_id);
		m_domainName = wstr;
	}

	return m_domainName;
}

bool DomainCampaign::GetSave()
{
	return true;
}

int	DomainCampaign::OnCreate(IPubKernel * pPubKernel, IPubSpace * pPubSpace)
{
	return OnLoad(pPubKernel, pPubSpace);
}

int DomainCampaign::OnLoad(IPubKernel * pPubKernel, IPubSpace * pPubSpace)
{
	m_pDomainCampaign = this;

	const std::wstring & name = GetDomainName(pPubKernel);
	IPubData * pPubData = pPubSpace->GetPubData(name.c_str());

	if (NULL == pPubData)
	{
		return 1;
	}

	// ���������
	CVarList var;
	var << VTYPE_INT << VTYPE_INT << VTYPE_INT << VTYPE_INT64;

	_create_record(pPubKernel, pPubData, PUB_CAMPAIGN_MAIN_REC, MAX_CMR_PUB_ROW_NUM, var, false);

	if (!LoadResource(pPubKernel))
	{
		return false;
	}

	m_vecDomainCampaign.resize(MAX_CAMPAIGN_TYPE_NUM);
	LoopBeginCheck(a);
	for (int i = 0; i < MAX_CAMPAIGN_TYPE_NUM; ++i)
	{
		LoopDoCheck(a);
		DomainCampaignBase* pCampaign = DomainCampaignBase::CreateCampaign((CampaignType)i);
		if (NULL != pCampaign)
		{
			pCampaign->OnLoad(pPubKernel, pPubData);
		}
		m_vecDomainCampaign[i] = pCampaign;
	}

	pPubKernel->AddTimer(CHECK_CAMPAIGN_STATE_TIMER, DomainCampaign::CheckCampaignStateTimer, pPubSpace->GetSpaceName(), GetDomainName(pPubKernel).c_str(), CHECK_CAMPAIGN_STATE_INTERVAL, 0);
	return 1;
}

///  \brief ���յ���������������Ϣ
///  \param source_id ����Ϣ�ķ�������ʶ
///  \param msg ��Ϣ����
int DomainCampaign::OnMessage(IPubKernel * pPubKernel, IPubSpace * pPubSpace, int source_id, int scene_id, const IVarList & msg)
{
	const std::wstring & name = GetDomainName(pPubKernel);
	IPubData * pCampaignData = pPubSpace->GetPubData(name.c_str());
	if (NULL == pCampaignData)
	{
		return 1;
	}

	const int msg_type = msg.IntVal(2);
	switch (msg_type)
	{
	case SP_DOMAIN_MSG_CAMPAIGN_END:
		{
			int nCampaignType = msg.IntVal(3); 
			m_pDomainCampaign->OnCampaignEnd(pPubKernel, pCampaignData, nCampaignType);
		}
		break;
	case SP_DOMAIN_MSG_CAMPAIGN_GM_CHANGE_STATE:
		{
			int nCampaignId = msg.IntVal(3); 
			int nCampaignState = msg.IntVal(4);
			m_pDomainCampaign->OnGMControlCampaignState(pPubKernel, pCampaignData, nCampaignId, nCampaignState);
		}
		break;
	}

	LoopBeginCheck(b);
	for (int i = 0; i < MAX_CAMPAIGN_TYPE_NUM; ++i)
	{
		LoopDoCheck(b);
		DomainCampaignBase* pCampaign = m_vecDomainCampaign[i];
		if (NULL != pCampaign)
		{
			pCampaign->OnMessage(pPubKernel, pCampaignData, source_id, scene_id, msg);
		}
	}

	return 0;
}

// ��ȡ����
bool DomainCampaign::LoadResource(IPubKernel* pPubKernel)
{
	// �����ļ�·��
	std::string pathName = pPubKernel->GetResourcePath();
	pathName += CAMPAIGN_CONFIG;

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning_pub(LOG_ERROR, info.c_str());
		return false;
	}

	m_vecCampaignData.clear();
	const int iSectionCount = (int)xmlfile.GetSectionCount();
	m_vecCampaignData.reserve(iSectionCount);
	LoopBeginCheck(a);
	for (int i = 0; i < iSectionCount; i++)
	{
		LoopDoCheck(a);
		const char* section = xmlfile.GetSectionByIndex(i);

		CampaignData data;
		data.nCampaignId = StringUtil::StringAsInt(section);
		data.nGameType = xmlfile.ReadInteger(section, "GameType", 0);
		const char* strScene = xmlfile.ReadString(section, "SceneID", "");
		ParseXmlSection(data.vecSceneId, strScene);
		const char* strWeek = xmlfile.ReadString(section, "Week", "");
		ParseXmlSection(data.vecWeek, strWeek);
		
		// �״̬���ݽ���
		LoopBeginCheck(b);
		for (int j = 0;;++j)
		{
			LoopDoCheck(b);
			char strState[32] = { 0 };
			SPRINTF_S(strState, "CampaignState%d", j+1);

			const char* strData = xmlfile.ReadString(section, strState, "");
			if (StringUtil::CharIsNull(strData))
			{
				break;
			}
			ParseCampaignState(data.vecStateData, strData, j);
		}

		m_vecCampaignData.push_back(data);
	}
	return true;
}

// ���״̬����
int DomainCampaign::CheckCampaignStateTimer(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name, int time)
{
	if (m_pDomainCampaign->m_bGmControl)
	{
		return 0;
	}
	IPubSpace* pPubSpace = pPubKernel->GetPubSpace(space_name);
	if (pPubSpace == NULL)
	{
		return 0;
	}
	IPubData* pPubData = pPubSpace->GetPubData(data_name);
	if (NULL == pPubData)
	{
		return 0;
	}
	IRecord* pCampaignRec = pPubData->GetRecord(PUB_CAMPAIGN_MAIN_REC);
	if (NULL == pCampaignRec)
	{
		return 0;
	}

	int nSize = (int)m_pDomainCampaign->m_vecCampaignData.size();
	LoopBeginCheck(e);
	for (int i = 0; i < nSize;++i)
	{
		LoopDoCheck(e);
		const CampaignData& data = m_pDomainCampaign->m_vecCampaignData[i];
		int64_t nEndTime = 0;
		int nCurState = m_pDomainCampaign->JudgeCampaignState(nEndTime, data);
		int nRowIndex = pCampaignRec->FindInt(CMR_PUB_COL_ACTIVITY_ID, data.nCampaignId);

		bool bSendMsg = false;
		// �δ����
		if (-1 == nRowIndex)
		{
			// �����
			if (nCurState == CAMPAIGN_BEGIN_STATE)
			{
				pCampaignRec->AddRowValue(-1, CVarList() << data.nCampaignId << data.nGameType << nCurState << nEndTime);
				bSendMsg = true;

				extend_warning_pub(LOG_INFO, "DomainCampaign::CheckCampaignStateTimer Campaign Start");
			}
		}
		else
		{
			int nLastState = pCampaignRec->QueryInt(nRowIndex, CMR_PUB_COL_STATE);
			// �״̬�ı�
			if (nLastState != nCurState)
			{
				pCampaignRec->SetInt(nRowIndex, CMR_PUB_COL_STATE, nCurState);
				pCampaignRec->SetInt64(nRowIndex, CMR_PUB_COL_END_TIME, nEndTime);

				bSendMsg = true;
				if (CAMPAIGN_CLOSE_STATE == nCurState)
				{
					pCampaignRec->RemoveRow(nRowIndex);
					extend_warning_pub(LOG_INFO, "DomainCampaign::CheckCampaignStateTimer Campaign Time Over Remove Data");
				}
				extend_warning_pub(LOG_INFO, "DomainCampaign::CheckCampaignStateTimer Campaign CurState is %d", nCurState);
			}
		}
		// ���ͻ״̬�ı�����
		if (bSendMsg)
		{
			m_pDomainCampaign->SendStateChangeToCampaignScene(pPubKernel, &data, nCurState);
		}
	}

	LoopBeginCheck(b);
	for (int i = 0; i < MAX_CAMPAIGN_TYPE_NUM; ++i)
	{
		LoopDoCheck(b);
		DomainCampaignBase* pCampaign = m_pDomainCampaign->m_vecDomainCampaign[i];
		if (NULL != pCampaign)
		{
			pCampaign->OnCampaignTimer(pPubKernel, pPubData);
		}
	}

	return 0;
}

// �жϵ�ǰ�Ļ״̬
int DomainCampaign::JudgeCampaignState(int64_t& nEndTime, const CampaignData& camdata)
{
	// �����ܼ�
	int nCurDay = util_get_day_of_week();

	// �ж��Ƿ��ڻ����
	std::vector<int>::const_iterator iter = find(camdata.vecWeek.begin(), camdata.vecWeek.end(), nCurDay);
	if (iter == camdata.vecWeek.end())
	{
		return CAMPAIGN_CLOSE_STATE;
	}

	// ��ʶ��ǰ��״̬
	int state = CAMPAIGN_CLOSE_STATE;
	if (camdata.vecStateData.empty())
	{
		return CAMPAIGN_CLOSE_STATE;
	}

	// ��ǰ��ʱ���
	int nCurTime = util_convert_zerotime_to_sec();

	// ��ǰ�ʱ������
	int nSize = camdata.vecStateData.size();
	LoopBeginCheck(c);
	for (int i = 0; i < nSize;++i)
	{
		LoopDoCheck(c);
		const StateData& data = camdata.vecStateData[i];
		if (nCurTime >= data.nStartTime && nCurTime < data.nEndTime)
		{
			state = data.nState;
			nEndTime = data.nEndTime;
			break;
		}
	}

	return state;
}

// ��ѯ�����
const DomainCampaign::CampaignData* DomainCampaign::QueryCampaignData(int nCampaignId)
{
	CampaignData* pFind = NULL;
	int nSize = m_vecCampaignData.size();
	LoopBeginCheck(q);
	for (int i = 0; i < nSize;++i)
	{
		LoopDoCheck(q);
		if (m_vecCampaignData[i].nCampaignId == nCampaignId)
		{
			pFind = &m_vecCampaignData[i];
			break;
		}
	}

	return pFind;
}

// ��������
bool DomainCampaign::ParseXmlSection(std::vector<int>& vecOutData, const char* strData)
{
	if (StringUtil::CharIsNull(strData))
	{
		return false;
	}
	vecOutData.clear();

	CVarList list;
	StringUtil::SplitString(list, strData, ",");

	int nCount = (int)list.GetCount();
	vecOutData.resize(nCount);
	LoopBeginCheck(a);
	for (int i = 0; i < nCount;++i)
	{
		LoopDoCheck(a);
		vecOutData[i] = list.IntVal(i);
	}
	return true;
}

// �����״̬����
bool DomainCampaign::ParseCampaignState(StateVec& vecOutData, const char* strState, int nState)
{
	if (StringUtil::CharIsNull(strState))
	{
		return false;
	}

	CVarList list;
	StringUtil::SplitString(list, strState, ",");

	// ֻ�п�ʼʱ��ͽ���ʱ��
	int nCount = (int)list.GetCount();
	if (nCount != 2)
	{
		return false;
	}
	
	StateData data;
	data.nState = nState;
	data.nStartTime = util_convert_string_to_sec(list.StringVal(0));
	data.nEndTime = util_convert_string_to_sec(list.StringVal(1));

	vecOutData.push_back(data);
	return true;
}

// ��Ӧ�״̬�ı�
bool DomainCampaign::OnCampaignEnd(IPubKernel* pPubKernel, IPubData* pPubData, int nCampaignType)
{
	IRecord* pCampaignRec = pPubData->GetRecord(PUB_CAMPAIGN_MAIN_REC);
	if (NULL == pCampaignRec)
	{
		return false;
	}

	int nRowIndex = pCampaignRec->FindInt(CMR_PUB_COL_ACTIVITY_TYPE, nCampaignType);
	// �δ���� ���ܹرջ
	if (-1 == nRowIndex)
	{
		return false;
	}

	int nCampaignId = pCampaignRec->QueryInt(nRowIndex, CMR_PUB_COL_ACTIVITY_ID);
	const CampaignData* pData = QueryCampaignData(nCampaignId);
	if (NULL == pData)
	{
		extend_warning_pub(LOG_ERROR, "DomainCampaign::OnCampaignEnd Config error CampaignId=%d", nCampaignId);
		return false;
	}

	//SendStateChangeToCampaignScene(pPubKernel, pData, CAMPAIGN_CLOSE_STATE);
	pCampaignRec->RemoveRow(nRowIndex);

	extend_warning_pub(LOG_INFO, "DomainCampaign::OnCampaignEnd Campaign Over Remove Data");
	return true;
}

// ��Ӧgm������ƻ״̬�ı�
bool DomainCampaign::OnGMControlCampaignState(IPubKernel* pPubKernel, IPubData* pPubData, int nCampaignId, int nCampaignState)
{
	// �Ƿ���gm�������״̬
	m_bGmControl = true;

	IRecord* pCampaignRec = pPubData->GetRecord(PUB_CAMPAIGN_MAIN_REC);
	if (NULL == pCampaignRec)
	{
		return false;
	}

	// gm �������⴦��
	const CampaignData* pData = QueryCampaignData(nCampaignId);
	if (NULL == pData)
	{
		return false;
	}

	int nRowIndex = pCampaignRec->FindInt(CMR_PUB_COL_ACTIVITY_ID, nCampaignId);

	bool bSendMsg = false; 
	if (nRowIndex == -1)
	{
		if (nCampaignState != CAMPAIGN_CLOSE_STATE)
		{
			pCampaignRec->AddRowValue(-1, CVarList() << nCampaignId << pData->nGameType << nCampaignState << (int64_t)0);
		}
		bSendMsg = true;
	}
	else
	{
		int nLastState = pCampaignRec->QueryInt(nRowIndex, CMR_PUB_COL_STATE);
		// �״̬�ı�
		if (nLastState != nCampaignState)
		{
			pCampaignRec->SetInt(nRowIndex, CMR_PUB_COL_STATE, nCampaignState);
			pCampaignRec->SetInt64(nRowIndex, CMR_PUB_COL_END_TIME, 0);

			bSendMsg = true;
			if (CAMPAIGN_CLOSE_STATE == nCampaignState)
			{
				pCampaignRec->RemoveRow(nRowIndex);
			}
		}
	}
	
	// ���ͻ״̬�ı�����
	if (bSendMsg)
	{
		SendStateChangeToCampaignScene(pPubKernel, pData, nCampaignState);
	}

	return true;
}

// ���������ı�״̬��Ϣ
void DomainCampaign::SendStateChangeToCampaignScene(IPubKernel* pPubKernel, const CampaignData* pData, int nCampaignState)
{
	if (NULL == pData)
	{
		return;
	}
	CVarList msg;
	msg << m_pDomainCampaign->GetDomainName(pPubKernel) << PS_DOMAIN_MSG_CAMPAIGN_STATE << nCampaignState << pData->nGameType;
	int nSceneNum = pData->vecSceneId.size();
	msg << nSceneNum;

	LoopBeginCheck(w);
	for (int j = 0; j < nSceneNum; ++j)
	{
		LoopDoCheck(w);
		msg << pData->vecSceneId[j];
	}

	pPubKernel->SendPublicMessage(0, 0, msg);
}